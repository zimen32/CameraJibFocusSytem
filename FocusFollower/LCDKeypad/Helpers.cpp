#include "Helpers.h"
#include <EEPROM.h>
#include <Arduino.h>
#include <LiquidCrystal.h>

void Helpers::lcdClearRow(LiquidCrystal lcd, int row){
	if (row == 0 || row == 1)
	{	
		lcd.setCursor(0, row);
		lcd.print("                ");
		lcd.setCursor(0, row);
	}	
}

bool Helpers::textContainsChar(char* text, char character, int length){

	for (int i = 0; i < length; i++)
	{
		if (text[i] == character)
			return true;
	}

	return false;
}

void Helpers::textClear(char* text){
	memset(text, 0, sizeof(text));
}


void Helpers::EEPROMWriteInt(int address, int value){
	byte lowByte = ((value >> 0) & 0xFF);
	byte highByte = ((value >> 8) & 0xFF);

	EEPROM.write(address, lowByte);
	EEPROM.write(address + 1, highByte);
}


unsigned int Helpers::EEPROMReadInt(int address){
	byte lowByte = EEPROM.read(address);
	byte highByte = EEPROM.read(address + 1);

	return ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
}

