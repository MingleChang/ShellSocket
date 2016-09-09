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
    [[HBTCPClientInstance instance]connect:@"172.16.81.57" port:10001];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)buttonClick:(UIButton *)sender {
    NSString *lString=@"HelloWorld";
    [[HBTCPClientInstance instance]sendData:[lString dataUsingEncoding:NSUTF8StringEncoding]];
//    [[HBTCPClientInstance instance]disconnect];
}
@end
