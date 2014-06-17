#include "Wire.h"
//#include "LiquidCrystal.h"
#include <ClickEncoder.h>
#include <TimerOne.h>
//LiquidCrystal lcd(10, 9, 5, 6, 7, 8);
#include <LiquidCrystal_SR.h>

LiquidCrystal_SR lcd(9,8,10);
//LiquidCrystal lcd(10, 9, 5, 6, 7, 8);
ClickEncoder *encoder;

void timerIsr() {
  encoder->service();
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

enum enumestado { 
  STAP, WARM };
enumestado estado=STAP;
unsigned long value,pas;
unsigned long t0=0;
unsigned long dt=0;
void loop() {
  switch(estado){
  case STAP:
    {
      value += encoder->getValue();
      positivo(value);
      if(value!=pas){
        pas=value;
        dt=value/4;
      }
      ClickEncoder::Button b = encoder->getButton();
      if (b != ClickEncoder::Open) {
        if(b==ClickEncoder::Clicked){
          digitalWrite(13,HIGH);
          t0=millis();
          value=0;
          estado=WARM;
        }
      }
      lcd.setCursor(0,0);
      lcd.print("Tiempo");
      lcd.setCursor(0,1);
      lcd.print("                ");
      lcd.setCursor(0,1);
      lcd.print(dt);
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

void positivo(unsigned long& value){
  if(value==0xFFFFFFFF){
    value=0;
  }
}

void pantalla(float num,int dec, int fila){
  
}
