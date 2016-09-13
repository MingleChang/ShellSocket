//
//  HBTCPApiManager.m
//  ShellSocket
//
//  Created by MingleChang on 16/9/13.
//  Copyright © 2016年 MingleChang. All rights reserved.
//

#import "HBTCPApiManager.h"
#import "HBTCPClientInstance.h"
#import "HBNotificationKey.h"
#import "HBTCPResponse.h"

@interface HBTCPApiManager ()

@property(nonatomic,strong)NSMutableDictionary *requestDic;

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
-(void)dealloc{
    [[NSNotificationCenter defaultCenter]removeObserver:self];
}
-(instancetype)init{
    self=[super init];
    if (self) {
        self.requestDic=[NSMutableDictionary dictionary];
        [[NSNotificationCenter defaultCenter]addObserver:self selector:@selector(receiveReponseNotification:) name:HB_TCP_NOTIFICATION_RESPONSE object:nil];
    }
    return self;
}

#pragma mark - Public
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
    [[HBTCPClientInstance instance]sendData:[request package]];
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

#pragma mark - Notification
-(void)receiveReponseNotification:(NSNotification *)sender{
    HBTCPResponse *lResponse=(HBTCPResponse *)sender.object;
    if (![lResponse isKindOfClass:[HBTCPResponse class]]) {
        return;
    }
    if ([self.requestDic.allKeys containsObject:@(lResponse.tid)]) {
        [self receiveHasKeyWith:lResponse];
    }else{
        [self receiveNoHasKeyWith:lResponse];
    }
}

@end
