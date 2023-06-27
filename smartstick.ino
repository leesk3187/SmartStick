// 블루투스, 부저, 초음파, 수위센서
//초음파 : 29us에 1cm를 이동
#include <SoftwareSerial.h>
SoftwareSerial HC06(2, 3);
#define trig 4
#define echo 5
const int buzzer = 9;
const int waterLevelPin = A0;// 수위 센서 핀 설정

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //BTSerial.begin(9600); /// 블루투스 출력
  HC06.begin(9600); //블루투스
  
  pinMode(buzzer, OUTPUT);
  
  pinMode(trig,OUTPUT); //trig 발사
  pinMode(echo,INPUT); //echo 받기

  pinMode(waterLevelPin, INPUT); // 수위 센서를 입력으로 설정

}
float dist = 0;
void loop() {
  int waterLevel = analogRead(waterLevelPin); // 수위 센서값 읽기
  float waterDepthPercent = (float)waterLevel / 1023 * 100;

  digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(5);
  digitalWrite(trig,LOW);

  dist = (pulseIn(echo, HIGH)/29.0)/2; //cm

  Serial.print("수위: ");
  Serial.print(waterDepthPercent);
  Serial.println("%");
  Serial.print("거리: ");
  Serial.print(dist);
  Serial.println("cm");

 if(waterDepthPercent > 55){
   HC06.println("현재: 물웅덩이 감지!");
 }
 else if(dist <= 23 && dist >=6){
    Serial.println("현재: 물체 감지!");
    HC06.println("현재: 물체 감지!");
  }
  else if(dist <6 && dist >= 0.1)
  {
    Serial.println("현재: 물체 접촉!");
    HC06.println("현재: 물체 접촉!");
  }
  else{
    Serial.println("현재: 물체 미감지!");
    HC06.println("현재: 물체 미감지!");
  }
  HC06.print("거리: ");
  HC06.print(dist,1);//소수 이하 1자리 허용
  HC06.println(" cm");
  HC06.print("수위: ");
  HC06.print(waterDepthPercent,1);// 
  HC06.println("%");
  
 
  if(waterDepthPercent > 55.0){
    tone(buzzer, 1700,1700); //부저에 440 Hz 소리 출력
    delay(1705);
  }
  else if(dist >= 6 && dist<=23)
  {
    tone(buzzer,2600,100);
    delay(102);
  }
  else if(dist<6 && dist >=0.1)
  {
    tone(buzzer,2600,1000);
    delay(1005);
  }
  else  {
    noTone(buzzer); // 거리가 10cm 이상일 경우 멈춤
  }
  delay(100);// 샘플링 간격 2초 부여

  // 블루투스 데이터 송신 주기 제어
  static unsigned long previousMillis = 0;
  const unsigned long interval = 1000; // 1초에 한 번씩 전송

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
  }
}