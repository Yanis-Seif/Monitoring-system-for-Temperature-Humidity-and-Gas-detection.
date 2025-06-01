#ifndef LEDS_H
#define LEDS_H

void leds_init(void);
void leds_set(unsigned char temperature, unsigned char gasDetected);
void leds_off(void);
void buzzer_on(void);
void buzzer_off(void);

#endif
