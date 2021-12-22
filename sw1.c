// Cris Droguett
// Embedded Systems Final Project
#include <sw1.h>
#include <MKL25Z4.h>
#include <stdbool.h>
#define SW1_LOCATION 20

void configure_sw1()
{
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
	PORTA->PCR[SW1_LOCATION] =
		PORT_PCR_PE(1)|
		PORT_PCR_PS(1)|
		PORT_PCR_PFE(0)|
		PORT_PCR_DSE(0)|
		PORT_PCR_SRE(0)|
		PORT_PCR_MUX(1)|
		PORT_PCR_IRQC(1)|
		PORT_PCR_ISF(1);
	PTA->PDDR &= ~(1<<SW1_LOCATION);
}

_Bool sw1_is_pressed(){
	return !(PTA->PDIR & (1<<SW1_LOCATION));
}

_Bool sw1_is_not_pressed(){
	return !sw1_is_pressed();
}
//enum press_type {NO_PRESS,SHORT_PRESS,LONG_PRESS};
enum press_type switch_press_duration(){
	static uint16_t cntr = 0;
	static enum {ST_NO_PRESS,ST_DEBOUNCE_PRESS,ST_SHORT_PRESS,ST_DEBOUNCE_RELEASE1,ST_LONG_PRESS,ST_DEBOUNCE_RELEASE2} state = ST_NO_PRESS;
	switch(state) {
		default:
		case ST_NO_PRESS:
			cntr = 0;
			if(sw1_is_pressed())
			{
				state = ST_SHORT_PRESS;
				cntr = 1;
			}
			else
				state = ST_NO_PRESS;
			break;
		case ST_DEBOUNCE_PRESS:
			if(sw1_is_pressed() && (cntr>=5))
			       state = ST_SHORT_PRESS;
			else if(sw1_is_pressed() && (cntr<5))
				state = ST_DEBOUNCE_PRESS;
			else if(sw1_is_not_pressed())
				state = ST_NO_PRESS;	
			cntr++;
			break;
		case ST_SHORT_PRESS:
			cntr ++;
			if(sw1_is_pressed() && (cntr >= 1499))
				state = ST_LONG_PRESS;
			else if(sw1_is_pressed() && (cntr < 1499))
				state = ST_SHORT_PRESS;
			else if(sw1_is_not_pressed())
			{
				cntr = 0;
				state = ST_DEBOUNCE_RELEASE1;
				return SHORT_PRESS;
			}
			break;
		case ST_LONG_PRESS:
			cntr = 0;
			if(sw1_is_not_pressed())
			{
				state = ST_DEBOUNCE_RELEASE2;
				return LONG_PRESS;
			}
			break;
		case ST_DEBOUNCE_RELEASE1:
			cntr++;
			if(sw1_is_not_pressed() && cntr >= 5)
				state = ST_NO_PRESS;
			else
				state = ST_DEBOUNCE_RELEASE1;
			break;
		case ST_DEBOUNCE_RELEASE2:
			cntr++;
			if(sw1_is_not_pressed() && cntr >= 5)
				state = ST_NO_PRESS;
			else
				state = ST_DEBOUNCE_RELEASE2;
			break;
	}
		return NO_PRESS;
} 
