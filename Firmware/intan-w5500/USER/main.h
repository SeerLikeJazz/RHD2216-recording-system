/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.h 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    18-January-2013
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"



/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Port A */
#define WIZ_RDY				GPIO_Pin_2			//in

#define WIZ_SCLK			GPIO_Pin_5			//out
#define WIZ_MISO			GPIO_Pin_6			//in
#define WIZ_MOSI			GPIO_Pin_7			//out

#define USART1_TX		GPIO_Pin_9			//out
#define USART1_RX		GPIO_Pin_10		//in

#define WIZ_SCS				GPIO_Pin_4		//out
#define WIZ_nRST			GPIO_Pin_3		//out


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void TimingDelay_Decrement(void);
void  wizchip_select(void);
void  wizchip_deselect(void);
void  wizchip_write(uint8_t wb);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
