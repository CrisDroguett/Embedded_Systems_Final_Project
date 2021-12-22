#include <MKL25Z4.h>
#include <systick.h>
#include <stdbool.h>

static volatile  _Bool systick_interrupt_has_occured =0;
#define SYSTICK_PERIOD ( 1000 ) 
#define SYSTICK_TOP ( SYS_CLOCK / SYSTICK_PERIOD )

void configure_systick()
{
	SysTick->LOAD = SYSTICK_TOP;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
			SysTick_CTRL_TICKINT_Msk   |
			SysTick_CTRL_ENABLE_Msk;
}
_Bool systick_has_fired()
{
	_Bool retval = systick_interrupt_has_occured;
	systick_interrupt_has_occured = 0;
	return retval;
}

void SysTick_Handler()
{
	systick_interrupt_has_occured = 1;
}
