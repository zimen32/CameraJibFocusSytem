#pragma once
#include <LiquidCrystal.h> 
class Helpers
{
public:	
	static const int encMinVal = 122;
	static const int encMaxVal = 625;

	//Text heleprs
	static bool textContainsChar(char* text, char character, int length = 16);
	static void textClear(char* text);

	//LCD helpers
	static void lcdClearRow(LiquidCrystal lcd, int row);

	//Encoder helpers
	static float calculateEncDegrees(int encVal);	

	//Math helpers
	static float calculateArcLength(float radius, float startAngle, float currentAngle);

	//EEPROM
	static void EEPROMWriteInt(int address, int value);
	 
	static unsigned int EEPROMReadInt(int address);

};

