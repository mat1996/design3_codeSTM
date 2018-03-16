#include "controleDistanceCapRobot.h"
#include "main.h"

static double distanceCible[2];
static double vitesseMax;

double calculerConsigneVitesseLineaire(double distanceParcouru)
{
  distanceCible[0] -= distanceParcouru;
  if(distanceCible[0] < (double)(DISTANCE_CRITIQUE))
  {
    setTypeAsservissement(0);
    return 0.0;
  }else
  {
    double vitesse = ((double)(Kdistance)) * distanceCible[0];
    return vitesse > vitesseMax ? vitesseMax : vitesse;
  }
}

//0 => avancer, 1 => reculer, 2 => droite, 3 => gauche
void setDistanceCible(double distance, uint16_t direction, double vitesse)
{
  distanceCible[0] = distance;
  distanceCible[1] = (double)(direction);
  vitesseMax = vitesse;
}

double getDistanceCible(void)
{
  return distanceCible[0];
}

uint16_t getDirectionDistanceCible(void)
{
  return (uint16_t)(distanceCible[1]);
}

