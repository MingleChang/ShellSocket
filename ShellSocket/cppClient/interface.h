/*
* Copyright 2016-2099 Galo QiDa,ChongQing Inc.
* Auhor:ligang axengine@163.com
* push客户端接口文件定义
*/
#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include <iostream>

using namespace std;

/*业务通知函数
* Type:消息类型
*	-1:网络异常
* tid:事务ID
* msg:消息内容
* code:请求消息返回码
* param:附加参数，如类型等
*/
typedef void (*NOTIFY)(int type,int tid,const char *msg,int code,int param);

/* Init
* 初始化一个PUSH客户端
*/
void Init();

/* SetUserId
* 设置用户ID，用户ID长度需小于或等于32字节
*/
void SetUserId(const string userid);

/* SetToken
* 设置token，token长度需小于或等于32字节
*/
void SetToken(const string token);

/* SetTermtype
* 设置终端类型 termtype为枚举值
* 1:android 2:ios 4:winphone 8:web 16:pcclient
*/
void SetTermtype(const int termtype );


/* SetServer
*  设置服务器地址
*  addr:ip或者域名
*  port:端口
*/
void SetServer(const string doamin,int port);

/* SetNotify
* 设置回调函数 当有事件发生时调用此回调函数
* NOTIFY 参考 NOTIFY 定义
*/
void SetNotify(NOTIFY func);

/* Start
* 启动push连接服务
* 如果启动失败，表示无法连接到服务器或者系统内部错误
* 返回值:0-成功 -1:失败
*/
int Start();

/* ReStart
* 重启push连接服务 通常由于socket异常后可手动重启连接
* 如果启动失败，表示无法连接到服务器或者系统内部错误
* 返回值:0-成功 -1:失败
*/
int ReStart();

/* CreateClass
* 创建课程 讲师调用
*/
int CreateClass(string classId);

/* DropClass
*  结束课程 讲师调用
*/
int DropClass(string classId);

/* EnterRoom
*  进入教室
*/
int EnterRoom(string classId);

/* ExitClass
*  退出教室
*/
int ExitClass(string classId);

/* Message
* 发送即时消息
* 入参:消息内容 只支持字符串形式
* 如果发送失败 通常是socket连接异常 建议操作:ReStart
* 返回值:0-成功 -1:失败
*/
int Message(const char *content);

/* Destroy
* 销毁push客户端，当不再需要push服务时调用此函数 
* 断开socket连接 退出心跳服务和读写线程 
* 返回值:N/A
*/
void Destroy();

/*
 * 检测TCP端口是否活跃
 * 入参:对端地址,端口,超时时长秒 地址可以为域名
 * 返回值:0-成功 -1:不可用
 */
int CheckTcpPort(const char *hostname,int port,int timeout);

#endif

