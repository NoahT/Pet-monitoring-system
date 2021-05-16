#define echo_pin 2
#define trig_pin 4
#define buzzer_pin 8
#define servo_pin 7
#include <Servo.h>

Servo servo;
int distance;
long duration;
int pos = random(0,180);
int deg;

unsigned long previousMillis = 0;
const long interval = 15;
int State;
int i;

void setup() {
  State = 0;
  i = 0;
  pinMode(buzzer_pin, OUTPUT);
  pinMode(trig_pin,OUTPUT);
  pinMode(echo_pin,INPUT);
  Serial.begin(9600);
  Serial.println("Ultrasonic Sensor, Buzzer System, and MicroServo");
  //Serial.println("Micro Servo Testing");
  servo.attach(servo_pin);
  servo.write(0);
}

void display_sonnar_distance(){
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm");
  Serial.println();
}

void handle_sonnar(){
  digitalWrite(trig_pin,LOW);
  delayMicroseconds(2);

  digitalWrite(trig_pin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin,LOW);

  duration = pulseIn(echo_pin,HIGH);
  distance = duration * 0.034 / 2;
  
}
void handle_buzzer(){
  if( distance < 5) {
    tone(buzzer_pin,1000);
    delay(1000);
    noTone(buzzer_pin);
    delay(1000);
  }else{
    noTone(buzzer_pin);
  }
}
void rotate_to_right(){
  unsigned long currentMillis = 0;
  int i = 0;
  //for(int i = 0; i < 180; i++){
  while(i < 180){
    currentMillis = millis();
    if(currentMillis - previousMillis >= interval){
      previousMillis = currentMillis;
      servo.write(i);
      i++;
    }
  }
}
void rotate_to_left(){
  unsigned long currentMillis = 0;
  int i = 180;
  while(i > 0){
    currentMillis = millis();
    if(currentMillis - previousMillis >= interval){
      previousMillis = currentMillis;
      servo.write(i);
      i--;
    }
  }
  //for(int i = 180; i > 0; i--){
  //  servo.write(i);
  //  delay(15);
  //}
}
void handle_servo(){
  rotate_to_right();
  rotate_to_left();
}
void loop() {
  //handle_sonnar();
  //display_sonnar_distance();
  //handle_buzzer();
  //handle_servo();
  if(State == 0){
    
    
  }else{
    
  }
}
