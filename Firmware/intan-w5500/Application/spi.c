#include "spi.h"


void SPI2_Init(void)
{   
	  SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
  
    //clk.miso.mosi
    GPIO_InitStructure.GPIO_Pin = SpiFlash_CLK | SpiFlash_MISO | SpiFlash_MOSI;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2);  //¸´ÓÃ
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2);
	  //CS
	  GPIO_InitStructure.GPIO_Pin =   SpiFlash_nCs ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
 
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;   
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;   
    SPI_InitStructure.SPI_CPOL  = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;   
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2,&SPI_InitStructure);
    SPI_Cmd(SPI2,ENABLE);
}


/************SPI2¶ÁÐ´º¯Êý***************/
u16 SPI2_ReadWriteByte(u16 byte)
{		 			 
	
	 while((SPI2->SR&(1<<1))==0);           

	 SPI2->DR=byte;                 

	 while((SPI2->SR&(1<<0))==0);                                                           

	 return SPI2->DR;         
 		    
}








