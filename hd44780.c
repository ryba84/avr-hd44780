//=================================================================
// AVR library for hd44780 compatible LCD display
// ryba.lodz@gmail.com
// works in 4-bit or 8-bit mode with busy flag read
//=================================================================
#include <hd44780.h>

#if !(defined(CHARS_PER_LINE_8) || defined(CHARS_PER_LINE_16) || defined(CHARS_PER_LINE_20))
	#error Please define chow many chars per line have Your LCD!!
#elif defined(CHARS_PER_LINE_8) && defined(CHARS_PER_LINE_16)
	#error Define only one CHARS_PER_LINE!!
#elif defined(CHARS_PER_LINE_8) && defined(CHARS_PER_LINE_20)
	#error Define only one CHARS_PER_LINE!!
#elif defined(CHARS_PER_LINE_16) && defined(CHARS_PER_LINE_20)
	#error Define only one CHARS_PER_LINE!!
#endif
#if !(defined(ONE_LINE) || defined(TWO_LINES) || defined(FOUR_LINES))
	#error Please define chow many lines have Your LCD!!
#elif defined(ONE_LINE) && defined(TWO_LINES)
	#error Define only number of lines!!
#elif defined(ONE_LINE) && defined(FOUR_LINES)
	#error Define only number of lines!!
#elif defined(TWO_LINES) && defined(FOUR_LINES)
	#error Define only number of lines!!
#endif
#if !(defined(CURSOR_DECREMENT) || defined(CURSOR_INCREMENT))
	#error Please define way of cursor move!!
#elif defined(CURSOR_DECREMENT) && defined(CURSOR_INCREMENT)
	#error Define only one way of cursor move!!
#endif
#if !(defined(MODE_4_BIT) || defined(MODE_8_BIT))
	#error Please define way of cursor move!!
#elif defined(MODE_4_BIT) && defined(MODE_8_BIT)
	#error Define only one way of cursor move!!
#endif

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
#ifdef MODE_4_BIT
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
#endif
#ifdef MODE_8_BIT
unsigned char lcdRead(void)
{
	unsigned char tmp;
	DDR(LCD_DATA_PORT) &= 0xff;
	PORT(LCD_CONTROL_PORT) |= LCD_RW;
	PORT(LCD_CONTROL_PORT) |= LCD_E;
	asm volatile ("nop"); // without this doesn't working with 8Mhz AVR
	tmp = PIN(LCD_DATA_PORT);
	PORT(LCD_CONTROL_PORT) &= ~LCD_E;

	return tmp;
}
#endif
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
#ifdef MODE_4_BIT
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
#endif
#ifdef MODE_8_BIT
void lcdSend(unsigned char dataToSend)
{
	DDR(LCD_DATA_PORT) |= 0xff;
	PORT(LCD_CONTROL_PORT) &= ~LCD_RW;
	PORT(LCD_CONTROL_PORT) |= LCD_E;
	PORT(LCD_DATA_PORT) &= ~LCD_DATA_MASK;
	PORT(LCD_DATA_PORT) |= dataToSend;
	PORT(LCD_CONTROL_PORT) &= ~LCD_E;
}
#endif
void lcdInit(void)
{
	unsigned char cnt = 2;
	DDR(LCD_CONTROL_PORT) |= LCD_E | LCD_RW | LCD_RS;
#ifdef MODE_4_BIT
	DDR(LCD_DATA_PORT) |= LCD_DATA_MASK;
#endif
#ifdef MODE_8_BIT
	DDR(LCD_DATA_PORT) |= 0xff;
#endif
	_delay_ms(15);

	PORT(LCD_CONTROL_PORT) &= ~(LCD_E | LCD_RW | LCD_RS);
#ifdef MODE_4_BIT
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
#endif
#ifdef MODE_8_BIT
	// doesn't work for now
#endif
	_delay_ms(1);
#ifdef TWO_LINES
	lcdSendCommand(functionSet | display2lines);
#endif
#ifdef ONE_LINE
	lcdSendCommand(functionSet | display1line);
#endif
#ifdef FOUR_LINES
	lcdSendCommand(functionSet | display2lines);
#endif
	while (lcdIsBusy());
	lcdSendCommand(displayOnOff);
	while (lcdIsBusy());
	lcdSendCommand(clearDisplay);
	while (lcdIsBusy());
#ifdef CURSOR_INCREMENT
	lcdSendCommand(entryModeSet | cursorIncrement);
#endif
#ifdef CURSOR_DECREMENT
	lcdSendCommand(entryModeSet | cursorDecrement);
#endif
	while (lcdIsBusy());
	lcdSendCommand(displayOnOff | displayOn);
}
