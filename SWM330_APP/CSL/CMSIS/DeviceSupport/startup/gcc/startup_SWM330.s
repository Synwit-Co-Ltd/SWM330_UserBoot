    .syntax unified
    .arch armv7-m
	.thumb

/* Memory Model
   The HEAP starts at the end of the DATA section and grows upward.
   
   The STACK starts at the end of the RAM and grows downward     */
    .section .stack
    .align 3
    .globl    __StackTop
    .globl    __StackLimit
__StackLimit:
    .space    0x4000
__StackTop:


    .section .heap
    .align 3
    .globl    __HeapBase
    .globl    __HeapLimit
__HeapBase:
    .space    0x1000
__HeapLimit:


    .section .isr_vector
    .align 2
    .globl __isr_vector
__isr_vector:
    .long    __StackTop            
    .long    Reset_Handler         
    .long    NMI_Handler          
    .long    HardFault_Handler     
    .long    MemManage_Handler     
    .long    BusFault_Handler      
    .long    UsageFault_Handler   
    .long    0                    
    .long    0                    
    .long    0                    
    .long    0                     
    .long    SVC_Handler          
    .long    DebugMon_Handler     
    .long    0                     
    .long    PendSV_Handler           
    .long    SysTick_Handler         

    /* External interrupts */
    .long    UART0_Handler
    .long    UART1_Handler
    .long    UART2_Handler
    .long    UART3_Handler
    .long    UART4_Handler
    .long    PWM0_Handler
    .long    PWM1_Handler
    .long    PWMBRK_Handler
    .long    SPI0_Handler
    .long    I2S0_Handler
    .long    ADC0_Handler
    .long    BTIMR0_Handler
    .long    BTIMR1_Handler
    .long    BTIMR2_Handler
    .long    BTIMR3_Handler
    .long    TIMR0_Handler
    .long    TIMR1_Handler
    .long    CAN0_Handler
    .long    I2C0_Handler
    .long    I2C1_Handler
    .long    DMA_Handler
    .long    DMA2D_Handler
    .long    BOD_Handler
    .long    QSPI0_Handler
    .long    QSPI1_Handler
    .long    SRA_Handler
    .long    DVP_Handler
    .long    LCD_Handler
    .long    JPEG_Handler
    .long    SDIO_Handler
    .long    WDT_Handler
    .long    RTC_Handler
    .long    EXTI0_Handler
    .long    EXTI1_Handler
    .long    EXTI2_Handler
    .long    EXTI3_Handler
    .long    EXTI4_Handler
    .long    EXTI5_Handler
    .long    EXTI6_Handler
    .long    EXTI7_Handler
    .long    EXTI8_11_Handler
    .long    EXTI12_15_Handler


	.section .text.Reset_Handler
    .align 2
    .globl    Reset_Handler
    .type     Reset_Handler, %function
Reset_Handler:
/* Loop to copy data from read only memory to RAM. The ranges
 * of copy from/to are specified by symbols evaluated in linker script.  */
    ldr    sp, =__StackTop    		 /* set stack pointer */

    ldr    r1, =__data_load__
    ldr    r2, =__data_start__
    ldr    r3, =__data_end__

.Lflash_to_ram_loop:
    cmp     r2, r3
    ittt    lo
    ldrlo   r0, [r1], #4
    strlo   r0, [r2], #4
    blo    .Lflash_to_ram_loop


    ldr    r2, =__bss_start__
    ldr    r3, =__bss_end__

.Lbss_to_ram_loop:
    cmp     r2, r3
    ittt    lo
    movlo   r0, #0
    strlo   r0, [r2], #4
    blo    .Lbss_to_ram_loop

    ldr    r0, =main
    bx     r0
    .pool    


    .text
/* Macro to define default handlers. 
   Default handler will be weak symbol and just dead loops. */
    .macro    def_default_handler    handler_name
    .align 1
    .thumb_func
    .weak    \handler_name
    .type    \handler_name, %function
\handler_name :
    b    .
    .endm

    def_default_handler    NMI_Handler
    def_default_handler    HardFault_Handler
    def_default_handler    MemManage_Handler
    def_default_handler    BusFault_Handler
    def_default_handler    UsageFault_Handler
    def_default_handler    SVC_Handler
    def_default_handler    DebugMon_Handler
    def_default_handler    PendSV_Handler
    def_default_handler    SysTick_Handler

    def_default_handler    UART0_Handler
    def_default_handler    UART1_Handler
    def_default_handler    UART2_Handler
    def_default_handler    UART3_Handler
    def_default_handler    UART4_Handler
    def_default_handler    PWM0_Handler
    def_default_handler    PWM1_Handler
    def_default_handler    PWMBRK_Handler
    def_default_handler    SPI0_Handler
    def_default_handler    I2S0_Handler
    def_default_handler    ADC0_Handler
    def_default_handler    BTIMR0_Handler
    def_default_handler    BTIMR1_Handler
    def_default_handler    BTIMR2_Handler
    def_default_handler    BTIMR3_Handler
    def_default_handler    TIMR0_Handler
    def_default_handler    TIMR1_Handler
    def_default_handler    CAN0_Handler
    def_default_handler    I2C0_Handler
    def_default_handler    I2C1_Handler
    def_default_handler    DMA_Handler
    def_default_handler    DMA2D_Handler
    def_default_handler    BOD_Handler
    def_default_handler    QSPI0_Handler
    def_default_handler    QSPI1_Handler
    def_default_handler    SRA_Handler
    def_default_handler    DVP_Handler
    def_default_handler    LCD_Handler
    def_default_handler    JPEG_Handler
    def_default_handler    SDIO_Handler
    def_default_handler    WDT_Handler
    def_default_handler    RTC_Handler
    def_default_handler    EXTI0_Handler
    def_default_handler    EXTI1_Handler
    def_default_handler    EXTI2_Handler
    def_default_handler    EXTI3_Handler
    def_default_handler    EXTI4_Handler
    def_default_handler    EXTI5_Handler
    def_default_handler    EXTI6_Handler
    def_default_handler    EXTI7_Handler
    def_default_handler    EXTI8_11_Handler
    def_default_handler    EXTI12_15_Handler

    def_default_handler    Default_Handler

    .end
