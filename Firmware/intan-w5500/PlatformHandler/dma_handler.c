#include "stm32f4xx.h"
#include "stm32f4xx_dma.h"

#include "dma_handler.h"
#include "common.h"



void DMA_Configuration(void)
{

	
	/* Deinitialize DMA Streams */
//	DMA_DeInit(DMA2_Stream3);	//SPI1_TX_DMA_STREAM
//	DMA_DeInit(DMA2_Stream2);	//SPI1_RX_DMA_STREAM
	

//DMA_InitTypeDef DMA_InitStructure;

//	DMA_InitStructure.DMA_BufferSize = (uint16_t)(tx_len + 3);///////////
//  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable ;
//  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull ;
//  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single ;
//  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
//	
//	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(SPI1->DR));
//	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
//  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//	DMA_InitStructure.DMA_MemoryDataSize =  DMA_MemoryDataSize_Byte;
//	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//	DMA_InitStructure.DMA_Priority = DMA_Priority_High;

//	/* Configure Tx DMA */
//	DMA_InitStructure.DMA_Channel = DMA_Channel_3;
//	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
//	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) pTmpBuf1;//////////////// DMAy_Streamx->M0AR = MemoryBaseAddr; 
//	DMA_Init(DMA2_Stream3, &DMA_InitStructure);

///* Configure Rx DMA */
//	DMA_InitStructure.DMA_Channel = DMA_Channel_3;
//	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
//	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) pTmpBuf1;/////////////  DMAy_Streamx->M0AR = MemoryBaseAddr; 
//	DMA_Init(DMA2_Stream2, &DMA_InitStructure);
	
}
