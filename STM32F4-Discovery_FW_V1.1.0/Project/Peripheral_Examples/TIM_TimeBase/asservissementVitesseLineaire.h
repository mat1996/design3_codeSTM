#ifndef ASSERVISSEMENT2
#define ASSERVISSEMENT2

#include "stm32f4xx_gpio.h"

#define NB_GROUPE       2
#define NB_PARAM        2

void initPIVitesselineaire(void);
double calculerActionVitesseLineaire(uint16_t numeroGroupeMoteur, double vitesseCalcule);
void setConsigneVitesseLineaire(uint16_t numeroGroupeMoteur, double consigne);
double getConsigneVitesseLineaire(uint16_t numeroGroupeMoteur);
uint16_t getConsigneDirectionVitesseLineaire(uint16_t numeroGroupeMoteur);

#endif