/**
  ******************************************************************************
  * @file    TIM_TimeBase/main.c 
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
#include "controleMoteur.h"
#include "configTimer.h"
#include "asservissementVitesseAngulaire.h"
#include "asservissementVitesseLineaire.h"
#include "USART_config.h"
#include "FSM_USART_codePays.h"
#include "FSM_USART_commande.h"
#include "controleDistanceCapRobot.h"
#include "asservissementCap.h"
#include "main.h"

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
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
__IO uint16_t CCR1_Val = 54618;
__IO uint16_t CCR2_Val = 27309;
__IO uint16_t CCR3_Val = 13654;
__IO uint16_t CCR4_Val = 6826;
uint16_t PrescalerValue = 0;
static __IO uint32_t TimingDelay;
static int typeAsservissement = 0;

//interrupt à chaque 100 ms
uint16_t prescalerTimer2 = 40000;
uint16_t periodeTimer2 = 210;
uint16_t prescalerTimer3 = 0;

//USART2 (COM IR)
__IO uint32_t indexEcritureRX = 0;
__IO uint32_t indexLectureRX = 0;
__IO char bufferRx[TAILLE_BUFFER];
extern __IO char bufferTX[TAILLE_BUFFER];
extern __IO uint32_t indexEcritureTX;
extern __IO uint32_t indexLectureTX;

//USART3 (COM mini-PC)
__IO uint32_t indexEcritureRX_PC = 0;
__IO uint32_t indexLectureRX_PC = 0;
__IO char bufferRx_PC[TAILLE_BUFFER];
extern __IO char bufferTX_PC[TAILLE_BUFFER];
extern __IO uint32_t indexEcritureTX_PC;
extern __IO uint32_t indexLectureTX_PC;

int codePaysRecu = 0;
char octetRecuIR, octetRecuPC;
int recevoirDrapeau = 0;

//char *codePays;

/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */

  /* TIM Configuration */

  /* -----------------------------------------------------------------------
    TIM3 Configuration: Output Compare Timing Mode:
    
    In this example TIM3 input clock (TIM3CLK) is set to 2 * APB1 clock (PCLK1), 
    since APB1 prescaler is different from 1.   
      TIM3CLK = 2 * PCLK1  
      PCLK1 = HCLK / 4 
      => TIM3CLK = HCLK / 2 = SystemCoreClock /2
          
    To get TIM3 counter clock at 50 MHz, the prescaler is computed as follows:
       Prescaler = (TIM3CLK / TIM3 counter clock) - 1
       Prescaler = ((SystemCoreClock /2) /50 MHz) - 1
                                              
    CC1 update rate = TIM3 counter clock / CCR1_Val = 9.154 Hz
    ==> Toggling frequency = 4.57 Hz
    
    C2 update rate = TIM3 counter clock / CCR2_Val = 18.31 Hz
    ==> Toggling frequency = 9.15 Hz
    
    CC3 update rate = TIM3 counter clock / CCR3_Val = 36.62 Hz
    ==> Toggling frequency = 18.31 Hz
    
    CC4 update rate = TIM3 counter clock / CCR4_Val = 73.25 Hz
    ==> Toggling frequency = 36.62 Hz

    Note: 
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f4xx.c file.
     Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
     function to update SystemCoreClock variable value. Otherwise, any configuration
     based on this variable will be incorrect.    
  ----------------------------------------------------------------------- */  

  //configurer les timer pour compter pulse
//  initTimerCLKExterne(TIM11, 0, 50000);
//  initTimerCLKExterne(TIM12, 0, 50000);
//  initTimerCLKExterne(TIM13, 0, 50000);
//  initTimerCLKExterne(TIM14, 0, 50000);
  
  //init timer interrupt 100 ms
  //initTimerInterruptXms(40000, 210);
  
    if (SysTick_Config(SystemCoreClock / 1000))
  { 
    /* Capture error */ //[81]
    while (1);
  }
  
 
  
  //char *codePays;
  //tests
  //configurer le bouton le GPIO du bouton bleu user
  GPIO_InitTypeDef user_button;
  user_button.GPIO_Pin = GPIO_Pin_0;
  user_button.GPIO_Mode = GPIO_Mode_IN;
  user_button.GPIO_OType = GPIO_OType_PP;
  user_button.GPIO_PuPd = GPIO_PuPd_DOWN;
  user_button.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOA, &user_button);
  
  //INIT LED
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED5);
  STM_EVAL_LEDInit(LED6); 
  
  //configurer les timer pour interrupt 100 ms
  initTimerCLKExterne();
  initTimerInterruptXms(10000, 210);
  
  //configurer USART
  InitUSART();
  
   //configure la drive du moteur
   initIOMoteur();
   initPWM();
   initPIVitesseAngulaire();
   initPIVitesselineaire();
   initPICap();
   
   //int i =0;
   //while(i < 1)
   //{
//   setConsigneVitesseAngulaire(1, 2.5,1);
//   setConsigneVitesseAngulaire(2, 2.5,1);
//   setConsigneVitesseAngulaire(3, 2.5,1);
//   setConsigneVitesseAngulaire(4, 2.5,1);
   //setConsigneVitesseLineaire(2, 0.14);
   
   //avancer moteur
   //setDistanceCible(0.66, 3); //test
   //setCommencerTest(1);      //test
   //avancerMoteur(50,1,1);
   //avancerMoteur(50,3,0); 
   //setConsigneCap(90,0);
   //setCommencerTest(1);
//   setConsigneVitesseAngulaire(1, 1.5,1);
//   setConsigneVitesseAngulaire(2, 1.5,1);
//   setConsigneVitesseAngulaire(3, 1.5,1);
//   setConsigneVitesseAngulaire(4, 1.5,1);
    //Delay(10000);
    //setCommencerTest(0);
    //setDistanceCible(0.66, 0);
    //setCommencerTest(1);
    //Delay(10000);
  // setConsigneCap(90,1);
  // Delay(5000);
   //setCommencerTest(0);
   //setDistanceCible(0.66, 3);
   //setCommencerTest(1);
   //Delay(5000);
   //setCommencerTest(0);
//   setDistanceCible(0.66, 1);
//   setCommencerTest(1);
//   Delay(5000);
//   setCommencerTest(0);
//   setDistanceCible(0.66, 2);
//   setCommencerTest(1);
////   Delay(5000);
//   setConsigneVitesseAngulaire(1, 0,0);
//   setConsigneVitesseAngulaire(2, 0,0);
//   setConsigneVitesseAngulaire(3, 0,0);
//   setConsigneVitesseAngulaire(4, 0,0);
   //Delay(2000);
   //setConsigneVitesseAngulaire(3, -5.0);
   //arreterMoteur(1);
   //arreterMoteur(3);
   //Delay(3000);
   //avancerMoteur(50,1,0);
   //avancerMoteur(50,3,1);
   //Delay(3000);
   //arreterMoteur(1);
  //arreterMoteur(3);
   //setConsigneVitesseAngulaire(1, 0.0,0);
   //setConsigneVitesseAngulaire(2, 0.0,0);
   //setConsigneVitesseAngulaire(3, 0.0,0);
   //setConsigneVitesseAngulaire(4, 0.0,0);
//   
   //setConsigneVitesseLineaire(2, 0.0);
//   setCommencerTest(0);
//   arreterMoteur(2);
//   arreterMoteur(4);
//   initPIVitesseAngulaire();
//   initPIVitesselineaire();
   //Delay(1000);
//   setCommencerTest(1);
   //setCommencerTest(0);
   //setCommencerTest(1);
//   setConsigneVitesseAngulaire(1, 1.5,0);
//   setConsigneVitesseAngulaire(2, 1.5,0);
//   setConsigneVitesseAngulaire(3, 1.5,0);
//   setConsigneVitesseAngulaire(4, 1.5,0);
   //setConsigneVitesseLineaire(2, -0.14);
//   Delay(3250);
//   setConsigneVitesseAngulaire(1, 0.0,0);
//   setConsigneVitesseAngulaire(2, 0.0,0);
//   setConsigneVitesseAngulaire(3, 0.0,0);
//   setConsigneVitesseAngulaire(4, 0.0,0);
//   setConsigneVitesseLineaire(2, 0.0);
//   setCommencerTest(0);
//   arreterMoteur(2);
//   arreterMoteur(4);
//   initPIVitesseAngulaire();
//   initPIVitesselineaire(); 
   //Delay(1000);
   //setCommencerTest(0);
   //Delay(1000);
   //i++;
   //}

   //arreterMoteur(1);
   //arreterMoteur(3);
   //setVitesseMoteur(25,2);
   //setVitesseMoteur(25,1);
   //setVitesseMoteur(25,3);
   //setVitesseMoteur(25,4);
   //Delay(2000);
   //arreterMoteur(1);
   
   //arreterMoteur(3);
   
   //Delay(1000);
   //arreterMoteur(4);
   
//  avancerMoteur(50,1,0); 
//  avancerMoteur(50,2,0);
//  avancerMoteur(50,3,0);
//  avancerMoteur(50,4,0);
//
//  Delay(5000);
//  
//  arreterMoteur(1);
//  arreterMoteur(2);
//  arreterMoteur(3);
//  arreterMoteur(4);
  
  while (1)
  {
    
    //Voir pourquoi le robot n'exécute pas des commandes?
    //déplacement trop petit?
    //rotation trop petite?
    //manque des commande autant pour les rotations/déplacement
    //reste pris dans l'asservissement du déplacement à l'infini!! pourquoi??
    //teste l'envoie de commandes successives, voir ce qu'on recoit
    //revoir les fichiers asservissementCap, asservissementVitesselineaire, asservissementVitesseAngulaire, controleDistanceCap
    //
    
    //récupération du code du pays
    if(indexLectureRX != indexEcritureRX && recevoirDrapeau == 1)
    {
      octetRecuIR = bufferRx[indexLectureRX];
      indexLectureRX++;
      if(indexLectureRX > (TAILLE_BUFFER - 1))
        indexLectureRX = 0;
      int valeurOut = recevoirOctetDrapeau(octetRecuIR);
      if(valeurOut != 0)
      {
        codePaysRecu = valeurOut;
        char envoiCodePays[3];
        envoiCodePays[0] = '1';
        envoiCodePays[1] = (uint8_t)(codePaysRecu);
        envoiCodePays[2] = (uint8_t)(256 - ('1' + (uint8_t)(codePaysRecu)));
        envoyerOctets(envoiCodePays, 3);
        recevoirDrapeau = 0;
      }
      //faire ce que l'on veut avec l'octet, envoyer sur TX?
    }
    
    //récupération des commandes envoyée du MINI-PC
    if(indexLectureRX_PC != indexEcritureRX_PC)
    {
      octetRecuPC = bufferRx_PC[indexLectureRX_PC];
      indexLectureRX_PC++;
      if(indexLectureRX_PC > (TAILLE_BUFFER -1))
        indexLectureRX_PC = 0;
      int retour = recevoirOctetCMD(octetRecuPC);
      
      if(retour == 1)
      {
        //commande de déplacement
        GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
        appliquerConsigneDeplacement();
      }else if(retour == 2)
      {
        //commande de rotation
        GPIO_ToggleBits(GPIOD,GPIO_Pin_14);
        appliquerConsigneRotation();
      }else if(retour == 3)
      {
        //commande de demande du drapeau
        GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
        recevoirDrapeau = 1;
      }
     
      //rien faire
      
      //décoder les octets recu
      //envoyer des commandes aux robots
      //changer l'état du robot 
    }
    
    
//    //allumer LED 4 et LED 6
//    GPIO_SetBits(GPIOD, GPIO_Pin_12);
//    GPIO_SetBits(GPIOD, GPIO_Pin_15);
//
//    //délai de 1 ms
//    Delay(10);
//    
//    //éteindre les LED 4 et 6
//    GPIO_ResetBits(GPIOD, GPIO_Pin_12);
//    GPIO_ResetBits(GPIOD, GPIO_Pin_15);
//    
//    //délai 0.5 ms
//    Delay(5);
  }
}

void envoyerOctets(const char* envoie, int tailleEnvoie)
{
  //Pour tester, changer dans le code python à 9600 bauds
  for(int i = 0;i<tailleEnvoie;i++)
  {
    bufferTX_PC[indexEcritureTX_PC] = envoie[i];
    indexEcritureTX_PC++;
    if(indexEcritureTX_PC > sizeof(bufferTX_PC))
      indexEcritureTX_PC = 0;
  }
  USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
  
}

void envoyerFinDeplacement()
{
  char finDep[1] = {'M'};
  envoyerOctets(finDep, 1);
}

void envoyerFinRotation()
{
  
  char finRot[1] = {'R'};
  envoyerOctets(finRot, 1);
}

void appliquerConsigneDeplacement(void)
{
  initPIVitesseAngulaire();
  initPIVitesselineaire();
  initPICap();
  setDistanceCible(getDistanceDeplacement(), getDirectionDeplacement(), getVitesseLineaireMax());
  typeAsservissement = 2;
  
  //activer les timer des encodeurs
  TIM_Cmd(TIM1, ENABLE);
  TIM_Cmd(TIM2, ENABLE);
  TIM_Cmd(TIM3, ENABLE);
  TIM_Cmd(TIM4, ENABLE);
  
  //activer le timer des interrupt
  TIM_Cmd(TIM5, ENABLE);
  
  //faire de l'attente active pour attendre la fin du déplacement (changer pour attente active)

  
  //activer les timer des encodeurs
  
  //activer le timer des interrupt
}

void appliquerConsigneRotation(void)
{
  initPIVitesseAngulaire();
  initPIVitesselineaire();
  initPICap();
  setConsigneCap(getAngleRotation(), getDirectionRotation());
  typeAsservissement = 1;
  
  //activer les timer des encodeurs
  TIM_Cmd(TIM1, ENABLE);
  TIM_Cmd(TIM2, ENABLE);
  TIM_Cmd(TIM3, ENABLE);
  TIM_Cmd(TIM4, ENABLE);
  
  //activer le timer des interrupt
  TIM_Cmd(TIM5, ENABLE);  
  
  //faire de l'attente active pour attendre la fin du déplacement (changer pour attente active)
  //while(typeAsservissement != 0){};
  
  //activer les timer des encodeurs

  
  //activer le timer des interrupt
  
  
}

int getTypeAsservissement(void)
{
  return typeAsservissement;
}

void setTypeAsservissement(uint16_t type)
{
  typeAsservissement = type;
}


/**
  * @brief  Configure the TIM IRQ Handler.
  * @param  None
  * @retval None
  */
void TIM_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

 //initialiser les interruptions du timer 3
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
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

  while (1)
  {}
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
