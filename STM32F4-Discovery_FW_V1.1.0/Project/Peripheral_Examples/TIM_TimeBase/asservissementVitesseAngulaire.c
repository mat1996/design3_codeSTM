#include "asservissementVitesseAngulaire.h"

//0 => kp, 1 => ti , indice = numeroMoteur - 1
static double tableauParametrePI[NB_MOTEUR][NB_PARAM] = {{3.3944, 0.20493},{2.9435,0.14174},{3.1873 , 0.1865},{3.0224, 0.14173}};

//0 => e(n), 1 => e(n-1), indice = numeroMoteur - 1             //faire l'asservissement en vitesse linéaire!! vérifier les paramètres!! //{15.43, -13.68},{15.66,-13.4},{15.32 , -13.70},{15.68, -13.48}
static double tableauParametrePIDiscret[NB_MOTEUR][NB_PARAM] = {{15.03, -13.78},{15.66,-13.53},{15.92 , -13.70},{15.68, -13.48}}; 

//{15.58, -13.43},{15.66,-13.53},{15.55 , -13.70},{15.68, -13.48} //test
//tableaux => 0 : x(n), 1 : x(n-1)
//tableau de l'action posé
static double tableauActionPI[NB_MOTEUR][NB_PARAM];

//tableau de l'erreur
static double tableauErreurPI[NB_MOTEUR][NB_PARAM];

//tableau de la consigne de vitesse angulaire
static double tableauConsignePI[NB_MOTEUR][NB_PARAM];

void initPIVitesseAngulaire(void)
{
  for(int i=0;i<NB_MOTEUR;i++)
  {
    for(int j=0;j<NB_PARAM;j++)
    {
      tableauActionPI[i][j] = 0;
      tableauErreurPI[i][j] = 0;
      tableauConsignePI[i][j] = 0;
    }
  }
}

double calculerActionMoteur(uint16_t numeroMoteur, double vitesseCalcule)
{
  //calcule de l'erreur
   double actionMoteur;
   tableauErreurPI[numeroMoteur-1][1] = tableauErreurPI[numeroMoteur-1][0];
   tableauErreurPI[numeroMoteur-1][0] = tableauConsignePI[numeroMoteur - 1][0] - vitesseCalcule;
  
  //calcule de la nouvelle action
  tableauActionPI[numeroMoteur-1][1] = tableauActionPI[numeroMoteur-1][0];
  tableauActionPI[numeroMoteur-1][0] = tableauActionPI[numeroMoteur-1][1] + tableauParametrePIDiscret[numeroMoteur-1][0]*tableauErreurPI[numeroMoteur-1][0]
                                              + tableauParametrePIDiscret[numeroMoteur-1][1]*tableauErreurPI[numeroMoteur-1][1];
  if(tableauActionPI[numeroMoteur-1][0] < 0)
  {
    actionMoteur = 0;
  }else if(tableauActionPI[numeroMoteur-1][0] > 100)
  {
    actionMoteur = 100;
  }else
  {
    actionMoteur = tableauActionPI[numeroMoteur-1][0];
  }
    
  return actionMoteur;
}

void setConsigneVitesseAngulaire(uint16_t numeroMoteur, double consigne, uint16_t direction)
{
  tableauConsignePI[numeroMoteur-1][0] = consigne;
  tableauConsignePI[numeroMoteur-1][1] = (double)(direction);
}


double getConsigneVitesseAngulaire(uint16_t numeroMoteur)
{
  return tableauConsignePI[numeroMoteur-1][0];
}

uint16_t getConsigneDirectionVitesseAngulaire(uint16_t numeroMoteur)
{
  return (uint16_t)(tableauConsignePI[numeroMoteur-1][1]);
}
