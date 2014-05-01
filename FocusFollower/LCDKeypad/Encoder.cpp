#include "Encoder.h"
#include "Arduino.h"
#define ToRad(x) ((x)*0.01745329252)  // *pi/180
Encoder::Encoder(){
}

Encoder::Encoder(int input){
	Input = input;
	MinValue = 256;
	MaxValue = 0;
}

Encoder::Encoder(int input, int minValue, int maxValue){
	Input = input;
	MinValue = minValue;
	MaxValue = maxValue;
}

Encoder::~Encoder(){
}	

float Encoder::GetAngle(){
	float angle = (Value - MinValue) >= 0 ? (Value - MinValue) : 0;

	angle *= (float)360 / (MaxValue - MinValue);

	return angle;
}

float Encoder::GetArc(){
	return 2.0 * PI * ArmLength * abs((GetAngle() < StartAngle ? 360 + GetAngle() : GetAngle()) - StartAngle) / 360.0;
}

float Encoder::GetChord(){
	
	return (ArmLength*sin(abs((GetAngle() < StartAngle ? 360 + GetAngle() : GetAngle()) - StartAngle) *PI / 180)) / sin((180 - abs((GetAngle() < StartAngle ? 360 + GetAngle() : GetAngle()) - StartAngle))*PI / 360);
}

float Encoder::GetHeadAngle(){
	return (float) (RawHeadAngle - abs(GetAngle() + EncHeadXDiff));
}

float Encoder::GetDelta(){
	//return abs(180.0 - abs(GetHeadAngle()));
	return 180.0 - RelativeStartHeadAngle;
}


float Encoder::GetGamma(){
	return  (180 - abs(GetAngle() - StartAngle) ) / 2;
}

float Encoder::GetEpsilon(){
	return 360 - GetGamma() - 180;// GetDelta();
}

float Encoder::GetTX(){
	return (sqrt((pow(ArmLength, 2) + pow(StartDistance, 2) - 2 * ArmLength*StartDistance*cos(GetDelta()))));
}

float Encoder::GetDistance(){

	float angle = GetAngle();
	float beta = abs(angle - StartAngle);
	float delta = 180;//GetDelta();
	float gamma = (180 - beta) / 2;
	float epsilon = 360 - gamma - delta;
	float chord = GetChord();
	
	float distance = sqrt(pow(chord, 2) + pow(StartDistance, 2) - 2 * chord*StartDistance*cos(ToRad(epsilon)));

	Serial.println(distance);
	//Serial.print("Distance: ");
	//Serial.print(distance);
	//Serial.print("  ");
	//Serial.print("Arnm length: ");
	//Serial.print(ArmLength);
	//Serial.print("  ");
	//Serial.print("Enc angle: ");
	//Serial.print(angle);
	//Serial.print("  ");
	//Serial.print("Start angle: ");
	//Serial.print(StartAngle);
	//Serial.print("  ");
	//Serial.print("Beta: ");
	//Serial.print(beta);
	//Serial.print("  ");
	//Serial.print("Start head angle: ");
	//Serial.print(RelativeStartHeadAngle);
	//Serial.print("  ");
	//Serial.print("Start distance: ");
	//Serial.print(StartDistance);
	//Serial.print("  ");
	//Serial.print("Delta: ");
	//Serial.print(delta);
	//Serial.print("  ");
	//Serial.print("Gamma: ");
	//Serial.println(gamma);
	//Serial.print("  ");
	//Serial.print("Epsilon: ");
	//Serial.print(epsilon);
	//Serial.print("  ");
	//Serial.print("C: ");
	//Serial.println(chord);
	
	
	

	return distance;
}

void Encoder::SetStartDistance(float distance){
	StartDistance = distance;
}

void Encoder::SetStartAngle(){
	StartAngle = GetAngle();
}

void Encoder::SetHeadStartAngle(){
	StartHeadAngle = RawHeadAngle;
	EncHeadXDiff = StartHeadAngle - GetAngle();
	RelativeStartHeadAngle = (float)GetHeadAngle();
}

void Encoder::SetRawHeadAngle(float angle){
	RawHeadAngle = angle;	
}

