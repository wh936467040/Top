#include <ctype.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <iostream>
#include <myProc.h>
#include <sstream>
#include "getCpuCoreNum.h"
#include "getMemTotal.h"
#include "sendinfo.h"
#include "alarmVector.h"
#include "util.h"
using namespace std;
SendInfo s_sendinfo;
SendInfo sendAlarm;
string nodeID = "0";
TOP3_INFO top3_info;
int procNumAlarmMark = 0;
int procNumAlarmThold = 0;
string procNumAlarmData = "";
string procNumAlarmStartTime = "";
struct cpu_info {
    long unsigned utime, ntime, stime, itime;
    long unsigned iowtime, irqtime, sirqtime;
};

#define PROC_NAME_LEN 64
#define THREAD_NAME_LEN 32
#define DB_NAME "FJ_MONITOR"
struct proc_info {
    struct proc_info *next;
    pid_t pid;
    pid_t tid;
    uid_t uid;
    gid_t gid;
    char name[PROC_NAME_LEN];
    char tname[THREAD_NAME_LEN];
    char state;
    long unsigned utime;
    long unsigned stime;
    long unsigned delta_utime;
    long unsigned delta_stime;
    long unsigned delta_time;
    long vss;
    long rss;
	long v_write;
	long v_read;
	long connect_count;
    int num_threads;
    char policy[32];
	
	long total_write;
	long total_read;
};

struct proc_list {
    struct proc_info **array;
    int size;
};

#define die(...) { fprintf(stderr, __VA_ARGS__); exit(EXIT_FAILURE); }

#define INIT_PROCS 50
#define THREAD_MULT 8

void VectorClear(vector<myProc*> & procSet);
static int sleep_time;
static int cpuCoreNum;
long memTotalKB;
static struct proc_info **old_procs, **new_procs;
static int num_old_procs, num_new_procs;
static struct proc_info *free_procs;
static int num_used_procs, num_free_procs;
static int max_procs, delay, iterations, threads;
static struct cpu_info old_cpu, new_cpu;

static time_t newStatTime=0;
static time_t oldStatTime=0;

static struct proc_info *alloc_proc(void);
static void free_proc(struct proc_info *proc);
static void read_procs(void);
static int read_stat(char *filename, struct proc_info *proc);
static void read_policy(int pid, struct proc_info *proc);
static void add_proc(int proc_num, struct proc_info *proc);
static int read_cmdline(char *filename, struct proc_info *proc);
static int read_status(char *filename, struct proc_info *proc);
static void print_procs(vector<myProc*> & procSet);
static struct proc_info *find_old_proc(pid_t pid, pid_t tid);
static void free_old_procs(void);
static int (*proc_cmp)(const void *a, const void *b);
static int proc_cpu_cmp(const void *a, const void *b);
static int proc_vss_cmp(const void *a, const void *b);
static int proc_rss_cmp(const void *a, const void *b);
static int proc_thr_cmp(const void *a, const void *b);
static int numcmp(long long a, long long b);
static void usage(char *cmd);
static int read_socketConnect(int pid ,struct proc_info * proc);
static int read_io(int pid ,struct proc_info * proc);
static int getIO(vector<myProc*> & oldProc,vector<myProc*> & newProc);
int getConfInfo();
int getAlarmConfInfo();
int getMaxProcNum();
int main(int argc, char *argv[]) {
	string dbname = string(DB_NAME);
	getConfInfo();
	getAlarmConfInfo();
	cpuCoreNum=getCpuCoreNum();
	memTotalKB=getMemTotal();
	cout<<"mem size "<<memTotalKB<<endl;
	num_used_procs = num_free_procs = 0;

	alarmVector alarmSet;
	max_procs = 0;
	delay = 3;
	iterations = -1;
	proc_cmp = &proc_cpu_cmp;
	free_procs = NULL;
	num_new_procs = num_old_procs = 0;
	new_procs = old_procs = NULL;

	vector<myProc* > procSet;
	vector<myProc* > oldProc;
	vector<myProc* > newProc;

	old_procs = new_procs;
	num_old_procs = num_new_procs;
	memcpy(&old_cpu, &new_cpu, sizeof(old_cpu));
	read_procs();
	int init=1;

	char time_now[30];
	getSysTime(time_now,30);

	int maxProcNum = getMaxProcNum();
	long alarmProcNum = procNumAlarmThold /100.0 * maxProcNum;
	cout << "proc num alarm thold = " << alarmProcNum << endl;
	char sql[256] = "";
	sprintf(sql,"sql: update %s.SYSDBA.SERVER set max_proc_num = %d where id = %s",dbname.c_str(),maxProcNum,nodeID.c_str());
	vector<string > vec_sql ;
	vec_sql.push_back(sql);
	s_sendinfo.sendAllInfo("50000",nodeID,string(time_now),vec_sql);
	sendAlarm.sendDStartAlarmInfo(nodeID,"00020071",string(time_now));
	while ((iterations == -1) || (iterations-- > 0)) 
	{
		vector<string> vecWrite;
		vector<string> vecRead;
		vector<string> vecSocket;
		newStatTime=time(NULL);
		getSysTime(time_now,30);
		string time = time_now;

		old_procs = new_procs;
		num_old_procs = num_new_procs;
		memcpy(&old_cpu, &new_cpu, sizeof(old_cpu));
		read_procs();
		vector<myProc* > procSet;
		print_procs(newProc);
		int count=0;
		if(init)
		{
			oldStatTime=newStatTime;
			init=0;
			sleep(1);
			myProc::VectorCopy(oldProc,newProc);
			myProc::VectorClear(newProc);
			free_old_procs();
			continue;
		}

		int totalProcNum = newProc.size();
		vector<string> vec_proc_num;
		string str_proc_num = "proc_num:";
		str_proc_num = str_proc_num + itos(totalProcNum);
		vec_proc_num.push_back(str_proc_num);
		cout << "proc num " << str_proc_num << endl;
		s_sendinfo.sendAllInfo("10062",nodeID,time,vec_proc_num);

		getIO(oldProc,newProc);
		sort(newProc.begin(),newProc.end(),myProc::writeSpeedCmp);  
		string pidStr;
		string writeSpeedStr;
		string processNameStr;
		for(vector<myProc*>::iterator it = newProc.begin(); it != newProc.end(); ++it)
		{
			if(count==0)
			{
				pidStr="value:"+itos((*it)->pid);
				writeSpeedStr="value:"+dtos((*it)->v_write);
				processNameStr="value:"+((*it)->name);
			}
			else if(count <=9)
			{
				pidStr=pidStr+";"+itos((*it)->pid);
				writeSpeedStr=writeSpeedStr+";"+dtos((*it)->v_write);
				processNameStr=processNameStr+";"+((*it)->name);
			}
			//cout<<"***"<<(*it)->pid<<" "<<"mem:"<<(*it)->memUsed<<" cpu:"<<(*it)->cpuUseRatio<<" conn:"<<(*it)->connectCount<<" read:"<<(*it)->v_read<<" write:"<<(*it)->v_write<<(*it)->name<<endl;
			count++;
			if(count>9)
			{
				vecWrite.push_back(pidStr);
				vecWrite.push_back(writeSpeedStr);
				vecWrite.push_back(processNameStr);
				cout<<"send top_write"<<endl;
				s_sendinfo.sendAllInfo("10034",nodeID,time,vecWrite);
				break;
			}
		}

		sort(newProc.begin(),newProc.end(),myProc::readSpeedCmp);  
		pidStr="";
		string readSpeedStr="";
		processNameStr="";
		count =0;
		for(vector<myProc*>::iterator it = newProc.begin(); it != newProc.end(); ++it)
		{
			if(count==0)
			{
				pidStr="value:"+itos((*it)->pid);
				readSpeedStr="value:"+dtos((*it)->v_read);
				processNameStr="value:"+((*it)->name);
			}
			else if(count <=9)
			{
				pidStr=pidStr+";"+itos((*it)->pid);
				readSpeedStr=readSpeedStr+";"+dtos((*it)->v_read);
				processNameStr=processNameStr+";"+((*it)->name);
			}
			//cout<<"***"<<(*it)->pid<<" "<<"mem:"<<(*it)->memUsed<<" cpu:"<<(*it)->cpuUseRatio<<" conn:"<<(*it)->connectCount<<" read:"<<(*it)->v_read<<" write:"<<(*it)->v_write<<(*it)->name<<endl;
			count++;
			if(count>9)
			{
				vecRead.push_back(pidStr);
				vecRead.push_back(readSpeedStr);
				vecRead.push_back(processNameStr);
				cout<<"send top_read"<<endl;
				s_sendinfo.sendAllInfo("10032",nodeID,time,vecRead);
				break;
			}
		}

		sort(newProc.begin(),newProc.end(),myProc::socketCountCmp);
		string socketCountStr="";
		count =0;
		for(vector<myProc*>::iterator it = newProc.begin(); it != newProc.end(); ++it)
		{
			if(count==0)
			{
				pidStr="value:"+itos((*it)->pid);
				socketCountStr="value:"+itos((*it)->connectCount);
				processNameStr="value:"+((*it)->name);
			}
			else if(count <=9)
			{
				pidStr=pidStr+";"+itos((*it)->pid);
				socketCountStr=socketCountStr+";"+itos((*it)->connectCount);
				processNameStr=processNameStr+";"+((*it)->name);
			}
			//cout<<"***"<<(*it)->pid<<" "<<"mem:"<<(*it)->memUsed<<" cpu:"<<(*it)->cpuUseRatio<<" conn:"<<(*it)->connectCount<<" read:"<<(*it)->v_read<<" write:"<<(*it)->v_write<<(*it)->name<<endl;
			count++;
			if(count>9)
			{
				vecSocket.push_back(pidStr);
				vecSocket.push_back(socketCountStr);
				vecSocket.push_back(processNameStr);
				cout<<"send top_socket_count"<<endl;
				s_sendinfo.sendAllInfo("10033",nodeID,time,vecSocket);
				break;
			}
		}

		count=0;
		//send cpu top3
		//   cout<<"cpu top 3"<<endl;
		sort(newProc.begin(),newProc.end(),myProc::cpuUseRatioCmp);  
		sleep(3);
		for(vector<myProc*>::iterator it = newProc.begin(); it != newProc.end(); ++it)
		{
			if(count == 0)
			{
				top3_info.pid = itos((*it)->pid) ;
				top3_info.top_cpu = itos((*it)->cpuUseRatio);
				top3_info.top_mem = lltos((*it)->memUsed) ;
				top3_info.command = (*it)->name ;
			}
			else
			{
				top3_info.pid += ";" + itos((*it)->pid) ;
				top3_info.top_cpu += ";" + itos((*it)->cpuUseRatio) ;
				top3_info.top_mem += ";" + lltos((*it)->memUsed) ;
				top3_info.command += ";" + (*it)->name ;
			}
			//cout<<(*it)->pid<<" "<<"mem:"<<(*it)->memUsed<<" cpu:"<<(*it)->cpuUseRatio<<" conn:"<<(*it)->connectCount<<" read:"<<(*it)->readBytes<<" write:"<<(*it)->writeBytes<<(*it)->name<<endl;
			count++;
			if(count>9)
			{
				cout<<"send cpu"<<endl;
				cout<<s_sendinfo.m_port_main<<endl;
				cout<<s_sendinfo.m_ip_main<<endl;
				s_sendinfo.sendTopCpuInfo(nodeID,time,top3_info);
				break;
			}
		}

		// 	cout<<"mem top3"<<endl;
		count = 0;
		sort(newProc.begin(),newProc.end(),myProc::memUsedCmp);
		for(vector<myProc*>::iterator it = newProc.begin(); it != newProc.end(); ++it)
		{
			if(count ==0)
			{
				top3_info.pid = itos((*it)->pid) ;
				top3_info.top_cpu = itos((*it)->cpuUseRatio);
				top3_info.top_mem = lltos((*it)->memUsed) ;
				top3_info.command = (*it)->name ;
			}
			else
			{
				top3_info.pid += ";" + itos((*it)->pid) ;
				top3_info.top_cpu += ";" + itos((*it)->cpuUseRatio) ;
				top3_info.top_mem += ";" + lltos((*it)->memUsed) ;
				top3_info.command += ";" + (*it)->name ;
			}

			//		cout<<"MEM"<<(*it)->pid<<" pid "<<(*it)->memUsed<<" mem "<<(*it)->cpuUseRatio<<" cpu"<<(*it)->name<<endl;
			count++;
			if(count>9)
			{
				cout<<"send mem"<<endl;
				s_sendinfo.sendTopMemInfo(nodeID,time,top3_info);
				break;
			}
		}
		//alarmSet.findAndSendMemAlarm(newProc,time);

		if(totalProcNum > alarmProcNum && procNumAlarmMark <=0)
		{
			procNumAlarmMark =1;
			procNumAlarmData = "进程数过高,当前值" + ltos(totalProcNum);
			struct ALARM_INFO_D5000 alarmInfo;
			alarmInfo.itemid = "00020071";
			alarmInfo.data = "";
			alarmInfo.data = alarmInfo.data + procNumAlarmData;
			procNumAlarmStartTime = time;
			sendAlarm.sendD5000AlarmInfo(nodeID,time,alarmInfo);
		}	
		else if(totalProcNum < alarmProcNum && procNumAlarmMark >0)
		{
			procNumAlarmMark =0;
			sendAlarm.sendD5000DisAlarmInfo(nodeID,"00020071",procNumAlarmStartTime,time,procNumAlarmData);
			procNumAlarmStartTime = "";
		}

		myProc::VectorClear(oldProc);
		free_old_procs();
		myProc::VectorCopy(oldProc,newProc);
		myProc::VectorClear(newProc);
		oldStatTime=newStatTime;
	}
	return 0;
}

static int getIO(vector<myProc*> & oldProc,vector<myProc*> & newProc)
{
	for(vector<myProc*>::iterator itNew = newProc.begin(); itNew != newProc.end(); ++itNew)
	{
		for(vector<myProc*>::iterator itOld = oldProc.begin(); itOld != oldProc.end(); ++itOld)
		{
			if((*itNew)->pid == (*itOld)->pid && (*itNew)->name==(*itOld)->name)
			{
				long oldRead=(*itOld)->total_read;
				long oldWrite=(*itOld)->total_write;
				long newRead=(*itNew)->total_read;
				long newWrite=(*itNew)->total_write;

				(*itNew)->v_read=(newRead-oldRead)/1024.0/(newStatTime-oldStatTime);
				(*itNew)->v_write=(newWrite-oldWrite)/1024.0/(newStatTime-oldStatTime);
				if((*itNew)->v_read>1000000000000000||(*itNew)->v_write>1000000000000000)
				{
					cout<<"1111111111111111111111111111"<<endl;
					cout<<"pid "<<(*itNew)->pid<<"newRead "<<newRead<<"  oldRead "<<oldRead<<endl;
					cout<<"pid "<<(*itNew)->pid<<"newWrtie "<<newWrite<<"  oldWrite: "<<oldWrite<<endl;
					cout<<"pid: "<<(*itNew)->pid<<" read : "<<(*itNew)->v_read<<" write: "<<(*itNew)->v_write<<endl;
					cout<<"2222222222222222222222222222"<<endl;
				}
				break;
			}
		}      
	}          
}

static struct proc_info *alloc_proc(void) {
	struct proc_info *proc;
	if (free_procs) {
		proc = free_procs;
		free_procs = free_procs->next;
		num_free_procs--;
	} else {
		proc = (proc_info *)malloc(sizeof(*proc));
		if (!proc) die("Could not allocate struct process_info.\n");
	}

	num_used_procs++;

	return proc;
}

static void free_proc(struct proc_info *proc) {
	proc->next = free_procs;
	free_procs = proc;

	num_used_procs--;
	num_free_procs++;
}

#define MAX_LINE 256

static void read_procs(void) {
	DIR *proc_dir, *task_dir;
	struct dirent *pid_dir, *tid_dir;
	char filename[64];
	FILE *file;
	int proc_num;
	struct proc_info *proc;
	pid_t pid, tid;

	int i;

	proc_dir = opendir("/proc");
	if (!proc_dir) die("Could not open /proc.\n");

	new_procs =(proc_info **) calloc(INIT_PROCS * (threads ? THREAD_MULT : 1), sizeof(struct proc_info *));
	num_new_procs = INIT_PROCS * (threads ? THREAD_MULT : 1);

	file = fopen("/proc/stat", "r");
	if (!file) die("Could not open /proc/stat.\n");
	fscanf(file, "cpu  %lu %lu %lu %lu %lu %lu %lu", &new_cpu.utime, &new_cpu.ntime, &new_cpu.stime,
			&new_cpu.itime, &new_cpu.iowtime, &new_cpu.irqtime, &new_cpu.sirqtime);
	fclose(file);

	proc_num = 0;
	while ((pid_dir = readdir(proc_dir))) {
		if (!isdigit(pid_dir->d_name[0]))
			continue;

		pid = atoi(pid_dir->d_name);

		struct proc_info cur_proc;

		if (!threads) {
			proc = alloc_proc();

			proc->pid = proc->tid = pid;

			sprintf(filename, "/proc/%d/stat", pid);
			read_stat(filename, proc);

			sprintf(filename, "/proc/%d/cmdline", pid);
			read_cmdline(filename, proc);

			sprintf(filename, "/proc/%d/status", pid);
			read_status(filename, proc);
			read_io(pid,proc);
			read_socketConnect(pid ,proc); 
			read_policy(pid, proc);

			proc->num_threads = 0;
		} else {
			sprintf(filename, "/proc/%d/cmdline", pid);
			read_cmdline(filename, &cur_proc);

			sprintf(filename, "/proc/%d/status", pid);
			read_status(filename, &cur_proc);

			proc = NULL;
		}

		sprintf(filename, "/proc/%d/task", pid);
		task_dir = opendir(filename);
		if (!task_dir) continue;

		while ((tid_dir = readdir(task_dir))) {
			if (!isdigit(tid_dir->d_name[0]))
				continue;

			if (threads) {
				tid = atoi(tid_dir->d_name);

				proc = alloc_proc();

				proc->pid = pid; proc->tid = tid;

				sprintf(filename, "/proc/%d/task/%d/stat", pid, tid);
				read_stat(filename, proc);

				read_policy(tid, proc);

				strcpy(proc->name, cur_proc.name);
				proc->uid = cur_proc.uid;
				proc->gid = cur_proc.gid;

				add_proc(proc_num++, proc);
			} else {
				proc->num_threads++;
			}
		}

		closedir(task_dir);

		if (!threads)
			add_proc(proc_num++, proc);
	}

	for (i = proc_num; i < num_new_procs; i++)
		new_procs[i] = NULL;

	closedir(proc_dir);
}

static int read_stat(char *filename, struct proc_info *proc) {
	FILE *file;
	char buf[MAX_LINE], *open_paren, *close_paren;
	int res, idx;

	file = fopen(filename, "r");
	if (!file) return 1;
	fgets(buf, MAX_LINE, file);
	fclose(file);

	/* Split at first '(' and last ')' to get process name. */
	open_paren = strchr(buf, '(');
	close_paren = strrchr(buf, ')');
	if (!open_paren || !close_paren) return 1;

	*open_paren = *close_paren = '\0';
	strncpy(proc->tname, open_paren + 1, THREAD_NAME_LEN);
	proc->tname[THREAD_NAME_LEN-1] = 0;

	/* Scan rest of string. */
	sscanf(close_paren + 1, " %c %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d "
			"%lu %lu %*d %*d %*d %*d %*d %*d %*d %lu %ld",
			&proc->state, &proc->utime, &proc->stime, &proc->vss, &proc->rss);
	return 0;
}


int read_io(int pid ,struct proc_info * proc) 
{

	int result=0;
	char filename[256]="";
	sprintf(filename,"/proc/%d/io",pid);
	FILE *file;
	char line[MAX_LINE];
	line[0] = '\0';
	file = fopen(filename, "r");
	if (!file)
	{	
		proc->total_write=0;
		proc->total_read=0;
		return 0;
	}
	while(fgets(line, MAX_LINE, file)!=NULL)
	{
		if(strstr(line,"read_bytes: ")!=NULL)
		{
			string tmp=line;
			tmp=tmp.substr(12,tmp.length());
			proc->total_read=stol(tmp);
		} 
		else if(strstr(line,"write_bytes: ")!=NULL&&strstr(line,"cancelled_write_bytes: ")==NULL)
		{
			string tmp=line;
			tmp=tmp.substr(13,tmp.length());
			proc->total_write=stol(tmp);
		}  
	}
	fclose(file);
	return 1;
}
static int read_socketConnect(int pid ,struct proc_info * proc) 
{
	string pidStr=itos(pid);
	int result=0;
	string cmd ="ls /proc/"+pidStr+"/fd/ -l|grep socket|wc -l";
	//cout<<cmd<<endl;
	FILE *file;
	char line[MAX_LINE];
	line[0] = '\0';
	file = popen(cmd.c_str(), "r");
	if (!file) 
	{
		proc->connect_count=0;
		return 0;
	}
	fgets(line, MAX_LINE, file);
	pclose(file);
	if (strlen(line) > 0)
	{
		result=atoi(line);
		proc->connect_count=result;
	} else
	{
		result =0;
		proc->connect_count = 0;
	}
	return result;
}
static void add_proc(int proc_num, struct proc_info *proc) {
	int i;

	if (proc_num >= num_new_procs) {
		new_procs =(proc_info **) realloc(new_procs, 2 * num_new_procs * sizeof(struct proc_info *));
		if (!new_procs) die("Could not expand procs array.\n");
		for (i = num_new_procs; i < 2 * num_new_procs; i++)
			new_procs[i] = NULL;
		num_new_procs = 2 * num_new_procs;
	}
	new_procs[proc_num] = proc;
}

static int read_cmdline(char *filename, struct proc_info *proc) {
	FILE *file;
	char line[MAX_LINE];

	line[0] = '\0';
	file = fopen(filename, "r");
	if (!file) return 1;
	fgets(line, MAX_LINE, file);
	fclose(file);
	if (strlen(line) > 0) {
		strncpy(proc->name, line, PROC_NAME_LEN);
		proc->name[PROC_NAME_LEN-1] = 0;
	} else
		proc->name[0] = 0;
	return 0;
}

static void read_policy(int pid, struct proc_info *proc) {
	/**
	  SchedPolicy p;
	  if (get_sched_policy(pid, &p) < 0)
	  strcpy(proc->policy, "unk");
	  else {
	  if (p == SP_BACKGROUND)
	  strcpy(proc->policy, "bg");
	  else if (p == SP_FOREGROUND)
	  strcpy(proc->policy, "fg");
	  else
	  strcpy(proc->policy, "er");
	  }*/
}

static int read_status(char *filename, struct proc_info *proc) {
	FILE *file;
	char line[MAX_LINE];
	unsigned int uid, gid;

	file = fopen(filename, "r");
	if (!file) return 1;
	while (fgets(line, MAX_LINE, file)) {
		sscanf(line, "Uid: %u", &uid);
		sscanf(line, "Gid: %u", &gid);
	}
	fclose(file);
	proc->uid = uid; proc->gid = gid;
	return 0;
}

static void print_procs(vector<myProc *> & procSet) {
	int i;
	struct proc_info *old_proc, *proc;
	long unsigned total_delta_time;
	struct passwd *user;
	struct group *group;
	char *user_str, user_buf[20];
	char *group_str, group_buf[20];

	for (i = 0; i < num_new_procs; i++) {
		if (new_procs[i]) {
			old_proc = find_old_proc(new_procs[i]->pid, new_procs[i]->tid);
			if (old_proc) {
				new_procs[i]->delta_utime = new_procs[i]->utime - old_proc->utime;
				new_procs[i]->delta_stime = new_procs[i]->stime - old_proc->stime;
			} else {
				new_procs[i]->delta_utime = 0;
				new_procs[i]->delta_stime = 0;
			}
			new_procs[i]->delta_time = new_procs[i]->delta_utime + new_procs[i]->delta_stime;
		}
	}

	total_delta_time = (new_cpu.utime + new_cpu.ntime + new_cpu.stime + new_cpu.itime
			+ new_cpu.iowtime + new_cpu.irqtime + new_cpu.sirqtime)
		- (old_cpu.utime + old_cpu.ntime + old_cpu.stime + old_cpu.itime
				+ old_cpu.iowtime + old_cpu.irqtime + old_cpu.sirqtime);

	qsort(new_procs, num_new_procs, sizeof(struct proc_info *), proc_cmp);
	if (!threads) 

		//   printf("%5s %4s %1s %5s %7s %7s %3s %-8s %s\n", "PID", "CPU%", "S", "#THR", "VSS", "RSS", "PCY", "UID", "Name");
		for (i = 0; i < num_new_procs; i++) {
			proc = new_procs[i];

			if (!proc || (max_procs && (i >= max_procs)))
				break;
			user  = getpwuid(proc->uid);
			group = getgrgid(proc->gid);
			if (user && user->pw_name) {
				user_str = user->pw_name;
			} else {
				snprintf(user_buf, 20, "%d", proc->uid);
				user_str = user_buf;
			}
			if (group && group->gr_name) {
				group_str = group->gr_name;
			} else {
				snprintf(group_buf, 20, "%d", proc->gid);
				group_str = group_buf;
			}
			if (!threads)
			{	
				//            printf("%5d %ld%% %c %5d %6ldK %6ldK %3s %-8.8s %s\n", proc->pid, 16*proc->delta_time * 100 / total_delta_time, proc->state, proc->num_threads,
				//                proc->vss / 1024, proc->rss * getpagesize() / 1024, proc->policy, user_str, proc->name[0] != 0 ? proc->name : proc->tname);
				char  procName[1024]="";
				sprintf(procName,"%s  %s",user_str, proc->name[0] != 0 ? proc->name : proc->tname);
				if(memTotalKB>1024)//mem size>1024kb
				{

					myProc * procMetric =new myProc(proc->pid,cpuCoreNum*proc->delta_time * 100/ total_delta_time,proc->rss * getpagesize(),proc->connect_count,proc->total_write,proc->total_read,procName);
					procSet.push_back(procMetric);
				}
			}
		}
}

static struct proc_info *find_old_proc(pid_t pid, pid_t tid) {
	int i;

	for (i = 0; i < num_old_procs; i++)
		if (old_procs[i] && (old_procs[i]->pid == pid) && (old_procs[i]->tid == tid))
			return old_procs[i];

	return NULL;
}

static void free_old_procs(void) {
	int i;

	for (i = 0; i < num_old_procs; i++)
		if (old_procs[i])
			free_proc(old_procs[i]);

	free(old_procs);
}

static int proc_cpu_cmp(const void *a, const void *b) {
	struct proc_info *pa, *pb;

	pa = *((struct proc_info **)a); pb = *((struct proc_info **)b);

	if (!pa && !pb) return 0;
	if (!pa) return 1;
	if (!pb) return -1;

	return -numcmp(pa->delta_time, pb->delta_time);
}

static int proc_vss_cmp(const void *a, const void *b) {
	struct proc_info *pa, *pb;

	pa = *((struct proc_info **)a); pb = *((struct proc_info **)b);

	if (!pa && !pb) return 0;
	if (!pa) return 1;
	if (!pb) return -1;

	return -numcmp(pa->vss, pb->vss);
}

static int proc_rss_cmp(const void *a, const void *b) {
	struct proc_info *pa, *pb;

	pa = *((struct proc_info **)a); pb = *((struct proc_info **)b);

	if (!pa && !pb) return 0;
	if (!pa) return 1;
	if (!pb) return -1;

	return -numcmp(pa->rss, pb->rss);
}

static int proc_thr_cmp(const void *a, const void *b) {
	struct proc_info *pa, *pb;

	pa = *((struct proc_info **)a); pb = *((struct proc_info **)b);

	if (!pa && !pb) return 0;
	if (!pa) return 1;
	if (!pb) return -1;

	return -numcmp(pa->num_threads, pb->num_threads);
}

static int numcmp(long long a, long long b) {
	if (a < b) return -1;
	if (a > b) return 1;
	return 0;
}

int getConfInfo()
{
	char *HOME = getenv("$(D5000_HOME)");
	char path[128] = "";
	sprintf(path, "%s/%s", getenv("D5000_HOME"), "conf/auto_monitor.conf");
	cout<<path<<endl;
	FILE *pp;
	pp = fopen(path,"r");
	if(NULL == pp)
	{
		cout<<"conf file open failed,check you path"<<endl;
		return -1;
	}

	string str_code;
	string str_content;
	char buf[128];
	string str = "";
	int index = 0;
	int flag_process =0 ;

	while(fgets(buf, 128, pp) != NULL)
	{
		str = buf;
		str = str.substr(0,str.length()-1);
		index = str.find_first_of(":", 0);
		if(index > 0)
		{
			str_code = str.substr(0, index);
			str_content = str.substr(index+1, str.length());
			//	cout << str_code << endl;
			if(str_code ==  "id" || str_code == "ID")
			{
				//idtemp = atoi(str_content.c_str());
				nodeID = str_content.c_str();
			}
			else if(str_code ==  "time" || str_code == "TIME") 
				sleep_time = atoi(str_content.c_str());
			else if(str_code ==  "serverport" || str_code == "SERVERPORT")
				s_sendinfo.m_port_main = atoi(str_content.c_str());
			else if(str_code == "serverip" || str_code == "SERVERIP")
				s_sendinfo.m_ip_main = str_content.c_str();
			else if(str_code == "serverport_bak" || str_code == "SERVERPORT_BAK")
				s_sendinfo.m_port_back = atoi(str_content.c_str());
			else if(str_code ==  "serverip_bak" || str_code == "SERVERIP_BAK")
				s_sendinfo.m_ip_back = str_content.c_str();
			else if(str_code == "alarmport")
				sendAlarm.alarm_port_main = atoi(str_content.c_str());
			else if(str_code == "alarmport_bak")
				sendAlarm.alarm_port_back = atoi(str_content.c_str());
			else if(str_code == "alarmip")
				sendAlarm.alarm_ip_main = str_content;
			else if(str_code == "alarmip_bak")
				sendAlarm.alarm_ip_back = str_content;
		}
	}
	fclose(pp);
	return 0;
}

int getAlarmConfInfo()
{
	char *HOME = getenv("$(D5000_HOME)");
	char path[128] = "";
	sprintf(path, "%s/%s", getenv("D5000_HOME"), "conf/AlarmInfo.conf");
	cout<<path<<endl;
	FILE *pp;
	pp = fopen(path,"r");
	if(NULL == pp)
	{
		cout<<"conf file open failed,check you path"<<endl;
		return -1;
	}
	string str_code;
	string str_content;
	char buf[128];
	string str = "";
	int index = 0;
	int flag_process =0 ;
	while(fgets(buf, 128, pp) != NULL)
	{
		str = buf;
		str = str.substr(0,str.length()-1);
		index = str.find_first_of(":", 0);
		if(index > 0)
		{
			str_code = str.substr(0, index);
			str_content = str.substr(index+1, str.length());
			if(str_code == "00020071")
			{
				procNumAlarmThold = atoi(str_content.c_str());
			}
		}
	}
	fclose(pp);
}

int getMaxProcNum()
{
	string cmd = "cat /proc/sys/kernel/pid_max";
	FILE * pp = popen(cmd.c_str(),"r");
	if(pp == NULL)
	{
		return 32768; // default num;
	}
	char str[1024]="";
	memset(str,0,sizeof(str));
	fgets(str,sizeof(str),pp);
	if(strlen(str)==0)
	{
		return 32768; //default num ;
	}
	int maxNum = atoi(str);
	if(maxNum <= 0)
	{
		return 32768; // default num ;
	}
	return maxNum;

}


