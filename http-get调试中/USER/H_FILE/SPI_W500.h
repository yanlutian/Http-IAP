#ifndef __SPIW500_H
#define __SPIW500_H 

#include <stdint.h>
#include "stm32f4xx.h"

/**
 * W5500SPI硬件配置
 */
void W5500SPIHardWareConfig(void);

void W5500HardReset(void);
/**
 * 内部函数，同SPI发送一个字节并返回接收的一个字节    
 * @param  tData 要发送的字节 
 * @return    返回的字节
 */
uint8_t _SPI_SendByte(uint8_t tData);
 void _SPI_SetCS(void);
 void _SPI_ResetCS(void);
#endif

