#ifndef _LCD_1602A_1_OPTIONS_
#define _LCD_1602A_1_OPTIONS_

#include <avr/io.h>

#define F_CPU 16000000 //16MHz

//Порт данных
#define _DATA_DDR  DDRD
#define _DATA_PORT PORTD
#define _DATA_PIN  PIND
#define _D4 5
#define _D5 4
#define _D6 3
#define _D7 2

//Порт управления
#define _CONTROL_DDR  DDRB
#define _CONTROL_PORT PORTB
#define _CONTROL_PIN  PINB
#define _RS 0
#define _RW 1
#define _E  2

//Задержка строба
#define _STROB_DELAY_US 2
//Стартовая задержка
#define _START_DELAY_MS 50

#endif //_LCD_1602A_1_OPTIONS_