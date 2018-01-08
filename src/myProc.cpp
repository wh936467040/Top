#include "iostream"
#include "myProc.h"
using namespace std;
extern long memTotalKB;
myProc::myProc(int p,double cpu,double mem,int c,long  w,long  r,string procName)
{
	pid=p;
	cpuUseRatio=cpu;
	if(memTotalKB<1024)
	{
		cout<<"get mem size error"<<endl;
		exit(0);
	}
	memUsed=mem/1024.0/1024;
	this->connectCount=c;
	this->total_write=w;
	this->total_read=r;
	this->v_read=0;
	this->v_write=0;
	char tmp[256]="";
	sprintf(tmp,"%.2f",memUsed);
	memUsed=atof(tmp);
	name=procName;
}

bool myProc::cpuUseRatioCmp(myProc*a,myProc* b)
{
	return (a->cpuUseRatio>b->cpuUseRatio);
}
bool myProc::writeSpeedCmp(myProc*a,myProc* b)
{
	return (a->v_write>b->v_write);
} 
bool myProc::readSpeedCmp(myProc*a,myProc* b)
{
	return (a->v_read>b->v_read);
} 

bool myProc::memUsedCmp(myProc*a,myProc* b)
{
	return (a->memUsed>b->memUsed);
}

bool myProc::socketCountCmp(myProc*a,myProc* b)
{
	return (a->connectCount>b->connectCount);
}
/*
bool myProc::readCmp(myProc*a,myProc*b)
{
	return (a->readBytes > b->readBytes);
}

bool myProc::writeCmp(myProc*a,myProc*b)
{
	return (a->writeBytes > b->writeBytes);
}
*/
bool myProc::connectionCmp(myProc*a,myProc*b)
{
	return (a->connectCount > b->connectCount);
}

void myProc::VectorClear(vector<myProc*> & procSet)
{
	for(vector<myProc*>::iterator it = procSet.begin(); it != procSet.end(); ++it)
	{
		if(NULL!= *it)
		{
			delete *it;
			*it =NULL;
		}
	}
	procSet.clear();
	vector<myProc*>(procSet).swap(procSet);
}
int myProc::VectorCopy(vector<myProc*> & to,vector<myProc*> & from)
{
	for(vector<myProc*>::iterator it = from.begin(); it != from.end(); ++it)
	{
		if(NULL!= *it)
		{
			 myProc * procMetric =new myProc((*it)->pid,(*it)->cpuUseRatio,(*it)->memUsed,(*it)->connectCount,(*it)->total_write,(*it)->total_read,(*it)->name);
			 to.push_back(procMetric);
		}
	}
}
