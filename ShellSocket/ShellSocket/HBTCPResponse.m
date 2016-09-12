//
//  HBTCPResponse.m
//  ShellSocket
//
//  Created by MingleChang on 16/9/12.
//  Copyright © 2016年 MingleChang. All rights reserved.
//

#import "HBTCPResponse.h"

@implementation HBTCPResponse

+(HBTCPResponse *)responseWith:(NSInputStream *)inputStream{
    HBTCPResponse *lReponse=[[HBTCPResponse alloc]init];
    NSInteger length=0;
    NSUInteger type=0;
    length = [inputStream read:(void *)(&type) maxLength:1];
    lReponse.type=type>>2;
    NSUInteger tid=0;
    length = [inputStream read:(void *)(&tid) maxLength:4];
    lReponse.tid=htonl(tid);
    NSInteger msgLen=0;
    length = [inputStream read:(void *)(&msgLen) maxLength:4];
    lReponse.length=htonl(msgLen);
    uint8_t buffer[lReponse.length];
    length = [inputStream read:buffer maxLength:lReponse.length];
    lReponse.data=[NSData dataWithBytes:buffer length:lReponse.length];
    return lReponse;
}

@end
