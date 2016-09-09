#ifndef __UTILS_H__
#define __UTILS_H__
//#define LOGDEBUG(a,b...) fprintf(stdout,a,b);fprintf(stdout,"\n")
//#define LOGERR(a,b...) fprintf(stderr,a,b);fprintf(stderr,"\n")

#define LOGDEBUG(a,b...) fprintf(stdout,a,b);fprintf(stdout," %s:%d\n",__FILE__,__LINE__)
#define LOGERR(a,b...) fprintf(stderr,a,b);fprintf(stderr," %s:%d\n",__FILE__,__LINE__)
//#define LOGDEBUG(a,b...)  __android_log_print(ANDROID_LOG_INFO,"miux INFO jni",a,b)
//#define LOGERR(a,b...)  __android_log_print(ANDROID_LOG_INFO,"miux error jni",a,b)
#define DOMAIN_MAX_LENGTH 32
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#ifdef __cplusplus
extern "C" {
#endif

int parseUrl(const char *url,char *ip,int *port,char *query);
int parseIpPort(const char *addr,char *ip,int *port);
int htconnect(const char *domain,int port);
int htsend(int sock,const char *fmt,...);
int htpost(const char *ip,int port,const char * data,const char * query);

#ifdef __cplusplus
	}
#endif
#endif
