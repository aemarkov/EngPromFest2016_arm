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

#define MOTOR_SPEED 100       //Скорость мотора (x/255)
#define MOTOR_TURN_SPEED 150
#define TRIM_SPEED 50

#define THRESHOLD_LEFT 900
#define THRESHOLD_CENTER 900
#define THRESHOLD_RIGHT 900

#define ARM_MOVE_TIME 300     //Время поднятия руки на нужный угол

//Функции управления движением
void forward(uint8_t speed);
void backward(uint8_t speed);
void turn_left();
void turn_right();
void forward_turn(int8_t ang);
void stop();

void arm_up();
void arm_down();

void wait()
{
  stop();
  delay(600);
}

//Серва руки
Servo arm;
uint8_t state=0;

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
    forward(MOTOR_SPEED);
    Serial.println("forward");
    state=0;
    digitalWrite(13, LOW);
  }
  else if(!left && !center && !right)
  {
    forward(60);
    Serial.println("forward white");
    state=0;
    digitalWrite(13, LOW);
  }
  else if(left && center && !right)
  {
    //Turn left
    turn_left();
    Serial.println("left");
    state=1;
  }
  else if(left && !right && state==0)
  {
    forward_turn(TRIM_SPEED);
    digitalWrite(13,HIGH)
;  }
  if(!left && center && right)
  {
    //Turn right
    turn_right();
    Serial.println("right");
    state=2;

  }
  else if(!left && right && state==0)
  {
    forward_turn(-TRIM_SPEED);
    digitalWrite(13,HIGH);
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

void turn_left()
{
  analogWrite(ML_PIN1, MOTOR_TURN_SPEED);
  analogWrite(ML_PIN2, 0);
  analogWrite(MR_PIN1, 0);
  analogWrite(MR_PIN2, MOTOR_TURN_SPEED);
}

void turn_right()
{
  analogWrite(ML_PIN1, 0);
  analogWrite(ML_PIN2, MOTOR_TURN_SPEED);
  analogWrite(MR_PIN1, MOTOR_TURN_SPEED);
  analogWrite(MR_PIN2, 0);
}

void forward_turn(int8_t ang)
{
  analogWrite(ML_PIN1, 0);

  if(ang>0)
    analogWrite(ML_PIN2, MOTOR_SPEED+ang);

  analogWrite(MR_PIN1, 0);

  if(ang<0)
    analogWrite(MR_PIN2, MOTOR_SPEED-ang);
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
