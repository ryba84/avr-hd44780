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
#define LCD_E (1<<PD6)
#define LCD_RW (1<<PD7)
#define	LCD_DATA_PORT B
#define LCD_DATA_MASK 0xf0
//=================================================================
// other needed definitions for working macros
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
	#define cursorIncrement		(1<<1)
	#define shiftDisplay		(1<<0)
#define displayOnOff			0x08
	#define displayOn			(1<<2)
	#define cursorOn			(1<<1)
	#define cursorBlink			(1<<0)
#define displayShift			0x10
	#define shiftCursorLeft		(0<<0)
	#define shiftCursorRight	(1<<2)
	#define shiftDisplayLeft	(1<<3)
	#define shiftDisplayRight	(3<<2)
#define functionSet				0x20
	#define interface8bit		(1<<4)
	#define display2lines		(1<<3)
	#define font5x11			(1<<2)
#define lcdBusyFlag				(1<<7)
#define setDdramAddress			0x80
	#define firstLine			0x00
	#define secondLine			0x40
//=================================================================
// functions
//=================================================================
unsigned char lcdIsBusy(void);
unsigned char lcdReadData();
unsigned char lcdRead();
unsigned char lcdReadNibble();
void lcdSend(unsigned char dataToSend);
void lcdSendNibble(unsigned char data);
void lcdSendCommand(unsigned char data);
void lcdSendChar(unsigned char charToSend);
void lcdInit(void);
//=================================================================
// EOF
//=================================================================
