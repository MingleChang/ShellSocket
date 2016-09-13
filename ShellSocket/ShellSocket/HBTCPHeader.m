//
//  HBTCPHeader.m
//  ShellSocket
//
//  Created by MingleChang on 16/9/12.
//  Copyright © 2016年 MingleChang. All rights reserved.
//

#import "HBTCPHeader.h"

static NSUInteger tid=1;

@implementation HBTCPHeader

+(NSUInteger)nextTid{
    return tid++;
}

#pragma mark - HBTCPProtocol
-(NSMutableData *)package{
    NSMutableData *lData=[NSMutableData data];
    unsigned int header=self.type;
    header=header<<2;
    header=header+1;
    [lData appendBytes:&header length:1];
    unsigned int tid=htonl(self.tid);
    [lData appendBytes:&tid length:4];
    return lData;
}



@end
