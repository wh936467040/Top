#ifndef __ALARM_VECTOR_H_
#define __ALARM_VECTOR_H_
#include "iostream"
#include "vector"
#include "alarmProc.h"
using namespace std;
#define CPUALARM  1
#define MEMALARM  2
#define CONNALARM 3 

/**
*一个单类
*存储不同的告警集合
*/
class alarmVector{
public:
	vector<alarmProc*> cpuAlarmSet;
	vector<alarmProc*> memAlarmSet;
	vector<alarmProc*> connAlarmSet;
		
	/*查找一个进程是否已经告警 */
	int findAlarmFromAlarmSet(int alarmType, int pid ,string procName);
	
	/*删除队列中的告警 */
	int deleteAlarmFromAlarmSet(int alarmType ,int pid ,string procName);
	
	/* 插入一条告警*/
	int insertAlarmIntoAlarmSet(int alarmType, int pid ,string procName,string start,string data);
	
	int searchNotExitProc(); 

	int refreshProc();

	int findAndSendMemAlarm(vector<myProc*> procSet,string timeNow);
};
#endif
