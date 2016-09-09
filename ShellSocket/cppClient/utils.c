#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "utils.h"

/* 解析URL地址
*  解析参数:ip,port,query
*  不支持域名 不支持https
*  入参:url地址,外部分配内存的ip[16],port,query[128]
*  返回值:0-success -1:failed
*/
int parseUrl(const char *url,char *ip,int *port,char *query)
{
	if(url == NULL)
	{
		return -1;
	}
	if(strncasecmp(url,"http://",7) != 0){
		return -1;
	}
	char szIP[DOMAIN_MAX_LENGTH]={0x00};
	char szPort[6]={0x00};
	char szQuery[128]={0x00};

	char *p=(char *)url+7;
	int totalLen = strlen(p);
	//查找:索引
	int tokenIdx = 0;
	while(*p!='\0'){
		if(*p==':'){
			break;
		}
		p++;
		tokenIdx++;
	}
	//查找/索引
	p=(char *)url+7;
	int slashIdx = 0;
	while(*p!='\0'){
		if(*p=='/'){
			break;
		}
		p++;
		slashIdx++;
	}
	p=(char *)url+7;
	//有:有/
	if(tokenIdx>0 && tokenIdx!=totalLen && tokenIdx<DOMAIN_MAX_LENGTH){
		strncpy(szIP,p,tokenIdx);
	} else if(slashIdx>0 && slashIdx!=totalLen){
		//没有:有/
		strncpy(szIP,p,slashIdx);
	} else {
		//没有:没有/
		strncpy(szIP,p,DOMAIN_MAX_LENGTH);
	}
	
	//有端口
	if(tokenIdx>0 && tokenIdx!=totalLen){
		//you /
		if(slashIdx>0 && tokenIdx!=slashIdx){
			strncpy(szPort,p+tokenIdx+1,slashIdx-tokenIdx-1);
		}else{
			//没有/
			strncpy(szPort,p+tokenIdx,5);
		}
	}

	//有query
	if(slashIdx>0 && tokenIdx!=slashIdx){
		strncpy(szQuery,p+slashIdx,127);
	}

	strncpy(ip,szIP,DOMAIN_MAX_LENGTH);
	*port = atoi(szPort);
	if(*port==0){
		*port=80;
	}
	strncpy(query,szQuery,128);
	return 0;
}


/* 解析IP端口 入参形式ip:port
*  入参:ip:port地址,外部分配内存的ip[16],port
*  返回值:0-success -1:failed
*/
int parseIpPort(const char *addr,char *ip,int *port)
{
    if(addr==NULL)
    {
        return -1;
    }
    int idx = 0;
    char *p = (char *)addr;
    while(*p!='\0'){
        if(*p==':'){
                break;
        }
        p++;
        idx++;
    }
    if(idx>=DOMAIN_MAX_LENGTH){
        return -1;
    }
    strncpy(ip,addr,idx);
    char sport[6]={0x00};
    strncpy(sport,addr+idx+1,5);
    *port = atoi(sport);
    return 0;
}

int htconnect(const char *domain,int port)
{
	int white_sock;
	struct hostent * site;
	struct sockaddr_in me;
	site = gethostbyname(domain);
	if(site == NULL) return -2;
	white_sock = socket(AF_INET,SOCK_STREAM, 0);
	if(white_sock < 0) return -1;
	memset(&me, 0, sizeof(struct sockaddr_in));
	memcpy(&me.sin_addr, site->h_addr_list[0], site->h_length);
	me.sin_family = AF_INET;
	me.sin_port = htons(port);
	return (connect(white_sock, (struct sockaddr *)&me, sizeof(struct sockaddr)) < 0) ? -1 : white_sock;
}

int htsend(int sock,const char *fmt,...)
{
	char BUF[1024];
	va_list argptr;
	va_start(argptr, fmt);
	vsprintf(BUF,fmt, argptr);
	va_end(argptr);
	return send(sock, BUF, strlen(BUF), 0);
}

int htpost(const char *ip,int port,const char * data,const char * query)
{
	int black_sock;
	int len = 0;

	black_sock = htconnect(ip, port);
	if (black_sock < 0) return -1;
	len = strlen(data);
	htsend(black_sock, "POST %s HTTP/1.1\r\n", query, 10);
	htsend(black_sock, "Host: %s\r\n", ip, 10);
	//added by ligang @ 2016-05-30
	htsend(black_sock, "Content-Type: %s\r\n", "application/x-www-form-urlencoded", 10);
	htsend(black_sock, "Content-Length: %d\r\n", len, 10);
	htsend(black_sock, "Connection: close\r\n", 10);
	htsend(black_sock, "\r\n", 10);
	htsend(black_sock, "%s", data, 10);

	return black_sock;
}

