/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include "wizchip_conf.h"
#include "loopback.h"
#include "rcc_handler.h"
#include "gpio_handler.h"
//#include "timer_handler.h"
#include "uart_handler.h"
#include "spi_handler.h"
#include "common.h"
#include "socket.h"
#include "spi.h"
#include "INTAN.h"



/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t uwTimingDelay;

////////////////////Êý¾Ý´æ´¢/////////////////////////////
u16 temp[20]={NULL};																		
uint8_t gDATABUF[DATA_BUF_SIZE]={0,0,2,3,1,2,3,4,5,6,7,8,9};

//////////// Default Network Configuration //////////////

uint8_t remote_ip[4]={192,168,1,115};	/*Ô¶¶ËIPµØÖ·*/
uint16_t remote_port=3000;							/*Ô¶¶Ë¶Ë¿ÚºÅ*/
wiz_NetInfo gWIZNETINFO = {{0x00, 0x08, 0xdc, 0x11, 0x11, 0x11},
																							{192, 168, 1, 88},
																							{255,255,255,0},
																							{192, 168, 1, 1},
																							{8,8,8,8},
																							NETINFO_STATIC };

/* Private function prototypes -----------------------------------------------*/
static void Delay(__IO uint32_t nTime);

/* Private functions ---------------------------------------------------------*/


/**
  * @brief  This function handles TIM2 Handler.
  * @param  None
  * @retval None
  */

void  wizchip_select(void)
{

	GPIOA->BSRRH = WIZ_SCS;
}

void  wizchip_deselect(void)
{

	GPIOA->BSRRL = WIZ_SCS;
}

void  wizchip_write(uint8_t wb)
{
	SPI_SendByte(wb);
}

uint8_t wizchip_read()
{
	return SPI_ReceiveByte();
}



void network_init(void)
{
   uint8_t tmpstr[6];
	

  ctlnetwork(CN_SET_NETINFO, (void*)&gWIZNETINFO);

	ctlnetwork(CN_GET_NETINFO, (void*)&gWIZNETINFO);

	printf("\r\n=== %s NET CONF ===\r\n",(char*)tmpstr);
	printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n",gWIZNETINFO.mac[0],gWIZNETINFO.mac[1],gWIZNETINFO.mac[2],
		     gWIZNETINFO.mac[3],gWIZNETINFO.mac[4],gWIZNETINFO.mac[5]);
	printf("SIP: %d.%d.%d.%d\r\n", gWIZNETINFO.ip[0],gWIZNETINFO.ip[1],gWIZNETINFO.ip[2],gWIZNETINFO.ip[3]);
	printf("GAR: %d.%d.%d.%d\r\n", gWIZNETINFO.gw[0],gWIZNETINFO.gw[1],gWIZNETINFO.gw[2],gWIZNETINFO.gw[3]);
	printf("SUB: %d.%d.%d.%d\r\n", gWIZNETINFO.sn[0],gWIZNETINFO.sn[1],gWIZNETINFO.sn[2],gWIZNETINFO.sn[3]);
	printf("DNS: %d.%d.%d.%d\r\n", gWIZNETINFO.dns[0],gWIZNETINFO.dns[1],gWIZNETINFO.dns[2],gWIZNETINFO.dns[3]);
	printf("======================\r\n");
}
/////////////////////////////////////////////////////////////

/**
  * @brief   Main program
  * @param  None
  * @retval None

  */



int main(void)
{
	
	u16 i=0;

	uint8_t sn = 0;

	uint8_t memsize[2][8] = {{2,2,2,2,2,2,2,2}, {2,2,2,2,2,2,2,2}};
		
	gDATABUF[1026]=0xff;	 //±ê¼Ç

	RCC_Configuration();
	GPIO_Configuration();
//	Timer_Configuration();
//	DMA_Configuration();
  SPI_Configuration();
	USART_Configuration();  //115200  /* SysTick end of count event each 10ms */
	INTAN_Init();
	INTAN_ReadID(temp);
	
	if(INTAN_Verify_Setup())
	{
	  printf("Setup Pass.\r\n");
	}
	else printf("Setup Fail.\r\n");
	
	INTAN_Calibrate_ADC();
	
	INTAN_Configuration();
  
	 ///////////////////×¢²áº¯Êý///////////////////////////
	 reg_wizchip_cs_cbfunc(wizchip_select, wizchip_deselect);
	 reg_wizchip_spi_cbfunc(wizchip_read, wizchip_write);


	/* WIZCHIP SOCKET Buffer initialize */
	if(ctlwizchip(CW_INIT_WIZCHIP,(void*)memsize) == -1)
	{
		 printf("WIZCHIP Initialized fail.\r\n");
		 while(1);
	}

	/* Network Initialization */
	network_init();
		
	socket(sn,Sn_MR_UDP,3000,0x00);
		
  while (1)
  {

		MY_sendto(sn,0,1029,remote_ip,remote_port); 

	}
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{ 
  uwTimingDelay = nTime;

  while(uwTimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (uwTimingDelay != 0x00)
  { 
    uwTimingDelay--;
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
