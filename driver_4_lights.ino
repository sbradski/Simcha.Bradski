/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino model, check
  the Technical Specs of your board  at https://www.arduino.cc/en/Main/Products

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald

  modified 2 Sep 2016
  by Arturo Guadalupi

  modified 8 Sep 2016
  by Colby Newman
*/

// constants won't change. They're used here to
// set pin numbers:
const int pinButtonState = 8; // the number of the pushbutton pin
const int pinButtonCamera = 10; //button for the camera
const int ledPin = 13; // the number of the LED pin
const int cameraShutter = 11;
const int ledTR = 3;
const int ledTL = 4;
const int ledBR = 5;
const int ledBL = 6;

const int shutterLag = 50;
const int shutterDelay = 100;
const int ON = 1;
const int OFF = 0;
const int all = 10; // all lights ON
const int topright = 20; // top right is ON
const int topleft = 30;
const int bottomleft = 40;
const int bottomright = 50;
const int noShutter = 0; // there is no shutter
const int oneShutter = 1; // manual shutter
const int allShutter = 2; // sequence of lights and shutter

// Variables will change:
int ledState = HIGH; // the current state of the output pin
int buttonPinState; // the current reading from the input pin
int buttonCameraState = LOW; //so it does not start w/out button click
int lastButtonPinState = LOW; // the previous reading from the input pin
int lastButtonCameraState = LOW;
int lightState = 0;
int clickState = 0;


// the following variables are unsigned long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
unsigned long lastStateDebounceTime = 0; // the last time the output pin was toggled
unsigned long lastCameraDebounceTime = 0;
unsigned long debounceDelay = 50; // the debounce time; increase if the output flickers

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ledTR, OUTPUT);
  pinMode(ledTL, OUTPUT);
  pinMode(ledBR, OUTPUT);
  pinMode(ledBL, OUTPUT);
  pinMode(cameraShutter, OUTPUT); //camera shutter
  pinMode(pinButtonState, INPUT_PULLUP);
  pinMode(pinButtonCamera, INPUT_PULLUP);

  Serial.begin(115200);
  lightState = all;
  // set initial LED state
  digitalWrite(ledPin, ledState);
}

// the loop function runs over and over again forever
void loop() {
  //  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  //  delay(1000);                       // wait for a second
  //  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  //  delay(1000);                       // wait for a second

  int reading;

  reading = digitalRead(pinButtonCamera);   //reading input pin

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonCameraState) {
    // reset the debouncing timer
    lastCameraDebounceTime = millis();
  }
  if ((millis() - lastCameraDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonCameraState) {
      buttonCameraState = reading;

      // only toggle the LED if the new button state is HIGH
      if (clickState == allShutter) {
        doAllStates();
        clickState = oneShutter;
        Serial.println("allShutter");
      }
      
      else if (clickState == oneShutter) {
       // doManually();
        clickState = noShutter;
        Serial.println("oneShutter");
      }
      else if (clickState == noShutter) {
        doNoShutter();
        clickState = allShutter;
        Serial.println("noShutter");
      }
      ledState = !ledState;
      Serial.println("change cameraState");
      // camera_set(cameraShutter);
    }
  }


  digitalWrite(ledPin, ledState);

  lastButtonCameraState = reading;

  // read the state of the switch into a local variable:
  reading = digitalRead(pinButtonState);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonPinState) {
    // reset the debouncing timer
    lastStateDebounceTime = millis();
  }
  if ((millis() - lastStateDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonPinState) {
      buttonPinState = reading;
      Serial.println("stateButton");

      // only toggle the LED if the new button state is HIGH
      if (buttonPinState == HIGH) {
        ledState = !ledState;

        if (clickState == oneShutter) {
          doAllStates();
          Serial.println ("allShutterlol") ;
        } else if (lightState == doAllLights) {
          doAllLights();
          Serial.println("alllights");
        } else if (lightState == doTRLights) {
          doTRLights();
          Serial.println("TRlights");
        } else if (lightState == doTLLights) {
          doTLLights();
          Serial.println("TLlights");
        } else if (lightState == doBRLights) {
          doBRLights();
          Serial.println("BRlights");
        } else if (lightState == doBLLights) {
          doBLLights();
        } else {
          lightState = all;
        }
      }
    }
  }

  //  digitalWrite(3, HIGH);
  //  delay(1000);
  //  cameraClick();

  // set the LED:
  digitalWrite(ledPin, ledState);

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonPinState = reading;
}
void cameraClick() {
  digitalWrite(cameraShutter, LOW);
  delay(shutterLag);
  Serial.println("take my picture");
  digitalWrite(cameraShutter, HIGH);
  delay(shutterDelay);
  digitalWrite(cameraShutter, LOW);
  delay(shutterLag);

}

void led_set(uint8_t number, uint8_t setting) {
  if (setting == OFF) {
    digitalWrite(number, LOW);
  } else {
    digitalWrite(number, HIGH);
  }
}


void doAllStates() {
  doAllLights();
  //cameraClick();
  delay(600);
  clickState = ON; 
  doTRLights();
  delay(600);
  doTLLights();
  delay(600);
  doBRLights();
  delay(600);
  doBLLights();
}

/*void doManually() {
  doAllLights();
  pinMode(pinButtonState, INPUT_PULLUP);
  doTRLights();
  pinMode(pinButtonState, INPUT_PULLUP);
  doTLLights();
  pinMode(pinButtonState, INPUT_PULLUP);
  doBRLights();
  pinMode(pinButtonState, INPUT_PULLUP);
  doBLLights();
}
*/
void doNoShutter() {
  clickState = OFF;
  doAllLights();
  doTRLights();
  doTLLights();
  doBRLights();
  doBLLights();
}
void doAllLights() {
  led_set(ledTR, ON);
  led_set(ledTL, ON);
  led_set(ledBR, ON);
  led_set(ledBL, ON);
  delay(500);
  if (clickState == ON) {
    cameraClick();
    Serial.println("click");
  }
  Serial.println("State = all");
  lightState = topright;
}

void doTRLights() {
  led_set(ledTR, ON);
  led_set(ledTL, OFF);
  led_set(ledBR, OFF);
  led_set(ledBL, OFF);
  delay (500);
  if (clickState == ON) {
    cameraClick();
    Serial.println("click");
  }
  Serial.println("State = topright");
  lightState = topleft;
}

void doTLLights() {
  led_set(ledTR, OFF);
  led_set(ledTL, ON);
  led_set(ledBR, OFF);
  led_set(ledBL, OFF);
  delay(500);
  if (clickState == ON) {
    cameraClick();
    Serial.println("click");
  }
  Serial.println("State = topleft");
  lightState = bottomleft;
}

void doBRLights() {
  led_set(ledTR, OFF);
  led_set(ledTL, OFF);
  led_set(ledBR, ON);
  led_set(ledBL, OFF);
  delay(500);
  if (clickState == ON) {
    cameraClick();
    Serial.println("click");
  }
  Serial.println("State = bottomright");
  lightState = bottomright;
}

void doBLLights() {
  led_set(ledTR, OFF);
  led_set(ledTL, OFF);
  led_set(ledBR, OFF);
  led_set(ledBL, ON);
  delay(500);
  if (clickState == ON) {
    cameraClick();
    Serial.println("click");
  }
  Serial.println("State = bottomleft");
  lightState = all;
}

/*
int ledPin2 = 12;    //led connected to digital pin 12

void setup2() {
  pinMode(ledPin2, OUTPUT); 
}

void loop2 () {
  if (clickState == allShutter){
    digitalWrite(ledPin2, HIGH);             // light blinking quickly
    digitalWrite(ledPin2, LOW);
  }else if (clickState == oneShutter) {
   digitalWrite(ledPin2, HIGH);
    delay(400);                             //light blinking slowly
   digitalWrite(ledPin2, LOW);
  }else if (clickState == noShutter) {
    digitalWrite(ledPin2, HIGH);                    //light always on
  }
}

*/
