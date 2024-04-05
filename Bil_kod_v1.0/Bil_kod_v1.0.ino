v#include<Servo.h>
#include <NewPing.h> // För avståndsmätning med ultraljudssensorer
#include <stdio.h>

Servo myservo;
#define SERVO_PIN 6 // Servo PIN


#include "CytronMotorDriver.h"
#define MOTOR_PWM 5 // Motor PWM PIN
#define MOTOR_DIR 4 // Motor DIR PIN
CytronMD motor(PWM_DIR, MOTOR_PWM, MOTOR_DIR);  // PWM = Pin 4, DIR = Pin 5.


#define BUTTON1 3

// Ultraljudssensorer - Använda nya pins
#define trigPin_L 12
#define echoPin_L 11
#define trigPin_R 10
#define echoPin_R 9
#define trigPin_F 8
#define echoPin_F 7

// Maximalt avstånd för ultraljudssensorer
#define maxdist 30 

// Avståndsmätning - Instansiering av ultraljudssensorer
NewPing sonar_L(trigPin_L, echoPin_L, maxdist);
NewPing sonar_R(trigPin_R, echoPin_R, maxdist);
NewPing sonar_F(trigPin_F, echoPin_F, maxdist);



unsigned int motor_speed = 10;
bool start_motor = false;

bool last_direction = true;


// Funktion för att kontrollera om knappen är nedtryckt
bool sw1(void) {
    if (digitalRead(BUTTON1)) {
        return true;
    } else {
        return false;
    }
}

void sw1_press() {
    while (sw1()) {
        delay(10);
    }
}

void setup() {
    myservo.attach(SERVO_PIN);
    pinMode(BUTTON1, INPUT_PULLUP);
    Serial.begin(9600);
    
    
}

void loop() {
    motor.setSpeed(35);

    // Läs avståndet från vänster och höger sensorer
    unsigned int distance_L = sonar_L.ping_cm();
    unsigned int distance_R = sonar_R.ping_cm();
    unsigned int distance_F = sonar_F.ping_cm();

    // Om vänster sensor upptäcker ett närmare objekt än höger sensor
    if (distance_L < distance_R) {
        // Sväng åt vänster
        myservo.write(0); // Vänster max position
        last_direction = true;
    } 
    // Om höger sensor upptäcker ett närmare objekt än vänster sensor
    else if (distance_R < distance_L) {
        // Sväng åt höger
        myservo.write(180); // Höger max position
        last_direction = false;
    } 
    // Om inga sensorer upptäcker något nära objekt
    else {
        // Kör rakt fram
        myservo.write(90); // Mittenposition
        
    }
    if (distance_F){
      
       if (last_direction) 
          myservo.write(180);
      else
          myservo.write(0);


    }
    delay(5); // Låt en kort paus mellan varje avläsning för att undvika överbelastning
}