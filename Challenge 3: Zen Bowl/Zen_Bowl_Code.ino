     /* This is the Arduino Code for MTI (Make Things Interactive)
 
   Codes that are used in this project include:
    - Capacitive sensor code from Scott Fitzgerald, 18 Sep 2012 (provided by Karen Ann Donachie in MTI class) 
    - Stepper motor code from Dan of the Simple Electronics Youtube channel (www.SimpleElectronics.ca) 
    - Heart rate monitoring code from https://lastminuteengineers.com/pulse-sensor-arduino-tutorial/
    - Lie detector project used as DIY Galvanic Skin Response (GSR) from https://create.arduino.cc/projecthub/BuildItDR/arduino-lie-detector-a0b914  
  
    Libraries that are used for this project include:
    - CapacitiveSensor library by Paul Bagder, Paul Stoffregen https://www.arduino.cc/reference/en/libraries/capacitivesensor/ 
    - Stepper motor library https://www.arduino.cc/reference/en/libraries/stepper/
    - CuteBuzzerSounds library by Everton Ramires https://www.arduino.cc/reference/en/libraries/cutebuzzersounds/
    - PulseSensorPlayground library https://www.arduino.cc/reference/en/libraries/pulsesensor-playground/
  */
   
// SET UP INPUTS
 
//set up capacitive sensors
  //include library
  #include <CapacitiveSensor.h>
  //pin 13 sends electrical energy, pin 12 senses a change
  CapacitiveSensor capSensor1 = CapacitiveSensor(13, 12);
  //threshold for registering touch
  int threshold = 200;

 //set up GSR sensor
  const int GSRsensorPin = A3; 
  int thresholdGSR = 130;

//set up Pulse sensor
  // Set-up low-level interrupts for most acurate BPM math.
  #define USE_ARDUINO_INTERRUPTS true     
  // Includes the PulseSensorPlayground Library.
  #include <PulseSensorPlayground.h> 
  // PulseSensor PURPLE WIRE connected to ANALOG PIN 0 
  const int PulseWire = A0;       
  // The on-board Arduino LED, close to PIN 13.
  //const int LED13 = 13;  
  // Determine which Signal to "count as a beat" and which to ignore. 
  int thresholdPulse = 550; 
  int thresholdBPM = 115;
  // Creates an instance of the PulseSensorPlayground object called "pulseSensor" 
  PulseSensorPlayground pulseSensor; 
  // Calls function on our pulseSensor object that returns BPM as an "int". 
  int myBPM = pulseSensor.getBeatsPerMinute();  // "myBPM" hold this BPM value now. 
             

// SET UP OUTPUTS
  
// set up stepper
  // include library
  #include <Stepper.h>
  // define stepsperrevolution
  const int stepsPerRevolution = 2048;
  //Use pin 8-11 to IN1-IN4
  Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);
  //size of stepper motor steps
  int step = 16;
  int stepBack = -16;
  
// set up green LEDs
  int greenLED = 3;

// set up red LED  
  int redLED = A5;

//set up buzzer
  int buzz = 2;
  //include sound library
  #include <CuteBuzzerSounds.h> 
                                      /*SOUND OPTIONS: 
                                     S_CONNECTION   S_DISCONNECTION S_BUTTON_PUSHED   
                                     S_MODE1        S_MODE2         S_MODE3     
                                     S_SURPRISE     S_OHOOH         S_OHOOH2    
                                     S_CUDDLY       S_SLEEPING      S_HAPPY     
                                     S_SUPER_HAPPY  S_HAPPY_SHORT   S_SAD       
                                     S_CONFUSED     S_FART1         S_FART2     
                                     S_FART3        S_JUMP 20       */

// set up timer 
  unsigned long previousMillis = 0;

  
void setup() {
  // initialize the serial port:
  Serial.begin(9600);

  //define input
  // Pulse sensor
    pulseSensor.analogInput(PulseWire); 
    //auto-magically blink Arduino's LED with heartbeat.    
    //pulseSensor.blinkOnPulse(LED13);       
    pulseSensor.setThreshold(thresholdPulse);   
    // Double-check the "pulseSensor" object was created and "began" seeing a signal.  
    if (pulseSensor.begin()) { 
      Serial.println("This is a stress monitoring device!");  //This prints one time at Arduino power-up,  or on Arduino reset.   
    }  

  //define output
  //green LED
    pinMode(greenLED,OUTPUT);
  //red LED
    pinMode(redLED,OUTPUT);
  //buzzer
    pinMode(buzz,OUTPUT);
    cute.init(buzz);
}



 // function for normal state, continuous rotation of motor
 void normalstate () {
    myStepper.setSpeed(15);
    myStepper.step(stepsPerRevolution);
    analogWrite(greenLED, 0);
    analogWrite(redLED, 0);
 }

 // function for breathing exercise
 void breathing () {
  unsigned long currentMillis = millis();
  unsigned long Time = currentMillis - previousMillis;
  /*Serial.print("Time");
  Serial.print(Time/1000);
  Serial.println("seconds");*/

  // 4 seconds of breathing in, motor rotates clockwise and led brightness increases
  if (Time < 4000) {
    myStepper.setSpeed(15);
    myStepper.step(step);
    // map LED brightness increase to 4 second movement of motor
    int ledBrightness=map(Time, 0, 4000,0,255);
    analogWrite(greenLED, ledBrightness);
    }

  // 7 seconds of holding breath, motor stops, led brightness is at max
  if (Time >= 4000 && Time < 11000){
    analogWrite(greenLED, 255);
    }

   if (Time >= 11000 && Time < 11100){
    cute.play(S_CONNECTION);
   }
  
  // 7 seconds of breathing out, motor rotates counter clockwise and led brightness decreases
  if (Time >= 11000 && Time < 18000){
    myStepper.setSpeed(8.6);
    myStepper.step(-step);
    //  map LED brightness decrease to 7 second movement of motor
    int ledBrightness=map(Time, 11000, 18000, 255, 0);
    analogWrite(greenLED, ledBrightness);
    }
    
  // 1 second break, motor stops, led brightness is at min
  if (Time >= 18000 && Time < 19000){
    analogWrite(greenLED, 0);
    }

  // reset time after 19 seconds to trigger new breathing round
  if (Time >= 19000){
    cute.play(S_CONNECTION);
    previousMillis = currentMillis;
    }
  }


// function for stress warning
 void stresswarning () {
  Serial.println("you are stressed! ");  
  Serial.print("BPM: ");                        
  Serial.println(myBPM);  
  analogWrite(redLED, 255);
  myStepper.setSpeed(5);
  myStepper.step(-step);
  //delay (5000);
  }

void loop () {

  // read GSR
  int sensorValueGSR = analogRead(GSRsensorPin);
  // print the GSR sensor value
  Serial.print("GSR:");
  Serial.println(sensorValueGSR);  

  // read pulse 
  int myBPM = pulseSensor.getBeatsPerMinute();  // "myBPM" hold this BPM value now.
  if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened".  
  //Serial.println("♥  Have a great day today ! "); // If test is "true", print a message "a heartbeat happened". 
  Serial.print("BPM: ");                          
  Serial.println(myBPM);                          
  } 
  
  // read capacitive sensors value
  long sensorValueTouch = capSensor1.capacitiveSensor(30);
  // print the capacitive sensor value
  Serial.print("TouchSensor:");
  Serial.println(sensorValueTouch);

  // read the value from the GSR and heart rate sensor, if both exceed the threshhold, enable stress warning
  // if (sensorValueGSR > thresholdGSR && sensorValueHR > thresholdHR) {
  if (sensorValueGSR > thresholdGSR && myBPM > 115 && sensorValueTouch < threshold) {
    Serial.println("You are stressed, do the breathing exercise!");
    stresswarning ();
  }
  // if the value of the capacitive sensor value is greater than the threshold, start the breathing exercise
  else if (sensorValueTouch > threshold && sensorValueGSR > thresholdGSR && myBPM > 115) {
    Serial.println("You are still stressed, keep breathing!");
    breathing();
  }
  // if the value of the capacitive sensor value is greater than the threshold, start the breathing exercise
  else if (sensorValueTouch > threshold && (sensorValueGSR < thresholdGSR || myBPM < 115)) {
    Serial.println("You are doing great, breathing is important!");
    analogWrite(redLED, 0);
    breathing();
  }
  // if it's lower than the threshold
  else {
   Serial.println("You are amazing!♥");
   normalstate();
  }


}
