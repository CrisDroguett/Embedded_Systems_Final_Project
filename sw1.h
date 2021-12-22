#ifndef SW1_H
#define SW1_H

#include <stdbool.h>

void configure_sw1();
_Bool sw1_is_pressed();
_Bool sw1_is_not_pressed();
enum press_type {NO_PRESS,SHORT_PRESS,LONG_PRESS};
enum press_type switch_press_duration();
#endif
