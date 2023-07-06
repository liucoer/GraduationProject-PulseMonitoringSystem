#ifndef __SIIC_H
#define __SIIC_H

#include <stdint.h>

#define SDA_IN()                     \
  {                                  \
    GPIOB->MODER &= ~(3 << (7 * 2)); \
    GPIOB->MODER |= 0 << 7 * 2;      \
  } // PB9输入模式
#define SDA_OUT()                    \
  {                                  \
    GPIOB->MODER &= ~(3 << (7 * 2)); \
    GPIOB->MODER |= 1 << 7 * 2;      \
  } // PB9输出模式

/* IO操作函数 */
#define IIC_SCL(x) x ? GPIO_SetBits(GPIOB, GPIO_Pin_6) : GPIO_ResetBits(GPIOB, GPIO_Pin_6)
#define IIC_SDA(x) x ? GPIO_SetBits(GPIOB, GPIO_Pin_7) : GPIO_ResetBits(GPIOB, GPIO_Pin_7)
#define READ_SDA   GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)

/*定义应答状态*/
typedef enum { Nack = 0, Ack = 1 } FunctionalResponse;

void siic_init(void);                      // 初始化IIC的IO口
void siic_start(void);                     // 发送IIC开始信号
void siic_stop(void);                      // 发送IIC停止信号
void siic_write_byte(uint8_t txd);         // IIC发送一个字节
uint8_t siic_read_byte(unsigned char ack); // IIC读取一个字节
uint8_t siic_wait_ack(void);               // IIC等待ACK信号
void siic_ack(void);                       // IIC发送ACK信号
void siic_nack(void);                      // IIC不发送ACK信号
#endif
