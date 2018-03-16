//header file

#ifndef USART_CONFIG
#define USART_CONFIG

#include "stm32f4_discovery.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"

#define baudrate        2400;
#define baudratePC      9600;

#define GPIO_USART              GPIOA
#define TX_PIN                  GPIO_Pin_2
#define RX_PIN                  GPIO_Pin_3

#define GPIO_USART_PC           GPIOD
#define TX_PIN_PC               GPIO_Pin_8
#define RX_PIN_PC               GPIO_Pin_9

#define PIN_2                   GPIO_PinSource2
#define PIN_3                   GPIO_PinSource3
#define PIN_8                   GPIO_PinSource8
#define PIN_9                   GPIO_PinSource9


void InitUSART(void);
void InitGPIO_USART(void);

#endif