#include <IRControl.h>
#include <MotorControl.h>
#include <UltrasonicControl.h>

#include <IRControl.h>
#include <MotorControl.h>
#include <UltrasonicControl.h>

// April 29th - Started as Yom-Pom1 - Basic right turn code when an obstacle is detected. (Same as Roller_Line_Stop2.ino) howdy
// April 30th - Roller_Line_Stop3 - First major coding attempt.  This code can't currently
//              deal with turning into an obstacle which we will definitely need to deal with
//              (But for a plan on how to deal with this, see the notes in the black/black
//              section.
//  May 4th -   Roller_Line_Stop8 - Added Outputs to drive LEDs for x and y co-ords.  I/O pin 2 (MSB) and 3 (LSB) specify x 
//              co-ord in binary. I/O pin 8 (MSB)and 9 (LSB) specify y co-ord in binary.   
//  May 5th -   Roller_Line_Stop9 - Motor speed values set to 74/76 for Yom-Yom, but now made into constants, 
//              left_mot_fwd_speed and right_mot_fwd_speed, to make it easier to calibrate after transport.
//              Also added correct_mot_speed=100 for the correcting speed.  Slowed down the turns to 85 from 120
//              and increased the time it takes from 32000 to 50000.
//              since things were a bit jerky and quick.  Noted that random function isn't working properly because
//              a random number of 1 or 2 should be specified with random(1,3).
//  May 11th -  Roller_Line_Stop11 - This code should fix the turn into a block problem by turning back the way that
//              we came from.  Checks a second sonar detect before we kickstart. Major Modifications.  evaluateTurn
//              now takes in 4 arguments (xCOORD, yCOORD, Dir, and a boolean second) and returns the new direction.  Also
//              the boolean "second" deals with turning into an obstacle.  Essentially we go back the way that we came if
//              we have turned into an obstacle.  Also added constants for the turning times but for some unknown reason
//              this doesn't work and we had to revert to hard numbers in the for loops (May 15th).  There are still endless
//              loops that we can get ourselves into, but this code should be able to deal with most situations.
//  May 17th -  Roller_Line_Stop15 - Again a straight jump from 11.  Version 11 is now the most recent working version after
//              finding that the for loop condition statement couldn't use constants.  Version 15 now attempts to tackle
//              the fact that our turns are attrocious!  
//  May 27th -  Roller_Line_Stop15 (forgot so save as 16!)  Added backup() method for look and return type turns.
#include <IRControl.h>
#include <MotorControl.h>
#include <UltrasonicControl.h>

IRControl irR(0);
IRControl irL(1);
MotorControl mot1(1);
MotorControl mot2(2);
UltrasonicControl sonic (13,12);

const int UP=0, RIGHT=1, DOWN=2, LEFT=3;
const int left_mot_fwd_speed=74, right_mot_fwd_speed=76;
const int correct_mot_speed=100;
const int left_turn_speed=85, right_turn_speed=85;

int xCOORD=0, yCOORD=1, globalDir=UP; //Location variables
int newDir;
boolean secondTry=false;

void setup() 
{
   Serial.begin(9600);
   setLEDs();
   delay(5000);
   randomSeed(analogRead(2));
   pinMode(2, OUTPUT); //X co-ord MSB LED
   pinMode(3, OUTPUT); //X co-ord LSB LED
   pinMode(8, OUTPUT); //Y co-ord MSB LED
   pinMode(9, OUTPUT); //Y co-ord LSB LED
   
}

void loop() 
{  
  
   Serial.print(irR.isBlack());
   Serial.print("\t");
   Serial.print(irL.isBlack());
   Serial.print("\t");
   Serial.println(sonic.getDistance());

}

void setLEDs()
{
  //light up proper binary code for x co-ord
  
   if (xCOORD==0)
   {
     digitalWrite(2,HIGH);
     digitalWrite(3,HIGH);
   }
   else if (xCOORD==1)
   {
     digitalWrite(2,HIGH);
     digitalWrite(3,LOW);
   }
   else if (xCOORD==2)
   {
     digitalWrite(2,LOW);
     digitalWrite(3,HIGH);
   }
   else if (xCOORD==3)
   {
     digitalWrite(2,LOW);
     digitalWrite(3,LOW);
   }

   //light up proper binary code for y co-ord
   
   if (yCOORD==0)
   {
     digitalWrite(8,HIGH);
     digitalWrite(9,HIGH);
   }
   else if (yCOORD==1)
   {
     digitalWrite(8,HIGH);
     digitalWrite(9,LOW);
   }
   else if (yCOORD==2)
   {
     digitalWrite(8,LOW);
     digitalWrite(9,HIGH);
   }
   else if (yCOORD==3)
   {
     digitalWrite(8,LOW);
     digitalWrite(9,LOW);
   }
     
}



