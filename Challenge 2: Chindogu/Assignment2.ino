
/* This is my arduino code for my MTI (Make Things Interactive) project: Challenge 2 - Chindogu and Travel

  Codes that are used in this project include:
  - "RFID tag present or removed ? Arduino MFRC522 tutorial" from Asim Zulfiqar published March 11, 2020 on High Voltages 
    https://highvoltages.co/tutorial/arduino-tutorial/arduino-mfrc522-tutorial-is-rfid-tag-present-or-removed/
  
  Libraries that are used for this project include:
  - MFRC522 library by Miguel balboa https://github.com/miguelbalboa/rfid 
  - CuteBuzzerSounds library by Everton Ramires https://www.arduino.cc/reference/en/libraries/cutebuzzersounds/

  Inputs are: Photoresistor, Toggleswitch, RFID sensor
  Outputs are: LED, Servomotor, Piezo Buzzer

  Find circuit and full instructions here: https://github.com/olivialinaa/MTI-Projects/tree/main/Challenge%202:%20Chindogu
  */

//set up photoresistor
  //constant variables won't change values
  const int LIGHT_SENSOR_PIN = A0; 
  const int ANALOG_THRESHOLD = 300;
  int analogValue;

//set up toggleswitch
  const int toggleswitch= 8;
  int value;

//set up RFID sensor 
  #include <SPI.h>
  #include <MFRC522.h>
  int RST_PIN = 9;           
  int SS_PIN = 10; 
  // Create MFRC522 instance 
  MFRC522 mfrc522(SS_PIN, RST_PIN);   

  uint8_t buf[10]= {};
  MFRC522::Uid id;
  MFRC522::Uid id2;
  bool is_card_present = false;
  uint8_t control = 0x00;

  void PrintHex(uint8_t *data, uint8_t length) {
    char tmp[16];
    for (int i=0; i<length; i++) { 
      sprintf(tmp, "0x%.2X",data[i]); 
      Serial.print(tmp); Serial.print(" ");
      }
  }
   
  void cpid(MFRC522::Uid *id){
    memset(id, 0, sizeof(MFRC522::Uid));
    memcpy(id->uidByte, mfrc522.uid.uidByte, mfrc522.uid.size);
    id->size = mfrc522.uid.size;
    id->sak = mfrc522.uid.sak;
  }

//set up LED
   int Green_LED = 7; 

//set up servo motor
  #include <Servo.h>
  // create servo object to control a servo
  Servo myservo;  

//set up buzzer
  int buzz = 4;
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
                                     
 
void setup(){
  //open serial connection
  Serial.begin(9600);

  //define input
  pinMode(toggleswitch, INPUT);

  //define output
  pinMode(Green_LED,OUTPUT);
  pinMode(buzz,OUTPUT);
  cute.init(buzz);

  //attach servo to pin 5
  myservo.attach(2);  

  //init SPI bus for RFID                                           
  SPI.begin();                                                
  mfrc522.PCD_Init(); 
}

void loop(){

  //put servo in right starting position
  myservo.write(110);

  // read the input on analog pin from photoresistor
  analogValue = analogRead(LIGHT_SENSOR_PIN); 
  // show brightness status in serial monitor
  Serial.print("Brightness:");
  Serial.println(analogValue);

  // read the value from the toggleswitch
  value= digitalRead(toggleswitch);
  // show if screen is turned on or off in serial monitor
  if (value == HIGH) {
    Serial.println("Screen is turned on");
  }
  else {
    Serial.println("Screen is turned off");
  }

  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
  MFRC522::StatusCode status;
 
  // Look for new cards
  if ( !mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if ( !mfrc522.PICC_ReadCardSerial()) {
    return;
  }
 
  bool result = true;
  uint8_t buf_len=4;
  cpid(&id);
  Serial.print("NewCard ");
  PrintHex(id.uidByte, id.size);
  Serial.println("");

 /*while a card is presented to the RFID reader AND screen is on (toggle switch on) AND brightness is above threshhold,
 move servo motor to 180 degrees, turn on green LED and play buzzer sound */
  while((true) && (analogValue > ANALOG_THRESHOLD) && (value == HIGH)){
  //move servo to 90 degrees
  myservo.write(0);
  //turn on LED
  digitalWrite(Green_LED,HIGH);
  //play sound from buzzer 
  cute.play(S_OHOOH2);
  control=0;
  for(int i=0; i<3; i++){
    if(!mfrc522.PICC_IsNewCardPresent()){
      if(mfrc522.PICC_ReadCardSerial()){
        //Serial.print('a');
        control |= 0x16;
      }
      if(mfrc522.PICC_ReadCardSerial()){
        //Serial.print('b');
        control |= 0x16;
      }
      //Serial.print('c');
        control += 0x1;
    }
    //Serial.print('d');
    control += 0x4;
  }
   
  Serial.println(control);
  if(control == 13 || control == 14){
  //card is still there
  } else {
    break;
  }
}

  //if card is removed, move servo back to 0 degrees and turn LED off
  Serial.println("CardRemoved");
  myservo.write(110);
  digitalWrite(Green_LED,LOW);
  delay(500); 
 
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
   
}
