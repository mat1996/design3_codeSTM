//version du laboratoire 1 cours SMI
//Mathieu Gagnon


/* Includes ------------------------------------------------------------------*/
#include "defines.h"
#include "stm32f4xx.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_hd44780.h"

GPIO_InitTypeDef  GPIO_colonne, GPIO_ligne, GPIO_LCD_data, GPIO_LCD_ctrl;
GPIO_InitTypeDef user_button;
static __IO uint32_t TimingDelay;

#define nb_ligne 4
#define nb_col 4

/** @addtogroup STM32F4_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup IO_Toggle
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
//GPIO_InitTypeDef  GPIO_InitStructure;
uint16_t GPIO_PORT_OUT_LIGNE[4] = {GPIO_Pin_6, GPIO_Pin_7, GPIO_Pin_8,
                                 GPIO_Pin_9};
uint16_t GPIO_PORT_IN_COLONNE[4] = {GPIO_Pin_11, GPIO_Pin_12, GPIO_Pin_13,
                                 GPIO_Pin_14};

char* tableau_string_ecriture[4][4] = {{"1","2","3","A"},{"4","5","6","B"},{"7","8","9","C"},{"*","0","#","D"}};

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

int tableau_clavier[nb_ligne][nb_col];
int position_touche = 0;

void activer_ligne(__IO uint32_t p_numero_ligne);
void desactiver_ligne(__IO uint32_t p_numero_ligne);
int scanner_clavier(void);
void initialiser_clavier(void);
void reinitialiser_tableau_clavier(void);
char conversion_position_char(void);
__IO uint32_t lire_colonne(__IO uint32_t p_numero_colonne);
void afficher_char();


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  //initialiser système
  SystemInit();
  
  //initialiser clavier
  initialiser_clavier();
  
  //initialiser le LCD
  TM_HD44780_Init(16, 2);
  
  //écrire sur le LCD
  TM_HD44780_Puts(0, 0, "ALLO M.G.");
  int nb_touche_appuyee, nb_touche_appuyee_apres;
  while (1)
  {
    do
    {
      nb_touche_appuyee = scanner_clavier();
      Delayms(5);
    }while(nb_touche_appuyee == 0);
    
    //attendre pour debounce
    Delayms(50);
    nb_touche_appuyee_apres = scanner_clavier();
    
    if(nb_touche_appuyee == nb_touche_appuyee_apres)
    {
      //imprimer le/les char sur le LCD
      if(nb_touche_appuyee  < 3)
      {
        afficher_char();
      }
    }
    Delayms(115);
  }
}

//initialisation des I/O du clavier en input
void initialiser_clavier(void)
{
  //configurer les PIN du clavier en input pour ligne
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  GPIO_ligne.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9;
  GPIO_ligne.GPIO_Mode = GPIO_Mode_IN;
  GPIO_ligne.GPIO_OType = GPIO_OType_PP;
  GPIO_ligne.GPIO_PuPd = GPIO_PuPd_UP; //UP ou DOWN? Je crois que c'est PULL UP (a vcc quand on presse pas)
  GPIO_ligne.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOD, &GPIO_ligne);
  
  //configurer les PIN du cavier en input pour colonne
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  GPIO_colonne.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12| GPIO_Pin_13| GPIO_Pin_14;
  GPIO_colonne.GPIO_Mode = GPIO_Mode_IN;
  GPIO_colonne.GPIO_OType = GPIO_OType_PP;
  GPIO_colonne.GPIO_PuPd = GPIO_PuPd_UP; //UP ou DOWN?
  GPIO_colonne.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOB, &GPIO_colonne);
}

//remettre le tableau du clavier à 0
void reinitialiser_tableau_clavier(void)
{
  for(int i=0;i<nb_ligne;i++)
    for(int j=0;j<nb_col;j++)
      tableau_clavier[i][j] = 0;
}

//scanner le clavier
int scanner_clavier(void)
{
  int i,j;
  int nb_touche_presse = 0;
  int touche_presse = 0;
  reinitialiser_tableau_clavier();
  for(i = 0; i<nb_ligne; i++)
  {
    activer_ligne(i);
    
    for(j = 0;j<nb_col;j++)
    {
      touche_presse = lire_colonne(j);
      if(touche_presse == 1)
      {
        tableau_clavier[i][j] = 1;
        nb_touche_presse++;
      }
    }
    
    desactiver_ligne(i);
  }
  
  return nb_touche_presse;
}

//faire l'association entre position dans le tableau et le caractère du clavier
void afficher_char()
{
  
  for(int i=0;i<nb_ligne;i++)
  {
    for(int j =0;j<nb_col;j++)
    {
      if(tableau_clavier[i][j] == 1)
      {
        if(i == 3 && j == 2)
        {
          TM_HD44780_Clear();
          TM_HD44780_Puts(0, 0, "ALLO M.G.");
          position_touche = 0;
        }
        else
        {
          if(position_touche < (nb_col*nb_ligne))
          {
            TM_HD44780_Puts(position_touche, 1, tableau_string_ecriture[i][j]);
            position_touche++;
          }
        }
      }
    }
  }
}

//lire la colonne du clavier et retourner si la touche est pressée
__IO uint32_t lire_colonne(__IO uint32_t p_numero_colonne)
{
  if(GPIO_ReadInputDataBit(GPIOB, GPIO_PORT_IN_COLONNE[p_numero_colonne]))
    return 0;
  else
    return 1;
}
                                                       
//activer la ligne du clavier pour la lecture
void activer_ligne(__IO uint32_t p_numero_ligne)
{
  //configurer PIN en output
  GPIO_ligne.GPIO_Pin = GPIO_PORT_OUT_LIGNE[p_numero_ligne];
  GPIO_ligne.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_ligne.GPIO_OType = GPIO_OType_PP;
  GPIO_ligne.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_ligne.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOD, &GPIO_ligne);
  
  //éteindre ligne
  GPIO_ResetBits(GPIOD, GPIO_PORT_OUT_LIGNE[p_numero_ligne]);
}

//desactiver la ligne du clavier pour la lecture (en mode input)
void desactiver_ligne(__IO uint32_t p_numero_ligne)
{
  //configurer PIN en input
  GPIO_ligne.GPIO_Pin = GPIO_PORT_OUT_LIGNE[p_numero_ligne];
  GPIO_ligne.GPIO_Mode = GPIO_Mode_IN;
  GPIO_ligne.GPIO_OType = GPIO_OType_PP;
  GPIO_ligne.GPIO_PuPd = GPIO_PuPd_UP; //UP ou DOWN?
  GPIO_ligne.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOD, &GPIO_ligne);
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
