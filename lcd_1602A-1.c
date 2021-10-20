#include "lcd_1602A-1.h"

#include <util/delay.h>

inline void _e_on()
{
	_CONTROL_PORT |= (1 << _E);
}

inline void _e_off()
{
	_CONTROL_PORT &= ~(1 << _E);
}

inline void _rw_on()
{
	_CONTROL_PORT |= (1 << _RW);
}

inline void _rw_off()
{
	_CONTROL_PORT &= ~(1 << _RW);
}

inline void _rs_on()
{
	_CONTROL_PORT |= (1 << _RS);
}

inline void _rs_off()
{
	_CONTROL_PORT &= ~(1 << _RS);
}

//Стробирование
void _strob() 
{
	//Поднимаем строб
	_e_on();

	_delay_us(_STROB_DELAY_US);

	//Опускаем строб
	_e_off();
}

inline uint8_t _get_data_pins()
{
	return ((1 << _D4) | (1 << _D5) |
			(1 << _D6) | (1 << _D7));
}

//Возвращает 1 если бит n установлен, иначе 0.
inline uint8_t _get_byte(uint8_t data, uint8_t n)
{
	if (data & (1 << n)) {
		return 1;
	} else {
		return 0;
	}
}

//Отображает данные на порт данных
inline uint8_t _data_to_pins(uint8_t data)
{
	return ((_get_byte(data, 0) << _D4) |
			(_get_byte(data, 1) << _D5) |
			(_get_byte(data, 2) << _D6) |
			(_get_byte(data, 3) << _D7));
}

//Получить 4 бита
uint8_t _receive_4bits()
{
	//Порты на чтение команды
	_rw_on();
	_rs_off();
	
	_DATA_DDR &= ~_get_data_pins();
	_DATA_PORT |= _get_data_pins();
	
	//Взводим строб
	_e_on();
	_delay_us(_STROB_DELAY_US);

	uint8_t pin = _DATA_PIN;
	
	//Опускаем строб
	_e_off();
	
	uint8_t temp = 0;

	if (pin & (1 << _D4)) {
		temp |= 0x1;
	}
	
	if (pin & (1 << _D5)) {
		temp |= 0x2;
	}
	
	if (pin & (1 << _D6)) {
		temp |= 0x4;
	}
	
	if (pin & (1 << _D7)) {
		temp |= 0x8;
	}

	_DATA_DDR |= _get_data_pins();
	_rw_off();

	return temp;
}

//Получить байт
uint8_t _receive_byte()
{
	uint8_t temp = 0;

	temp |= _receive_4bits();
	temp = temp << 4;
	temp |= _receive_4bits();

	return temp;
}

//Прочитать данные
uint8_t _receive_data_byte()
{
	_rs_on();
	uint8_t data = _receive_byte();
	_rs_off();
	
	return data;
}

//Прочитать команду
uint8_t _receive_command_byte()
{
	_rs_off();
	return _receive_byte();
}

//Проверка флага занятости
uint8_t _is_busy()
{
	if (_receive_command_byte() & (1 << 7)) {
		return 0xff;
		} else {
		return 0;
	}
}

//Передаёт младшие 4 бита без проверки занятости.
void _send_low(uint8_t data) 
{
	//Порты на запись 
	_rw_off();

	//Очищаем порт данных
	_DATA_PORT &= ~_get_data_pins();
	//Выставляем данные
	_DATA_PORT |= _data_to_pins(data);

	_strob();
}

//Передаёт старшие 4 бита без проверки занятости.
inline void _send_high(uint8_t data)
{
	_send_low(data >> 4);
}

//Передаёт байт
void _send_byte(uint8_t data)
{
	//Сначала старшая половина, затем младшая.
	
	_send_high(data);
	_send_low(data);
}

//Передаёт младшие 4 бита как данные
void _send_data_low(uint8_t data)
{
	while (_is_busy());
	
	//Пишем данные
	_rs_on();
	_send_low(data);
	_rs_off();
}

//Передаёт байт как данные
void _send_data_byte(uint8_t data)
{
	while (_is_busy());
	
	//Пишем данные
	_rs_on();
	_send_byte(data);
	_rs_off();
}

//Передаёт младшие 4 бита как команду
void _send_command_low(uint8_t data)
{
	while (_is_busy());
	
	//Пишем команду 
	_rs_off();
	_send_low(data);
}

//Передаёт младшие байт как команду
void _send_command_byte(uint8_t data)
{
	while (_is_busy());
	
	//Пишем команду 
	_rs_off();
	_send_byte(data);
}

void lcd_clear()
{
	_send_command_byte(0x01);
}

void lcd_return_home()
{	
	_send_command_byte(0x02);
}

void lcd_set_entry_mod(uint8_t source, uint8_t direction)
{	
	uint8_t data = 0x04;
	
	if (direction & LCD_INCREASE) {
		data |= 0x02;
	}
	
	if (source & LCD_DISPLAY) {
		data |= 0x01;
	}
	
	_send_command_byte(data);
}

void lcd_on_off(uint8_t display, uint8_t cursor, uint8_t cursor_blink)
{	
	uint8_t data = 0x08;
	
	if (display & LCD_ON) {
		data |= 0x04;
	}
	
	if (cursor & LCD_ON) {
		data |= 0x02;
	}
	
	if (cursor_blink & LCD_ON) {
		data |= 0x01;	
	}
	
	_send_command_byte(data);
}

void lcd_set_shift(uint8_t source, uint8_t direction)
{	
	uint8_t data = 0x0f;
	
	if (source & LCD_DISPLAY) {
		data |= 0x08;
	}
	
	if (direction & LCD_RIGHT) {
		data |= 0x04;
	}
	
	_send_command_byte(data);
}

void lcd_set_config(uint8_t lcd_cfg)
{	
	uint8_t data = 0x20;
	
	if (lcd_cfg & LCD_CFG_8_BIT) {
		data |= 0x0f;
	}
	
	if (lcd_cfg & LCD_CFG_2_LINE) {
		data |= 0x08;
	}
	
	if (lcd_cfg & LCD_CFG_FNT_5x11) {
		data |= 0x04;
	}
	
	_send_command_byte(data);
}

void lcd_init(uint8_t lcd_cfg, uint8_t display_on, uint8_t cursor_on, uint8_t blink_on,
	uint8_t source, uint8_t direction)
{
	_CONTROL_DDR |= (1 << _E) | (1 << _RS) | (1 << _RW);
	_DATA_DDR    |= _get_data_pins();
	
	_delay_ms(_START_DELAY_MS);
	
	_send_command_low(0b0010);
	
	//function set
	//_delay_us(50);
	lcd_set_config(lcd_cfg);
	
	//display on off
	//_delay_us(50);
	lcd_on_off(display_on, cursor_on, blink_on);
	
	//clear
	//_delay_ms(50);
	lcd_clear();
	
	//entry mod
	lcd_set_entry_mod(source, direction);
}

void lcd_set_ddram_address(uint8_t address)
{
	_send_command_byte(address | 0x80);
}

void lcd_draw_string(const char* str)
{
	uint8_t data = 0;
	while (*str) {
		data = *str++;
		_send_data_byte(data);
	}
}

void lcd_draw_symbol(uint8_t c)
{	
	_send_data_byte(c);
}

//Установка позиции в CGRAM для добавления нового символа
inline void _set_cgram_address(uint8_t address)
{
	_send_command_byte(address | 0x40);
}

void lcd_add_symbol(uint8_t address, const uint8_t sym[8])
{
	_set_cgram_address(address);
	
	for (uint8_t i = 0; i != 8; ++i) {
		_send_data_byte(sym[i]);
	}
}
