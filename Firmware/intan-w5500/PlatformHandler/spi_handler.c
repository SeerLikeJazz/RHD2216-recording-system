#include "stm32f4xx.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_dma.h"

#include "spi_handler.h"
#include "common.h"
#include <string.h>

#include "main.h"

DMA_InitTypeDef DMA_InitStructure;
uint8_t pTmpBuf1[DATA_BUF_SIZE + 3];
uint8_t pTmpBuf2[DATA_BUF_SIZE + 3];

void SPI_Configuration(void)
{
	SPI_InitTypeDef	SPI_InitStructure;
	
	/* SPI Config */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;	
	SPI_Init(SPI1, &SPI_InitStructure);
	
	DMA_DeInit(DMA2_Stream3);	//SPI1_TX_DMA_STREAM
	DMA_DeInit(DMA2_Stream2);	//SPI1_RX_DMA_STREAM
	
//	DMA_InitStructure.DMA_BufferSize = (uint16_t)(tx_len + 3);///////////DMAy_Streamx->NDTR = (uint16_t)Counter;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable ;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull ;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single ;
//  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(SPI1->DR));
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize =  DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;

	/* Configure Tx DMA */
	DMA_InitStructure.DMA_Channel = DMA_Channel_3;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
//	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) pTmpBuf1;//////////////// DMA2_Stream3->M0AR = (uint32_t) pTmpBuf1; 
	DMA_Init(DMA2_Stream3, &DMA_InitStructure);

/* Configure Rx DMA */
	DMA_InitStructure.DMA_Channel = DMA_Channel_3;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
//	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) pTmpBuf2;///////////// DMA2_Stream2->M0AR = (uint32_t) pTmpBuf2; 
	DMA_Init(DMA2_Stream2, &DMA_InitStructure);
	
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
	SPI_Cmd(SPI1, ENABLE);
	

	
}

uint8_t SPI_SendByte(uint8_t byte)
{
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, byte);
	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	
	return SPI_I2S_ReceiveData(SPI1);
}

uint8_t SPI_ReceiveByte(void)
{
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) ;

	  SPI_I2S_SendData(SPI1, 0);

	  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

	  return SPI_I2S_ReceiveData(SPI1);
}

void SPI_SendBurst(uint8_t* pBuf, uint16_t len)
{
}

void SPI_ReceiveBurst(uint8_t* pBuf, uint16_t len)
{
}

void SPI_DMA_WRITE(uint8_t* Addref, uint8_t* pTxBuf, uint16_t tx_len)
{
	uint16_t i;



	pTmpBuf1[0] = Addref[0];
	pTmpBuf1[1] = Addref[1];
	pTmpBuf1[2] = Addref[2];

	for(i=0; i<tx_len; i++)
		pTmpBuf1[3 + i] = pTxBuf[i];


	
DMA2_Stream3->NDTR = (uint16_t)(tx_len + 3);
DMA2_Stream2->NDTR = (uint16_t)(tx_len + 3);
DMA2_Stream3->M0AR = (uint32_t) pTmpBuf1;
DMA2_Stream2->M0AR = (uint32_t) pTmpBuf1; 




	/* Enable the DMA channel */

	wizchip_select();

	/* Enable the DMA SPI TX Stream */
	DMA_Cmd(DMA2_Stream3, ENABLE);
	/* Enable the DMA SPI RX Stream */
	DMA_Cmd(DMA2_Stream2, ENABLE);
	 
  /* Waiting the end of Data transfer */
  while (DMA_GetFlagStatus(DMA2_Stream3, DMA_FLAG_TCIF3)==RESET);
  while (DMA_GetFlagStatus(DMA2_Stream2, DMA_FLAG_TCIF2)==RESET);



	wizchip_deselect();


	DMA_ClearFlag(DMA2_Stream3, DMA_FLAG_TCIF3);
	DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);

	DMA_Cmd(DMA2_Stream3, DISABLE);
	DMA_Cmd(DMA2_Stream2, DISABLE);

}

void SPI_DMA_READ(uint8_t* Addref, uint8_t* pRxBuf, uint16_t rx_len)
{
	uint16_t i;


	memset(pTmpBuf1, 0, rx_len + 3);
	memset(pTmpBuf2, 0, rx_len + 3);

	pTmpBuf1[0] = Addref[0];
	pTmpBuf1[1] = Addref[1];
	pTmpBuf1[2] = Addref[2];



DMA2_Stream2->NDTR = (uint16_t)(rx_len + 3);
DMA2_Stream3->NDTR = (uint16_t)(rx_len + 3);
DMA2_Stream3->M0AR = (uint32_t) pTmpBuf1;
DMA2_Stream2->M0AR = (uint32_t) pTmpBuf2; 

	/* Enable the DMA channel */


	wizchip_select();

	/* Enable the DMA SPI TX Stream */
	DMA_Cmd(DMA2_Stream3, ENABLE);
	/* Enable the DMA SPI RX Stream */
	DMA_Cmd(DMA2_Stream2, ENABLE);


  /* Waiting the end of Data transfer */
  while (DMA_GetFlagStatus(DMA2_Stream3, DMA_FLAG_TCIF3)==RESET);
  while (DMA_GetFlagStatus(DMA2_Stream2, DMA_FLAG_TCIF2)==RESET);



	wizchip_deselect();

	DMA_ClearFlag(DMA2_Stream3, DMA_FLAG_TCIF3);
	DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);

	DMA_Cmd(DMA2_Stream3, DISABLE);
	DMA_Cmd(DMA2_Stream2, DISABLE);


	memcpy(pRxBuf, pTmpBuf2 + 3, rx_len);

}
