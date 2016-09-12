//
//  HBTCPHeader.h
//  ShellSocket
//
//  Created by MingleChang on 16/9/12.
//  Copyright © 2016年 MingleChang. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HBTCPEnum.h"

@interface HBTCPHeader : NSObject

@property(nonatomic,assign)HBTCPMessageType type;
@property(nonatomic,assign)NSUInteger tid;

@end
