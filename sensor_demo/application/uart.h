#ifndef UART_H
#define UART_H

#include "frdm_bsp.h"

void UART_Init(uint32_t baud_rate);
void UART_Println(const char *str);

#endif
