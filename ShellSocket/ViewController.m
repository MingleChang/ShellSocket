//
//  ViewController.m
//  ShellSocket
//
//  Created by MingleChang on 16/9/9.
//  Copyright © 2016年 MingleChang. All rights reserved.
//

#import "ViewController.h"
#import "HBTCPClientInstance.h"
#import "protocol.h"
#import "HBTCPHeartRequest.h"
#import "HBTCPApiManager.h"
@interface ViewController ()
- (IBAction)buttonClick:(UIButton *)sender;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    [[HBTCPClientInstance instance]connect:@"111.10.24.47" port:5150];
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
    [[HBTCPApiManager manager]sendRequest:[HBTCPHeartRequest heart] completion:^(NSError *error, id response) {
        NSLog(@"%@",error);
    }];
}
@end
