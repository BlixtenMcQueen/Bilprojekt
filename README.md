# Bilprojekt

# Bil_kod_V1.0
Version med <NewPing.h> library för avståndsmätning med ultraljudssensorer
Använder Left, Right & Front sensorerna. Man sätter maxvärden för sensorerna vid initiering av library instanserna.

# Bil_kod_V1.1
Bygger på Bil_kod_V1.0 med <NewPing.h> library.
Ultraljudssensorer Vänster, Höger & Front. Utskrift av sensorvärden till Serial monitor.
Backfunktion - under visst avstånd stannar bilen 0,5s backar 1s med lägre hastighet och stoppar 0,5s igen innan den kör framåt.

# Bil_kod_14Maj
VINNAR KODEN! Bygger på Bil_kod_V1.1. Efter varje avläsning av varje sidosensor tar programmet ett beslut hur servot ska svänga.
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

# Bil_kod_14Maj_MittenPos
Utgår från Bil_kod_14Maj_Slack.
Ny funktion är att om ena sidosensorn registrerar max värde så används det gamla sensorvärdet istället.
Detta gjordes för att se om vänster sensorn registrerar max värde när den ligger nära vänster kant på banan.
Och för att komma runt det eventuellt.
      // Vänster sensor registrerar max värde (ev pga reflektioner och vinklade sensorer)
      if (distance_L == side_maxdist) {
          // Gammalt värde vänster sensor mindre än höger plus slack
          if (distance_L_old < (distance_R - slack)) { 
              turn = true; 
              use_old_sensor_value = true;
          }
      }

# Bil_kod_Final
Utgår från Bil_kod_14Maj (Vinnarkoden).
Ändrat vissa datatyper för att passa anropade funktioner mm.
Infört några konstanter för mer överskådlighet.
Kommenterat och strukturerat koden ytterligare.

     
