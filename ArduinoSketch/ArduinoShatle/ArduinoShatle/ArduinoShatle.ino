#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeMegaPiPro.h>
#include <MePS2.h>
#include "ArduinoJson.h"


#ifdef MeAuriga_H
MePS2 MePS2(PORT_16);
#elif defined(MeMegaPi_H)
MePS2 MePS2(PORT_15);
#elif defined(MeMegaPiPro_H)
MePS2 MePS2(PORT_15);
#else
MePS2 MePS2(PORT_5);
#endif

StaticJsonDocument<200> jsondoc;

// переменные скорости моторов
int16_t speedL = 0;
int16_t speedR = 0;

MeEncoderNew superMotor_0_1(0x09+0,1);
MeEncoderNew superMotor_0_2(0x09+0,2);
MeEncoderNew superMotor_7_1(0x09+7,1);
MeEncoderNew superMotor_7_2(0x09+7,2);

// void Forwardspeed(double S);
// void Backwardspeed(double S);
// void Right(double S);
//
// void Stop();
// void Motor();
//
//
// void leftspeed(double S);
// void turnrightspeed(double S);
// void backrightspeed(double S);
//
// void turnleftspeed(double S);
//
// void frontrightspeed(double S);
// void frontleftspeed(double S);
// void backleftspeed(double S);


int A=0;
double x;
double y;
double a;
char b;

void getjoystickrate();
void switchmode();
void computespeed(double X, double Y, double A);
void controlmode();
void joystickcontrol();
void testmode();
void default1();

double Motor1speed;
double Motor2speed;
double Motor3speed;
double Motor4speed;
double TempMax;
double MaxLinespeed;
double Mode;
double KeySpeed;
double Linespeed;
double Anglespeed;
double MaxAnglespeed;
double MinLinespeed;
double MinAnglespeed;

//------------------------------------------------------------------------------------//


void Left(double S) {
    superMotor_0_1.setPWM((-1) * (S));
    superMotor_0_2.setPWM((-1) * (S));
    superMotor_7_1.setPWM((1) * (S));
    superMotor_7_2.setPWM((1) * (S));
}

void Right(double S) {
    superMotor_0_1.setPWM((1) * (S));
    superMotor_0_2.setPWM((1) * (S));
    superMotor_7_1.setPWM((-1) * (S));
    superMotor_7_2.setPWM((-1) * (S));
}

void Forward(double S) {
    superMotor_0_1.setPWM((-1) * (S));
    superMotor_0_2.setPWM((1) * (S));
    superMotor_7_1.setPWM((-1) * (S));
    superMotor_7_2.setPWM((1) * (S));
}

void Back(double S) {
    superMotor_0_1.setPWM((1) * (S));
    superMotor_0_2.setPWM((-1) * (S));
    superMotor_7_1.setPWM((1) * (S));
    superMotor_7_2.setPWM((-1) * (S));
}

void Stop()
{
    superMotor_0_1.setPWM(0); 
    superMotor_0_2.setPWM(0);   
    superMotor_7_1.setPWM(0); 
    superMotor_7_2.setPWM(0);   
}

//-----------------------------------------------------------------------------------------//

void _loop(){
    MePS2.loop();
        
}
void _delay(float seconds){
    long endTime = millis() + seconds * 1000;
    while(millis() < endTime)_loop();
}

void default1()
{
    Mode = 0;

    KeySpeed = 70;
    Linespeed = 130;
    Anglespeed = 130;
    MaxLinespeed = 255;
    MaxAnglespeed = 255;
    MinLinespeed = 30;
    MinAnglespeed = 30; 
}

void refreshSpeed(double Lspeed, double Rspeed) {
    double Speed1;
    double Speed2;
    double Speed3;
    double Speed4;

    if ((Lspeed >= 0 && Rspeed >= 0) || (Lspeed <= 0 && Rspeed <= 0)) {//вперед или назад 
      Speed1 = Lspeed;
      Speed2 = Rspeed;
      Speed3 = Lspeed;
      Speed4 = Rspeed;
    } else if(Lspeed >= 0 && Rspeed <= 0){//вправо
      Speed1 = Lspeed;
      Speed2 = Rspeed;
      Speed3 = Rspeed;
      Speed4 = Lspeed;
    } else if(Lspeed <= 0 && Rspeed >= 0){//влево
      Speed1 = Lspeed;
      Speed2 = Rspeed;
      Speed3 = Rspeed;
      Speed4 = Lspeed;
    } else {
      Speed1 = 0;
      Speed2 = 0;
      Speed3 = 0;
      Speed4 = 0;
    }
    
    superMotor_0_1.setPWM((-1) *(Speed3));//зад лев инверс
    superMotor_0_2.setPWM((1) * (Speed4));//зад прав
    superMotor_7_1.setPWM((-1) *(Speed1));//перед лев инверс
    superMotor_7_2.setPWM((1) * (Speed2));//перед прав
}

void setup(){
    superMotor_0_1.begin();
    superMotor_0_2.begin();
    superMotor_7_1.begin();
    superMotor_7_2.begin();
    MePS2.begin(115200);
    Serial.begin(115200);
    TCCR1A = _BV(WGM10);//PIN12
    TCCR1B = _BV(CS11) | _BV(CS10) | _BV(WGM12);
    TCCR2A = _BV(WGM21) | _BV(WGM20);//PIN8
    TCCR2B = _BV(CS22);
    TCCR3A = _BV(WGM30);//PIN9
    TCCR3B = _BV(CS31) | _BV(CS30) | _BV(WGM32);
    TCCR4A = _BV(WGM40);//PIN5
    TCCR4B = _BV(CS41) | _BV(CS40) | _BV(WGM42);
    default1();
    }
    
void loop(){
  DeserializationError err = deserializeJson(jsondoc, Serial);  // получаем сообщение от мини ПК Шатл через uart
  
  if (err == DeserializationError::Ok)    // если cообщение принято
  {
    speedL = (float)jsondoc["speedA"];  // приходит диапазон [-100; 100]
    speedR = (float)jsondoc["speedB"];  // расширяем до [-255; 255]
  }
  else
  {
    while (Serial.available() > 0) Serial.read(); // чистим буфер
  }
  refreshSpeed(speedL, speedR);
  MePS2.loop();
}
