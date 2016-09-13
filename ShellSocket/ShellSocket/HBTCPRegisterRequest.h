//
//  HBTCPRegisterRequest.h
//  ShellSocket
//
//  Created by MingleChang on 16/9/12.
//  Copyright © 2016年 MingleChang. All rights reserved.
//

#import "HBTCPHeader.h"

@interface HBTCPRegisterRequest : HBTCPHeader

@property(nonatomic,assign)HBTCPDeviceType deviceType;//终端类型
@property(nonatomic,copy)NSString *userId;//用户ID
@property(nonatomic,copy)NSString *token;//用户授权码

+(instancetype)registerWithUserId:(NSString *)userId andToken:(NSString *)token;

@end
