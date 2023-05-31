#include "MKL05Z4.h"
#include	"pit.h"

void PIT_Init(void)
{
	uint32_t tsv;
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;			// Enable clock signal for PIT
	PIT->MCR &= ~PIT_MCR_MDIS_MASK;				// Enable PIT
	tsv=BUS_CLOCK;												// Interrupt every 1s
	PIT->CHANNEL[0].LDVAL = PIT_LDVAL_TSV(tsv);		// load the start value
	PIT->CHANNEL[0].TCTRL = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK;		// unlock interrupt and enable timer
	NVIC_ClearPendingIRQ(PIT_IRQn);
	NVIC_EnableIRQ(PIT_IRQn);	
}
