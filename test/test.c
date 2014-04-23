#ifndef F_CPU
# define F_CPU 2000000UL // 2 MHz
#endif

#ifndef F_CPU_CODE
# define F_CPU_CODE 0x03 // 2 MHz
#endif

#include <avr/pgmspace.h>
#include <util/delay.h>

#define LED_ON()        (PORTD |= (1<<6))
#define LED_OFF()       (PORTD &= ~(1<<6))
#define LED_CONFIG()    (DDRD |= (1<<6))
#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))
#define DELAY_MS        1000

int main(void)
{
	// Set CPU frequency and make sure the LED is off
	CPU_PRESCALE (F_CPU_CODE);
	LED_CONFIG ();
	LED_OFF ();

	while (1) {
		LED_ON ();
		_delay_ms (DELAY_MS);
		LED_OFF ();
		_delay_ms (DELAY_MS);
	}
}
