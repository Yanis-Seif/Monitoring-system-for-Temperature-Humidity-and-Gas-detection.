#include "lcdd.h"
#include <xc.h>
#define _XTAL_FREQ 20000000

void Lcd_SetBit(char data_bit) {
    D4 = (data_bit & 1) ? 1 : 0;
    D5 = (data_bit & 2) ? 1 : 0;
    D6 = (data_bit & 4) ? 1 : 0;
    D7 = (data_bit & 8) ? 1 : 0;
}

void Lcd_Cmd(char cmd) {
    RS = 0;
    Lcd_SetBit((cmd >> 4) & 0x0F);
    EN = 1;
    __delay_us(40);
    EN = 0;

    Lcd_SetBit(cmd & 0x0F);
    EN = 1;
    __delay_us(40);
    EN = 0;

    __delay_ms(2);
}

void Lcd_Clear(void) {
    Lcd_Cmd(0x01);
    __delay_ms(2);
}

void Lcd_Set_Cursor(char row, char col) {
    char address = (row == 1) ? (0x80 + col - 1) : (0xC0 + col - 1);
    Lcd_Cmd(address);
}

void Lcd_Start(void) {
    TRISD = 0x00;

    __delay_ms(20);

    RS = 0; EN = 0;

    Lcd_SetBit(0x03);
    EN = 1; __delay_us(40); EN = 0;
    __delay_ms(5);

    Lcd_SetBit(0x03);
    EN = 1; __delay_us(40); EN = 0;
    __delay_ms(5);

    Lcd_SetBit(0x03);
    EN = 1; __delay_us(40); EN = 0;
    __delay_ms(5);

    Lcd_SetBit(0x02);
    EN = 1; __delay_us(40); EN = 0;
    __delay_ms(5);

    Lcd_Cmd(0x28);
    Lcd_Cmd(0x0C);
    Lcd_Cmd(0x06);

    Lcd_Clear();
}

void Lcd_Print_Char(char data) {
    RS = 1;

    Lcd_SetBit((data >> 4) & 0x0F);
    EN = 1; __delay_us(40); EN = 0;

    Lcd_SetBit(data & 0x0F);
    EN = 1; __delay_us(40); EN = 0;

    __delay_ms(2);
}

void Lcd_Print_String(const char *str) {
    while (*str) {
        Lcd_Print_Char(*str++);
    }
}
