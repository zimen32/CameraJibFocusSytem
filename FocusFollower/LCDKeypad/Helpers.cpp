#include "Helpers.h"
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

float Helpers::calculateEncDegrees(int encVal){
	float deegree = (encVal - encMinVal) >= 0 ? (encVal - encMinVal) : 0;

	deegree *= (float)360/(encMaxVal - encMinVal);

	return deegree;
}

float Helpers::calculateArcLength(float radius, float startAngle, float currentAngle){
	return 2.0 * PI * radius *abs(currentAngle - startAngle) / 360.0;
}