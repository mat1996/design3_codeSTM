/**
  ******************************************************************************
  * @file    EXTI/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Main program body
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
#include "stm32f4_discovery.h"
#include "defines.h"
#include "stm32f4xx.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_hd44780.h"
#include "config_timer.h"
#include "main.h"
#include "USART_config.h"
#include "command.h"
    
__IO uint32_t incrementMS = 0;
__IO uint32_t indexEcriture = 0;
__IO uint32_t indexLecture = 0;
__IO char bufferRx[20];

/** @addtogroup STM32F4_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup EXTI
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/



/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  //initiliser l'écran
  TM_HD44780_Init(16, 2);
  
  //inittialiser le timer 2
  initTimerInterruptMS();
  
  //initialiser le USART 2
  InitUSART();
  
  //afficher les initiales sur le clavier
  TM_HD44780_Puts(0, 0, "M.G.");

  //variables pour les commandes
  char commandeAExecuter;
  char donneeCommande;
  char octetAConvertir;
  
  while (1)
  {
    if(incrementMS % 1000 == 0)
    {
      //compter les secondes
      int time_stamp = incrementMS;
      int seconde = time_stamp/1000;
      char secondeSTR[5];
      sprintf(secondeSTR, "%5d", seconde);
      TM_HD44780_Puts(11, 0, secondeSTR);
    }
    
    if(indexLecture != indexEcriture)
    {
      //récupérer l'octet à convertir
      octetAConvertir = bufferRx[indexLecture];
      indexLecture++;
      if(indexLecture >= sizeof(bufferRx))
        indexLecture = 0;
      
      //convertir l'octet recu
      int commandeEstValide = convertirCommande(octetAConvertir, &commandeAExecuter, &donneeCommande);
      if(commandeEstValide == 1)
        executerCommande(commandeAExecuter, donneeCommande);
    }
    
    //patie 2 lab
    GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
  }
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
