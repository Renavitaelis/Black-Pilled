#ifndef LCD_I2C_H
#define LCD_I2C_H

//--- So We Can Use Standard Integer Types With Fixed Width Guarantees ----------------------------

#include <stdint.h>

//--- End Of So We Can Use Standard Integer Types With Fixed Width Guarantees ---------------------

//--- Which STM32 Boards It Is Ready To Function With, Add As Necessary ----------------------------

#if __has_include("stm32f1xx_hal.h")
	#include "stm32f1xx_hal.h"
#elif __has_include("stm32f4xx_hal.h")
	#include "stm32f4xx_hal.h"
#endif

//--- End Of Which STM32 Boards It Is Ready To Function With, Add As Necessary --------------------

//--- Defines All The Data One Can Send Or Change On The LCD --------------------------------------

//-- Data Types -----------------------------------------------------------------------------------

typedef enum
{
    LCD_Command,
    LCD_Text
} LCD_Type_Of_Data;

//-- End Of Data Types ----------------------------------------------------------------------------

//-- Modifies Expected Data -----------------------------------------------------------------------

//- 0010 (M)(L)(F)0

#define LCD_Mod_Expected_Data 0x20 // 0010 0000

//- M
#define LCD_8bits_Mode 0x10 // 0001 0000
#define LCD_4bits_Mode 0x00 // 0000 0000

//- L
#define LCD_2Line_Mode 0x08 // 0000 1000
#define LCD_1Line_Mode 0x00 // 0000 0000

//- F
#define LCD_5x10_Font 0x04 // 0000 0100
#define LCD_5x8_Font 0x00 // 0000 0000

//-- End Of Modifies Expected Data ----------------------------------------------------------------

//-- Modifies User Interface Behavior -------------------------------------------------------------

//- 0000 1(D)(C)(B)

#define LCD_Mod_UI 0x08 // 0000 1000

//- D
#define LCD_Display_On 0x04 // 0000 0100
#define LCD_Display_Off 0x00 // 0000 0000

//- C
#define LCD_Cursor_On 0x02 // 0000 0010
#define LCD_Cursor_Off 0x00 // 0000 0000

//- B
#define LCD_Blink_On 0x01 // 0000 0001
#define LCD_Blink_Off 0x00 // 0000 0000

//-- End Of Modifies User Interface Behavior ------------------------------------------------------

//-- Modifies Entry Behavior ----------------------------------------------------------------------

//- 0000 01(D)(S)

#define LCD_Mod_Entry 0x04 // 0000 0100

//- D
#define LCD_Cursor_Move_Right 0x02 // 0000 0010
#define LCD_Cursor_Move_Left 0x00 // 0000 0000

//- S
#define LCD_Display_Shift_Enable 0x01 // 0000 0001
#define LCD_Display_Shift_Disable 0x00 // 0000 0000

//-- End Of Modifies Entry Behavior ---------------------------------------------------------------

//-- Shifts Display ^ Cursor ----------------------------------------------------------------------

//- 0001 (E)(D)XX

#define LCD_Shift_Control 0x10 // 0001 0000

//- E
#define LCD_Shift_Display 0x08 // 0000 1000
#define LCD_Shift_Cursor 0x00 // 0000 0000

//- D
#define LCD_Shift_Right 0x04 // 0000 0100
#define LCD_Shift_Left 0x00 // 0000 0000

//-- End Of Shifts Display ^ Cursor ---------------------------------------------------------------

//-- Control Bits (0000 (BL)(EN)(RW)(RS)) ---------------------------------------------------------

//- BL
#define LCD_BackLight_On   0x08		   // 0000 1000
#define LCD_BackLight_Off  0x00		   // 0000 0000

//- EN
#define LCD_EN_On 0x04 		   		   // 0000 0100
#define LCD_EN_Off 0x00		   		   // 0000 0000

//- RW
#define LCD_RW_On 0x02		   		   // 0000 0010
#define LCD_RW_Off 0x00		   		   // 0000 0000

//- RS
#define LCD_RS_On 0x01         		   // 0000 0001
#define LCD_RS_Off 0x00        		   // 0000 0000

//-- End Of Control Bits (0000 (BL)(EN)(RW)(RS)) --------------------------------------------------

//-- Other Commands -------------------------------------------------------------------------------

#define LCD_Clear_Display 0x01 		   // 0000 0001

#define LCD_Return_Home 0x02		   // 0000 0010

#define LCD_Set_CGRAM_Address 0x40	   // 0100 0000

#define LCD_Set_DDRAM_Address 0x80	   // 1000 0000

//-- End Of Other Commands ------------------------------------------------------------------------

//-- Others ---------------------------------------------------------------------------------------

#define LCD_Max_Internal_DDRAM_nColumns 40 // 0010 1000

//-- End Of Others --------------------------------------------------------------------------------

//--- Defines All The Data One Can Send Or Change On The LCD --------------------------------------

//--- Struct To Manipulate The LCD As Needed -------------------------------------------------------

typedef struct
{
//-- STM32 ----------------------------------------------------------------------------------------
    I2C_HandleTypeDef *STM_I2C_Handler;     // I2C Handler For Communication
//-- End Of STM32 ---------------------------------------------------------------------------------

//-- LCD User Given Characteristics ---------------------------------------------------------------
    uint8_t address,            			// I2C Address Of The LCD
    		nColumns,						// Max Number Of Columns On The LCD
    		nLines,							// Max Number Of Lines On The LCD
			CurrentColumn,					// Current Column On The LCD
			CurrentLine,					// Current Line On The LCD
//-- End Of LCD User Given Characteristics --------------------------------------------------------

//-- LCD Configuration ----------------------------------------------------------------------------
			BackLight,						// Blacklight Of The LCD
//- Expected Data
			BitMode,						// (4^8)Bits_Mode
			LineMode,						// (1^2)Line_Mode
			Font,							// 5x(8^10)_Font
//- UI
			Display,						// Display_(Off^On)
    		Cursor,							// Cursor_(Off^On)
			Blinking,						// Blink_(Off^On)
//- Entry Behaviour
    		WritingDirection,				// Cursor_Moves_To_(Left^Right)
			WritingShift;					// Display_Shift_(Dis)Enable
//-- End Of LCD Configuration ---------------------------------------------------------------------
}I2C_LCD_Handler;

//--- End Of Struct To Manipulate The LCD As Needed -----------------------------------------------

//--- LCD Hardware Focused Functions --------------------------------------------------------------

//- STM32 Electrical Translation Of Data To I2C
void LCD_Write_Bus(I2C_LCD_Handler *lcd, uint8_t *data, uint16_t size);

//Used To Send (Commands ^ Text) To LCD
void LCD_Send(I2C_LCD_Handler *lcd, uint8_t Data_Or_CMD, LCD_Type_Of_Data type);

void LCD_Set_Backlight(I2C_LCD_Handler *lcd, uint8_t mode);

void LCD_Set_Expected_Data(I2C_LCD_Handler *lcd, uint8_t BMode, uint8_t LMode, uint8_t FontType);

void LCD_Set_UI_Behavior(I2C_LCD_Handler *lcd, uint8_t DState, uint8_t CState, uint8_t BState);

void LCD_Set_Entry_Behavior(I2C_LCD_Handler *lcd, uint8_t Direction, uint8_t Shift);

void LCD_Default_Init(I2C_LCD_Handler *lcd, I2C_HandleTypeDef *hi2c, uint8_t address, uint8_t nColumns, uint8_t nLines);

//--- End Of LCD Hardware Focused Functions -------------------------------------------------------

//--- User Focused Functions ----------------------------------------------------------------------

//-- LCD Handler Gets -----------------------------------------------------------------------------

//- Althought You Can Use It Directly, The Compiler Often Complains If You Do, So Use These

uint8_t LCD_Get_nColumns(I2C_LCD_Handler *lcd);

uint8_t LCD_Get_nLines(I2C_LCD_Handler *lcd);

uint8_t LCD_Get_Column(I2C_LCD_Handler *lcd);

uint8_t LCD_Get_Line(I2C_LCD_Handler *lcd);

//-- End Of LCD Handler Gets ----------------------------------------------------------------------

//-- LCD Handler Sets -----------------------------------------------------------------------------

void LCD_Set_Pos(I2C_LCD_Handler *lcd, uint8_t column, uint8_t line);

//- (LCD_Set_Pos(0,0) & Undoes Any Shifts Made On The Screen)

void LCD_Pos_Home(I2C_LCD_Handler *lcd);

void LCD_Set_Current_Column(I2C_LCD_Handler *lcd, uint8_t column);

void LCD_Set_Current_Line(I2C_LCD_Handler *lcd, uint8_t line);

//-- End Of LCD Handler Sets ----------------------------------------------------------------------

//-- Writing Functions -----------------------------------------------------------------------------

void LCD_Write_Char(I2C_LCD_Handler *lcd, char ch);

void LCD_Fill_Line(I2C_LCD_Handler *lcd, uint8_t line, char ch);

void LCD_Fill_Partial_Line(I2C_LCD_Handler *lcd, uint8_t line, uint8_t start, char ch);

void LCD_Fill_All(I2C_LCD_Handler *lcd, char ch);

//- (Clears A Line Starting At A Specific Column & Set_Pos(start,line))
void LCD_Clear_Partial_Line(I2C_LCD_Handler *lcd, uint8_t line, uint8_t start);

//- (Clears A Line & Set_Pos(0,line))
void LCD_Clear_Line(I2C_LCD_Handler *lcd, uint8_t line);

//- (Clears ALL Text & Set_Pos(0,0))
void LCD_Clear_All(I2C_LCD_Handler *lcd);

void LCD_Write_String(I2C_LCD_Handler *lcd, const char *str);

//- Converts Numbers Into Text For Exhibition
void LCD_Write_Number(I2C_LCD_Handler *lcd, double num, uint8_t DecimalPrecision);

//-- End Of Writing Functions ---------------------------------------------------------------------

//-- LCD Advanced Screen Manipulation Functions ---------------------------------------------------

//- Any Funtion That Cannot Be Executed SingleHandedly By LCD_Send() (Needs MCU To Happen Properly)

void LCD_Scroll_Shift(I2C_LCD_Handler *lcd, uint8_t Display_Or_Cursor, uint8_t Direction ,  uint32_t shift_interval);

//-- End Of LCD Advanced Screen Manipulation Functions --------------------------------------------

//--- End Of User Focused Functions ---------------------------------------------------------------

#endif /* LCD_I2C_H */
