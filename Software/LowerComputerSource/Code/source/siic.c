#include "siic.h"
#include "delay.h"
#include "stm32f4xx_conf.h"

void siic_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    /* 时钟配置 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    /* GPIO设置 */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Medium_Speed;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    /* 引脚上拉 */
    IIC_SCL(1);
    IIC_SDA(1);
}

void siic_start(void)
{
    SDA_OUT();
    IIC_SDA(1);
    IIC_SCL(1);
    delay_us(4);
    IIC_SDA(0);
    delay_us(4);
    IIC_SCL(0); // 钳住I2C总线，准备发送或接收数据
}

void siic_stop(void)
{
    SDA_OUT();
    IIC_SCL(0);
    IIC_SDA(0);
    delay_us(4);
    IIC_SCL(1);
    IIC_SDA(1); // 发送I2C总线结束信号
    delay_us(4);
}

uint8_t siic_wait_ack(void)
{
    uint8_t ucErrTime = 0;
    SDA_IN();
    IIC_SDA(1);
    delay_us(1);
    IIC_SCL(1);
    delay_us(1);
    while(READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime > 250)
        {
            siic_stop();
            return 1;
        }
    }
    IIC_SCL(0);
    return 0;
}

void siic_ack(void)
{
    IIC_SCL(0);
    SDA_OUT();
    IIC_SDA(0);
    delay_us(2);
    IIC_SCL(1);
    delay_us(2);
    IIC_SCL(0);
}

void siic_nack(void)
{
    IIC_SCL(0);
    SDA_OUT();
    IIC_SDA(1);
    delay_us(2);
    IIC_SCL(1);
    delay_us(2);
    IIC_SCL(0);
}

void siic_write_byte(uint8_t txd)
{
    uint8_t t;
    SDA_OUT();
    IIC_SCL(0); // 拉低时钟开始数据传输
    for(t = 0; t < 8; t++)
    {
        IIC_SDA((txd & 0x80) >> 7);
        txd <<= 1;
        delay_us(2);
        IIC_SCL(1);
        delay_us(2);
        IIC_SCL(0);
        delay_us(2);
    }
}

uint8_t siic_read_byte(unsigned char ack)
{
    unsigned char i, receive = 0;
    SDA_IN(); // SDA设置为输入
    for(i = 0; i < 8; i++)
    {
        IIC_SCL(0);
        delay_us(2);
        IIC_SCL(1);
        receive <<= 1;
        if(READ_SDA) receive++;
        delay_us(1);
    }
    if(!ack) siic_nack();
    else siic_ack();
    return receive;
}
