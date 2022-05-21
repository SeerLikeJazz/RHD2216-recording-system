#include  "INTAN.h"
#include "usart.h"	


/********************************************
* 功能：SPI初始化
********************************************/
void INTAN_Init(void)
{
	SPI2_Init();
	SpiFlash_CS(1);
}


/********************************************************************
*	功能：INTAN读命令
********************************************************************/
void INTAN_ReadData(u16 command,u8 length,u16 *SaveTo)
{
	u8 i;
	SpiFlash_CS(0);
	SPI2_ReadWriteByte(command);
	
	SpiFlash_CS(1);
	SpiFlash_CS(0);//attention if delay?
	
	SPI2_ReadWriteByte(0);//dummy bytes
	
	SpiFlash_CS(1);
	SpiFlash_CS(0);//attention if delay?
	
	for(i=0;i<length;i++)
	{
		*SaveTo = SPI2_ReadWriteByte(0);
	}
	SpiFlash_CS(1);
}


/********************************************************************
* Name:       INTAN_WriteData
*	Summary：   INTAN写命令 
* Parameters: length->dummy words
********************************************************************/
void INTAN_WriteData(u16 command,u8 length)
{
	u8 i;
	SpiFlash_CS(0);
	SPI2_ReadWriteByte(command);	
	SpiFlash_CS(1);
	
	for(i=0;i<length;i++)
	{
		SpiFlash_CS(0);
	  SPI2_ReadWriteByte(0);//dummy bytes
		SpiFlash_CS(1);
	}	
}


/********************************************************************
* Name:       CONVERT
*	Summary：   ADC convert
********************************************************************/
void INTAN_Convert(u16 command,u16 *ptr)
{	
	SpiFlash_CS(0);
	*ptr=SPI2_ReadWriteByte(command);	
	SpiFlash_CS(1); 

}


/********************************************************************
*	功能：INTAN读取厂家ID
********************************************************************/
void INTAN_ReadID(u16 *ptr)
{
	u8 i=0;
  INTAN_ReadData(INTAN_REG_40_DEF,1,(ptr));
	INTAN_ReadData(INTAN_REG_41_DEF,1,(ptr+1));
	INTAN_ReadData(INTAN_REG_42_DEF,1,(ptr+2));
	INTAN_ReadData(INTAN_REG_43_DEF,1,(ptr+3));
	INTAN_ReadData(INTAN_REG_44_DEF,1,(ptr+4));	
  for(i=0;i<5;i++)
	{
	 printf("%c",*(ptr+i));
	}
  printf("\r\n");
}


/********************************************************************
* Name:     INTAN_Calibrate_ADC
*
*	Summary：Initiate ADC Self-Calibration
********************************************************************/
void INTAN_Calibrate_ADC(void)
{
  INTAN_WriteData(INTAN_DUMMY,20);
	INTAN_WriteData(INTAN_CALIBRATE_ADC,20);   // REG0: ADC Config and Fast Settle 
}


/********************************************************************
* NAME:     INTAN_Configuration
*
*	Summary：Setup INTAN settings defined in header file.
********************************************************************/
void INTAN_Configuration(void)
{
	  INTAN_WriteData(INTAN_DUMMY,10);
    // Setup Registers 0-7    
    INTAN_WriteData(INTAN_REG_0_DEF,0);   // REG0: ADC Config and Fast Settle 
    INTAN_WriteData(INTAN_REG_1_DEF,0);   // REG1: Supply Sensor and ADC Buffer Bias Current 
    INTAN_WriteData(INTAN_REG_2_DEF,0);   // REG2: MUX Bias Current
    INTAN_WriteData(INTAN_REG_3_DEF,0);   // REG3: MUX Load, Temp Sensor, Aux Dout
    INTAN_WriteData(INTAN_REG_4_DEF,0);   // REG4: ADC Output Format and DSP offset Removal
    INTAN_WriteData(INTAN_REG_5_DEF,0);   // REG5: Impedance Check Control
    INTAN_WriteData(INTAN_REG_6_DEF,0);   // REG6: Impedance Check ADC
    INTAN_WriteData(INTAN_REG_7_DEF,0);   // REG7: Impedance Amp Select

    // Setup On-Chip Amplifer Bandwidth Registers
    INTAN_WriteData(INTAN_REG_8_DEF,0);   // REG8: On-Chip Amp Bandwidth Select
    INTAN_WriteData(INTAN_REG_9_DEF,0);   // REG9: On-Chip Amp Bandwidth Select
    INTAN_WriteData(INTAN_REG_10_DEF,0);  // REG10: On-Chip Amp Bandwidth Select
    INTAN_WriteData(INTAN_REG_11_DEF,0);  // REG11: On-Chip Amp Bandwidth Select
    INTAN_WriteData(INTAN_REG_12_DEF,0);  // REG12: On-Chip Amp Bandwidth Select
    INTAN_WriteData(INTAN_REG_13_DEF,0);  // REG13: On-Chip Amp Bandwidth Select

    // Setup Individual Amplifier Power Registers
    INTAN_WriteData(INTAN_REG_14_DEF,0);  // REG14: Individual Amp Power
    INTAN_WriteData(INTAN_REG_15_DEF,0);  // REG15: Individual Amp Power
    INTAN_WriteData(INTAN_REG_16_DEF,0);  // REG16: Individual Amp Power
    INTAN_WriteData(INTAN_REG_17_DEF,0);  // REG17: Individual Amp Power
       
    // Send several dummy commands ... just because i can...
    INTAN_WriteData(INTAN_DUMMY,10);
}

/*******************************************************************************
* Name: INTAN_Verify_Setup
********************************************************************************
*
* Summary:
*  Reads each writeable register and varifies the return value with the expected
*  defined setting.
*
* Parameters:
*  None.
*
* Return:
*  Returns bit 1 if all checks pass and bit 0 if any fail to be verified.
*
*******************************************************************************/
u8 INTAN_Verify_Setup(void)
{
	u8 i=0;
	u8 result;
	u8 flag;
	    // build lookup table of defined register values
  u16 def[18]={INTAN_REG_0_DEF, INTAN_REG_1_DEF, INTAN_REG_2_DEF, INTAN_REG_3_DEF, INTAN_REG_4_DEF,          \
                    INTAN_REG_5_DEF, INTAN_REG_6_DEF, INTAN_REG_7_DEF, INTAN_REG_8_DEF, INTAN_REG_9_DEF,        \
                    INTAN_REG_10_DEF, INTAN_REG_11_DEF, INTAN_REG_12_DEF, INTAN_REG_13_DEF, INTAN_REG_14_DEF,   \
                    INTAN_REG_15_DEF, INTAN_REG_16_DEF, INTAN_REG_17_DEF};
	
	INTAN_WriteData(INTAN_DUMMY,10);
	for(i=0;i<18;i++)
	{
   INTAN_WriteData(def[i],1);
		
	 SpiFlash_CS(0);

	 result=SPI2_ReadWriteByte(0);//dummy commands
	 SpiFlash_CS(1);
		
	 printf("%x\r\n",result);
				// compare result with expected defined value
		if (result == (u8)(def[i]) )
		{
				flag=1;
		}    
		else
		{
				flag=0;
				break;
		}		
	}
	return flag;
}

/* [] END OF FILE */
