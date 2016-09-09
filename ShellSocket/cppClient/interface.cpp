#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <errno.h>
#include <unistd.h>
#include "client.h"
#include "base64.h"
#include "interface.h"

static PushClient *client = NULL;

void startHeartbeat(int sec);
void stopHeartbeat();
void signalRoutine(int signo);
void callBack(int type,const char *msg,int offline,int flag,int tid);

void Init()
{
    PushClient *pCli = new(PushClient);
    client = pCli;
}

void SetUserId(const string userid)
{
    if(userid.length()>32){
        return;
    }
    if(client!=NULL)
    {
        client->SetUserId(userid);
    }
}

void SetToken(const string token)
{
    if(token.length()>32){
        return;
    }
    if(client!=NULL)
    {
        client->SetToken(token);
    }
}

void SetTermtype(const int termtype )
{
    if(client!=NULL)
    {
        client->SetTermtype(termtype);
    }
}

void SetServer(const string domain ,int port)
{
    if(client!=NULL)
    {
        client->SetServer(domain,port);
    }
}

void SetNotify(NOTIFY func)
{
    if(func==NULL)
    {
        return;
    }
    if(client!=NULL)
    {
        client->SetNotify(func);
    }
}

int Start()
{
    if(client == NULL)
    {
        return -1;
    }
    int ret = client->Start();
    if (ret!=0){
        fprintf(stderr,"start client error %d",ret);
        return -1;
    }
    startHeartbeat(300);
    return 0;
}

int ReStart()
{
    if(client == NULL)
    {
        return -1;
    }
    client->Stop();
    int ret = client->Start();
    if (ret!=0){
        fprintf(stderr,"start client error %d",ret);
        return -1;
    }
    return 0;
}

int Register()
{
    if(client == NULL)
    {
        return -1;
    }    
    return client->Register();
}

int CreateClass(string classId)
{
    if(client == NULL)
    {
        return -1;
    }    
    return client->OpClass(1,classId.c_str());
}

int DropClass(string classId)
{
    if(client == NULL)
    {
        return -1;
    }    
    return client->OpClass(2,classId.c_str());
}

int EnterRoom(string classId)
{
    if(client == NULL)
    {
        return -1;
    }    
    return client->OpRoom(1,classId.c_str());
}

int ExitRoom(string classId)
{
    if(client == NULL)
    {
        return -1;
    }    
    return client->OpRoom(2,classId.c_str());
}

int Heartbeat()
{
    if(client == NULL)
    {
        return -1;
    }    
    return client->Heartbeat();
}

int Message(const char *content)
{
    if(content == NULL){
        return -1;
    }
    if(client == NULL)
    {
        return -1;
    }    
    return client->Message(content);
}

void Destroy()
{
    stopHeartbeat();
    if(client!=NULL)
    {
    	client->Stop();
		sleep(1);
    	
        delete(client);
        client = NULL;
    }
}

void startHeartbeat(int sec)
{
    signal(SIGALRM,&signalRoutine);
    struct itimerval value,ovalue;
    value.it_value.tv_sec = sec;
    value.it_value.tv_usec = 0;
    value.it_interval.tv_sec = sec;
    value.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL,&value,&ovalue);
}

void stopHeartbeat()
{
    signal(SIGALRM,&signalRoutine);
    struct itimerval value,ovalue;
    value.it_value.tv_sec = 0;
    value.it_value.tv_usec = 0;
    value.it_interval.tv_sec = 0;
    value.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL,&value,&ovalue);
}

void signalRoutine(int signo)
{
    switch(signo){
        case SIGALRM:
            {
                if(client == NULL)
                {
                    return;
                }
                int i = client->Heartbeat();
                if (i!=0){
                    fprintf(stderr,"Heartbeat error = %d",i);
                    //心跳错误 重新连接 并启动新的读线程
                    stopHeartbeat();
                    ReStart();
                }
            }
            break;
        default:
            break;
    }
}

/*
 * 检测TCP端口是否活跃
 * 入参:对端地址,端口,超时时长秒 地址可以为域名
 * 返回值:0-成功 -1:不可用
 */
int CheckTcpPort(const char *hostname,int port,int timeout)
{
	int sockfd;
	struct sockaddr_in server_addr; 
	struct hostent *host; 
	if((host=gethostbyname(hostname))==NULL) 
	{ 
		return -1;
	} 
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1) 
	{
		return -1;
	}

	struct timeval timeo = {0, 0}; 
	timeo.tv_sec = timeout;
	socklen_t len = sizeof(timeo);  
	setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeo, len); 

	bzero(&server_addr,sizeof(server_addr)); 
	server_addr.sin_family=AF_INET;  
	server_addr.sin_port=htons(port); 
	server_addr.sin_addr=*((struct in_addr *)host->h_addr); 
	if(connect(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)
	{
		if (errno == EINPROGRESS) {  
			//fprintf(stderr, "timeout/n");  
			return -1;  
		}
		//perror("connect");  
		return -1;  
	}
	close(sockfd);

	return 0; 
}

