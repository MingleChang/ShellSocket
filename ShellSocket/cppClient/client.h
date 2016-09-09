#ifndef __CLIENT_H__
#define __CLIENT_H__
#include <iostream>
#include <pthread.h>
#include "protocol.h"
#include "utils.h"
using namespace std;

#define PROC_VERSION 0x01
#define PROTOCOL_VERSION 0x01

/*业务通知函数
* Type:消息类型
*	-1:网络异常
* msg:消息内容
* code:请求消息返回码
* param:附加参数，如类型等
*/
typedef void (*NOTIFY)(int type,int tid,const char *msg,int code,int param);

class PushClient {
    public:
        PushClient();
        ~PushClient();

    public:
        void SetUserId(const string userid);
        void SetToken(const string token);
        void SetTermtype(const int termtype );
        void SetServer(const string server ,int port);
        void SetNotify(NOTIFY func);

        int Start();
        int Register();
        int Heartbeat();
        int Message(const char *content);
		int Response(int msgType,unsigned int tid,unsigned int code);

		int OpClass(int type,const char *classId);
		int OpRoom(int type,const char *classId);
		
        int Stop();
    private:
        int Connect();
        static void* ReadThread(void *arg);
        void Proc();
		int ProcResponse(int msgType,int tid,int code,int param);
        void Notify(const int type,int tid,const char* s,int code,int param);
        unsigned int NextTid();
		int CheckLen(int len);
		int ReadToBuf(int fd,void *buff,int length);
    private:
        string m_szUserId;
        string m_szToken;
        int m_iPlat;
		
        string m_szCometIP;  //comet ip
        int m_iCometPort;    //comet端口

        int m_iFd;          //socket 
        pthread_t pReadId;  //soket 读线程
        
        unsigned int m_iTid;//事务ID

        //业务通知
        NOTIFY notify;
};

#endif
