#ifndef _LCD_1602A_1_
#define _LCD_1602A_1_

/*
DDRAM ADDRESS:
**  | 1| 2| 3| 4| 5| 6| 7| 8| 9|10|11|12|13|14|15|16|

 1  |00|01|02|03|04|05|06|07|08|09|0A|0B|0C|0D|0E|0F|
 2  |40|41|42|43|44|45|46|47|48|49|4A|4B|4C|4D|4E|4F|
*/

#include "lcd_1602A-1_options.h"

#define LCD_CURSOR  0x00
#define LCD_DISPLAY 0x01

#define LCD_DECREASE 0x00
#define LCD_INCREASE 0x01

#define LCD_LEFT  0x00
#define LCD_RIGHT 0x01

#define LCD_OFF 0x00
#define LCD_ON  0x01

//|0|1|2|
//|B|L|F|
#define LCD_CFG_4_BIT    0x00
#define LCD_CFG_8_BIT    0x01
#define LCD_CFG_1_LINE   0x00
#define LCD_CFG_2_LINE   0x02
#define LCD_CFG_FNT_5x8  0x00
#define LCD_CFG_FNT_5x11 0x04

/**
 * @brief Инициализация дисплея
 * @param -
 * @return -
 */
void lcd_init(uint8_t lcd_cfg, uint8_t display_on, uint8_t cursor_on, uint8_t blink_on,
	uint8_t source, uint8_t direction);

/**
 * @brief Установка позиции курсора
 * @param -
 * @return -
 */
void lcd_set_ddram_address(uint8_t address);

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
void lcd_set_entry_mod(uint8_t source, uint8_t direction);

/**
 * @brief Включает/выключает дисплей, устанавливает отабражение курсора.
 * @param -
 * @return -
 */
void lcd_on_off(uint8_t display, uint8_t cursor, uint8_t cursor_blink);

/**
 * @brief Сдвигает курсор/дисплей в заданную сторону.
 * @param -
 * @return -
 */
void lcd_set_shift(uint8_t source, uint8_t direction);

/**
 * @brief Устанавливает параметры дисплея (Function Set)
 * @param -
 * @return -
 */
void lcd_set_config(uint8_t lcd_cfg);

/**
 * @brief Записывает новый символ в CGRAM
 * @param -
 * @return -
 */
void lcd_add_symbol(uint8_t address, const uint8_t sym[8]);

#endif //_LCD_1602A_1_
