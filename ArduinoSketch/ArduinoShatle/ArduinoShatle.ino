#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeMegaPiPro.h>
#include <MePS2.h>

#ifdef MeAuriga_H
MePS2 MePS2(PORT_16);
#elif defined(MeMegaPi_H)
MePS2 MePS2(PORT_15);
#elif defined(MeMegaPiPro_H)
MePS2 MePS2(PORT_15);
#else
MePS2 MePS2(PORT_5);
#endif



MeEncoderNew superMotor_0_1(0x09+0,1);
MeEncoderNew superMotor_0_2(0x09+0,2);
MeEncoderNew superMotor_7_1(0x09+7,1);
MeEncoderNew superMotor_7_2(0x09+7,2);


void Forwardspeed(double S);
void Backwardspeed(double S);
void Right(double S);

void Stop();
void Motor();


void leftspeed(double S);
void turnrightspeed(double S);
void backrightspeed(double S);

void turnleftspeed(double S);

void frontrightspeed(double S);
void frontleftspeed(double S);
void backleftspeed(double S);


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

MeEncoderOnBoard Encoder_1(SLOT1);
MeEncoderOnBoard Encoder_2(SLOT2);
MeEncoderOnBoard Encoder_3(SLOT3);
MeEncoderOnBoard Encoder_4(SLOT4);

void isr_process_encoder1(void)
{
    if(digitalRead(Encoder_1.getPortB()) == 0){
        Encoder_1.pulsePosMinus();
    }else{
        Encoder_1.pulsePosPlus();
    }
}
void isr_process_encoder2(void)
{
    if(digitalRead(Encoder_2.getPortB()) == 0){
        Encoder_2.pulsePosMinus();
    }else{
        Encoder_2.pulsePosPlus();
    }
}
void isr_process_encoder3(void)
{
    if(digitalRead(Encoder_3.getPortB()) == 0){
        Encoder_3.pulsePosMinus();
    }else{
        Encoder_3.pulsePosPlus();
    }
}
void isr_process_encoder4(void)
{
    if(digitalRead(Encoder_4.getPortB()) == 0){
        Encoder_4.pulsePosMinus();
    }else{
        Encoder_4.pulsePosPlus();
    }
}

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

void getjoystickrate()
{
    x = (1) * ((MePS2.MeAnalog(4)) / (255));
    y = (-1) * ((MePS2.MeAnalog(2)) / (255));
    a = (1) * ((MePS2.MeAnalog(6)) / (255));
}


void computespeed(double X, double Y, double A)
{
    Motor1speed = ((Y) + (X)) - (A);
    Motor2speed = ((Y) - (X)) + (A);
    Motor3speed = ((Y) - (X)) - (A);
    Motor4speed = ((Y) + (X)) + (A);
    TempMax = Motor1speed;
    
    if((TempMax) < (Motor2speed)){
        TempMax = Motor2speed;
    }
    
    if((TempMax) < (Motor3speed)){
        TempMax = Motor3speed;
    }
    
    if((TempMax) < (Motor4speed)){
        TempMax = Motor4speed;
    }
    
    if((TempMax) > (MaxLinespeed)){
        Motor1speed = ((Motor1speed) / (TempMax)) * (MaxLinespeed);
        Motor2speed = ((Motor2speed) / (TempMax)) * (MaxLinespeed);
        Motor3speed = ((Motor3speed) / (TempMax)) * (MaxLinespeed);
        Motor4speed = ((Motor4speed) / (TempMax)) * (MaxLinespeed);
    }
    
}



void _loop(){
    MePS2.loop();
        
}
void _delay(float seconds){
    long endTime = millis() + seconds * 1000;
    while(millis() < endTime)_loop();
}

void switchmode()
{
    if(MePS2.ButtonPressed(5)){
        //MePS2.ButtonPressed(5) = L1 на джойстике
        Serial.println("Mode 0");
        Mode = 0;
    }else{
        if(MePS2.ButtonPressed(1)){
          //MePS2.ButtonPressed(1) = R1 на джойстике
            Mode = 1;
            //Mode = 0;
            Serial.println("Mode 1");
        }
    }
    
    if(MePS2.ButtonPressed(7)){
      //MePS2.ButtonPressed(7) = L2 на джойстике
       Serial.println("ButtonPressed(7)");
       // увеличение скорости
        if((Mode)==(0))
        {
           if((KeySpeed) < (255))
           {
                Linespeed += 5;
                Anglespeed += 5;
                KeySpeed += 5;
           }
        }else
        {
            if((KeySpeed) < (255))
            {
                Linespeed += 5;
                Anglespeed += 5;
                KeySpeed += 5;
            }
        }
        _delay(0.05);
    }else{
        if(MePS2.ButtonPressed(3)){
         //MePS2.ButtonPressed(3) = R2 на джойстике
         Serial.println("ButtonPressed(3)");
         // уменьшение скорости 
            if(((Mode)==(0)))
            {
                if((KeySpeed) > (0))
                {
                    Linespeed += -5;
                    Anglespeed += -5;
                    KeySpeed += -5;
                }
            }else{
                if((KeySpeed) > (0))
                {
                    Linespeed += -5;
                    Anglespeed += -5;
                    KeySpeed += -5;
                }
            }
            _delay(0.05);
        }
    }
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

void controlmode()
{
    if( MePS2.ButtonPressed(14) )  {
      //MePS2.ButtonPressed(3) = вперед на джойстике
      Serial.println("ButtonPressed(14)");
      Forwardspeed(KeySpeed);
    } else if( MePS2.ButtonPressed(15) ) {
      //MePS2.ButtonPressed(15) = вниз джойстике
      Serial.println("ButtonPressed(15)");
      Backwardspeed(KeySpeed);
    } else if( MePS2.ButtonPressed(17) ) {
      //MePS2.ButtonPressed(17) = вправо на джойстике
      Serial.println("ButtonPressed(17)");
      frontrightspeed(KeySpeed);
    } else if( MePS2.ButtonPressed(16) ) {
      //MePS2.ButtonPressed(16) = влево на джойстике
      Serial.println("ButtonPressed(16)");
      frontleftspeed(KeySpeed);
    } else if( (MePS2.ButtonPressed(14)) && (MePS2.ButtonPressed(17)) ) {
      //MePS2.ButtonPressed(3) = R2 на джойстике
      Serial.println("ButtonPressed(14) ButtonPressed(17)");
      frontrightspeed(KeySpeed);
    } else if( (MePS2.ButtonPressed(14)) && (MePS2.ButtonPressed(16)) ) {
      //MePS2.ButtonPressed(3) = R2 на джойстике
      Serial.println("ButtonPressed(14) ButtonPressed(16)");
      frontleftspeed(KeySpeed);
    } else if( (MePS2.ButtonPressed(15)) && (MePS2.ButtonPressed(16)) ) {
      //MePS2.ButtonPressed(3) = R2 на джойстике
      Serial.println("ButtonPressed(15) ButtonPressed(16)");
      backleftspeed(KeySpeed);
    } else if( (MePS2.ButtonPressed(15)) && (MePS2.ButtonPressed(17)) ) {
      //MePS2.ButtonPressed(3) = R2 на джойстике
      Serial.println("ButtonPressed(15) ButtonPressed(17)");
      backrightspeed(KeySpeed);
    } else if( MePS2.ButtonPressed(11) ) {
      //MePS2.ButtonPressed(3) = R2 на джойстике
      Serial.println("ButtonPressed(11)");
      Right(KeySpeed);
    } else if( MePS2.ButtonPressed(9) ) {
      //MePS2.ButtonPressed(3) = R2 на джойстике
      Serial.println("ButtonPressed(9)");
      leftspeed(KeySpeed);
    } else if( MePS2.ButtonPressed(12) ) {
      //MePS2.ButtonPressed(3) = R2 на джойстике
      Serial.println("ButtonPressed(12)");
      turnrightspeed(KeySpeed);
    } else if( MePS2.ButtonPressed(10) ) {
      //MePS2.ButtonPressed(3) = R2 на джойстике
      Serial.println("ButtonPressed(10)");
      turnleftspeed(KeySpeed);
    } else {joystickcontrol();}
}

void testmode()
{

//if (Serial.available()!=0) {
//  leftspeed(KeySpeed);
 // char b = Serial.read();
 // Serial.print(b);
//}
//else {
 // Stop();

 if (Serial.available() > 0) 
 {
      // A = Serial.parseInt();
    A = Serial.read();
    Serial.println(A, DEC);
      // Serial.read();
    // Serial.flush();
    //Serial.print(A);
 }
// if (A == 10) {
//      Forwardspeed(KeySpeed);
//      A = 0;
//      delay(1000);
//      //Stop();
//}
// else if (A==0) {
//      Stop();
// }
//  else if (A==5) {
//      frontrightspeed(KeySpeed);
// }
//  else if (A==-5) {
//      frontleftspeed(KeySpeed);
// }
//  else if (A==1) {
//      turnrightspeed(KeySpeed);
// }
//  else if (A==-1) {
//      turnleftspeed(KeySpeed);
// }
// else {Stop();}
 switch(A) {
    case 65: //A
    {
      Serial.println("65/A from serial");
      Forwardspeed(KeySpeed);
      A = 0;
      break;
    }
    case 83: //0 S
    {
      Serial.println("83/0 from serial");
      Stop();
      A = 0;
      break;
    }
    case 82: //5 R
    {
      Serial.println("82/5 from serial");
      frontrightspeed(KeySpeed);
      A = 0;
      break;
    }
    case 76: //-5 L
    {
      Serial.println("76/-5 from serial");
      frontleftspeed(KeySpeed);
      A = 0;
      break;
    }
    case 114: //1 r
    {
      Serial.println("114/1 from serial");
      turnrightspeed(KeySpeed);
      A = 0;
      break; 
    }
    case 108: //-1 l
    {
      Serial.println("108/1 from serial");
      turnleftspeed(KeySpeed);
      A = 0;
      break;
    }
    case 7:
    {
       Serial.println("7 from serial");
       turnrightspeed(130);
       delay(750);
       Stop();
       A = 0;
       break;
    }
    case -7:
    {
       turnleftspeed(130);
       delay(750);
       Stop();
       A = 0;
       break;
    }
    default:
    {
      A = 0;
      break;
    }
    //Stop();
      
 }
// Stop();
}

void joystickcontrol()
{
    getjoystickrate();
    computespeed((x) * (Linespeed),(y) * (Linespeed),(a) * (Anglespeed));
    Motor();
    
}



void setup(){
    superMotor_0_1.begin();
    superMotor_0_2.begin();
    superMotor_7_1.begin();
    superMotor_7_2.begin();
    MePS2.begin(115200);
    Serial.begin(9600);
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
    int S = 200;
    superMotor_0_1.setPWM((-1) * (S));
    superMotor_0_2.setPWM((-1) * (S));
    superMotor_7_1.setPWM((1) * (S));
    superMotor_7_2.setPWM((1) * (S));
    MePS2.loop();
}
