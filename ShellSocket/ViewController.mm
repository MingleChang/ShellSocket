//
//  ViewController.m
//  ShellSocket
//
//  Created by MingleChang on 16/9/9.
//  Copyright © 2016年 MingleChang. All rights reserved.
//

#import "ViewController.h"
#import "protocol.h"
#import "HBTCPHeartRequest.h"
#import "HBTCPRegisterRequest.h"
#import "HBTCPApiManager.h"
#import "HBTCPClient.h"

#include <iostream>
#include <pthread.h>
#include "protocol.h"
#include "utils.h"

#include "client.h"
#include "protocol.h"
#include "utils.h"
@interface ViewController ()
- (IBAction)buttonClick:(UIButton *)sender;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
//    [[HBTCPClientInstance instance]connect:@"111.10.24.47" port:5150];
//    [[HBTCPApiManager manager].client connect:@"111.10.24.47" port:5150];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)buttonClick:(UIButton *)sender {
//    MHeartbeat msg;
//    memset(&msg,0x00,sizeof(MHeartbeat));
//    SetMsgType(&msg.h, MSGTYPE_HEARTBEAT);
//    SetPtl(&msg.h,1);
//    msg.h.tid = htonl(1);
//    //    NSString *lString=@"HelloWorld";
//    NSMutableData *lData=[NSMutableData data];
//    unsigned int a=3;
//    a=a<<2;
//    a=a+1;
//    [lData appendBytes:&a length:1];
//    unsigned int b=htonl(1);
//    [lData appendBytes:&b length:4];
//    unsigned int c=0;
//    [lData appendBytes:&c length:4];
//    [[HBTCPClientInstance instance]sendData:lData];
//    [[HBTCPClientInstance instance]disconnect];
    

//    [[HBTCPClientInstance instance]sendData:[lHeart package]];
    
    //d33b7049c9f94a7487bc90f929da78e0
    //e8d12b7c41d433396d511ca3618ae2d3
    
//    string m_szUserId="d33b7049c9f94a7487bc90f929da78e0";
//    string m_szToken="e8d12b7c41d433396d511ca3618ae2d3";
//    MRegister msg;
//    memset(&msg,0x00,sizeof(MRegister));
//    SetMsgType(&msg.h, MSGTYPE_REGISTER);
//    SetPtl(&msg.h,1);
//    msg.h.tid = htonl(1);
//    msg.adh.len = htonl(65);
//    msg.plat = 0x02;
//    strncpy((char *)msg.id,m_szUserId.c_str(),32);
//    strncpy((char *)msg.token,m_szToken.c_str(),32);
//    
//    NSData *lData=[NSData dataWithBytes:&msg length:sizeof(MRegister)];
//    [[HBTCPClientInstance instance]sendData:lData];
    
    [[HBTCPApiManager manager]loginWithUserId:@"d33b7049c9f94a7487bc90f929da78e0" andToken:@"e8d12b7c41d433396d511ca3618ae2d3" completion:^(NSError *error, id response) {
        NSLog(@"Register:%@",error);
    }];
    
//    [[HBTCPApiManager manager].client connect:@"111.10.24.47" port:5150 completion:^(NSError *error, id response) {
//        if (error) {
//            return;
//        }
//        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.5 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
//            HBTCPRegisterRequest *lRegister=[HBTCPRegisterRequest registerWithUserId:@"d33b7049c9f94a7487bc90f929da78e0" andToken:@"e8d12b7c41d433396d511ca3618ae2d3"];
//            [[HBTCPApiManager manager]sendRequest:lRegister completion:^(NSError *error, id response) {
//                NSLog(@"Register:%@",error);
//            }];
//        });
//    }];
    
    
    
//    [[HBTCPApiManager manager]sendRequest:[HBTCPHeartRequest heart] completion:^(NSError *error, id response) {
//        NSLog(@"%@",error);
//    }];
}
@end
