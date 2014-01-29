//=================================================================
// AVR library for hd44780 compatible LCD display
// ryba.lodz@gmail.com
// 4-bit with busy flag read
//=================================================================
#include <hd44780.h>

// Read BusyFlag
unsigned char lcdIsBusy(void)
{
	PORT(LCD_CONTROL_PORT) &= ~LCD_RS;
	if (lcdRead() & lcdBusyFlag)
	{
		return 1;
	}
	return 0;
}

// Read data from LCD RAM
unsigned char lcdReadData(void)
{
	PORT(LCD_CONTROL_PORT) |= LCD_RS;
	return lcdRead();
}

// Read byte from LCD
unsigned char lcdRead(void)
{
	unsigned char tmp;
	DDR(LCD_DATA_PORT) &= ~LCD_DATA_MASK;
	PORT(LCD_CONTROL_PORT) |= LCD_RW;
	if (LCD_DATA_MASK & 0xf0)
	{
		tmp = lcdReadNibble();
		tmp |= (lcdReadNibble()>>4);
	}
	else
	{
		tmp = (lcdReadNibble()<<4);
		tmp |= lcdReadNibble();
	}
	return tmp;
}

// Read nibble from LCD
unsigned char lcdReadNibble(void)
{
	unsigned char inNibble;
	PORT(LCD_CONTROL_PORT) |= LCD_E;
	asm volatile ("nop"); // without this doesn't working with 8Mhz AVR
	inNibble = (PIN(LCD_DATA_PORT) & LCD_DATA_MASK);
	PORT(LCD_CONTROL_PORT) &= ~LCD_E;
	return inNibble;
}

// Send command to LCD
void lcdSendCommand(unsigned char commandToSend)
{
	PORT(LCD_CONTROL_PORT) &= ~LCD_RS;
	lcdSend(commandToSend);
}

// Send char to LCD
void lcdSendChar(unsigned char charToSend)
{
	PORT(LCD_CONTROL_PORT) |= LCD_RS;
	lcdSend(charToSend);
}

// Send data to LCD
void lcdSend(unsigned char dataToSend)
{
	DDR(LCD_DATA_PORT) |= LCD_DATA_MASK;
	PORT(LCD_CONTROL_PORT) &= ~LCD_RW;
	if (LCD_DATA_MASK & 0xf0)
	{
		lcdSendNibble (dataToSend & LCD_DATA_MASK);
		lcdSendNibble ((dataToSend << 4) & LCD_DATA_MASK);
	}
	else
	{
		lcdSendNibble ((dataToSend >> 4) & LCD_DATA_MASK);
		lcdSendNibble (dataToSend & LCD_DATA_MASK);
	}
}

// Send nibble to LCD
void lcdSendNibble(unsigned char nibbleToSend)
{
	PORT(LCD_CONTROL_PORT) |= LCD_E;
	PORT(LCD_DATA_PORT) &= ~LCD_DATA_MASK;
	PORT(LCD_DATA_PORT) |= nibbleToSend;
	PORT(LCD_CONTROL_PORT) &= ~LCD_E;
}

void lcdInit(void)
{
	unsigned char cnt = 2;
	DDR(LCD_CONTROL_PORT) |= LCD_E | LCD_RW | LCD_RS;
	DDR(LCD_DATA_PORT) |= LCD_DATA_MASK;
	_delay_ms(15);

	PORT(LCD_CONTROL_PORT) &= ~(LCD_E | LCD_RW | LCD_RS);
	PORT(LCD_DATA_PORT) &= ~LCD_DATA_MASK;
	if (LCD_DATA_MASK & 0xf0)
	{
		do
		{
			lcdSendNibble(0x03<<4);
			_delay_ms (5);
		} while (cnt--);
		lcdSendNibble(0x02<<4);
	}
	else
	{
		do
		{
			lcdSendNibble(0x03);
			_delay_ms (5);
		} while (cnt--);
		lcdSendNibble(0x02);
	}
	_delay_ms(1);
	lcdSendCommand(functionSet | display2lines);
	while (lcdIsBusy());
	lcdSendCommand(displayOnOff);
	while (lcdIsBusy());
	lcdSendCommand(clearDisplay);
	while (lcdIsBusy());
	lcdSendCommand(entryModeSet | cursorIncrement);
	while (lcdIsBusy());
	lcdSendCommand(displayOnOff | displayOn);
}
