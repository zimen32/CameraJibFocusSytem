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

//calibration
int sampleNum = 1000;
double dc_offset_x = 0.0;
double dc_offset_y = 0.0;
double dc_offset_z = 0.0;

double noise_x = 0;
double noise_y = 0;
double noise_z = 0;

boolean done = false;

const int timeThreshold = 1000;
const double sensitivity = 70;
int counter = 0;

void setup() {
  Serial.begin(115200);
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
  
//  if (!SD.begin(53)) {
//    Serial.println("initialization failed!");
//    return;
//  }
  //Serial.println("initialization done.");  
 // SD.remove("LINEAR.txt");
  //myFile = SD.open("LINEAR.txt", FILE_WRITE);
    
    for(int i = 0; i < sampleNum; ++i)
    {
      gyro.read();
    
      dc_offset_x += gyro.g.x;
      dc_offset_y += gyro.g.y;
      dc_offset_z += gyro.g.z;
    }
    
    dc_offset_x /= sampleNum;
    dc_offset_y /= sampleNum;
    dc_offset_z /= sampleNum;
  
    for(int i=0; i < sampleNum; i++){
      
     gyro.read();    
     
      //noise for X axis
      if((int)gyro.g.x - dc_offset_x > noise_x)      
        noise_x = gyro.g.x-dc_offset_x;
        
      else if((int)gyro.g.x-dc_offset_x < - noise_x)      
        noise_x = -gyro.g.x-dc_offset_x;
       
      //noise for Y axis 
      if(gyro.g.y - dc_offset_y > noise_y)      
        noise_y = gyro.g.y-dc_offset_y;
        
      else if(gyro.g.y-dc_offset_y < - noise_y)      
        noise_y = -gyro.g.y-dc_offset_y;
      
      //noise for Z axis
      if((int)gyro.g.z - dc_offset_z > noise_z)      
        noise_z = gyro.g.z-dc_offset_z;
        
      else if((int)gyro.g.z-dc_offset_z < - noise_z)      
        noise_z = -gyro.g.z-dc_offset_z;
    }
    
    noise_x=noise_x/100; 
    noise_y=noise_y/100;
    noise_z=noise_z/100;
    
    Serial.print("\nDC Offsets: \n");
    Serial.print(dc_offset_x);
    Serial.print(", ");
    Serial.print(dc_offset_y);
    Serial.print(", ");
    Serial.println(dc_offset_z);
    
    
    Serial.println("Noise Level: \n");
    Serial.print(noise_x);
    Serial.print(", ");
    Serial.print(noise_y);
    Serial.print(", ");
    Serial.println(noise_z);
    return;
 
    compass.read();
  
  
  

  digitalWrite(41, LOW);
  Serial.print("Packet number,Gyroscope X (deg/s),Gyroscope Y (deg/s),Gyroscope Z (deg/s),Accelerometer X (g),Accelerometer Y (g),Accelerometer Z (g),Magnetometer X (G),Magnetometer Y (G),Magnetometer Z (G)");
  absoulteStartTime = millis();
  
  startTime = millis();
  prevStartTime = millis();
  stopTime = millis();
}

void loop() {
  

  
  gyro.read();
  double ratex = (gyro.g.x-dc_offset_x)/100.0;
  
  if (!(ratex >= noise_x || ratex <= -noise_x))
    ratex = 0.0;
  
  double ratey = (gyro.g.y-dc_offset_y)/100.0;
  
  if (!(ratey >= noise_y || ratey <= -noise_y))
    ratey = 0.0;
    
  double ratez = (gyro.g.z-dc_offset_z)/100.0;
  
  if (!(ratez >= noise_z || ratez <= -noise_z))
    ratez = 0.0;
    
    
  Serial.print(ratex);
  Serial.print(", ");
  Serial.print(ratey);
  Serial.print(", ");
  Serial.println(ratez);

  delay(100);
  return;
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  stopTime = millis();   
  
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
      
      Serial.print(counter);
      Serial.print(",");
      Serial.print(rateX);
      Serial.print(",");
      Serial.print(rateY);
      Serial.print(",");
      Serial.print(rateZ);
      Serial.print(",");
      Serial.print(compass.a.x / 16000.0);
      Serial.print(",");
      Serial.print(compass.a.y / 16000.0);
      Serial.print(",");
      Serial.print(compass.a.z / 16000.0);
      Serial.print(",");
      Serial.print(compass.m.x / 1055.0);
      Serial.print(",");
      Serial.print(compass.m.y / 1055.0);
      Serial.print(",");
      Serial.print(compass.m.x / 950.0);
      Serial.print(",");
      Serial.println(stopTime - prevStartTime);  
              
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
