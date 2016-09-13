//
//  HBTCPApiManager.h
//  ShellSocket
//
//  Created by MingleChang on 16/9/13.
//  Copyright © 2016年 MingleChang. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HBTCPHeader.h"

@interface HBTCPApiManager : NSObject

+(HBTCPApiManager *)manager;

//向服务端发起一个请求
-(void)sendRequest:(HBTCPHeader *)request completion:(completeBlock)complete;

@end
