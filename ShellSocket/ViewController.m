//
//  ViewController.m
//  ShellSocket
//
//  Created by MingleChang on 16/9/9.
//  Copyright © 2016年 MingleChang. All rights reserved.
//

#import "ViewController.h"
#import "HBTCPClientInstance.h"

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
//    NSString *lString=@"HelloWorld";
    NSMutableData *lData=[NSMutableData data];
    unsigned int a=2;
    a=a<<2;
    a=a+1;
    [lData appendBytes:&a length:1];
    unsigned int b=1;
    [lData appendBytes:&b length:4];
//    unsigned int c=0;
//    [lData appendBytes:&c length:4];
    [[HBTCPClientInstance instance]sendData:lData];
//    [[HBTCPClientInstance instance]disconnect];
}
@end
