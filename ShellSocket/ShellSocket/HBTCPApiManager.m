//
//  HBTCPApiManager.m
//  ShellSocket
//
//  Created by MingleChang on 16/9/13.
//  Copyright © 2016年 MingleChang. All rights reserved.
//

#import "HBTCPApiManager.h"
#import "HBExternKey.h"
#import "HBTCPResponse.h"
#import "HBTCPClient.h"
#import "HBTCPRegisterRequest.h"

@interface HBTCPApiManager ()<HBTCPClientDelegate>

@property(nonatomic,strong,readwrite)HBTCPClient *client;
@property(nonatomic,strong,readwrite)NSMutableDictionary *requestDic;

@end

@implementation HBTCPApiManager

+(HBTCPApiManager *)manager{
    static HBTCPApiManager *manager;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        manager=[[HBTCPApiManager alloc]init];
    });
    return manager;
}

-(instancetype)init{
    self=[super init];
    if (self) {
        self.client=[[HBTCPClient alloc]init];
        self.client.delegate=self;
        self.requestDic=[NSMutableDictionary dictionary];
    }
    return self;
}

#pragma mark - Public

-(void)loginWithUserId:(NSString *)userId andToken:(NSString *)token completion:(completeBlock)complete{
    [[HBTCPApiManager manager].client connect:HB_TCP_HOST port:5150 completion:^(NSError *error, id response) {
        if (error) {
            if (complete) {
                complete(error,response);
            }
        }
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.5 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            HBTCPRegisterRequest *lRegister=[HBTCPRegisterRequest registerWithUserId:userId andToken:token];
            [[HBTCPApiManager manager]sendRequest:lRegister completion:^(NSError *error, id response) {
                if (complete) {
                    complete(error,response);
                }
            }];
        });
    }];
}

-(void)sendRequest:(HBTCPHeader *)request completion:(completeBlock)complete{
    request.complete=complete;
    NSNumber *lKey=@(request.tid);
    [self.requestDic setObject:request forKey:lKey];
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(30 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        if ([self.requestDic.allKeys containsObject:lKey]){
            NSError *lError=[NSError errorWithDomain:@"连接超时" code:HB_TCP_ERROR_CODE_TIMEOUT userInfo:nil];
            if (request.complete) {
                request.complete(lError,nil);
            }
            [self.requestDic removeObjectForKey:lKey];
        }
    });
    [self.client sendData:[request package]];
}

-(void)receiveHasKeyWith:(HBTCPResponse *)response{
    switch (response.type) {
        case HB_TCP_MSGTYPE_REGRESP:
        case HB_TCP_MSGTYPE_HBRESP:{
            HBTCPHeader *request=[self.requestDic objectForKey:@(response.tid)];
            uint8_t code;
            [response.data getBytes:&code length:1];
            if (code==0) {
                if (request.complete) {
                    request.complete(nil,nil);
                }
            }else{
                NSError *lError=[NSError errorWithDomain:@"失败" code:code userInfo:nil];
                if (request.complete) {
                    request.complete(lError,nil);
                }
            }
            [self.requestDic removeObjectForKey:@(response.tid)];
        }break;
            
        default:
            break;
    }
}

-(void)receiveNoHasKeyWith:(HBTCPResponse *)response{
    switch (response.type) {
        case HB_TCP_MSGTYPE_KICK:{
            [[NSNotificationCenter defaultCenter]postNotificationName:HB_TCP_NOTIFICATION_KICK object:response];
        }break;
            
        default:
            break;
    }
}

-(void)receiveReponse:(HBTCPResponse *)response{
    if ([self.requestDic.allKeys containsObject:@(response.tid)]) {
        [self receiveHasKeyWith:response];
    }else{
        [self receiveNoHasKeyWith:response];
    }
}

#pragma mark - Delegate
#pragma mark - HBTCPClient Delegate
-(void)client:(HBTCPClient *)client handleStreamEventOpenCompleted:(NSStream *)stream{
    
}
-(void)client:(HBTCPClient *)client handleStreamEventHasBytesAvailable:(NSStream *)stream{
    if (stream) {
        NSInputStream *inputStream=(NSInputStream *)stream;
        HBTCPResponse *lResponse=[HBTCPResponse responseWith:inputStream];
        [self receiveReponse:lResponse];
    }
}
-(void)client:(HBTCPClient *)client handleStreamEventHasSpaceAvailable:(NSStream *)stream{
    NSOutputStream *outputStream=(NSOutputStream *)stream;
}
-(void)client:(HBTCPClient *)client handleStreamEventErrorOccurred:(NSStream *)stream{
    
}
-(void)client:(HBTCPClient *)client handleStreamEventEndEncountered:(NSStream *)stream{
    
}
@end
