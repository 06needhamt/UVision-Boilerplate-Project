//STM32 MCU specific definitions
#include "stm32f10x.h"                  /* STM32F10x.h definitions            */


/*----------------------------------------------------------------------------
  Function that initializes LEDs - kind of a LED driver!
 *----------------------------------------------------------------------------*/
void LED_init(void) {
	//Peripheral clock enable register (APB2ENR) is part of
	//Reset and Clock Control (RCC) register. Bit 3 when set
	//enables the I/O Port B clock which is required for Port B
	//to function.
  	RCC->APB2ENR |= (1UL << 3);           /* Enable GPIOB clock                 */

	//The KEIL evaluation board MCBSTM32EXL has 8 green LEDs.
	//These LEDs are connected to MCU's PB8 to PB15 (see the board shematic).

	//The procedure for configuring PB8 to PB15 as output pins:
	//1. PortB's data register ODR defines bits 0 to 15 as output pins.
	//   The following statement resets PB8 to PB15, i.e. LEDs are off.
  	GPIOB->ODR   &= ~0x0000FF00;          /* switch off LEDs                    */
	//2. PortB's configuration register CRH is used to define which PortB
	//   data pins will be configured as output pins. First clear the CRH
	//   register - this sets the pins to input mode.
  	GPIOB->CRH   &= ~0xFFFFFFFF;          /* Configure the GPIO for LEDs        */
	//3. Now configure pins PB8 to PB15 to general purpose output mode (see MCU spec.).
	//   Mode bits set to bin 11 (i.e. output mode) and the configuration bits are set
	//   to bin 00 (i.e. general purpose output).
  	GPIOB->CRH   |=  0x33333333;
}

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int num) {

  	//Do the following to switch on a LED:
	//Set PortB Bit Set/Reset Register (BSSR) to output a signal on corresponding port pin
	//which is used to active the LED (i.e. switch it on) connected to that pin.
	//Here, num is a number from 0 to 8 for LED0 to LED7. Since register bits for LEDs
	//start at bit 8, we need to add 8 to num in order to set the correct LED output data
	//bit using the following bit shift statement.
  	GPIOB->BSRR |= (1UL << num + 8);
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int num) {

  	//PortB Bit Reset Register (BRR) is used to switch off output data signals to
	//the LEDs thus effectively turning them off. If you set a bit in this register
	//then the corresponding output pin is reset.
  	GPIOB->BRR |= (1UL << num + 8);
}

void All_LEDs_On() {
   GPIOB->BSRR |= 0x0000FF00;
}

void All_LEDs_Off() {
   GPIOB->BRR |= 0x0000FF00;
}
