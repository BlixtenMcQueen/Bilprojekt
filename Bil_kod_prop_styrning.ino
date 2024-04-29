#include <Arduino.h>
#include<Servo.h>
#include <NewPing.h> // För avståndsmätning med ultraljudssensorer
#include <stdio.h>

Servo myservo;
#define SERVO_PIN 6 // Servo PIN


#include "CytronMotorDriver.h"
#define MOTOR_PWM 5 // Motor PWM PIN
#define MOTOR_DIR 4 // Motor DIR PIN
CytronMD motor(PWM_DIR, MOTOR_PWM, MOTOR_DIR);  // PWM = Pin 4, DIR = Pin 5.


#define start_module 2

// Ultraljudssensorer - Använda nya pins
#define trigPin_L 10
#define echoPin_L 9
#define trigPin_R 12
#define echoPin_R 11
#define trigPin_F 8
#define echoPin_F 7

// Maximalt avstånd för ultraljudssensorer (cm)
#define side_maxdist 50 
#define front_maxdist 50

// Avståndsmätning - Instansiering av ultraljudssensorer
NewPing sonar_L(trigPin_L, echoPin_L, side_maxdist);
NewPing sonar_R(trigPin_R, echoPin_R, side_maxdist);
NewPing sonar_F(trigPin_F, echoPin_F, front_maxdist);

char F_value_str[20];

uint16_t motor_speed = 50;
bool start_module_state = false;
//konstant
float constant = 2.5;
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
    motor.setSpeed(50); // Startar motorn

    // Läs avståndet från vänster och höger sensorer
    // Blir 0 när inget hinder finns
    uint16_t distance_L = sonar_L.ping_cm();
    uint16_t distance_R = sonar_R.ping_cm();
    uint16_t distance_F = sonar_F.ping_cm();
    if(distance_L == 0){
      distance_L = 50;
    }
    if(distance_R == 0){
      distance_R = 50;
    }
    int16_t servo_result = distance_L - distance_R;
   


    //distance_L - distance_R = servo result
    // servo_result * konstant + 90 = servosvängning
    uint16_t servo_steer = servo_result * constant + 87;
   

    // Om vänster sensor upptäcker ett närmare objekt än höger sensor
    if ((distance_L < distance_R) || (distance_L > distance_R)) {
        myservo.write(servo_steer);
        last_direction = true;
    } 
    // Om inga sensorer upptäcker något nära objekt
    else {
        // Kör rakt fram
        myservo.write(87); // Mittenposition
        
    }

    // Objekt nära front sensor men inte utanför max distans (distance_F = 0)
    if ((distance_F < 10) && (distance_F != 0)){ 
      motor.setSpeed(0);
      delay(500);
      motor.setSpeed(-30);  // Backa
      delay(1000);
      motor.setSpeed(0);
      delay(500);     
    }  
  }
  else{
    motor.setSpeed(0);
  }
}