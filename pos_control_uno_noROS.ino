#include <SoftwareSerial.h>
#include <Metro.h>
#define encoder0PinA 2
#define encoder0PinB 3
#define MotorPin0 9
#define MotorPin1 10
int pinAState = 0;
int pinAStateOld = 0;
int pinBState = 0;
int pinBStateOld = 0;
int x=0;
int cmd0Pos=180; //degree
double angle0Pos = 0;
volatile long encoder0Pos = 0;
volatile long unknownvalue = 0;
Metro mainTimer = Metro(10);

void printDouble( double val, unsigned int precision){
// prints val with number of decimal places determine by precision
// NOTE: precision is 1 followed by the number of zeros for the desired number of decimial places
// example: printDouble( 3.1415, 100); // prints 3.14 (two decimal places)

   Serial.print (int(val));  //prints the int part
   Serial.print("."); // print the decimal point
   unsigned int frac;
   if(val >= 0)
       frac = (val - int(val)) * precision;
   else
       frac = (int(val)- val ) * precision;
   Serial.print(frac,DEC) ;
} 



void setup() { 
  pinMode(MotorPin0, OUTPUT);
  pinMode(MotorPin1, OUTPUT);

  
  pinMode(encoder0PinA, INPUT); 
  digitalWrite(encoder0PinA, HIGH); // turn on pullup resistor
  pinMode(encoder0PinB, INPUT); 
  digitalWrite(encoder0PinB, HIGH); // turn on pullup resistor
  
  attachInterrupt(0, doEncoder, CHANGE); // encoder pin on interrupt 0 - pin 2
  attachInterrupt(1, doEncoder, CHANGE);
  
  Serial.begin (57600);
  Serial.println("start"); // a personal quirk
} 

void loop(){
  if (mainTimer.check() == true) { 
    ////positive angle
    angle0Pos=-1*encoder0Pos*0.00694;//(double)360/(64*810*1);
    //angle0Pos=(double)((double)(360.0/64)/810);
    double cmdPwm=10*(cmd0Pos-angle0Pos);
    
    int vPlus=0,vMinus=0;
    if(cmdPwm>=0)
    {
      vPlus=cmdPwm;
      vMinus=0;
    }
    else if(cmdPwm<0)
     {
      vPlus=0;
      vMinus=-cmdPwm;
    }   
    
     if(vPlus>=255)
        vPlus=255;
     else if(vPlus<-255)
          vPlus=-255;
          
          
     if(vMinus>=255)
        vMinus=255;
     else if(vMinus<-255)
          vMinus=-255;      
    
    //Serial.print("error  ");Serial.print(cmd0Pos-angle0Pos); Serial.print("     vPlus  ");Serial.print(vPlus);Serial.print("     vMinus  ");  Serial.print(vMinus);    Serial.print("   Encoder: ");  Serial.print(encoder0Pos);      
    //Serial.print(" cmdPwm: ");Serial.println(cmdPwm, DEC);  
    
    analogWrite(MotorPin1,vPlus);
    analogWrite(MotorPin0,vMinus);
 
    Serial.print("  encoder0Pos ");Serial.print(encoder0Pos);Serial.print("  angle0Pos ");Serial.print(angle0Pos);  Serial.print("     vPlus  ");Serial.print(vPlus);Serial.print("     vMinus  "); Serial.println(vMinus); 





/////READ ENC
/*
    x=encoder0Pos;  
    char xS='S';
    byte xH =highByte(x);
    byte xL =lowByte(x);
    char xF='\0'; 
    Serial.print(xS);Serial.write(xH);Serial.write(xL);Serial.write(xF);
*/
    
    }
}

void doEncoder() {
    // encoder0Pos++;
    pinAState = digitalRead(2);
    pinBState = digitalRead(3);

    if (pinAState == 0 && pinBState == 0) {
    if (pinAStateOld == 1 && pinBStateOld == 0) // forward
    encoder0Pos ++;
    if (pinAStateOld == 0 && pinBStateOld == 1) // reverse
    encoder0Pos --;
    if (pinAStateOld == 1 && pinBStateOld == 1) // unknown
    unknownvalue ++;
    if (pinAStateOld == 0 && pinBStateOld == 0) // unknown
    unknownvalue ++;
    }
    if (pinAState == 0 && pinBState == 1) {
    if (pinAStateOld == 0 && pinBStateOld == 0) // forward
    encoder0Pos ++;
    if (pinAStateOld == 1 && pinBStateOld == 1) // reverse
    encoder0Pos --;
    if (pinAStateOld == 1 && pinBStateOld == 0) // unknown
    unknownvalue ++;
    if (pinAStateOld == 0 && pinBStateOld == 1) // unknown
    unknownvalue ++;
    }
    if (pinAState == 1 && pinBState == 1) {
    if (pinAStateOld == 0 && pinBStateOld == 1) // forward
    encoder0Pos ++;
    if (pinAStateOld == 1 && pinBStateOld == 0) // reverse
    encoder0Pos --;
    if (pinAStateOld == 0 && pinBStateOld == 0) // unknown
    unknownvalue ++;
    if (pinAStateOld == 1 && pinBStateOld == 1) // unknown
    unknownvalue ++;
    }

    if (pinAState == 1 && pinBState == 0) {
    if (pinAStateOld == 1 && pinBStateOld == 1) // forward
    encoder0Pos ++;
    if (pinAStateOld == 0 && pinBStateOld == 0) // reverse
    encoder0Pos --;
    if (pinAStateOld == 0 && pinBStateOld == 1) // unknown
    unknownvalue ++;
    if (pinAStateOld == 1 && pinBStateOld == 0) // unknown
    unknownvalue ++;
    }
    pinAStateOld = pinAState;
    pinBStateOld = pinBState;
}


/*
Serial.print("Encoder: "); Serial.print(encoder0Pos); 
Serial.print(" unKnown: ");Serial.print(unknownvalue, DEC); 
Serial.println(""); 
*/
