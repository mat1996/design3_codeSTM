#include "fichier.h"
#include "stm32f4_discovery.h"
#include "tm_stm32f4_spi.h"
#include "tm_stm32f4_delay.h"

GPIO_InitTypeDef  GPIO_CS;

int LireMemoireEEPROM(uint16_t AdresseEEPROM, uint32_t NbreOctet, uint8_t *Destination)
{
  if(AdresseEEPROM > nbOctetMax -1 )
  {
    return 1;
  }
  
  uint16_t nbOctetEnvoie = NbreOctet + 3;
  uint8_t* data_a_envoyer = (uint8_t*)malloc(nbOctetEnvoie);
  
  for(int i =0; i<(nbOctetEnvoie); i++)
    {
      if(i == 0)
      {
        data_a_envoyer[i] = instruction_read;
      }
      else if(i == 1)
      {
        data_a_envoyer[i] = (uint8_t)(AdresseEEPROM/256);
      }
      else if(i == 2)
      {
        data_a_envoyer[i] = (uint8_t)(AdresseEEPROM & 255);
      }
      else
      {
        data_a_envoyer[i] = 0;
      }
    }
  
  //envoyer instruction de lecture
  GPIO_ResetBits(GPIOD, GPIO_Pin_6);
  TM_SPI_SendMulti(SPI3, &(*data_a_envoyer), Destination, nbOctetEnvoie);
  Delayms(2);
  GPIO_SetBits(GPIOD, GPIO_Pin_6);


  return 0;
}


int EcrireMemoireEEPROM(uint16_t AdresseEEPROM, uint32_t NbreOctet, uint8_t *Source)
{
  if((AdresseEEPROM > nbOctetMax -1 ) || ((AdresseEEPROM + NbreOctet) > nbOctetMax -1))
  {
    return 1;
  }
  
 
  uint8_t* dataGarbage = 0;
  for(int i = AdresseEEPROM; i < AdresseEEPROM + NbreOctet; i++)
  {
    
     //envoyer le write ENABLE (delay en 10Xus)
    GPIO_ResetBits(GPIOD, GPIO_Pin_6);
    TM_SPI_Send(SPI3, 6);
    Delayms(1);
    GPIO_SetBits(GPIOD, GPIO_Pin_6);
  
    Delayms(1);
  
    uint8_t* data_a_envoyer = (uint8_t*)malloc(4);
    data_a_envoyer[0] = 2;
    data_a_envoyer[1] = (uint8_t)(i/256);
    data_a_envoyer[2] = (uint8_t)(i & 255);
    data_a_envoyer[3] = Source[i-AdresseEEPROM];
    
    //envoyer instruction d'écriture
    GPIO_ResetBits(GPIOD, GPIO_Pin_6);
    TM_SPI_SendMulti(SPI3, data_a_envoyer, dataGarbage, 4);
    Delayms(2);
    GPIO_SetBits(GPIOD, GPIO_Pin_6);
    
    //attendre
    Delayms(550);
  }      
  return 0;
}

//initialisation du module
void initModule(void)
{
  //initialiser le timer Systick
  if (SysTick_Config(SystemCoreClock / 100000))
  { 
    /* Capture error */ 
    while (1);
  }
  
  //initialiser GPIO pour chip enable
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  GPIO_CS.GPIO_Pin = GPIO_Pin_6;
  GPIO_CS.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_CS.GPIO_OType = GPIO_OType_PP;
  GPIO_CS.GPIO_PuPd = GPIO_PuPd_UP; 
  GPIO_CS.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOD, &GPIO_CS);
  GPIO_SetBits(GPIOD, GPIO_Pin_6);
  
  //initialiser le module SPI
  TM_SPI_InitFull(SPI3, TM_SPI_PinsPack_1, SPI_BaudRatePrescaler_256, TM_SPI_Mode_0 , SPI_Mode_Master, SPI_FirstBit_MSB);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}
