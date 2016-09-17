//
//  HBTCPClient.m
//  ShellSocket
//
//  Created by 常峻玮 on 16/9/15.
//  Copyright © 2016年 MingleChang. All rights reserved.
//

#import "HBTCPClient.h"

@interface HBTCPClient ()<NSStreamDelegate>

@property(nonatomic,strong,readwrite)NSInputStream *inputStream;
@property(nonatomic,strong,readwrite)NSOutputStream *outputStream;

@property(nonatomic,copy)completeBlock connectCompleteBlock;

@end

@implementation HBTCPClient

#pragma mark - Public
-(void)connect:(NSString *)host port:(NSInteger)port{
    [self connect:host port:port completion:nil];
}
-(void)connect:(NSString *)host port:(NSInteger)port completion:(completeBlock)complete{
    if (self.state!=HBTCPConnectStateDisconnect) {
        [self disconnect];
    }
    
    self.state=HBTCPConnectStateConnecting;
    self.connectCompleteBlock=complete;
    
    
    NSInputStream  *tempInput  = nil;
    NSOutputStream *tempOutput = nil;
    
    //    [HBTCPHelper getStreamsToHostNamed:host port:port inputStream:&tempInput outputStream:&tempOutput];
    [NSStream getStreamsToHostWithName:host port:port inputStream:&tempInput outputStream:&tempOutput];
    self.inputStream  = tempInput;
    self.outputStream = tempOutput;
    
    [self.inputStream setDelegate:self];
    [self.outputStream setDelegate:self];
    
    [self.inputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    [self.outputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    
    [self.inputStream open];
    [self.outputStream open];
}
-(void)disconnect{
    if (self.state==HBTCPConnectStateDisconnect) {
        return;
    }
    
    self.state=HBTCPConnectStateDisconnect;
    if (self.inputStream) {
        [self.inputStream close];
        [self.inputStream removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        self.inputStream=nil;
    }
    if (self.outputStream) {
        [self.outputStream close];
        [self.outputStream removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        self.outputStream=nil;
        
    }
}
-(void)sendData:(NSData *)data{
    [self.outputStream write:[data bytes] maxLength:[data length]];
}
#pragma mark - Private
-(void)handleStreamEventOpenCompleted:(NSStream *)stream{//连接成功
    if (stream == self.outputStream) {
        self.state=HBTCPConnectStateConnected;
        if (self.connectCompleteBlock) {
            self.connectCompleteBlock(nil,nil);
            self.connectCompleteBlock=nil;
        }
    }
    if ([self.delegate respondsToSelector:@selector(client:handleStreamEventOpenCompleted:)]) {
        [self.delegate client:self handleStreamEventOpenCompleted:stream];
    }
}
-(void)handleStreamEventHasBytesAvailable:(NSStream *)stream{//接收数据
    if ([self.delegate respondsToSelector:@selector(client:handleStreamEventHasBytesAvailable:)]) {
        [self.delegate client:self handleStreamEventHasBytesAvailable:stream];
    }
}
-(void)handleStreamEventHasSpaceAvailable:(NSStream *)stream{//发送数据    
    if ([self.delegate respondsToSelector:@selector(client:handleStreamEventHasSpaceAvailable:)]) {
        [self.delegate client:self handleStreamEventHasSpaceAvailable:stream];
    }
}
-(void)handleStreamEventErrorOccurred:(NSStream *)stream{//错误
    if (self.state==HBTCPConnectStateConnecting && self.connectCompleteBlock) {
        NSError *lError=[NSError errorWithDomain:@"连接失败" code:HB_TCP_ERROR_CODE_DIDCONNECT userInfo:nil];
        self.connectCompleteBlock(lError,nil);
    }
    [self disconnect];
    if ([self.delegate respondsToSelector:@selector(client:handleStreamEventErrorOccurred:)]) {
        [self.delegate client:self handleStreamEventErrorOccurred:stream];
    }
}
-(void)handleStreamEventEndEncountered:(NSStream *)stream{//inputStream接收到的末尾
    if ([self.delegate respondsToSelector:@selector(client:handleStreamEventEndEncountered:)]) {
        [self.delegate client:self handleStreamEventEndEncountered:stream];
    }
}
#pragma mark - Delegate
#pragma mark - NSStream Delegate
-(void)stream:(NSStream *)aStream handleEvent:(NSStreamEvent)eventCode{
    switch (eventCode) {
        case NSStreamEventNone:
            break;
        case NSStreamEventOpenCompleted:
            [self handleStreamEventOpenCompleted:aStream];
            break;
        case NSStreamEventHasBytesAvailable:
            [self handleStreamEventHasBytesAvailable:aStream];
            break;
        case NSStreamEventHasSpaceAvailable:
            [self handleStreamEventHasSpaceAvailable:aStream];
            break;
        case NSStreamEventErrorOccurred:
            [self handleStreamEventErrorOccurred:aStream];
            break;
        case NSStreamEventEndEncountered:
            [self handleStreamEventEndEncountered:aStream];
            break;
        default:
            break;
    }
    NSLog(@"%@\n%lu",aStream,(unsigned long)eventCode);
}
@end
