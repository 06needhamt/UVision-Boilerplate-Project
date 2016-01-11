/******************************************************************************/
/* IRQ.c: IRQ Handler                                                         */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2010 Keil - An ARM Company. All rights reserved.             */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/

#include "stm32f10x.h"                  /* STM32F10x.h definitions            */
#include "Button_Functions.h"

extern void Vectored_Interrupt(int amount);

unsigned short AD_last;                 /* Last converted value               */
unsigned char  AD_done = 0;             /* AD conversion done flag            */

unsigned long ticks;

unsigned char  clock_1s;                /* Flag activated each second         */

/*----------------------------------------------------------------------------
  Systick Interrupt Handler
  SysTick interrupt happens every 10 ms
 *----------------------------------------------------------------------------*/
void SysTick_Handler (void) {           /* SysTick Interrupt Handler (10ms)   */

	
	//1 sec = 100 x 10 msec, hence the 10 msec counter 'ticks'
//	if (ticks++ >= 1) {                /* Set clock_1s every 1/10 second    	  */
//		ticks    = 0;
//		clock_1s = 1;
//	}
		if (ticks > 0) {
		//Decrement the tick counter every 1 msec as long as it is greater than 0.
		ticks -= 1;
	}
}


/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is done
 *----------------------------------------------------------------------------*/
void ADC1_2_IRQHandler(void) {
  if (ADC1->SR & (1 << 1)) {            /* ADC1 EOC interrupt?                */
    AD_last = ADC1->DR;
    AD_done = 1;

    ADC1->SR &= ~(1 << 1);              /* clear EOC interrupt                */
  }

}


/*----------------------------------------------------------------------------
  EXTI Line 3 Interrupt Handler - Used to handle interrupts from
  the Joystick switch on PortD input source PD3 (see the board schematics)
 *----------------------------------------------------------------------------*/ 
void EXTI3_IRQHandler (void) {
  	if (EXTI->PR & (1<<3)) {
		//Yes, it is, so clear (i.e. reset) it.
		//If you don't do this then the interrupt
		//will keep occuring again and again as the
		//system thinks it is still pending!
    	EXTI->PR |= (1<<3);

		//Joystick DOWN
	  //joystick_int = JOYSTICK_DOWN;
			Vectored_Interrupt(JOYSTICK_DOWN);
   	}
}

/*----------------------------------------------------------------------------
  EXTI Line 9..5 Interrupt Handler - Used to handle interrupts from
  the User button on PortG input source PG8 (see the board schematics)
  and Joystick switch on PortG input source PG7.
 *----------------------------------------------------------------------------*/ 
void EXTI9_5_IRQHandler (void) {
	//Check to see if PG8 interrupt is pending
  	if (EXTI->PR & (1<<8)) {
		//Yes, it is, so clear (i.e. reset) it.
		//If you don't do this then the interrupt
		//will keep occuring again and again as the
		//system thinks it is still pending!
    	EXTI->PR |= (1<<8);
			Vectored_Interrupt(USER_BUTTON);
		//Set the flag to indicate button pressed status - need resetting later
		//user_btn_int = 1;
   	}

	//Check to see if PG7 interrupt is pending
  	if (EXTI->PR & (1<<7)) {
		//Yes, it is, so clear (i.e. reset) it.
		//If you don't do this then the interrupt
		//will keep occuring again and again as the
		//system thinks it is still pending!
    	EXTI->PR |= (1<<7);
			
			
		//Joystick SELECT
	  //joystick_int = JOYSTICK_SELECT;
			Vectored_Interrupt(JOYSTICK_SELECT);
   	}
}

/*----------------------------------------------------------------------------
  EXTI Line 15..9 Interrupt Handler - Used to handle interrupts from
  the Joystick switches on PortG input sources PG15-13 (see the board schematics)
 *----------------------------------------------------------------------------*/ 
void EXTI15_10_IRQHandler (void) {
	//Check to see if PG15 interrupt is pending
  	if (EXTI->PR & (1<<15)) {
		//Yes, it is, so clear (i.e. reset) it.
		//If you don't do this then the interrupt
		//will keep occuring again and again as the
		//system thinks it is still pending!
    	EXTI->PR |= (1<<15);
		
		//Joystick UP
		//joystick_int = JOYSTICK_UP;
			Vectored_Interrupt(JOYSTICK_UP);
   	}

	//Check to see if PG14 interrupt is pending
  	if (EXTI->PR & (1<<14)) {
		//Yes, it is, so clear (i.e. reset) it.
		//If you don't do this then the interrupt
		//will keep occuring again and again as the
		//system thinks it is still pending!
    	EXTI->PR |= (1<<14);

		//Joystick LEFT
		//joystick_int = JOYSTICK_LEFT;
			Vectored_Interrupt(JOYSTICK_LEFT);
   	}

	//Check to see if PG13 interrupt is pending
  	if (EXTI->PR & (1<<13)) {
		//Yes, it is, so clear (i.e. reset) it.
		//If you don't do this then the interrupt
		//will keep occuring again and again as the
		//system thinks it is still pending!
    	EXTI->PR |= (1<<13);

		//Joystick RIGHT
	  //joystick_int = JOYSTICK_RIGHT;
			Vectored_Interrupt(JOYSTICK_RIGHT);
   	}
}

