//=================================================================
// AVR library for hd44780 compatible LCD display
// ryba.lodz@gmail.com
// 4-bit with busy flag read
//=================================================================
#include <avr/io.h>
#include <util/delay.h>
//=================================================================
// port & pin config
//=================================================================
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
	#define display2lines		0x08
	#define font5x11			0x04
#define setDdramAddress			0x80
	#define firstLine			0x00
	#define secondLine			0x40
#define lcdBusyFlag				0x80
//=================================================================
// functions
//=================================================================
unsigned char lcdIsBusy(void);
unsigned char lcdReadData();
unsigned char lcdRead();
unsigned char lcdReadNibble();
void lcdSendCommand(unsigned char data);
void lcdSendChar(unsigned char charToSend);
void lcdSend(unsigned char dataToSend);
void lcdSendNibble(unsigned char data);
void lcdInit(void);
//=================================================================
// EOF
//=================================================================
