#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#define MQ3pin 0
#define ECHO 13
#define TRIGGER 12
#define PULS_TRIGGER 1
unsigned long time;
int switch_1 = 0;
int buzzerPin = 8;
float temp = 0;
int ECHO_DURATION = 0;
float CM_DIST;
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);
float sensorValue = 0;
const int LED_PIN1 =  11;
const int LED_PIN2 =  A3;
const int BUTTON_PIN = 9;
const int BUTTON_PIN2 = 2;
const int LED_PIN =  10; 
int buttonState = 0;
int timer;
int buttonState_mode = 0;
float aer;
int citit;

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  time = 0;
  timer = 0;
  aer = analogRead(MQ3pin);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUTTON_PIN2, INPUT_PULLUP);
  Serial.begin(9600);
  display.begin();
  display.setContrast(56);
  citit = 0;
}

void loop(){
  //implementare ruleta electrica
  buttonState_mode = digitalRead(BUTTON_PIN2);
  timer = 100;
  if(buttonState_mode == LOW){
    while(timer){
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Distance:");
    digitalWrite(TRIGGER, HIGH);
    delay(PULS_TRIGGER); 
    digitalWrite(TRIGGER, LOW);
    ECHO_DURATION= pulseIn(ECHO,HIGH);
    CM_DIST = (ECHO_DURATION/2)*0.0346;
    if(CM_DIST < 0){
      CM_DIST = 0;
    }
    display.setCursor(60,0);
    display.println(int(CM_DIST));
    display.println("CM");
    display.display();
    display.clearDisplay();
    timer = timer - 1;
    int power = 1000 - (CM_DIST * 10);
    if (power < 0) {
      noTone(buzzerPin);
    } else{
    tone(buzzerPin, power);
    }
    delay(300);
    noTone(buzzerPin);
    }
  }
  //implementare etilotest
  buttonState = digitalRead(BUTTON_PIN);  
  if(time <= 100){
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("The sensor is warming up!");
  display.setCursor(0,20);
  display.println("Have patience.");
  display.setCursor(30,30);
  display.println(time);
  display.setCursor(48,30);
  display.println("%");
  display.display();
  display.clearDisplay();
  time = time + 1;
  delay(100);
  } else {
      if(citit == 0){
        display.clearDisplay();
        digitalWrite(LED_PIN1, LOW);
        digitalWrite(LED_PIN2, LOW);
        display.setCursor(0,0);
        display.println("Press the button [RIGHT]");
        display.setCursor(0,20);
        display.println("Breathe while the led is blinking blue..");
        display.display();
      }
     if(buttonState == LOW){
        display.clearDisplay();
        citit = 1;       
         for (int i = 0; i < 10; i++) {
          delay(1000);
          temp = analogRead(MQ3pin) - aer;
          if(temp < 30){
            temp = 0;
          }
          display.setCursor(0,0);
          display.println("Breathe close to the device.");
          digitalWrite(LED_PIN, HIGH);
          sensorValue = sensorValue + temp;
          display.display();
          }
        sensorValue = sensorValue / 10;
        digitalWrite(LED_PIN, LOW);      
  }
    if(citit == 1) {
        float val_aer = sensorValue / 550;
         float val_sange = val_aer * 2.1;
          if(val_aer >= (float)0.4){
            digitalWrite(LED_PIN1, LOW);
            digitalWrite(LED_PIN2, HIGH);
            } else {
            digitalWrite(LED_PIN1, HIGH);
            digitalWrite(LED_PIN2,LOW);
              }
        display.clearDisplay();
        display.setCursor(0,0);
        display.println("PPM:");
        display.setCursor(25,0);
        display.println(sensorValue);
        display.display();
        delay(5000);
        display.clearDisplay();

        display.setCursor(0,0);
        display.println("Aer: ");
        display.setCursor(30,0);
        display.println(val_aer);
        display.setCursor(60,0);
        display.println("mg/L");
        display.display();
        delay(5000);
        display.clearDisplay();

        display.setCursor(0,0);
        display.println("Sange: ");
        display.setCursor(35,0);
        display.println(val_sange);
        display.setCursor(65,0);
        display.println("g/L");
        display.display();
        
        delay(5000);
        display.clearDisplay();
        display.setCursor(0,0);
        display.println("Alcool Pur:");
        display.setCursor(0,20);
        float x = val_aer * (float)100;
        display.println((int)x);
        display.setCursor(25,20);
        display.println("ml");
        display.display();
        delay(5000);
        display.clearDisplay();
        display.setCursor(0,0);
        display.println("Timp:");
        display.setCursor(30,0);
        display.println(x / 12.5);
        display.setCursor(60,0);
        display.println("ore");
        display.display();
        delay(5000);
        citit = 0;
        }
     }
  } 

  