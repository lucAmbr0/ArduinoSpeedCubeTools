// Arduino SpeedCube Timer and Scrambler
// https://github.com/lucAmbr0/ArduinoSpeedCubeTools
// Created by @luca.ambroo_ [instagram]
// 07/25/2022


/* 
  ________________________________________________________
  |                      USER SETUP                       |
  |                                                       |          
  | change MoYu... to your cube name                      |
  |*/ char cubeName[] = "MoYu RS3M 2020"; /*              | 
  |                                                       |
  | time (in ms) to scramble the cube before going to     |
  | next screen, customize as you want (higher value      |
  | means higher time to scramble the cube)               |
  | [to modify this setting please go to line 60]         |
  |                                                       |
  | moves are sorted in categories right/left, up/down,   |
  | front/back [RL, UD, FB] you can decide to delete      |
  | moves that you are not comfortable with               |
  |*/ char *RL[] = {  /*                                  |
  |*/ "R ", "R' ", "R2 ", "L ", "L' ", "L2 "}; /*         |
  |*/ char *UD[] = { /*                                   |
  |*/ "U ", "U' ", "U2 ", "D ", "D' ", "D2 "}; /*         |
  |*/ char *FB[] = { /*                                   |
  |*/ "F ", "F' ", "F2 ", "B ", "B' ", "B2 "}; /*         |
  |_______________________________________________________|

*/

// Libraries
#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
#include "SR04.h"
#define TRIG_PIN 2
#define ECHO_PIN 3
#define RED 5
#define BLUE 6
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);

// Variables
long dist;
float time;
float rawTime;
int delay1;
int delay2;
int delay3;
int num;
// Scramble Variables


int moveRL;
int moveUD;
int moveFB;

void setup() {
  delay1 = 500; // the first time it displays the program name for longer
  delay2 = 2000; // same as delay1
  delay3 = 10500; // time to scramble the cube before going to next screen
  Serial.begin(9600);
  Serial.println("");
  Serial.println("code by @luca.ambroo_"); // credits, please don't remove
  Serial.println("___________________________");
  Serial.println("| -- SPEEDCUBE RECORDS -- |"); // records table header
  START:
  num += 1; // times the program ran
  rawTime = 0; // resets time
  time = 0;
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  lcd.begin(16, 2);
  lcd.print("Speed-Cube Timer");
  lcd.setCursor(0, 1);
  delay(delay1);
  lcd.print("by luca.ambroo_"); // credits, please don't remove
  delay(delay2);
  lcd.clear();
  delay(500);
  lcd.clear();
  lcd.print("Scrambling...");
  delay(750);
  lcd.clear();
  randomSeed(analogRead(0));
  for (int i=0; i < 2; i++)
  {
    for (int i=0; i < 2; i++)
    {
      delay(30);
      lcd.print(RL[random(0,6)]);
      delay(30);
      lcd.print(UD[random(0,6)]);
      delay(30);
      lcd.print(FB[random(0,6)]);
    }
    lcd.setCursor(0,1);
    for (int i=0; i < 2; i++)
    {
      delay(30);
      lcd.print(RL[random(0,6)]);
      delay(30);
      lcd.print(FB[random(0,6)]);
      delay(30);
      lcd.print(UD[random(0,6)]);
    }
    delay(delay3);
    lcd.clear();
  }
  lcd.clear();
  lcd.print("Place Cube");
  digitalWrite(RED, HIGH);
  lcd.setCursor(0, 1);
  lcd.print("WAITING");
  delay(1500);
  dist=sr04.Distance();
    while (dist > 7)
    dist=sr04.Distance();
    if (dist <= 7)
     goto READY;
  READY:
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(cubeName); // change var to your cube name
    lcd.setCursor(0, 1);
    delay(400);
    lcd.print("READY");
    delay(1000);
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
  while (dist <= 7)
    dist=sr04.Distance();
    if (dist > 7) // when the cube leaves the sensor the timer starts
     goto SOLVING;
  SOLVING:
    digitalWrite(RED, LOW);
    digitalWrite(BLUE, HIGH); // The timer is now running
    lcd.print("SOLVING...");
    dist=sr04.Distance();
    rawTime = millis(); // preparing the time count
    goto TIMER;
  TIMER:
      dist=sr04.Distance(); // checks the ultrasonic sensor
      if (dist <= 7)
        goto DONE;
      goto TIMER;
  DONE: // stops the timer and gives the result
    time = (millis()-rawTime)/1000; // converting the time to seconds
    digitalWrite(BLUE, LOW);
    digitalWrite(RED, HIGH);
    lcd.clear();
    lcd.print("Cube Finished");
    lcd.setCursor(0, 1);
    if (time <= 5)
      lcd.print("WORLD RECORD?!");
    else if (time <= 20)
      lcd.print("GREAT Solve!");
    else if (time <= 40)
      lcd.print("Fast Solve!");
    else if (time <= 60)
     lcd.print("Good Solve!");
    else if (time <= 120)
      lcd.print("Done!");
    else if (time > 120)
      lcd.print("u fell asleep?");
    delay(1500);
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    delay(200);
    lcd.print("Time: ");
    lcd.print(time);
    lcd.print("sec");
    delay(3000);
    lcd.clear();
    lcd.print("Print to Serial"); // open serial monitor to view results
    Serial.print("| No.");
    Serial.print(num);
    if (num <= 9) // change the space to adjust the table width
      Serial.print("     Time: ");
    else if (num <= 99)
      Serial.print("    Time: ");
    else if (num >= 100)
      Serial.print("    Time: ");
    Serial.print(time);
    if (time >= 10) // change the space to adjust the table width
      Serial.println("sec |");
    else if (time >= 100)
      Serial.println("sec|");
    else if (time < 10)
      Serial.println(" sec |");
    lcd.setCursor(0,1);
    lcd.print("Restarting..."); // restarts the program
    digitalWrite(RED, LOW);
    delay(1000);
    lcd.clear();
    delay1 = 200;
    delay2 = 1000;
    goto START;
}

//
//              _..._
//            .'     '.      _
//           /    .-""-\   _/ \
//         .-|   /:.   |  |   |
//         |  \  |:.   /.-'-./
//         | .-'-;:__.'    =/
//         .'=  *=|     _.='
//        /   _.  |    ;
//       ;-.-'|    \   |     @luca.ambroo__
//      /   | \    _\  _\
//      \__/'._;.  ==' ==\
//              \    \   |
//              /    /   /
//             /-._/-._/
//             \   `\  \
//              `-._/._/
//