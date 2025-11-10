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


; Amount of memory (in bytes) allocated for Stack
; Tailor this value to your application needs
; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00002000

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp               ; Top of Stack
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
				DCD	   XTALSTOP_Handler
				DCD	   RDMA_Handler

__Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors



                AREA    |.text|, CODE, READONLY


Reset_Handler    PROC
                 EXPORT  Reset_Handler             [WEAK]
				 IMPORT  __main
                 LDR     R0, =__main
                 BX      R0
                 ENDP

; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler                [WEAK]
                B       .
                ENDP

HardFault_Handler PROC
                EXPORT  HardFault_Handler          [WEAK]
                B       .
                ENDP

MemManage_Handler PROC
                EXPORT  MemManage_Handler          [WEAK]
                B       .
                ENDP

BusFault_Handler PROC
                EXPORT  BusFault_Handler           [WEAK]
                B       .
                ENDP

UsageFault_Handler PROC
                EXPORT  UsageFault_Handler         [WEAK]
                B       .
                ENDP

SVC_Handler     PROC
                EXPORT  SVC_Handler                [WEAK]
                B       .
                ENDP

DebugMon_Handler PROC
                EXPORT  DebugMon_Handler           [WEAK]
                B       .
                ENDP

PendSV_Handler  PROC
                EXPORT  PendSV_Handler             [WEAK]
                B       .
                ENDP

SysTick_Handler PROC
                EXPORT  SysTick_Handler            [WEAK]
                B       .
                ENDP

UART0_Handler  PROC
                EXPORT  UART0_Handler      [WEAK]
                B       .
                ENDP

UART1_Handler  PROC
                EXPORT  UART1_Handler      [WEAK]
                B       .
                ENDP

UART2_Handler  PROC
                EXPORT  UART2_Handler      [WEAK]
                B       .
                ENDP

UART3_Handler  PROC
                EXPORT  UART3_Handler      [WEAK]
                B       .
                ENDP

UART4_Handler  PROC
                EXPORT  UART4_Handler      [WEAK]
                B       .
                ENDP

PWM0_Handler  PROC
                EXPORT  PWM0_Handler       [WEAK]
                B       .
                ENDP

PWM1_Handler  PROC
                EXPORT  PWM1_Handler       [WEAK]
                B       .
                ENDP

PWMBRK_Handler  PROC
                EXPORT  PWMBRK_Handler     [WEAK]
                B       .
                ENDP

SPI0_Handler  PROC
                EXPORT  SPI0_Handler       [WEAK]
                B       .
                ENDP

I2S0_Handler  PROC
                EXPORT  I2S0_Handler       [WEAK]
                B       .
                ENDP

ADC0_Handler  PROC
                EXPORT  ADC0_Handler       [WEAK]
                B       .
                ENDP

BTIMR0_Handler  PROC
                EXPORT  BTIMR0_Handler     [WEAK]
                B       .
                ENDP

BTIMR1_Handler  PROC
                EXPORT  BTIMR1_Handler     [WEAK]
                B       .
                ENDP

BTIMR2_Handler  PROC
                EXPORT  BTIMR2_Handler     [WEAK]
                B       .
                ENDP

BTIMR3_Handler  PROC
                EXPORT  BTIMR3_Handler     [WEAK]
                B       .
                ENDP

TIMR0_Handler  PROC
                EXPORT  TIMR0_Handler      [WEAK]
                B       .
                ENDP

TIMR1_Handler  PROC
                EXPORT  TIMR1_Handler      [WEAK]
                B       .
                ENDP

CAN0_Handler  PROC
                EXPORT  CAN0_Handler       [WEAK]
                B       .
                ENDP

I2C0_Handler  PROC
                EXPORT  I2C0_Handler       [WEAK]
                B       .
                ENDP

I2C1_Handler  PROC
                EXPORT  I2C1_Handler       [WEAK]
                B       .
                ENDP

DMA_Handler  PROC
                EXPORT  DMA_Handler        [WEAK]
                B       .
                ENDP

DMA2D_Handler  PROC
                EXPORT  DMA2D_Handler      [WEAK]
                B       .
                ENDP

BOD_Handler  PROC
                EXPORT  BOD_Handler        [WEAK]
                B       .
                ENDP

QSPI0_Handler  PROC
                EXPORT  QSPI0_Handler      [WEAK]
                B       .
                ENDP

QSPI1_Handler  PROC
                EXPORT  QSPI1_Handler      [WEAK]
                B       .
                ENDP

SRA_Handler  PROC
                EXPORT  SRA_Handler        [WEAK]
                B       .
                ENDP

DVP_Handler  PROC
                EXPORT  DVP_Handler        [WEAK]
                B       .
                ENDP

LCD_Handler  PROC
                EXPORT  LCD_Handler        [WEAK]
                B       .
                ENDP

JPEG_Handler  PROC
                EXPORT  JPEG_Handler       [WEAK]
                B       .
                ENDP

SDIO_Handler  PROC
                EXPORT  SDIO_Handler       [WEAK]
                B       .
                ENDP

WDT_Handler  PROC
                EXPORT  WDT_Handler        [WEAK]
                B       .
                ENDP

RTC_Handler  PROC
                EXPORT  RTC_Handler        [WEAK]
                B       .
                ENDP

EXTI0_Handler  PROC
                EXPORT  EXTI0_Handler      [WEAK]
                B       .
                ENDP

EXTI1_Handler  PROC
                EXPORT  EXTI1_Handler      [WEAK]
                B       .
                ENDP

EXTI2_Handler  PROC
                EXPORT  EXTI2_Handler      [WEAK]
                B       .
                ENDP

EXTI3_Handler  PROC
                EXPORT  EXTI3_Handler      [WEAK]
                B       .
                ENDP

EXTI4_Handler  PROC
                EXPORT  EXTI4_Handler      [WEAK]
                B       .
                ENDP

EXTI5_Handler  PROC
                EXPORT  EXTI5_Handler      [WEAK]
                B       .
                ENDP

EXTI6_Handler  PROC
                EXPORT  EXTI6_Handler      [WEAK]
                B       .
                ENDP

EXTI7_Handler  PROC
                EXPORT  EXTI7_Handler      [WEAK]
                B       .
                ENDP

EXTI8_11_Handler  PROC
                EXPORT  EXTI8_11_Handler   [WEAK]
                B       .
                ENDP

EXTI12_15_Handler  PROC
                EXPORT  EXTI12_15_Handler  [WEAK]
                B       .
                ENDP

XTALSTOP_Handler  PROC
                EXPORT  XTALSTOP_Handler   [WEAK]
                B       .
                ENDP

RDMA_Handler  PROC
                EXPORT  RDMA_Handler	   [WEAK]
                B       .
                ENDP

                ALIGN


;*******************************************************************************
; User Stack and Heap initialization
;*******************************************************************************
                IF      :DEF:__MICROLIB
                
                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit
                
                ELSE
                
                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
                
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, = (Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, =  Stack_Mem
                BX      LR

                ALIGN

                ENDIF

                END
