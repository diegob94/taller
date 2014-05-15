#include <Wire.h>
#include <LiquidCrystal_SR.h>

// constructor prototype parameter:
//  LiquidCrystal_SR lcd(DataPin, ClockPin, EnablePin);
LiquidCrystal_SR lcd(8,9,7);
#define ENC_A 14
#define ENC_B 15
#define ENC_PORT PINC

void setup(){
  pinMode(ENC_A,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  pinMode(ENC_B,INPUT_PULLUP);
  attachInterrupt(1, boton_ISR, RISING);
  lcd.begin(16,2);
  //lcd.createChar (0, armsUp);    // load character to the LCD
  lcd.home();
}

void loop(){
  lcd.home();
  lcd.print("LiquidCrystal_SR");
  static uint8_t counter = 0;      //this variable will be changed by encoder input
  int8_t tmpdata;
  tmpdata = read_encoder();
  if( tmpdata ) {
    Serial.print("Counter value: ");
    Serial.println(counter/4, DEC);
    counter += tmpdata;
  }
}

int8_t read_encoder(){
  static int8_t enc_states[] = {
    0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0  };
  static uint8_t old_AB = 0;
  old_AB <<= 2;                   //remember previous state
  old_AB |= ( ENC_PORT & 0x03 );  //add current state
  return ( enc_states[( old_AB & 0x0f )]);
}

volatile int b=0;
void boton_ISR(){
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 500) {
    b=1;
  }
  last_interrupt_time = interrupt_time;
}

byte armsUp[8] = {
  0b00100,
  0b01010,
  0b00100,
  0b10101,
  0b01110,
  0b00100,
  0b00100,
  0b01010
};

