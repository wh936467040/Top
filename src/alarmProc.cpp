#include "iostream"
#include "vector"
#include "alarmProc.h"
using namespace std;

alarmProc::alarmProc(int type,int no,string name,string time,string detail)
{
	this -> alarmType = type;
	this -> pid =no;
	this -> procName = name;
	startTime = time;
	data = detail;
	isExit=1;
}

alarmProc::~alarmProc()
{
	cout<<"free alarmProc"<<endl;
}
