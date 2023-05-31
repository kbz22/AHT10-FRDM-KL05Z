#include "MKL05Z4.h"
#include <stdio.h>
#include "uart.h"
#include "pit.h"
#include "AHT10.h"

static char str[100];

void PIT_IRQHandler()
{
	uint32_t read_t,read_h;
	double temp=0.0,hum=0.0;
		
	PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;		// Reset interrupt flag
	NVIC_ClearPendingIRQ(PIT_IRQn);
	
	AHT10_Init();	// Initialize measurement
	AHT10_GetRawReadings(&read_t,&read_h);
	AHT10_ConvertReadings(&temp,&hum,read_t,read_h);
	
	// I couldn't get sprintf to print doubles correctly, so here's a workaround
	// It only works for two decimal places
	int tmp=(int)((temp-(double)(int)(temp))*100);
	sprintf(str,"T=%d.%dC, RH=%d%%",(int)temp,tmp,(int)hum);
	
	UART_Println(str);	
}

int main (void){	
	
	PIT_Init();
	
	UART_Init(9600);
	UART_Println("\nBegin measurment...\n");
	
	while(1){
		__WFI(); // wait for interrupt
	}
}
