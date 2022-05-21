#ifndef _INTAN_h_
#define _INTAN_h_
#include <stm32f4xx.h>
#include "spi.h"



// Define commands
#define INTAN_CONVERT_CH_0  (0x0000u)   // Run ADC conversion on ch 0
#define INTAN_CONVERT_CH_1  (0x0100u)   // Run ADC conversion on ch 1
#define INTAN_CONVERT_CH_2  (0x0200u)   // Run ADC conversion on ch 2
#define INTAN_CONVERT_CH_3  (0x0300u)   // Run ADC conversion on ch 3
#define INTAN_CONVERT_CH_4  (0x0400u)   // Run ADC conversion on ch 4
#define INTAN_CONVERT_CH_5  (0x0500u)   // Run ADC conversion on ch 5
#define INTAN_CONVERT_CH_6  (0x0600u)   // Run ADC conversion on ch 6
#define INTAN_CONVERT_CH_7  (0x0700u)   // Run ADC conversion on ch 7
#define INTAN_CONVERT_CH_8  (0x0800u)   // Run ADC conversion on ch 8
#define INTAN_CONVERT_CH_9  (0x0900u)   // Run ADC conversion on ch 9
#define INTAN_CONVERT_CH_10 (0x0A00u)   // Run ADC conversion on ch 10
#define INTAN_CONVERT_CH_11 (0x0B00u)   // Run ADC conversion on ch 11
#define INTAN_CONVERT_CH_12 (0x0C00u)   // Run ADC conversion on ch 12
#define INTAN_CONVERT_CH_13 (0x0D00u)   // Run ADC conversion on ch 13
#define INTAN_CONVERT_CH_14 (0x0E00u)   // Run ADC conversion on ch 14
#define INTAN_CONVERT_CH_15 (0x0F00u)   // Run ADC conversion on ch 15

#define INTAN_CALIBRATE_ADC (0x5500u)   // Initiate ADC Self-Calibration
#define INTAN_CLEAR_ADC     (0x6A00u)   // Clear ADC calibration
#define INTAN_DUMMY         (INTAN_REG_63_DEF)  // Dummy command

// Define Default Register Values (LSB = settings for the register)
// (MSB includes read bits and register number. bits = 1 0 R[5] R[4] R[3] R[2] R[1] R[0]) 
#define INTAN_REG_0_DEF     (0x80DEu)     // REG0: ADC Config and Fast Settle (Regular operation)
#define INTAN_REG_1_DEF     (0x8120u)     // REG1: Supply Sensor and ADC Buffer Bias Current
#define INTAN_REG_2_DEF     (0x8228u)     // REG2: MUX Bias Current
#define INTAN_REG_3_DEF     (0x8302u)     // REG3: MUX Load, Temp Sensor, Aux Dout
#define INTAN_REG_4_DEF     (0x8480u)     // REG4: ADC Output Format and DSP offset Removal (DSP off, two's compliment output)
#define INTAN_REG_5_DEF     (0x8500u)     // REG5: Impedance Check Control (Zcheck off)
#define INTAN_REG_6_DEF     (0x8600u)     // REG6: Impedance Check ADC
#define INTAN_REG_7_DEF     (0x8700u)     // REG7: Impedance Amp Select

// Define On-Chip Amplifer Bandwidth Registers (LSB = settings for the register)
// (MSB includes read bits and register number. bits = 1 0 R[5] R[4] R[3] R[2] R[1] R[0]) 
#define INTAN_REG_8_DEF     (0x8821u)     // REG8: On-Chip Amp Bandwidth Select (LPF: Fc=5kHz)
#define INTAN_REG_9_DEF     (0x8900u)     // REG9: On-Chip Amp Bandwidth Select (LPF: Fc=5kHz, Aux1_en=0)
#define INTAN_REG_10_DEF    (0x8a25u)     // REG10: On-Chip Amp Bandwidth Select (LPF: Fc=5kHz
#define INTAN_REG_11_DEF    (0x8b00u)     // REG11: On-Chip Amp Bandwidth Select (LPF: Fc=5kHz, Aux2_en=0))
#define INTAN_REG_12_DEF    (0x8c11u)     // REG12: On-Chip Amp Bandwidth Select (HPF: Fc=250Hz)
#define INTAN_REG_13_DEF    (0x8d00u)     // REG13: On-Chip Amp Bandwidth Select (HPF: Fc=250Hz, Aux3_en=0)

// Define Individual Amplifier Power Registers (LSB = settings for the register)
// (MSB includes write bits and register number. bits = 1 0 R[5] R[4] R[3] R[3] R[1] R[0]) 
#define INTAN_REG_14_DEF    (0x8effu)     // REG14: Individual Amp Power (Ch4-7 on)
#define INTAN_REG_15_DEF    (0x8fffu)     // REG15: Individual Amp Power (Ch9-15 off)
#define INTAN_REG_16_DEF    (0x9000u)     // REG16: Individual Amp Power (Ch17-24 off)
#define INTAN_REG_17_DEF    (0x9100u)     // REG17: Individual Amp Power (Ch25-32 off)

// Define (Read-Only) Registers 40-44 (LSB = settings for the register)
// (MSB includes write bits and register number. bits = 1 0 R[5] R[4] R[3] R[3] R[1] R[0]) 
// Company Designation
#define INTAN_REG_40_DEF    (0xe800u)     // REG40: Company Designation character "I"
#define INTAN_REG_41_DEF    (0xe900u)     // REG41: Company Designation character "N"
#define INTAN_REG_42_DEF    (0xea00u)     // REG42: Company Designation character "T"
#define INTAN_REG_43_DEF    (0xeb00u)     // REG43: Company Designation character "A"
#define INTAN_REG_44_DEF    (0xec00u)     // REG44: Company Designation character "N"

// Define (Read-Only) Registers 60-63 (LSB = settings for the register)
// (MSB includes write bits and register number. bits = 1 0 R[5] R[4] R[3] R[3] R[1] R[0]) 
#define INTAN_REG_60_DEF    (0xfc00u)     // REG60: Die Revision (8bit result with die revision)
#define INTAN_REG_61_DEF    (0xfd00u)     // REG61: Unipolar/Bipolar Amp (8bit result 0=bipolar, 1=unipolar)
#define INTAN_REG_62_DEF    (0xfe00u)     // REG62: Number of Amplifiers (8bit result encodes total nuber of amps)
#define INTAN_REG_63_DEF    (0xff00u)     // REG63: Intan Chip ID (8bit result encodes chip ID number)

// Define some expected result values from the chip
#define INTAN_TEST_RESPONSE_CHAR "INTAN"        // Expected Response from INTAN for test commands (char)
#define INTAN_TEST_RESPONSE (0x494E54414Eu)     // Expected Response from INTAN for test commands 




/***************************************
*        Function Prototypes
***************************************/
void Calculate_Filter_Registers(void);//Ã»ÓÃ¹ý£

void INTAN_Init(void);
void INTAN_ReadData(u16 command,u8 length,u16 *SaveTo);
void INTAN_WriteData(u16 command,u8 length);
void INTAN_Convert(u16 command,u16 * ptr);
void INTAN_ReadID(u16 *ptr);
void INTAN_Calibrate_ADC(void);
void INTAN_Configuration(void);
u8 INTAN_Verify_Setup(void);


#endif /* INCLUDED_INTAN_H */

//[] END OF FILE

