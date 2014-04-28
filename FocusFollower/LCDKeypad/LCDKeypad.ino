#include <LiquidCrystal.h>
#include <Keypad.h>
#include <ctype.h>
#include "Steps.h"
#include "Helpers.h"

char liczba[1];
int step = 0;
char armLengthText[16];
int armLengthTextCount = 0;
float armLength = 0.0;
long lastEncPrintTime;
float encAngle = 0;
float startAngle = 0;
float arcLength = 0;

//INPUTS
int encIn = A5;


//VALUES
int encVal;

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns

char numberKeys[ROWS][COLS] = {
    { '1','2','3' },
    { '4','5','6' },
    { '7','8','9' },
    { '.','0','#' }
};

byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 7, 6}; //connect to the column pinouts of the keypad

// Create two new keypads, one is a number pad and the other is a letter pad.
Keypad numpad( makeKeymap(numberKeys), rowPins, colPins, sizeof(rowPins), sizeof(colPins) );

unsigned long startTime;
const byte ledPin = 13;                            
char key;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 44, 45, 46, 47);
static byte kpadState;


void setup() {
	// set up the LCD's number of columns and rows: 
	lcd.begin(16, 2);
	// Print a message to the LCD.  
	Serial.begin(9600);
	pinMode(ledPin, OUTPUT);
	digitalWrite(ledPin, LOW);                 // Turns the LED on.
	numpad.begin(makeKeymap(numberKeys));	
	numpad.addEventListener(keypadEvent_num);  // Add an event listener.
	numpad.setHoldTime(500);                   // Default is 1000mS
	lcd.println("AutoFocus SYSTEM");
	lcd.setCursor(0, 1);
	lcd.println("press any key...");
	lastEncPrintTime = millis();
}

void loop() {      

	//TODO: analog signal noise reduction - just like in the AHRS MatLab program 
	encVal = analogRead(encIn);
	key = numpad.getKey();

	if (step == LiveRead && millis() - lastEncPrintTime > 100 )
	{
		lcd.setCursor(0, 1);	
		lcd.write("Enc: ");	
		encAngle = Helpers::calculateEncDegrees(encVal);
		lcd.print(floor(encAngle), 2);
		lastEncPrintTime = millis();		
	}

	else if (step == LiveArcLength && millis() - lastEncPrintTime > 100)
	{
		lcd.setCursor(0, 1);
		lcd.write("Arc: ");
		encAngle = Helpers::calculateEncDegrees(encVal);
		arcLength = Helpers::calculateArcLength(armLength, startAngle, encAngle);
		lcd.print(arcLength);
		lastEncPrintTime = millis();
	}

	delay(10);
}

void keypadEvent_num( KeypadEvent key ) {
	
    // in here when using number keypad
    kpadState = numpad.getState( );
    swOnState( key );
	if (step == Start)
	{
		lcd.clear();
		lcd.println("Arm length [cm] ");
		lcd.setCursor(0, 1);
		lcd.cursor();
		step = Radius;
	}
} 

void swOnState( char key ) {
    switch( kpadState ) {
        case PRESSED:           
			if (step == (int)Radius && armLengthTextCount < 16 && (isdigit(key) || key == ' ' || key == '.'))
			{
				if (key == '.' && Helpers::textContainsChar(armLengthText, '.'))
				{
					break;
				}					
				else
				{
					lcd.write(key);
					armLengthText[armLengthTextCount] = key;
					armLengthTextCount++;
				}				
			}	

			else if (step == Radius && key == '#' && armLengthTextCount > 0)
			{
				armLength = atof(armLengthText);
				Helpers::textClear(armLengthText);
				armLengthTextCount = 0;

				lcd.clear();
				lcd.write("Arm: ");			
				lcd.print(armLength);
				lcd.write("cm");
				lcd.noCursor();
				step = LiveRead;
				lcd.setCursor(0, 1);
			}

			else if (step == LiveRead && key == '#')
			{
				Serial.println(encAngle);
				startAngle = encAngle;
				step = LiveArcLength;
			}
			
			break;    

		case HOLD:
			if (step == Radius &&  key == '.')
			{
				Helpers::lcdClearRow(lcd, 1);
				Helpers::textClear(armLengthText);
				armLengthTextCount = 0;				
			}
			break;

    }  // end switch-case
}// end switch on state function


