/*
 * leds.c
 *
 *  Author:  David Pye
 *  Contact: davidmpye@gmail.com
 *  Licence: GNU GPL v3 or later
 */ 
#include "leds.h"

//speed of LED sequence
#define LED_SEQ_TIME 10

#define NUM_LEDS 6
uint16_t leds[] = { LED_FILTER, LED_BLOCKED, LED_ERR, LED_BAT_LO, LED_BAT_MED, LED_BAT_HI };
	
void leds_init() {
	//Set up the LED pins as IO
	struct port_config led_port_config;
	port_get_config_defaults(&led_port_config);
	led_port_config.direction = PORT_PIN_DIR_OUTPUT;
	for (int i=0; i<NUM_LEDS; ++i) {
		port_pin_set_config(leds[i], &led_port_config);
	}
}

void leds_sequence() {	
	for (int i=0; i<NUM_LEDS; ++i) {
		port_pin_set_output_level(leds[i], true);
		delay_ms(LED_SEQ_TIME);
	}
	for (int i = NUM_LEDS-1; i>=0; --i) {
		port_pin_set_output_level(leds[i], false);
		delay_ms(LED_SEQ_TIME);
	}
}

void leds_off() {
	for (int i=0; i<NUM_LEDS; ++i) {
		port_pin_set_output_level(leds[i], false);
	}
}

void leds_on() {
	for (int i=0; i<NUM_LEDS; ++i) {
		port_pin_set_output_level(leds[i], true);
	}
}

void leds_display_battery_soc(int percent_soc) {
	//LEDs off to start
	port_pin_set_output_level(LED_BAT_LO, false );
	port_pin_set_output_level(LED_BAT_MED, false );
	port_pin_set_output_level(LED_BAT_HI, false );

	//Three LEDs to indicate SoC, so 0-35, 35-70, 70-100.
	//Voltage thresholds:   
	port_pin_set_output_level(LED_BAT_LO, true );
	if (percent_soc > 35) {
		port_pin_set_output_level(LED_BAT_MED, true );
	}
	if (percent_soc>70) {
		port_pin_set_output_level(LED_BAT_HI, true );
	}
}

void leds_flash_charging_segment(int percent_soc) {
	if (percent_soc <35) {	
		//Flash lo
		port_pin_set_output_level(LED_BAT_LO, true );
		delay_ms(500);
		port_pin_set_output_level(LED_BAT_LO, false );
		delay_ms(500);
	}
	else if (percent_soc<70) {
		//Low on, flash med
		port_pin_set_output_level(LED_BAT_LO, true );

		port_pin_set_output_level(LED_BAT_MED, true );
		delay_ms(500);
		port_pin_set_output_level(LED_BAT_MED, false );
		delay_ms(500);
	}
	else {
		//Low + med on, flash hi
		port_pin_set_output_level(LED_BAT_LO, true );
		port_pin_set_output_level(LED_BAT_MED, true );
		port_pin_set_output_level(LED_BAT_HI, true );
		delay_ms(500);
		port_pin_set_output_level(LED_BAT_HI, false );
		delay_ms(500);
	}
}

void leds_blink_error_led(int ms) {
		port_pin_set_output_level(LED_ERR, true );
		delay_ms(ms/2);
		port_pin_set_output_level(LED_ERR, false );
		delay_ms(ms/2);
}

void leds_show_pack_flat() {
	for (int i=0; i<5; ++i) {
		port_pin_set_output_level(LED_BAT_LO, true );
		delay_ms(100);
		port_pin_set_output_level(LED_BAT_LO, false );
		delay_ms(100);
	}
}

void leds_show_filter_err_status(bool status) {
	port_pin_set_output_level(LED_FILTER, status );
}

void leds_show_blocked_err_status(bool status) {
	port_pin_set_output_level(LED_BLOCKED, status);	
}