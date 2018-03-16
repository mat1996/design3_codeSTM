#ifndef CONFIGTIMER
#define CONFIGTIMER

#include "stm32f4xx_tim.h"
#include "stm32f4xx_gpio.h"

#define GPIO_TIM1_TIM4                  GPIOE
#define TIM1_CLK_EXTERNE                GPIO_Pin_7
#define TIM4_CLK_EXTERNE                GPIO_Pin_0
#define TIM1_CLK_EXTERNE_SOURCE         GPIO_PinSource7
#define TIM4_CLK_EXTERNE_SOURCE         GPIO_PinSource0

#define GPIO_TIM2                       GPIOA
#define TIM2_CLK_EXTERNE                GPIO_Pin_15
#define TIM2_CLK_EXTERNE_SOURCE         GPIO_PinSource15

#define GPIO_TIM3                       GPIOD
#define TIM3_CLK_EXTERNE                GPIO_Pin_2
#define TIM3_CLK_EXTERNE_SOURCE         GPIO_PinSource2

void initTimerCLKExterne(void);
void initTimerInterruptXms(uint16_t prescaler, uint16_t period);
void initGPIOTimerCLKExterne(void);

#endif