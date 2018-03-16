#include "FSM_USART_codePays.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


uint32_t etatFSM = 0; //0 => attente, 1 => valider le headers, 2 => recevoir le payload, 3 => recevoir le checksum, 4 => valider le checksum

char headerCOM[TAILLE_HEADER];
char codePays[TAILLE_HEADER];
char checksumCOM[TAILLE_HEADER];
int indiceHeader = 0;
int indiceCodePays = 0;
int indiceChecksum = 0;
uint8_t codePaysInteger = 0;
int checksum = 0;

int recevoirOctetDrapeau(char octetRecu)
{
  switch(etatFSM)
  {
    case(0):
    //recevoir le '$'
      indiceCodePays = 0;
      indiceHeader = 0;
      indiceChecksum = 0;
      codePaysInteger = 0;
      //checksum = 0;
      reinitialiserTableau();
      if(octetRecu == '$')
      {
        etatFSM = 1;
      }
      return 0;
      
    case(1):
    //recevoir le header
      if(octetRecu != '*')
      {
         headerCOM[indiceHeader] = octetRecu;
         indiceHeader++;
         if(indiceHeader > TAILLE_HEADER)
         {
           etatFSM = 0;
         }
      }else
      {
        //valider le header
        if(strcmp(headerCOM, "E03") != 0)
        {
          etatFSM = 0;
        }else
        {
          etatFSM = 2;
        }
      }       
      return 0;
      
    case(2):
    //recevoir le code du pays
     if(octetRecu != '*')
     {
         codePays[indiceCodePays] = octetRecu;
         indiceCodePays++;
         if(indiceCodePays > TAILLE_HEADER)
         {
           etatFSM = 0;
         }
      }else
      {
        //valider le code du pays
        codePaysInteger = (uint8_t)(atoi(codePays));
        if(codePaysInteger > 197 || codePaysInteger <= 0 )
        {
          etatFSM = 0;
        }else
        {
          etatFSM = 3;
        }
        
      }       
      return 0;
        
    case(3):
    //récupérer le checksum
      if(octetRecu != '\r')
      {
         checksumCOM[indiceChecksum] = octetRecu;
         indiceChecksum++;
         if(indiceChecksum > TAILLE_HEADER)
         {
           etatFSM = 0;
         }
         return 0;
      }else
      {
        etatFSM = 0;
        if(!calculerCheckSum())
        {
          return 0;
        }else
        {
          return codePaysInteger;
        }
      }
      
    default:
      etatFSM = 0;
      return 0;
  } 
  
}

int calculerCheckSum(void)
{
  checksum = 0;
   
  for(int i = 0;i<TAILLE_HEADER;i++)
  {
    checksum += (int)(headerCOM[i]);
  }
  
  checksum = checksum + '$' + '*' + '*' + (int)(atoi(checksumCOM)) + codePaysInteger;

  if((checksum%256) == 0)
  {
    return 1;
  }else
  {
    return 0;
  }
}
                                    
void reinitialiserTableau()
{
  for(int i = 0;i<TAILLE_HEADER;i++)
  {
    headerCOM[i] = '\0';
    codePays[i] = '\0';
    checksumCOM[i] = '\0';
  }
}
