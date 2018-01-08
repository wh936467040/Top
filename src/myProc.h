#ifndef __MY_PROC_H_
#define __MY_PROC_H_

#include "iostream"
#include "vector"
using namespace std; 
class myProc{
	public:
	int pid;
	int cpuUseRatio;
	double  memUsed;
	int connectCount;
	long total_write;
	long total_read;
	double v_write;
	double v_read;
	string name;
	myProc(int p,double cpu,double mem,int conn,long wb,long rb,string procName);
	//myProc(int p,double cpu,double mem,string procName);
	static bool cpuUseRatioCmp(myProc*a,myProc* b);
	static bool memUsedCmp(myProc*a,myProc* b);
	static bool connectionCmp(myProc*a,myProc*b);
	static bool writeSpeedCmp(myProc*a,myProc*b);
	static bool readSpeedCmp(myProc*a,myProc*b);
	static bool socketCountCmp(myProc*a,myProc* b);
	static void VectorClear(vector<myProc*> & procSet);
	static int VectorCopy(vector<myProc*> & toProc,vector<myProc*> & fromProc);
	~myProc(){};
};
#endif
