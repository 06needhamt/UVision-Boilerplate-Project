 /*(C) COPYRIGHT 2007 STMicroelectronics 
 File Name           stm32f10x_lib.h
 Author              MCD Application Team
 Version             V1.0
 Date                10082007
 Description         This file includes the peripherals header files in the
                      user application.

 THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
 INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
 CONTENT OF SUCH SOFTWARE ANDOR THE USE MADE BY CUSTOMERS OF THE CODING
 INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.


 Define to prevent recursive inclusion ------------------------------------- */
 
#ifndef __STM32F10x_LIB_H
#define __STM32F10x_LIB_H

#ifdef __cplusplus
  #define   __I     volatile             //! defines 'read only' permissions                 
#else
  #define   __I     volatile const       //! defines 'read only' permissions   
#endif	

#define     __O     volatile             //! defines 'write only' permissions                
#define     __IO    volatile             //! defines 'read  write' permissions              

#include <stdint.h>

 /* Includes ------------------------------------------------------------------ */

#define _ADC
  #include "stm32f10x_adc.h"
  

#define _BKP
  #include "stm32f10x_bkp.h"
  

#define _CAN
  #include "stm32f10x_can.h"
  

#define _DMA
  #include "stm32f10x_dma.h"
  

#define _EXTI
  #include "stm32f10x_exti.h"
  

#define _FLASH
  #include "stm32f10x_flash.h"
  

#define _GPIO
  #include "stm32f10x_gpio.h"
  

#define _I2C
  #include "stm32f10x_i2c.h"
  

#define _IWDG
  #include "stm32f10x_iwdg.h"
  

#define _NVIC
  #include "stm32f10x_nvic.h"
  

#define _PWR
  #include "stm32f10x_pwr.h"
  

#define _RCC
  #include "stm32f10x_rcc.h"
  

#define _RTC
  #include "stm32f10x_rtc.h"
  

#define _SPI
  #include "stm32f10x_spi.h"
  

#define _SysTick
  #include "stm32f10x_systick.h"
  

#define _TIM
  #include "stm32f10x_tim.h"
  

#define _USART
  #include "stm32f10x_usart.h"
  

#define _WWDG
  #include "stm32f10x_wwdg.h"
  

/* Exported types ------------------------------------------------------------
 Exported constants --------------------------------------------------------
 Exported macro ------------------------------------------------------------
 Exported functions ------------------------------------------------------- */
  #endif
void debug(void);

  

/* (C) COPYRIGHT 2007 STMicroelectronics END OF FILE */