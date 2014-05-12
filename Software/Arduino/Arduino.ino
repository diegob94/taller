volatile boolean uno=false;
volatile boolean dos=false;
volatile boolean last=false;
volatile boolean lasto=false;
volatile int lol=0;
int cont=0;

void setup(){
  Serial.begin(9600);
  Serial.setTimeout(100);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  //Usar GND
  attachInterrupt(0, cambio, CHANGE);
  attachInterrupt(1, cambio, CHANGE);
  lol=digitalRead(2);
}

void loop(){
  if (lol){
    if(lol==2){
      lol=-1;
    }
    cont+=lol;
    Serial.println(cont);
    lol=0;
    /*Debug
    Serial.println(lol);
    lol=0;
    Serial.print("-");
    Serial.print(uno);
    Serial.print(" ");
    Serial.println(dos);*/
  }
}

void cambio(){
  uno=digitalRead(2);
  dos=digitalRead(3);
  if(uno==dos && uno==true){
    if (last && !lasto){
      lol=1;
    }else if(!last && lasto){
      lol=2;
    }
  }
  last=uno;
  lasto=dos;
}
