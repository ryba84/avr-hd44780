#include <hd44780.h>

unsigned char lcdIsBusy(void)
{
	PORT(LCD_CONTROL_PORT) &= ~LCD_RS;
	if (lcdRead() & lcdBusyFlag)
	{
		return 1;
	}
	return 0;
}
unsigned char lcdReadData(void)
{
	PORT(LCD_CONTROL_PORT) |= LCD_RS;
	return lcdRead();
}
unsigned char lcdRead(void)
{
	unsigned char tmp;
	DDR(LCD_DATA_PORT) &= ~LCD_DATA_MASK;
	PORT(LCD_CONTROL_PORT) |= LCD_RW;
	tmp = lcdReadNibble();
	tmp |= (lcdReadNibble()>>4);
	return tmp;
}
unsigned char lcdReadNibble(void)
{
	unsigned char tmp;
	PORT(LCD_CONTROL_PORT) |= LCD_E;
	tmp = (PIN(LCD_DATA_PORT) & LCD_DATA_MASK);
	PORT(LCD_CONTROL_PORT) &= ~LCD_E;
	return tmp;
}

void lcdSend(unsigned char dataToSend)
{
	DDR(LCD_DATA_PORT) |= LCD_DATA_MASK;
	PORT(LCD_CONTROL_PORT) &= ~LCD_RW;
	lcdSendNibble (dataToSend & LCD_DATA_MASK);
	lcdSendNibble ((dataToSend << 4) & LCD_DATA_MASK);
}

void lcdSendNibble(unsigned char data)
{
	PORT(LCD_CONTROL_PORT) |= LCD_E;
	PORT(LCD_DATA_PORT) &= ~LCD_DATA_MASK;
	PORT(LCD_DATA_PORT) |= data;
	PORT(LCD_CONTROL_PORT) &= ~LCD_E;
}

void lcdSendCommand(unsigned char data)
{
	PORT(LCD_CONTROL_PORT) &= ~LCD_RS;
	lcdSend(data);
}

void lcdSendChar(unsigned char charToSend)
{
	PORT(LCD_CONTROL_PORT) |= LCD_RS;
	lcdSend(charToSend);
}

void lcdInit(void)
{
	unsigned char cnt;
	DDR(LCD_CONTROL_PORT) |= LCD_E | LCD_RS | LCD_RS;
	DDR(LCD_DATA_PORT) |= LCD_DATA_MASK;
	_delay_ms(15);

	PORT(LCD_CONTROL_PORT) &= ~(LCD_E | LCD_RW | LCD_RS);
	for (cnt = 0; cnt < 3; cnt++)
	{
		PORT(LCD_CONTROL_PORT) |= LCD_E;
		PORT(LCD_DATA_PORT) &= ~LCD_DATA_MASK;
		PORT(LCD_DATA_PORT) |= (0x02<<4);
		PORT(LCD_CONTROL_PORT) &= ~LCD_E;
		_delay_ms (5);
	}
	_delay_ms(1);
	lcdSendCommand(functionSet | !interface8bit | display2lines);
	while (lcdIsBusy());
	lcdSendCommand(displayOnOff | !displayOn);
	while (lcdIsBusy());
	lcdSendCommand(clearDisplay);
	while (lcdIsBusy());
	lcdSendCommand(entryModeSet | cursorIncrement);
	while (lcdIsBusy());
	lcdSendCommand(displayOnOff | displayOn | !cursorOn | !cursorBlink);
}
