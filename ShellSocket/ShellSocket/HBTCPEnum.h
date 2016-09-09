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
    HB_TCP_MSGTYPE_REGISTER=1,//注册请求，客户端发起
    HB_TCP_MSGTYPE_REGRESP,//注册应答
    HB_TCP_MSGTYPE_HEARTBEAT,//心跳请求，客户端发起
    HB_TCP_MSGTYPE_REGISTER,
    HB_TCP_MSGTYPE_REGISTER,
    HB_TCP_MSGTYPE_REGISTER,
};

#endif /* HBTCPEnum_h */
