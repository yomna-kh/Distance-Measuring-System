
#include "ultrasonic.h"
#include "Icu.h"
#include "gpio.h"
#include <util/delay.h> /* For the delay functions */


uint16 g_timeHigh;
uint16 g_edgeCount;

void ULTRASONIC_init(void)
{
	/* Create configuration structure for Icu driver */
	Icu_ConfigType Icu_Config = {F_CPU_8,RISING};

	/* Initialize the Icu driver */
	Icu_init(&Icu_Config);


	/* Set the Call back function pointer in the Icu driver */
	Icu_setCallBack(ULTRASONIC_edgeProcessing);

	/* Setup the direction of the trigger pin as output pin*/
	GPIO_setupPinDirection(PORT_TRIG_ID,PIN_TRIG_ID,PIN_OUTPUT);
}

void ULTRASONIC_trigger(void)
{
    /* write pulse 10us to the trigger pin manually*/
	GPIO_writePin(PORT_TRIG_ID,PIN_TRIG_ID,LOGIC_HIGH);
	_delay_us(10);
	GPIO_writePin(PORT_TRIG_ID,PIN_TRIG_ID,LOGIC_LOW);
}

uint16 ULTRASONIC_readDistance(void)
{
	uint16 distance=0;
	/*start the Icu from this moment*/
	ULTRASONIC_init();
	/*send the trigger pulse*/
	ULTRASONIC_trigger();

	if (g_edgeCount == 2)
	{
		g_edgeCount = 0;
		/*calculate the distance*/
		distance = ((g_timeHigh/58.8)+2);
	}
	return distance;
}

void ULTRASONIC_edgeProcessing(void)
{
	g_edgeCount++;
	if(g_edgeCount == 1)
	{
		/*
		 * Clear the timer counter register to start measurements from the
		 * first detected rising edge
		 */
		Icu_clearTimerValue();
		/* Detect falling edge */
		Icu_setEdgeDetectionType(FALLING);
	}
	else if(g_edgeCount == 2)
	{
		/* Store the High time value */
		g_timeHigh = Icu_getInputCaptureValue();
		/* stop the Icu */
		Icu_DeInit();
	}
}
