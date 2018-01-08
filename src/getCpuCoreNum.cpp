#include "iostream"
#include "getCpuCoreNum.h"
using namespace std;


int getCpuCoreNum()
{
	string cmd="cat /proc/cpuinfo | grep processor | wc -l";
	FILE *pp=NULL;
	pp=popen(cmd.c_str(),"r");
	if(pp==NULL)
	{
		cout<<"get cpu core num failed "<<endl;
		exit(0);
	}

	char str[256]="";
	fgets(str,sizeof(str),pp);
	if(str==NULL)
	{
		cout<<"get cpu core num failed "<<endl;
		exit(0);
	}

	int coreNum=atoi(str);
	if(coreNum<=0)
	{
		cout<<"get cpu core num failed "<<endl;
		exit(0);
	}
	pclose(pp);
	return coreNum;
}
//DEMO
/**
int main()
{
	cout<<getCpuCoreNum()<<endl;
	return 1;
}
**/
