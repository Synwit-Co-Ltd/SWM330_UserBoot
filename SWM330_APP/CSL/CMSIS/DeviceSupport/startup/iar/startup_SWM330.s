;*******************************************************************************************************************************
; @brief    startup file
;
;
;*******************************************************************************************************************************
; @attention
;
; THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION 
; REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE 
; FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT 
; OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
; -ECTION WITH THEIR PRODUCTS.
;
; COPYRIGHT 2012 Synwit Technology 
;******************************************************************************************************************************/

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        PUBLIC  __vector_table

        DATA
__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler              ; Reset Handler
        DCD     NMI_Handler                ; NMI Handler
        DCD     HardFault_Handler          ; Hard Fault Handler
        DCD     MemManage_Handler          ; MPU Fault Handler
        DCD     BusFault_Handler           ; Bus Fault Handler
        DCD     UsageFault_Handler         ; Usage Fault Handler
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     SVC_Handler                ; SVCall Handler
        DCD     DebugMon_Handler           ; Debug Monitor Handler
        DCD     0                          ; Reserved
        DCD     PendSV_Handler             ; PendSV Handler
        DCD     SysTick_Handler            ; SysTick Handler

        ; External Interrupts
        DCD    UART0_Handler
        DCD    UART1_Handler
        DCD    UART2_Handler
        DCD    UART3_Handler
        DCD    UART4_Handler
        DCD    PWM0_Handler
        DCD    PWM1_Handler
        DCD    PWMBRK_Handler
        DCD    SPI0_Handler
        DCD    I2S0_Handler
        DCD    ADC0_Handler
        DCD    BTIMR0_Handler
        DCD    BTIMR1_Handler
        DCD    BTIMR2_Handler
        DCD    BTIMR3_Handler
        DCD    TIMR0_Handler
        DCD    TIMR1_Handler
        DCD    CAN0_Handler
        DCD    I2C0_Handler
        DCD    I2C1_Handler
        DCD    DMA_Handler
        DCD    DMA2D_Handler
        DCD    BOD_Handler
        DCD    QSPI0_Handler
        DCD    QSPI1_Handler
        DCD    SRA_Handler
        DCD    DVP_Handler
        DCD    LCD_Handler
        DCD    JPEG_Handler
        DCD    SDIO_Handler
        DCD    WDT_Handler
        DCD    RTC_Handler
        DCD    EXTI0_Handler
        DCD    EXTI1_Handler
        DCD    EXTI2_Handler
        DCD    EXTI3_Handler
        DCD    EXTI4_Handler
        DCD    EXTI5_Handler
        DCD    EXTI6_Handler
        DCD    EXTI7_Handler
        DCD    EXTI8_11_Handler
        DCD    EXTI12_15_Handler
        

        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reset_Handler
        LDR     R0, =__iar_program_start
        BX      R0
        
        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
NMI_Handler
        B NMI_Handler

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
HardFault_Handler
        B HardFault_Handler

        PUBWEAK MemManage_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
MemManage_Handler
        B MemManage_Handler

        PUBWEAK BusFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BusFault_Handler
        B BusFault_Handler

        PUBWEAK UsageFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UsageFault_Handler
        B UsageFault_Handler

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
        B SVC_Handler

        PUBWEAK DebugMon_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
DebugMon_Handler
        B DebugMon_Handler

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
        B PendSV_Handler

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
        B SysTick_Handler


        PUBWEAK UART0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART0_Handler
        B UART0_Handler

        PUBWEAK UART1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART1_Handler
        B UART1_Handler

        PUBWEAK UART2_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART2_Handler
        B UART2_Handler

        PUBWEAK UART3_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART3_Handler
        B UART3_Handler

        PUBWEAK UART4_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART4_Handler
        B UART4_Handler

        PUBWEAK PWM0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PWM0_Handler
        B PWM0_Handler

        PUBWEAK PWM1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PWM1_Handler
        B PWM1_Handler

        PUBWEAK PWMBRK_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PWMBRK_Handler
        B PWMBRK_Handler

        PUBWEAK SPI0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SPI0_Handler
        B SPI0_Handler

        PUBWEAK I2S0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2S0_Handler
        B I2S0_Handler

        PUBWEAK ADC0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
ADC0_Handler
        B ADC0_Handler

        PUBWEAK BTIMR0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BTIMR0_Handler
        B BTIMR0_Handler

        PUBWEAK BTIMR1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BTIMR1_Handler
        B BTIMR1_Handler

        PUBWEAK BTIMR2_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BTIMR2_Handler
        B BTIMR2_Handler

        PUBWEAK BTIMR3_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BTIMR3_Handler
        B BTIMR3_Handler

        PUBWEAK TIMR0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMR0_Handler
        B TIMR0_Handler

        PUBWEAK TIMR1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMR1_Handler
        B TIMR1_Handler

        PUBWEAK CAN0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
CAN0_Handler
        B CAN0_Handler

        PUBWEAK I2C0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2C0_Handler
        B I2C0_Handler

        PUBWEAK I2C1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2C1_Handler
        B I2C1_Handler

        PUBWEAK DMA_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMA_Handler
        B DMA_Handler

        PUBWEAK DMA2D_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMA2D_Handler
        B DMA2D_Handler

        PUBWEAK BOD_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BOD_Handler
        B BOD_Handler

        PUBWEAK QSPI0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
QSPI0_Handler
        B QSPI0_Handler

        PUBWEAK QSPI1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
QSPI1_Handler
        B QSPI1_Handler

        PUBWEAK SRA_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SRA_Handler
        B SRA_Handler

        PUBWEAK DVP_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
DVP_Handler
        B DVP_Handler

        PUBWEAK LCD_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
LCD_Handler
        B LCD_Handler

        PUBWEAK JPEG_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
JPEG_Handler
        B JPEG_Handler

        PUBWEAK SDIO_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SDIO_Handler
        B SDIO_Handler

        PUBWEAK WDT_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
WDT_Handler
        B WDT_Handler

        PUBWEAK RTC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
RTC_Handler
        B RTC_Handler

        PUBWEAK EXTI0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
EXTI0_Handler
        B EXTI0_Handler

        PUBWEAK EXTI1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
EXTI1_Handler
        B EXTI1_Handler

        PUBWEAK EXTI2_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
EXTI2_Handler
        B EXTI2_Handler

        PUBWEAK EXTI3_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
EXTI3_Handler
        B EXTI3_Handler

        PUBWEAK EXTI4_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
EXTI4_Handler
        B EXTI4_Handler

        PUBWEAK EXTI5_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
EXTI5_Handler
        B EXTI5_Handler

        PUBWEAK EXTI6_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
EXTI6_Handler
        B EXTI6_Handler

        PUBWEAK EXTI7_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
EXTI7_Handler
        B EXTI7_Handler

        PUBWEAK EXTI8_11_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
EXTI8_11_Handler
        B EXTI8_11_Handler

        PUBWEAK EXTI12_15_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
EXTI12_15_Handler
        B EXTI12_15_Handler

        END
