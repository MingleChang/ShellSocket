//
//  HBTCPHeartRequest.m
//  ShellSocket
//
//  Created by MingleChang on 16/9/13.
//  Copyright © 2016年 MingleChang. All rights reserved.
//

#import "HBTCPHeartRequest.h"

@implementation HBTCPHeartRequest

+(instancetype)heart{
    HBTCPHeartRequest *lHeart=[[HBTCPHeartRequest alloc]init];
    lHeart.type=HB_TCP_MSGTYPE_HEARTBEAT;
    lHeart.tid=[HBTCPHeader nextTid];
    return lHeart;
}

@end
