#include "sys.h"
#include "i2c.h"

int i2c_init(void)
{
	RCC->AHB1ENR 	|= 1 << 3;
	SDA->MODER 		|= 1 << SDA_PIN*2;
	SDA->OTYPER 	|= 0 << SDA_PIN;
	SDA->OSPEEDR 	|= 2 << SDA_PIN*2;
	SDA->PUPDR 		|= 1 << SDA_PIN*2;
	SDA->BSRRH 		|= 1 << SDA_PIN;
	
	SCL->MODER 		|= 1 << SCL_PIN*2;
	SCL->OTYPER 	|= 0 << SCL_PIN;
	SCL->OSPEEDR 	|= 2 << SCL_PIN*2;
	SCL->PUPDR 		|= 1 << SCL_PIN*2;
	SCL->BSRRL 		|= 1 << SCL_PIN;
	
	SDA_OUT = 1;
	SCL_OUT = 1;
	
	return 0;
}

void i2c_start(void)
{
	SDA_OUT = 0;
	delay_ms(I2C_DELAY);
	SCL_OUT = 0;
	delay_ms(I2C_DELAY);
}

void i2c_stop(void)
{
	SCL_OUT = 1;
	delay_ms(I2C_DELAY);
	SDA_OUT = 1;
	delay_ms(I2C_DELAY);
}

int i2c_write_byte(const u8 data)
{
	u8 time = 8;
	u8 data_temp = data;
	while(time--)
	{
		SDA_OUT = (data_temp&0x80)>>7;
		delay_ms(I2C_DELAY);
		SCL_OUT = 1;
		delay_ms(I2C_DELAY);
		SCL_OUT = 0;
		delay_ms(I2C_DELAY);
		SDA_OUT = 0;
		data_temp = data_temp << 1;
	}
	return 1;
}


int i2c_write( u8 addr,const u8 *data, u8 is_ack, u16 length)
{
	u16 i = 0;
	i2c_start();
	i2c_write_byte(addr << 1); //7-bit地址传入
	//配置SDA为input
	//读取ACK
	//把SDA还原为output
	if(is_ack)
	{
		SDA->MODER &= ~(3 << SDA_PIN*2);
		SDA->PUPDR |= ~(1 << SDA_PIN*2);
		SCL_OUT = 1;
		delay_ms(I2C_DELAY);		
		if(SDA_IN == 1) ;//return 0;  //NO ACK
		SCL_OUT = 0;
		SDA->MODER |= 1 << SDA_PIN*2;
		SDA->PUPDR |= 1 << SDA_PIN*2;
	}
	while(length--)
	{
		i2c_write_byte(data[i]);
		if(is_ack)
		{
			SDA->MODER &= ~(3 << SDA_PIN*2);
			SDA->PUPDR |= ~(1 << SDA_PIN*2);
			SCL_OUT = 1;
			delay_ms(I2C_DELAY);		
			if(SDA_IN == 1) ;//return 0;  //NO ACK
			SCL_OUT = 0;
			SDA->MODER |= 1 << SDA_PIN*2;
			SDA->PUPDR |= 1 << SDA_PIN*2;
		}
		i++;
	}
	i2c_stop();
	return 1;
}


