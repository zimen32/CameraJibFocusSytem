#include <Wire.h>
#include <L3G.h> 
#include <LSM303.h>
#include <SD.h>

File myFile;

L3G gyro;
LSM303 compass;

double avgRateXY;
double avgRateZY;
double avgRateXZ;
int avgCounter;
long prevTime;

long startTime;
long prevStartTime;
long stopTime;
long absoulteStartTime;

boolean done = false;

const int timeThreshold = 1000;
const double sensitivity = 70;
int counter = 0;



void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  pinMode(53, OUTPUT);
  pinMode(41, OUTPUT);
  
  digitalWrite(41, HIGH);

  if (!gyro.init())
  {
    Serial.println("Failed to autodetect gyro type!");
    while (1);
  }

  gyro.enableDefault();     
  compass.init();
  compass.enableDefault();
  
  //if (!SD.begin(53)) {
   // Serial.println("initialization failed!");
    //return;
 // }
  Serial.println("initialization done.");  
 // SD.remove("LINEAR.txt");
  //myFile = SD.open("LINEAR.txt", FILE_WRITE);
    
  gyro.read();
  compass.read();
  delay(15000);
  digitalWrite(41, LOW);
  
  absoulteStartTime = millis();
  
  startTime = millis();
  prevStartTime = millis();
  stopTime = millis();
}

void loop() {
  
  stopTime = millis();  
  char data[100] ;
  
  if(millis() - absoulteStartTime < 60000)
  {
    if(stopTime - startTime > 20)
    {
      startTime = millis();
      
      gyro.read();
      compass.read();
      
      double rawX = (double)gyro.g.x;
      double rawY = (double)gyro.g.y;
      double rawZ = (double)gyro.g.z;
  
      double rateX = (rawX - 10) * 8.75 / 1000.0;
      double rateY = (rawY - 10) * 8.75 / 1000.0;  
      double rateZ = (rawZ - 10) * 8.75 / 1000.0;      
      
      sprintf(data,"%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%d",counter,rateX, rateY, rateZ, (compass.a.x / 16000.0), (compass.a.x / 16000.0), (compass.a.x / 16000.0), (compass.m.x / 1055.0), (compass.m.x / 1055.0), (compass.m.x / 950.0), (stopTime - prevStartTime));
      
      Serial.println(data);

     
          
      Serial.println(data);
      
      prevStartTime = startTime;
      
      counter+=1;
      
    }
  }
  else
  {
    if (!done)
    {
      //myFile.close();
      digitalWrite(41, HIGH);
      done = true;
    }
  }

    
  
   
}
