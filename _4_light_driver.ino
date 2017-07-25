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
const int pinButtonState = 0;    // the number of the pushbutton pin
const int pinButtonCamera = 8;   //button for the camera
const int ledPin = 13;      // the number of the LED pin
const int ON = 1;
const int OFF = 0;
const int all = 10;           // all lights ON
const int topright = 20;      // top right is ON
const int topleft = 30;
const int bottomleft = 40;
const int bottomright = 50;

// Variables will change:
int ledState = HIGH;         // the current state of the output pin
int buttonPinState;             // the current reading from the input pin
int buttonCameraState;
int lastButtonPinState = LOW;   // the previous reading from the input pin
int lastButtonCameraState = LOW;
int lightState;
int clickState;

// the following variables are unsigned long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
unsigned long lastStateDebounceTime = 0;  // the last time the output pin was toggled
unsigned long lastCameraDebounceTime = 0;
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);          //camera
  pinMode(0, INPUT_PULLUP);    // button intup
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

  reading = digitalRead(pinButtonCamera);

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
      if (buttonCameraState == HIGH) {
        ledState = !ledState;
        camera_set(9);

      }
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

      // only toggle the LED if the new button state is HIGH
      if (buttonPinState == HIGH) {
        ledState = !ledState;

        if (lightState == all) {
          led_set(3, ON);
          led_set(4, ON);
          led_set(5, ON);
          led_set(6, ON);
          if (clickState == ON) {
            cameraClick();
          }
          Serial.println("State = all");
          lightState = topright;
        } else if (lightState == topright) {
          led_set(3, ON);
          led_set(4, OFF);
          led_set(5, OFF);
          led_set(6, OFF);
          if (clickState == ON) {
            cameraClick();
          }
          Serial.println("State = topright");
          lightState = topleft;
        } else if (lightState == topleft) {
          led_set(3, OFF);
          led_set(4, ON);
          led_set(5, OFF);
          led_set(6, OFF);
          if (clickState == ON) {
            cameraClick();
          }
          Serial.println("State = topleft");
          lightState = bottomleft;
        } else if (lightState == bottomleft) {
          led_set(3, OFF);
          led_set(4, OFF);
          led_set(5, ON);
          led_set(6, OFF);
          if (clickState == ON) {
            cameraClick();
          }
          Serial.print("State == bottomleft");
          lightState = bottomright;
        } else if (lightState == bottomright) {
          led_set(3, OFF);
          led_set(4, OFF);
          led_set(5, OFF);
          led_set(6, ON);
          if (clickState == ON) {
            cameraClick();
          }
          Serial.print("State == bottomright");
          lightState = all;
        } else {
          lightState = all;
        }
      }
    }
    //  digitalWrite(3, HIGH);
    //  delay(1000);
    //  cameraClick();
  }


  // set the LED:
  digitalWrite(ledPin, ledState);

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonPinState = reading;

}

void cameraClick() {
  Serial.println ("take my picture");
  digitalWrite(1, HIGH);
  digitalWrite(1, LOW);
}

void led_set(uint8_t number, uint8_t setting) {
  if (setting == OFF) {
    digitalWrite(number, LOW);
  } else
    digitalWrite(number, HIGH);

}
void camera_set(uint8_t number) {
  digitalWrite(number, HIGH);
  delay(3);
  digitalWrite(number, LOW);
}


