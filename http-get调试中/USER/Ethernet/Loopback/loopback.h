
#ifndef  __LOOPBACK_H_
#define  __LOOPBACK_H_

#include "stm32f4xx.h"	

#define DATA_BUF_SIZE  		 2048//w5500共有16k的接收缓存，假设这里每个socket分得2k，即2048byte

#define RECV_MAX_DATALEN   2048//每个socket平均分配2K缓存的情况下

#define CLIENT_MAXNUM      4//定义客户端数量


//定义需要发送的数据量
#define		SEND_DATA_LEN				8+1			//发送:4个AD值+2个脉冲周期值(us)+1个通道标识
//#define		AD_SAMPLIING_NUM		500			//ad采样频率
#define		CLIENT_NUM					1				//客户端数量


//server
extern volatile uint8_t tcp_server_recvbuf[];   	//发送数据缓存
extern volatile uint16_t tcp_server_recv_len;		//发送缓存长度
extern volatile uint8_t tcp_server_sendbuf[];   	//发送数据缓存	
extern volatile uint16_t tcp_server_send_len;		//发送缓存长度
extern volatile uint8_t tcp_server_sta[CLIENT_MAXNUM];//客户端状态












int32_t loopback_tcps(uint8_t, uint8_t*, uint16_t);
int32_t loopback_udps(uint8_t, uint8_t*, uint16_t);
int32_t loopback_server(uint8_t, uint8_t*, uint16_t);
int32_t loopback_client(uint8_t sn, uint8_t* buf,uint8_t *ip, uint16_t port);




#endif   // _WIZCHIP_LOOPBACK_H_
