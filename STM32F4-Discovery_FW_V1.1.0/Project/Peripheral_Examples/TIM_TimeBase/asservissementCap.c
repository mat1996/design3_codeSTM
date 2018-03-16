#include "asservissementCap.h"
#include "controleMoteur.h"
#include "asservissementVitesseAngulaire.h"
#include "main.h"

static double tableauParametrePIDiscretCap[NB_PARAM] = {0.1417, -0.1415}; //bon paramètres (15-03-2018)

//tableau de l'action posé
static double tableauActionPICap[NB_PARAM];

//tableau de l'erreur
static double tableauErreurPICap[NB_PARAM];

//tableau de la consigne du cap
static double tableauConsignePICap[NB_PARAM];

//tableau de la consigne progressive du cap
static double tableauConsigneProgressive[MAX_CONSIGNE_PROG];

uint16_t nbConsigne = 0;
uint16_t indiceConsigne = 0;
double angleDeplacement = 0;

void initPICap(void)
{

    for(int j =0;j<NB_PARAM;j++)
    {
      tableauActionPICap[j] = 0;
      tableauErreurPICap[j] = 0;
      tableauConsignePICap[j] = 0;
    }  
    
    for(int i =0;i<MAX_CONSIGNE_PROG;i++)
        tableauConsigneProgressive[i] = 0;
   
}

double calculerActionCap(double angleCalcule)
{ 
  //ajout du déplacement
  angleDeplacement += angleCalcule;
  
  //calcule de l'erreur
   double actionCap;
   tableauErreurPICap[1] = tableauErreurPICap[0];
   tableauErreurPICap[0] = tableauConsigneProgressive[indiceConsigne] - angleDeplacement;
  
  //calcule de la nouvelle action
  tableauActionPICap[1] = tableauActionPICap[0];
  tableauActionPICap[0] = tableauActionPICap[1] + tableauParametrePIDiscretCap[0]*tableauErreurPICap[0]
                                              + tableauParametrePIDiscretCap[1]*tableauErreurPICap[1];
  if(tableauActionPICap[0] < 0.1)
  {
    actionCap = 0;
    arreterMoteur(1);
    arreterMoteur(2);
    arreterMoteur(3);
    arreterMoteur(4);
    initPIVitesseAngulaire();
    //setTypeAsservissement(0);
  }else
  {
    actionCap = tableauActionPICap[0];
  }
  
  if(indiceConsigne < nbConsigne)
    indiceConsigne++;
  
  return actionCap;
  
}

//0 => robot tourne sens anti-horaire, 1 => robot tourne sens horaire
void setConsigneCap(double angle, uint16_t direction)
{
  initPICap();
  indiceConsigne = 0;
  angleDeplacement = 0.0;
  tableauConsignePICap[0] = angle;
  
  nbConsigne = (uint16_t)(angle/SAUT_CONSIGNE);
  
  for(int i =0;i<nbConsigne;i++)
  {
    tableauConsigneProgressive[i] = (double)(i)*(SAUT_CONSIGNE);
  }
  tableauConsigneProgressive[nbConsigne] = angle;
  tableauConsignePICap[1] = (double)(direction);
  
//  if(angle == 0.0)
//  {
//    arreterMoteur(1);
//    arreterMoteur(3);
//    arreterMoteur(2);
//    arreterMoteur(4);
//    initPIVitesselineaire();
//  }
}

double getConsigneCap(void)
{
  return tableauConsignePICap[0];
}

uint16_t getDirectionConsigneCap(void)
{
  return (uint16_t)(tableauConsignePICap[1]);
}

