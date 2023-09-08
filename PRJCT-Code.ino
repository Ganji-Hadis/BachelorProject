//BH1750
#include <Wire.h>
#include <BH1750.h>
BH1750 lightMeter;

//DHT11
#include <dht.h>  
#define outPin 6  
dht DHT;  

//Ultrasonic 1,2
const int TRIG_PIN1 = 11;
const int ECHO_PIN1 = 12;
const int TRIG_PIN2 = 3;
const int ECHO_PIN2 = 5;

//Relay
int RelayPin = 4;

////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  //BH1750
  Wire.begin();
  lightMeter.begin();
  Serial.println(F("BH1750 sampling..."));

  //DHT11
  Serial.println("DHT11 Sampling...");
  Serial.println("Sampling results are:");

  //LEDs
  pinMode(9, OUTPUT); //Red
  pinMode(10, OUTPUT); //Blue

  //Ultrasonic
  pinMode(TRIG_PIN1, OUTPUT);
  pinMode(ECHO_PIN1, INPUT);
  pinMode(TRIG_PIN2, OUTPUT);
  pinMode(ECHO_PIN2, INPUT);

  //Relay
  pinMode(RelayPin, OUTPUT);
  digitalWrite(RelayPin, HIGH);
}

//////////////////////////////////////////////////////////////////////

void loop() {
  
//BH1750
  float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");

 
//DHT11
  int readData = DHT.read11(outPin);
  float t = DHT.temperature;
  Serial.print("Temperature = ");
  Serial.print(t);
  Serial.println("°C  ");


//Ultrasonic
  long duration1, distanceCm1, duration2, distanceCm2;
  
  digitalWrite(TRIG_PIN1, LOW);
  delayMicroseconds(4);
  digitalWrite(TRIG_PIN1, HIGH);
  delayMicroseconds(12);
  digitalWrite(TRIG_PIN1, LOW);
  duration1 = pulseIn(ECHO_PIN1, HIGH);
  
  digitalWrite(TRIG_PIN2, LOW);
  delayMicroseconds(4);
  digitalWrite(TRIG_PIN2, HIGH);
  delayMicroseconds(12);
  digitalWrite(TRIG_PIN2, LOW);
  duration2 = pulseIn(ECHO_PIN2, HIGH);

  //convert the time into a distance
  distanceCm1 = duration1 * 0.034 / 2;
  distanceCm2 = duration2 * 0.034 / 2;
  
  Serial.print("distance1: ");
  Serial.print(distanceCm1);
  Serial.println("cm");
  Serial.print("distance2: ");
  Serial.print(distanceCm2);
  Serial.println("cm");
  delay(100);

  
//Control codes  
  if (distanceCm1 >= 150 or distanceCm2 >= 150 or distanceCm1 == 0 or distanceCm2 == 0 or lux > 350 ) {
    // Let's turn off the relay And turn on LED...
    if(distanceCm1 >= 100 or distanceCm2 >= 150 or distanceCm1 == 0 or distanceCm2 == 0){
      Serial.println("Out of range,Turn off the lamp. Red LED will be on: 10/3sec");
      digitalWrite(RelayPin, HIGH);
      digitalWrite(10, HIGH);
      delay(300); 
      digitalWrite(10, LOW);
      delay(300);
    }
    if(lux > 350){
      Serial.println("Enough light,Turn off the lamp.");
      digitalWrite(RelayPin, HIGH);
      if(lux > 500){
        Serial.println("Too much light,Turn off the lamp. Blue LED will be on:10/3sec");
        digitalWrite(9, HIGH);
        delay(300);
        digitalWrite(9, LOW);
        delay(300); 
      }
    }
  }
  else if(t > 30){
    digitalWrite(10, HIGH);
    delay(3000);
    digitalWrite(10, LOW);
    delay(3000);
    if(lux < 300){
    Serial.println("In rane. But low light so turn on the lamp. And it is warm: 1/3sec");
    // Let's turn on the relay and turn off LEDs
    digitalWrite(RelayPin, LOW);
    }
    else{
      Serial.println("In rane and enough light.But it is warm: 1/3sec");
    }
  }
  else if (t <= 23) {
    digitalWrite(9, HIGH);
    delay(3000);
    digitalWrite(9, LOW);
    delay(3000);
   if(lux < 300){
    Serial.println("In rane. But low light so turn on the lamp. And it is cold: 1/3sec");
    // Let's turn on the relay and turn off LEDs
    digitalWrite(RelayPin, LOW);
    }
    else{
      Serial.println("In rane and enough light.But it is cold: 1/3sec");
    }
  }
  else if(lux < 300){
    Serial.println("In rane.But low light so turn on the lamp. Also it is the best tempreture.");
    // Let's turn on the relay and turn off LEDs
    digitalWrite(RelayPin, LOW);
  }
  else{
    Serial.println("In rane and enough light. Also it is the best tempreture.");
    }

  delay(1000);
}
