// when the ultrasonic sensor detects someone approaching,
// the lantern will start spinning slowly,
// and the leds will turn on.


#include <Servo.h>

const int TRIG_PIN  = 10;  // Arduino pin connected to Ultrasonic Sensor's TRIG pin
const int ECHO_PIN  = 9;  // Arduino pin connected to Ultrasonic Sensor's ECHO pin
const int SERVO_PIN = 6; // Arduino pin connected to Servo Motor's pin
const int LED_PIN = 3; // Arduino pin connected to LED's pin
const int DISTANCE_THRESHOLD = 10; // centimeters
const int SPIN_DELAY = 15; // milliseconds

Servo servo; // create servo object to control a servo

long duration; // to store the duration of the echo
int distance; // to calculate the distance
int pos = 0; // to store the servo position 

void setup() {
  Serial.begin (9600);       // initialize serial port
  pinMode(TRIG_PIN, OUTPUT); // set arduino pin to output mode
  pinMode(ECHO_PIN, INPUT);  // set arduino pin to input mode
  servo.attach(SERVO_PIN);   // attach the servo on pin 6 to the servo object
  servo.write(pos); // initialize servo
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // initialize LED
}

void loop() {
  // clear the TRIG pin
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  // set the TRIG pin on HIGH state for 10 micro seconds
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // measure duration of pulse from ECHO pin
  duration = pulseIn(ECHO_PIN, HIGH);
  // calculate the distance
  distance = duration * 0.034 / 2;

  if(distance < DISTANCE_THRESHOLD){
    digitalWrite(LED_PIN, HIGH);   // turn the LED on

    for(pos = 0; pos <= 180; pos += 1) // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    servo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(SPIN_DELAY);                       // waits 15ms for the servo to reach the position 
  } 
  for(pos = 180; pos>=0; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    servo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(SPIN_DELAY);                       // waits 15ms for the servo to reach the position 
  } 
  }
  else{
    digitalWrite(LED_PIN, LOW); // turn the LED off
  }
}
