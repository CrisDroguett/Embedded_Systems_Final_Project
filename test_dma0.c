// Cris Droguett
// Embedded Systems Final Project

#include <stdint.h>
#include "systick.h"
#include "copwdt.h"
#include "dma0.h"
#include "sw1.h"
#include "RGBLED_PWM.h"

#define NUM_COLORS 32
int16_t colors[NUM_COLORS]={
	0x0357, 0x8ACF, 0x0F0F, 0x0F0F,
	0x3FFF, 0x7FFF, 0x0000, 0xFFFF,
	0xAABB, 0x5566, 0xFB85, 0x4321,
	0x0307, 0x0A0F, 0x2222, 0x7777,
	0xF000, 0x000F, 0xFF00, 0xFF00,
	0x1234, 0x5678, 0x9ABC, 0xDEF0,
	0x9382, 0xFAEB, 0x620F, 0x0690,
	0x0102, 0x0304, 0x0506, 0x0708
};

volatile uint32_t rg_color;

void dma0_fsm(){
	static uint16_t cntr = 0;

	uint16_t color1 = rg_color>>28;
	uint16_t color2 = rg_color>>24;
	uint16_t color3 = rg_color>>20;
	uint16_t color4 = rg_color>>16;
	uint16_t color5 = rg_color>>12;
	uint16_t color6 = rg_color>>8;
	uint16_t color7 = rg_color>>4;
	uint16_t color8 = rg_color;

	color1 &= 0x000F;
	color2 &= 0x000F;
	color3 &= 0x000F;
	color4 &= 0x000F;
	color5 &= 0x000F;
	color6 &= 0x000F;
	color7 &= 0x000F;
	color8 &= 0x000F;

	color1 = (color1 << 12);
	color2 = (color2 << 12);
	color3 = (color3 << 12);
	color4 = (color4 << 12);
	color5 = (color5 << 12);
	color6 = (color6 << 12);
	color7 = (color7 << 12);
	color8 = (color8 << 12);

	static enum {ST_COLOR1, ST_COLOR2, ST_COLOR3,ST_COLOR4, ST_COLOR5,ST_COLOR6, ST_COLOR7, ST_COLOR8} state =ST_COLOR1;
	//counter FSM
	cntr ++;
	if(cntr > 8000)
		cntr = 1;
	//LED FSM
	switch(state)
	{
		default:
		case ST_COLOR1:
			if(cntr >= 1000)
				state = ST_COLOR2;
			set_rgb_color_to(color1,0,0);
			break;
		case ST_COLOR2 :
			if(cntr >= 2000)
				state = ST_COLOR3;
			set_rgb_color_to(color2,0,0);
			break;
		case ST_COLOR3:
			if (cntr >= 3000)
				state = ST_COLOR4;
			set_rgb_color_to(color3,0, 0);
			break;
		case ST_COLOR4:
			if (cntr >= 4000)
				state = ST_COLOR5;
			set_rgb_color_to(color4,0, 0);
			break;
		case ST_COLOR5:
			if(cntr >= 5000)
				state = ST_COLOR6;
			set_rgb_color_to(color5,0,0);
			break;
		case ST_COLOR6 :
			if(cntr >= 6000)
				state = ST_COLOR7;
			set_rgb_color_to(color6,0,0);
			break;
		case ST_COLOR7:
			if (cntr >= 7000)
				state = ST_COLOR8;
			set_rgb_color_to(color7,0, 0);
			break;
		case ST_COLOR8:
			if (cntr >= 8000)
				state = ST_COLOR1;
			set_rgb_color_to(color8,0, 0);
			break;
	}
}

void main()
{
	asm("CPSID i");
	configure_systick();
	configure_copwdt();
	configure_rgbled();
	configure_sw1();
	configure_dma0(colors, &rg_color, NUM_COLORS);
	asm("CPSIE i");
	
	while(1)
	{
		asm("WFI");
		if(!systick_has_fired())
			continue;
		dma0_fsm();
		feed_the_watchdog();
	}
}

