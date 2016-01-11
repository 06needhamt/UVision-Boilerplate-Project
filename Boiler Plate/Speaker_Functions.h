#include "stm32f10x.h"

extern void delay(int secs);

void pin_PA4_For_Speaker(void);

void generate_Tone_On_Speaker(int f, int d);

void generate_FSK_Tone(unsigned char *s);
