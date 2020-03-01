/* ----------------------------------------------------------------------------
 * Copyright (c) 2015-2017 Semiconductor Components Industries, LLC (d/b/a
 * ON Semiconductor), All Rights Reserved
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor.
 * The terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 *
 * This is Reusable Code.
 *
 * ----------------------------------------------------------------------------
 * startup.s
 * - GCC compatible CMSIS Cortex-M3 Core Device start-up file for the  
 *   ARM Cortex-M3 processor
 *
 * - Assumes the use of CodeSourcery/Mentor Sourcery G++ with its CodeSourcery
 *   Common Startup Code Sequence (CS3) naming and conventions.  Note, however,
 *   that the default reset handler, Reset_Handler, does not follow CS3
 *   conventions because it calls a routine (SystemInit) that is typically
 *   written in C.  The reset handler should (in general) not assume that the
 *   stack pointer is set up already for C functions, even though it is set up
 *   at reset by the Cortex-M3 hardware.  The CS3 _start() function that
 *   Reset_Handler calls will set up the stack pointer again, before calling
 *   __cs3_start_c to initialize the C program's data areas and call main().
 * ----------------------------------------------------------------------------
 * $Revision: 1.2 $
 * $Date: 2017/08/10 15:18:40 $
 * ------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Stack Configuration
 * ------------------------------------------------------------------------- */
    .equ    Stack_Size, 1024  /* Bytes */
    .section ".stack", "w"
    .align  3
    .globl  __stack_mem
    .globl  __stack_size
__stack_mem:
    .if     Stack_Size
    .space  Stack_Size
    .endif
    .size   __stack_mem,  . - __stack_mem
    .set    __stack_size, . - __stack_mem


/* ----------------------------------------------------------------------------
 * Heap Configuration
 * ------------------------------------------------------------------------- */
    .equ    Heap_Size,  0  /* Bytes - heap is empty by default */

    .section ".heap", "w"
    .align  3
    .globl  __heap_start
    .globl  __heap_end
__heap_start:
    .if     Heap_Size
    .space  Heap_Size
    .endif
__heap_end:


/* ----------------------------------------------------------------------------
 * Vector Table
 * - Mapped to address 0 at Reset
 * ------------------------------------------------------------------------- */

    .section ".interrupt_vector"
    .globl  ISR_Vector_Table
    .type   ISR_Vector_Table, %object


ISR_Vector_Table:
    .long   __stack                         /*  0 Initial Stack Pointer */
    .long   Reset_Handler                   /*  1 Reset Handler */
    .long   NMI_Handler                     /*  2 Non-Maskable Interrupt Handler */
    .long   HardFault_Handler               /*  3 Hard Fault Handler */
    .long   MemManage_Handler               /*  4 Mem Manage Fault Handler */
    .long   BusFault_Handler                /*  5 Bus Fault Handler */
    .long   UsageFault_Handler              /*  6 Usage Fault Handler */
    .long   0                               /*  7 Reserved */
    .long   0                               /*  8 Reserved */
    .long   0                               /*  9 Reserved */
    .long   0                               /* 10 Reserved */
    .long   SVC_Handler                     /* 11 Supervisor Call Handler */
    .long   DebugMon_Handler                /* 12 Debug Monitor Handler */
    .long   0                               /* 13 Reserved */
    .long   PendSV_Handler                  /* 14 PendSV Handler */
    .long   SysTick_Handler                 /* 15 SysTick Handler */

    /* External Interrupts */
    .long  WAKEUP_IRQHandler               /* 16 Wake-up interrupt */
    .long  RTC_ALARM_IRQHandler            /* 17 RTC alarm interrupt */
    .long  RTC_CLOCK_IRQHandler            /* 18 RTL clock interrupt */
    .long  ADC_BATMON_IRQHandler           /* 19 ADC / Battery monitor interrupt */
    .long  TIMER0_IRQHandler               /* 20 Timer0 interrupt */
    .long  TIMER1_IRQHandler               /* 21 Timer1 interrupt */
    .long  TIMER2_IRQHandler               /* 22 Timer2 interrupt */
    .long  TIMER3_IRQHandler               /* 23 Timer3 interrupt */
    .long  DMA0_IRQHandler                 /* 24 DMA channel 0 interrupt */
    .long  DMA1_IRQHandler                 /* 25 DMA channel 1 interrupt */
    .long  DMA2_IRQHandler                 /* 26 DMA channel 2 interrupt */
    .long  DMA3_IRQHandler                 /* 27 DMA channel 3 interrupt */
    .long  DMA4_IRQHandler                 /* 28 DMA channel 4 interrupt */
    .long  DMA5_IRQHandler                 /* 29 DMA channel 5 interrupt */
    .long  DMA6_IRQHandler                 /* 30 DMA channel 6 interrupt */
    .long  DMA7_IRQHandler                 /* 31 DMA channel 7 interrupt */
    .long  DIO0_IRQHandler                 /* 32 DIO0 interrupt */
    .long  DIO1_IRQHandler                 /* 33 DIO1 interrupt */
    .long  DIO2_IRQHandler                 /* 34 DIO2 interrupt */
    .long  DIO3_IRQHandler                 /* 35 DIO3 interrupt */
    .long  WATCHDOG_IRQHandler             /* 36 Watchdog interrupt */
    .long  SPI0_RX_IRQHandler              /* 37 SPI0 receive interrupt */
    .long  SPI0_TX_IRQHandler              /* 38 SPI0 transmit interrupt */
    .long  SPI0_ERROR_IRQHandler           /* 39 SPI0 error interrupt */
    .long  SPI1_RX_IRQHandler              /* 40 SPI1 receive interrupt */
    .long  SPI1_TX_IRQHandler              /* 41 SPI1 transmit interrupt */
    .long  SPI1_ERROR_IRQHandler           /* 42 SPI1 error interrupt */
    .long  I2C_IRQHandler                  /* 43 I2C interrupt */
    .long  UART_RX_IRQHandler              /* 44 UART receive interrupt */
    .long  UART_TX_IRQHandler              /* 45 UART transmit interrupt */
    .long  UART_ERROR_IRQHandler           /* 46 UART error interrupt */
    .long  DMIC_OUT_OD_IN_IRQHandler       /* 47 DMIC and output driver data ready interrupt */
    .long  DMIC_OD_ERROR_IRQHandler        /* 48 DMIC overrun and output driver underrun detect interrupt */
    .long  PCM_RX_IRQHandler               /* 49 PCM receive interrupt */
    .long  PCM_TX_IRQHandler               /* 50 PCM transmit interrupt */
    .long  PCM_ERROR_IRQHandler            /* 51 PCM error interrupt */
    .long  DSP0_IRQHandler                 /* 52 DSP event interrupt 0 */
    .long  DSP1_IRQHandler                 /* 53 DSP event interrupt 1 */
    .long  DSP2_IRQHandler                 /* 54 DSP event interrupt 2 */
    .long  DSP3_IRQHandler                 /* 55 DSP event interrupt 3 */
    .long  DSP4_IRQHandler                 /* 56 DSP event interrupt 4 */
    .long  DSP5_IRQHandler                 /* 57 DSP event interrupt 5 */
    .long  DSP6_IRQHandler                 /* 58 DSP event interrupt 6 */
    .long  DSP7_IRQHandler                 /* 59 DSP event interrupt 7 */
    .long  BLE_CSCNT_IRQHandler            /* 60 BLE 625us time reference interrupt */
    .long  BLE_SLP_IRQHandler              /* 61 BLE sleep mode interrupt */
    .long  BLE_RX_IRQHandler               /* 62 BLE received packet interrupt */
    .long  BLE_EVENT_IRQHandler            /* 63 BLE event interrupt */
    .long  BLE_CRYPT_IRQHandler            /* 64 BLE AES ciphering complete interrupt */
    .long  BLE_ERROR_IRQHandler            /* 65 BLE baseband error interrupt */
    .long  BLE_GROSSTGTIM_IRQHandler       /* 66 BLE gross timer interrupt */
    .long  BLE_FINETGTIM_IRQHandler        /* 67 BLE fine timer interrupt */
    .long  BLE_SW_IRQHandler               /* 68 BLE SW triggered inerrupt */
    .long  BLE_COEX_RX_TX_IRQHandler       /* 69 Coexistance BLE start/stop RX or TX interrupt */
    .long  BLE_COEX_IN_PROCESS_IRQHandler  /* 70 Coexistance BLE in process interrupt */
    .long  RF_TX_IRQHandler                /* 71 BLE transmit interrupt */
    .long  RF_RXSTOP_IRQHandler            /* 72 BLE receive stop interrupt */
    .long  RF_IRQ_RECEIVED_IRQHandler      /* 73 BLE received packet interrupt */
    .long  RF_SYNC_IRQHandler              /* 74 BLE received sync word interrupt */
    .long  RF_TXFIFO_IRQHandler            /* 75 TX FIFO near underflow detect interrupt */
    .long  RF_RXFIFO_IRQHandler            /* 76 RX FIFO near overflow detect interrupt */
    .long  ASRC_ERROR_IRQHandler           /* 77 ASRC error interrupt */
    .long  ASRC_IN_IRQHandler              /* 78 ASRC data input interrupt */
    .long  ASRC_OUT_IRQHandler             /* 79 ASRC data output interrupt */
    .long  AUDIOSINK_PHASE_IRQHandler      /* 80 Audio sink clock phase interrupt */
    .long  AUDIOSINK_PERIOD_IRQHandler     /* 81 Audio sink clock period interrupt */
    .long  CLKDET_IRQHandler               /* 82 Clock detection interrupt */
    .long  FLASH_COPY_IRQHandler           /* 83 Flash copy interrupt */
    .long  FLASH_ECC_IRQHandler            /* 84 Flash ECC event interrupt */
    .long  MEMORY_ERROR_IRQHandler         /* 85 Memory error event interrupt */
#if RSL10_CID == 101
    .long  BLE_AUDIO0_IRQHandler           /* 86 Audio over BLE channel 0 interrupt */
    .long  BLE_AUDIO1_IRQHandler           /* 87 Audio over BLE channel 1 interrupt */
    .long  BLE_AUDIO2_IRQHandler           /* 88 Audio over BLE channel 2 interrupt */
#endif
    .size   ISR_Vector_Table, . - ISR_Vector_Table

    .thumb

/* ----------------------------------------------------------------------------
 * Reset Handler
 * ------------------------------------------------------------------------- */

    .section .reset,"x",%progbits
    .thumb_func
    .globl  Reset_Handler
    .type   Reset_Handler, %function
Reset_Handler:
    .fnstart
    MOV R0, SP
    BFC R0, #2, #1
    MOV SP, R0
    LDR     R0, =SystemInit
    BLX     R0
    LDR     R0, =_start
    BX      R0
    .pool
    .cantunwind
    .fnend
    .size   Reset_Handler, . - Reset_Handler

    .section ".text"

/* ----------------------------------------------------------------------------
 * Place-holder Exception Handlers
 * - Weak definitions
 * - If not modified or replaced, these handlers implement infinite loops
 * ------------------------------------------------------------------------- */
    .weak   NMI_Handler
    .type   NMI_Handler, %function
NMI_Handler:
    B       .
    .size   NMI_Handler, . - NMI_Handler

    .weak   HardFault_Handler
    .type   HardFault_Handler, %function
HardFault_Handler:
    B       .
    .size   HardFault_Handler, . - HardFault_Handler

    .weak   MemManage_Handler
    .type   MemManage_Handler, %function
MemManage_Handler:
    B       .
    .size   MemManage_Handler, . - MemManage_Handler

    .weak   BusFault_Handler
    .type   BusFault_Handler, %function
BusFault_Handler:
    B       .
    .size   BusFault_Handler, . - BusFault_Handler

    .weak   UsageFault_Handler
    .type   UsageFault_Handler, %function
UsageFault_Handler:
    B       .
    .size   UsageFault_Handler, . - UsageFault_Handler

    .weak   SVC_Handler
    .type   SVC_Handler, %function
SVC_Handler:
    B       .
    .size   SVC_Handler, . - SVC_Handler

    .weak   DebugMon_Handler
    .type   DebugMon_Handler, %function
DebugMon_Handler:
    B       .
    .size   DebugMon_Handler, . - DebugMon_Handler

    .weak   PendSV_Handler
    .type   PendSV_Handler, %function
PendSV_Handler:
    B       .
    .size   PendSV_Handler, . - PendSV_Handler

    .weak   SysTick_Handler
    .type   SysTick_Handler, %function
SysTick_Handler:
    B       .
    .size   SysTick_Handler, . - SysTick_Handler

/* ----------------------------------------------------------------------------
 * Default Interrupt Request Handlers for External Interrupts
 * ------------------------------------------------------------------------- */

    .globl  Default_Handler
    .type   Default_Handler, %function
Default_Handler:
    B       .
    .size   Default_Handler, . - Default_Handler

    .macro  IRQ handler
    .weak   \handler
    .set    \handler, Default_Handler
    .endm
    IRQ     WAKEUP_IRQHandler
    IRQ     RTC_ALARM_IRQHandler
    IRQ     RTC_CLOCK_IRQHandler
    IRQ     ADC_BATMON_IRQHandler
    IRQ     TIMER0_IRQHandler
    IRQ     TIMER1_IRQHandler
    IRQ     TIMER2_IRQHandler
    IRQ     TIMER3_IRQHandler
    IRQ     DMA0_IRQHandler
    IRQ     DMA1_IRQHandler
    IRQ     DMA2_IRQHandler
    IRQ     DMA3_IRQHandler
    IRQ     DMA4_IRQHandler
    IRQ     DMA5_IRQHandler
    IRQ     DMA6_IRQHandler
    IRQ     DMA7_IRQHandler
    IRQ     DIO0_IRQHandler
    IRQ     DIO1_IRQHandler
    IRQ     DIO2_IRQHandler
    IRQ     DIO3_IRQHandler
    IRQ     WATCHDOG_IRQHandler
    IRQ     SPI0_RX_IRQHandler
    IRQ     SPI0_TX_IRQHandler
    IRQ     SPI0_ERROR_IRQHandler
    IRQ     SPI1_RX_IRQHandler
    IRQ     SPI1_TX_IRQHandler
    IRQ     SPI1_ERROR_IRQHandler
    IRQ     I2C_IRQHandler
    IRQ     UART_RX_IRQHandler
    IRQ     UART_TX_IRQHandler
    IRQ     UART_ERROR_IRQHandler
    IRQ     DMIC_OUT_OD_IN_IRQHandler
    IRQ     DMIC_OD_ERROR_IRQHandler
    IRQ     PCM_RX_IRQHandler
    IRQ     PCM_TX_IRQHandler
    IRQ     PCM_ERROR_IRQHandler
    IRQ     DSP0_IRQHandler
    IRQ     DSP1_IRQHandler
    IRQ     DSP2_IRQHandler
    IRQ     DSP3_IRQHandler
    IRQ     DSP4_IRQHandler
    IRQ     DSP5_IRQHandler
    IRQ     DSP6_IRQHandler
    IRQ     DSP7_IRQHandler
    IRQ     BLE_CSCNT_IRQHandler
    IRQ     BLE_SLP_IRQHandler
    IRQ     BLE_RX_IRQHandler
    IRQ     BLE_EVENT_IRQHandler
    IRQ     BLE_CRYPT_IRQHandler
    IRQ     BLE_ERROR_IRQHandler
    IRQ     BLE_GROSSTGTIM_IRQHandler
    IRQ     BLE_FINETGTIM_IRQHandler
    IRQ     BLE_SW_IRQHandler
    IRQ     BLE_COEX_RX_TX_IRQHandler
    IRQ     BLE_COEX_IN_PROCESS_IRQHandler
    IRQ     RF_TX_IRQHandler
    IRQ     RF_RXSTOP_IRQHandler
    IRQ     RF_IRQ_RECEIVED_IRQHandler
    IRQ     RF_SYNC_IRQHandler
    IRQ     RF_TXFIFO_IRQHandler
    IRQ     RF_RXFIFO_IRQHandler
    IRQ     ASRC_ERROR_IRQHandler
    IRQ     ASRC_IN_IRQHandler
    IRQ     ASRC_OUT_IRQHandler
    IRQ     AUDIOSINK_PHASE_IRQHandler
    IRQ     AUDIOSINK_PERIOD_IRQHandler
    IRQ     CLKDET_IRQHandler
    IRQ     FLASH_COPY_IRQHandler
    IRQ     FLASH_ECC_IRQHandler
    IRQ     MEMORY_ERROR_IRQHandler
#if RSL10_CID == 101
    IRQ     BLE_AUDIO0_IRQHandler
    IRQ     BLE_AUDIO1_IRQHandler
    IRQ     BLE_AUDIO2_IRQHandler
#endif
    .end

