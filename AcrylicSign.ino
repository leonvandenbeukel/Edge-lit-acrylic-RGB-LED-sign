#include <EEPROM.h>

int RPIN = 3; // Red
int GPIN = 5; // Green
int BPIN = 6; // Blue

int btnPIN = 2;

int address = 0;
byte pattern = 0;
int maxPattern = 6;

// button
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;   // the last time the output pin was toggled
long debounceDelay = 50;     // the debounce time; increase if the output flickers

int state = LOW;

//uncomment this line if using a Common Anode LED
#define COMMON_ANODE

void setup() {

  //EEPROM.write(address, 0);

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  
  pinMode(RPIN, OUTPUT);
  pinMode(BPIN, OUTPUT);
  pinMode(GPIN, OUTPUT);

  pinMode(btnPIN,INPUT_PULLUP);

  // get the last pattern from memory
  pattern = EEPROM.read(address);
  Serial.print("Pattern from EEPROM: ");
  Serial.println(pattern);

  // disable rgb led
  ledsOff();
}

void loop() {  

  // read the state of the switch into a local variable:
  int reading = digitalRead(btnPIN);

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        state = !state;
        pattern += 1;        

        // reset if max is reached
        if (pattern == maxPattern) {
          pattern = 0;    
        }
      
        // save the pattern
        EEPROM.write(address, pattern);

        // disable leds
        ledsOff();        
      }
    }
  }

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;

  Serial.print("Active: ");  
  Serial.println(pattern);
  
  // execute pattern 
  switch (pattern) {
    case 0:
      setColor(255, 0, 0);    // red
      break;
    case 1:
      setColor(0, 255, 0);    // green
      break;
    case 2:
      setColor(0, 0, 255);    // blue
      break;
    case 3:
      setColor(255, 255, 0);  // yellow
      break;
    case 4:
      setColor(80, 0, 80);    // purple
      break;
    case 5:
      setColor(0, 255, 255);  // aqua
      break;
  }
}

void ledsOff() {
  digitalWrite(RPIN, HIGH);
  digitalWrite(GPIN, HIGH);
  digitalWrite(BPIN, HIGH);
}

void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(RPIN, red);
  analogWrite(GPIN, green);
  analogWrite(BPIN, blue);  
}

