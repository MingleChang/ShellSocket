//
//  HBTCPResponse.h
//  ShellSocket
//
//  Created by MingleChang on 16/9/12.
//  Copyright © 2016年 MingleChang. All rights reserved.
//

#import "HBTCPHeader.h"

@interface HBTCPResponse : HBTCPHeader

@property(nonatomic,assign)NSInteger length;
@property(nonatomic,copy)NSData *data;

+(HBTCPResponse *)responseWith:(NSInputStream *)inputStream;

@end
