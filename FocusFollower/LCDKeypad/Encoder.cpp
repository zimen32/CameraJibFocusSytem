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
	float angle = (Value - MinValue) >= 1 ? (Value - MinValue) : 1;

	angle *= (float)360 / (MaxValue - MinValue);

	return angle > 360 ? 360 : angle;
}

float Encoder::GetArc(){
	return 2.0 * PI * ArmLength * GetBeta() / 360.0;
}

float Encoder::GetChord(){

	float chord = (ArmLength*sin(GetBeta() *PI / 180)) / sin((180 - GetBeta())*PI / 360);

	if (chord > 2*ArmLength)
		chord = 2*ArmLength;

	if (chord < 0)
		chord = 0.0;

	return chord;
}

float Encoder::GetHeadAngle(){
	return (float) -(RawHeadAngle - abs(GetAngle() + EncHeadXDiff));
}

float Encoder::GetDelta(){	
	return abs(180.0 - RelativeStartHeadAngle);
}

float Encoder::GetBeta(){
	float beta = abs((GetAngle() < StartAngle ? 360 + GetAngle() : GetAngle()) - StartAngle);
	return  (beta < 180.1 && beta > 179.9) ? 179.9 : beta;
}

float Encoder::GetGamma(){
	return  (180 - GetBeta() ) / 2;
}

float Encoder::GetEpsilon(){
	return 360 - GetGamma() - GetDelta();
}

float Encoder::GetTX(){
	return (sqrt((pow(ArmLength, 2) + pow(StartDistance, 2) - 2 * ArmLength*StartDistance*cos(GetDelta()))));
}

float Encoder::GetDistance(){

	float angle = GetAngle();
	float chord = GetChord();
	float beta = GetBeta();
	float delta = GetDelta();
	float gamma = GetGamma();//(180 - beta) / 2;
	float epsilon = GetEpsilon();// 360 - gamma - delta;
	
	
	float distance = sqrt(pow(chord, 2) + pow(StartDistance, 2) - 2 * chord*StartDistance*cos(ToRad(epsilon)));

	// printing consistent with MatLab script
	//Serial.print(chord); // chord
	//Serial.print(",");
	//Serial.print(beta); // beta
	//Serial.print(",");
	//Serial.print(gamma); // gamma
	//Serial.print(",");
	//Serial.print(epsilon); // epsilon
	//Serial.print(",");
	Serial.print(distance); // distance
	Serial.print("\n");



	/*Serial.print("Distance: ");
	Serial.print(distance);
	Serial.print("  ");
	Serial.print("Arnm length: ");
	Serial.print(ArmLength);
	Serial.print("  ");
	Serial.print("Enc angle: ");
	Serial.print(angle);
	Serial.print("  ");
	Serial.print("Start angle: ");
	Serial.print(StartAngle);
	Serial.print("  ");
	Serial.print("Beta: ");
	Serial.print(beta);
	Serial.print("  ");
	Serial.print("Start head angle: ");
	Serial.print(RelativeStartHeadAngle);
	Serial.print("  ");
	Serial.print("Start distance: ");
	Serial.print(StartDistance);
	Serial.print("  ");
	Serial.print("Delta: ");
	Serial.print(delta);
	Serial.print("  ");
	Serial.print("Gamma: ");
	Serial.println(gamma);
	Serial.print("  ");
	Serial.print("Epsilon: ");
	Serial.print(epsilon);
	Serial.print("  ");
	Serial.print("C: ");
	Serial.println(chord);*/
	
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
}

void Encoder::SetRelativeStartHeadAngle(){
	RelativeStartHeadAngle = (float)GetHeadAngle();
}

void Encoder::SetRawHeadAngle(float angle){
	RawHeadAngle = angle;	
}

