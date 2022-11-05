String msg;             // переменная для хранения сообщения
byte parseStart = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}

void moveRobot() {
  String data = serialEvent();
  int maxIndex = data.length();
  for (int i = 0; i < maxIndex; i++) {
    if (!(data[i] == '\n' || data[i] == '\r')){
      if (data[i] == 'L') {
        Mouvement('F', msg)
      }
    }
    
  }
}

String serialEvent() {
  String input = '';
  bool flag = false;
  while (Serial.available()) {
    // get the new byte:
    char in = (char)Serial.read(); 
    if (in == '&' && flag == false) {  
      flag = true;
    }
    if (flag == true) {
      msg += in;
    }
    if (inChar == '.') {
      flag = false;
      break;
    } 
    // add it to the inputString:
    input += inChar;
  }
  return input;
}
int Mouvement(char action, uint8_t motorSpeed){

  if (action == 'F'){ //forward
    motor1.run(motorSpeed); /* value: between -255 and 255. */
    motor2.run(motorSpeed);
    motor3.run(-motorSpeed); 
    motor4.run(-motorSpeed);
  }
  else if (action == 'L'){ //left
    motor1.run(motorSpeed); /* value: between -255 and 255. */
    motor2.run(motorSpeed);
    motor3.run(motorSpeed); 
    motor4.run(motorSpeed);
  }
  else if (action == 'R'){ //right
    motor1.run(-motorSpeed); /* value: between -255 and 255. */
    motor2.run(-motorSpeed);
    motor3.run(-motorSpeed); 
    motor4.run(-motorSpeed);
  }
  else if (action == 'B'){ //back
    motor1.run(-motorSpeed); /* value: between -255 and 255. */
    motor2.run(-motorSpeed);
    motor3.run(motorSpeed); 
    motor4.run(motorSpeed);
  }
  else{
    motor1.stop();
    motor2.stop();
    motor3.stop();
    motor4.stop(); 
  }
 
}

void serialParse() {
  if (Serial.available())                 //  если что-то пришло в Serial-порт
  {
    char in = Serial.read();              //  читаем один байт (символ)
    if (!(in == '\n' || in == '\r'))      //  отсеиваем символы возврата картеки и переноса строки
    {
        switch (in)       // в зависимости от принятого символа, делаем выбор
        {
            case 'F': parseStart = 0; Mouvement('F', msg); break;  // вперед со скоростью MSG
            case 'B': parseStart = 0; Mouvement('B', msg); break;  // назад со скоростью MSG
            case 'L': parseStart = 0; Mouvement('L', msg); break;  // влево
            case 'R': parseStart = 0; Mouvement('R', msg); break;  // вправо
            case 'S': parseStart = 0; Mouvement('S', 0); break;    // стоп
            default: Mouvement('S', 0); break; // иначе стоп
        }
        
        // если парсинг запущен и это не символы начала или окончания посылки
        if ((parseStart == 2) && (in != '#') && (in != '$') && (in != '^') && (in != '@')) 
        {  
          msg += in;    // запоминаем переданное число (складываем по одному байту в общую строку)
        }
     }
  }
  
  if(parseStart == 1)   //  если прием остановлен
  {
    // выполняем действия с полученными данными и обнуляем переменные
      parseStart = 0;
      msg = ""; 
    }
}
