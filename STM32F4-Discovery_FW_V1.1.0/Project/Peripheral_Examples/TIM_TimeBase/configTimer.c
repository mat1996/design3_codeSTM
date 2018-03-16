#include "configTimer.h"

void initTimerCLKExterne()
{
  //ativer les lignes d'horloge des timer
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  
  //configurer les timer
  TIM_TimeBaseInitTypeDef timerInitStructure;
  timerInitStructure.TIM_Prescaler = 0;
  timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  timerInitStructure.TIM_Period = 50000;
  timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  timerInitStructure.TIM_RepetitionCounter = 0;
  
  TIM_TimeBaseInit(TIM1, &timerInitStructure);
  TIM_TimeBaseInit(TIM2, &timerInitStructure);
  TIM_TimeBaseInit(TIM3, &timerInitStructure);
  TIM_TimeBaseInit(TIM4, &timerInitStructure);
  
  TIM_ETRClockMode2Config(TIM1, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0x00);
  TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0x00);
  TIM_ETRClockMode2Config(TIM3, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0x00);
  TIM_ETRClockMode2Config(TIM4, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0x00);
  
  //initialiser les GPIO
  initGPIOTimerCLKExterne();
}

void initGPIOTimerCLKExterne()
{
  //enable les pin de PD
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOD, ENABLE);
  
  GPIO_InitTypeDef  GPIO_external_CLK;
  
  //initialiser GPIO timer 1 et 4
  GPIO_external_CLK.GPIO_Pin = TIM1_CLK_EXTERNE | TIM4_CLK_EXTERNE;
  GPIO_external_CLK.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(GPIO_TIM1_TIM4, &GPIO_external_CLK);
  
  GPIO_PinAFConfig(GPIO_TIM1_TIM4, TIM1_CLK_EXTERNE_SOURCE, GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIO_TIM1_TIM4, TIM4_CLK_EXTERNE_SOURCE, GPIO_AF_TIM4);
  
  //initialiser GPIO timer 2
  GPIO_external_CLK.GPIO_Pin = TIM2_CLK_EXTERNE;
  GPIO_external_CLK.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(GPIO_TIM2, &GPIO_external_CLK);
  
  GPIO_PinAFConfig(GPIO_TIM2, TIM2_CLK_EXTERNE_SOURCE, GPIO_AF_TIM2);
  
  //initialiser GPIO timer 3
  GPIO_external_CLK.GPIO_Pin = TIM3_CLK_EXTERNE;
  GPIO_external_CLK.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(GPIO_TIM3, &GPIO_external_CLK);

  GPIO_PinAFConfig(GPIO_TIM3  , TIM3_CLK_EXTERNE_SOURCE, GPIO_AF_TIM3);
  
}

void initTimerInterruptXms(uint16_t prescaler, uint16_t period)
{
  //activer la ligne d'horloge
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
   
  //configurer ler timer 2
  TIM_TimeBaseInitTypeDef timerInitStructure;
  timerInitStructure.TIM_Prescaler = prescaler;
  timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  timerInitStructure.TIM_Period = period;
  timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  timerInitStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM5, &timerInitStructure);
  
  //configurer les interrupt
  NVIC_InitTypeDef nvicStructureTIM;
  nvicStructureTIM.NVIC_IRQChannel = TIM5_IRQn;
  nvicStructureTIM.NVIC_IRQChannelPreemptionPriority = 0;
  nvicStructureTIM.NVIC_IRQChannelSubPriority = 1;
  nvicStructureTIM.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvicStructureTIM);
  
  //activer les interrupt et timer
  TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);

  
}