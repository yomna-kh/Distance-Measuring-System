/*
 * main.c
 *
 *  Created on: 15 Oct 2022
 *      Author: world
 */

#include "gpio.h"
#include "common_macros.h" /* To use the macros like SET_BIT */
#include "avr/io.h" /* To use the IO Ports Registers */
#include "icu.h"
#include "ultrasonic.h"
#include <util/delay.h>
#include "lcd.h"


int main()
{
	uint16 distance;
	LCD_init();
	SREG = (1<<7);
	ULTRASONIC_init();
	LCD_displayString("Distance=    cm");
	 while(1)
	 {
		 distance = ULTRASONIC_readDistance();
		 LCD_moveCursor(0,10);
		 LCD_intgerToString(distance);
		 _delay_ms(500);

	 }
}

