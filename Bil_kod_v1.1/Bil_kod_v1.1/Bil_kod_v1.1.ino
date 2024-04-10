#include<Servo.h>
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
#define trigPin_L 10
#define echoPin_L 9
#define trigPin_R 12
#define echoPin_R 11
#define trigPin_F 8
#define echoPin_F 7

// Maximalt avstånd för ultraljudssensorer (cm)
#define side_maxdist 30 
#define front_maxdist 50 

// Avståndsmätning - Instansiering av ultraljudssensorer
NewPing sonar_L(trigPin_L, echoPin_L, side_maxdist);
NewPing sonar_R(trigPin_R, echoPin_R, side_maxdist);
NewPing sonar_F(trigPin_F, echoPin_F, front_maxdist);

char F_value_str[20];

unsigned int motor_speed = 42;
bool start_motor = true;

bool last_direction = true;


// Funktion för att kontrollera om knappen är nedtryckt
bool sw1(void) {
    if (PIND & (1 << BUTTON1)) {
        return false; // Ingång HÖG
    }
    else {
        return true;  // Ingång LÅG
    }
}

void sw1_press() {
    while (sw1()) {
        delay(10);
    }
}

void setup() {
    myservo.attach(SERVO_PIN);
    pinMode(BUTTON1, INPUT);
    //digitalWrite(BUTTON1, HIGH);
    Serial.begin(9600); 
}

void loop() {
/*   if(sw1()) {
        sw1_press();
        start_motor = !start_motor; // Togglar start variable
        //Serial.print("BUTTON ");
    }*/

    motor.setSpeed(42); // Startar motorn

/*    if(start_motor) {
        motor.setSpeed(42); // Startar motorn
        //Serial.print("START ");
     }
    else {
        motor.setSpeed(0); // Stopar motorn
        //Serial.println("STOP ");
    }*/
 
    // Läs avståndet från vänster och höger sensorer
    // Blir 0 när inget hinder finns
    unsigned int distance_L = sonar_L.ping_cm();
    unsigned int distance_R = sonar_R.ping_cm();
    unsigned int distance_F = sonar_F.ping_cm();

    // Utskrift av sensorvärden
    sprintf(F_value_str, "LEFT: %u ", distance_L);
    Serial.print(F_value_str);

    sprintf(F_value_str, "RIGHT: %u ", distance_R);
    Serial.print(F_value_str);

    sprintf(F_value_str, "FRONT: %u ", distance_F);
    Serial.println(F_value_str);

    // Om vänster sensor upptäcker ett närmare objekt än höger sensor
    if (distance_L < distance_R) {
        // Sväng åt höger
        myservo.write(160);
        last_direction = true;
    } 
    // Om höger sensor upptäcker ett närmare objekt än vänster sensor
    else if (distance_R < distance_L) {
        // Sväng åt vänster
        myservo.write(20);  // Marginal så inte hjulet fastnar
        last_direction = false;
    } 
    // Om inga sensorer upptäcker något nära objekt
    else {
        // Kör rakt fram
        myservo.write(90); // Mittenposition
        
    }

    //delay(250);

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