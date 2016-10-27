//**************************************************************//
//  Name    : temposchild                                       //
//  Author  : Karsten Schlachter                                //
//  Date    : Oktober 2016                                      //
//  Version : 1.0                                               //
//  Notes   : Steuerung fuer Anzeigetafel Modelblitzer          //
//****************************************************************


int gndPin=2;//(port d)
int latchPin = 5;
int clockPin = 11;
int dataPin = 7;
const int digit1Pin=8;
const int digit2Pin=9;
const int digit3Pin=10;
const int CLEMENS=50;//vmax(cmax) ab hier rot anzeigen
const long TIMEOUT=5000; 

//holders for infromation you're going to pass to shifting function
byte dataRED;
byte dataGREEN;
byte digitArray[11];

void setup() {
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(digit2Pin,OUTPUT);
  pinMode(digit1Pin,OUTPUT);
  pinMode(gndPin,OUTPUT);
  digitalWrite(gndPin,LOW);
  DDRD=0xFF;
  Serial.begin(9600);

  //mapping fuer digitalanzeige...10=alles aus
  //1 nach links geshiftet, um nur die ausgaenge auf einer seite des shiftregisters (1-7) verwendne zu koennen
  digitArray[0] = 0b00111111<<1; 
  digitArray[1] = 0b00000110<<1;
  digitArray[2] = 0b01011011<<1; 
  digitArray[3] = 0b01001111<<1;
  digitArray[4] = 0b01100110<<1;
  digitArray[5] = 0b01101101<<1;
  digitArray[6] = 0b01111101<<1;
  digitArray[7] = 0b00000111<<1; 
  digitArray[8] = 0b01111111<<1;
  digitArray[9] = 0b01101111<<1;
  digitArray[10] = 0b00000000; 

}

void loop() {
static long lastDataTime=0;
static byte speedval;
static byte mask;
static int meassureCount=0;
static byte vmax=0;

  if(Serial.available()){
    speedval=Serial.read();
  //  Serial.println(speedval);
    lastDataTime=millis();
        meassureCount++;
    if(counter2>vmax) vmax=counter2;
    if(vmax>199) vmax=199;
  }

mask=0b01<<4;
  for (int j = 0; j < 3; j++) {
   
    
    int digit=((int)(speedval/(pow(10,j)))) % 10; 

    if(j==2 && digit==0){
      digit=10;//bei 0 wird bei 3ter stelle nichts angezeigt
    }

    dataRED = digitArray[digit]^0xFF;//&mask;
    
    dataGREEN = digitArray[digit]^0xFF;

    if((millis()-lastDataTime)>TIMEOUT){
      dataGREEN = digitArray[10]^0xFF;//&mask;
      dataRED = digitArray[10]^0xFF;//&mask;
    }else if(speedval>=80){
      dataGREEN = digitArray[10]^0xFF;//&mask;
    }else{
      dataRED = digitArray[10]^0xFF;//&mask;
    }
    
    //latch pin waehrend uebertragung auf low
    digitalWrite(latchPin, 0);

    shiftOut(dataPin, clockPin, dataRED);
    shiftOut(dataPin, clockPin, dataGREEN);
   
    digitalWrite(latchPin, 1);
    
      PORTB=mask;
      

    
    mask=mask<<1;

  }

}


//TODO: shiftout code aus arduino bsp...durch eigenen code ersetzen

//**************************************************************//
//  Name    : shiftOutCode, Predefined Dual Array Style         //
//  Author  : Carlyn Maw, Tom Igoe                              //
//  Date    : 25 Oct, 2006                                      //
//  Version : 1.0                                               //
//  Notes   : Code for using a 74HC595 Shift Register           //
//          : to count from 0 to 255                            //
//****************************************************************

void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  // This shifts 8 bits out MSB first,
  //on the rising edge of the clock,
  //clock idles low

  //internal function setup
  int i = 0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);

  //clear everything out just in case to
  //prepare shift register for bit shifting
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);

  //for each bit in the byte myDataOut�
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights.
  for (i = 7; i >= 0; i--)  {
    digitalWrite(myClockPin, 0);

    //if the value passed to myDataOut and a bitmask result
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000
    // and proceeds to set pinState to 1.
    if ( myDataOut & (1 << i) ) {
      pinState = 1;
    }
    else {
      pinState = 0;
    }

    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(myDataPin, pinState);
    //register shifts bits on upstroke of clock pin
    digitalWrite(myClockPin, 1);
    //zero the data pin after shift to prevent bleed through
    digitalWrite(myDataPin, 0);
  }

  //stop shifting
  digitalWrite(myClockPin, 0);
}


