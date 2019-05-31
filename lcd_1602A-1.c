#include "lcd_1602A-1.h"

#include <util/delay.h>

inline void e_on()
{
	_CONTROL_PORT |= (1 << _E);
}

inline void e_off()
{
	_CONTROL_PORT &= ~(1 << _E);
}

inline void rw_on()
{
	_CONTROL_PORT |= (1 << _RW);
}

inline void rw_off()
{
	_CONTROL_PORT &= ~(1 << _RW);
}

inline void rs_on()
{
	_CONTROL_PORT |= (1 << _RS);
}

inline void rs_off()
{
	_CONTROL_PORT &= ~(1 << _RS);
}

void strob() 
{
	//Поднимаем строб
	e_on();

	_delay_us(_STROB_DELAY_US);

	//Опускаем строб
	e_off();
}

inline uint8_t get_data_bus_4()
{
	return ((1 << _D4) | (1 << _D5) |
			(1 << _D6) | (1 << _D7));
}

inline uint8_t get_byte(uint8_t data, uint8_t n)
{
	if (data & (1 << n)) {
		return 1;
	} else {
		return 0;
	}
}

//Отображает данные на порт данных
inline uint8_t data_to_bus_4(uint8_t data)
{
	return ((get_byte(data, 0) << _D4) |
			(get_byte(data, 1) << _D5) |
			(get_byte(data, 2) << _D6) |
			(get_byte(data, 3) << _D7));
}

//Передаёт младшие 4 бита
void common_write_func_4(uint8_t data) 
{
	//Порты на запись 
	rw_off();

	//Очищаем порт данных
	_DATA_PORT &= ~get_data_bus_4();
	//Выставляем данные
	_DATA_PORT |= data_to_bus_4(data);

	strob();
}

void common_write_func_low(uint8_t data)
{
	common_write_func_4(data);
}

void common_write_func_high(uint8_t data)
{
	common_write_func_4(data >> 4);
}

void common_write_byte_4(uint8_t data)
{
	common_write_func_high(data);
	common_write_func_low(data);
}

void write_data_low(uint8_t data)
{
	//Пишем данные
	rs_on();
	common_write_func_low(data);
}

void write_data_4(uint8_t data)
{
	//Пишем данные
	rs_on();
	common_write_byte_4(data);
}

void write_command_low(uint8_t data)
{
	//Пишем команду 
	rs_off();
	common_write_func_low(data);
}

void write_command_4(uint8_t data)
{
	//Пишем команду 
	rs_off();
	common_write_byte_4(data);
}

uint8_t common_read_half()
{
	//Порты на чтение команды
	rw_on();
	rs_off();
	
	_DATA_DDR &= ~get_data_bus_4();
	_DATA_PORT |= get_data_bus_4();
	
	//Взводим строб
	e_on();
	_delay_us(_STROB_DELAY_US);

	uint8_t pin = _DATA_PIN;
	
	//Опускаем строб
	e_off();
	
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

	_DATA_DDR |= get_data_bus_4();
	rw_off();

	return temp;
}

uint8_t common_read_byte_4()
{
	uint8_t temp = 0;

	temp |= common_read_half();
	temp = temp << 4;
	temp |= common_read_half();

	return temp;
}

uint8_t read_data_byte() 
{
	rs_on();
	return common_read_byte_4();
}

uint8_t read_command_byte()
{
	rs_off();
	return common_read_byte_4();
}

uint8_t is_busy()
{
	if (read_command_byte() & (1 << 7)) {
		return 0xff;
	} else {
		return 0;
	}
}

void lcd_clear()
{
	while (is_busy());
	
	write_command_4(0x01);
}

void lcd_return_home()
{
	while (is_busy());
	
	write_command_4(0x02);
}

void lcd_set_entry_mod(enum lcd_entry_mod mod)
{
	while (is_busy());
	
	uint8_t data = 0x04;
	
	if (mod & LCD_EM_INCREMENT) {
		data |= 0x02;
	}
	
	if (mod & LCD_EM_SHIFT_ON) {
		data |= 0x01;
	}
	
	write_command_4(data);
}

void lcd_set_display_on_off(enum lcd_display_on_off mod)
{
	while (is_busy());
	
	uint8_t data = 0x08;
	
	if (mod & LCD_D_ON) {
		data |= 0x04;
	}
	
	if (mod & LCD_D_CURSOR_ON) {
		data |= 0x02;
	}
	
	if (mod & LCD_D_CURSOR_BLINK_ON) {
		data |= 0x01;	
	}
	
	write_command_4(data);
}

void lcd_set_shift(enum lcd_shift mod)
{
	while (is_busy());
	
	uint8_t data = 0x0f;
	
	if (mod & LCD_SH_DISPLAY) {
		data |= 0x08;
	}
	
	if (mod & LCD_SH_RIGHT) {
		data |= 0x04;
	}
	
	write_command_4(data);
}

void lcd_set_function(enum lcd_function mod)
{
	while (is_busy());
	
	uint8_t data = 0x20;
	
	if (mod & LCD_FCN_8_BIT) {
		data |= 0x0f;
	}
	
	if (mod & LCD_FCN_2_LINE) {
		data |= 0x08;
	}
	
	if (mod & LCD_FCN_FONT_5x11) {
		data |= 0x04;
	}
	
	write_command_4(data);
}

void lcd_init(enum lcd_function fcn, enum lcd_display_on_off disp,
	enum lcd_entry_mod entry)
{
	_CONTROL_DDR |= (1 << _E) | (1 << _RS) | (1 << _RW);
	_DATA_DDR    |= get_data_bus_4();
	
	_delay_ms(_START_DELAY_MS);
	
	write_command_low(0b0010);
	
	//function set
	//_delay_us(50);
	lcd_set_function(fcn);
	
	//display on off
	//_delay_us(50);
	lcd_set_display_on_off(disp);
	
	//clear
	//_delay_ms(50);
	lcd_clear();
	
	//entry mod
	lcd_set_entry_mod(entry);
}

void lcd_set_ddram_address(uint8_t address)
{
	while (is_busy());
	
	write_command_4(address | 0x80);
}

void lcd_set_cgram_address(uint8_t address)
{
	while (is_busy());
	
	write_command_4(address | 0x40);
}

void lcd_draw_string(const char* str)
{
	uint8_t data = 0;
	while (*str) {
		data = *str++;
		while (is_busy());
		write_data_4(data);
	}
}

void lcd_draw_symbol(uint8_t c)
{
	while (is_busy());
	
	write_data_4(c);
}

void lcd_add_symbol(const uint8_t sym[8])
{
	for (uint8_t i = 0; i != 8; ++i) {
		while (is_busy());
		write_data_4(sym[i]);
	}
}