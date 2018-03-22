#ifndef ASSCAP
#define ASSCAP

#include "stm32f4xx_gpio.h"

#define NB_PARAM                2
#define MAX_CONSIGNE_PROG       180
#define SAUT_CONSIGNE           1.0
#define ANGLE_CRITIQUE          0.5 //voir si c'est correct

void initPICap(void);
double calculerActionCap(double angleCalcule);
void setConsigneCap(double angle, uint16_t direction);
double getConsigneCap(void);
uint16_t getDirectionConsigneCap(void);

#endif