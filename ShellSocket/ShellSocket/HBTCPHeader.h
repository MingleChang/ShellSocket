//
//  HBTCPHeader.h
//  ShellSocket
//
//  Created by MingleChang on 16/9/12.
//  Copyright © 2016年 MingleChang. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HBTCPConfigure.h"
#import "HBTCPProtocol.h"

@interface HBTCPHeader : NSObject<HBTCPProtocol>

@property(nonatomic,assign)HBTCPMessageType type;
@property(nonatomic,assign)NSUInteger tid;

@property(nonatomic,copy)completeBlock complete;

+(NSUInteger)nextTid;

@end
