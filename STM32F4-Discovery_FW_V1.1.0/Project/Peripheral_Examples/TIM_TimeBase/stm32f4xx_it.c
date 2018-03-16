/**
  ******************************************************************************
  * @file    TIM_TimeBase/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "stm32f4_discovery.h"
#include "main.h"
#include "controleMoteur.h"
#include "asservissementVitesseAngulaire.h"
#include "asservissementVitesseLineaire.h"
#include "controleDistanceCapRobot.h"
#include "asservissementCap.h"

    
#define abs(x)(x>=0 ? x : ((double)(-1))*x)
    
/** @addtogroup STM32F4_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup TIM_TimeBase
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t capture = 0;
extern __IO uint16_t CCR1_Val;
extern __IO uint16_t CCR2_Val;
extern __IO uint16_t CCR3_Val;
extern __IO uint16_t CCR4_Val;

uint32_t comptePulseTIM1 = 0;
uint32_t comptePulseTIM2 = 0;
uint32_t comptePulseTIM3 = 0;
uint32_t comptePulseTIM4 = 0;
double tableauVitesseM1[300];
double tableauVitesseM2[300];
double consigneVitesseM1[300];
double consigneVitesseM2[300];
uint16_t indiceM1 = 0;
uint16_t indiceM2 = 0;

double diametreRoue = 0.065; 
double rayonBraquage = 0.0525;
double angleRobot = 0;


extern __IO char bufferRx[50];
extern __IO uint32_t indexEcritureRX;
extern __IO uint32_t indexLectureRX;
__IO char bufferTX[50];
__IO uint32_t indexEcritureTX = 0;
__IO uint32_t indexLectureTX = 0;

extern __IO char bufferRx_PC[50];
extern __IO uint32_t indexEcritureRX_PC;
extern __IO uint32_t indexLectureRX_PC;
__IO char bufferTX_PC[50];
__IO uint32_t indexEcritureTX_PC = 0;
__IO uint32_t indexLectureTX_PC = 0;





/* Private function prototypes -----------------------------------------------*/



/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  TimingDelay_Decrement();
}

/******************************************************************************/
/*            STM32F4xx Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
  * @brief  This function handles TIM3 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM5_IRQHandler(void)
{
//  if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
//  {
//    TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
//
//    /* LED4 toggling with frequency = 4.57 Hz */
//    STM_EVAL_LEDToggle(LED4);
//	capture = TIM_GetCapture1(TIM3);
//    TIM_SetCompare1(TIM3, capture + CCR1_Val);
//  }
//  else if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
//  {
//    TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
//
//    /* LED3 toggling with frequency = 9.15 Hz */
//    STM_EVAL_LEDToggle(LED3);
//    capture = TIM_GetCapture2(TIM3);
//    TIM_SetCompare2(TIM3, capture + CCR2_Val);
//  }
//  else if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET)
//  {
//    TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
//
//    /* LED5 toggling with frequency = 18.31 Hz */
//    STM_EVAL_LEDToggle(LED5);
//    capture = TIM_GetCapture3(TIM3);
//    TIM_SetCompare3(TIM3, capture + CCR3_Val);
//  }
//  else
//  {
//    TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);
//
//    /* LED6 toggling with frequency = 36.62 Hz */
//    STM_EVAL_LEDToggle(LED6);
//    capture = TIM_GetCapture4(TIM3);
//    TIM_SetCompare4(TIM3, capture + CCR4_Val);
//  }
  
  //interruption pour seulement calculer la vitesse (pas faire de consigne, traitement court)
  if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
  {
    //calcul de la vitesse angulaire
    comptePulseTIM1 = TIM_GetCounter(TIM1);
    comptePulseTIM2 = TIM_GetCounter(TIM2);
    comptePulseTIM3 = TIM_GetCounter(TIM3);
    comptePulseTIM4 = TIM_GetCounter(TIM4);
    double pi = (double)(3.1416);
    double nbRevolution1Tour = (double)(1600);
    double nbTourM1 = (double)(comptePulseTIM1/nbRevolution1Tour);
    double nbTourM2 = (double)(comptePulseTIM2/nbRevolution1Tour);
    double nbTourM3 = (double)(comptePulseTIM3/nbRevolution1Tour);
    double nbTourM4 = (double)(comptePulseTIM4/nbRevolution1Tour);
    
    //début de l'asservissement
    //if(getCommencerTest() == 1)
    //{
      //calcul de la consigne
      //int directionM1 = getDirectionMoteur(1);
      //int directionM2 = getDirectionMoteur(3);
      double vitesseAngulaireM1 = (double)(2)*pi*nbTourM1/(double)(0.025);
      double vitesseAngulaireM2 = (double)(2)*pi*nbTourM2/(double)(0.025);
      double vitesseAngulaireM3 = (double)(2)*pi*nbTourM3/(double)(0.025);
      double vitesseAngulaireM4 = (double)(2)*pi*nbTourM4/(double)(0.025);
      
      
      //affichage récupération données
      double vitesseLineaireG1 = (double)(0.5)*(diametreRoue/((double)(2.0)))*(vitesseAngulaireM1 + vitesseAngulaireM3);
      double vitesseLineaireG2 = (double)(0.5)*(diametreRoue/((double)(2.0)))*(vitesseAngulaireM2 + vitesseAngulaireM4);
      
      double moyenneVitesse = (vitesseAngulaireM1 + vitesseAngulaireM2 + vitesseAngulaireM3 + vitesseAngulaireM4)/((double)(4.0)) ;
      angleRobot = (moyenneVitesse)*(double)(0.025)/rayonBraquage;//((double)(0.025)*pi*diametreRoue*moyenneTour)/rayonBraquage;
      tableauVitesseM1[indiceM1] = comptePulseTIM2;//getDirectionMoteur(2) == 0?angleRobot:(double)(-1)*angleRobot;//vitesseAngulaireM1;//getConsigneVitesseLineaire(1);//vitesseLineaireG1;//getDirectionMoteur(2) == 0?angleRobot:(double)(-1)*angleRobot;
      tableauVitesseM2[indiceM2] = vitesseAngulaireM2;//getConsigneVitesseLineaire(2);//vitesseLineaireG2;//getDirectionMoteur(2) == 0?vitesseAngulaireM1:(double)(-1)*vitesseAngulaireM1;
      consigneVitesseM1[indiceM1] = getDirectionMoteur(2) == 0?getConsigneVitesseAngulaire(2):(double)(-1)*getConsigneVitesseAngulaire(2);
      
      double moyenneTourG1 = (nbTourM1 + nbTourM3)/((double)(2.0));
      double moyenneTourG2 = (nbTourM2 + nbTourM4)/((double)(2.0));

      int typeAsservissement = getTypeAsservissement();
      
      if(typeAsservissement == 1)
      {
        //asservissement du cap
        double actionVitesseAngulaire = calculerActionCap(angleRobot);
        setConsigneVitesseAngulaire(1, actionVitesseAngulaire, getDirectionConsigneCap());
        setConsigneVitesseAngulaire(2, actionVitesseAngulaire, getDirectionConsigneCap());
        setConsigneVitesseAngulaire(3, actionVitesseAngulaire, getDirectionConsigneCap());
        setConsigneVitesseAngulaire(4, actionVitesseAngulaire, getDirectionConsigneCap());
      }else if(typeAsservissement == 2)
      {
        //asservissement de vitesse linéaire
        double distanceParcouruG1 = pi*diametreRoue*moyenneTourG1;
        double distanceParcouruG2 = pi*diametreRoue*moyenneTourG2;
        //hardcodé
        
        //calculerConsigneVitesseLineaire(distanceParcouruG1);
        //double consigneVitesseLineaireG2 = 
        if(getDirectionDistanceCible() == 0)
        {
          //on veut aller en avant
          setConsigneVitesseLineaire(1, calculerConsigneVitesseLineaire(distanceParcouruG1));
        }else if(getDirectionDistanceCible() == 1)
        {
          //on veut aller par l'arrière
          setConsigneVitesseLineaire(1,(double)(-1)* calculerConsigneVitesseLineaire(distanceParcouruG1));
        }else if(getDirectionDistanceCible() == 2)
        {
          //on veut aller à droite
          setConsigneVitesseLineaire(2, calculerConsigneVitesseLineaire(distanceParcouruG2));
        }else
        {
          //on veut aller à gauche
          setConsigneVitesseLineaire(2,(double)(-1)* calculerConsigneVitesseLineaire(distanceParcouruG2));
        }
        
        
        //appliquer la consigne de vitesse pour les moteur
        double actionVitesseM13 = calculerActionVitesseLineaire(1, vitesseLineaireG1);
        setConsigneVitesseAngulaire(1, actionVitesseM13, getConsigneDirectionVitesseLineaire(1) == 0 ? 1 : 0);
        setConsigneVitesseAngulaire(3, actionVitesseM13, getConsigneDirectionVitesseLineaire(1));
        
        double actionVitesseM24 = calculerActionVitesseLineaire(2, vitesseLineaireG2);
        setConsigneVitesseAngulaire(2, actionVitesseM24, getConsigneDirectionVitesseLineaire(2));
        setConsigneVitesseAngulaire(4, actionVitesseM24, getConsigneDirectionVitesseLineaire(2) == 0 ? 1 : 0);
      }
   
      //appliquer la consigne du PWM sur les moteur
      avancerMoteur(calculerActionMoteur(1,vitesseAngulaireM1), 1, getConsigneDirectionVitesseAngulaire(1));
      avancerMoteur(calculerActionMoteur(2,vitesseAngulaireM2), 2, getConsigneDirectionVitesseAngulaire(2));
      avancerMoteur(calculerActionMoteur(3,vitesseAngulaireM3), 3, getConsigneDirectionVitesseAngulaire(3));
      avancerMoteur(calculerActionMoteur(4,vitesseAngulaireM4), 4, getConsigneDirectionVitesseAngulaire(4));      
      
      TIM_SetCounter(TIM1, 0);
      TIM_SetCounter(TIM2, 0);
      TIM_SetCounter(TIM3, 0);
      TIM_SetCounter(TIM4, 0);
      
      GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
    
    TIM_ClearITPendingBit (TIM5, TIM_IT_Update);
  }
}



void USART2_IRQHandler(void)
{
  if(USART_GetITStatus(USART2, USART_IT_RXNE) )
  {
    //ajouter octet dans buffer circulaire
    char octetRecu = USART2->DR;
    bufferRx[indexEcritureRX] = octetRecu;
    indexEcritureRX++;
    if(indexEcritureRX >= sizeof(bufferRx))
      indexEcritureRX = 0;
    
    USART_ClearITPendingBit (USART2, USART_IT_RXNE);
  }
  
  if(USART_GetITStatus(USART2, USART_IT_TXE))
  {
    //envoyer les octets 
    if(indexLectureTX != indexEcritureTX)
    {
      USART2->DR = bufferTX[indexLectureTX]; //vérifier que c'est le bon registre
      indexLectureTX++;
      if(indexLectureTX > sizeof(bufferTX))
        indexLectureTX = 0;
    }else
    {
        USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
    }
    
    USART_ClearITPendingBit (USART2, USART_IT_TC);
  }
}

void USART3_IRQHandler(void)
{
  if(USART_GetITStatus(USART3, USART_IT_RXNE) )
  {
    //ajouter octet dans buffer circulaire
    char octetRecu = USART3->DR;
    bufferRx_PC[indexEcritureRX_PC] = octetRecu;
    indexEcritureRX_PC++;
    if(indexEcritureRX_PC >= sizeof(bufferRx_PC))
      indexEcritureRX_PC = 0;
    
    USART_ClearITPendingBit (USART3, USART_IT_RXNE);
  }
  
  if(USART_GetITStatus(USART3, USART_IT_TXE))
  {
    //envoyer les octets 
    if(indexLectureTX_PC != indexEcritureTX_PC)
    {
      USART3->DR = bufferTX_PC[indexLectureTX_PC]; //vérifier que c'est le bon registre
      indexLectureTX_PC++;
      if(indexLectureTX_PC > sizeof(bufferTX_PC))
        indexLectureTX_PC = 0;
    }else
    {
        USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
    }
    
    USART_ClearITPendingBit (USART3, USART_IT_TC);
  }
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
