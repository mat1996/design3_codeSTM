

#include "controleMoteur.h"

TIM_TimeBaseInitTypeDef structTimer1;
TIM_OCInitTypeDef structPWMtim;
GPIO_InitTypeDef structGPIO;

static structMoteur moteur1,moteur2,moteur3,moteur4;
static uint16_t commencerTest = 0;

uint16_t prescaler = 0;
uint16_t period = 60000;
uint16_t dutyCycleChannel1 = 0;
uint16_t dutyCycleChannel2 = 0;
uint16_t dutyCycleChannel3 = 0;
uint16_t dutyCycleChannel4 = 0;

//utilise le timer 4
void initPWM()
{
  //activer la ligne d'horloge
  RCC_APB2PeriphClockCmd(TIM_CLK_ENABLE, ENABLE);

  //7000000 => fréquence de 116.6 Hz
  prescaler = (uint16_t) ((SystemCoreClock ) / 7000000) - 1;
  
  //configuration base timer 1
  structTimer1.TIM_Period = period;
  structTimer1.TIM_Prescaler = prescaler;
  structTimer1.TIM_ClockDivision = 0;
  structTimer1.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIMER, &structTimer1);
  
  //configuration channel PWM
  //channel1
  structPWMtim.TIM_OCMode = TIM_OCMode_PWM1;
  structPWMtim.TIM_OutputState = TIM_OutputState_Enable;
  structPWMtim.TIM_Pulse = dutyCycleChannel1;
  structPWMtim.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIMER, &structPWMtim);

  TIM_OC1PreloadConfig(TIMER, TIM_OCPreload_Enable);
  
  //channel2
  structPWMtim.TIM_OutputState = TIM_OutputState_Enable;
  structPWMtim.TIM_Pulse = dutyCycleChannel2;

  TIM_OC2Init(TIMER, &structPWMtim);

  TIM_OC2PreloadConfig(TIMER, TIM_OCPreload_Enable);
  
  //channel3
  structPWMtim.TIM_OutputState = TIM_OutputState_Enable;
  structPWMtim.TIM_Pulse = dutyCycleChannel3;

  TIM_OC3Init(TIMER, &structPWMtim);

  TIM_OC3PreloadConfig(TIMER, TIM_OCPreload_Enable); 
  
  //channel4
  structPWMtim.TIM_OutputState = TIM_OutputState_Enable;
  structPWMtim.TIM_Pulse = dutyCycleChannel4;

  TIM_OC4Init(TIMER, &structPWMtim);

  TIM_OC4PreloadConfig(TIMER, TIM_OCPreload_Enable);   
  
  //activer le timer1
  TIM_ARRPreloadConfig(TIMER, ENABLE);
  TIM_CtrlPWMOutputs(TIMER, ENABLE); 
  TIM_Cmd(TIMER, ENABLE);
  
}

void initIOMoteur()
{
  //initialiser les IO du PWM pour timer1
  RCC_AHB1PeriphClockCmd(GPIO_CLK_ENABLE, ENABLE);
  
  
  //initialiser le GPIO pour le PWM
  structGPIO.GPIO_Pin = moteur1PWM | moteur2PWM |moteur3PWM | moteur4PWM;
  structGPIO.GPIO_Mode = GPIO_Mode_AF;
  structGPIO.GPIO_Speed = GPIO_Speed_100MHz;
  structGPIO.GPIO_OType = GPIO_OType_PP;
  structGPIO.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIO_PWM, &structGPIO); 
 
//  structGPIO.GPIO_Pin = moteur3PWM | moteur4PWM;
//  structGPIO.GPIO_Mode = GPIO_Mode_AF;
//  structGPIO.GPIO_Speed = GPIO_Speed_100MHz;
//  structGPIO.GPIO_OType = GPIO_OType_PP;
//  structGPIO.GPIO_PuPd = GPIO_PuPd_UP ;
//  GPIO_Init(GPIO_PWM2, &structGPIO); 
 
  GPIO_PinAFConfig(GPIO_PWM, moteur1PWM_pinSource, GPIO_TIM_FCT);
  GPIO_PinAFConfig(GPIO_PWM, moteur2PWM_pinSource, GPIO_TIM_FCT); 
  GPIO_PinAFConfig(GPIO_PWM, moteur3PWM_pinSource, GPIO_TIM_FCT);
  GPIO_PinAFConfig(GPIO_PWM, moteur4PWM_pinSource, GPIO_TIM_FCT); 
  
  //initialiser le GPIO pour le contrôle des moteurs
  structGPIO.GPIO_Pin = moteur1CTRL1 | moteur1CTRL2 | moteur2CTRL1 | moteur2CTRL2 | moteur3CTRL1 | moteur3CTRL2 | moteur4CTRL1 | moteur4CTRL2;
  structGPIO.GPIO_Mode = GPIO_Mode_OUT;
  structGPIO.GPIO_Speed = GPIO_Speed_100MHz;
  structGPIO.GPIO_OType = GPIO_OType_PP;
  structGPIO.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIO_IO_CTRL, &structGPIO);   
  
}

void arreterMoteur(uint16_t numeroMoteur)
{
  uint16_t moteurCTRL1, moteurCTRL2;
  
  switch(numeroMoteur)
  {
  case 1 :
    {
      moteurCTRL1 = moteur1CTRL1;
      moteurCTRL2 = moteur1CTRL2;
      dutyCycleChannel1 = 0;
      moteur1.dutyCycleMoteur = 0;
      break;
    }
  case 2 :
    {
      //setter moteur 2
      moteurCTRL1 = moteur2CTRL1;
      moteurCTRL2 = moteur2CTRL2;
      dutyCycleChannel2 = 0;
      moteur2.dutyCycleMoteur = 0;
      break;
    }
  case 3 :
    {
      //setter moteur 3
      moteurCTRL1 = moteur3CTRL1;
      moteurCTRL2 = moteur3CTRL2;
      dutyCycleChannel3 = 0;
      moteur3.dutyCycleMoteur = 0;
      break;
    }
  case 4 :
    {
      //setter moteur 4
      moteurCTRL1 = moteur4CTRL1;
      moteurCTRL2 = moteur4CTRL2;
      dutyCycleChannel4 = 0;
      moteur4.dutyCycleMoteur = 0;
      break;
    }
  default :
    {
      //ne rien faire
      break;
    }
  }
  
  //setter les pin de controle à 0
  GPIO_IO_CTRL->BSRRH = moteurCTRL1;
  GPIO_IO_CTRL->BSRRH = moteurCTRL2;
  
  //setter le duty cycle du channel concerné à 0 ça fontionne?
  TIMER->CCR1 = dutyCycleChannel1;
  TIMER->CCR2 = dutyCycleChannel2;
  TIMER->CCR3 = dutyCycleChannel3;
  TIMER->CCR4 = dutyCycleChannel4;
}

void avancerMoteur(double dutyCycle, uint16_t numeroMoteur, uint16_t choixSensMoteur)
{
  uint16_t valeurDutyCycle = calculerValeurDutyCycle(dutyCycle);
  uint16_t moteurCTRL1, moteurCTRL2;
  
  switch(numeroMoteur)
  {
  case 1 :
    {
      moteurCTRL1 = moteur1CTRL1;
      moteurCTRL2 = moteur1CTRL2;
      dutyCycleChannel1 = valeurDutyCycle;
      moteur1.dutyCycleMoteur = dutyCycle;
      moteur1.direction = choixSensMoteur;
      break;
    }
  case 2 :
    {
      //setter moteur 2
      moteurCTRL1 = moteur2CTRL1;
      moteurCTRL2 = moteur2CTRL2;
      dutyCycleChannel2 = valeurDutyCycle;
      moteur2.dutyCycleMoteur = dutyCycle;
      moteur2.direction = choixSensMoteur;
      break;
    }
  case 3 :
    {
      //setter moteur 3
      moteurCTRL1 = moteur3CTRL1;
      moteurCTRL2 = moteur3CTRL2;
      dutyCycleChannel3 = valeurDutyCycle;
      moteur3.dutyCycleMoteur = dutyCycle;
      moteur3.direction = choixSensMoteur;
      break;
    }
  case 4 :
    {
      //setter moteur 4
      moteurCTRL1 = moteur4CTRL1;
      moteurCTRL2 = moteur4CTRL2;
      dutyCycleChannel4 = valeurDutyCycle;
      moteur4.dutyCycleMoteur = dutyCycle;
      moteur4.direction = choixSensMoteur;
      break;
    }
  default :
    {
      //ne rien faire
      break;
    }
  }
  
  //setter les pin de controle (voir si c'est correct)
  // 1 => moteur tourne sens anti-horaire (négatif)
  // 0 => moteur tourne sens horaire (positif)
  if(choixSensMoteur == 1)
  {
    GPIO_IO_CTRL->BSRRH = moteurCTRL1; //à 0
    GPIO_IO_CTRL->BSRRL = moteurCTRL2; //à 1
  }else
  {
    GPIO_IO_CTRL->BSRRL = moteurCTRL1; //à 1
    GPIO_IO_CTRL->BSRRH = moteurCTRL2; //à 0
  }
  
  //setter le duty cycle des PWM
  TIMER->CCR1 = dutyCycleChannel1;
  TIMER->CCR2 = dutyCycleChannel2;
  TIMER->CCR3 = dutyCycleChannel3;
  TIMER->CCR4 = dutyCycleChannel4;
}


void setVitesseMoteur(double dutyCycle, uint16_t numeroMoteur)
{
  uint16_t valeurDutyCycle = calculerValeurDutyCycle(dutyCycle);
  
    switch(numeroMoteur)
  {
  case 1 :
    {
      //setter moteur 1
      dutyCycleChannel1 = valeurDutyCycle;
      moteur1.dutyCycleMoteur = dutyCycle;
      break;
    }
  case 2 :
    {
      //setter moteur 2
      dutyCycleChannel2 = valeurDutyCycle;
      moteur2.dutyCycleMoteur = dutyCycle;
      break;
    }
  case 3 :
    {
      //setter moteur 3
      dutyCycleChannel3 = valeurDutyCycle;
      moteur3.dutyCycleMoteur = dutyCycle;
      break;
    }
  case 4 :
    {
      //setter moteur 4
      dutyCycleChannel4 = valeurDutyCycle;
      moteur4.dutyCycleMoteur = dutyCycle;
      break;
    }
  default :
    {
      //ne rien faire
      break;
    }
  }
  
  //setter le duty cycle des PWM
  TIMER->CCR1 = dutyCycleChannel1;
  TIMER->CCR2 = dutyCycleChannel2;
  TIMER->CCR3 = dutyCycleChannel3;
  TIMER->CCR4 = dutyCycleChannel4;
}

uint16_t calculerValeurDutyCycle(double dutyCycle)
{
    double valeur = (dutyCycle/(double)(100.00))*((double)(period));
    return (uint16_t)(valeur);
}

//retourner le duty cycle
double getDutyCycleMoteur(uint16_t numeroMoteur)
{
  if(numeroMoteur == 1)
  {
    return moteur1.dutyCycleMoteur;
  }else if(numeroMoteur == 2)
  {
    return moteur2.dutyCycleMoteur;
  }else if(numeroMoteur == 3)
  {
    return moteur3.dutyCycleMoteur;
  }else
  {
    return moteur4.dutyCycleMoteur;
  }
}

//retourner la direction du moteur
uint16_t getDirectionMoteur(uint16_t numeroMoteur)
{
  if(numeroMoteur == 1)
  {
    return moteur1.direction;
  }else if(numeroMoteur == 2)
  {
    return moteur2.direction;
  }else if(numeroMoteur == 3)
  {
    return moteur3.direction;
  }else
  {
    return moteur4.direction;
  }
}

void setCommencerTest(uint16_t valeur)
{
  commencerTest = valeur;
}

uint16_t getCommencerTest(void)
{
  return commencerTest;
}


