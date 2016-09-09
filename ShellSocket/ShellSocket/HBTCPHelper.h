//
//  HBTCPHelper.h
//  ShellSocket
//
//  Created by MingleChang on 16/9/9.
//  Copyright © 2016年 MingleChang. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface HBTCPHelper : NSObject
+ (void)getStreamsToHostNamed:(NSString *)hostName port:(NSInteger)port inputStream:(NSInputStream **)inputStream outputStream:(NSOutputStream **)outputStream;
@end
