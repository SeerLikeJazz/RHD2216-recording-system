#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
#include <stm32f4xx.h>

/****************spi���Ŷ���*******************/
#define SpiFlash_nCs 	GPIO_Pin_12
#define SpiFlash_CLK 	GPIO_Pin_13
#define SpiFlash_MISO 	GPIO_Pin_14
#define SpiFlash_MOSI 	GPIO_Pin_15


#define SpiFlash_CS(x)		(x)?(GPIOB->BSRRL = SpiFlash_nCs):(GPIOB->BSRRH = SpiFlash_nCs)

 
						  	    													  
void SPI2_Init(void);			 //��ʼ��SPI2��
u16 SPI2_ReadWriteByte(u16 byte);//SPI21���߶�дһ���ֽ�
		 
#endif

