#include "LCD_I2C.h"

//--- LCD Hardware Focused Functions ---------------------------------------------------------------

void LCD_Write_Bus(I2C_LCD_Handler *lcd, uint8_t *data, uint16_t size)
{
    HAL_I2C_Master_Transmit(lcd->STM_I2C_Handler, lcd->address, data, size, 100);
}

void LCD_Send(I2C_LCD_Handler *lcd, uint8_t Data_Or_CMD, LCD_Type_Of_Data Type)
{
    uint8_t Data_Buffer[4];

    for(uint8_t Data_Pulse = 0; Data_Pulse < 4 ; Data_Pulse++)
    	Data_Buffer[Data_Pulse] = ((Data_Pulse&1)==0)?
				( ( (Data_Or_CMD << (0x04 & Data_Pulse<<1) ) & 0xF0 ) | lcd->BackLight | LCD_EN_On | LCD_RW_Off | (Type&LCD_RS_On))
				:
				( ( (Data_Or_CMD << (0x04 & Data_Pulse<<1) ) & 0xF0 ) | lcd->BackLight | LCD_EN_Off | LCD_RW_Off | (Type&LCD_RS_On));

    LCD_Write_Bus(lcd, Data_Buffer, sizeof(Data_Buffer));
}

void LCD_Set_Backlight(I2C_LCD_Handler *lcd, uint8_t mode)
{
    lcd->BackLight = mode;

    LCD_Write_Bus(lcd, &lcd->BackLight, sizeof(lcd->BackLight));
}

void LCD_Set_Expected_Data(I2C_LCD_Handler *lcd, uint8_t BMode, uint8_t LMode, uint8_t FontType)
{
	lcd->BitMode = BMode;

	lcd->LineMode = LMode;

	lcd->Font = FontType;

	LCD_Send(lcd, (LCD_Mod_Expected_Data|BMode|LMode|FontType), LCD_Command);
}

void LCD_Set_UI_Behavior(I2C_LCD_Handler *lcd, uint8_t DState, uint8_t CState, uint8_t BState)
{
	lcd->Display = DState;

	lcd->Cursor = CState;

	lcd->Blinking = BState;

	LCD_Send(lcd, (LCD_Mod_UI|DState|CState|BState), LCD_Command);
}

void LCD_Set_Entry_Behavior(I2C_LCD_Handler *lcd, uint8_t Direction, uint8_t Shift)
{
	lcd->WritingDirection = Direction;

	lcd->WritingShift = Shift;

	LCD_Send(lcd, (LCD_Mod_Entry|Direction|Shift), LCD_Command);
}

void LCD_Default_Init(I2C_LCD_Handler *lcd, I2C_HandleTypeDef *STM_H_I2C, uint8_t address, uint8_t nColumns, uint8_t nLines)
{
//- Struct Building
	lcd->STM_I2C_Handler = STM_H_I2C;
	lcd->address = (address<<1);
	lcd->nColumns = nColumns;
	lcd->nLines = nLines;
	lcd->CurrentColumn = 0;
	lcd->CurrentLine = 0;
	LCD_Set_Backlight(lcd, LCD_BackLight_On);

//- Wake Up Sequence
    HAL_Delay(50); // Wait For LCD To Power-Up

    for(uint8_t CPR = 0; CPR < 3; CPR++) // Does "CPR" On The LCD Hoping It Will Begin Receiving Data
    {
    	LCD_Set_Expected_Data(lcd, LCD_8bits_Mode, LCD_1Line_Mode, LCD_5x8_Font);
        HAL_Delay(1); // "Compression Rhythm"
    }

    LCD_Set_Expected_Data(lcd, LCD_4bits_Mode, LCD_1Line_Mode, LCD_5x8_Font);
    HAL_Delay(1);

//- Function Set
    LCD_Set_Expected_Data(lcd, LCD_4bits_Mode, LCD_2Line_Mode, LCD_5x8_Font);
    HAL_Delay(1);

//- Display Off
    LCD_Set_UI_Behavior(lcd, LCD_Display_Off, LCD_Cursor_Off, LCD_Blink_Off);

//- Clear Display
    LCD_Send(lcd, LCD_Clear_Display, LCD_Command);
    HAL_Delay(2); //Required By DataSheet (max ~2ms)

//- Entry Mode Set
    LCD_Set_Entry_Behavior(lcd, LCD_Cursor_Move_Right, LCD_Display_Shift_Disable);
    HAL_Delay(1);

//- Display ON
    LCD_Set_UI_Behavior(lcd, LCD_Display_On, LCD_Cursor_Off, LCD_Blink_Off);
    HAL_Delay(1);
}

//--- End Of LCD Hardware Focused Functions -------------------------------------------------------

//--- User Focused Functions ----------------------------------------------------------------------

//-- LCD Handler Gets -----------------------------------------------------------------------------

//- Althought You Can Use It Directly, The Compiler Often Complains If You Do, So Use These

uint8_t LCD_Get_nColumns(I2C_LCD_Handler *lcd)
{
	return lcd->nColumns;
}

uint8_t LCD_Get_nLines(I2C_LCD_Handler *lcd)
{
	return lcd->nLines;
}

uint8_t LCD_Get_Column(I2C_LCD_Handler *lcd)
{
	return lcd->CurrentColumn;
}

uint8_t LCD_Get_Line(I2C_LCD_Handler *lcd)
{
	return lcd->CurrentLine;
}

//-- End Of LCD Handler Gets ----------------------------------------------------------------------

//-- LCD Handler Sets -----------------------------------------------------------------------------

void LCD_Set_Pos(I2C_LCD_Handler *lcd, uint8_t column, uint8_t line)
{
	if(column >= lcd->nColumns)
	    return; //Invalid Column

    uint8_t address;

    switch (line) // Address Values May Differ, But This Ought To Work With Most LCDs
    {
        case 0: address = 0x00 + column; break;  // First line
        case 1: address = 0x40 + column; break;  // Second line
        case 2: address = 0x14 + column; break;  // Third line
        case 3: address = 0x54 + column; break;  // Fourth line
        default: return; // Invalid Line
    }

    lcd->CurrentColumn = column;
    lcd->CurrentLine = line;

    LCD_Send(lcd, (LCD_Set_DDRAM_Address | address), LCD_Command);
}

void LCD_Set_Pos_Home(I2C_LCD_Handler *lcd)
{
    LCD_Send(lcd, LCD_Return_Home, LCD_Command);

    HAL_Delay(2);

    lcd->CurrentColumn = 0;
    lcd->CurrentLine = 0;
}

void LCD_Set_Current_Column(I2C_LCD_Handler *lcd, uint8_t column)
{
	LCD_Set_Pos(lcd, column , lcd->CurrentLine);
}

void LCD_Set_Current_Line(I2C_LCD_Handler *lcd, uint8_t line)
{
	LCD_Set_Pos(lcd, lcd->CurrentColumn, line);
}

//-- End Of LCD Handler Sets ----------------------------------------------------------------------

//-- Writing Functions -----------------------------------------------------------------------------

void LCD_Write_Char(I2C_LCD_Handler *lcd, char ch)
{
    LCD_Send(lcd, ch, LCD_Text);

    if(++lcd->CurrentColumn >= lcd->nColumns)
    {
        lcd->CurrentColumn = 0;

        if(++lcd->CurrentLine >= lcd->nLines)
            lcd->CurrentLine = 0;

        LCD_Set_Pos(lcd, lcd->CurrentColumn,lcd->CurrentLine);
    }
}

void LCD_Fill_Partial_Line(I2C_LCD_Handler *lcd, uint8_t line, uint8_t start, char ch)
{
    LCD_Set_Pos(lcd, start, line);
    for (uint8_t column = start; column < lcd->nColumns; column++)
    	LCD_Write_Char(lcd, ch);
}

void LCD_Fill_Line(I2C_LCD_Handler *lcd, uint8_t line, char ch)
{
    LCD_Fill_Partial_Line(lcd, line, 0, ch);
}

void LCD_Fill_All(I2C_LCD_Handler *lcd, char ch)
{
    for (uint8_t line = 0; line < lcd->nLines; line++)
    	LCD_Fill_Line(lcd, line, ch);
}

//- (Clears A Line Starting At A Specific Column & Set_Pos(start,line))
void LCD_Clear_Partial_Line(I2C_LCD_Handler *lcd, uint8_t line, uint8_t start)
{
    LCD_Fill_Partial_Line(lcd, line, start, ' ');

    LCD_Set_Pos(lcd, start, line);
}

//- (Clears A Line & Set_Pos(0,line))
void LCD_Clear_Line(I2C_LCD_Handler *lcd, uint8_t line)
{
    LCD_Fill_Line(lcd, line , ' ');

    LCD_Set_Pos(lcd, 0, line);
}

//- (Clears ALL Text & Set_Pos(0,0))
void LCD_Clear_All(I2C_LCD_Handler *lcd)
{
    LCD_Send(lcd, LCD_Clear_Display, LCD_Command);

    HAL_Delay(2); //Required By DataSheet (max ~2ms)

    lcd->CurrentColumn = 0;
    lcd->CurrentLine = 0;

    // LCD_Clear_Display Also Resets Entry_Behavior, So This Line Undoes That
    LCD_Set_Entry_Behavior(lcd, lcd->WritingDirection, lcd->WritingShift);
}

void LCD_Write_String(I2C_LCD_Handler *lcd, const char *str)
{
    while (*str) LCD_Write_Char(lcd, *str++);  // Send each character in the string
}

//- Converts Numbers Into Text For Exhibition
void LCD_Write_Number(I2C_LCD_Handler *lcd, double Num, uint8_t DecimalPrecision)
{
	if(DecimalPrecision > 9)
	    return; //Decimal Precision is too big for uint32_t conversion

	uint8_t TxtSize = 0;

    uint32_t Uint32Num = Num<0?
    		(TxtSize++,(uint32_t)(-Num))
    		:
			(uint32_t)Num;

    uint8_t Uint32NumDigits = 1;

    for(uint32_t CountingDigits = Uint32Num ; CountingDigits >= 10; CountingDigits /= 10)
        Uint32NumDigits++;

     TxtSize += DecimalPrecision?
		Uint32NumDigits + 1 + DecimalPrecision + 1
		:
		Uint32NumDigits + 1;

    char Txt[TxtSize];

    uint8_t index = sizeof(Txt) - 1;

    if(index>(lcd->nColumns-lcd->CurrentColumn))
    	return; //num too big to exhibit

    Txt[index] = '\0';

    if(DecimalPrecision)
    {
        uint32_t Pow10[] =
        {
            1, 			// 0
            10, 		// 1
            100,		// 2
            1000,		// 3
            10000,		// 4
            100000,		// 5
            1000000,	// 6
            10000000,	// 7
            100000000,	// 8
            1000000000 	// 9
        }; // uint32_t has max value of 10 digits

        double decimal = Num<0?
        		((-Num) - Uint32Num)
				:
				(Num - Uint32Num);

        uint32_t dNum = (uint32_t)(decimal * Pow10[DecimalPrecision]);

        for(uint8_t DecimalDigits = 0; DecimalDigits < DecimalPrecision; DecimalDigits++)
        {
            Txt[--index] = '0' + (dNum % 10);
            dNum /= 10;
        }

        Txt[--index] = ',';
    }

    do
    {
        Txt[--index] = '0' + (Uint32Num % 10);
        Uint32Num /= 10;
    }
    while(Uint32Num);

    if (Num<0)
    	Txt[--index] = '-';

    LCD_Write_String(lcd, &Txt[index]);
}

//-- End Of Writing Functions ---------------------------------------------------------------------

//-- LCD Advanced Screen Manipulation Functions ---------------------------------------------------

//- Any Funtion That Cannot Be Executed SingleHandedly By LCD_Send() (Needs MCU To Happen Properly)

void LCD_Scroll_Shift(I2C_LCD_Handler *lcd, uint8_t Display_Or_Cursor, uint8_t Direction ,  uint32_t shift_interval)
{
	if ((Display_Or_Cursor == LCD_Shift_Display || Display_Or_Cursor == LCD_Shift_Cursor) && (Direction == LCD_Shift_Left ||Direction == LCD_Shift_Right))
		for(uint8_t column = 0; column < LCD_Max_Internal_DDRAM_nColumns; column++)
		{
			LCD_Send(lcd, (LCD_Shift_Control | Display_Or_Cursor | Direction), LCD_Command);
			HAL_Delay(shift_interval); //Time between each shift
		}
}

//-- End Of LCD Advanced Screen Manipulation Functions --------------------------------------------

//--- End Of User Focused Functions ---------------------------------------------------------------
