#ifndef _LCD_1602A_1_
#define _LCD_1602A_1_

/*
DDRAM ADDRESS:
**  | 1| 2| 3| 4| 5| 6| 7| 8| 9|10|11|12|13|14|15|16|

 1  |00|01|02|03|04|05|06|07|08|09|0A|0B|0C|0D|0E|0F|
 2  |40|41|42|43|44|45|46|47|48|49|4A|4B|4C|4D|4E|4F|
*/

#include "lcd_1602A-1_options.h"

enum lcd_entry_mod {
	LCD_EM_INCREMENT   = 0x01,
	//LCD_EM_DECREMENT
	LCD_EM_SHIFT_ON    = 0x02,
	//LCD_EM_SHIFT_OFF
};

enum lcd_display_on_off {
	LCD_D_ON               = 0x01,
	//LCD_D_OFF            
	LCD_D_CURSOR_ON        = 0x02,
	//LCD_D_CURSOR_OFF    
	LCD_D_CURSOR_BLINK_ON  = 0x04
	//LCD_D_CURSOR_BLINK_OFF 
};

enum lcd_function {
	LCD_FCN_8_BIT     = 0x01,
	//LCD_FCN_4_BIT 
	//LCD_FCN_1_LINE
	LCD_FCN_2_LINE    = 0x02,
	LCD_FCN_FONT_5x11 = 0x04
	//LCD_FCN_FONT_5x8
};

enum lcd_shift {
	LCD_SH_CURSOR  = 0x01,
	LCD_SH_DISPLAY = 0x02,
	LCD_SH_RIGHT   = 0x04,
	LCD_SH_LEFT    = 0x08
};

/**
 * @brief Инициализация дисплея
 * @param -
 * @return -
 */
void lcd_init(enum lcd_function fcn, enum lcd_display_on_off disp,
	enum lcd_entry_mod entry);

/**
 * @brief Установка позиции курсора
 * @param -
 * @return -
 */
void lcd_set_ddram_address(uint8_t address);

/**
 * @brief Установка позиции в CGRAM для добавления нового символа
 * @param -
 * @return -
 */
void lcd_set_cgram_address(uint8_t address);

/**
 * @brief Отобразить строку
 * @param -
 * @return -
 */
void lcd_draw_string(const char* str);

/**
 * @brief Отобразить символ
 * @param -
 * @return -
 */
void lcd_draw_symbol(uint8_t c);

/**
 * @brief Очистка дисплея
 * @param -
 * @return -
 */
void lcd_clear();

/**
 * @brief Устанавливает DDRAM в 0x00 и возвращает курсор в оригинальную позицию. Данные DDRAM не изменяются.
 * @param -
 * @return -
 */
void lcd_return_home();

/**
 * @brief Устанавливает направление движения курсора и сдвига дисплеяю.
 * Перемещение происходит во время чтения/записи.
 * @param -
 * @return -
 */
void lcd_set_entry_mod(enum lcd_entry_mod mod);

/**
 * @brief Включает/выключает дисплей, устанавливает отабражение курсора.
 * @param -
 * @return -
 */
void lcd_set_display_on_off(enum lcd_display_on_off mod);

/**
 * @brief Сдвигает курсор/дисплей в заданную сторону.
 * @param -
 * @return -
 */
void lcd_set_shift(enum lcd_shift mod);

/**
 * @brief Устанавливает параметры дисплея
 * @param -
 * @return -
 */
void lcd_set_function(enum lcd_function mod);

/**
 * @brief Записывает новый символ в CGRAM
 * @param -
 * @return -
 */
void lcd_add_symbol(const uint8_t sym[8]);

#endif //_LCD_1602A_1_
