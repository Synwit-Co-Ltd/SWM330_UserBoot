#ifndef __SWM330_RTC_H__
#define __SWM330_RTC_H__


#define RTC_CLKSRC_LRC32K	0
#define RTC_CLKSRC_XTAL32K	1


#define RTC_ALARM_A		0
#define RTC_ALARM_B		1


#define RTC_ALARM_Day	0	// alarm at day of the week
#define RTC_ALARM_Date	1	// alarm at specified date
#define RTC_ALARM_Daily	2	// daily alarm


#define RTC_MON     1
#define RTC_TUE     2
#define RTC_WED     3
#define RTC_THU     4
#define RTC_FRI     5
#define RTC_SAT     6
#define RTC_SUN     7


typedef struct {
	uint8_t  clksrc;		// RTC_CLKSRC_LRC32K, RTC_CLKSRC_XTAL32K
	uint16_t Year;
	uint8_t  Month;			// 1--12
	uint8_t  Date;			// 1--31
	uint8_t  Hour;			// 0--23
	uint8_t  Minute;		// 0--59
	uint8_t  Second;		// 0--59
} RTC_InitStructure;


typedef struct {
	union {
		uint8_t  Day;		// RTC_MON, RTC_TUE, RTC_WED, RTC_THU, RTC_FRI, RTC_SAT, RTC_SUN
		uint8_t  Date;
	};
	uint8_t  Mode;			// RTC_ALARM_Day, RTC_ALARM_Date, RTC_ALARM_Daily
	uint8_t  Hour;
	uint8_t  Minute;
	uint8_t  Second;
	uint8_t  AlarmIEn;
} RTC_AlarmStructure;


typedef struct {
	uint16_t Year;
	uint8_t  Month;
	uint8_t  Date;
	uint8_t  Day;			// RTC_MON, RTC_TUE, RTC_WED, RTC_THU, RTC_FRI, RTC_SAT, RTC_SUN
	uint8_t  Hour;
	uint8_t  Minute;
	uint8_t  Second;
	uint16_t SubSecond;
} RTC_DateTime;


/* Interrupt Type */
#define RTC_IT_ALRMA		(1 << 0)
#define RTC_IT_ALRMB		(1 << 1)
#define RTC_IT_WKUP			(1 << 2)
#define RTC_IT_TS			(1 << 3)



void RTC_Init(RTC_TypeDef * RTCx, RTC_InitStructure * initStruct);

void RTC_GetDateTime(RTC_TypeDef * RTCx, RTC_DateTime * dateTime);

void RTC_WakeupSetup(RTC_TypeDef * RTCx, uint16_t second, uint32_t int_en);

void RTC_AlarmSetup(RTC_TypeDef * RTCx, uint32_t alarmx, RTC_AlarmStructure * alarmStruct);


void RTC_INTEn(RTC_TypeDef * RTCx, uint32_t it);
void RTC_INTDis(RTC_TypeDef * RTCx, uint32_t it);
void RTC_INTClr(RTC_TypeDef * RTCx, uint32_t it);
uint32_t RTC_INTStat(RTC_TypeDef * RTCx, uint32_t it);


static inline void RTC_unlock(RTC_TypeDef * RTCx)
{
	SYS->RTCWKCR |= SYS_RTCWKCR_WREN_Msk;
	
	RTCx->UNLOCK = 0xCA;
	RTCx->UNLOCK = 0x53;
}

static inline void RTC_lock(RTC_TypeDef * RTCx)
{
	RTCx->UNLOCK = 0x00;
}


#endif
