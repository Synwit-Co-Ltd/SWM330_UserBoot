#ifndef __SWM330_DAC_H__
#define __SWM330_DAC_H__


typedef struct {
	uint8_t  Vref;			// DAC_Vref_VDD, DAC_Vref_VREFP
	uint8_t  Format;		// DAC_Format_LSB12B, DAC_Format_MSB12B, DAC_Format_8B
	uint8_t  OutBuffer;		// DAC_OutBuffer_On, DAC_OutBuffer_Off
} DAC_InitStructure;


#define DAC_Format_LSB12B	0	// 12-bit data, DHR[11:0] => DOR[11:0]
#define DAC_Format_MSB12B	1	// 12-bit data, DHR[15:4] => DOR[11:0]
#define DAC_Format_8B		3	//  8-bit data, DHR[7 :0] => DOR[11:4]

#define DAC_Vref_VDD		0
#define DAC_Vref_VREFP		1	// VREFP pin

#define DAC_OutBuffer_On	0
#define DAC_OutBuffer_Off	1


void DAC_Init(DAC_TypeDef * DACx, DAC_InitStructure * initStruct);
void DAC_Open(DAC_TypeDef * DACx);
void DAC_Close(DAC_TypeDef * DACx);


#endif
