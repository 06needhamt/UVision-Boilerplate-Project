#include "stm32f10x.h"
#include "Speaker_Functions.h"

extern void delay10th(unsigned long tenthMsecs);

//Initialises the speaker output pin PA4
void pin_PA4_For_Speaker(void) {
	RCC->APB2ENR |= (1UL << 2);         /* Enable GPIOA clock                 	*/

	GPIOA->ODR   &= ~0x00000100;        //Switch off PA4
	GPIOA->CRL   &= ~0xFFFFFFFF;        
  	GPIOA->CRL   |=  0x00030000;	   	//Configure pins PA4 to general purpose output mode
}

//Produces a square wave of frequency f Hz and duration of d msecs
void generate_Tone_On_Speaker(int f, int d) {
	int n;
	int t;

	t = (10000 / f);					//10thMsecs
	for (n = ((d * 10) / t); n; n--) {
		GPIOA->BSRR |= (1UL << 4);   	//Pin PA4 on
		
		//@TODO make clock tick every 10ms
		delay10th(t >> 1);
		//delay(1);
		GPIOA->BRR |= (1UL << 4);    	//Pin PA4 off
	
		//@TODO make clock tick every 10ms
		//delay(1);
		delay10th(t >> 1);
	}
}

//Frequency Shift Key modulation example - takes a string and produces FSK tones!
void generate_FSK_Tone(unsigned char *s) {
	int i = 0;
	int n;
	int v;
	unsigned char c;
	unsigned char m[2] = {' ','\0'};

	//Get each character of the string
	
	//GLCD_DisplayString(7, i, __FI, "                   ");
	
	//GLCD_SetBackColor(Blue);
	//GLCD_SetTextColor(White);

	while (s[i]) {
		c = s[i];
		m[0] = c;
		//GLCD_DisplayString(7, i + 1, __FI, (unsigned char *)m);
		//Check each bit of the character
		for (n = 0; n < 8; n++) {
			//Determine if a 1 or a 0
			v = ((c >> n) & 1);
			if (v) {
				//If a 1 then generate 600 Hz tone for 20 msecs
				generate_Tone_On_Speaker(600, 20);
			}
			else {
				//If a 0 then generate 1200 Hz tone for 20 msecs
				generate_Tone_On_Speaker(1200, 20);
			}
		}
		i++;
	}
}
