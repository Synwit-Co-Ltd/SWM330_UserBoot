/*******************************************************************************************************************************
* @brief	RTC driver
*
*
********************************************************************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION 
* REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE 
* FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT 
* OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
* -ECTION WITH THEIR PRODUCTS.
*
* COPYRIGHT 2012 Synwit Technology 
*******************************************************************************************************************************/
#include "SWM330.h"
#include "SWM330_rtc.h"


static uint32_t calcWeekDay(uint32_t year, uint32_t month, uint32_t date);
/*******************************************************************************************************************************
* @brief	RTC init
* @param	RTCx is the RTC to init
* @param	initStruct is data used to init the RTC
* @return
*******************************************************************************************************************************/
void RTC_Init(RTC_TypeDef * RTCx, RTC_InitStructure * initStruct)
{
	SYS->CLKEN1 |= SYS_CLKEN1_RTC_Msk;
	
	RTC_unlock(RTCx);
	
	if(initStruct->clksrc == RTC_CLKSRC_XTAL32K)
	{
		RTC->X32KCR |= RTC_X32KCR_ON_Msk;
	}
	else
	{
		SYS->RCCR |= SYS_RCCR_LON_Msk;
	}
	
	RTC->PWRCR &= ~RTC_PWRCR_CLKSRC_Msk;
	RTC->PWRCR |= (initStruct->clksrc << RTC_PWRCR_CLKSRC_Pos);
	
	RTCx->SR |= RTC_SR_INIT_Msk;
	while((RTCx->SR & RTC_SR_INITWEN_Msk) == 0) __NOP();
	
	RTCx->CKDIV  = (255 << RTC_CKDIV_SDIV_Pos);
	RTCx->CKDIV |= (127 << RTC_CKDIV_ADIV_Pos);
	
	RTCx->TR = ((initStruct->Second % 10) << RTC_TR_SEC_Pos) |
			   ((initStruct->Second / 10) << RTC_TR_SEC10_Pos) |
			   ((initStruct->Minute % 10) << RTC_TR_MIN_Pos) |
			   ((initStruct->Minute / 10) << RTC_TR_MIN10_Pos) |
			   ((initStruct->Hour   % 10) << RTC_TR_HOUR_Pos) |
			   ((initStruct->Hour   / 10) << RTC_TR_HOUR10_Pos);
	
	RTCx->DR = ((initStruct->Date   % 10) << RTC_DR_DATE_Pos) |
			   ((initStruct->Date   / 10) << RTC_DR_DATE10_Pos) |
			   (calcWeekDay(initStruct->Year, initStruct->Month, initStruct->Date) << RTC_DR_DAY_Pos) |
			   ((initStruct->Month  % 10) << RTC_DR_MON_Pos) |
			   ((initStruct->Month  / 10) << RTC_DR_MON10_Pos) |
			   ((initStruct->Year   % 10) << RTC_DR_YEAR_Pos) |
			   (((initStruct->Year % 100) / 10) << RTC_DR_YEAR10_Pos);
		
	RTCx->CR = (4 << RTC_CR_WKCLK_Pos) |
			   (1 << RTC_CR_TSEDGE_Pos) |
			   (1 << RTC_CR_BYPSHAD_Pos) |
			   (0 << RTC_CR_FMT12_Pos);
	
	RTCx->SR &= ~RTC_SR_INIT_Msk;
}

/*******************************************************************************************************************************
* @brief	get current time and date
* @param	RTCx is the RTC to query
* @param	dateTime is used to save obtained time and date
* @return
*******************************************************************************************************************************/
void RTC_GetDateTime(RTC_TypeDef * RTCx, RTC_DateTime * dateTime)
{
	uint32_t rtc_tr, rtc_dr;
	
	while((rtc_tr != RTCx->TR) || (rtc_dr != RTCx->DR))
	{
		rtc_tr = RTCx->TR;
		rtc_dr = RTCx->DR;
		dateTime->SubSecond = RTCx->SUBSEC;
	}
	
	dateTime->Year = 2000 + ((rtc_dr & RTC_DR_YEAR10_Msk) >> RTC_DR_YEAR10_Pos) * 10 + ((rtc_dr & RTC_DR_YEAR_Msk) >> RTC_DR_YEAR_Pos);
	dateTime->Month = ((rtc_dr & RTC_DR_MON10_Msk) >> RTC_DR_MON10_Pos) * 10 + ((rtc_dr & RTC_DR_MON_Msk) >> RTC_DR_MON_Pos);
	dateTime->Date = ((rtc_dr & RTC_DR_DATE10_Msk) >> RTC_DR_DATE10_Pos) * 10 + ((rtc_dr & RTC_DR_DATE_Msk) >> RTC_DR_DATE_Pos);
	dateTime->Day = (rtc_dr & RTC_DR_DAY_Msk) >> RTC_DR_DAY_Pos;
	dateTime->Hour = ((rtc_tr & RTC_TR_HOUR10_Msk) >> RTC_TR_HOUR10_Pos) * 10 + ((rtc_tr & RTC_TR_HOUR_Msk) >> RTC_TR_HOUR_Pos);
	dateTime->Minute = ((rtc_tr & RTC_TR_MIN10_Msk) >> RTC_TR_MIN10_Pos) * 10 + ((rtc_tr & RTC_TR_MIN_Msk) >> RTC_TR_MIN_Pos);
	dateTime->Second = ((rtc_tr & RTC_TR_SEC10_Msk) >> RTC_TR_SEC10_Pos) * 10 + ((rtc_tr & RTC_TR_SEC_Msk) >> RTC_TR_SEC_Pos);
}

/*******************************************************************************************************************************
* @brief	set alarm time
* @param	RTCx is the RTC to set
* @param	alarmx is the alarm to set, can be RTC_ALARM_A and RTC_ALARM_B
* @param	alarmStruct is data used to set alarm time
* @return
*******************************************************************************************************************************/
void RTC_AlarmSetup(RTC_TypeDef * RTCx, uint32_t alarmx, RTC_AlarmStructure * alarmStruct)
{
	volatile uint32_t * Alarmx = (alarmx == RTC_ALARM_A) ? &RTCx->ALRMA : &RTCx->ALRMB;
	volatile uint32_t * AlarmxSS = (alarmx == RTC_ALARM_A) ? &RTCx->ALRMASS : &RTCx->ALRMBSS;
	
	RTCx->CR &= ~(1 << (RTC_CR_ALRAEN_Pos + alarmx));
	
	while((RTCx->SR & (1 << (RTC_SR_ALRAWEN_Pos + alarmx))) == 0) __NOP();
	
	*Alarmx = ((alarmStruct->Second % 10) << RTC_ALRMA_SEC_Pos) |
			  ((alarmStruct->Second / 10) << RTC_ALRMA_SEC10_Pos) |
			  ((alarmStruct->Minute % 10) << RTC_ALRMA_MIN_Pos) |
			  ((alarmStruct->Minute / 10) << RTC_ALRMA_MIN10_Pos) |
			  ((alarmStruct->Hour   % 10) << RTC_ALRMA_HOUR_Pos) |
			  ((alarmStruct->Hour   / 10) << RTC_ALRMA_HOUR10_Pos) |
			  ((alarmStruct->Mode == RTC_ALARM_Day ? alarmStruct->Day : (alarmStruct->Date % 10)) << RTC_ALRMA_DATE_Pos) |
			  ((alarmStruct->Date   / 10) << RTC_ALRMA_DATE10_Pos) |
			  ((alarmStruct->Mode == RTC_ALARM_Daily) << RTC_ALRMA_DATEMSK_Pos);
	
	*AlarmxSS = 0x00;	// not compare the sub-seconds of alarm
	
	RTCx->CR |=  (1 << (RTC_CR_ALRAEN_Pos + alarmx));
	
	RTCx->SR &= ~(1 << (RTC_SR_ALRAF_Pos + alarmx));
	
	RTCx->CR |= (alarmStruct->AlarmIEn << (RTC_CR_ALRAIE_Pos + alarmx));
	
	if(alarmStruct->AlarmIEn)
	{
		NVIC_EnableIRQ(RTC_IRQn);
	}
	else
	{
		NVIC_DisableIRQ(RTC_IRQn);
	}
}

/*******************************************************************************************************************************
* @brief	set wakeup time
* @param	RTCx is the RTC to set
* @param	second is the wakeup interval
* @param	int_en is interrupt enable
* @return
*******************************************************************************************************************************/
void RTC_WakeupSetup(RTC_TypeDef * RTCx, uint16_t second, uint32_t int_en)
{
	RTCx->CR &= ~RTC_CR_WKUPEN_Msk;
	
	while((RTCx->SR & RTC_SR_WKUPWEN_Msk) == 0) __NOP();
	
	RTCx->WKTIM = second;
	
	RTCx->CR |=  RTC_CR_WKUPEN_Msk;
	
	RTCx->SR &= ~RTC_SR_WKUPF_Msk;
	
	RTCx->CR |=  (int_en << RTC_CR_WKUPIE_Pos);
	
	if(int_en)
	{
		NVIC_EnableIRQ(RTC_IRQn);
	}
	else
	{
		NVIC_DisableIRQ(RTC_IRQn);
	}
}

/*******************************************************************************************************************************
* @brief	calculate the day of the week for the specified day
* @param	year, month, date is used to specify the day to calculate
* @param	month
* @param	date
* @return
*******************************************************************************************************************************/
static uint32_t calcWeekDay(uint32_t year, uint32_t month, uint32_t date)
{
    uint32_t i, cnt = 0;
    const uint32_t daysOfMonth[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	
	for(i = 1; i < month; i++) cnt += daysOfMonth[i];
	
    cnt += date;
	
    if((year%4 == 0) && ((year%100 != 0) || (year%400 == 0)) && (month >= 3)) cnt += 1;
	
    cnt += (year - 1901) * 365;
	
    for(i = 1901; i < year; i++)
    {
        if((i%4 == 0) && ((i%100 != 0) || (i%400 == 0))) cnt += 1;
    }
	
    return cnt % 7;
}

/*******************************************************************************************************************************
* @brief	RTC interrupt enable
* @param	RTCx is the RTC to set
* @param	it is interrupt type, can be RTC_IT_ALRMA, RTC_IT_ALRMB, RTC_IT_WKUP, RTC_IT_TS and their '|' operation
* @return
*******************************************************************************************************************************/
void RTC_INTEn(RTC_TypeDef * RTCx, uint32_t it)
{
	RTCx->CR |= (it << RTC_CR_ALRAIE_Pos);
}

/*******************************************************************************************************************************
* @brief	RTC interrupt disable
* @param	RTCx is the RTC to set
* @param	it is interrupt type, can be RTC_IT_ALRMA, RTC_IT_ALRMB, RTC_IT_WKUP, RTC_IT_TS and their '|' operation
* @return
*******************************************************************************************************************************/
void RTC_INTDis(RTC_TypeDef * RTCx, uint32_t it)
{
	RTCx->CR &= ~(it << RTC_CR_ALRAIE_Pos);
}

/*******************************************************************************************************************************
* @brief	RTC interrupt flag clear
* @param	RTCx is the RTC to clear
* @param	it is interrupt type, can be RTC_IT_ALRMA, RTC_IT_ALRMB, RTC_IT_WKUP, RTC_IT_TS and their '|' operation
* @return
*******************************************************************************************************************************/
void RTC_INTClr(RTC_TypeDef * RTCx, uint32_t it)
{
	RTCx->SR |= ~(it << RTC_SR_ALRAF_Pos);
}

/*******************************************************************************************************************************
* @brief	RTC interrupt state query
* @param	RTCx is the RTC to query
* @param	it is interrupt type, can be RTC_IT_ALRMA, RTC_IT_ALRMB, RTC_IT_WKUP, RTC_IT_TS and their '|' operation
* @return	1 interrupt happened, 0 interrupt not happen
*******************************************************************************************************************************/
uint32_t RTC_INTStat(RTC_TypeDef * RTCx, uint32_t it)
{
	return (RTCx->SR & (it << RTC_SR_ALRAF_Pos)) ? 1 : 0;
}
