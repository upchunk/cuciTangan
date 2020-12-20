/* Example code for HC-SR04 ultrasonic distance sensor with Arduino. No library required. More info: https://www.makerguides.com */
#include <TM1637Display.h>
#include <DHT.h>
#include <NewPing.h>

// Define
#define trigPin 1
#define echoPin 0
#define CLK 3
#define DIO 4
#define DHTPin 5
#define DHTType DHT11
#define pinIR 2
#define pinLed1 25
#define pinLed2 26
#define maxDist 400

NewPing sonar(trigPin, echoPin, maxDist);

// Define variables:
float hum;
float temp;
float duration;
float distance;
float soundsp;
float soundcm;
int timer = 0;
int sensorState;
int iterations = 3;

DHT dht(DHTPin, DHTType);

TM1637Display display(CLK, DIO);
uint8_t data[] = {0, 0, 0, 0};

void setup() {
  // Define inputs and outputs:
  pinMode(pinIR, INPUT);
  pinMode(pinLed1, OUTPUT);
  pinMode(pinLed2, OUTPUT);
  dht.begin();
  display.setBrightness(0x0f);
}

void loop() {
  //Setup TM1637 Display
  data[3] = display.encodeDigit(timer/1 % 10);
  data[2] = display.encodeDigit(timer/10 % 10);
  data[1] = display.encodeDigit(timer/100 % 10);
  data[0] = display.encodeDigit(timer/1000 % 10);
  display.setSegments(data);
  
  hum = dht.readHumidity();
  temp = dht.readTemperature();

  // Calculate the speed of sound in M/S
  soundsp = 331.4 + (0.606 * temp) + (0.0124 * hum);
  // Convert to cm/ms
  soundcm = soundsp/10000;
  
  duration = sonar.ping_median(iterations);
  // Calculate the distance:
  distance = (duration/2) * soundcm;

  //read IR Sensor State
  sensorState = digitalRead(pinIR);
  if (distance <= 20){
     if (sensorState == LOW) {
       if (timer >= 0 && timer < 5){
        digitalWrite(pinLed1, LOW);
        digitalWrite(pinLed2, HIGH);
       }
       if (timer >= 5 && timer < 8){
        digitalWrite(pinLed1, HIGH);
        digitalWrite(pinLed2, LOW);
       }
       if (timer >= 8 && timer < 15){
        digitalWrite(pinLed1, LOW);
        digitalWrite(pinLed2, LOW);
       }
       if (timer >= 15 && timer < 20){
        digitalWrite(pinLed1, LOW);
        digitalWrite(pinLed2, HIGH);
       }
       if (timer >= 20){
        digitalWrite(pinLed1, LOW);
        digitalWrite(pinLed2, LOW);
        timer = 0;
        delay (3000);
       }
      delay(1000);
      timer++;
     }
  }
  else{
    digitalWrite(pinLed1, LOW);
    digitalWrite(pinLed2, LOW);
    delay(1000);
    timer = 0;
  }
}
