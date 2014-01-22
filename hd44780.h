//=================================================================
// biblioteka do obslugi wyswietlacza hd44780 w trybie
// 4-bit z obsluga flagi zajetosci
// wymagania co do pinow:
// *na port danych nalezy wybrac starsza lub mlodsza polowke portu
// *piny sterujace wyswietlaczem musza byc na jednym porcie
//=================================================================
#include <avr/io.h>
#include <util/delay.h>
//=================================================================
// konfiguracja pinow do obslugi wyswietlacza
//=================================================================
#define LCD_CONTROL_PORT D
#define LCD_RS (1<<PD5)
#define LCD_E (1<<PD6)
#define LCD_RW (1<<PD7)
#define	LCD_DATA_PORT B
#define LCD_DATA_MASK 0xf0
//=================================================================
// definicja komend wyswietlacza
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
// definicje funkcji
//=================================================================
unsigned char lcdIsBusy(void);// sprawdzenie gotowosci wyswietlacza LCD zwraca 1 gdy zajety
unsigned char lcdReadData();// odczyt danych z RAM wyswietlacza
unsigned char lcdRead();// odczyt bajtu
unsigned char lcdReadNibble();// odczyt polbajtu
void lcdSend(unsigned char dataToSend);// wysyla znak do lcd
void lcdSendNibble(unsigned char data);// wysyla polbajt do lcd
void lcdSendCommand(unsigned char data);// wysyla komende do wyswietlacza
void lcdSendChar(unsigned char charToSend);// wysyla znak do wyswietlacza
void lcdInit(void);// inicjalizacja wyswietlacza
//=================================================================
// koniec pliku
//=================================================================
