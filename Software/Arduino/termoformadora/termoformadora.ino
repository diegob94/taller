#include "Wire.h"
#include "LiquidCrystal.h"
#include <ClickEncoder.h>
#include <TimerOne.h>
//LiquidCrystal lcd(10, 9, 5, 6, 7, 8);

LiquidCrystal lcd(10, 9, 5, 6, 7, 8);
ClickEncoder *encoder;
unsigned long value,pas;

void timerIsr() {
  encoder->service();
}

void displayAccelerationStatus() {
  lcd.setCursor(0, 1);  
  lcd.print("Acceleration ");
  lcd.print(encoder->getAccelerationEnabled() ? "on " : "off");
}

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  pinMode(13,OUTPUT);
  encoder = new ClickEncoder(A1, A2, A0);
  encoder->setAccelerationEnabled(false);
  Timer1.initialize(100);
  Timer1.attachInterrupt(timerIsr); 
}

float tiempo=0;
enum enumestado { 
  STAP, WARM };
enumestado estado=STAP;
unsigned long t0=0;
unsigned long dt=0;
void loop() {
  switch(estado){
  case STAP:
    {
      value += encoder->getValue();
      if(value!=pas){
        pas=value;
        if(value > 0){
          tiempo=float(value/4)/1.0;
        }
        else {
          value=0;
          tiempo=0;
        }
      }
      ClickEncoder::Button b = encoder->getButton();
      if (b != ClickEncoder::Open) {
        if(b==ClickEncoder::Clicked){
          digitalWrite(13,HIGH);
          t0=millis();
          dt=(unsigned long)tiempo*1000;
          Serial.println(dt);
          value=0;
          tiempo=0.0;
          estado=WARM;
        }
      }
      lcd.setCursor(0,0);
      lcd.print("Tiempo");
      lcd.setCursor(0,1);
      lcd.print("                ");
      lcd.setCursor(0,1);
      lcd.print(tiempo);
      break;
    }
  case WARM:
    {
      if(millis()-t0<dt){
        lcd.setCursor(0,0);
        lcd.print("Tiempo");
        lcd.setCursor(0,1);
        lcd.print("                ");
        lcd.setCursor(0,1);
        lcd.print(float(dt-millis()+t0)/1000.0);
        ClickEncoder::Button b = encoder->getButton();
        if (b != ClickEncoder::Open) {
          if(b==ClickEncoder::Clicked){
            digitalWrite(13,LOW);
            dt=0;
            estado=STAP;
          }
        }
      } 
      else {
        digitalWrite(13,LOW);
        dt=0;
        estado=STAP;
      }
      break;
    }
  }
}

float roundf(float f,float pres){
    return (float) (floor(f*(1.0f/pres) + 0.5)/(1.0f/pres));
}
void pantalla(float num,int dec, int fila){
  
}
