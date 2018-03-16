#ifndef FSM_USART_CP
#define FSM_USART_CP

#include "stm32f4_discovery.h"


#define TAILLE_HEADER   4

int recevoirOctetDrapeau(char octetRecu);
int calculerCheckSum(void);
void reinitialiserTableau(void);

#endif