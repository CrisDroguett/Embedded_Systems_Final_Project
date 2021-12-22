// Cris Droguett
// Embedded Systems 
// Drivers
#ifndef REDLED_PWM_H
#define REDLED_PWM_H
#include <stdint.h>

void set_red_led_duty_cycle(uint16_t duty);
void set_green_led_duty_cycle(uint16_t duty);
void set_blue_led_duty_cycle(uint16_t duty);
void configure_rgbled();
void set_rgb_color_to( uint16_t red, uint16_t green, uint16_t blue);
void turn_off_rgbled();

#endif

