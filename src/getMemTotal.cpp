#include "iostream"
using namespace std;
string & trim(string &s);
int getMemTotal()
{
	string cmd="cat /proc/meminfo | grep MemTotal";
	FILE *pp=NULL;
	pp=popen(cmd.c_str(),"r");
	if(pp==NULL)
	{
		cout<<"get cpu core num failed "<<endl;
		exit(0);
	}
	char str[256]="";
	string res;
	fgets(str,sizeof(str),pp);
	res=str;
	int index = res.find_first_of(":", 0);
	if(index > 0)
	{
		res = res.substr(index+1, res.length()-4);
	}
	index = res.find_first_of("kB", 0);
	if(index > 0)
	{
		res = res.substr(0, index);
	}
	res=trim(res);
	if(str==NULL)
	{
		cout<<"get mem size failed "<<endl;
		exit(0);
	}
	//cout<<res<<"***"<<endl;
	int total=atoi(res.c_str());
	if(total<=0)
	{
		cout<<"get mem size failed"<<endl;
	}

	pclose(pp);
	return total;
}


string & trim(string &s)
{
	if(s.empty())
	{
		return s;
	}
	s.erase(0,s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ")+1);
	return s;
}
