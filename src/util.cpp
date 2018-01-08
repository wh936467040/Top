#include "iostream"
#include "sstream"
#include "string"
#include "util.h"
using namespace std;
unsigned long stolu(string str)
{
	unsigned long result;
	istringstream is(str);
	is >> result;
	return result;
}

int getSysTime(char* str, int stime)
{
	struct tm *ctm;   
	time_t ts;
	int year, mon, day, hour, min, sec;
	int index = 0;
	ts = time(NULL);
	ctm = localtime(&ts);
	year = ctm->tm_year + 1900;
	mon = ctm->tm_mon + 1;
	day = ctm->tm_mday;
	hour = ctm->tm_hour;
	min = ctm->tm_min;
	sec = ctm->tm_sec;
	sprintf(str, "%04d-%02d-%02d %02d:%02d:%02d", year, mon, day, hour, min, sec);
	
	if(sec <= stime)
	{
		index = sec;
		sec = stime;
		sprintf(str, "%04d-%02d-%02d %02d:%02d:%02d", year, mon, day, hour, min, sec);
		cout << "sleep time :" << stime - index << endl;
		sleep(stime - index);
	}
	else
	{
		sleep(60 - sec);
		cout << "sleep time :" << 60 - sec << endl;
		ts = time(NULL);
		ctm = localtime(&ts);	
		year = ctm->tm_year + 1900;
		mon = ctm->tm_mon + 1;
		day = ctm->tm_mday;
		hour = ctm->tm_hour;
		min = ctm->tm_min;
		sec = ctm->tm_sec;
		sprintf(str, "%04d-%02d-%02d %02d:%02d:%02d", year, mon, day, hour, min, sec);
	}
	return 1;
}


long stol(string str)
{
		long result;
		istringstream is(str);
		is >> result;
		return result;
}

int stoi(string str)
{
		int result;
		istringstream is(str);
		is >> result;
		return result;
}

string itos(int i)
{
		ostringstream os;
		os<<i;
		string result;
		istringstream is(os.str());
		is>>result;
		return result;
}
string ltos(long l)
{
		ostringstream os;
		os<<l;
		string result;
		istringstream is(os.str());
		is>>result;
		return result;
}

float stof(string str)
{
		float result;
		istringstream is(str);
		is>>result;
		return result;
}
string ftos(float f)
{
		ostringstream os;
		os<<f;
		string result;
		istringstream is(os.str());
		is>>result;
		return result;
}
string lltos(long long l)
{
		ostringstream os;
		os<<l;
		string result;
		istringstream is(os.str());
		is>>result;
		return result;
}

string  dtos(double d)
{
	string result;
	if(d>10000000000000)
	{
		cout<<"d:"<<d<<endl;
		cout<<"***********************"<<endl;
	}
	char tmp[512]="";
	sprintf(tmp,"%.2lf",d);
	result=string(tmp);
	return result;
}
