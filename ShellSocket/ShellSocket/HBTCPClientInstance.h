//
//  HBTCPClientInstance.h
//  ShellSocket
//
//  Created by MingleChang on 16/9/9.
//  Copyright © 2016年 MingleChang. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface HBTCPClientInstance : NSObject

+(HBTCPClientInstance *)instance;
-(void)connect:(NSString *)host port:(NSInteger)port;
-(void)disconnect;
-(void)sendData:(NSData *)data;
@end
