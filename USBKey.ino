#include <EEPROM.h>
#include <USBComposite.h>

#define BOOT_MODE_PIN 21
#define LED_PIN 33

int ledPin =  13;    // LED connected to digital pin 13

const char USERNAME[] = "USERNAME";
const char PASSWORD[] = "PASSWORD";
const char DELAY[] = "DELAY";

USBHID HID;
HIDKeyboard Keyboard(HID);

uint16 DataWrite = 0;
uint16 AddressWrite = 0x10;

uint16 USERNAME_ADDRESS = 0x10;
uint16 PASSWORD_ADDRESS = 0x210;
uint16 DELAY_ADDRESS = 0x310;

char serInString[64];
char userName[64];
char password[64];
char timerDelay[64];

int serInIndx = 0;
int serOutIndx = 0;

void chomp(char *s) {
    while(*s && *s != '\n' && *s != '\r') s++;
    
    *s = 0;
}

void blocking_readSerialString(){
  int sb;
  while (Serial.available() == 0);
  if(Serial.available()) { 
    //Serial.print("reading Serial String: ");     //optional confirmation
    while (Serial.available()){ 
      sb = Serial.read();             
      serInString[serInIndx] = sb;
      serInIndx++;
      //serialWrite(sb);                        //optional confirmation
    }
       //Serial.println();
  } 
}

void readSerialString () {
    int sb;   
    if(Serial.available()) { 
       //Serial.print("reading Serial String: ");     //optional confirmation
       while (Serial.available()){ 
          sb = Serial.read();             
          serInString[serInIndx] = sb;
          serInIndx++;
          //serialWrite(sb);                        //optional confirmation
       }
       //Serial.println();
    }  
}


void printSerialString() {
   if( serInIndx > 0) {
      //Serial.print("Arduino memorized that you said: ");     
      //loop through all bytes in the array and print them out
      for(serOutIndx=0; serOutIndx < serInIndx; ++serOutIndx) {
          Serial.print( serInString[serOutIndx] );    //print out the byte at the specified index
          //serInString[serOutIndx] = "";            //optional: flush out the content
      }        
      //reset all the functions to be able to fill the string back with content
      serOutIndx = 0;
      serInIndx  = 0;
      //Serial.println();
   }

}


void setup(){
	// initialize the digital pin as an output:
	pinMode(ledPin, OUTPUT);
  pinMode(BOOT_MODE_PIN, INPUT);
  Serial.begin(115200);

}

void loop(){

  uint16 Status;
  uint16 Data;
  
  delay(10);

  //The serial menu switch is closed, we are trying to get a serial menu, not keyboard mode
  if(HIGH == digitalRead(BOOT_MODE_PIN)){
  	
    int continueLoop;
    int i;

    digitalWrite(LED_PIN, HIGH);
  
    continueLoop = 1;
    while(continueLoop){
  
      //Clear the input string
      serInIndx = 0;
      for(i = 0; i < 64; ++i){
        serInString[i]='\0';
      }
  
      //Get input from the user
      blocking_readSerialString();
  
      //Find out what the user wants to do
      if(strcmp("setusername\r\n",serInString) == 0){
  
        serInIndx = strlen("\r\nPlease enter new USERNAME:");
        strncpy(serInString,"\r\nPlease enter new USERNAME:",strlen("\r\nPlease enter new USERNAME:"));
        printSerialString();
  
        blocking_readSerialString();
  
        chomp(serInString);
  
        for(i = 0; i < 64; ++i){
           EEPROM.write(USERNAME_ADDRESS + i, 0xFFFF);
        }
        i=0;
  
        for(i = 0; i < strlen(serInString); ++i){
          EEPROM.write(USERNAME_ADDRESS + i, serInString[i]);
        }
  
        Serial.print("\r\nUSERNAME has been set to:");
        Serial.print(serInString);
  
      }
      
      else if(strcmp("setpassword\r\n",serInString) == 0){
        
        serInIndx = strlen("\r\nPlease enter new PASSWORD:");
        strncpy(serInString,"\r\nPlease enter new PASSWORD:",strlen("\r\nPlease enter new PASSWORD:"));
        printSerialString();
  
        blocking_readSerialString();
  
        chomp(serInString);
  
        for(i = 0; i < 64; ++i){
           EEPROM.write(PASSWORD_ADDRESS + i, 0xFFFF);
        }
        i=0;
  
        for(i = 0; i < strlen(serInString); ++i){
          EEPROM.write(PASSWORD_ADDRESS + i, serInString[i]);
        }
  
        Serial.print("\r\nPASSWORD has been set to:");
        Serial.print(serInString);
  
      }
  
      else if(strcmp("setdelay\r\n",serInString) == 0){
        
        serInIndx = strlen("\r\nPlease enter new Delay Time (ms):");
        strncpy(serInString,"\r\nPlease enter new Delay Time (ms):",strlen("\r\nPlease enter new Delay Time (ms):"));
        printSerialString();
  
        blocking_readSerialString();
        chomp(serInString);
  
        //clear the old info
        for(i = 0; i < 64; ++i){
           EEPROM.write(DELAY_ADDRESS + i, 0xFFFF);
        }
        i=0;
  
        //set the value
        for(i = 0; i < strlen(serInString); ++i){
          EEPROM.write(DELAY_ADDRESS + i, serInString[i]);
        }
  
        Serial.print("\r\nDelay Time (ms) has been set to:");
        Serial.print(serInString);
  
      }
  
      else if(strcmp("resetdevice\r\n",serInString) == 0){
        
        Serial.print("Resetting device...");
        for (int i = 0 ; i < EEPROM.format() ; i++) {
          EEPROM.write(i, 0);
        }
        Serial.print("Device reset.");
        
  
      }
  
      else if(strcmp("help\r\n",serInString) == 0){
  
        i = 0;
  
        Serial.println();
        Serial.println("Type \"setusername\" to set the user name.");
        Serial.println("Type \"setpassword\" to set the password.");
        Serial.println("Type \"setdelay\" to set the key delay time (in milliseconds).");
        Serial.print("Type \"resetdevice\" to clear all stored data and reset the device to factory.");
  
        Serial.print("\r\nUSERNAME:");
        for(i = 0; i < 64; ++i){
          EEPROM.read(USERNAME_ADDRESS + i,&Data);
          if(Data != 0xFFFF){
            userName[i] = Data;
            Serial.print((char)userName[i]);
          }
        }
  
        Serial.print("\r\nPASSWORD:");
        for(i = 0; i < 64; ++i){
          EEPROM.read(PASSWORD_ADDRESS + i,&Data);
          if(Data != 0xFFFF){
            password[i] = Data;
            Serial.print((char)password[i]);
          }
        }
  
        Serial.print("\r\nDELAY ms:");
        for(i = 0; i < 64; ++i){
          EEPROM.read(DELAY_ADDRESS + i,&Data);
          if(Data != 0xFFFF){
            timerDelay[i] = Data;
            Serial.print((char)timerDelay[i]);
          }
        }
  
      }
  
    } 
  } else {

    char uname[64];
    char pword[64];
    char dTime[64];

    int i;

    i = 0;

    for(i = 0; i < 64; ++i){
      uname[i] = '\0';
      pword[i] = '\0';
      dTime[i] = '\0';
    }

    for(i = 0; i < 64; ++i){
      EEPROM.read(USERNAME_ADDRESS + i,&Data);
      if(Data != 0xFFFF){
        uname[i] = Data;
      }
    }

    for(i = 0; i < 64; ++i){
      EEPROM.read(PASSWORD_ADDRESS + i,&Data);
      if(Data != 0xFFFF){
        pword[i] = Data;
      }
    }

    for(i = 0; i < 64; ++i){
      EEPROM.read(DELAY_ADDRESS + i,&Data);
      if(Data != 0xFFFF){
        dTime[i] = Data;
      }
    }
    
    //We're in normal operations mode. Act like a keyboard.
    digitalWrite(LED_PIN, LOW);

    HID.begin(HID_KEYBOARD);
    Keyboard.begin(); // useful to detect host capslock state and LEDs
    
    while(true){

      delay(5000);

      Keyboard.print(uname);
      delay(5000);
      
      Keyboard.print(pword);
    }
  }
}
