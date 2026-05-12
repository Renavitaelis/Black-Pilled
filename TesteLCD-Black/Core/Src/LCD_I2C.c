/** 
 * I2C LCD Library for STM32 
 * Object-oriented version with multiple LCD support
 */

#include "LCD_I2C.h"

/**
 * @brief  Initializes the LCD in 4-bit mode.
 * @param  lcd: Pointer to the LCD handle
 * @retval None
 */
void LCD_Init(I2C_LCD_HandleTypeDef *lcd)
{
    HAL_Delay(50);  // Wait for LCD power-up
    LCD_Send(lcd, 0x30, Command);  // Wake up command
    HAL_Delay(5);
    LCD_Send(lcd, 0x30, Command);  // Wake up command
    HAL_Delay(1);
    LCD_Send(lcd, 0x30, Command);  // Wake up command
    HAL_Delay(10);
    LCD_Send(lcd, 0x20, Command);  // Set to 4-bit mode
    HAL_Delay(10);

    // LCD configuration commands
    LCD_Send(lcd, 0x28, Command);  // 4-bit mode, 2 lines, 5x8 font
    HAL_Delay(1);
    LCD_Send(lcd, 0x08, Command);  // Display off, cursor off, blink off
    HAL_Delay(1);
    LCD_Send(lcd, 0x01, Command);  // Clear display
    HAL_Delay(2);
    LCD_Send(lcd, 0x06, Command);  // Entry mode: cursor moves right
    HAL_Delay(1);
    LCD_Send(lcd, 0x0C, Command);  // Display on, cursor off, blink off
}

/**
 * @brief  Sends a command to the LCD.
 * @param  lcd: Pointer to the LCD handle
 * @param  cmd: Command byte to send
 * @retval None
 */
void LCD_Send(I2C_LCD_HandleTypeDef *lcd, char cmd, char type)
{
    char upper_nibble, lower_nibble;
    uint8_t data_t[4];

    upper_nibble = (cmd & 0xF0);            // Extract upper nibble
    lower_nibble = ((cmd << 4) & 0xF0);     // Extract lower nibble
    
    if(type == Command)
    {
		data_t[0] = upper_nibble | 0x0C;  // en=1, rs=0
		data_t[1] = upper_nibble | 0x08;  // en=0, rs=0
		data_t[2] = lower_nibble | 0x0C;  // en=1, rs=0
		data_t[3] = lower_nibble | 0x08;  // en=0, rs=0
    }
    else if (type == Text)
    {
        data_t[0] = upper_nibble | 0x0D;  // en=1, rs=1
        data_t[1] = upper_nibble | 0x09;  // en=0, rs=1
        data_t[2] = lower_nibble | 0x0D;  // en=1, rs=1
        data_t[3] = lower_nibble | 0x09;  // en=0, rs=1
    }

    HAL_I2C_Master_Transmit(lcd->hi2c, lcd->address, data_t, 4, 100);
}

/**
 * @brief  Moves the cursor to a specific position on the LCD.
 * @param  lcd: Pointer to the LCD handle
 * @param  col: Column number (0-20)
 * @param  row: Row number (0-4)
 * @retval None
 */
void LCD_New_XY(I2C_LCD_HandleTypeDef *lcd, int col, int row)
{
    uint8_t address;

    switch (row)
    {
        case 0: address = 0x80 + col; break;  // First row
        case 1: address = 0xC0 + col; break;  // Second row
        case 2: address = 0x94 + col; break;  // Third row
        case 3: address = 0xD4 + col; break;  // Fourth row
        default: return;  // Ignore invalid row numbers
    }

    LCD_Send(lcd, address, Command);  // Send command to move the cursor
}

/**
 * @brief  Clears the LCD display.
 * @param  lcd: Pointer to the LCD handle
 * @retval None
 */
void LCD_Clear_All(I2C_LCD_HandleTypeDef *lcd)
{
	LCD_Send(lcd, 0x80, Command);  // Move cursor to the home position
    // Clear all characters
    // 16x2 = 32 characters
    // 20x2 = 40 characters
    // 16x4 = 64 characters
    // 20x4 = 80 characters
    // So 80 character clearing is enough for both 16x2, 16x4, 20x2 and 20x4 displays
    for (int character = 0; character < 80; character++)
    	LCD_Send(lcd, ' ', Text);  // Write a space on each position
}

void LCD_Clear_Row(I2C_LCD_HandleTypeDef *lcd, int row)
{
    LCD_New_XY(lcd, 0, row); // Move cursor to the beginning of selected row
    for (int column = 0; column < 20; column++)
    {
    	LCD_Send(lcd,' ', Text);  // Write a space on each position

        // If it has size 16, it won't erase the other lines accidentally:
        if (column+1 == 0xC0 || column+1 == 0x94 || column+1 == 0xD4)
        	break;
    }
}

/**
 * @brief  Sends a single character to the LCD.
 * @param  lcd: Pointer to the LCD handle
 * @param  ch: Character to send
 * @retval None
 */
void LCD_Put_Char(I2C_LCD_HandleTypeDef *lcd, char ch)
{
	LCD_Send(lcd, ch, Text);  // Send a character to the display
}

/**
 * @brief  Sends a string to the LCD.
 * @param  lcd: Pointer to the LCD handle
 * @param  str: Null-terminated char array (string) to display
 * @retval None
 */
void LCD_Put_String(I2C_LCD_HandleTypeDef *lcd, char *str)
{
    while (*str) LCD_Send(lcd, *str++, Text);  // Send each character in the string
}

/**
 * @brief Shifts all the characters displayed on the LCD one column to the left LCD.
 * @param lcd: Pointer to the LCD handle
 */
void LCD_Shift_Left(I2C_LCD_HandleTypeDef *lcd)
{
	LCD_Send(lcd, 0x1C, Command); //Shifts one column to the left
}

/**
 * @brief Shifts all the characters displayed on the LCD one column to the right.
 * @param lcd: Pointer to the LCD handle
 */
void LCD_Shift_Right(I2C_LCD_HandleTypeDef *lcd)
{
	LCD_Send(lcd, 0x18, Command); //Shifts one column to the right
}

/**
 * @brief Shifts all the characters displayed on the LCD one column to the left at a constant rate.
 * @param lcd: Pointer to the LCD handle
 * @shift_interval: Time between each shift in miliseconds
 */
void LCD_Scroll_Left(I2C_LCD_HandleTypeDef *lcd, uint32_t shift_interval)
{
	for(int column = 0; column < 40; column++)
	{
		LCD_Shift_Left(lcd); //Shifts one column to the left
		HAL_Delay(shift_interval); //Time between each shift
	}
}

/**
 * @brief Shifts all the characters displayed on the LCD one column to the right at a constant rate.
 * @param lcd: Pointer to the LCD handle
 * @shift_interval: Time between each shift in miliseconds
 */
void LCD_Scroll_Right(I2C_LCD_HandleTypeDef *lcd, uint32_t shift_interval)
{
	for(int column = 0; column < 40; column++)
	{
		LCD_Shift_Right(lcd); //Shifts one column to the right
		HAL_Delay(shift_interval); //Time between each shift
	}
}
