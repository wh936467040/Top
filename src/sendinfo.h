#ifndef __SENDINFO_H__
#define __SENDINFO_H__

#include <stdio.h>
#include <iostream.h>
#include <map>
#include <string>
#include <vector>

using namespace std;

const string TYPE_TIME_INFO = "0";
const string TYPE_ITEM_INFO = "1";
const string TYPE_SERVER_INFO = "2";
const string TYPE_DISK_INFO = "3";
const string TYPE_NET_INFO = "4";
//const string TYPE_SHOWSERVICE_INFO = "5";
const string TYPE_DB_IO_STAT = "6";
const string TYPE_DB_MEM_STAT = "7";
const string TYPE_DB_SESSION_STAT = "8";
const string TYPE_DB_SQL_STAT = "9";
const string TYPE_DB_STAT = "10";
const string TYPE_DB_TABLESPACE_STAT = "11";
const string TYPE_NET_ICMP = "12";
const string TYPE_PROCESS_DYNAMIC_INFO = "13";
const string TYPE_SERVICE_INFO = "14";
const string TYPE_MSGBUF_INFO = "15";
const string TYPE_PROXY_INFO = "16";
const string TYPE_CORE_INFO = "17";
const string TYPE_NETSTAT_INFO = "18";
const string TYPE_DALARM_INFO = "19";
const string TYPE_SCADA_DATA_INFO = "20";
const string TYPE_DB_STATIC_STAT_INFO = "21";
const string TYPE_JUMP_INFO = "22";
const string TYPE_DISPATCH_LINK_STATE_INFO = "23";
const string TYPE_MSG_CHANNEL_INFO = "24";
const string TYPE_SYSCTL_DATA_INFO = "25";
const string TYPE_DSYSTEM_STATUS_INFO = "26";
const string TYPE_REALTIME_STATE_INFO = "27";
const string TYPE_JOB_MGR_INFO = "28";
const string TYPE_REALTIME_ACCESSNUM_INFO = "29";
const string TYPE_REALTIME_DOWNLOADNUM_INFO = "30";
const string TYPE_SERVICE_STATUS_INFO = "31";
const string TYPE_TOP_CPU_INFO = "32";
const string TYPE_TOP_MEM_INFO = "33";
const string TYPE_SERVER_PHY = "34";
const string TYPE_ALL = "43";
const string TYPE_ALARM_INFO_HARD = "44";
const string TYPE_ALARM_INFO_DISHARD = "45";
const string TYPE_ALARM_INFO_SYS = "46";
const string TYPE_ALARM_INFO_DISYS = "47";
const string TYPE_ALARM_INFO = "48";
const string TYPE_DISALARM_INFO = "49";
const string TYPE_NOTICE_INFO = "50";
const string TYPE_DSTART_ALARM_INFO = "51";
const string TYPE_DSSTART_ALARM_INFO = "52";
const string TYPE_WALARM_INFO = "53";
const string TYPE_WNOTICE_INFO = "54";
const string TYPE_DISWALARM_INFO = "55";

const string SHOWSERVICE_INFO = "10001";
const string PROCESS_DYNAMIC_INFO_ = "10002";
const string DATA_SCADA_DATAQ_ACCORD = "10003";
const string DATA_SCADA_DATAQ_BALANCE = "10004";
const string DATA_SCADA_DATAQ_DEVICE = "10005";
const string DATA_SCADA_DATAQ_MEASURE = "10006";
const string DATA_SCADA_DATAQ_MODEL = "10007";
const string DATA_SCADA_DATAQ_SUBSTATION = "10008";
const string DATA_SCADA_DATAQ_UPLOAD = "10009";
const string DATA_SCADA_DATAQ_YX = "10010";
const string SYSCTL_DATA_INFO_ = "10024";
const string SERVICE_STATUS_INFO ="10030";
const string PORT_NETINFO = "10031";
const string TOP_READ = "10032";
const string TOP_SOCKET_CONNECT = "10033";
const string TOP_WRITE = "10034";
const string FILE_VERSION = "10035";
const string DATA_MAIN_FRAM_NODE = "10048";
const string DATA_DC_EQU = "10049";
const string DATA_CLOCK_MGR = "10050";
const string LOADAVG = "10058";
const string DATA_PUM_DATA = "10052";
const string MSG_ENENT_NUMBER = "10060";
const string KID_CONNECT_NAME = "20002";
const string UPDATE_FILE_VERSION = "20004";
const string RESEND_PKG_FORMULA_DEFINE = "20005";
const string JBZDH_CPUINFO = "30001";
const string JBZDH_MEMINFO = "30002";
const string JBZDH_ENCRATEINFO = "30003";
const string JBZDH_DEVCOUNT = "30004";
const string JBZDH_NETFLOW = "30005";
const string JBZDH_SECURITYINDEX = "30006";
const string JBZDH_HISENCRATE = "30007";
const string JBZDH_HISONLINERATE = "30008";
const string JBZDH_HISDEVCOUNT = "30009";
const string JBZDH_HISWARNINGNUM = "30010";
const string JBZDH_HISSECURITYINDEX = "30011";
const string JBZDH_MONITORINFO = "30012";
const string JBZDH_WARNINGCURVE = "30013";
const string JBZDH_WARNINGLIST = "30014";
const string JBZDH_WARNINGNUM = "30015";
const string FORMULA_CLASS_INFO = "30016";
const string FORMULA_DEFINE = "30017";
const string JBZDH_HISVOLONLINERATE = "30018";
const string SESSION_ALARM_INFO = "40001";



struct SHOWSERVEICE_INFO {
	string context; //态号,多个态中间以;分割，示例：context1;context2;context3
	string app;  //应用号，多个应用名以;分割，并且与context一一对应
	string app_name;//应用名
	string priority;// 优先级，多个情况同上
	string status;//当前状态，多个情况同上
	string nodename;//节点名，多个情况同上
	string fresh_status;//刷新状态，多个情况同上
	string switch_status;//切换状态，多个情况同上
};

struct DB_IO_STAT {
	string read_count; //读次数
	string write_count; //写次数
	string read_bytes; //读大小
	string write_bytes; //写大小
};

struct DB_MEM_STAT {
	string mem_total_bytes; //内存总大小
	string mem_used_bytes; //内存已用总大小
	string os_used_bytes; //os已用总大小
	string buf_bytes; //buf大小
	string log_buf_bytes; //日志buf大小
};

struct DB_SESSION_STAT {
	string sess_addr; //session地址
	string create_time; //创建时间
	string login_name; //登录名
	string client_app; //
	string client_ip; //客户端ip
	string status; //状态
	string sql_text; //sql语句
	string cpu_time; //
	string cpu_time_call; //
	string logic_read; //
	string logic_read_call; //
	string mem_used; //已用内存
};

struct DB_SQL_STAT {
	string start_time; //执行时间
	string exec_time; //执行时间
	string client_ip; //客户端ip
	string db_name; //数据库名
	string login_name; //登录名
	string app_name; //应用名
	string sess_addr;
	string tran_id;
	string sql_text; //sql语句
};

struct DB_STAT {
	string session_count; //session个数
	string trx_count;
	string start_time;
};

struct DB_STATIC_STAT{
	string version;
	string max_session_count;
	string port;
};

struct DB_TABLESPACE_STAT {
	string tp_name; //
	string group_name; //组名
	string file_path; //文件路径
	string tp_id; //
	string group_id; //
	string file_id; //文件id
	string file_num; //文件数量
	string total_bytes; //总大小
	string free_bytes; //空闲大小
	string use_ratio; //使用率
};

struct NET_ICMP {
	string net_ip; //ip地址
	string net_mac; //mac地址
};

struct PROCESS_DYNAMIC_INFO {
	string context_name; //态
	string app_name;  //应用名
	string proc_name; //进程名
	string critical; //进程等级
	string status; //进程状态
	string cpu_percent; //cpu使用率
	string mem_occupy; //内存占用率
	string run_time; //运行时间
	string start_time; //开始时间
	string thread_num; //线程数
	string file_discriptor_used; //占用文件描述个数
	string coredump_num; //coredump次数
	string disk_read; //磁盘读入速率
	string disk_write; //磁盘写入速率
	string net_in; //接收数据量
	string net_out; //发出数据量
	string str_time; //当前时间
	string proc_pid; //进程pid
	string core_lasttime;//最新事件
	string coredump_sum;//core总和
};

struct SERVICE_INFO {
	string name; //服务名
	string sysid; //系统id
	string servid; //服务id
	string addr; //主机地址
	string port; //端口号
	string balance; //均衡
	string connum; //连接数
	string accessnum; //访问数
};

struct MSGBUF_INFO {
	string recv_msg; //消息总线接受消息数
	string send_msg; //消息总线发送消息数
};

struct PROXY_INFO {
	string prot;  //协议名
	string recv_q;//发送请求数
	string send_q;//接受请求数
	string foreign_addr;//本地地址
	string local_addr;//连接地址
	string status;//连接状态
	string process;//进程名和pid
};

struct CORE_INFO {
	string core_name;
	string serv_name;
	string core_time;
};
/*
 * 根据传入的节点名、时间和监测数据拼成xml
 * nodeID：   节点编号
 * time:      监测时间
 * map_item:  监测值，格式为：<itemid,value>
 */

 struct NET_STAT_INFO  //网卡数据
{
    string Id;    
	string netcard_name;/* 网卡名 */
	string ip_addr;/* 网卡地址 */
    string net_status;   /* 网卡状态 */
    string mtu;    /* 最大传输单元 */
    string tx_queue_len;   /* 传输队列长度 */
    string average_flow;   /* 网卡平均流量(字节每秒) */
    string rx_packets;    /* 接收包的数量 */
    string tx_packets;    /* 发送包的数量 */
    string rx_kbytes; /* 收到的千字节数 */
    string tx_kbytes; /* 发送的千字节数 */
    string rx_errors; /* 收到出错包数 */
    string tx_errors; /* 发送包出错数 */
    string collisions;    /* 发生冲突次数 */
};

struct ALARM_INFO
{
	string itemid;
	string data;
};

struct NOTICE_INFO
{
	string itemid;
	string data;
};
struct ALARM_INFO_HARD
{
	string itemid;
	string data;
};

struct ALARM_INFO_D5000
{
	string itemid;
	string data;
};

struct DISALARM_INFO_D5000
{
	string itemid;
	string starttime;
	string endtime;
	string flag;
};

struct SCADA_DATA_QUALITY
{
	string kid;//key_id
	string current_data;
	string field_id;//域号
	string flash_flag;//刷新标志
	string unflash_time;
};


struct JUMP_INFO
{
	string key_id;//key_id
	string desc;
	string field_id;//域号
	string jump_flag;//是否跳变标志
	string jump_time;//跳变时间
};

struct DISPATCHCENTER_LINK_STATE
{
	string dispatchcenter_name;//分发中心名
	string link_state;//链路状态
	string link_num;//链路号
};

struct MSG_CHANNEL_INFO
{
	string channel_id;//通道号
	string event_id;//事件号
	string event_count;//事件流量个数
	string event_length;//事件流量字节数
};

struct SYSCTL_DATA_INFO
{
	string sum_page;//最大共享内存页的数量
	string usable_page;//剩余共享内存页的数量
	string sum_mins;//最大共享内存段的数量
	string usable_mins;//剩余共享内存段的数量
	string sum_array;//最大信号量的数量
	string usable_array;//剩余信号量的数量
};

struct DSYSTEM_STATUS_INFO
{
	string start_time;//D5000运行开始时间
	string operation_time;//D5000持续运行时间
	string status;//D5000运行状态
};

struct REALTIME_STATE_INFO
{
	string tableid;//表id
	string appname;//应用名
	string tablename;//表名
	string use_ratio;//使用率
	string record_num;//记录数
	string record_sum;//记录总数
};
struct JOB_MGR
{
	string node_name;//节点名
	string command;//命令
	string run_time;//开始运行时间
};
struct REALTIME_TABLE_ACCESS
{
	string app_id;//应用号
	string table_name;//表名
	string access_num;//访问次数
};
struct REALTIME_DOWNLOAD_NUM
{
	string download_num;//下装次数
	string download_time;//下装时间
};

struct SERVICE_STATUS
{
	string name;//服务名
	string serv_host;//主机名
	string serv_port;//主机端口
	string client_ip;//客户端ip
	string client_port;//客户端端口
	string status;//状态
};

struct TOP3_INFO
{
	string pid;
	string top_mem;
	string top_cpu;
	string command;
};


class SendInfo 
{
public:
	int m_port_main;
	string m_ip_main;
	int m_port_back;
	string m_ip_back;
	int alarm_port_main;
	string alarm_ip_main;
	int alarm_port_back;
	string alarm_ip_back;

public:
	string createXML(string nodeID, string time, map<string, string> map_item);

	int ifReSendXml(string xml);
	/*
	 * 通过tcp连接发送xml数据
	 * port: 连接的服务器端的端口号
	 * ip:   连接的服务器端的ip地址
	 * */
	int sendXML(string xml);
	int sendBackXML(string xml);
	int ifReSendAlarmXml(string xml);
	int sendAlarmXML(string xml);
	int sendAlarmBackXML(string xml);

	/*
	 * 创建并发送动态的item数据
	 * nodeID:  节点号、磁盘号或者网卡号
	 * time:    监测时间
	 * map_time: <itemid, value> 监测项与监测数据的键值对序列
	 * port:    连接的服务器端端口号
	 * ip:      连接的服务器端ip地址
	 */
	int sendInfo(string nodeID, string time, map<string, string>map_item);

	/*
	 * 创建并发送监测的时间数据
	 * nodeID:  节点号
	 * time:    监测时间
	 * port:    连接的服务器端端口号
	 * ip:      连接的服务器端ip地址
	 */
	int sendTimeInfo(string nodeID, string time);

	/*
	 * 根据传入的服务器静态信息创建XML
	 * nodeID:    服务器id
	 * name:      服务器节点名
	 * memTotal:  内存总容量
	 * cpuNumber: cpu个数
	 * diskCapacity: 磁盘总容量
	 * system:    操作系统信息
	 * */
	string createServerXML(string nodeID, string name,string memTotal, string cpuNumber, string diskCapacity, string system,string ip,string mac,string mask_ip,string safekey_id);

	/*
	 * 创建并发送服务器静态信息
	 * nodeID:   服务器节点号
	 * name:     服务器名
	 * memTotal: 内存总容量
	 * cpuNumber:cpu个数
	 * diskCapacity: 内存总容量
	 * system:   系统版本信息
	 * port:     连接的服务器端端口号
	 * ip:       连接的服务器端ip地址
	 */
	int sendServerInfo(string nodeID, string name, int memTotal, int cpuNumber, long diskCapacity, string system,string ip,string mac,string mask_ip,string safekey_id);

	/*
	 * 创建并发送文件系统的静态信息
	 * nodeID:    服务器节点号
	 * id:        文件系统编号,多个id中间以;分割，示例：id1;id2;id3
	 * name:      文件系统名称，多个name以;分割，并且与id一一对应
	 * content:   文件系统目录，多个情况同上
	 * capyacity: 文件系统的容量，多个情况同上
	 * port:      连接的服务器端端口号
	 * ip:        连接的服务器端ip地址
	 * */
	int sendDiskInfo(string nodeID, string id, string name, string content, string capacity);

	/*
	 * 创建并发送网络的静态信息
	 * nodeID： 节点编号
	 * id:      网卡编号,多个id中间以;分割，示例：id1;id2;id3
	 * name:    网卡名，多个name以;分割，并且与id一一对应
	 * ip:      网卡Ip，多个情况同上
	 * mac:     网卡的MAC地址，多个情况同上
	 * port:    连接的服务器端端口号
	 * server_ip:连接的服务器端ip地址
	 * */
	int sendNetInfo(string nodeID, string id, string name ,string ip, string mac);

	/*
	 * 创建并发送应用状态信息
	 * nodeID：      节点编号
	 * time:         采集时间
	 * info:         应用状态结构体信息
	 * port:         连接的服务器端端口号
	 * server_ip:    连接的服务器端ip地址
	 * */
	int sendShowServiceInfo(string nodeID, string time, SHOWSERVEICE_INFO info);

	/*
	 * 创建并发送数据库IO信息
	 * nodeID：      节点编号
	 * time:         采集时间
	 * info:         数据库IO信息
	 * port:         连接的服务器端端口号
	 * server_ip:    连接的服务器端ip地址
	 * */
	int sendDBIoStatInfo(string nodeid,string time,DB_IO_STAT info);

	/*
	 * 创建并发送数据库内存信息
	 * nodeID：      节点编号
	 * time:         采集时间
	 * info:         数据库内存信息
	 * port:         连接的服务器端端口号
	 * server_ip:    连接的服务器端ip地址
	 * */
	int sendDBMemStatInfo(string nodeid,string time,DB_MEM_STAT info);

	/*
	 * 创建并发送session信息
	 * nodeID：      节点编号
	 * time:         采集时间
	 * info:         session信息
	 * port:         连接的服务器端端口号
	 * server_ip:    连接的服务器端ip地址
	 * */
	int sendDBSessionStatInfo(string nodeid,string time,DB_SESSION_STAT info);

	/*
	 * 创建并发送超时SQL信息
	 * nodeID：      节点编号
	 * time:         采集时间
	 * info:         超时SQL信息
	 * port:         连接的服务器端端口号
	 * server_ip:    连接的服务器端ip地址
	 * */

	int sendDBSqlStat(string nodeID, string time, DB_SQL_STAT info);

	/*
	 * 创建并发送数据库状态信息
	 * nodeID：      节点编号
	 * time:         采集时间
	 * info:         数据库状态信息
	 * port:         连接的服务器端端口号
	 * server_ip:    连接的服务器端ip地址
	 * */
	int sendDBStat(string nodeID, string time, DB_STAT info);
	
	int sendDBStaticStat(string nodeID, string time, DB_STATIC_STAT info);

	int sendDBTableSpaceStat(string nodeID, string time, DB_TABLESPACE_STAT info);

	int sendNetIcmpInfo(string time, NET_ICMP info);

	int sendProcessDynamicInfo(string nodeid,string time,PROCESS_DYNAMIC_INFO info);

	int sendServiceInfo(string nodeid,string time,SERVICE_INFO info);

	int sendMsgBufInfo(string nodeid,string time,MSGBUF_INFO info);

	int sendProxyInfo(string nodeid,string time,PROXY_INFO info);

	int sendCoreInfo(string nodeid,string time,CORE_INFO info);
	
	int sendNetStatInfo(string nodeid,string time,NET_STAT_INFO info);
	
	int sendHardAlarmInfo(string nodeid,string starttime, ALARM_INFO_HARD info);
	
	int sendDHardAlarmInfo(string nodeid,string itemid,string starttime,string endtime);
	
	int sendD5000AlarmInfo(string nodeid,string starttime, ALARM_INFO_D5000 info);
	
	//应用取消告警
	int sendD5000DisAlarmInfo(string nodeid,DISALARM_INFO_D5000 info);
	
	int sendD5000DisAlarmInfo(string nodeid,string itemid,string starttime,string endtime,string flag);
	
	int sendD5000DisAlarmInfoWithType(string nodeid,string type, DISALARM_INFO_D5000 info);
	
	//新取消告警，硬件+系统
	int sendDAlarmInfo(string nodeid,string itemid,string starttime,string endtime,string data);
	
	//新告警，硬件+系统
	int sendAlarmInfo(string nodeid,string starttime, ALARM_INFO info);
	
	int sendNoticeInfo(string nodeid,string starttime, NOTICE_INFO info);
	
	int sendWAlarmInfo(string nodeid,string nodename,string starttime, ALARM_INFO_D5000 info);

	int sendDStartAlarmInfo(string nodeid,string itemid,string time);
	
	int sendDSStartAlarmInfo(string nodeid,string itemid,string time);
	
	int sendWNoticeInfo(string nodeid,string nodename,string starttime, NOTICE_INFO info);
	
	int sendScadaData(string nodeid,string time,SCADA_DATA_QUALITY info);
	
	int sendJumpInfo(string nodeid,string time,JUMP_INFO info);
	
	int sendDispatchLinkStateInfo(string nodeid,string time,DISPATCHCENTER_LINK_STATE info);
	
	int sendMSGChannelInfo(string nodeid,string time,MSG_CHANNEL_INFO info);
	
	int sendSysctlDataInfo(string nodeid,string time,SYSCTL_DATA_INFO info);
	
	int sendDsystemStatusInfo(string nodeid,string time,DSYSTEM_STATUS_INFO info);
	
	int sendRealtimeStateInfo(string nodeid,string time,REALTIME_STATE_INFO info);
	
	int sendJobMgrInfo(string nodeid,string time,JOB_MGR info);
	
	int sendRealtimeAccessNumInfo(string nodeid,string time,REALTIME_TABLE_ACCESS info);
	
	int sendRealtimeDownloadNumInfo(string nodeid,string time,REALTIME_DOWNLOAD_NUM info);
	
	int sendServiceStatusInfo(string nodeid,string time,SERVICE_STATUS info);
	
	int sendTopCpuInfo(string nodeid,string time,TOP3_INFO info);
	
	int sendTopMemInfo(string nodeid,string time,TOP3_INFO info);

	int sendAllInfo(string type,string nodeid,string time,vector<string> vec);
	
	vector<string> parseListData(string &str, string &delim);
};

#endif
