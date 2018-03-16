#include "FSM_USART_commande.h"
#include <stdlib.h>

uint16_t etatFSMCommande = 0;

char distanceDeplacementChar[TAILLE_MAX_PAYLOAD];
char directionDeplacementChar[TAILLE_MAX_PAYLOAD];
char vitesseLineaireMaxChar[TAILLE_MAX_PAYLOAD];
char checksumChar[TAILLE_MAX_PAYLOAD];
char angleRotationChar[TAILLE_MAX_PAYLOAD];
char directionRotationChar[TAILLE_MAX_PAYLOAD];
char commande;

static double distanceDeplacement = 0.0;
static uint16_t directionDeplacement = 0;
static double vitesseLineaireMax = 0.0;

static double angleRotation = 0.0;
static uint16_t directionRotation = 0;

int indiceTableau = 0;

//0 => attente
//1 => recevoir commande move
//2 => recevoir commande rotation
//3 => recevoir comamnde drapeau

//retourner 1 => prêt à donner la commande de déplacement
//retourner 2 => prêt à donner la commande de rotation
//retourner 3 => prêt à donner la commande de capter le code du drapeau

int recevoirOctetCMD(char octetRecu)
{
  switch(etatFSMCommande)
  {
  case 0 :
    indiceTableau = 0;
    reinitialiserTableauCMD();
    if(octetRecu == 'M')
    {
      //commande de déplacement
       commande = 'M';
       etatFSMCommande = 1;
    }else if (octetRecu == 'R')
    {
      //commande de rotation
       commande = 'R';
       etatFSMCommande = 2;
    }else if (octetRecu == 'D')
    {
      //commande de drapeau
       commande = 'D';
       etatFSMCommande = 3;
    }else
    {
      //commande non-reconnu
      commande = '\0';
      etatFSMCommande = 0;
    }
    return 0;
    
    
  case 1 :
    if(octetRecu == '*')
    {
      etatFSMCommande = 4;
    }else
    {
      etatFSMCommande = 0;
    }
    return 0;
    
  case 2 :
    if(octetRecu == '*')
    {
      etatFSMCommande = 7;
    }else
    {
      etatFSMCommande = 0;
    }
    return 0;
    
  case 3 :
    etatFSMCommande = 0;
    if(octetRecu == '*')
    {
      return 3;
    }else
    {
      return 0;
    }
    
  case 4 :
    //recevoir la distance du déplacement
    if(octetRecu != '*')
    {
      distanceDeplacementChar[indiceTableau] = octetRecu;
      indiceTableau++;
      if(indiceTableau > TAILLE_MAX_PAYLOAD)
        etatFSMCommande = 0;
    }else
    {
      indiceTableau = 0;
      etatFSMCommande = 5;
    }
    return 0;
    
  case 5:
    //recevoir la direction du déplacement
    if(octetRecu != '*')
    {
      directionDeplacementChar[indiceTableau] = octetRecu;
      indiceTableau++;
      if(indiceTableau > TAILLE_MAX_PAYLOAD)
        etatFSMCommande = 0;
    }else
    {
      indiceTableau = 0;
      etatFSMCommande = 6;
    }
    return 0;
    
  case 6 :
    //recevoir la vitesse linéaire max
    if(octetRecu != '*')
    {
      vitesseLineaireMaxChar[indiceTableau] = octetRecu;
      indiceTableau++;
      if(indiceTableau > TAILLE_MAX_PAYLOAD)
        etatFSMCommande = 0;
      
      return 0;
    }else
    {
      indiceTableau = 0;
      etatFSMCommande = 0;
      stockerDonneeRecu();
      if(commande == 'M')
        return 1;
      else if(commande == 'R')
        return 2;
      else if(commande == 'D')
        return 3;
    }
    
  case 7 :
    //recevoir l'angle
    if(octetRecu != '*')
    {
      angleRotationChar[indiceTableau] = octetRecu;
      indiceTableau++;
      if(indiceTableau > TAILLE_MAX_PAYLOAD)
        etatFSMCommande = 0;
    }else
    {
      indiceTableau = 0;
      etatFSMCommande = 8;
    }
    return 0;
    
  case 8 :
    //recevoir la direction
    if(octetRecu != '*')
    {
      directionRotationChar[indiceTableau] = octetRecu;
      indiceTableau++;
      if(indiceTableau > TAILLE_MAX_PAYLOAD)
        etatFSMCommande = 0;
    }else
    {
      indiceTableau = 0;
      etatFSMCommande = 0;
      stockerDonneeRecu();
      if(commande == 'M')
        return 1;
      else if(commande == 'R')
        return 2;
      else if(commande == 'D')
        return 3;
    }
    return 0;
    
    
    
    
  default : 
    etatFSMCommande = 0;
    return 0; 
    
  }
}


void stockerDonneeRecu(void)
{
  angleRotation = atof(angleRotationChar);
  distanceDeplacement = atof(distanceDeplacementChar);
  directionDeplacement = atoi(directionDeplacementChar);
  vitesseLineaireMax = atof(vitesseLineaireMaxChar);
  directionRotation = atoi(directionRotationChar);
}

double getDistanceDeplacement(void)
{
  return distanceDeplacement;
}

uint16_t getDirectionDeplacement(void)
{
  return directionDeplacement;
}

double getVitesseLineaireMax(void)
{
  return vitesseLineaireMax;
}

double getAngleRotation(void)
{
  return angleRotation;
}

uint16_t getDirectionRotation(void)
{
  return directionRotation;
}

void reinitialiserTableauCMD(void)
{
  for(int i = 0;i<TAILLE_MAX_PAYLOAD;i++)
  {
    distanceDeplacementChar[i] = '\0';
    directionDeplacementChar[i] = '\0';
    vitesseLineaireMaxChar[i] = '\0';
    angleRotationChar[i] = '\0';
    checksumChar[i] = '\0';
  }
}




