#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
#include <stm32f4xx.h>

/****************spi引脚定义*******************/
#define SpiFlash_nCs 	GPIO_Pin_12
#define SpiFlash_CLK 	GPIO_Pin_13
#define SpiFlash_MISO 	GPIO_Pin_14
#define SpiFlash_MOSI 	GPIO_Pin_15


#define SpiFlash_CS(x)		(x)?(GPIOB->BSRRL = SpiFlash_nCs):(GPIOB->BSRRH = SpiFlash_nCs)

 
						  	    													  
void SPI2_Init(void);			 //初始化SPI2口
u16 SPI2_ReadWriteByte(u16 byte);//SPI21总线读写一个字节
		 
#endif

