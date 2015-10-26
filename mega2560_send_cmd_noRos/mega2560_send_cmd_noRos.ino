  
#include <SoftwareSerial.h>
#include <Metro.h>
SoftwareSerial mySerial(10, 11); // RX, TX
Metro mainTimer = Metro(10);
//cmd Byte to UNO
char commandArray[3];
char cS=0;
byte cH=0;
byte cL=0;
byte cF=0;
int cmd_P=180;
void setup() { 

 Serial.begin (115200);
 Serial.println("start");                // a personal quirk
 mySerial.begin(19200);


} 

void loop(){
  if (mainTimer.check() == true) {
      
	/////WRITE CMD to UNO

	    char cS='{';
	    byte cH =highByte(cmd_P);
	    byte cL =lowByte(cmd_P);
	    char cF='}'; 
	    mySerial.print(cS);mySerial.write(cH);mySerial.write(cL);mySerial.write(cF);
      


  }

}




