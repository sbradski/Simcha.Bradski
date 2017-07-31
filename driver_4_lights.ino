//Light machine logic

/*
  State button (LED to Human)       meaning
  0 Nothing (LED 1/sec blink)       Just looping looking for input
  1 StepLights (LED 3/sec blink)    Can step through lights
  2 ManualStepThrough (LED 10/sec)  When you push camera button, light on and picture
  3 AutoStepThrough (LED on)        Goes through all lights and all pictures

  If you are in the middle of a state, pressing state returns you to state 0 (Nothing)

  Camera button: Push to take picture while in state
  1 (StepLights) changes the lights
  2 (ManualStepThrough) change the lights and camera
  3 (AutoStepThrough) collect pictures with light automatically for all lights

*/

const int pinButtonState = 8; // the number of the pushbutton pin
const int pinButtonCamera = 10; //button for the camera
const int ledPin = 13; // the number of the LED pin
const int indicatorLed = 12; //the led that shows what state you are in on circuit board
const int cameraShutter = 11;
const int ledTR = 3;   //TR light on circut board
const int ledTL = 4;   //TL light on cirut board
const int ledBR = 5;   //BR light on circut board
const int ledBL = 6;   //BL light on circut board
const int ledAll = 10; // all the lights are ON (not on circut board)

const int whichLightOn; //which of the 4 lights are on

const int shutterLag = 50;
const int shutterDelay = 100;
const int ON = 1;
const int OFF = 0;

//for cameraButton
const int stepLights = 1; // changes the lights with no pictures taken
const int manualStepThrough = 2; // change the lights and camera
const int autoStepThrough = 3; // collect pictures with light automatically for all lights in sequence

// Variables will change:
int ledState = HIGH; // the current state of the output pin
int buttonState = HIGH; // the current reading from the input pin
int buttonCamera = HIGH; //so it does not start w/out button click
int lastButtonState = HIGH; // the previous reading from the input pin
int lastButtonCamera = HIGH;
int state = 0; //0 Do nothing, 1 StepLIghts, 2 ManualStepThrough, 3 AutoStepThrough
int lightOnMode = 0; //0 no lights, 1 TR on, 2 TL on, 3 BR on, 4 BL on


// the following variables are unsigned long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
unsigned long lastStateDebounceTime = 0; // the last time the output pin was toggled
unsigned long lastCameraDebounceTime = 0;
unsigned long debounceDelay = 50; // the debounce time; increase if the output flickers
unsigned long lastIndicatorLightChange = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ledTR, OUTPUT);
  pinMode(ledTL, OUTPUT);
  pinMode(ledBR, OUTPUT);
  pinMode(ledBL, OUTPUT);
  pinMode(indicatorLed, OUTPUT);
  pinMode(cameraShutter, OUTPUT); //camera shutter
  pinMode(pinButtonState, INPUT_PULLUP);
  pinMode(pinButtonCamera, INPUT_PULLUP);
  lastIndicatorLightChange = millis();
  Serial.begin(115200);
  state = 0;
  // set initial LED state
  digitalWrite(ledPin, ledState);
}


//If true, then buttonState is changed.
bool debounceButtonState() {
  bool debounced = false; //not a debounced button
  int button = digitalRead(pinButtonState);
  if ( button != lastButtonState) {
    lastStateDebounceTime = millis(); //Just a timer
  }
  lastButtonState = button; //Reset the last button state

  if ((millis() - lastStateDebounceTime) > debounceDelay) { //enough time?
    if (buttonState != button) {
      buttonState = button; //Change buttonState if it is different and there's been enough time
      debounced = true;
    }
  }
  return debounced; //if true, state button has really changed
}

//If true, then buttonCamera is changed.
bool debounceButtonCamera() {
  bool debounced = false; //not a debounced button
  int button = digitalRead(pinButtonCamera);
  if ( button != lastButtonCamera) {
    lastCameraDebounceTime = millis(); //Just a timer
  }
  lastButtonCamera = button; //Reset the last button state

  if ((millis() - lastCameraDebounceTime) > debounceDelay) { //enough time?
    if (buttonCamera != button) {
      buttonCamera = button; //Change buttonState if it is different and there's been enough time
      debounced = true;
    }
  } //long enough to check
  return debounced; //if true, camera button has really changed
}



unsigned long handleIndicatorLED(int state, unsigned long last_indicator_change) {
  if (state == 0) { //Do nothing
    if (millis() - last_indicator_change > 1000) { //1x/sec
      Serial.println("sec");
      if (HIGH == digitalRead(indicatorLed)) {
        digitalWrite(indicatorLed, LOW);
      } else {
        digitalWrite(indicatorLed, HIGH);
      }
      last_indicator_change = millis();
    }
  } else if (state == 1) { //StepLights state
    if (millis() - last_indicator_change > 300) { //3x/sec
      if (HIGH == digitalRead(indicatorLed)) {
        digitalWrite(indicatorLed, LOW);
      } else {
        digitalWrite(indicatorLed, HIGH);
      }
      last_indicator_change = millis();
    }
  } else if (state == 2) { //ManualStepThrough state
    if (millis() - last_indicator_change > 100) { //10x/sec
      if(HIGH == digitalRead(indicatorLed)) {
        digitalWrite(indicatorLed, LOW);
      } else {
        digitalWrite(indicatorLed, HIGH);
      }
      last_indicator_change = millis();
    }
  } else if (state == 3) { //AutoStepThrough keep on
      if(LOW == digitalRead(indicatorLed)) {
        digitalWrite(indicatorLed, HIGH);  //keeps the indicator LED on
      }
      last_indicator_change = millis();
  }
  return last_indicator_change;
}


void resetSystemState() {
  Serial.println("I'm resetting variables and hardware to starting configuration");
  doNoLights(); // Turn all lights off
  lightOnMode = 0;
}

void takePicture() {   //shutter clicks and takes a picture
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


void doNoLights() {
  led_set(ledTR, OFF);
  led_set(ledTL, OFF);
  led_set(ledBR, OFF);
  led_set(ledBL, OFF);
  Serial.println("Turning all lights off");
}

void doAllLights() {
  led_set(ledTR, ON);
  led_set(ledTL, ON);
  led_set(ledBR, ON);
  led_set(ledBL, ON);
  delay(500);
  if (buttonCamera == ON) {
    takePicture();
    Serial.println("picture taken for all lights");
  }
}

void doTRLights() {
  led_set(ledTR, ON);
  led_set(ledTL, OFF);
  led_set(ledBR, OFF);
  led_set(ledBL, OFF);
  delay (500);
  if (buttonCamera == ON) {
    takePicture();
    Serial.println("picture taken for TR");
  }
}

void doTLLights() {
  led_set(ledTR, OFF);
  led_set(ledTL, ON);
  led_set(ledBR, OFF);
  led_set(ledBL, OFF);
  delay(500);
  if (buttonCamera == ON) {
    takePicture();
    Serial.println("picture taken for TL");
  }
}

void doBRLights() {
  led_set(ledTR, OFF);
  led_set(ledTL, OFF);
  led_set(ledBR, ON);
  led_set(ledBL, OFF);
  delay(500);
  if (buttonCamera == ON) {
    takePicture();
    Serial.println("picture taken for BR");
  }
}

void doBLLights() {
  led_set(ledTR, OFF);
  led_set(ledTL, OFF);
  led_set(ledBR, OFF);
  led_set(ledBL, ON);
  delay(500);
  if (buttonCamera == ON) {
    takePicture();
    Serial.println("picture taken for BL");
  }
}


///////////////////////////////////////////////////... main loop
void loop() {

  //There is probably a function where you use state and millis() 
  //    to decide when to turn indicateor light on, of or stay on
  //    depending on state and timer.
  lastIndicatorLightChange = handleIndicatorLED (state, lastIndicatorLightChange);

  //Check state change
  bool stateChange = debounceButtonState();
  if (stateChange && buttonState == LOW) {
    state = (state + 1) % 4; //cycle state 0, 1, 2, 3
    lastIndicatorLightChange = handleIndicatorLED(state, lastIndicatorLightChange); //Indicate by IndicatorLED what state we are in
    Serial.print("State = ");
    Serial.println(state);
    resetSystemState(); //todo set state to starting state again
  }

  //Handle the states
  if (state == 1) { //stepping lights
    bool cameraChange = debounceButtonCamera();
    if (cameraChange && buttonCamera == LOW) {
      //  ??? Needs mod 6, 0-5 'states' no, all, tr,tl,bt,bl
      lightOnMode = (lightOnMode + 1) % 6; //0 no lights, 1 all, 2 TR on, 3 TL on, 4 BR on, 5 BL on
      switch (lightOnMode) {
        case 0:
          doNoLights();
          break;
        case 1:
          doAllLights();
          break;
        case 2:
          doTRLights();
          break;
        case 3:
          doTLLights();
          break;
        case 4:
          doBRLights();
          break;
        case 5:
          doBLLights();
          break;
      } //end switch lightOnMode
    }
  } else if (state == 2) { //Manual step light and camera
    bool cameraChange = debounceButtonCamera();
    if (cameraChange && buttonCamera == LOW) {  //step through lights manually with camera shutter
      // ??? again mod 6
      lightOnMode = (lightOnMode + 1) % 6; //0 no lights, 1 all, 2 TR on, 3 TL on, 4 BR on, 5 BL on
      switch (lightOnMode) {
        case 0:
          doNoLights();
          break;
        case 1:
          doAllLights();
          takePicture();
          break;
        case 2:
          doTRLights();
          takePicture();
          break;
        case 3:
          doTLLights();
          takePicture();
          break;
        case 4:
          doBRLights();
          takePicture();
          break;
        case 5:
          doBLLights();
          takePicture();
          break;
         
      } //end switch lightOnMode
    }
  } else if (state == 3) { //auto step light and camera
    bool cameraChange = debounceButtonCamera();
    if (cameraChange && buttonCamera == LOW) { //automaticaly step through the lights while taking pics
      doAllLights();
      delay(200);
      takePicture();
      doTRLights();
      delay(200);
      takePicture();
      doTLLights();
      delay(200);
      takePicture();
      doBRLights();
      delay(200);
      takePicture();
      doBLLights();
      delay(200);
      takePicture();
      doNoLights();
    }
  }
}
////////////////////////////////////////////////////////////////...end main loop

