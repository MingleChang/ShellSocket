//
//  HBTCPRegisterRequest.m
//  ShellSocket
//
//  Created by MingleChang on 16/9/12.
//  Copyright © 2016年 MingleChang. All rights reserved.
//

#import "HBTCPRegisterRequest.h"

@implementation HBTCPRegisterRequest

+(instancetype)registerWithUserId:(NSString *)userId andToken:(NSString *)token{
    HBTCPRegisterRequest *lRegister=[[HBTCPRegisterRequest alloc]init];
    lRegister.type=HB_TCP_MSGTYPE_REGISTER;
    lRegister.tid=[HBTCPHeader nextTid];
    lRegister.deviceType=HB_TCP_DEVICE_TYPE_IOS;
    lRegister.userId=userId;
    lRegister.token=token;
    return lRegister;
}

#pragma mark - HBTCPProtocol
-(NSMutableData *)package{
    NSMutableData *lData=[super package];
    
    //生成消息体的数据
    NSMutableData *lBodyData=[NSMutableData data];
    uint8_t device=self.deviceType;
    [lBodyData appendBytes:&device length:1];
    [lBodyData appendData:[self.userId dataUsingEncoding:NSASCIIStringEncoding]];
    [lBodyData appendData:[self.token dataUsingEncoding:NSASCIIStringEncoding]];
    
    //消息的长度
    NSUInteger length=[lBodyData length];
    length=htonl(length);
    [lData appendBytes:&length length:4];
    [lData appendData:lBodyData];
    
    return lData;
}

@end
