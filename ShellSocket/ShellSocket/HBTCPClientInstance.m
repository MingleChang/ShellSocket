//
//  HBTCPClientInstance.m
//  ShellSocket
//
//  Created by MingleChang on 16/9/9.
//  Copyright © 2016年 MingleChang. All rights reserved.
//

#import "HBTCPClientInstance.h"
#import "HBTCPHelper.h"
@interface HBTCPClientInstance ()<NSStreamDelegate>

@property(nonatomic,strong)NSInputStream *inputStream;
@property(nonatomic,strong)NSOutputStream *outputStream;

@end

@implementation HBTCPClientInstance

+(HBTCPClientInstance *)instance
{
    static HBTCPClientInstance *manager;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        manager = [[HBTCPClientInstance alloc] init];
    });
    return manager;
}

-(instancetype)init{
    self=[super init];
    if (self) {
        
    }
    return self;
}

#pragma mark - Public 
-(void)connect:(NSString *)host port:(NSInteger)port{
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
    
}
-(void)handleStreamEventHasBytesAvailable:(NSStream *)stream{//接收数据
    if (stream) {
        NSInputStream *inputStream=(NSInputStream *)stream;
        uint8_t buf[1024];
        NSInteger len = 0;
        len = [inputStream read:buf maxLength:1024];
        if (len>0) {
            NSData *lData=[NSData dataWithBytes:buf length:len];
            NSLog(@"%@",lData);
        }
    }
}
-(void)handleStreamEventHasSpaceAvailable:(NSStream *)stream{//发送数据
    
}
-(void)handleStreamEventErrorOccurred:(NSStream *)stream{//错误
//    [self disconnect];
}
-(void)handleStreamEventEndEncountered:(NSStream *)stream{//inputStream接收到的末尾
    
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
