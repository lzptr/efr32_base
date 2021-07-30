/**
 *  Simple UART example using the sdk_support library on a thunderboard sense 2
 * 
 */

#include <stdlib.h>
#include <stdint.h>
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "em_chip.h"


#define BUFFER_SIZE 80
#define  HFXOFrequency 32000000UL



int main() 
{
  CHIP_Init();
  
  // Enable oscillator to GPIO and USART2 modules
  CMU_ClockEnable(cmuClock_GPIO, true);
  CMU_ClockEnable(cmuClock_USART2, true);

  // set pin modes for UART TX and RX pins
  GPIO_PinModeSet(gpioPortA, 7, gpioModePushPull, 1);
  GPIO_PinModeSet(gpioPortA, 6, gpioModeInput, 0);

  // Initialize USART asynchronous mode and route pins
  USART_InitAsync_TypeDef init = {  usartEnable,           /* Enable RX/TX when initialization is complete. */                     \
                                    0,                     /* Use current configured reference clock for configuring baud rate. */ \
                                    9600,                /* 9600 bits/s. */                                                    \
                                    usartOVS16,            /* 16x oversampling. */                                                 \
                                    usartDatabits8,        /* 8 data bits. */                                                      \
                                    usartNoParity,         /* No parity. */                                                        \
                                    usartStopbits1,        /* 1 stop bit. */                                                       \
                                    false,                 /* Auto CS functionality enable/disable switch */                       \
                                  };
  USART_InitAsync(USART2, &init);
  USART2->ROUTELOC0 |= (USART_ROUTELOC0_RXLOC_LOC0) | USART_ROUTELOC0_TXLOC_LOC2;
  USART2->ROUTEPEN |= USART_ROUTEPEN_TXPEN | USART_ROUTEPEN_RXPEN;

  int i,j;
  char txBuffer[] = "Hello World";
  for (i = 0 ; txBuffer[i] != 0; i++)
  {
    USART_Tx(USART2, txBuffer[i]);
  }

  char buffer[BUFFER_SIZE];
  while (1) 
  {

    // Read a line from the UART
    for (i = 0; i < BUFFER_SIZE - 1 ; i++ )
    {
      buffer[i] = USART_Rx(USART2);
      if (buffer[i] == '\r')
      {
        break; // Breaking on CR prevents it from being counted in the number of bytes
      }
    }

    // Echo the line back, adding CRLF
    for (j = 0; j < i ; j ++ )
    {
      USART_Tx(USART2, buffer[j]);
    }

    USART_Tx(USART2, '\r');
    USART_Tx(USART2, '\f');
  }
}
