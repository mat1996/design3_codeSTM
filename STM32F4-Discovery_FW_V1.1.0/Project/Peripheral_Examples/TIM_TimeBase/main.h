#ifndef __MAIN_H
#define __MAIN_H



void TimingDelay_Decrement(void);
int getTypeAsservissement(void);
void setTypeAsservissement(uint16_t type);
void TIM_Config(void);
void Delay(__IO uint32_t nTime);
void envoyerOctets(const char* envoie, int tailleEnvoie);
void appliquerConsigneDeplacement(void);
void appliquerConsigneRotation(void);
void envoyerFinDeplacement(void);
void envoyerFinRotation(void);

#endif 