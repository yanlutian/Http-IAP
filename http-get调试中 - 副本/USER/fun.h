#ifndef __FUN_H__
#define	__FUN_H__

#include <stdint.h>
#include "string.h"//周期定时、频率测量
#include "loopback.h"//周期定时、频率测量
#include "delay.h"//周期定时、频率测量
#include "CRC.h"//周期定时、频率测量


//调试模式
#define		DEBUG		1

//每次http请求获取的字节数
#define		BYTESPERREQ		1000

//bin文件info信息字节数
#define		BINFILESIZE		10


//http响应等待时间，单位待定
#define		HTTPRESPWAITTIME		50	//100ms

//http发送最大长度
#define		HTTPSENDLENVMAX		500

//http接收最大长度
#define		HTTPRECLENVMAX		500

extern int HttpReqGetData(char* data, uint16_t start,uint16_t end);
extern int HttpDataResUnpack(char* buf1, uint16_t* statr, uint16_t* end, unsigned char* data);
extern int HttpReqGetInfo(char* data);
extern void ibyte2cbyte(char* Des, unsigned char* Res, uint16_t len);
extern void cbyte2ibyte(unsigned char* Des, char* Res, uint16_t len);
extern void ibyte2ibyte(unsigned char* Des, unsigned char* Res, uint16_t len);


extern void HttpDataSend();

extern unsigned int UpdateFileDataGet(unsigned char* UpdateBin);

extern void HttpSendData(char* send_buf,uint16_t WaitTime);


extern unsigned char httpreq[];

extern int HttpInfoResUnpack(char* buf1,unsigned char* data);






#endif // !__FUN_H__
