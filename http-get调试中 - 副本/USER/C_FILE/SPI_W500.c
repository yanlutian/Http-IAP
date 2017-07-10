
/***    
        首先得关闭防火墙
        W5500做TCP Client: W5500HardWareConfig => W5500Init => W5500SetSocketPort => W5500SetRomoteIPAndPort => W5500TCPClientConnect 
        W5500做TCP Server: W5500HardWareConfig => W5500Init => W5500SetSocketPort => W5500TCPServerListen
        W5500做UDP       ：W5500HardWareConfig => W5500Init => W5500SetSocketPort => W5500SetRomoteIPAndPort => W5500SetSocketUDP

***********/

#include "SPI_W500.h"
#include "delay.h"

/*
    MISO:PA6 MOSI:PA7 SCK:PA5 NSS:PEO NRST:PE1 NINT:PB7 SPI1
 */
//#define W5500_INT_CLK   RCC_AHB1Periph_GPIOB
//#define W5500_INT_PIN   GPIO_Pin_7
//#define W5500_INT_GPIO  GPIOB

//#define W5500_INT_EXT_GPIO EXTI_PortSourceGPIOB
//#define W5500_INT_EXT_PINSOURCE EXTI_PinSource7
//#define W5500_INT_EXT_LINE      EXTI_Line7
//#define W5500_INT_EXT_IRQ      EXTI0_IRQn
//#define W5500_INT_EXT_IRQHandler  EXTI0_IRQHandler
//复位
#define W5500_RST_CLK   RCC_AHB1Periph_GPIOE
#define W5500_RST_PIN   GPIO_Pin_1
#define W5500_RST_GPIO  GPIOE
//#define W5500_RST_CLK   RCC_AHB1Periph_GPIOE
//#define W5500_RST_PIN   GPIO_Pin_1
//#define W5500_RST_GPIO  GPIOE
//主出从入
#define W5500_SPI_MOSI_CLK  RCC_AHB1Periph_GPIOA
#define W5500_SPI_MOSI_PIN  GPIO_Pin_7
#define W5500_SPI_MOSI_PINSOURCE    GPIO_PinSource7
#define W5500_SPI_MOSI_GPIO GPIOA
//主入从出
#define W5500_SPI_MISO_CLK  RCC_AHB1Periph_GPIOA
#define W5500_SPI_MISO_PIN  GPIO_Pin_6
#define W5500_SPI_MISO_PINSOURCE    GPIO_PinSource6
#define W5500_SPI_MISO_GPIO GPIOA
//时钟
#define W5500_SPI_SCLK_CLK  RCC_AHB1Periph_GPIOA
#define W5500_SPI_SCLK_PIN  GPIO_Pin_5
#define W5500_SPI_SCLK_PINSOURCE    GPIO_PinSource5
#define W5500_SPI_SCLK_GPIO GPIOA
//片选
#define W5500_SPI_NSS_CLK  RCC_AHB1Periph_GPIOE
#define W5500_SPI_NSS_PIN  GPIO_Pin_0
#define W5500_SPI_NSS_PINSOURCC    GPIO_PinSource0
#define W5500_SPI_NSS_GPIO GPIOE
//#define W5500_SPI_NSS_CLK  RCC_AHB1Periph_GPIOE
//#define W5500_SPI_NSS_PIN  GPIO_Pin_0
//#define W5500_SPI_NSS_PINSOURCE    GPIO_PinSource0
//#define W5500_SPI_NSS_GPIO GPIOE

#define W5500_SET_CS  GPIO_SetBits(W5500_SPI_NSS_GPIO, W5500_SPI_NSS_PIN)
#define W5500_RESET_CS GPIO_ResetBits(W5500_SPI_NSS_GPIO, W5500_SPI_NSS_PIN)

#define W5500_SPI  SPI1
#define W5500_SPI_CLK RCC_APB2Periph_SPI1
#define W5500_SPI_AF GPIO_AF_SPI1

static uint8_t _Read_W5500(uint16_t addr);
static void _Write_W5500(uint16_t addr, uint8_t dat);
void W5500HardReset(void);



/**
 * W5500SPI硬件配置
 */
void W5500SPIHardWareConfig(void)
{
    GPIO_InitTypeDef    GPIO_InitStruct;
    SPI_InitTypeDef     SPI_InitStructure;  
    EXTI_InitTypeDef   EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
//    RCC_AHB1PeriphClockCmd(W5500_INT_CLK,  ENABLE); 
    RCC_AHB1PeriphClockCmd(W5500_RST_CLK,  ENABLE); 
    RCC_AHB1PeriphClockCmd(W5500_SPI_MOSI_CLK,  ENABLE);    
    RCC_AHB1PeriphClockCmd(W5500_SPI_MISO_CLK,  ENABLE);    
    RCC_AHB1PeriphClockCmd(W5500_SPI_SCLK_CLK,  ENABLE);    
    RCC_APB2PeriphClockCmd(W5500_SPI_CLK,  ENABLE); 
    RCC_AHB1PeriphClockCmd(W5500_SPI_NSS_CLK,  ENABLE); 
        //AF
    GPIO_PinAFConfig(W5500_SPI_MOSI_GPIO, W5500_SPI_MOSI_PINSOURCE, W5500_SPI_AF);
    GPIO_PinAFConfig(W5500_SPI_MISO_GPIO, W5500_SPI_MISO_PINSOURCE, W5500_SPI_AF);
    GPIO_PinAFConfig(W5500_SPI_SCLK_GPIO, W5500_SPI_SCLK_PINSOURCE, W5500_SPI_AF);
    // GPIO_PinAFConfig(W5500_SPI_NSS_GPIO, W5500_SPI_NSS_PIN, W5500_SPI_AF);
    // 
    //INT
//    GPIO_StructInit(&GPIO_InitStruct);
//    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
//    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
//    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStruct.GPIO_Pin = W5500_INT_PIN;
//    GPIO_Init(W5500_INT_GPIO, &GPIO_InitStruct);
    //RST
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Pin = W5500_RST_PIN;
    GPIO_Init(W5500_RST_GPIO, &GPIO_InitStruct);

    // //NSS
    GPIO_InitStruct.GPIO_Pin =  W5500_SPI_NSS_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;             
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;           
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;          
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;   
    GPIO_Init(W5500_SPI_NSS_GPIO, &GPIO_InitStruct);
    //MOSI
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Pin = W5500_SPI_MOSI_PIN ;
    GPIO_Init(W5500_SPI_MOSI_GPIO, &GPIO_InitStruct);
    //MISO
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Pin = W5500_SPI_MISO_PIN ;
    GPIO_Init(W5500_SPI_MISO_GPIO, &GPIO_InitStruct);
    //CLK
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Pin = W5500_SPI_SCLK_PIN ;
    GPIO_Init(W5500_SPI_SCLK_GPIO, &GPIO_InitStruct);

    //SPI
    SPI_I2S_DeInit(W5500_SPI);
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_Init(W5500_SPI, &SPI_InitStructure);

    SPI_Cmd(W5500_SPI, ENABLE);

    /**  管脚中断*/
    // SYSCFG_EXTILineConfig(W5500_INT_EXT_GPIO,W5500_INT_EXT_PINSOURCE);
    // EXTI_InitStructure.EXTI_Line = W5500_INT_EXT_LINE;
    // EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    // EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    // EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    // EXTI_Init(&EXTI_InitStructure);


    // NVIC_InitStructure.NVIC_IRQChannel = W5500_INT_EXT_IRQ;
    // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    // NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    // NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    // NVIC_Init(&NVIC_InitStructure);

      // GPIO_WriteBit(GPIOA, GPIO_Pin_15, (BitAction)(0));
    GPIO_ResetBits(W5500_SPI_NSS_GPIO,W5500_SPI_NSS_PIN);
    W5500HardReset();



}


/**
 * 5200硬件复位
 */
void W5500HardReset(void)
{
    GPIO_ResetBits(W5500_RST_GPIO,W5500_RST_PIN);
    //用软件定时器延时100ms
    delay_ms( 100);

    GPIO_SetBits(W5500_RST_GPIO,W5500_RST_PIN);
    //用软件定时器延时100ms
    delay_ms( 100);
}

/**
 * [prvSpiSetCS description] set cs single
 */
__inline void _SPI_SetCS(void)
{                                            
//    delay_ms( 10);
    W5500_SET_CS;
}

/**
 * [prvSpiResetCS description] reset cs single
 */
__inline void _SPI_ResetCS(void)
{
    W5500_RESET_CS;
//    delay_ms( 10);
}



/**
 * 内部函数，同SPI发送一个字节并返回接收的一个字节    
 * @param  tData 要发送的字节 
 * @return    返回的字节
 */
uint8_t _SPI_SendByte(uint8_t tData)
{
    while(SPI_I2S_GetFlagStatus(W5500_SPI, SPI_I2S_FLAG_TXE) == RESET); 
    SPI_I2S_SendData(W5500_SPI, tData); 
    while(SPI_I2S_GetFlagStatus(W5500_SPI, SPI_I2S_FLAG_RXNE) == RESET); 
    return SPI_I2S_ReceiveData(W5500_SPI); 
}

/**
 * 读5200指定地址的寄存器数据
 * @param  addr 寄存器地址
 * @return      读到的数据
 */
static uint8_t _Read_W5500(uint16_t addr)
{
    uint8_t i;

    _SPI_SendByte(addr>>8); //发送地址高位
    _SPI_SendByte(addr);     //发送地址低位        
    _SPI_SendByte(0x00);     //发送读命令
    _SPI_SendByte(0x01);     //发送读数据的长度
    i = _SPI_SendByte(0);      //发送一个哑数据并读取数据

    return i;
}

/**
 * 内部函数，读W5500的寄存器内容
 * @param  addr 要读的寄存器的地址
 * @param  buff 存放的位置
 * @param  len  长度
 * @return      返回数据长度
 */
static uint16_t _ReadBuff_W5500(uint16_t addr, uint8_t* buff,uint16_t len)
{
    uint16_t i = 0;
        
    _SPI_SendByte(addr >> 8); //发送地址高位
    _SPI_SendByte(addr);     //发送地址低位   
    i = len&0x7fff;         
    _SPI_SendByte(i >> 8);     //发送读命令及需要读的数据长度
    _SPI_SendByte(i);         

    for(i=0; i<len; i++)             
    {
        buff[i] = _SPI_SendByte(0x00);//发送一个哑数据并读取数据
        
    }

    return len;
}

/**
 * 内部函数，给W5500的一个寄存器写入数据
 * @param addr 地址
 * @param dat  数据
 */
static void _Write_W5500(uint16_t addr, uint8_t dat)
{
    _SPI_SendByte(addr >> 8); //发送地址高位
    _SPI_SendByte(addr);      //发送地址低位
    _SPI_SendByte(0x80);     //发送写命令 
    _SPI_SendByte(0x01);     //发送要写的数据长度
    _SPI_SendByte(dat);      //写入数据
}

/**
 * 内部函数，给5200寄存器连续写入数据
 * @param  addr 地址
 * @param  buff 要写的数据的地址
 * @param  len  长度
 * @return      返回写入的长度
 */
static uint16_t _WriteBuff_W5500(uint16_t addr,uint8_t* buff,uint16_t len)
{
    uint16_t i = 0;
      
    _SPI_SendByte(addr >> 8); //发送地址高位
    _SPI_SendByte(addr);      //发送地址低位 
    i = len|0x8000;         
    _SPI_SendByte(i >> 8);     //发送写命令及要写的数据长度
    _SPI_SendByte(i);        

    for(i=0; i<len; i++)    
    {       
        _SPI_SendByte(buff[i]);//写入数据
    }
    
    return len; 
}


