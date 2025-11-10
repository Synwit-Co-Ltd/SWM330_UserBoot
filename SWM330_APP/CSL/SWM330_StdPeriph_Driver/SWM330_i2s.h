#ifndef __SWM330_I2S_H__
#define __SWM330_I2S_H__


typedef struct {
	uint8_t  Mode;			//I2S_MASTER_TX、I2S_MASTER_RX、I2S_SLAVE_TX、I2S_SLAVE_RX
	uint32_t ClkFreq;
	uint8_t  ClkPolarity;	//I2S_CLK_IDLE_LOW、I2S_CLK_IDLE_HIGH
	uint8_t  FrameFormat;	//I2S_I2S_PHILIPS、I2S_MSB_JUSTIFIED、I2S_LSB_JUSTIFIED、I2S_PCM_SHORT、I2S_PCM_LONG
	uint8_t  ChannelLen; 	//I2S_CHNNLEN_16、I2S_CHNNLEN_32
	uint8_t  DataLen;		//I2S_DATALEN_8、I2S_DATALEN_16、I2S_DATALEN_24、I2S_DATALEN_32
	uint8_t  INTEn;			//I2S_IT_SLV_TX_UNR、I2S_IT_RX_OVR、I2S_IT_RX_NOT_EMPTY、I2S_IT_TX_EMPTY 式凪＾賜￣
} I2S_InitStructure;

#define I2S_SLAVE_TX		0
#define I2S_SLAVE_RX		1
#define I2S_MASTER_TX		2
#define I2S_MASTER_RX		3

#define I2S_CLK_IDLE_LOW	0
#define I2S_CLK_IDLE_HIGH	1

#define I2S_I2S_PHILIPS		0
#define I2S_MSB_JUSTIFIED	1
#define I2S_LSB_JUSTIFIED	2
#define I2S_PCM_SHORT		3	//PCM Long Mode Sync Width 1 SCLK period
#define I2S_PCM_LONG		7	//PCM Long Mode Sync Width 1 Data Length

#define I2S_CHNNLEN_16		0
#define I2S_CHNNLEN_32		1

#define I2S_DATALEN_8		0
#define I2S_DATALEN_16		1
#define I2S_DATALEN_24		2
#define I2S_DATALEN_32		3



/* Interrupt Type */
#define I2S_IT_SLV_TX_UNR	(1 << 0)	//Slave TX Buffer Underrun
#define I2S_IT_RX_OVR		(1 << 1)	//RX Buffer Overrun
#define I2S_IT_RX_NOT_EMPTY	(1 << 2)
#define I2S_IT_TX_EMPTY		(1 << 3)



void I2S_Init(I2S_TypeDef * I2Sx, I2S_InitStructure * initStruct);
void I2S_Open(I2S_TypeDef * I2Sx);
void I2S_Close(I2S_TypeDef * I2Sx);
void I2S_MCLKConfig(I2S_TypeDef * I2Sx, uint32_t output_enable, uint32_t mclk_freq);

void I2S_INTEn(I2S_TypeDef * I2Sx, uint32_t it);
void I2S_INTDis(I2S_TypeDef * I2Sx, uint32_t it);
void I2S_INTClr(I2S_TypeDef * I2Sx, uint32_t it);
bool I2S_INTStat(I2S_TypeDef * I2Sx, uint32_t it);

#endif
