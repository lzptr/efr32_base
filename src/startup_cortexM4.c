#include <stdint.h>

//*****************************************************************************
//
// Forward declaration of the default fault handlers.
//
//*****************************************************************************
void ResetISR(void);
static void NmiSR(void);
static void FaultISR(void);
static void IntDefaultHandler(void);

//*****************************************************************************
//
// The entry point for the application.
//
//*****************************************************************************
extern int main(void);

//*****************************************************************************
//
// The following are constructs created by the linker, indicating where the
// the "data" and "bss" segments reside in memory.  The initializers for the
// for the "data" segment resides immediately following the "text" segment.
//
//*****************************************************************************
extern uint32_t _stack_ptr;
extern uint32_t __text_end_vma;
extern uint32_t __data_start_vma;
extern uint32_t __data_end_vma;
extern uint32_t __bss_start_vma;
extern uint32_t __bss_end_vma;

//*****************************************************************************
//
// The vector table.  Note that the proper constructs must be placed on this to
// ensure that it ends up at physical address 0x0000.0000.
//
//*****************************************************************************
__attribute__ ((section(".isr_vector")))
uintptr_t isrVectorTable[] =
{
	(uintptr_t )&_stack_ptr,                            // The initial stack pointer
	(uintptr_t )ResetISR,                               // The reset handler
	(uintptr_t )NmiSR,                                  // The NMI handler
	(uintptr_t )FaultISR,                               // The hard fault handler
	(uintptr_t )IntDefaultHandler,                      // The MPU fault handler
	(uintptr_t )IntDefaultHandler,                      // The bus fault handler
	(uintptr_t )IntDefaultHandler,                      // The usage fault handler
	(uintptr_t )0,                                      // Reserved
	(uintptr_t )0,                                      // Reserved
	(uintptr_t )0,                                      // Reserved
	(uintptr_t )0,                                      // Reserved
	(uintptr_t )IntDefaultHandler,                      // SVCall handler
	(uintptr_t )IntDefaultHandler,                      // Debug monitor handler
	(uintptr_t )0,                                      // Reserved
	(uintptr_t )IntDefaultHandler,                      // The PendSV handler
	(uintptr_t )IntDefaultHandler,                      // The SysTick handler

	/* External EFR32MG12 interrupts */
	(uintptr_t )IntDefaultHandler,                      // 0 - EMU       
	(uintptr_t )IntDefaultHandler,                      // 1 - FRC_PRI   
	(uintptr_t )IntDefaultHandler,                      // 2 - WDOG0     
	(uintptr_t )IntDefaultHandler,                      // 3 - WDOG1     
	(uintptr_t )IntDefaultHandler,                      // 4 - FRC       
	(uintptr_t )IntDefaultHandler,                      // 5 - MODEM     
	(uintptr_t )IntDefaultHandler,                      // 6 - RAC_SEQ   
	(uintptr_t )IntDefaultHandler,                      // 7 - RAC_RSM   
	(uintptr_t )IntDefaultHandler,                      // 8 - BUFC      
	(uintptr_t )IntDefaultHandler,                      // 9 - LDMA      
	(uintptr_t )IntDefaultHandler,                      // 10 - GPIO_EVEN
	(uintptr_t )IntDefaultHandler,                      // 11 - TIMER0   
	(uintptr_t )IntDefaultHandler,                      // 12 - USART0_RX
	(uintptr_t )IntDefaultHandler,                      // 13 - USART0_TX
	(uintptr_t )IntDefaultHandler,                      // 14 - ACMP0    
	(uintptr_t )IntDefaultHandler,                      // 15 - ADC0     
	(uintptr_t )IntDefaultHandler,                      // 16 - IDAC0    
	(uintptr_t )IntDefaultHandler,                      // 17 - I2C0     
	(uintptr_t )IntDefaultHandler,                      // 18 - GPIO_ODD 
	(uintptr_t )IntDefaultHandler,                      // 19 - TIMER1   
	(uintptr_t )IntDefaultHandler,                      // 20 - USART1_RX
	(uintptr_t )IntDefaultHandler,                      // 21 - USART1_TX
	(uintptr_t )IntDefaultHandler,                      // 22 - LEUART0  
	(uintptr_t )IntDefaultHandler,                      // 23 - PCNT0    
	(uintptr_t )IntDefaultHandler,                      // 24 - CMU      
	(uintptr_t )IntDefaultHandler,                      // 25 - MSC      
	(uintptr_t )IntDefaultHandler,                      // 26 - CRYPTO0  
	(uintptr_t )IntDefaultHandler,                      // 27 - LETIMER0 
	(uintptr_t )IntDefaultHandler,                      // 28 - AGC      
	(uintptr_t )IntDefaultHandler,                      // 29 - PROTIMER 
	(uintptr_t )IntDefaultHandler,                      // 30 - RTCC     
	(uintptr_t )IntDefaultHandler,                      // 31 - SYNTH    
	(uintptr_t )IntDefaultHandler,                      // 32 - CRYOTIMER
	(uintptr_t )IntDefaultHandler,                      // 33 - RFSENSE  
	(uintptr_t )IntDefaultHandler,                      // 34 - FPUEH    
	(uintptr_t )IntDefaultHandler,                      // 35 - SMU      
	(uintptr_t )IntDefaultHandler,                      // 36 - WTIMER0  
	(uintptr_t )IntDefaultHandler,                      // 37 - WTIMER1  
	(uintptr_t )IntDefaultHandler,                      // 38 - PCNT1    
	(uintptr_t )IntDefaultHandler,                      // 39 - PCNT2    
	(uintptr_t )IntDefaultHandler,                      // 40 - USART2_RX
	(uintptr_t )IntDefaultHandler,                      // 41 - USART2_TX
	(uintptr_t )IntDefaultHandler,                      // 42 - I2C1     
	(uintptr_t )IntDefaultHandler,                      // 43 - USART3_RX
	(uintptr_t )IntDefaultHandler,                      // 44 - USART3_TX
	(uintptr_t )IntDefaultHandler,                      // 45 - VDAC0    
	(uintptr_t )IntDefaultHandler,                      // 46 - CSEN     
	(uintptr_t )IntDefaultHandler,                      // 47 - LESENSE  
	(uintptr_t )IntDefaultHandler,                      // 48 - CRYPTO1  
	(uintptr_t )IntDefaultHandler,                      // 49 - TRNG0    
	(uintptr_t )0,                                      // 50 - Reserved 
	
};

//*****************************************************************************
//
// This is the code that gets called when the processor first starts execution
// following a reset event.  Only the absolutely necessary set is performed,
// after which the application supplied entry() routine is called.  Any fancy
// actions (such as making decisions based on the reset cause register, and
// resetting the bits in that register) are left solely in the hands of the
// application.
//
//*****************************************************************************
void ResetISR(void)
{
	uint32_t *memSrc, *memDestination;

	//
	// Copy the data segment initializers from flash to SRAM.
	//
	memSrc = &__text_end_vma;
	memDestination = &__data_start_vma;
	while(memDestination < &__data_end_vma)
	{
		*memDestination++ = *memSrc++;
	}

	memDestination = &__bss_start_vma;
	while(memDestination < &__bss_end_vma) 
	{
		*memDestination++ = 0;
	}

	//
	// Call the application's entry point.
	//
	main();
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a NMI.  This
// simply enters an infinite loop, preserving the system state for examination
// by a debugger.
//
//*****************************************************************************
static void NmiSR(void)
{
	//
	// Enter an infinite loop.
	//
	while(1)
	{
	}
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a fault
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
static void FaultISR(void)
{
	//
	// Enter an infinite loop.
	//
	while(1)
	{
	}
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives an unexpected
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
static void IntDefaultHandler(void)
{
	//
	// Go into an infinite loop.
	//
	while(1)
	{
	}
}
