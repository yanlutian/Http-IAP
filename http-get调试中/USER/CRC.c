#include "CRC.h"

/*******************************************************************************
* Function Name  : crc16
* Description	 : Configures the different system clocks.
* Input 		 : *puchMsg(数据地址)    usDataLen
* Return		 : 2 byte   
*******************************************************************************/
uint16_t CRC16(volatile uint8_t *puchMsg, uint16_t usDataLen)
{
	uint8_t uchCRCHi = 0xFF ; /* 高CRC字节初始化 */
	uint8_t uchCRCLo = 0xFF ; /* 低CRC 字节初始化 */
	uint32_t uIndex ; /* CRC循环中的索引 */
	while (usDataLen--) /* 传输消息缓冲区 */
	{
		uIndex = uchCRCLo ^ *puchMsg++;
		uchCRCLo = uchCRCHi ^ auchCRCHi[uIndex];
		uchCRCHi = auchCRCLo[uIndex];
	}
   return (uchCRCHi << 8 | uchCRCLo) ;
}

uint16_t slaveReturnCRC(uint8_t slaveAddr, uint8_t functionCode, uint8_t bytesCnt,  uint8_t *puchMsg, uint16_t usDataLen)
{
	uint8_t uchCRCHi = 0xFF ; /* 高CRC字节初始化 */
	uint8_t uchCRCLo = 0xFF ; /* 低CRC 字节初始化 */
	uint32_t uIndex ; /* CRC循环中的索引 */
	uint16_t i = 0;

	uIndex = uchCRCLo ^ slaveAddr;
	uchCRCLo = uchCRCHi ^ auchCRCHi[uIndex];
	uchCRCHi = auchCRCLo[uIndex];

	uIndex = uchCRCLo ^ functionCode;
	uchCRCLo = uchCRCHi ^ auchCRCHi[uIndex];
	uchCRCHi = auchCRCLo[uIndex];

	uIndex = uchCRCLo ^ bytesCnt;
	uchCRCLo = uchCRCHi ^ auchCRCHi[uIndex];
	uchCRCHi = auchCRCLo[uIndex];
	
	while (usDataLen--) /* 传输消息缓冲区 */
	{
		uIndex = uchCRCLo ^ *(puchMsg + i + 1);//大小端问题
		uchCRCLo = uchCRCHi ^ auchCRCHi[uIndex];
		uchCRCHi = auchCRCLo[uIndex];
		usDataLen--;
		uIndex = uchCRCLo ^ *(puchMsg + i );
		uchCRCLo = uchCRCHi ^ auchCRCHi[uIndex];
		uchCRCHi = auchCRCLo[uIndex];
		i = i + 2;

	}
   return (uchCRCHi << 8 | uchCRCLo) ;
}
