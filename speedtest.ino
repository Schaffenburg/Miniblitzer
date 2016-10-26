const int gndPIN=2;
const int radarPIN=7;
const int flashPIN=5;
const int CLEMENS = 30;
const unsigned long TIMEOUT=2000015000;

//frequenbestimmung mit singalgenerator getestet...min bis 20khz brauchbar (hoeher nicht probiert)
void setup() {
  // put your setup code here, to run once:
pinMode(radarPIN,INPUT);
pinMode(gndPIN,OUTPUT);
pinMode(flashPIN,OUTPUT);
digitalWrite(gndPIN,LOW);
Serial.begin(9600);

}


unsigned long duration;
unsigned long oldduration=0;
unsigned long freq;
unsigned long v;
unsigned long lastFlash;

byte vkmh;
#define VMAX 30
#define LAMBDA 0.0321

unsigned long getDuration(){

  unsigned long dur=0;
  unsigned long buff;
      for(int i=0;i<2;i++){
            buff=pulseIn(radarPIN,LOW,TIMEOUT);// 10ms timeout
  if(buff==0) return 0;
  //Serial.println(buff);
      dur+=buff;
      
      buff= pulseIn(radarPIN,HIGH,TIMEOUT);
      if(buff==0) return 0;//Serial.println(buff);
      dur+=buff;
     
    }
  // Serial.println(dur);
    return dur/2;
    
  
}

void loop() {

  if((millis()-lastFlash)>100){
    digitalWrite(flashPIN,LOW);
  }
  
  // kein 50/50, sondern variierender "duty cyle", daher high und low messen
  duration=pulseIn(radarPIN,LOW,TIMEOUT);
//  duration+=pulseIn(7,HIGH,TIMEOUT);
  if(duration!=0){
 duration=getDuration();
   /* if(oldduration!=0){
    duration=(oldduration*3+duration)/4;  
    }
    oldduration=duration;*/
  }
 
  if(duration){


//  duration/=20;
    freq=1000000/duration;
    vkmh=LAMBDA/4*freq*3.6*16;
  //  vkmh=v*3.6;
  //duration=digitalRead(7);
 // if(vkmh!=0 && vkmh<1200)
  Serial.write(vkmh);
 // Serial.println(vkmh);
  if(vkmh>CLEMENS && (millis()-lastFlash)>5000){

    lastFlash=millis();
    digitalWrite(flashPIN,HIGH);
  }
  
  }
//delay(500);
}
