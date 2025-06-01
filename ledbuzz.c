#include <xc.h>
#include "ledbuzz.h"

#define BLUE_LED PORTCbits.RC0
#define YELLOW_LED PORTCbits.RC1
#define RED_LED PORTCbits.RC2
#define BUZZER PORTCbits.RC3  // Buzzer connected to RC3

#define BLUE_LED_DIR TRISCbits.TRISC0
#define YELLOW_LED_DIR TRISCbits.TRISC1
#define RED_LED_DIR TRISCbits.TRISC2
#define BUZZER_DIR TRISCbits.TRISC3  // Set direction of RC3 for the buzzer

void leds_init(void) {
    // Initialize LED pins as outputs
    BLUE_LED_DIR = 0;
    YELLOW_LED_DIR = 0;
    RED_LED_DIR = 0;
    BUZZER_DIR = 0;  // Buzzer as output

    // Initialize LEDs and buzzer to off
    BLUE_LED = 0;
    YELLOW_LED = 0;
    RED_LED = 0;
    BUZZER = 0;  // Buzzer off
}

void leds_off(void) {
    BLUE_LED = 0;
    YELLOW_LED = 0;
    RED_LED = 0;
    BUZZER = 0;  // Buzzer off
}

void buzzer_on(void) {
    BUZZER = 1;  // Turn on the buzzer
}

void buzzer_off(void) {
    BUZZER = 0;  // Turn off the buzzer
}

void leds_set(unsigned char temperature, unsigned char gasDetected) {
    if (gasDetected) {
        // Gas detected: turn on all LEDs and buzzer
        BLUE_LED = 1;
        YELLOW_LED = 1;
        RED_LED = 1;
        buzzer_on();  // Activate the buzzer
    } else {
        // No gas detected: turn off LEDs and buzzer
        BLUE_LED = 0;
        YELLOW_LED = 0;
        RED_LED = 0;
        buzzer_off();  // Deactivate the buzzer

        if (temperature <= 22) {
            BLUE_LED = 1;  // Blue LED on for low temperature
        } else if (temperature <= 27) {
            YELLOW_LED = 1;  // Yellow LED on for medium temperature
        } else {
            RED_LED = 1;  // Red LED on for high temperature
        }
    }
}
