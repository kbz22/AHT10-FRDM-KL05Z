#include "AHT10.h"

static uint16_t timeout;

void AHT10_Init(){	
	
	I2C_Init();
	I2C_Write(AHT_ADD, CMD_INIT);
	mdelay(500);
	
}

void AHT10_GetRawReadings(uint32_t *t_data, uint32_t *h_data){

	uint8_t data[6]={0,0,0,0,0,0};
	
	I2C_Write(AHT_ADD, CMD_POM); 				// send measurement command
	mdelay(80);													// wait until the measurement is done
	I2C_Read(AHT_ADD,0,6,data);					// get the data
		
	// data from the sensor is sent on 5 bytes: 12 bits for tempreture and 12 for humidity
	// this splits them
	*h_data=(uint32_t)( data[1] << 12)  |
			  (uint32_t)( data[2] << 4)	 |
		    (uint32_t)((data[3] & 0xF0) >> 4);
	*t_data=(uint32_t)((data[3] & 0x0F) << 16 )|
			  (uint32_t)( data[4] << 8)	|
		    (uint32_t)  data[5];
	
}

void AHT10_ConvertReadings(double *temp_C,double *hum_P,uint32_t temp_raw,uint32_t wilg_raw){
	
	*temp_C=(double)temp_raw*0.000191-50;
	*hum_P=(double)wilg_raw*0.000095;
	
}

void I2C_Init(){
	
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK ;		// clock for I2C0 
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;		// clock for PORTB 
	
	PORTB->PCR[3] |= PORT_PCR_MUX(2); 	// I2C0 SCL	(PTB3)
	PORTB->PCR[4] |= PORT_PCR_MUX(2); 	// I2C0 SDA	(PTB4)
	
	I2C0->C1 &= ~(I2C_C1_IICEN_MASK);			/* disable module during modyfications*/	

	I2C0->F  |= I2C_F_MULT(0); // multiplexer
	I2C0->F  |= I2C_F_ICR(0x19);	// SCLdivider = Table 36-28. I2C divider and hold values. Reference Manual p.622

	I2C0->C1 |= I2C_C1_IICIE_MASK; // enable interrupts
	
}

void I2C_Wait() {
	
  timeout = 0;
	
  while (((I2C0->S & I2C_S_IICIF_MASK)==0) && (timeout<10000)) timeout++;
	I2C0->S |= I2C_S_IICIF_MASK;
	
}

void I2C_Write(uint8_t address, uint8_t data) {

	I2C0->C1 |= I2C_C1_IICEN_MASK; // enable I2C
	I2C0->C1 |= I2C_C1_TX_MASK;	// set to transmit mode
	I2C0->C1 |= I2C_C1_MST_MASK; // send start
	I2C0->D = (uint8_t)(address << 1); // send write address
	I2C_Wait();	// wait for ack from slave
	I2C0->D = data;	// send data
	I2C_Wait();
	I2C0->C1 &= ~I2C_C1_MST_MASK;	// clear start mask
	I2C0->C1 &= ~I2C_C1_IICEN_MASK;
	
}

void I2C_Read(uint8_t address, uint8_t reg, uint8_t size, uint8_t* data) {
	
	uint8_t dummy = 0;
	uint8_t cnt = 0;
	
	I2C0->C1 |= I2C_C1_IICEN_MASK; // enable I2C
	I2C0->C1 |= I2C_C1_TX_MASK; // set to transmit mode
	I2C0->C1 |= I2C_C1_MST_MASK; // send start
	I2C0->D = (uint8_t)(address << 1); // send write address
	I2C_Wait();	// wait for ack from slave
	I2C0->D = reg; //select register
	I2C_Wait();	
	
	// restart
	uint8_t tempF = I2C0->F;
	I2C0->F   = 0;
  I2C0->C1 |= I2C_C1_RSTA_MASK;
	I2C0->F   = tempF;	

	I2C0->D = (uint8_t)(address << 1) | 0x01; // send read address
	I2C_Wait();
	
	I2C0->C1 &= ~I2C_C1_TX_MASK; // set to receive mode 
	
	while(cnt < (size-1)) {
		
		I2C0->C1 &= ~I2C_C1_TXAK_MASK; // transmit acknowledge bit
		dummy = I2C0->D; // read data
		I2C_Wait();	
		data[cnt++] = I2C0->D;
	}
	
	I2C0->C1 |= I2C_C1_TXAK_MASK; // no acknowledge bit 
	dummy = I2C0->D; // read data
	I2C_Wait();
	I2C0->C1 &= ~I2C_C1_MST_MASK; // set start mask off
	data[cnt] = I2C0->D;

	I2C0->C1 &= ~I2C_C1_IICEN_MASK;
	
}
