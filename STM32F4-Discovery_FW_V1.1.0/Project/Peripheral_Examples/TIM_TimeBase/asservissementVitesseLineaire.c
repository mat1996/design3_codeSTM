#include "asservissementVitesseLineaire.h"

static double tableauParametrePIDiscretVL[NB_GROUPE][NB_PARAM] = {{10.04, -8.57}, {10.33 -8.77}};

//tableaux => 0 : x(n), 1 : x(n-1)
//groupe 1 => moteur 1 et 3, groupe 2 => moteur 2 et 4

//tableau de l'action posé
static double tableauActionPIVL[NB_GROUPE][NB_PARAM];

//tableau de l'erreur
static double tableauErreurPIVL[NB_GROUPE][NB_PARAM];

//tableau de la consigne de vitesse angulaire
static double tableauConsignePIVL[NB_GROUPE][NB_PARAM];

void initPIVitesselineaire(void)
{
  for(int i =0;i<NB_GROUPE;i++)
  {
    for(int j =0;j<NB_PARAM;j++)
    {
      tableauActionPIVL[i][j] = 0;
      tableauErreurPIVL[i][j] = 0;
      tableauConsignePIVL[i][j] = 0;
    }  
  }  
}

double calculerActionVitesseLineaire(uint16_t numeroGroupeMoteur, double vitesseCalcule)
{
  //calcule de l'erreur
   double actionVitesseLineaire;
   tableauErreurPIVL[numeroGroupeMoteur-1][1] = tableauErreurPIVL[numeroGroupeMoteur-1][0];
   tableauErreurPIVL[numeroGroupeMoteur-1][0] = tableauConsignePIVL[numeroGroupeMoteur - 1][0] - vitesseCalcule;
  
  //calcule de la nouvelle action
  tableauActionPIVL[numeroGroupeMoteur-1][1] = tableauActionPIVL[numeroGroupeMoteur-1][0];
  tableauActionPIVL[numeroGroupeMoteur-1][0] = tableauActionPIVL[numeroGroupeMoteur-1][1] + tableauParametrePIDiscretVL[numeroGroupeMoteur-1][0]*tableauErreurPIVL[numeroGroupeMoteur-1][0]
                                              + tableauParametrePIDiscretVL[numeroGroupeMoteur-1][1]*tableauErreurPIVL[numeroGroupeMoteur-1][1];
  if(tableauActionPIVL[numeroGroupeMoteur-1][0] < 0)
  {
    actionVitesseLineaire = 0;
  }else
  {
    actionVitesseLineaire = tableauActionPIVL[numeroGroupeMoteur-1][0];
  }
    
  return actionVitesseLineaire;
}

void setConsigneVitesseLineaire(uint16_t numeroGroupeMoteur, double consigne)
{
  //Groupe 1 : 0 => avancer (positif), 1 => reculer (négatif)
  //Groupe 2 : 0 => droite (positif), 1 => gauche (négatif)
  tableauConsignePIVL[numeroGroupeMoteur-1][0] = consigne < 0 ? ((double)(-1))*consigne : consigne;
  tableauConsignePIVL[numeroGroupeMoteur-1][1] = consigne < 0 ? 1 : 0;
  
  if(consigne == 0.0)
  {
    if(numeroGroupeMoteur == 0)
    {
      arreterMoteur(1);
      arreterMoteur(3);
    }else
    {
      arreterMoteur(2);
      arreterMoteur(4);
    }
    initPIVitesselineaire();
  }
}

double getConsigneVitesseLineaire(uint16_t numeroGroupeMoteur)
{
  return (double)tableauConsignePIVL[numeroGroupeMoteur - 1][0];
}

uint16_t getConsigneDirectionVitesseLineaire(uint16_t numeroGroupeMoteur)
{
  return (uint16_t)(tableauConsignePIVL[numeroGroupeMoteur - 1 ][1]);
}