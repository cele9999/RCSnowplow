////////////////////////////
//Title: SnowByte         //
//Author: Gerardo Cruz    //
//Date: 1/14/23           //
//License: CC BY-NC-SA 4.0//
////////////////////////////

//Var Setup
bool control = 0;                  //Shows if controller has been detected since power up
int speed = 75;                    //Dictates initial speed of the motors. Value can be set between 0-255
int oldoption = 0;                 //Holds the last RC command
int option = 0;                    //Stores any new RC button presses
int sidectrl, fwdctrl, leverctrl;  //Store RF Receiver values
//Arduino Variable  --> Arduino Pin --> TYPE   --> Description
//---------------------------------------------------------------
const int ch1 = 2;     // --> D2          --> INPUT  --> channel 1 of RF Receiver
const int ch2 = 3;     // --> D3          --> INPUT  --> channel 2 of RF Receiver
const int ch4 = 4;     // --> D4          --> INPUT  --> channel 4 of RF Receiver
const int en1 = 5;     // --> D5          --> OUTPUT --> enable 1 of Motor Shield
const int en2 = 6;     // --> D6          --> OUTPUT --> enable 2 of Motor Shield
const int in1 = 7;     // --> D7          --> OUTPUT --> input 1 of Motor Shield
const int in2 = 8;     // --> D8          --> OUTPUT --> input 2 of Motor Shield
const int in3 = 9;     // --> D9          --> OUTPUT --> input 3 of Motor Shield
const int in4 = 10;    // --> D10        --> OUTPUT --> input 4 of Motor Shield
const int armup = 11;  // --> D11      --> OUTPUT --> Linear Actuator Up
const int armdn = 12;  // --> D12      --> OUTPUT --> Linear Actuator Down

//Motor Speed Increasing Loop
void speedcontrol() {
  if ((oldoption == option) && (oldoption != 0) && (option != 0)) {
    if (speed < 256) {
      speed += 3;
      analogWrite(en1, speed);
      analogWrite(en2, speed);
    }
  } else {
    speed = 75;
    oldoption = option;
  }
}

//Setup Loop
void setup() {
  //ASSIGN PINS MODES AND VALUES
  Serial.begin(9600);
  pinMode(ch1, INPUT);
  pinMode(ch2, INPUT);
  pinMode(ch4, INPUT);
  pinMode(en1, OUTPUT);
  pinMode(en2, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(armup, OUTPUT);
  pinMode(armdn, OUTPUT);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  digitalWrite(armup, LOW);
  digitalWrite(armdn, LOW);
  //ENABLE AND SET INITIAL MOTOR SPEED
  analogWrite(en1, speed);
  analogWrite(en2, speed);
}

//Main Loop
void loop() {
  //READ RF RECEIVER SIGNALS
  sidectrl = constrain(pulseIn(ch1, HIGH), 1000, 2000);
  fwdctrl = constrain(pulseIn(ch2, HIGH), 1000, 2000);
  leverctrl = constrain(pulseIn(ch4, HIGH), 1000, 2000);
  Serial.println(sidectrl);
  Serial.println(fwdctrl);
  Serial.println(leverctrl);
  //WHEELS TURN LEFT/RIGHT OR NOT
  if (sidectrl == 1000 && fwdctrl == 1000 && leverctrl == 1000 && control == 0) {
    Serial.println("No controller detected");
  } else if (sidectrl <= 1600 && sidectrl >= 1400) {
    //WHEELS GO FORWARD/BACKWARDS OR NOT
    Serial.println("ch1=~1500");
    control = 1;
    if (fwdctrl <= 1600 && fwdctrl >= 1400) {
      Serial.println("ch2=~1500");
      option = 0;
      speedcontrol();
      analogWrite(in1, 0);
      analogWrite(in2, 0);
      analogWrite(in3, 0);
      analogWrite(in4, 0);
    } else if (fwdctrl > 1600) {
      Serial.println("ch2=~2000");
      option = 22;
      speedcontrol();
      analogWrite(in1, 0);
      analogWrite(in2, map(fwdctrl, 1600, 2000, 51, 255));
      analogWrite(in3, 0);
      analogWrite(in4, map(fwdctrl, 1600, 2000, 51, 255));
    } else if (fwdctrl < 1400) {
      Serial.println("ch2=~1000");
      option = 21;
      speedcontrol();
      analogWrite(in1, map(fwdctrl, 1400, 1000, 51, 255));
      analogWrite(in2, 0);
      analogWrite(in3, map(fwdctrl, 1400, 1000, 51, 255));
      analogWrite(in4, 0);
    } else {
      Serial.println("ch2=~1500-last");
      option = 0;
      speedcontrol();
      analogWrite(in1, 0);
      analogWrite(in2, 0);
      analogWrite(in3, 0);
      analogWrite(in4, 0);
    }
  } else if (sidectrl > 1600) {
    Serial.println("ch1=~2000");
    control = 1;
    option = 12;
    speedcontrol();
    analogWrite(in1, 0);
    analogWrite(in2, map(sidectrl, 1600, 2000, 51, 255));
    analogWrite(in3, map(sidectrl, 1600, 2000, 51, 255));
    analogWrite(in4, 0);
  } else if (sidectrl < 1400) {
    Serial.println("ch1=~1000");
    control = 1;
    option = 11;
    speedcontrol();
    analogWrite(in1, map(sidectrl, 1400, 1000, 51, 255));
    analogWrite(in2, 0);
    analogWrite(in3, 0);
    analogWrite(in4, map(sidectrl, 1400, 1000, 51, 255));
  }
  //LEVER UP/DOWN
  if (sidectrl == 1000 && fwdctrl == 1000 && leverctrl == 1000 && control == 0) {
    Serial.println("No controller detected");
  } else if (leverctrl <= 1600 && leverctrl >= 1400) {
    Serial.println("ch4=~1500");
    control = 1;
    digitalWrite(armup, LOW);
    digitalWrite(armdn, LOW);
  } else if (leverctrl > 1600) {
    Serial.println("ch4=~2000");
    control = 1;
    digitalWrite(armup, HIGH);
    digitalWrite(armdn, LOW);
  } else if (leverctrl < 1400) {
    Serial.println("ch4=~1000");
    control = 1;
    digitalWrite(armup, LOW);
    digitalWrite(armdn, HIGH);
  } else {
    Serial.println("ch4=~1500-last");
    control = 1;
    digitalWrite(armup, LOW);
    digitalWrite(armdn, LOW);
  }
  delay(50);
}