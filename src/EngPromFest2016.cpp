#include <Arduino.h>
#include <inttypes.h>
#include <Servo.h>

#define LEFT_PIN 10
#define CENTER_PIN 9
#define RIGHT_PIN 8

#define ML_PIN1 7
#define ML_PIN2 6
#define MR_PIN1 46
#define MR_PIN2 45

#define ARM_PIN 5

#define THRESHOLD 1000

#define ARM_MOVE_TIME 1000

void forward();
void backward();
void turn_left();
void turn_right();
void stop();

void arm_up();
void arm_down();

Servo arm;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(ML_PIN1, OUTPUT);
  pinMode(ML_PIN2, OUTPUT);
  pinMode(MR_PIN1, OUTPUT);
  pinMode(MR_PIN2, OUTPUT);
  pinMode(13,OUTPUT);

  arm.attach(ARM_PIN);
}

void loop()
 {

   #ifdef PRINT_VALS
   uint16_t left_v = analogRead(LEFT_PIN);
   uint16_t center_v = analogRead(CENTER_PIN);
   uint16_t right_v = analogRead(RIGHT_PIN);

   Serial.print(left_v);Serial.print(' ');
   Serial.print(center_v);Serial.print(' ');
   Serial.println(right_v);
   #endif

  uint8_t left = analogRead(LEFT_PIN)  > THRESHOLD;
  uint8_t center = analogRead(CENTER_PIN) > THRESHOLD;
  uint8_t right = analogRead(RIGHT_PIN) > THRESHOLD;

  if(!left && center && !right)
  {
    //Move forward
    forward();
    Serial.println("forward");
  }
  if(left && center && !right)
  {
    //Turn left
    turn_left();
    Serial.println("left");
  }
  if(!left && center && right)
  {
    //Turn right
    turn_right();
    Serial.println("right");
  }
  else if(left && right && center)
  {
    //Stop
    Serial.println("stop");
    stop();
    delay(1000);
    arm_up();
    delay(2000);
    arm_down();
    delay(500);

    while(1);
  }
  /*else
  {
    Serial.println("wtf");
  }*/
  /*else
  {
    //WTF
    while(1)
    {
      digitalWrite(13,HIGH);
      delay(100);
      digitalWrite(13,LOW);
      delay(100);
    }
  }*/

  delay(100);
}


void forward()
{
  digitalWrite(ML_PIN1, LOW);
  digitalWrite(ML_PIN2, HIGH);
  digitalWrite(MR_PIN1, LOW);
  digitalWrite(MR_PIN2, HIGH);
}

void backward()
{
  digitalWrite(ML_PIN1, HIGH);
  digitalWrite(ML_PIN2, LOW);
  digitalWrite(MR_PIN1, HIGH);
  digitalWrite(MR_PIN2, LOW);
}

void turn_left()
{
  digitalWrite(ML_PIN1, HIGH);
  digitalWrite(ML_PIN2, LOW);
  digitalWrite(MR_PIN1, LOW);
  digitalWrite(MR_PIN2, HIGH);
}

void turn_right()
{
  digitalWrite(ML_PIN1, LOW);
  digitalWrite(ML_PIN2, HIGH);
  digitalWrite(MR_PIN1, HIGH);
  digitalWrite(MR_PIN2, LOW);
}

void stop()
{
  digitalWrite(ML_PIN1, LOW);
  digitalWrite(ML_PIN2, LOW);
  digitalWrite(MR_PIN1, LOW);
  digitalWrite(MR_PIN2, LOW);
}

void arm_up()
{
    Serial.println("arm up");
    arm.writeMicroseconds(1200);
    delay(ARM_MOVE_TIME);
    arm.writeMicroseconds(1500);
}

void arm_down()
{
  Serial.println("arm down");
  arm.writeMicroseconds(1800);
  delay(ARM_MOVE_TIME);
  arm.writeMicroseconds(1500);
}
