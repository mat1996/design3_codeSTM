#ifndef DRIVEMOTEUR
#define DRIVEMOTEUR

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"

//DEFINE

#define TIMER                   TIM8
#define GPIO_TIM_FCT            GPIO_AF_TIM8            
#define GPIO_CLK_ENABLE         RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE
#define TIM_CLK_ENABLE          RCC_APB2Periph_TIM8

#define GPIO_PWM                GPIOC 
#define GPIO_IO_CTRL            GPIOE

#define moteur1PWM              GPIO_Pin_6
#define moteur1PWM_pinSource    GPIO_PinSource6
#define moteur1CTRL1            GPIO_Pin_8
#define moteur1CTRL2            GPIO_Pin_9

#define moteur2PWM              GPIO_Pin_7
#define moteur2PWM_pinSource    GPIO_PinSource7
#define moteur2CTRL1            GPIO_Pin_10
#define moteur2CTRL2            GPIO_Pin_11

#define moteur3PWM              GPIO_Pin_8
#define moteur3PWM_pinSource    GPIO_PinSource8
#define moteur3CTRL1            GPIO_Pin_12
#define moteur3CTRL2            GPIO_Pin_13

#define moteur4PWM              GPIO_Pin_9
#define moteur4PWM_pinSource    GPIO_PinSource9
#define moteur4CTRL1            GPIO_Pin_14
#define moteur4CTRL2            GPIO_Pin_15



typedef struct{
  double dutyCycleMoteur;
  uint16_t direction;
  //ajouter au besoin
} structMoteur;

void initPWM(void);
void initIOMoteur(void);

void arreterMoteur(uint16_t numeroMoteur);
void avancerMoteur(double dutyCycle, uint16_t numeroMoteur, uint16_t choixSensMoteur);
void setVitesseMoteur(double dutyCycle, uint16_t numeroMoteur);
uint16_t calculerValeurDutyCycle(double dutyCycle);
double getDutyCycleMoteur(uint16_t numeroMoteur);
uint16_t getDirectionMoteur(uint16_t numeroMoteur);

//méthode de test
void setCommencerTest(uint16_t valeur);
uint16_t getCommencerTest(void);
#endif