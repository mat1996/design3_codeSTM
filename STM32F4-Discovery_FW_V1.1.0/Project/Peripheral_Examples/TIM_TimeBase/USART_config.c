#include "USART_config.h"


USART_InitTypeDef USART2_init, USART3_init;
NVIC_InitTypeDef NVIC_usart2, NVIC_usart3;
GPIO_InitTypeDef GPIO_init;

void InitUSART(void)
{
  //initialiser GPIO pour pin RX et TX
  InitGPIO_USART();
  
  //initialiser USART 2
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  
  USART2_init.USART_BaudRate = baudrate;
  USART2_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART2_init.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  USART2_init.USART_Parity = USART_Parity_No;
  USART2_init.USART_StopBits = USART_StopBits_1;
  USART2_init.USART_WordLength = USART_WordLength_8b;
  USART_Init(USART2, &USART2_init);
  
  //initialiser USART 3
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  
  USART3_init.USART_BaudRate = baudratePC;
  USART3_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART3_init.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  USART3_init.USART_Parity = USART_Parity_No;
  USART3_init.USART_StopBits = USART_StopBits_1;
  USART3_init.USART_WordLength = USART_WordLength_8b;
  USART_Init(USART3, &USART3_init);  
  
  
  //activer les interrupts
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);  
  
  //initialiser les interrupts
  NVIC_usart2.NVIC_IRQChannel = USART2_IRQn;
  NVIC_usart2.NVIC_IRQChannelCmd = ENABLE;
  NVIC_usart2.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_usart2.NVIC_IRQChannelSubPriority = 0;
  NVIC_Init(&NVIC_usart2);
  
  NVIC_usart3.NVIC_IRQChannel = USART3_IRQn;
  NVIC_usart3.NVIC_IRQChannelCmd = ENABLE;
  NVIC_usart3.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_usart3.NVIC_IRQChannelSubPriority = 0;
  NVIC_Init(&NVIC_usart3);  
  
  //activer périphérique
  USART_Cmd(USART2, ENABLE);
  USART_Cmd(USART3, ENABLE);
}

void InitGPIO_USART(void)
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
  GPIO_PinAFConfig(GPIO_USART, PIN_2, GPIO_AF_USART2);
  GPIO_PinAFConfig(GPIO_USART, PIN_3, GPIO_AF_USART2);
  GPIO_PinAFConfig(GPIO_USART_PC, PIN_8, GPIO_AF_USART3);
  GPIO_PinAFConfig(GPIO_USART_PC, PIN_9, GPIO_AF_USART3);
  
  //configurer les GPIO USART2
  GPIO_init.GPIO_Pin = TX_PIN | RX_PIN;
  GPIO_init.GPIO_Mode = GPIO_Mode_AF;
  GPIO_init.GPIO_OType = GPIO_OType_PP;
  GPIO_init.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_init.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIO_USART, &GPIO_init);
  
  //configurer le GPIO USART3
  GPIO_init.GPIO_Pin = TX_PIN_PC | RX_PIN_PC;
  GPIO_init.GPIO_Mode = GPIO_Mode_AF;
  GPIO_init.GPIO_OType = GPIO_OType_PP;
  GPIO_init.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_init.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIO_USART_PC, &GPIO_init);
  
  
}

