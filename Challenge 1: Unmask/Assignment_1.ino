/* This is my arduino code for my MTI (Make Things Interactive) project

  Codes that are used in this project include:
  - Capacitive sensor code from Scott Fitzgerald, 18 Sep 2012 (provided by Karen Ann Donachie in MTI class)
  - Code to display colors on RGB LED and change brightness from https://wokwi.com/projects/313797447551812161
  - Code to map potentiometer values to full color range from https://blog.codebender.cc/2018/08/25/rgb-led-with-potentiometer-arduino-tutorial/

  Libraries that are used for this project include:
  - CapacitiveSensor library by Paul Bagder, Paul Stoffregen https://www.arduino.cc/reference/en/libraries/capacitivesensor/
  - CuteBuzzerSounds library by Everton Ramires https://www.arduino.cc/reference/en/libraries/cutebuzzersounds/
  */

//set up RGB LED
  //pay attention to type of RGB LED (common annode or common cathode) and which leg is which color
  //need to be on PWM (pulse width modulation) pins to set intensity of each internal LED
  int red_pin = 9;
  int green_pin = 11;
  int blue_pin = 10;

//set up capacitive sensors
  //include library
  #include <CapacitiveSensor.h>
  //pin 8 and 4 send electrical energy, pin 7 and 2 sense a change
  CapacitiveSensor capSensor1 = CapacitiveSensor(8, 7);
  CapacitiveSensor capSensor2 = CapacitiveSensor(4, 2);
  //threshold for registering touch (a smile)
  int threshold = 300;

//set up potentiometer
  //rotation sensor on analog pin A0
  int rotaPin = A0;
  //stores rotation sensor input value
  int rotaValue;

//set up buzzer
  int buzz = 12;
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

// set up RGB LED, first three digits represent red, green, blue values, last digit represents brightness of led 
void RGB_color(int red, int green, int blue, int brightness_percent) {
  
  analogWrite(red_pin, red * brightness_percent / 100);
  analogWrite(green_pin, green * brightness_percent / 100);
  analogWrite(blue_pin, blue * brightness_percent / 100);
  
}

void setup() {
  
  //open serial connection
  Serial.begin(9600);

  //set potentiometer as input
  pinMode(rotaPin, INPUT);

  //set the LED pins as output
  pinMode(red_pin,OUTPUT);
  pinMode(green_pin,OUTPUT);
  pinMode(blue_pin,OUTPUT);

  //set the buzzer as output
  pinMode(buzz,OUTPUT);

  cute.init(buzz);

}

void loop() {

  // read sensor input value from potentiometer
  rotaValue = analogRead(rotaPin);

  // scale sensor input value from potentiometer [0,1023] to LED brightness range [0,255]
  int dim = map(rotaValue, 0, 1023, 0, 255);

  // print potentiometer values to serial monitor/plotter
  Serial.print("Mood:");
  Serial.print(rotaValue);
  Serial.print(" ");
  Serial.println(dim);

  // read capacitive sensors value
  long sensorValue1 = capSensor1.capacitiveSensor(30);
  long sensorValue2 = capSensor2.capacitiveSensor(30);

  // print the capacitive sensor value
  Serial.print("Sensor1:");
  Serial.println(sensorValue1);
  Serial.print("Sensor2:");
  Serial.println(sensorValue2);

  // if the value of both capacitive sensor values is greater than the threshold, turn the LED to 100% brightness on according to set mood
  if (sensorValue1 > threshold && sensorValue2 > threshold) {
          
         if (dim <= 42.5) {
         RGB_color(255, dim*6 , 125, 100);
         delay (1000);
         }

         if ((dim > 42.5) && (dim <= 85)) {
         RGB_color(255 - (dim - 43) * 6, 255 , 0, 100);
         delay (1000);
         }

         if ((dim > 85) && (dim <= 127.5)) {
         RGB_color(0, 255, (dim -85) *6, 100);
         delay (1000);
         }

        if ((dim > 127.5) && (dim <= 170)) {
        RGB_color(0, (dim - 127.5) * 6, 255, 100);
        delay (1000);
        }

        if ((dim > 170) && (dim <= 212.5)) {
        RGB_color((dim - 170) * 6, 0, 255, 100);
        delay (1000);
        }

        if ((dim > 212.5) && (dim <= 255)) {
        RGB_color(255, 0, (255 - (dim - 212.5) * 6), 100);
        delay (1000);
        };

        // play tune when smile is detected
        cute.play(S_HAPPY);
        delay(200);
  }
  
  // if the value of either one of the capacitive sensor values is lower than the threshold, turn the LED only with 50% brightness of color according to set mood
  else {
           if (dim <= 42.5) {
         RGB_color(255, dim*6 , 125, 50);
         }

         if ((dim > 42.5) && (dim <= 85)) {
         RGB_color(255 - (dim - 43) * 6, 255 , 0, 50);
         }

         if ((dim > 85) && (dim <= 127.5)) {
         RGB_color(0, 255, (dim -85) *6, 50);
         }

        if ((dim > 127.5) && (dim <= 170)) {
        RGB_color(0, (dim - 127.5) * 6, 255, 50);
        }

        if ((dim > 170) && (dim <= 212.5)) {
        RGB_color((dim - 170) * 6, 0, 255, 50);
        }

        if ((dim > 212.5) && (dim <= 255)) {
        RGB_color(255, 0, (255 - (dim - 212.5) * 6), 50);
        };

  }
  
}
