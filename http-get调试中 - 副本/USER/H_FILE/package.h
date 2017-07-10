#ifndef __PACKAGE__H_
#define __PACKAGE__H_


#define		AS_MASTER_STATION		1//作为主站
#define		AS_SLAVE_STATION		0//作为从站

#define		modbus_master_add				1	//主站地址
#define 	modbus_cmd_Radio				0x00//广播地址
#define 	modbus_cmd_ReadKeepReg	0x03//读保持寄存器
#define		modbus_cmd_WriteOneReg	0x06	//写单个寄存器
#define		modbus_cmd_WriteMultiReg	0x10	//写多个寄存器
#define		modbus_cmd_ReadWriteReg	0x17	//读写多个寄存器



//#define		PLC_ADDRESS_BASE1
//#define		PROTOCOL_ADDRESS_BASE0
//寄存器地址BASE_0 OR BASE_1


//#define		DATA_TYPE_FLOAT


//#ifdef		PLC_ADDRESS_BASE1		//BASE 1
//#define 	REG_ADDR_MIN			1	//寄存器最小地址
//#define 	REG_ADDR_MAX			9 //寄存器最大地址
//#define		BUFFER_OFFSET			2	//寄存器地址偏移
//#else									  	 		//BASE 0

#define		DATABUF_MAXLEN		104//比实际需要的大小+1<modbus协议问题>
#define 	REG_ADDR_MIN			0	//寄存器最小地址
#define 	REG_ADDR_MAX			(REG_ADDR_MIN+DATABUF_MAXLEN-1) //寄存器最大地址

//#define		BUFFER_OFFSET			1 //寄存器地址偏移
//#endif


extern volatile unsigned short int Register_Buffer[DATABUF_MAXLEN];
extern volatile unsigned short int Register_Buffer_Channel1[DATABUF_MAXLEN];
extern volatile unsigned short int Register_Buffer_Channel2[DATABUF_MAXLEN];
extern volatile unsigned short int Register_Buffer_Channel3[DATABUF_MAXLEN];
extern float Register_BufferFloat[10];




int modbus_send( unsigned char *data);
unsigned long int slave_modbus_response( unsigned char *data);

#endif


