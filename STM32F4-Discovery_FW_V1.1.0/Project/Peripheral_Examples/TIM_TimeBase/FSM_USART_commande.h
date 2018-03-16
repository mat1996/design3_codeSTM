#ifndef FSMCOMMANDE
#define FSMCOMMANDE

#include "stm32f4xx_gpio.h"

#define TAILLE_MAX_PAYLOAD      10

int recevoirOctetCMD(char octetRecu);
void stockerDonneeRecu(void);
double getDistanceDeplacement(void);
uint16_t getDirectionDeplacement(void);
double getVitesseLineaireMax(void);
double getAngleRotation(void);
uint16_t getDirectionRotation(void);
void reinitialiserTableauCMD(void);

#endif