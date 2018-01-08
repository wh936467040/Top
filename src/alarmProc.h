#ifndef __ALARM_PROC_H_
#define __ALARM_PROC_H_


#include "iostream"
#include "myProc.h"
#include "vector"
using namespace std;
class alarmProc
{
public :
	int pid;
	string procName;
	int alarmType;
	alarmProc(int type,int no,string name,string time,string detail);
	string startTime;
	string data;
	int isExit;
	~alarmProc();
};
#endif
