#ifndef __SWM330_PSRAM_H__
#define __SWM330_PSRAM_H__


typedef struct {
	uint8_t  RowSize;	// PSRAM_ROWSIZE_1KB, PSRAM_ROWSIZE_2KB, PSRAM_ROWSIZE_4KB, PSRAM_ROWSIZE_8KB
	
#ifndef PSRAM_XCCELA
	uint16_t tRWR;		// HyperRAM Read-Write Recovery Time in ns
	uint16_t tACC;		// HyperRAM Read Initial Access Time in ns
	uint16_t tCSM;		// Chip Select Maximum Low Time in us
#else
	uint16_t tRC;		// Write/Read Cycle in ns
	uint16_t tCEM;		// CE# low pulse width in us
#endif
} PSRAM_InitStructure;


#ifndef PSRAM_XCCELA
#define PSRAM_RowSize_1KB	0
#define PSRAM_RowSize_2KB	1
#define PSRAM_RowSize_4KB	2
#define PSRAM_RowSize_8KB	3
#else
#define PSRAM_RowSize_512B	0
#define PSRAM_RowSize_1KB	1
#define PSRAM_RowSize_2KB	2
#define PSRAM_RowSize_4KB	3

#define PSRAM_IR01	&PSRAMC->IR01
#define PSRAM_IR23	&PSRAMC->IR23
#define PSRAM_IR48	&PSRAMC->IR48
#endif


void PSRAM_Init(PSRAM_InitStructure * initStruct);
uint32_t PSRAM_ReadIR(__I uint32_t * ir);

bool PSRAM_Busy(void);

void PSRAM_Sleep(void);
void PSRAM_SleepExit(void);
bool PSRAM_IsSleep(void);

void PSRAM_PowerDown(void);
void PSRAM_PowerDownExit(void);
bool PSRAM_IsPowerDown(void);


#endif
