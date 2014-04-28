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
double offsetGyroX = 0.0;
double offsetGyroY = 0.0;
double offsetGyroZ = 0.0;

double offsetAccX = 0.0;
double offsetAccY = 0.0;
double offsetAccZ = 0.0;

double noiseGyroX = 0;
double noiseGyroY = 0;
double noiseGyroZ = 0;

double noiseAccX = 0;
double noiseAccY = 0;
double noiseAccZ = 0;

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
      compass.read();
    
      offsetGyroX += gyro.g.x;
      offsetGyroY += gyro.g.y;
      offsetGyroZ += gyro.g.z;
      
      offsetAccX += compass.a.x;
      offsetAccY += compass.a.y;
      offsetAccZ += compass.a.z;
    }
    
    offsetGyroX /= sampleNum;
    offsetGyroY /= sampleNum;
    offsetGyroZ /= sampleNum;
    
    offsetAccX /= sampleNum;
    offsetAccY /= sampleNum;
    offsetAccZ /= sampleNum;
  
    for(int i=0; i < sampleNum; i++){
      
     gyro.read();  
     compass.read();
     
      // Gyro noise 
       
      //noise for X axis
      if((int)gyro.g.x - offsetGyroX > noiseGyroX)      
        noiseGyroX = gyro.g.x-offsetGyroX;
        
      else if((int)gyro.g.x-offsetGyroX < - noiseGyroX)      
        noiseGyroX = -gyro.g.x-offsetGyroX;
       
      //noise for Y axis 
      if(gyro.g.y - offsetGyroY > noiseGyroY)      
        noiseGyroY = gyro.g.y-offsetGyroY;
        
      else if(gyro.g.y-offsetGyroY < - noiseGyroY)      
        noiseGyroY = -gyro.g.y-offsetGyroY;
      
      //noise for Z axis
      if((int)gyro.g.z - offsetGyroZ > noiseGyroZ)      
        noiseGyroZ = gyro.g.z-offsetGyroZ;
        
      else if((int)gyro.g.z-offsetGyroZ < - noiseGyroZ)      
        noiseGyroZ = -gyro.g.z-offsetGyroZ;
        
        
      // Acc noise    
      //noise for X axis      
      if((int)compass.a.x - offsetAccX > noiseAccX)      
        noiseAccX = compass.a.x-offsetAccX;
        
      else if((int)compass.a.x-offsetAccX < - noiseAccX)      
        noiseAccX = -compass.a.x-offsetAccX;
       
      //noise for Y axis 
      if(compass.a.y - offsetAccY > noiseAccY)      
        noiseAccY = compass.a.y-offsetAccY;
        
      else if(compass.a.y-offsetAccY < - noiseAccY)      
        noiseAccY = -compass.a.y-offsetAccY;
      
      //noise for Z axis
      if((int)compass.a.z - offsetAccZ > noiseAccZ)      
        noiseAccZ = compass.a.z-offsetAccZ;
        
      else if((int)compass.a.z-offsetAccZ < - noiseAccZ)      
        noiseAccZ = -compass.a.z-offsetAccZ;
    }
    
    noiseGyroX /= 100; 
    noiseGyroY /= 100;
    noiseGyroZ /= 100;
    
    noiseAccX /= 16000.0;
    noiseAccY /= 16000.0;
    noiseAccZ /= 16000.0;
    
//    Serial.print("\nDC Offsets: \n");
//    Serial.print(offsetGyroX);
//    Serial.print(", ");
//    Serial.print(offsetGyroY);
//    Serial.print(", ");
//    Serial.print(offsetGyroZ);
//    Serial.print(", ");
//    Serial.print(offsetAccX);
//    Serial.print(", ");
//    Serial.print(offsetAccY);
//    Serial.print(", ");
//    Serial.println(offsetAccZ);
//    
//    
//    Serial.println("Noise Level: \n");
//    Serial.print(noiseGyroX);
//    Serial.print(", ");
//    Serial.print(noiseGyroY);
//    Serial.print(", ");
//    Serial.print(noiseGyroZ);
//    Serial.print(", ");
//    Serial.print(noiseAccX);
//    Serial.print(", ");
//    Serial.print(noiseAccY);
//    Serial.print(", ");
//    Serial.println(noiseAccZ);
    
  digitalWrite(41, LOW);
  //Serial.print("Packet number,Gyroscope X (deg/s),Gyroscope Y (deg/s),Gyroscope Z (deg/s),Accelerometer X (g),Accelerometer Y (g),Accelerometer Z (g),Magnetometer X (G),Magnetometer Y (G),Magnetometer Z (G)");
  absoulteStartTime = millis();
  
  startTime = millis();
  prevStartTime = millis();
  stopTime = millis();
}

void loop() {
    
  stopTime = millis();   
  
  if(millis() - absoulteStartTime < 60000)
  {
    if(stopTime - startTime > 7)
    {
      startTime = millis();
      
      gyro.read();
      compass.read();
      
      double rateX = (gyro.g.x-offsetGyroX)/100.0;
  
      if (!(rateX >= noiseGyroX || rateX <= -noiseGyroX))
        rateX = 0.0;
      
      double rateY = (gyro.g.y-offsetGyroY)/100.0;
      
      if (!(rateY >= noiseGyroY || rateY <= -noiseGyroY))
        rateY = 0.0;
        
      double rateZ = (gyro.g.z-offsetGyroZ)/100.0;
      
      if (!(rateZ >= noiseGyroZ || rateZ <= -noiseGyroZ))
        rateZ = 0.0;         
      
      
      double accX = (compass.a.x )/ 16000.0;
      
      if (!(accX >= noiseAccX || accX <= -noiseAccX))
        accX = 0.0;
      
      double accY = (compass.a.y )/ 16000.0;
      
      if (!(accY >= noiseAccY || accY <= -noiseAccY))
        accY = 0.0;
      
      double accZ = (compass.a.z )/ 16000.0;
      
      if (!(accZ >= noiseAccZ || accZ <= -noiseAccZ))
        accZ = 0.0;
      
      
      
      Serial.print(counter);
      Serial.print(",");
      Serial.print(rateX);
      Serial.print(",");
      Serial.print(rateY);
      Serial.print(",");
      Serial.print(rateZ);
      Serial.print(",");
      Serial.print(accX);
      Serial.print(",");
      Serial.print(accY);
      Serial.print(",");
      Serial.print(accZ);
      Serial.print(",");
      Serial.print(compass.m.x / 1055.0);
      Serial.print(",");
      Serial.print(compass.m.y / 1055.0);
      Serial.print(",");
      Serial.println(compass.m.z / 950.0);
      //Serial.println(",");
     // Serial.println(stopTime - prevStartTime);  
              
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
