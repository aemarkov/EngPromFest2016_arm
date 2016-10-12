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

#define MOTOR_SPEED 70      //Скорость мотора (x/255)
#define MOTOR_TURN_SPEED 110


#define THRESHOLD_LEFT 900
#define THRESHOLD_CENTER 900
#define THRESHOLD_RIGHT 900


#define ARM_MOVE_TIME 300     //Время поднятия руки на нужный угол

//Функции управления движением
void forward(uint8_t speed);
void turn_left(uint8_t turn_speed);
void turn_right(uint8_t turn_speed);
void stop();

void arm_up();
void arm_down();

//Серва руки
Servo arm;
uint8_t last_turn = 0;

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

    //going
  forward(MOTOR_SPEED);

  if(left && right && center)
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
  //if(!left && center && !right)
  //  last_turn = 0;

  if(left&&!right)
  {
    turn_left(MOTOR_TURN_SPEED);
    //last_turn = 1;
  }
  if(right&&!left)
  {
    turn_right(MOTOR_TURN_SPEED);
    //last_turn = 2;
  }

  /*if(!center)
  {
      if(last_turn==1)
        turn_left(MOTOR_TURN_SPEED/3);
      else if(last_turn==2)
        turn_right(MOTOR_TURN_SPEED/3);
  }*/

  //delay(100);
}

//Функции движения
void forward(uint8_t speed)
{
  analogWrite(ML_PIN1, 0);
  analogWrite(ML_PIN2, speed);
  analogWrite(MR_PIN1, 0);
  analogWrite(MR_PIN2, speed);
}

void backward(uint8_t speed)
{
  analogWrite(ML_PIN1, speed);
  analogWrite(ML_PIN2, 0);
  analogWrite(MR_PIN1, speed);
  analogWrite(MR_PIN2, 0);
}

void turn_left(uint8_t turn_speed)
{
  analogWrite(ML_PIN1, turn_speed+90); //0
  analogWrite(ML_PIN2, 0);
  analogWrite(MR_PIN1, 0);
  analogWrite(MR_PIN2, turn_speed+50);
}

void turn_right(uint8_t turn_speed)
{
  analogWrite(ML_PIN1, 0);
  analogWrite(ML_PIN2, turn_speed+120); //+50
  analogWrite(MR_PIN1, turn_speed);
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
