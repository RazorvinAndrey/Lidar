#include "ArduinoJson.h"
#define L_CON_PIN   11  // Левый управляющий
#define R_CON_PIN   10  // Правый управляющий
#define L_1_PIN   9   // левый 1
#define L_2_PIN   8   // левый 2 
#define R_1_PIN   7   // правый 1
#define R_2_PIN   6   // правый 2

// json буфер для пакетов от orange pi
StaticJsonDocument<200> jsondoc;

// переменные скорости моторов
int16_t speedL = 0;
int16_t speedR = 0;
   

void setup() {
  Serial.begin(9600);
  
  pinMode(L_CON_PIN, OUTPUT);
  pinMode(R_CON_PIN, OUTPUT);
  pinMode(L_1_PIN, OUTPUT);
  pinMode(L_2_PIN, OUTPUT);
  pinMode(R_1_PIN, OUTPUT);
  pinMode(R_2_PIN, OUTPUT);

  digitalWrite(10, HIGH);
  analogWrite(11, 125);

}

void loop() {
  DeserializationError err = deserializeJson(jsondoc, Serial);  // получаем сообщение от orange pi через uart
  
  if (err == DeserializationError::Ok)    // если cообщение принято
  {
    speedL = (float)jsondoc["speedA"] * 2.55;  // приходит диапазон [-100; 100]
    speedR = (float)jsondoc["speedB"] * 2.55;  // расширяем до [-255; 255]
  }
  else
  {
    while (Serial.available() > 0) Serial.read(); // чистим буфер
  }
  moveL(speedL);  // обновляем состояние скоростей моторов
  moveR(speedR);  //
  delay(10);
}

void moveL(int16_t speed){
  //speed = -speed;
  if(speed >= 0){
    digitalWrite(L_1_PIN, LOW);
    digitalWrite(L_2_PIN, HIGH);  
    analogWrite(L_CON_PIN, abs(speed));
  } else {
    digitalWrite(L_1_PIN, HIGH);
    digitalWrite(L_2_PIN, LOW);  
    analogWrite(L_CON_PIN, abs(speed));
  }
}

void moveR(int16_t speed){
  if(speed >= 0){
    digitalWrite(R_1_PIN, HIGH);
    digitalWrite(R_2_PIN, LOW);  
    analogWrite(R_CON_PIN, abs(speed));
  } else {
    digitalWrite(R_1_PIN, LOW);
    digitalWrite(R_2_PIN, HIGH);  
    analogWrite(R_CON_PIN, abs(speed));
  }
}
