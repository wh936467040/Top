#include "iostream"
#include "vector"
#include "util.h"
#include "sendinfo.h"
#include "alarmProc.h"
#include "alarmVector.h"
using namespace std;

extern string nodeID;
///*æŸ¥æ‰¾ä¸€ä¸ªè¿›ç¨‹æ˜¯å¦å·²ç»å‘Šè­¦ */
int alarmVector::findAlarmFromAlarmSet (int alarmType, int pid ,string procName)
{
	vector<alarmProc*> * set;
	if(alarmType == CPUALARM)
	{
		set = & this -> cpuAlarmSet ;
	}
	else if (alarmType == MEMALARM)
	{
		set = & this -> memAlarmSet;
	}
	else if (alarmType == CONNALARM)
	{
		set = & this ->connAlarmSet;
	}
	
	for(vector<alarmProc*>::iterator it = set->begin(); it != set->end(); ++it)
	{
		if((*it)->pid == pid  &&  (*it)->procName == procName)
		{
			(*it) ->isExit = 1;
			return 1;
		}
	}
	return 0;
}


/*åˆ é™¤é˜Ÿåˆ—ä¸­çš„å‘Šè­¦ */
int alarmVector::deleteAlarmFromAlarmSet(int alarmType ,int pid ,string procName)
{
	vector<alarmProc*> * set;
	if(alarmType == CPUALARM)
	{
		set = & this -> cpuAlarmSet ;
	}
	else if (alarmType == MEMALARM)
	{
		set = & this -> memAlarmSet;
	}
	else if (alarmType == CONNALARM)
	{
		set = & this ->connAlarmSet;
	}	
	//×¢Òâ£º it++²»ÄÜ·ÅÔÚforµÄµÚÒ»ĞĞ£¬ÒòÎªÓĞeraseº¯Êı
	for(vector<alarmProc*>::iterator it = set->begin(); it != set->end();1)
	{   
		if((*it)->pid == pid  &&   (*it)->procName == procName)
		{
			delete (*it);
			*it=NULL;
			set->erase(it);             
			break;
		}
		else
		{
			it++;
		}
		
	}
	return 1;
}

/* æ’å…¥ä¸€æ¡å‘Šè­¦*/
int alarmVector::insertAlarmIntoAlarmSet(int alarmType, int pid ,string procName,string start,string data)
{
	if(findAlarmFromAlarmSet(alarmType,pid,procName)!=0)
	{
		return 0;
	}
	else
	{	
		vector<alarmProc*> * set;
		if(alarmType == CPUALARM)
		{
			set = & (this -> cpuAlarmSet) ;
		}
		else if (alarmType == MEMALARM)
		{
			set = & (this -> memAlarmSet);
		}
		else if (alarmType == CONNALARM)
		{
			set = & (this ->connAlarmSet);
		}
		
		alarmProc * it=new alarmProc(alarmType,pid,procName,start,data);
		set->push_back(it);
		return 1;
	}
}

int alarmVector::searchNotExitProc()
{
	for(vector<alarmProc*>::iterator it = cpuAlarmSet.begin(); it != cpuAlarmSet.end(); ++it)
	{   
		if((*it)->isExit==0)
		{
			cout<<"½ø³Ì²»´æÔÚ£¬È¡ÏûCPU¸æ¾¯ "<<(*it)->pid<<"  "<<(*it)->procName<<endl;
			deleteAlarmFromAlarmSet(CPUALARM ,(*it)->pid ,(*it)->procName);
		}
	}
	for(vector<alarmProc*>::iterator it = memAlarmSet.begin(); it != memAlarmSet.end(); 1)					
	{   
		if((*it)->isExit==0)
		{
			cout<<"½ø³Ì²»´æÔÚ£¬È¡ÏûMEM¸æ¾¯ "<<(*it)->pid<<"  "<<(*it)->procName<<endl;
			deleteAlarmFromAlarmSet(MEMALARM ,(*it)->pid ,(*it)->procName);
		}
		else
		{
			it++;
		}
	}
	for(vector<alarmProc*>::iterator it = connAlarmSet.begin(); it != connAlarmSet.end();1)
	{
		if((*it)->isExit==0)
		{
			cout<<"½ø³Ì²»´æÔÚ£¬È¡ÏûCONN¸æ¾¯ "<<(*it)->pid<<"  "<<(*it)->procName<<endl;
			deleteAlarmFromAlarmSet(CONNALARM ,(*it)->pid ,(*it)->procName);
		}
		else
		{
			it++;
		}
	}
}


int alarmVector::refreshProc()
{
	for(vector<alarmProc*>::iterator it = cpuAlarmSet.begin(); it != cpuAlarmSet.end(); ++it)
	{
		(*it)->isExit=0;	
	}
	for(vector<alarmProc*>::iterator it = memAlarmSet.begin(); it != memAlarmSet.end(); ++it)                                              
	{
		(*it)->isExit=0;
	}
	for(vector<alarmProc*>::iterator it = connAlarmSet.begin(); it != connAlarmSet.end(); ++it)
	{
		(*it)->isExit=0;
	}
}


int alarmVector::findAndSendMemAlarm(vector<myProc*> procSet,string timeNow)
{
	for(vector<myProc*>::iterator it = procSet.begin(); it != procSet.end(); ++it)
	{
		if((*it)->memUsed >100)
		{
			if(findAlarmFromAlarmSet(MEMALARM,(*it)->pid,(*it)->name))                                                        
			{
				cout<<"alarm already exit,do nothing"<<endl;
			}
			else
			{
				string alarmDetail = "½ø³ÌÄÚ´æÕ¼ÓÃ¹ı¸ß ½ø³ÌºÅ"+itos((*it)->pid)+string((*it)->name)+" Õ¼ÓÃÄÚ´æ"+itos((*it)->memUsed)+"M";
				insertAlarmIntoAlarmSet(MEMALARM,(*it)->pid,(*it)->name,timeNow,alarmDetail);
				cout<<alarmDetail<<endl;
				struct ALARM_INFO_D5000 alarmInfo;
				alarmInfo.itemid="000200050";                                                                                                   
				alarmInfo.data=alarmDetail;
				//alarmInfo.level="4";
				//sendAlarm.sendD5000AlarmInfo(nodeID, timeNow,alarmInfo);
			}
		}
		else
		{
			if(findAlarmFromAlarmSet(MEMALARM,(*it)->pid,(*it)->name))
			{
				cout<<"alarm cancel"<<(*it)->pid<<"  "<<(*it)->name<<endl;
				deleteAlarmFromAlarmSet(MEMALARM,(*it)->pid,(*it)->name);
				cout<<"sendDisAlarm"<<endl;
				//sendAlarm.sendD5000DisAlarmInfo(nodeID,"000200050",(*it)->start,timeNow,(*it)->data);
			}
			else
			{
				//do nothing
			}
		}
	}

	searchNotExitProc();
	refreshProc();
}

