//
//  HBTCPClient.h
//  ShellSocket
//
//  Created by 常峻玮 on 16/9/15.
//  Copyright © 2016年 MingleChang. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HBTCPConfigure.h"
@class HBTCPClient;

@protocol HBTCPClientDelegate <NSObject>

-(void)client:(HBTCPClient *)client handleStreamEventOpenCompleted:(NSStream *)stream;
-(void)client:(HBTCPClient *)client handleStreamEventHasBytesAvailable:(NSStream *)stream;
-(void)client:(HBTCPClient *)client handleStreamEventHasSpaceAvailable:(NSStream *)stream;
-(void)client:(HBTCPClient *)client handleStreamEventErrorOccurred:(NSStream *)stream;
-(void)client:(HBTCPClient *)client handleStreamEventEndEncountered:(NSStream *)stream;

@end


@interface HBTCPClient : NSObject
@property(nonatomic,strong,readonly)NSInputStream *inputStream;
@property(nonatomic,strong,readonly)NSOutputStream *outputStream;

@property(nonatomic,assign)HBTCPConnectState state;
@property(nonatomic,weak)id<HBTCPClientDelegate> delegate;

-(void)connect:(NSString *)host port:(NSInteger)port;
-(void)connect:(NSString *)host port:(NSInteger)port completion:(completeBlock)complete;
-(void)disconnect;
-(void)sendData:(NSData *)data;

@end
