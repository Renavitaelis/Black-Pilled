#ifndef LCD_I2C_H
#define LCD_I2C_H

#include <stdint.h>
#include <stdio.h>

#define Command 'C'
#define Text 'T'

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
void LCD_Init(I2C_LCD_HandleTypeDef *lcd);

/**
 * @brief Sends a command to the LCD.
 * @param lcd: Pointer to the LCD handle
 * @param cmd: Command byte to send
 */
void LCD_Send(I2C_LCD_HandleTypeDef *lcd, char cmd, char type);

/**
 * @brief Moves the cursor to a specific position on the LCD.
 * @param lcd: Pointer to the LCD handle
 * @param col: Column number (0-20)
 * @param row: Row number (0 or 4)
 */
void LCD_New_XY(I2C_LCD_HandleTypeDef *lcd, int col, int row);

/**
 * @brief Clears all rows on LCD display.
 * @param lcd: Pointer to the LCD handle
 */
void LCD_Clear_All(I2C_LCD_HandleTypeDef *lcd);

/**
 * @brief Clears an specific row on the LCD display.
 * @param lcd: Pointer to the LCD handle
 * @param row : Row number (1-4)
 */
void LCD_Clear_Row(I2C_LCD_HandleTypeDef *lcd, int row);

/**
 * @brief Clears an specific row on the LCD display.
 * @param lcd: Pointer to the LCD handle
 * @param row : Row number (1-4)
 */
void LCD_Clear_Partial_Row(I2C_LCD_HandleTypeDef *lcd, int row, int start, int end);

/**
 * @brief Sends a single character to the LCD.
 * @param lcd: Pointer to the LCD handle
 * @param ch: Character to send
 */
void LCD_Put_Char(I2C_LCD_HandleTypeDef *lcd, char ch);

/**
 * @brief Sends a string to the LCD.
 * @param lcd: Pointer to the LCD handle
 * @param str: Null-terminated char array (string) to send
 */
void LCD_Put_String(I2C_LCD_HandleTypeDef *lcd, char *str);

/**
 * @brief Sends a string to the LCD.
 * @param lcd: Pointer to the LCD handle
 * @param str: Null-terminated char array (string) to send
 */
void LCD_Put_Converted_StringU16(I2C_LCD_HandleTypeDef *lcd, uint16_t num);

void LCD_Put_Converted_StringF(I2C_LCD_HandleTypeDef *lcd, float num);

/**
 * @brief Shifts all the characters displayed on the LCD one column to the left LCD.
 * @param lcd: Pointer to the LCD handle
 */

void LCD_Shift_Left(I2C_LCD_HandleTypeDef *lcd);

/**
 * @brief Shifts all the characters displayed on the LCD one column to the right.
 * @param lcd: Pointer to the LCD handle
 */
void LCD_Shift_Right(I2C_LCD_HandleTypeDef *lcd);

/**
 * @brief Shifts all the characters displayed on the LCD one column to the left at a constant rate.
 * @param lcd: Pointer to the LCD handle
 * @shift_interval: Time between each shift in miliseconds
 */
void LCD_Scroll_Left(I2C_LCD_HandleTypeDef *lcd, uint32_t shift_interval);

/**
 * @brief Shifts all the characters displayed on the LCD one column to the right at a constant rate.
 * @param lcd: Pointer to the LCD handle
 * @shift_interval: Time between each shift in miliseconds
 */
void LCD_Scroll_Right(I2C_LCD_HandleTypeDef *lcd, uint32_t shift_interval);

#endif /* LCD_I2C_H */
