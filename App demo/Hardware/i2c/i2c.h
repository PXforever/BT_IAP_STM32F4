#ifndef __I2C_H
#define __I2C_H

#include "delay.h"

//#define SDA_GPIO_PORT GPIOD
//#define SCL_GPIO_PORT GPIOD

#define SDA GPIOD
#define SCL GPIOD
#define SDA_PIN 11
#define SCL_PIN 7

#define I2C_DELAY 1

#define SDA_OUT PDout(SDA_PIN)
#define SCL_OUT PDout(SCL_PIN)

#define SDA_IN PDin(SDA_PIN)

int i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
//-----------------------------------------------------------------------------//
//Function name: i2c_write_byte  i2cд���� 1byte
//data��д����
//return: ....
int i2c_write_byte(const u8 data);
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//Function name: i2c_write  i2cд����
//addr:7-bit ��ַ��data��д�����ݣ�is_ack:�Ƿ�ack length:���ݳ���
//return: ....
int i2c_write( u8 addr,const u8 *data, u8 is_ack, u16 length);
//-----------------------------------------------------------------------------//

#endif 



