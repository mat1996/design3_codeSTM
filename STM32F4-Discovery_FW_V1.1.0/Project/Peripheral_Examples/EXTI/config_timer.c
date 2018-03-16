//fichier pour configurer le timer

#include "config_timer.h"
#include "stm32f4_discovery.h"

void initTimerInterruptMS(void)
{
  //initialiser timer 2
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
 
  TIM_TimeBaseInitTypeDef timerInitStructure; 
  timerInitStructure.TIM_Prescaler = 1000; //déterminer pour 1 ms
  timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  timerInitStructure.TIM_Period = 84; //déterminer pour 1ms
  timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  timerInitStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM2, &timerInitStructure);
  
  //initialiser interrupt du timer 2
  NVIC_InitTypeDef nvicStructure;
  nvicStructure.NVIC_IRQChannel = TIM2_IRQn;
  nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
  nvicStructure.NVIC_IRQChannelSubPriority = 1;
  nvicStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvicStructure);

  //démarrer le timer 2
  TIM_Cmd(TIM2, ENABLE);
  
  //activer interruptions du timer 2
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}