#include <Servo.h>
// #include <Wire.h>

// #define I2C_SDA A5
// #define I2C_SCL A6
// #define S11059_ADDR 0x2A

// unsigned long last_color = 0;

// void color(int color_interv);

Servo armservo;
Servo glservo;

int STCW = 3;
int STCL = 4;
int ARSV = 5;
int GLSV = 9;
int LED = 13;
int MSW = 14;

int count = 0;

void setup() {
  Serial.begin( 9600 );     // シリアル通信を初期化する。通信速度は9600bps
  Serial.println("START");//このSTARTは、デバックに便利でした//

  pinMode(STCW, OUTPUT); // 方向
  pinMode(STCL, OUTPUT); // クロック
  digitalWrite(STCW, LOW);

  pinMode(MSW, INPUT); // アームが下についていると 0
  pinMode(LED, OUTPUT);

  armservo.attach(ARSV); //10~40
  glservo.attach(GLSV);
  armservo.write(10);
  glservo.write(0);
}

void loop() {
  armservo.write(10);
  glservo.write(0);

  // Serial.println(digitalRead(MSW));

  int state = digitalRead(MSW);
  if(state == 0){
    digitalWrite(STCW,LOW);
  }
  else if(state == 1){
    digitalWrite(STCW, HIGH);
    for(int i=0; i<900; i++){
      steping_motor(); // アームを下降
      if(digitalRead(MSW) == 0){
        break;
      }
    }
    digitalWrite(STCW,LOW);
  }

  // 受信データがあった時だけ、処理を行う
  if ( Serial.available() ) {       // 受信データがあるか？
    byte key = (byte)Serial.read();            // 1文字だけ読み込む
      if ( key == 1 ) {
        arm_mode();
      }
      else {
        goal_mode();
      }
    // arm_mode();
  }
  // arm_mode();

}

void arm_mode() {
  for(int theta=25; theta<=10; theta--){
    armservo.write(theta);
    delay(1000);
  }
  digitalWrite(STCW, LOW);
  for(int i=0; i<1000; i++){
    steping_motor(); // アームを上昇
  }
  for(int theta=10; theta<=25; theta++){
    armservo.write(theta);
    delay(1000);
  }
  digitalWrite(STCW, HIGH);
  for(int i=0; i<1000; i++){
    steping_motor(); // アームを下降
    if(digitalRead(MSW) == 0){
      break;
    }
  }
}

void goal_mode() {
  glservo.write(90);
  delay(2000);
}

void steping_motor() {
  digitalWrite(STCL, HIGH);
  delay(20);
  digitalWrite(STCL, LOW);
  delay(20);
}

