#ifndef FICHIER_H
#define FICHIER_H

#include "stm32f4_discovery.h"

#define instruction_read 3;
#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)

static const int nbOctetMax = 16384;

static __IO uint32_t TimingDelay;
void TimingDelay_Decrement(void);

extern int LireMemoireEEPROM(uint16_t AdresseEEPROM, uint32_t NbreOctet, uint8_t *Destination);
extern int EcrireMemoireEEPROM(uint16_t AdresseEEPROM, uint32_t NbreOctet, uint8_t *Source);
extern void initModule(void);

#endif