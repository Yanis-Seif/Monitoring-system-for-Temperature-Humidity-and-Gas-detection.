#include <xc.h>
#include <stdio.h>
#include "lcdd.h"
#include "ledbuzz.h"

#define _XTAL_FREQ 20000000

#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define MQ2_PIN PORTBbits.RB2
#define MQ2_DIR TRISBbits.TRISB2

#define DHT11_PIN PORTBbits.RB1
#define DHT11_TRIS TRISBbits.TRISB1

unsigned char temperature = 0;
unsigned char humidity = 0;

// DHT11 helper functions
void dht11_start_signal() {
    DHT11_TRIS = 0; // output
    PORTBbits.RB1 = 0;
    __delay_ms(20);
    PORTBbits.RB1 = 1;
    __delay_us(30);
    DHT11_TRIS = 1; // input
}

char dht11_check_response() {
    unsigned char count = 0;
    while (DHT11_PIN == 1) {
        count++;
        if (count > 100) return 0;
        __delay_us(1);
    }
    count = 0;
    while (DHT11_PIN == 0) {
        count++;
        if (count > 100) return 0;
        __delay_us(1);
    }
    count = 0;
    while (DHT11_PIN == 1) {
        count++;
        if (count > 100) return 0;
        __delay_us(1);
    }
    return 1;
}

unsigned char dht11_read_byte() {
    unsigned char i, byte = 0;
    for (i = 0; i < 8; i++) {
        while (DHT11_PIN == 0);
        __delay_us(30);
        if (DHT11_PIN == 1) {
            byte |= (1 << (7 - i));
        }
        while (DHT11_PIN == 1);
    }
    return byte;
}

char dht11_read_data(unsigned char *humidity, unsigned char *temperature) {
    unsigned char hum_int, hum_dec, temp_int, temp_dec, checksum;
    dht11_start_signal();
    if (!dht11_check_response()) return 0;

    hum_int = dht11_read_byte();
    hum_dec = dht11_read_byte();
    temp_int = dht11_read_byte();
    temp_dec = dht11_read_byte();
    checksum = dht11_read_byte();

    if (((hum_int + hum_dec + temp_int + temp_dec) & 0xFF) != checksum) return 0;

    *humidity = hum_int;
    *temperature = temp_int;
    return 1;
}

void main(void) {
    MQ2_DIR = 1;  // MQ2 input
    leds_init();
    Lcd_Start();
    Lcd_Clear();

    // Startup messages
    Lcd_Set_Cursor(1,1);
    Lcd_Print_String("Project group 16");
    Lcd_Set_Cursor(2,1);
    Lcd_Print_String("Microprocessors");
    __delay_ms(2000);

    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Print_String("Interfacing MQ2,");
    Lcd_Set_Cursor(2,1);
    Lcd_Print_String("DHT11 in PIC");
    __delay_ms(2000);

    while (1) {
        if (!dht11_read_data(&humidity, &temperature)) {
            temperature = 0;
            humidity = 0;
        }

        char buffer[17];
        Lcd_Clear();
        Lcd_Set_Cursor(1,1);
        sprintf(buffer, "Temp:%2d  H:%2d", temperature, humidity);
        Lcd_Print_String(buffer);

        if (MQ2_PIN == 1) {
            leds_set(temperature, 1);
            Lcd_Set_Cursor(2,1);
            Lcd_Print_String("!!! GAS ALERT !!!");
            __delay_ms(500);
            leds_off();
            __delay_ms(500);
        } else {
            leds_set(temperature, 0);
            Lcd_Set_Cursor(2,1);
            Lcd_Print_String("No Gas detected ");
            __delay_ms(1500);
        }
    }
}
