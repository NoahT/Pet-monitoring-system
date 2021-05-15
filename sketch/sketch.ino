#define echo_pin 2
#define trig_pin 4
#define buzzer_pin 8

int distance;
long duration;

void setup() {
  pinMode(buzzer_pin, OUTPUT);
  pinMode(trig_pin,OUTPUT);
  pinMode(echo_pin,INPUT);
  Serial.begin(9600);
  Serial.println("Ultrasonic Sensor and Buzzer System");
  
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
void loop() {
  handle_sonnar();
  display_sonnar_distance();
  handle_buzzer();
}
