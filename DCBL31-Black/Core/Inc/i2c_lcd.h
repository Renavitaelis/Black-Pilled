#ifndef I2C_LCD_H
#define I2C_LCD_H

#include <stdint.h>
//#include <stdio.h>

/**
 * @brief Includes the HAL driver present in the project
 */
#if __has_include("stm32f1xx_hal.h")
	#include "stm32f1xx_hal.h"
#elif __has_include("stm32f4xx_hal.h")
	#include "stm32f4xx_hal.h"
#endif
/**
 * @brief Structure to hold LCD instance information
 */
typedef struct
{
    I2C_HandleTypeDef *hi2c;     // I2C handler for communication
    uint8_t address;            // I2C address of the LCD
} I2C_LCD_HandleTypeDef;

/**
 * @brief Initializes the LCD.
 * @param lcd: Pointer to the LCD handle
 */
void lcd_init(I2C_LCD_HandleTypeDef *lcd);

/**
 * @brief Sends a command to the LCD.
 * @param lcd: Pointer to the LCD handle
 * @param cmd: Command byte to send
 */
void lcd_send_cmd(I2C_LCD_HandleTypeDef *lcd, char cmd);

/**
 * @brief Moves the cursor to a specific position on the LCD.
 * @param lcd: Pointer to the LCD handle
 * @param col: Column number (0-20)
 * @param row: Row number (0 or 4)
 */
void lcd_gotoxy(I2C_LCD_HandleTypeDef *lcd, int col, int row);

/**
 * @brief Clears all rows on LCD display.
 * @param lcd: Pointer to the LCD handle
 */
void lcd_clear_all(I2C_LCD_HandleTypeDef *lcd);

/**
 * @brief Clears an specific row on the LCD display.
 * @param lcd: Pointer to the LCD handle
 * @param row : Row number (1-4)
 */
void lcd_clear_row(I2C_LCD_HandleTypeDef *lcd, int row);

/**
 * @brief Sends a single character to the LCD.
 * @param lcd: Pointer to the LCD handle
 * @param ch: Character to send
 */
void lcd_put_char(I2C_LCD_HandleTypeDef *lcd, char ch);

/**
 * @brief Sends a string to the LCD.
 * @param lcd: Pointer to the LCD handle
 * @param str: Null-terminated char array (string) to send
 */
void lcd_put_string(I2C_LCD_HandleTypeDef *lcd, char *str);

/**
 * @brief Sends a string to the LCD.
 * @param lcd: Pointer to the LCD handle
 * @param str: Null-terminated char array (string) to send
 */
void lcd_put_ConvertedString(I2C_LCD_HandleTypeDef *lcd, char *str);

/**
 * @brief Shifts all the characters displayed on the LCD one column to the left LCD.
 * @param lcd: Pointer to the LCD handle
 */

void lcd_shift_left(I2C_LCD_HandleTypeDef *lcd);

/**
 * @brief Shifts all the characters displayed on the LCD one column to the right.
 * @param lcd: Pointer to the LCD handle
 */
void lcd_shift_right(I2C_LCD_HandleTypeDef *lcd);

/**
 * @brief Shifts all the characters displayed on the LCD one column to the left at a constant rate.
 * @param lcd: Pointer to the LCD handle
 * @shift_interval: Time between each shift in miliseconds
 */
void lcd_scroll_left(I2C_LCD_HandleTypeDef *lcd, uint32_t shift_interval);

/**
 * @brief Shifts all the characters displayed on the LCD one column to the right at a constant rate.
 * @param lcd: Pointer to the LCD handle
 * @shift_interval: Time between each shift in miliseconds
 */
void lcd_scroll_right(I2C_LCD_HandleTypeDef *lcd, uint32_t shift_interval);

#endif /* I2C_LCD_H */
