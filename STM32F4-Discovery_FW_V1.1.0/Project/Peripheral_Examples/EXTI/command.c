
#include "command.h"
#include "stm32f4_discovery.h"
#include "tm_stm32f4_hd44780.h"

uint32_t indiceEcritureLCD = 0;
uint32_t etatFSM = 0; //0 => attendre la commande, 1 => attendre donnée comande, 2 => calculer le checksum

int indiceColLCD = 0;

int convertirCommande(char octetAConvertir, char* commande, char* donneeCommande)
{
   switch(etatFSM)
  {
  case 0 : 
    if(octetAConvertir == 0x41 || octetAConvertir == 0x42 || octetAConvertir == 0x43)
    {
      *commande = octetAConvertir;
      etatFSM = 1;
    }
    return 0;
    
  case 1 :
    *donneeCommande = octetAConvertir;
    etatFSM = 2;
    return 0;
    
  case 2 :
    etatFSM = 0;
    unsigned char checksum = *commande + *donneeCommande + octetAConvertir;
    if(checksum == 0)
      return 1;
    else
      return 0;
    
  default :
    return 0;
  }
}

void executerCommande(char commande, char donneeCommande)
{
  switch(commande)
  {
  case 0x41 :
    //allumer ou éteindre la LED bleu
    if(donneeCommande == 0x31)
      GPIO_SetBits(GPIOD, GPIO_Pin_15);
    else
      GPIO_ResetBits(GPIOD, GPIO_Pin_15);
    break;
    
  case 0x42 :
    //clear la 2ième ligne du LCD
    TM_HD44780_Puts(0, 1, "                ");
    indiceColLCD = 0;
    break;
    
  case 0x43 :
    //écrire sur la 2ième ligne du LCD
    if(indiceColLCD < 16)
    {
      TM_HD44780_Puts(indiceColLCD, 1, &donneeCommande);
      indiceColLCD++;
    }
    break;
    
  default :
    break;
    
  }
  
}