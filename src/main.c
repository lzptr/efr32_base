/**
 *  Simple blinky example using the sdk_support library on a thunderboard sense 2
 * 
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"

#define RED 8
#define GREEN 9

// Default clock value (see p. 319 TRM)
#define HFRCO_IN_HZ 1000000
#define TIMER_1S 1*(HFRCO_IN_HZ)

int main() 
{
  CHIP_Init();

    CMU_ClockEnable(cmuClock_GPIO, true);

    // Initialize LED driver
    GPIO_PinModeSet(gpioPortD, RED, gpioModePushPull, 0);
    GPIO_PinOutSet(gpioPortD, RED);

    while (1)
    {
        // Ca. 1s blinking, depending on the oscillator setting
        for (int i = 0; i < TIMER_1S; i++)
        {
        }
        GPIO_PinOutToggle(gpioPortD, RED);
    }
}
