#include "stm32f10x.h"

#define __AFIO_EXTICR1_JS_PD3     0x00003000  //EXTI3 interrupt
#define __EXTI_IMR_JS_PD3         0x00000008  //Interrupt Mask Register - Unmask intr from src EXTI3
#define __EXTI_EMR_JS_PD3         0x00000000  //Event Mask Register - Mask events from all EXTI
#define __EXTI_RTSR_JS_PD3        0x00000000  //Rising Trigger Selection Register - Rising trigger disabled for all EXTI
#define __EXTI_FTSR_JS_PD3        0x00000008  //Falling Trigger Selection register - Falling trigger enabled for EXTI3
#define EXTI3_IRQChannel           ((u8)0x09)  // EXTI Line3 Interrupt (position 9  in interrupt table)

/* Definitions for PG7 (Joystick ) interrupt handling setup */
#define __AFIO_EXTICR2_JS_PG7     0x00006000  //EXTI7 interrupt
#define __EXTI_IMR_JS_PG7         0x00000080  //Interrupt Mask Register - Unmask intr from src EXTI7
#define __EXTI_EMR_JS_PG7         0x00000000  //Event Mask Register - Mask events from all EXTI
#define __EXTI_RTSR_JS_PG7        0x00000000  //Rising Trigger Selection Register - Rising trigger disabled for all EXTI
#define __EXTI_FTSR_JS_PG7        0x00000080  //Falling Trigger Selection register - Falling trigger enabled for EXTI7
#define EXTI9_5_IRQChannel        ((u8)0x17)  //External Line[9:5] Interrupts (position 23  in interrupt table)

/* Definitions for PG8 (User button) interrupt handling setup */
#define __AFIO_EXTICR3_UB_PG8     0x00000006  //EXTI8 interrupt
#define __EXTI_IMR_UB_PG8         0x00000100  //Interrupt Mask Register - Unmask intr from src EXTI8
#define __EXTI_EMR_UB_PG8         0x00000000  //Event Mask Register - Mask events from all EXTI
#define __EXTI_RTSR_UB_PG8        0x00000000  //Rising Trigger Selection Register - Rising trigger disabled for all EXTI
#define __EXTI_FTSR_UB_PG8        0x00000100  //Falling Trigger Selection register - Falling trigger enabled for EXTI8
//#define EXTI9_5_IRQChannel        ((u8)0x17)  //External Line[9:5] Interrupts

#define __AFIO_EXTICR4_PG15_13	  0x00006660  //EXTI15,14,14 interrupts
#define __EXTI_IMR_JS_PG15_13     0x0000E000  //Interrupt Mask Register - Unmask intr from src EXTI15, EXTI14, EXTI13
#define __EXTI_EMR_JS_PG15_13     0x00000000  //Event Mask Register - Mask events from all EXTI
#define __EXTI_RTSR_JS_PG15_13    0x00000000  //Rising Trigger Selection Register - Rising trigger disabled for all EXTI
#define __EXTI_FTSR_JS_PG15_13    0x0000E000  //Falling Trigger Selection register - Falling trigger enabled for EXTI15, EXTI14, EXTI13
#define EXTI15_10_IRQChannel        ((u8)0x28)  //External Line[15:10] Interrupts (position 40 in interrupt table)

/*----------------------------------------------------------------------------
  Function that initializes the User button	as an input
 *----------------------------------------------------------------------------*/
void userButton_Init(void) {
	RCC->APB2ENR |= (1UL << 8);         /* Enable GPIOG clock                 	*/

  	GPIOG->CRH   &= ~0x0000000F;        /* Configure the GPIO for the User button 	*/
  	GPIOG->CRH   |=  0x00000004;
}

/*----------------------------------------------------------------------------
  Function that initializes the Joystick switches as inputs
 *----------------------------------------------------------------------------*/
void joyStick_Init(void) {
	RCC->APB2ENR |= (1UL << 8);         /* Enable GPIOG clock                 	        */

   	GPIOG->CRL   &= ~0xF000F000;		/* Configure PG7 and PD3 as joystick inputs	    */
	GPIOG->CRL   |=  0x40004000; 		

  	GPIOG->CRH   &= ~0xFFF00000;        /* Configure the GPIO for the joystick buttons PG15, PG14, PG13	*/
  	GPIOG->CRH   |=  0x44400000;
}

/*----------------------------------------------------------------------------
  Function that enables the User button for interrupt handling
 *----------------------------------------------------------------------------*/
void userButton_IntrEnable(void) {
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;           			// Enable clock for Alternate Function register
    AFIO->EXTICR[2] &= 0xFFF0;                    			// Clear
    AFIO->EXTICR[2] |= (0x000F & __AFIO_EXTICR3_UB_PG8);	// Use PG8 as source of interrupt

    EXTI->IMR       |= ((1 << 8) & __EXTI_IMR_UB_PG8);      // Unmask interrupt	only from source PG8
	//EXTI->IMR     	= 0x00000000;            				// Mask all interrupt sources
    EXTI->EMR       |= ((1 << 8) & __EXTI_EMR_UB_PG8);      // Mask ALL events
    EXTI->RTSR      |= ((1 << 8) & __EXTI_RTSR_UB_PG8);     // Rising edge for PG8 interrupt - not set
    EXTI->FTSR      |= ((1 << 8) & __EXTI_FTSR_UB_PG8);     // Falling edge	for PG8 interrupt - set

	NVIC->ISER[0] |= (1 << (EXTI9_5_IRQChannel & 0x1F));  	// Enable interrupt handler for EXTI 9..5
}

/*----------------------------------------------------------------------------
  Function that enables the Joystick switches for interrupt handling
 *----------------------------------------------------------------------------*/
void joyStick_IntrEnable_PG15_13(void) {
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;           				// Enable clock for Alternate Function register
    AFIO->EXTICR[3] &= 0x000F;                    				// Clear
    AFIO->EXTICR[3] |= (0xFFF0 & __AFIO_EXTICR4_PG15_13);		// Use PG15, PG14, PG13 as interrupt sources

    EXTI->IMR       |= ((1 << 15) & __EXTI_IMR_JS_PG15_13);  	// Unmask interrupt	only from source PG15
    EXTI->IMR       |= ((1 << 14) & __EXTI_IMR_JS_PG15_13);  	// Unmask interrupt	only from source PG14
	EXTI->IMR       |= ((1 << 13) & __EXTI_IMR_JS_PG15_13);  	// Unmask interrupt	only from source PG13

    EXTI->EMR       |= ((1 << 15) & __EXTI_EMR_JS_PG15_13);  	// Mask ALL events
    EXTI->RTSR      |= ((1 << 15) & __EXTI_RTSR_JS_PG15_13); 	// Rising edge for PG15 interrupt - not set
    EXTI->FTSR      |= ((1 << 15) & __EXTI_FTSR_JS_PG15_13); 	// Falling edge for PG15 interrupt - set

    EXTI->RTSR      |= ((1 << 14) & __EXTI_RTSR_JS_PG15_13); 	// Rising edge for PG14 interrupt - not set
    EXTI->FTSR      |= ((1 << 14) & __EXTI_FTSR_JS_PG15_13); 	// Falling edge for PG14 interrupt - set

    EXTI->RTSR      |= ((1 << 13) & __EXTI_RTSR_JS_PG15_13);  	// Rising edge for PG13 interrupt - not set
    EXTI->FTSR      |= ((1 << 13) & __EXTI_FTSR_JS_PG15_13);  	// Falling edge for PG13 interrupt - set

	NVIC->ISER[1] |= (1 << (EXTI15_10_IRQChannel & 0x1F));    	// Enable interrupt handler for EXTI 15..10
}

/*----------------------------------------------------------------------------
  Function that enables the Joystick switch for interrupt handling
 *----------------------------------------------------------------------------*/
void joyStick_IntrEnable_PG7(void) {
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;           			// Enable clock for Alternate Function register
    AFIO->EXTICR[1] &= 0x0FFF;                    			// Clear
    AFIO->EXTICR[1] |= (0xF000 & __AFIO_EXTICR2_JS_PG7);	// Use PG7 as source of interrupt

    EXTI->IMR       |= ((1 << 7) & __EXTI_IMR_JS_PG7);      // Unmask interrupt	only from source PG7
	//EXTI->IMR     	= 0x00000000;            				// Mask all interrupt sources
    EXTI->EMR       |= ((1 << 7) & __EXTI_EMR_JS_PG7);      // Mask ALL events
    EXTI->RTSR      |= ((1 << 7) & __EXTI_RTSR_JS_PG7);     // Rising edge for PG7 interrupt - not set
    EXTI->FTSR      |= ((1 << 7) & __EXTI_FTSR_JS_PG7);     // Falling edge	for PG7 interrupt - set

	NVIC->ISER[0] |= (1 << (EXTI9_5_IRQChannel & 0x1F));  	// Enable interrupt handler for EXTI 9..5
}

/*----------------------------------------------------------------------------
  Function that enables the Joystick switch for interrupt handling
 *----------------------------------------------------------------------------*/
void joyStick_IntrEnable_PD3(void) {
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;                     // Enable clock for Alternate Function
    AFIO->EXTICR[0] &= 0x0FFF;                              // Clear used pin
    AFIO->EXTICR[0] |= (0xF000 & __AFIO_EXTICR1_JS_PD3);    // Use PD3 as source of interrupt

    EXTI->IMR       |= ((1 << 3) & __EXTI_IMR_JS_PD3);      // Unmask interrupt	only from source PD3
    EXTI->EMR       |= ((1 << 3) & __EXTI_EMR_JS_PD3);      // Mask ALL events
    EXTI->RTSR      |= ((1 << 3) & __EXTI_RTSR_JS_PD3);     // Rising edge for PD3 interrupt - not set
    EXTI->FTSR      |= ((1 << 3) & __EXTI_FTSR_JS_PD3);     // Falling edge	for PD3 interrupt - set

    NVIC->ISER[0] |= (1 << (EXTI3_IRQChannel & 0x1F));    	// Enable interrupt EXTI 3
}
