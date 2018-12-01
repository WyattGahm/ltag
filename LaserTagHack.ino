
#include <IRremote.h>
#define LEDBAR_PIN 4
#define TRIG_PIN 5
#define LIFE 6
#define LIFE_PIN1 9
#define LIFE_PIN2 10
#define LIFE_PIN3 11
#define WAITTIME 200
#define REVC_PIN A1
#define CENTERLED_PIN A0
#define BRIGHTNESS 200
#define RELOAD_PIN 8







IRsend irsend;
int buttonstate = 0;
int dead;
int reloading;
int life = LIFE;
unsigned long ldbt = 0;
int khz = 38; // 38kHz:NEC protocol
unsigned int irSignal[] = {
   1450 ,550 ,300 ,500 
  ,300 ,550 ,250 ,550 
  ,250 ,950 ,250 ,1000 
  ,200 ,1000 ,250 ,1000 
  ,200 ,600 ,250 ,550 
  ,250 ,600 ,250 ,550 
  ,650 ,550 ,300 ,550 
  ,250 ,550 ,250 ,950 
  ,650 ,600 ,250 ,550 
  ,250 ,950 ,300 ,550 
  ,250
};
void setup(){
  Serial.begin(9600);
  pinMode(TRIG_PIN,INPUT);
  pinMode(REVC_PIN,INPUT);
  pinMode(LIFE_PIN1,OUTPUT);
  pinMode(LIFE_PIN2,OUTPUT);
  pinMode(LIFE_PIN3,OUTPUT);
  pinMode(LEDBAR_PIN,OUTPUT);
  analogWrite(CENTERLED_PIN,BRIGHTNESS);
  analogWrite(3,255);
  delay(10);
  analogWrite(3,0);
}
void loop() {
  if(dead){
    analogWrite(CENTERLED_PIN,BRIGHTNESS);
    delay(750);
    analogWrite(CENTERLED_PIN,0);
    delay(750);
  }
  buttonstate = digitalRead(TRIG_PIN);
  if(buttonstate == HIGH && !dead){
    digitalWrite(LEDBAR_PIN,1);
    irsend.sendRaw(irSignal, sizeof(irSignal) / sizeof(irSignal[0]), khz);
    delay(80);
    irsend.sendRaw(irSignal, sizeof(irSignal) / sizeof(irSignal[0]), khz);
    delay(80);
    irsend.sendRaw(irSignal, sizeof(irSignal) / sizeof(irSignal[0]), khz);
    delay(80);
    irsend.sendRaw(irSignal, sizeof(irSignal) / sizeof(irSignal[0]), khz);
    digitalWrite(LEDBAR_PIN,0);
    ldbt = millis();
  }
  reloading = digitalRead(RELOAD_PIN);
  if(reloading==HIGH){
    life = LIFE;
    dead = 0;
    digitalWrite(CENTERLED_PIN,1);
  }
  int reading = analogRead(REVC_PIN);
  Serial.println(reading);
  
  if(reading < 700){
    if(millis() - ldbt > WAITTIME){
      ldbt = millis();
      life--;
    }
    //life--;
  }
  switch (life){
    case 0:
      dead = 1;
      analogWrite(LIFE_PIN1,0);
      break;
    case 1:
      analogWrite(LIFE_PIN1,128);
      break;
    case 2:
      analogWrite(LIFE_PIN1,255);
      break;
    case 3:
      analogWrite(LIFE_PIN1,255);
      analogWrite(LIFE_PIN2,128);
      break;
    case 4:
      analogWrite(LIFE_PIN1,255);
      analogWrite(LIFE_PIN2,255);
      break;
    case 5:
      analogWrite(LIFE_PIN1,255);
      analogWrite(LIFE_PIN2,255);
      analogWrite(LIFE_PIN3,128);
      break;
    case 6:
      analogWrite(LIFE_PIN1,255);
      analogWrite(LIFE_PIN2,255);
      analogWrite(LIFE_PIN3,255);
      break;
  }
  
      
}
