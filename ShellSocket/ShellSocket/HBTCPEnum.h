//
//  HBTCPEnum.h
//  ShellSocket
//
//  Created by MingleChang on 16/9/9.
//  Copyright © 2016年 MingleChang. All rights reserved.
//

#ifndef HBTCPEnum_h
#define HBTCPEnum_h

typedef NS_ENUM(NSInteger,HBTCPMessageType){
//    HB_TCP_RESERVED=0,//保留
    HB_TCP_MSGTYPE_REGISTER=1,  //注册请求，客户端发起
    HB_TCP_MSGTYPE_REGRESP,     //注册应答
    HB_TCP_MSGTYPE_HEARTBEAT,   //心跳请求，客户端发起
    HB_TCP_MSGTYPE_HBRESP,      //心跳应答
    HB_TCP_MSGTYPE_KICK,        //踢人通知，服务端发起
    HB_TCP_MSGTYPE_KICKRESP,    //踢人通知回执
    HB_TCP_MSGTYPE_CLSCREATE,   //课程创建请求，客户端发起
    HB_TCP_MSGTYPE_CLSCRTRESP,  //课程创建应答
    HB_TCP_MSGTYPE_CLSDROP,     //课程删除请求，客户端发起
    HB_TCP_MSGTYPE_CLSDROPRESP, //课程删除应答
    HB_TCP_MSGTYPE_ROOMIN,      //教室加入请求，客户端发起
    HB_TCP_MSGTYPE_ROOMINRESP,  //教室加入应答
    HB_TCP_MSGTYPE_ROOMOUT,     //教室退出请求，客户端发起
    HB_TCP_MSGTYPE_ROOMOUTRESP, //教室退出应答
    HB_TCP_MSGTYPE_MESSAGE,     //教室内发送消息请求/通知
    HB_TCP_MSGTYPE_MSGRESP,     //教室内发送消息应答/回执
    HB_TCP_MSGTYPE_PUSH,        //消息推送通知，服务端发起
    HB_TCP_MSGTYPE_PUSHRESP,    //消息推送回执
};

#endif /* HBTCPEnum_h */
