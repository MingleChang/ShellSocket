#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#include "client.h"
#include "protocol.h"
#include "utils.h"

PushClient::PushClient()
{
    m_szUserId.clear();
    m_szToken.clear();
    m_iPlat = 0;
    
    m_szCometIP.clear();
    m_iCometPort = 0;

    m_iFd = 0;
    
    m_iTid = 0;

    notify = NULL;
}

PushClient::~PushClient()
{
    close(m_iFd);
}

void PushClient::SetUserId(const string userid )
{
    m_szUserId = userid;
}

void PushClient::SetToken(const string token)
{
    m_szToken = token;
}

void PushClient::SetTermtype(const int termtype )
{
    m_iPlat= termtype;
}

void PushClient::SetServer(const string doamin ,int port)
{
    m_szCometIP = doamin;
	m_iCometPort = port;
}

/* SetNotify
* 设置业务通知回调函数
* 入参:回调函数句柄
* 返回值:N/A
*/
void PushClient::SetNotify(NOTIFY func)
{
    notify = func;
}

/* Start
* 启动push客户端，链接到服务器且启动数据读线程
* 入参:N/A
* 返回值:0-成功 -1-失败
*/
int PushClient::Start()
{
    int iFd = Connect();
    if (iFd == -1){
        LOGERR("connect to connct server %d", iFd);
        return iFd; 
    }
    m_iFd = iFd;

    int ret = pthread_create(&pReadId, NULL, &PushClient::ReadThread, this);
    if (ret)
    {
        LOGERR("Create pthread error! %d", ret);
        return ret;
    }
    return 0;
}

/* Stop
* 停止服务
* 关闭socket 连接
* 读线程会自动退出
* 入参:N/A
* 返回值:N/A
*/
int PushClient::Stop()
{
    close(m_iFd);
    return 0;
}

//return socket fd or -1
int PushClient::Connect()
{
    int sockfd;
    struct sockaddr_in server_addr; 
    struct hostent *host; 
    if((host=gethostbyname(m_szCometIP.c_str()))==NULL) 
    { 
		return -1;
    } 
    if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1) 
    {
		return -1;
    }
    bzero(&server_addr,sizeof(server_addr)); 
    server_addr.sin_family=AF_INET;  
    server_addr.sin_port=htons(m_iCometPort); 
    server_addr.sin_addr=*((struct in_addr *)host->h_addr); 
    if(connect(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)
    {
        LOGERR("connect to server error %s", strerror(errno)); 
        return -1;
    }
    return sockfd;
}

/* Heartbeat
* 向服务器发起注册消息
* 入参:文件描述符指针
* 返回值:N/A
*/

void* PushClient::ReadThread(void *arg)
{
    PushClient *p = (PushClient *)arg;
    p->Register();
    p->Proc();
	
    LOGDEBUG("exit ReadThread %ld", (unsigned long)p->pReadId);
    return NULL;
}


/* Proc
* 业务处理函数
*
*/
void PushClient::Proc()
{
    int fd = m_iFd;
    while(1){
        Header header ;
        memset(&header,0x00,sizeof(Header));
		if(ReadToBuf(fd,(void *)&header,sizeof(Header))==-1){
			return;
		}
        int msgType = GetMsgType(&header);
        header.tid = htonl(header.tid);
		int tid = header.tid;

        AddHeader adh;
		if(ReadToBuf(fd,(void *)&adh,sizeof(AddHeader))==-1){
			return;
		}

        adh.len = htonl(adh.len);
		if(CheckLen(adh.len)==-1){
			return;
		}
        
        LOGDEBUG("receive msgType=%d tid=%d len=%d",msgType,header.tid,adh.len);

        switch(msgType){
        case MSGTYPE_REGRESP:
            {
        		unsigned char code;
				if(ReadToBuf(fd,(void *)&code,1)==-1){
					return;
				}

				ProcResponse(msgType,tid,code,0);
                if(code != 0){
                    LOGERR("register failed code = %d", code);
                }
            }
    		break;
        case MSGTYPE_HBRESP:
            {
        		unsigned char code;
				if(ReadToBuf(fd,(void *)&code,1)==-1){
					return;
				}
				ProcResponse(msgType,tid,code,0);
                if(code!=0){
                    LOGERR("MSGTYPE_HBRESP failed code = %d", code);
                }
            }
    		break;
		case MSGTYPE_CLSCRTRESP:
		case MSGTYPE_CLSDROPRESP:
			{
        		unsigned char code;
				if(ReadToBuf(fd,(void *)&code,1)==-1){
					return;
				}
				ProcResponse(msgType,tid,code,0);
                if(code!=0){
                    LOGERR("MSGTYPE_CLSCRTRESP or MSGTYPE_CLSDROPRESP failed code = %d", code);
                }
			}
			break;
		case MSGTYPE_ROOMINRESP:
		case MSGTYPE_ROOMOUTRESP:
			{
				unsigned char code;
				if(ReadToBuf(fd,(void *)&code,1)==-1){
					return;
				}
				ProcResponse(msgType,tid,code,0);
				if(code!=0){
					LOGERR("MSGTYPE_ROOMINRESP or MSGTYPE_ROOMOUTRESP failed code = %d", code);
				}
			}
			break;
        case MSGTYPE_MSGRESP:
            {
        		unsigned char code;
				if(ReadToBuf(fd,(void *)&code,1)==-1){
					return;
				}
				ProcResponse(msgType,tid,code,0);
				if(code!=0){
					LOGERR("MSGTYPE_MSGRESP failed code = %d", code);
				}
            }
    		break;
        case MSGTYPE_PUSH:
            {
                MPushParam *msg = (MPushParam *)malloc(adh.len+1);
                memset(msg,0x00,adh.len+1);
				if(ReadToBuf(fd,(void *)msg,adh.len)==-1){
					free(msg);
					return;
				}
                Notify(MSGTYPE_PUSH,tid,(char *)msg->msg,0,msg->type);
                free(msg);
                Response(msgType, header.tid, 0);
            }
            break;
        case MSGTYPE_MESSAGE:
            {
                MMsgParam *msg = (MMsgParam *)malloc(adh.len+1);
				memset(msg,0x00,adh.len+1);
				if(ReadToBuf(fd,(void *)msg,adh.len)==-1){
					free(msg);
					return;
				}
				Notify(MSGTYPE_MESSAGE,tid,(char *)msg->msg,0,msg->type);
                free(msg);
                Response(msgType, header.tid, 0);
            }
            break;
		case MSGTYPE_KICK:
			{
        		unsigned char reason;
				if(ReadToBuf(fd,(void *)&reason,1)==-1){
					return;
				}
				Notify(MSGTYPE_KICK,tid,"",0,reason);
			}
			break;
        default:
            LOGERR("read UNKNOW MSGTYPE = %d", msgType);
    		continue;
    	}
    }
}

int PushClient::ProcResponse(int msgType,int tid,int code,int param)
{
	switch(msgType)
	{
		case MSGTYPE_REGRESP:
			Notify(msgType,tid,"",code,param);break;
		case MSGTYPE_HBRESP:
			Notify(msgType,tid,"",code,param);break;
		case MSGTYPE_CLSCRTRESP:
			Notify(msgType,tid,"",code,param);break;
		case MSGTYPE_CLSDROPRESP:
			Notify(msgType,tid,"",code,param);break;
		case MSGTYPE_ROOMINRESP:
			Notify(msgType,tid,"",code,param);break;
		case MSGTYPE_ROOMOUTRESP:
			Notify(msgType,tid,"",code,param);break;
		case MSGTYPE_MSGRESP:
			Notify(msgType,tid,"",code,param);break;
	}
	return 0;
}

int PushClient::ReadToBuf(int fd,void *buff,int length){
	int n = 0;
	if ((n=read(fd,buff,length))<=0){
		LOGERR("read msg error %s", strerror(errno)); 
		Notify(-1,0,"",0,0);
		return -1;
	}
	return 0;
}

void PushClient::Notify(const int type,int tid,const char* s,int code,int param)
{
    if(notify==NULL){
        return;
    }
    notify(type,tid,s,code,param);
}

/* Heartbeat
* 向服务器发起注册消息
* 入参:N/A
* 返回值:0-成功 -1-失败
*/
int PushClient::Register()
{
    MRegister msg;
    memset(&msg,0x00,sizeof(MRegister));
    SetMsgType(&msg.h, MSGTYPE_REGISTER);
	SetPtl(&msg.h,1);
    msg.h.tid = htonl(NextTid());
    msg.adh.len = htonl(65);
    msg.plat = m_iPlat;
	strncpy((char *)msg.id,m_szUserId.c_str(),32);
	strncpy((char *)msg.token,m_szToken.c_str(),32);	
    
    if(write(m_iFd,&msg,sizeof(MRegister))==-1){
        LOGERR("Register write error %s", strerror(errno));
        return -1;
    } 
    return 0;
}

/* Heartbeat
* 向服务器发起心跳消息
* 入参:N/A
* 返回值:0-成功 -1-失败
*/ 
int PushClient::Heartbeat()
{
    MHeartbeat msg;
    memset(&msg,0x00,sizeof(MHeartbeat));
    SetMsgType(&msg.h, MSGTYPE_HEARTBEAT);
    SetPtl(&msg.h,1);
    msg.h.tid = htonl(NextTid());

    if(write(m_iFd,&msg,sizeof(MHeartbeat))==-1){
        LOGERR("Heartbeat write error %s", strerror(errno));
        return -1;
    }
    return 0;
}

/* OpClass
* 创建课程或结束课程
* 入参:type 1-创建 2-结束
* 返回值:0-成功 -1-失败
*/
int PushClient::OpClass(int type,const char *classId)
{
    MClass msg;
    memset(&msg,0x00,sizeof(MClass));
	if(type==1)SetMsgType(&msg.h, MSGTYPE_CLSCREATE);
	else SetMsgType(&msg.h, MSGTYPE_CLSDROP);
	SetPtl(&msg.h,1);
    msg.h.tid = htonl(NextTid());
    msg.adh.len = htonl(33);
    msg.type = type;
	strncpy((char *)msg.classId,classId,32);
    
    if(write(m_iFd,&msg,sizeof(MClass))==-1){
        LOGERR("OpClass write error %s", strerror(errno));
        return -1;
    } 
    return 0;
}

/* OpRoom
* 加入或退出课程
* 入参:N/A
* 返回值:0-成功 -1-失败
*/
int PushClient::OpRoom(int type,const char *classId)
{
    MRoom msg;
    memset(&msg,0x00,sizeof(MRoom));
	if(type==1)SetMsgType(&msg.h, MSGTYPE_ROOMIN);
	else SetMsgType(&msg.h, MSGTYPE_ROOMOUT);
	SetPtl(&msg.h,1);
    msg.h.tid = htonl(NextTid());
    msg.adh.len = htonl(33);
    msg.type = type;
	strncpy((char *)msg.classId,classId,32);
    
    if(write(m_iFd,&msg,sizeof(MRoom))==-1){
        LOGERR("OpClass write error %s", strerror(errno));
        return -1;
    } 
    return 0;
}


/* Message
* 向服务器发起即时消息
* 入参:即时消息内容
* 返回值:0-成功 -1-失败
*/ 
int PushClient::Message(const char *content)
{
    int iRet = 0;
    unsigned int len = strlen(content);
    MMessage *msg = (MMessage *)malloc(sizeof(MMessage)+len);
    memset(msg,0x00,sizeof(MMessage)+len);
    
    SetMsgType(&msg->h, MSGTYPE_MESSAGE);
    SetPtl(&msg->h,1);
    msg->h.tid = htonl(NextTid());
    msg->adh.len = htonl(len+1);
	msg->type = 0x00;
    memcpy((char *)msg->msg,content,len);
    if(write(m_iFd,msg,sizeof(MMessage)+len)==-1){
        LOGERR("Message write error %s", strerror(errno));
        iRet = -1;
    }
    free(msg);
    return iRet;
}

/* Response
* 对服务器发起的请求进行应答
* 入参:请求消息类型，请求事务ID，应答码
* 返回值:0-成功 -1-失败
*/ 
int PushClient::Response(int msgType,unsigned int tid,unsigned int code)
{
    MResponse msg;
    memset(&msg,0x00,sizeof(MResponse));
    SetMsgType(&msg.h, msgType+1);
     SetPtl(&msg.h,1);
    msg.h.tid = htonl(tid);
    msg.adh.len = htonl(1);
    msg.code = code;
    if(write(m_iFd,&msg,sizeof(MResponse))==-1){
        LOGERR("Heartbeat write error %s", strerror(errno));
        return -1;
    } 
    return 0;
}

int PushClient::CheckLen(int len){
	if(len>1024*1024*10){
		LOGERR("read msg incorret body len%d", len);
		Notify(-1,0,"",0,0);
		return -1;
	}
	return 0;
}

unsigned int PushClient::NextTid()
{
    #define MAX_TID ((1<<15)-1)
    if(m_iTid>MAX_TID){
        m_iTid = 0;
    }
    return ++m_iTid;
}
