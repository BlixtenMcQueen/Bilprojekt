# Bilprojekt

# Bil_kod_V1.0
Version med <NewPing.h> library för avståndsmätning med ultraljudssensorer
Använder Left, Right & Front sensorerna. Man sätter maxvärden för sensorerna vid initiering av library instanserna.

# Bil_kod_V1.1
Bygger på Bil_kod_V1.0 med <NewPing.h> library.
Ultraljudssensorer Vänster, Höger & Front. Utskrift av sensorvärden till Serial monitor.
Backfunktion - under visst avstånd stannar bilen 0,5s backar 1s med lägre hastighet och stoppar 0,5s igen innan den kör framåt.

# Bil_kod_14Maj
Bygger på Bil_kod_V1.1. Efter varje avläsning av varje sidosensor tar programmet ett beslut hur servot ska svänga.
I variabel deklarationerna längst upp kan man sätta maximalt avstånd för ultraljudssensorer.
Max front sensor = 10 cm
Max sido sensorer = 55 cm
Proportionell styrning enligt följande uttryck:
servo_steer = servo_result * constant + 87;
Constant = 2.0

Förenklat några av if satsernas villkor

 # Bil_kod_14Maj_Slack
 Samma kod som Bil_kod_14Maj fast lagt till SLACK funktion.
 Programmet väljer att svänga när ena sidosensorn är SLACK cm längre ifrån den andra.
