#include <stdint.h>


volatile uint32_t* const CMU_ClockBase = (uint32_t*)0x400E4000;
volatile uint32_t* const CMU_HFBUSCLKEN0 = (uint32_t*)0x400E40B0;
uint32_t const CMU_GPIO_PORT = 3;

volatile uint32_t* const GPIO_PortD_Base = (uint32_t*)0x4000A090;
volatile uint32_t* const GPIO_PortD_PinModeHigh = (uint32_t*)0x4000A098;
volatile uint32_t* const GPIO_PortD_DOUT = (uint32_t*)0x4000A09C;
volatile uint32_t* const GPIO_PortD_DIN = (uint32_t*)0x4000A0AC;

#define PB0 14
#define PB1 15

#define LED_RED 8
#define LED_GREEN 9


void initGPIO(void) 
{
  // Enable GPIO clock
  *CMU_HFBUSCLKEN0 |= (1 << CMU_GPIO_PORT);
  
  // Set Port D, Pin15 (PB1) and Pin14 (PB0) as input
  uint8_t inputMode = 1; // Input
  *GPIO_PortD_PinModeHigh = (0x0FFFFFFF & *GPIO_PortD_PinModeHigh) | (inputMode << 28u);
  *GPIO_PortD_PinModeHigh = (0xF0FFFFFF & *GPIO_PortD_PinModeHigh) | (inputMode << 24u);

  inputMode = 4; // Push Pull 
  *GPIO_PortD_PinModeHigh = (0xFFFFFF0F & *GPIO_PortD_PinModeHigh) | (inputMode << 4u);
  *GPIO_PortD_PinModeHigh = (0xFFFFFFF0 & *GPIO_PortD_PinModeHigh) | (inputMode);
}

int main() 
{

  // Initializations
  initGPIO();


  while (1) 
  {
    // Buttons are active low
    if ( (*GPIO_PortD_DIN & (1 << PB0)) == 0)
    {
      // LED0 On
      *GPIO_PortD_DOUT |= (1 << LED_RED); 
    }
    else
    {
      // LED0 Off
      *GPIO_PortD_DOUT &= ~(1 << LED_RED);
    }

    if ((*GPIO_PortD_DIN & (1 << PB1)) == 0)
    {
      // LED1 On
      *GPIO_PortD_DOUT |= (1 << LED_GREEN); 
    }
    else
    {
      // LED1 Off
      *GPIO_PortD_DOUT &= ~(1 << LED_GREEN); 
    }
  }
}
