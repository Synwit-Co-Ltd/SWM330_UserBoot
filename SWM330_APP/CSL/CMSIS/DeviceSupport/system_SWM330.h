#ifndef __SYSTEM_SWM330_H__
#define __SYSTEM_SWM330_H__

#ifdef __cplusplus
 extern "C" {
#endif


extern uint32_t SystemCoreClock;		// System Clock Frequency (Core Clock)
extern uint32_t CyclesPerUs;			// Cycles per micro second


extern void SystemInit(void);

extern void SystemCoreClockUpdate (void);

extern void switchTo8MHz(void);
extern void switchTo1MHz(void);
extern void switchToXTAL(uint32_t div8);
void switchToPLL(uint32_t clksrc_xtal, uint32_t indiv, uint32_t fbdiv, uint32_t outdiv, uint32_t div8);
extern void switchTo32KHz(void);
extern void switchToXTAL_32K(void);


extern void LDO_1V8_On(uint32_t clksrc);


#ifdef __cplusplus
}
#endif

#endif
