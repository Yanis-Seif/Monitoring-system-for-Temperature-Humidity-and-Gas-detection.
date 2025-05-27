#ifndef LCD_H
#define LCD_H

#include <xc.h>

#define RS RD2
#define EN RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7

void Lcd_Start(void);
void Lcd_Clear(void);
void Lcd_Set_Cursor(char row, char col);
void Lcd_Print_String(const char *str);
void Lcd_Print_Char(char data);
void Lcd_Cmd(char cmd);
void Lcd_SetBit(char data_bit);

#endif
