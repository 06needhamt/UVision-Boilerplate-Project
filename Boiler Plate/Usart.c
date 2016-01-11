/*----------------------------------------------------------------------------
 * Name:    Usart.c
 * Purpose: USART usage for STM32
 * Version: V1.00
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * Copyright (c) 2005-2007 Keil Software. All rights reserved.
 *----------------------------------------------------------------------------*/
 

#include "stm32f10x_lib.h"                        // STM32F10x Library Definitions
//#include <stm32f10x.h>
#include "STM32_Reg.h"                            // missing bit definitions
#include <stdio.h>
#include "STM32_Init.h"                           // STM32 Initialization



/*----------------------------------------------------------------------------
  Notes:
  The length of the receive and transmit buffers must be a power of 2.
  Each buffer has a next_in and a next_out index.
  If next_in = next_out, the buffer is empty.
  (next_in - next_out) % buffer_size = the number of characters in the buffer.
 *----------------------------------------------------------------------------*/
#define TBUF_SIZE   256	     /*** Must be a power of 2 (2,4,8,16,32,64,128,256,512,...) ***/
#define RBUF_SIZE   256      /*** Must be a power of 2 (2,4,8,16,32,64,128,256,512,...) ***/

#define __USART2_BAUDRATE         115200
#define __USART2_BRR			  0x138			//See Cortex-M3 spec "Fractional baud rate generator" section
//#define __USART2_BAUDRATE         57600
//#define __USART2_BRR			  0x271			//See Cortex-M3 spec "Fractional baud rate generator" section

#define __USART2_DATABITS         0x00000000
#define __USART2_STOPBITS         0x00000000
#define __USART2_PARITY           0x00000000
#define __USART2_FLOWCTRL         0x00000000
#define __USART2_REMAP            0x00000000
#define __USART2_CR1              0x000000A0
#define __USART2_CR2              0x00000000
#define __USART2_CR3              0x00000000

/*----------------------------------------------------------------------------
 *----------------------------------------------------------------------------*/
#if TBUF_SIZE < 2
#error TBUF_SIZE is too small.  It must be larger than 1.
#elif ((TBUF_SIZE & (TBUF_SIZE-1)) != 0)
#error TBUF_SIZE must be a power of 2.
#endif

#if RBUF_SIZE < 2
#error RBUF_SIZE is too small.  It must be larger than 1.
#elif ((RBUF_SIZE & (RBUF_SIZE-1)) != 0)
#error RBUF_SIZE must be a power of 2.
#endif

/*----------------------------------------------------------------------------
 *----------------------------------------------------------------------------*/
struct buf_st {
  	unsigned int in;                                // Next In Index
  	unsigned int out;                               // Next Out Index
  	char buf [RBUF_SIZE];                           // Buffer
};

static struct buf_st rbuf = { 0, 0, };
#define SIO_RBUFLEN ((unsigned short)(rbuf.in - rbuf.out))

static struct buf_st tbuf = { 0, 0, };
#define SIO_TBUFLEN ((unsigned short)(tbuf.in - tbuf.out))

static unsigned int tx_restart = 1;               // NZ if TX restart is required

char strBuffer[RBUF_SIZE];

/*----------------------------------------------------------------------------
  USART2_IRQHandler
  Handles USART2 global interrupt request.
 *----------------------------------------------------------------------------*/
void USART2_IRQHandler (void) {
	volatile unsigned int IIR;
	struct buf_st *p;

    IIR = USART2->SR;
    if (IIR & USART_FLAG_RXNE) {                  // read interrupt
      	USART2->SR &= ~USART_FLAG_RXNE;	          // clear interrupt

      	p = &rbuf;

      	if (((p->in - p->out) & ~(RBUF_SIZE-1)) == 0) {
        	p->buf [p->in & (RBUF_SIZE-1)] = (USART2->DR & 0x1FF);
        	p->in++;
      	}
    }

    if (IIR & USART_FLAG_TXE) {
      	USART2->SR &= ~USART_FLAG_TXE;	          // clear interrupt

      	p = &tbuf;

      	if (p->in != p->out) {
        	USART2->DR = (p->buf [p->out & (TBUF_SIZE-1)] & 0x1FF);
        	p->out++;
        	tx_restart = 0;
      	}
      	else {
        	tx_restart = 1;
			USART2->CR1 &= ~USART_FLAG_TXE;		      // disable TX interrupt if nothing to send
      	}
    }
}

void USART2_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;                     // enable clock for Alternate Function
    AFIO->MAPR   &= ~(1 << 3);                              // clear USART2 remap

    RCC->APB2ENR |=  RCC_APB2ENR_IOPAEN;                  	// enable clock for GPIOA
    GPIOA->CRL   &= ~(0xFFUL  << 8);                      	// Clear PA2, PA3
    GPIOA->CRL   |=  (0x0BUL  << 8);                      	// USART2 Tx (PA2)  alternate output push-pull
    GPIOA->CRL   |=  (0x04UL  << 12);                     	// USART2 Rx (PA3)  input floating

    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;                   // enable clock for USART2

    USART2->BRR  = __USART2_BRR; 							// set baudrate
    USART2->CR1  = __USART2_DATABITS;                       // set Data bits
    USART2->CR2  = __USART2_STOPBITS;                       // set Stop bits
    USART2->CR1 |= __USART2_PARITY;                         // set Parity
    USART2->CR3  = __USART2_FLOWCTRL;                       // Set Flow Control

    USART2->CR1 |= (USART_CR1_RE | USART_CR1_TE);           // RX, TX enable

    USART2->CR1 |= __USART2_CR1;
    USART2->CR2 |= __USART2_CR2;
    USART2->CR3 |= __USART2_CR3;
	NVIC->ISER[1] |= (1 << (USART2_IRQChannel & 0x1F));   	// enable interrupt

    USART2->CR1 |= USART_CR1_UE;                            // USART enable
}
/*------------------------------------------------------------------------------
  buffer_Init
  initialize the buffers
 *------------------------------------------------------------------------------*/
void buffer_Init (void) {
  	tbuf.in = 0;                                    // Clear com buffer indexes
  	tbuf.out = 0;
  	tx_restart = 1;

  	rbuf.in = 0;
  	rbuf.out = 0;
}

/*------------------------------------------------------------------------------
  SenChar
  transmit a character
 *------------------------------------------------------------------------------*/
int SendChar (int c) {
  	struct buf_st *p = &tbuf;

                                                  	// If the buffer is full, return an error value
  	if (SIO_TBUFLEN >= TBUF_SIZE)
    	return (-1);
                                                  
  	p->buf [p->in & (TBUF_SIZE - 1)] = c;           // Add data to the transmit buffer.
  	p->in++;

  	if (tx_restart) {                               // If transmit interrupt is disabled, enable it
    	tx_restart = 0;
		USART2->CR1 |= USART_FLAG_TXE;		     	// enable TX interrupt
  	}

  	return (0);
}

/*------------------------------------------------------------------------------
  GetKey
  receive a character
 *------------------------------------------------------------------------------*/
int GetKey (void) {
  	struct buf_st *p = &rbuf;
												// If the buffer is empty, return an error value
  	if (SIO_RBUFLEN == 0)
    	return (-1);

  	return (p->buf [(p->out++) & (RBUF_SIZE - 1)]);
}

/*------------------------------------------------------------------------------
  writeStr
  sends a string of characters
 *------------------------------------------------------------------------------*/
int writeStr(char *str) {
	int n = 0;
	int i = 0;
	int control = 0;

	for (n = 0; str[n]; n++) {
		switch (str[n]) {
			case '\\':
				control = 1;
				break;
			case 'n':
				if (control) {
					i = putchar(0x0d);
					i = putchar(0x0a);
					control = 0;
				}
				break;
			default:
				if (control) {
					i = putchar('\\');
					control = 0;
				}							
				i = putchar(str[n]);							
				break;
		}
		if (i == -1) {
			break;
		}
	}
	
	if (control) {
		putchar('\\');
	}
	
	return n;
}
/*------------------------------------------------------------------------------
  writeStrln
  same as writeStr but sends a carriage return and line advance at the end
 *------------------------------------------------------------------------------*/
int writeStrln(char *str) {
	int n = 0;
	
	n = writeStr(str);
	putchar('\n');
	
	return n;
}

/*------------------------------------------------------------------------------
  getStr
  gets a string of characters; blocks until end of line is received
 *------------------------------------------------------------------------------*/
int getStr(void){
	char c = '\0';
	int n = 0;
	char *p = &strBuffer[0];

	while (c != 0x0d ) {
		c = getchar();
		*(p++) = c;
		n += 1;
	}
	
	return n;
}

void SUART_Init(void) {
  	buffer_Init();                                  // init RX / TX buffers
  	USART2_Init();								  	// USART2 is initialised in here
}

