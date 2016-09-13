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

-(void)sendRequest:(HBTCPHeader *)request completion:(completeBlock)complete;

@end
