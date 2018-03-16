#include "USART_config.h"
#include "stm32f4_discovery.h"
#include "stm32f4xx_usart.h"

USART_InitTypeDef USART2_init;
NVIC_InitTypeDef NVIC_usart2;
GPIO_InitTypeDef GPIO_init;

void InitUSART(void)
{
  //initialiser GPIO pour pin RX et TX
  InitGPIO();
  
  //initialiser USART 2
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  
  USART2_init.USART_BaudRate = baudrate;
  USART2_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART2_init.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  USART2_init.USART_Parity = USART_Parity_No;
  USART2_init.USART_StopBits = USART_StopBits_1;
  USART2_init.USART_WordLength = USART_WordLength_8b;
  USART_Init(USART2, &USART2_init);
  
  //activer les interrupts
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
  
  //initialiser les interrupts
  NVIC_usart2.NVIC_IRQChannel = USART2_IRQn;
  NVIC_usart2.NVIC_IRQChannelCmd = ENABLE;
  NVIC_usart2.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_usart2.NVIC_IRQChannelSubPriority = 0;
  NVIC_Init(&NVIC_usart2);
  
  //activer périphérique
  USART_Cmd(USART2, ENABLE);
}

void InitGPIO(void)
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
  
  //configurer les GPIO
  GPIO_init.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_init.GPIO_Mode = GPIO_Mode_AF;
  GPIO_init.GPIO_OType = GPIO_OType_PP;
  GPIO_init.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_init.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOA, &GPIO_init);
  
  //initialiser les LED
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED5);
  STM_EVAL_LEDInit(LED6); 
  
}
