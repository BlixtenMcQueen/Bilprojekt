#include <Arduino.h>
#include <stdio.h>
#include <Servo.h>    // Bibliotek för servo
#include <NewPing.h>  // Bibliotek för ultraljudssensorer

// Start modul
#define start_module 2            // Start modul PIN
bool start_module_state = false;  // Avläst tillstånd

// Servo
Servo myservo;
#define SERVO_PIN 6             // Servo PIN
const uint8_t SERVO_CENTER = 87;// Center pos
const uint8_t SERVO_MIN = 15;   // Ytterläge vänster
const uint8_t SERVO_MAX = 165;  // Ytterläge höger
unsigned long servo_result;     // Skillnad i avstånd från V & H sensor
int16_t servo_steer;            // Styrvariabel till servo (0 - 180 grader)
const float STEER_CONST = 2.0;  // Proportionell styrning konstant

// Motorstyrkort Cytron MD13S 
#include "CytronMotorDriver.h"
#define MOTOR_PWM 5 // Motor PWM PIN
#define MOTOR_DIR 4 // Motor DIR PIN
CytronMD motor(PWM_DIR, MOTOR_PWM, MOTOR_DIR);
int16_t MOTOR_SPEED = 70;

// Ultraljudssensorer pins
#define trigPin_L 10
#define echoPin_L 9
#define trigPin_R 12
#define echoPin_R 11
#define trigPin_F 8
#define echoPin_F 7

// Maximalt avstånd för ultraljudssensorer (cm)
uint16_t side_maxdist = 55; 
uint16_t front_maxdist = 10;   

// Ultraljudssensorer avläst avstånd (cm)
unsigned long distance_L;
unsigned long distance_R;
unsigned long distance_F;

// Initiering av ultraljudssensorer
NewPing sonar_L(trigPin_L, echoPin_L, side_maxdist);
NewPing sonar_R(trigPin_R, echoPin_R, side_maxdist);
NewPing sonar_F(trigPin_F, echoPin_F, front_maxdist);

void setup() {
    myservo.attach(SERVO_PIN); 
    pinMode(start_module, INPUT);   
    digitalWrite(start_module, LOW);
}
  
void loop() {

    // Läs av startmodul
    start_module_state = digitalRead(start_module);

    // Startmodul ger startsignal
    if(start_module_state == HIGH){
        motor.setSpeed(MOTOR_SPEED); // Startar motorn

        // VÄNSTER SENSOR
        distance_L = sonar_L.ping_cm(); 

        if(distance_L == 0){ // = 0 om avstånd > side_maxdist
            distance_L = side_maxdist;
        }

        // Proportionell styrning 
        servo_result = distance_L - distance_R;
        servo_steer = servo_result * STEER_CONST + SERVO_CENTER;

        // Kontrollerar att servon inte svänger för mycket 
        if(servo_steer > SERVO_MAX){
            servo_steer = SERVO_MAX;
        } 
        else if(servo_steer < SERVO_MIN){
            servo_steer = SERVO_MIN;
        }

        // Om vänster sensor upptäcker ett objekt närmare än höger sensor
        if(distance_L < distance_R){
            myservo.write(servo_steer);
        } 
        else{ // Kör rakt
            myservo.write(SERVO_CENTER); // Mittenposition        
        }

        // HÖGER SENSOR
        distance_R = sonar_R.ping_cm(); 

        if(distance_R == 0){ // = 0 om avstånd > side_maxdist
            distance_R = side_maxdist;
        }

        //Proportionell styrning 
        servo_result = distance_L - distance_R;
        servo_steer = servo_result * STEER_CONST + SERVO_CENTER;

        // Kontrollerar att servon inte svänger för mycket 
        if(servo_steer > SERVO_MAX){
            servo_steer = SERVO_MAX;
        } 
        else if(servo_steer < SERVO_MIN){
            servo_steer = SERVO_MIN;
        }

        // Om höger sensor upptäcker ett närmare objekt än vänster sensor
        if(distance_R < distance_L){
            myservo.write(servo_steer);
        } 
        else { // Kör rakt            
            myservo.write(SERVO_CENTER); // Mittenposition     
        }        
        
        // FRONT SENSOR
        distance_F = sonar_F.ping_cm(); 

        if(distance_F == 0){ // = 0 om avstånd > front_maxdist
            distance_F = front_maxdist;
        }
        
        // Objekt nära front sensor
        if(distance_F < front_maxdist){ 
            motor.setSpeed(0);  // Stanna
            delay(1000);
            distance_F = sonar_F.ping_cm(); // Läs av sensor igen

            if(distance_F == 0){
                distance_F = front_maxdist;
            }

            if(distance_F < front_maxdist){   // Objekt fortfarande nära
                motor.setSpeed(-30);  // Backa
                delay(1000);
                motor.setSpeed(0);    // Stanna
                delay(500);     
            }
        } 
    }
    else{ // Ingen startsignal från startmodul
        motor.setSpeed(0);  // Stanna
    }
}