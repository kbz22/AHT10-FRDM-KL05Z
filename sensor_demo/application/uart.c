#include "uart.h"

void uart_send(uint8_t data);

void UART_Init (uint32_t baud_rate) {
	
	uint32_t divisor, osr;
	
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
  
  PORTB->PCR[1] = PORT_PCR_MUX(2); 
  PORTB->PCR[2] = PORT_PCR_MUX(2);   
	
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);
	
	UART0->C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK);
	
	osr = 32;
	UART0->C4 |= UART0_C4_OSR(osr-1);
	
	baud_rate = 9600;
	divisor = SystemCoreClock / (osr * baud_rate);
	UART0->BDH = UART0_BDH_SBR(divisor >> 8);
	UART0->BDL = UART0_BDL_SBR(divisor);
	
	UART0->BDH |= UART0_BDH_SBNS_MASK;

	UART0->C2 = UART0_C2_TE_MASK | UART0_C2_RE_MASK;
}

void UART_Print(const char *str) {
	
	uint8_t str_len = 0;

  while (str[str_len] != '\0') {
    uart_send(str[str_len]);
    ++str_len;
  }
}

void UART_Println(const char *str) {
	
	UART_Print(str);
	UART_Print("\n\r");
}

void uart_send(uint8_t data) {
	while(!(UART0->S1 & UART0_S1_TDRE_MASK));
	UART0->D = data;
}
