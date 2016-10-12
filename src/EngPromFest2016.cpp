#include <Arduino.h>
#include <inttypes.h>
#include <Servo.h>

#define PRINT_VALS 1

//Пины датчиков
#define LEFT_PIN 10
#define CENTER_PIN 9
#define RIGHT_PIN 8

//Пины моторов
#define ML_PIN1 7
#define ML_PIN2 6
#define MR_PIN1 46
#define MR_PIN2 45

//Пин сервы поднтия руки
#define ARM_PIN 5

#define MOTOR_SPEED 128       //Скорость мотора (x/255)
#define THRESHOLD_LEFT 950
#define THRESHOLD_CENTER 950
#define THRESHOLD_RIGHT 970

#define ARM_MOVE_TIME 300     //Время поднятия руки на нужный угол

//Функции управления движением
void forward();
void backward();
void turn_left();
void turn_right();
void stop();

void arm_up();
void arm_down();

//Серва руки
Servo arm;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13,OUTPUT);
}

void loop()
 {

   //Отладочный вывод значекний с датчиков
   #ifdef PRINT_VALS
   uint16_t left_v = analogRead(LEFT_PIN);
   uint16_t center_v = analogRead(CENTER_PIN);
   uint16_t right_v = analogRead(RIGHT_PIN);

   Serial.print(left_v);Serial.print(' ');
   Serial.print(center_v);Serial.print(' ');
   Serial.println(right_v);
   #endif

  uint8_t left = analogRead(LEFT_PIN)  > THRESHOLD_LEFT;
  uint8_t center = analogRead(CENTER_PIN) > THRESHOLD_CENTER;
  uint8_t right = analogRead(RIGHT_PIN) > THRESHOLD_RIGHT;

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

    //Установка двигателя
    arm.attach(ARM_PIN);
    stop();
    delay(1000);
    arm_up();
    delay(2000);
    arm_down();
    delay(500);

    //Программа завершена

    while(1)
    {
      digitalWrite(13,HIGH);
      delay(100);
      digitalWrite(13, LOW);
      delay(100);
    }

  }

  //delay(100);
}

//Функции движения
void forward()
{
  analogWrite(ML_PIN1, 0);
  analogWrite(ML_PIN2, MOTOR_SPEED);
  analogWrite(MR_PIN1, 0);
  analogWrite(MR_PIN2, MOTOR_SPEED);
}

void backward()
{
  analogWrite(ML_PIN1, MOTOR_SPEED);
  analogWrite(ML_PIN2, 0);
  analogWrite(MR_PIN1, MOTOR_SPEED);
  analogWrite(MR_PIN2, 0);
}

void turn_left()
{
  analogWrite(ML_PIN1, MOTOR_SPEED);
  analogWrite(ML_PIN2, 0);
  analogWrite(MR_PIN1, 0);
  analogWrite(MR_PIN2, MOTOR_SPEED);
}

void turn_right()
{
  analogWrite(ML_PIN1, 0);
  analogWrite(ML_PIN2, MOTOR_SPEED);
  analogWrite(MR_PIN1, MOTOR_SPEED);
  analogWrite(MR_PIN2, 0);
}

void stop()
{
  analogWrite(ML_PIN1, 0);
  analogWrite(ML_PIN2, 0);
  analogWrite(MR_PIN1, 0);
  analogWrite(MR_PIN2, 0);
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
