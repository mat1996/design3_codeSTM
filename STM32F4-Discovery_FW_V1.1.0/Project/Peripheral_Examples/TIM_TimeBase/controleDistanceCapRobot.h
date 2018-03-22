#ifndef CTRLDCROBOT
#define CTRLDCROBOT

#include "stm32f4xx_gpio.h"

#define Kdistance               2
#define DISTANCE_CRITIQUE       0.01 // en m
#define VMAX                    0.15
#define VROTATION               0.05


double calculerConsigneVitesseLineaire(double distanceParcouru);
void setDistanceCible(double distance, uint16_t direction, double vitesse);
double getDistanceCible(void);
uint16_t getDirectionDistanceCible(void);

#endif