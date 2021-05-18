#define echo_pin 2
#define trig_pin 4
#define buzzer_pin 8
#define servo_pin 7
#include <Servo.h>

int MIDDLE = 90;
int LEFT = 180;
int RIGHT = 10;
int MIN_DISTANCE = 20;

Servo servo;
int distance;
long duration;
int deg;

int middle_distance, left_distance, rigth_distance;

void setup() {

  pinMode(buzzer_pin, OUTPUT);
  pinMode(trig_pin,OUTPUT);
  pinMode(echo_pin,INPUT);
  servo.attach(servo_pin);
  
  Serial.begin(9600);
  Serial.println("Ultrasonic Sensor, Buzzer System, and MicroServo");
}

void display_sonnar_distance(){
  Serial.print("Middle Distance: ");
  Serial.print(distance);
  Serial.print(" cm");
  Serial.println();
}

// Calculate the distance from the trig signall
// Obtain the duration of the pulse, and calculate
// The distance based on the speed of sound on microseconds
int calculate_distance(){
  digitalWrite(trig_pin,LOW);
  delayMicroseconds(2);

  digitalWrite(trig_pin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin,LOW);

  duration = pulseIn(echo_pin,HIGH);
  distance = int(duration * 0.034 / 2);
  return distance;
}

// Activate the buzzer for a specific amount of time
// Then deactivate the buzzer
void object_emergency_alarm(){
  tone(buzzer_pin,1000);
  delay(1000);
  noTone(buzzer_pin);
  delay(1000);
}

// Check the free space from the right and left of the sonnar
void detect_free_space(int right_dst, int left_dst){

  // Check if the distance to the right is greater therefore 
  // Should walk to the Right direction
  if( right_dst > left_dst){
    Serial.println("Turn Right, walk");
    object_emergency_alarm();
    delay(360);
  }

  // Check if the distance to the left is greater therefore
  // Should walk to the Left direction
  else if( right_dst < left_dst){
    Serial.println("Turn Left, walk");
    object_emergency_alarm();
    delay(360);
  }

  // Check if its not enough space to the left and right
  // Should walk backwards
  else if( (right_dst <= MIN_DISTANCE) || (left_dst <= MIN_DISTANCE) ){
    Serial.println("Walk Backward, not enough space left and right");
    object_emergency_alarm();
    delay(180);
  }

  // It is enough space from both sides therefore should keep walking Forward
  else{
    Serial.println("Walk Forward");
  }
}

void object_avoidance_system(){
  servo.write(MIDDLE);
  delay(500);
  middle_distance = calculate_distance();

  // Display Distance Information
  display_sonnar_distance();

  // Check if the middle distance is less than the thresshold
  // Calculate the distance, and obtain for the left and right of the sonnar
  // Detect the free space based on the right or left distance
  if( middle_distance  <= MIN_DISTANCE ){
    servo.write(RIGHT);
    delay(1000);
    rigth_distance = calculate_distance();
    delay(500);
    servo.write(MIDDLE);
    delay(1000);
    servo.write(LEFT);
    delay(1000);
    left_distance = calculate_distance();
    delay(500);
    servo.write(MIDDLE);
    delay(1000);
    detect_free_space(rigth_distance, left_distance);
  }else{
    Serial.println("Walk Forward");
  }
}

void loop() {
  object_avoidance_system();
}
