#include "SWM330.h"


void SerialInit(void);

int main(void)
{
 	SystemInit();
	
	SerialInit();
	
	GPIO_INIT(GPIOA, PIN8, GPIO_INPUT_PullUp);	// 输入，开上拉
	GPIO_INIT(GPIOA, PIN5, GPIO_OUTPUT);		// 输出
	GPIO_SetBit(GPIOA, PIN5);
	
	TIMR_Init(TIMR0, TIMR_MODE_TIMER, CyclesPerUs, 1500000, 1);	// 1.5s 定时中断
	TIMR_Start(TIMR0);
	
	__enable_irq();
	
	while(1==1)
 	{
		printf("Hi from App\n");
		
		for(int i = 0; i < SystemCoreClock/4; i++) __NOP();
		
		
		if(GPIO_GetBit(GPIOA, PIN8) == 0)	// 检测到按键按下，跳到UserBoot
		{
			__disable_irq();
			
			WDT_Init(WDT, 0, 5);			// 通过触发WDT复位跳转到UserBoot
			WDT_Start(WDT);
			while(1) __NOP();
		}
 	}
}


void TIMR0_Handler(void)
{
	TIMR_INTClr(TIMR0, TIMR_IT_TO);
	
	GPIO_InvBit(GPIOA, PIN5);
}


void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTA, PIN6, FUNMUX0_UART0_TXD, 0);
	PORT_Init(PORTA, PIN7, FUNMUX1_UART0_RXD, 1);
 	
 	UART_initStruct.Baudrate = 57600;
	UART_initStruct.DataBits = UART_DATA_8BIT;
	UART_initStruct.Parity = UART_PARITY_NONE;
	UART_initStruct.StopBits = UART_STOP_1BIT;
	UART_initStruct.RXThreshold = 3;
	UART_initStruct.RXThresholdIEn = 0;
	UART_initStruct.TXThreshold = 3;
	UART_initStruct.TXThresholdIEn = 0;
	UART_initStruct.TimeoutTime = 10;
	UART_initStruct.TimeoutIEn = 0;
 	UART_Init(UART0, &UART_initStruct);
	UART_Open(UART0);
}

int fputc(int ch, FILE *f)
{
	UART_WriteByte(UART0, ch);
	
	while(UART_IsTXBusy(UART0));
 	
	return ch;
}
