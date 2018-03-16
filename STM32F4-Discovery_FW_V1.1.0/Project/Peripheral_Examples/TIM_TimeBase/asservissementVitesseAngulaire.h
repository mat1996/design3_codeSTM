#ifndef ASSERVISSEMENT
#define ASSERVISSEMENT

#include "stm32f4xx_gpio.h"

#define NB_MOTEUR       4
#define NB_PARAM        2


void initPIVitesseAngulaire(void);
double calculerActionMoteur(uint16_t numeroMoteur, double vitesseCalcule); 
void setConsigneVitesseAngulaire(uint16_t numeroMoteur, double consigne, uint16_t direction);
double getConsigneVitesseAngulaire(uint16_t numeroMoteur);
uint16_t getConsigneDirectionVitesseAngulaire(uint16_t numeroMoteur);

#endif