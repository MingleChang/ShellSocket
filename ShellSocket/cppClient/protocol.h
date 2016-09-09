#ifndef _PROTOCOL_H__
#define _PROTOCOL_H__
#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)
typedef struct tagHeader{
    unsigned char line;
    unsigned int    tid;
}Header;

typedef struct tagAddHeader{
    unsigned int len;
}AddHeader;

typedef struct tagResponse{
    Header h;
    AddHeader adh;
    unsigned char code;
} MResponse;

typedef struct tagRegister{
    Header h;
    AddHeader adh;
    unsigned char plat;
    unsigned char id[32];
    unsigned char token[32];
} MRegister;

typedef struct tagHeartbeat{
    Header h;
} MHeartbeat;

typedef struct tagKick{
    Header h;
    AddHeader adh;
    unsigned char reson;
} MKick;

typedef struct tagClass{
    Header h;
    AddHeader adh;
	unsigned char type;
    unsigned char classId[32];
} MClass;

typedef struct tagRoom{
    Header h;
    AddHeader adh;
	unsigned char type;
    unsigned char classId[32];
} MRoom;

typedef struct tagMessage{
    Header h;
    AddHeader adh;
	unsigned char type;
    unsigned char msg[0];
} MMessage;

typedef struct tagMMsgParam{
	unsigned char type;
    unsigned char msg[0];
}MMsgParam;

typedef struct tagPush{
    Header h;
    AddHeader adh;
	unsigned char type;
    unsigned char msg[0];
}MPush;

typedef struct tagMPushParam{
	unsigned char type;
    unsigned char msg[0];
}MPushParam;

enum {
    MSGTYPE_DEFAULT   = 0,
    MSGTYPE_REGISTER  = 1,
    MSGTYPE_REGRESP   = 2,
    MSGTYPE_HEARTBEAT = 3,
    MSGTYPE_HBRESP    = 4,
    MSGTYPE_KICK      = 5,
    MSGTYPE_KICKRESP  = 6,
    MSGTYPE_CLSCREATE  = 7,
    MSGTYPE_CLSCRTRESP    = 8,
    MSGTYPE_CLSDROP,
    MSGTYPE_CLSDROPRESP,
    MSGTYPE_ROOMIN,
    MSGTYPE_ROOMINRESP,
    MSGTYPE_ROOMOUT,
    MSGTYPE_ROOMOUTRESP,
    MSGTYPE_MESSAGE,
    MSGTYPE_MSGRESP,
    MSGTYPE_PUSH,
    MSGTYPE_PUSHRESP,
    MSGTYPE_MAX       = 64,
};

enum {
    PLAT_DEFAULT = 0,
    PLAT_ANDROID = 1,
    PLAT_IOS     = 2,
    PLAT_WINPHONE= 4,
    PLAT_WEB     = 8,
    PALT_PCCLIENT=16,
};

#pragma pack()

int GetMsgType(Header *h);
void SetMsgType(Header *h,int msgType);
int GetPtl(Header *h);
void SetPtl(Header *h,int ptl);

#ifdef __cplusplus
}
#endif
#endif

