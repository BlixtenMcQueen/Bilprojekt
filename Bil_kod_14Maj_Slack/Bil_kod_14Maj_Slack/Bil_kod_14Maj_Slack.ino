#include <Arduino.h>
#include <Servo.h>
#include <NewPing.h> // För avståndsmätning med ultraljudssensorer
#include <stdio.h>

Servo myservo;
#define SERVO_PIN 6 // Servo PIN
float servo_result;
int16_t servo_steer;
float constant = 2.0; //Servo styr konstant

#include "CytronMotorDriver.h"
#define MOTOR_PWM 5 // Motor PWM PIN
#define MOTOR_DIR 4 // Motor DIR PIN
CytronMD motor(PWM_DIR, MOTOR_PWM, MOTOR_DIR);  // PWM = Pin 4, DIR = Pin 5.

#define start_module 2 //Start module PIN
uint16_t motor_speed = 65;
bool start_module_state = false;

// Ultraljudssensorer - Använda nya pins
#define trigPin_L 10
#define echoPin_L 9
#define trigPin_R 12
#define echoPin_R 11
#define trigPin_F 8
#define echoPin_F 7

// Maximalt avstånd för ultraljudssensorer (cm)
uint8_t side_maxdist = 55; 
uint8_t front_maxdist = 10;
uint8_t slack = 0;
// Avståndsmätning - Instansiering av ultraljudssensorer
NewPing sonar_L(trigPin_L, echoPin_L, side_maxdist);
NewPing sonar_R(trigPin_R, echoPin_R, side_maxdist);
NewPing sonar_F(trigPin_F, echoPin_F, front_maxdist);
//Sensor avläst avstond 
float distance_L;
float distance_R;
float distance_F;

bool last_direction = true;

void setup() {
    myservo.attach(SERVO_PIN);
    pinMode(start_module, INPUT);
    digitalWrite(start_module, LOW);
    
    Serial.begin(9600); 
}
  
void loop() {

      // Läs av knappens status
    start_module_state = digitalRead(start_module);

    // Om start_module blir hög
    if(start_module_state == HIGH){
      motor.setSpeed(motor_speed); // Startar motorn

      // VÄNSTER SENSOR
      distance_L = sonar_L.ping_cm();

      if(distance_L == 0){
        distance_L = side_maxdist;
      }

      //Proportionell styrning 
      servo_result = distance_L - distance_R;
      servo_steer = servo_result * constant + 87;

      //Kontrollerar att servon inte svänger för mycket 
      if(servo_steer > 165){
        servo_steer = 165;
      }else if(servo_steer < 15){
        servo_steer = 15;
      }

      // Om vänster sensor upptäcker ett närmare objekt än höger sensor
      if ((distance_L < (distance_R - slack)) || (distance_R < (distance_L - slack))) {
          myservo.write(servo_steer);
          last_direction = true;
      } 

      else {
          // Kör rakt fram
          myservo.write(87); // Mittenposition        
      }

      // HÖGER SENSOR
      distance_R = sonar_R.ping_cm();

      if(distance_R == 0){
        distance_R = side_maxdist;
      }

      //Proportionell styrning 
      servo_result = distance_L - distance_R;
      servo_steer = servo_result * constant + 87;

      //Kontrollerar att servon inte svänger för mycket 
      if(servo_steer > 165){
        servo_steer = 165;
      }else if(servo_steer < 15){
        servo_steer = 15;
      }
      // Om vänster sensor upptäcker ett närmare objekt än höger sensor
      if ((distance_L < (distance_R - slack)) || (distance_R < (distance_L - slack))) {
          myservo.write(servo_steer);
          last_direction = true;
      } 
      // Om inga sensorer upptäcker något nära objekt
      else {
          // Kör rakt fram
          myservo.write(87); // Mittenposition     
      }
      
      distance_F = sonar_F.ping_cm();

      if(distance_F == 0){
        distance_F = front_maxdist;
      }

      // Objekt nära front sensor
      if ((distance_F < front_maxdist)){ 
        motor.setSpeed(0);
        delay(1000);
        distance_F = sonar_F.ping_cm(); // Läs av sensor igen

        if(distance_F == 0){
          distance_F = front_maxdist;
        }

        if ((distance_F < front_maxdist)){   // Objekt fortfarande nära
          motor.setSpeed(-30);  // Backa
          delay(1000);
          motor.setSpeed(0);
          delay(500);     
        }
      }  
    }
  else{ // Ingen startsignal från startmodul
    motor.setSpeed(0);
  }
}