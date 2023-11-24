#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_NeoPixel.h>

const int triggerPin = 12;
const int echoPin = 13;

long duration;
int distance;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN  150 
#define SERVOMAX  600 
#define SERVO_FREQ 50 
#define SLOW_SPEED_PULSE 285 

// Air pump control pin and timer
const int airValvePin = 10; 
const unsigned long inflationTime = 10000; 
const unsigned long inflationPauseTime = 3000; 
bool isPumping = false; 


#define PIN 5
#define NUMPIXELS 72 // There are six 12-pixel LED rings
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void flashWhite() {
  // White light and dark
  fadeToBright(10, 255, 255, 255); 
  fadeToDim(10, 255, 255, 255); 
}

void flashRed() {
  // Red light and dark
  fadeToBright(6, 255, 0, 0); 
  fadeToDim(6, 255, 0, 0); 
}

void setup() {
  Serial.begin(9600);
  
  
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // Initializing PWM actuator drivers
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);

  // Set the air pump control pin to output and turn off the air pump
  pinMode(airValvePin, OUTPUT);
  digitalWrite(airValvePin, LOW);

  pixels.begin(); 

  delay(10);
}

void loop() {
  
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
 
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2; 

  // Print distance to serial monitor
  Serial.println(distance);
 
  int wait = 50; 
  uint8_t redIntensity = 255; 
  uint8_t greenIntensity = 0; 
  uint8_t blueIntensity = 0; 

    // Controls servos and air pumps according to distance
  if (distance < 50) {
    for (int i = 0; i < 5; i++) { // This loop performs 5 fades of light and dark.
    fadeToBright(wait, redIntensity, greenIntensity, blueIntensity); 
    fadeToDim(wait, redIntensity, greenIntensity, blueIntensity); 
    }
    // If the distance is less than 50 cm, rotate the servo
    for (uint8_t i = 0; i < 8; i++) {
      pwm.setPWM(i, 0, SLOW_SPEED_PULSE);
    }
    delay(15); 
    // 同时开启气泵
    for (uint8_t i = 0; i < 8; i++) {
      pwm.setPWM(i, 0, SLOW_SPEED_PULSE);
    }
    if(!isPumping){ // If the air pump is not running, start the air pump
      digitalWrite(airValvePin, HIGH); 
      isPumping = true;
      delay(inflationTime); 
      digitalWrite(airValvePin, LOW); 
      isPumping = false;
      delay(inflationPauseTime); // Pause time after inflation
    }
  } 
  else {
    flashWhite(); // If the distance is greater than 30 cm, the white colour fades in and out.

    for (uint8_t i = 0; i < 8; i++) {
      pwm.setPWM(i, 300, SERVOMIN); // Here SERVOMIN is the minimum value of the servo pulse
    }
    digitalWrite(airValvePin, LOW); // close pump
    
    delay(100); // Reduced detection frequency
  }
}

void fadeToBright(int wait, uint8_t red, uint8_t green, uint8_t blue) {
  for(int brightness = 0; brightness < 256; brightness++) {
    for(int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(red * brightness / 255, green * brightness / 255, blue * brightness / 255)); 
    }
    pixels.show();
    delay(1);
  }
}

void fadeToDim(int wait, uint8_t red, uint8_t green, uint8_t blue) {
  for(int brightness = 255; brightness >= 0; brightness--) {
    for(int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(red * brightness / 255, green * brightness / 255, blue * brightness / 255)); 
    }
    pixels.show();
    delay(1);
  }
  
}
