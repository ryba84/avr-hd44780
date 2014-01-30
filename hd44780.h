//=================================================================
// AVR library for hd44780 compatible LCD display
// ryba.lodz@gmail.com
// works in 4-bit or 8-bit mode with busy flag read
//=================================================================
#ifndef HD44780_H
#define HD44780_H

#include <avr/io.h>
#include <util/delay.h>
//=================================================================
// Uncomment below needed lines for Your display
//=================================================================
//#define CHARS_PER_LINE_8
#define CHARS_PER_LINE_16
//#define CHARS_PER_LINE_20
//#define ONE_LINE
#define TWO_LINES
//#define FOUR_LINES
#define CURSOR_DECREMENT
//#define CURSOR_INCREMENT
#define MODE_4_BIT
//#define MODE_8_BIT
//==================================================================
// In this part You can configure Your pins and ports used with LCD.
// Data port in 4-bit mode must be lower or higher nibble of used
// port. LCD_DATA_MASK must be set as the used nibble for data port
// (for 8 bit mode it doesn't matter).
// Pins for LCD control lines must be on one port and You must
// define this lines.
//==================================================================
#define LCD_CONTROL_PORT D
#define LCD_RS (1<<PD5)
#define LCD_RW (1<<PD6)
#define LCD_E (1<<PD7)
#define	LCD_DATA_PORT B
#define LCD_DATA_MASK 0xf0
//=================================================================
// other needed definitions for working defined macros
//=================================================================
#define PORT_(port) PORT ## port
#define DDR_(port)  DDR  ## port
#define PIN_(port)  PIN  ## port

#define PORT(port) PORT_(port)
#define DDR(port)  DDR_(port)
#define PIN(port)  PIN_(port)
//=================================================================
// LCD commands
//=================================================================
#define clearDisplay			0x01
#define returnHome				0x02
#define entryModeSet			0x04
	#define cursorIncrement		0x02
	#define cursorDecrement		0x00
	#define shiftDisplay		0x01
#define displayOnOff			0x08
	#define displayOn			0x04
	#define cursorOn			0x02
	#define cursorBlink			0x01
#define displayShift			0x10
	#define shiftCursorRight	0x04
	#define shiftDisplayLeft	0x08
	#define shiftDisplayRight	0x0c
#define functionSet				0x20
	#define interface8bit		0x10
	#define display1line		0x00
	#define display2lines		0x08
	#define font5x11			0x04
#define setDdramAddress			0x80
	#define firstLine			0x00
#ifdef CHARS_PER_LINE_8
	#define endFirstLine		0x07
#endif
#ifdef CHARS_PER_LINE_16
	#define endFirstLine		0x0f
#endif
#ifdef CHARS_PER_LINE_20
	#define endFirstLine		0x13
#endif
	#define secondLine			0x40
#ifdef CHARS_PER_LINE_8
	#define endSecondLine		0x47
#endif
#ifdef CHARS_PER_LINE_16
	#define endSecondLine		0x4f
#endif
#ifdef CHARS_PER_LINE_20
	#define endSecondLine		0x53
#endif
#ifdef FOUR_LINES
	#define thirdLine			0x14
	#define fourthLine			0x54
	#ifdef CHARS_PER_LINE_8
		#define endThirdLine	0x1b
		#define endFourthLine	0x5b
	#endif
	#ifdef CHARS_PER_LINE_16
		#define endThirdLine	0x23
		#define endFourthLine	0x63
	#endif
	#ifdef CHARS_PER_LINE_20
		#define endThirdLine	0x27
		#define endFourthLine	0x67
	#endif
#endif
#define lcdBusyFlag				0x80
//=================================================================
// functions definitions
//=================================================================
unsigned char lcdIsBusy(void);
unsigned char lcdReadData();
unsigned char lcdRead();
#ifdef MODE_4_BIT
unsigned char lcdReadNibble();
#endif
void lcdSendCommand(unsigned char data);
void lcdSendChar(unsigned char charToSend);
void lcdSend(unsigned char dataToSend);
#ifdef MODE_4_BIT
void lcdSendNibble(unsigned char data);
#endif
void lcdInit(void);

#endif /* HD44780_H */
//=================================================================
// EOF
//=================================================================
