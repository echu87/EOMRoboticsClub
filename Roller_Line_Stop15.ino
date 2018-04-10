#include <IRControl.h>
#include <MotorControl.h>
#include <UltrasonicControl.h>

// April 29th - Started as Yom-Pom1 - Basic right turn code when an obstacle is detected. (Same as Roller_Line_Stop2.ino)
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

IRControl ir1(0);
IRControl ir2(1);
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
   Serial.print(ir1.isBlack());
   Serial.print("\t");
   Serial.print(ir2.isBlack());
   Serial.print("\t");
   Serial.println(sonic.getDistance());

   
   if (ir1.isBlack() == 0 && ir2.isBlack() == 0)
   {
      mot1.forward(left_mot_fwd_speed);
      mot2.forward(right_mot_fwd_speed);
      secondTry=false;
   }
  
   else if ((ir1.isBlack() == 1) && (ir2.isBlack() == 0)) //moving left
   {
      mot1.forward(correct_mot_speed);  //turn up left wheel to correct to the right
      mot2.forward(right_mot_fwd_speed);
      secondTry=false;
   }

   else if ((ir1.isBlack() == 0) && (ir2.isBlack() == 1))//moving right
   {
      mot1.forward(left_mot_fwd_speed);  //turn up the right wheel to correct to the left
      mot2.forward(correct_mot_speed);
      secondTry=false;
   }
  
   else if ((ir1.isBlack() == 1) && (ir2.isBlack() == 1))  //hit and intersection and stop!
   {
      stop(); //delay for a brief second to evaluate

      secondTry=false;
      newDir=evaluateTurn(xCOORD,yCOORD,globalDir,secondTry);
      if (sonic.detect()){
        secondTry=true;
        stop();
        newDir=evaluateTurn(xCOORD,yCOORD,newDir,secondTry);
      }
      updateXY(newDir);
      setLEDs();
      globalDir=newDir;
      kickStart();  //Later on put this inside a sonic.detect() condition like:
                    //
                    //  if(!sonic.detect()){
                    //    kickStart();
                    //  }else{
                    //    evaluate Turn();
                    //    kickStart();
                    //  }
                    //   
                    //  Note: This may require a fine tuning of our turnRight
                    //        and turnLeft turns...we could make them much
                    //        more involved if necessary. 
                    //
                    //  Note2: Will also have to undecrement the variables if
                    //         we go into the else portion above.  This may
                    //         require us to return the x and y coord so we
                    //         can modify x,y and dir accordingly
        secondTry=false;
   }
   secondTry=false;
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

void stop()
{
   for (long i=0; i<20000 ; i++)  //Note: Needs to be long for 4 byte storage
   {                               //Rather than 2 byte storage of int
      mot1.halt();
      mot2.halt();
   }
}

void backup()
{
   stop();
   for (long i=0; i<30000; i++)
   {
      mot1.reverse(71);
      mot2.reverse(74);
   }
   stop();
}

void turnRight()
{
   for (long j=0; j<20000; j++)
   {
      mot1.forward(74);
      mot2.forward(76);
   }
   for (long j=0; j<32000; j++)
   {
      mot1.forward(70);
      mot2.reverse(70);
   }
}

void turnLeft(){
    for (long j=0; j<20000; j++)
   {
      mot1.forward(74);
      mot2.forward(76);
   }
   for (long j=0; j<30000; j++)
   {
      mot1.reverse(70);
      mot2.forward(70);
   }
}

void kickStart()
{
   for (long k=0; k<10000; k++)
   {
      mot1.forward(left_mot_fwd_speed);
      mot2.forward(right_mot_fwd_speed);
   }
}

void updateXY(int changedDir)
{
  if (changedDir==UP)
    yCOORD++;
  else if (changedDir==RIGHT)
    xCOORD++;
  else if (changedDir==DOWN)
    yCOORD--;
  else if (changedDir==LEFT)
    xCOORD--;
}

//function that decides the turn
//updating x,y and dir can be a separate function???
int evaluateTurn(int x, int y, int dir, boolean second)
{
      int choice=0;

      //Go through all 36 cases

       
      if (x==0 && y==0 && dir==UP)
      {
        if(!second)
        {
          if (sonic.detect())
          {
            turnRight();
            return RIGHT;
          }
          else
          {
            choice=random(1,3);
            if (choice == 1)
            {
              return UP;
            }
            else
            {
              turnRight();
              return RIGHT;
            }
          }
        }
        else
        {
           backup();
           turnRight();
           return RIGHT;
        }
      }
      
      else if (x==0 && y==0 && dir==RIGHT)
      {
        if (!second)
        {
          if (sonic.detect())
          {
            turnLeft();
            return UP;
          }
          else
          {
            choice=random(1,3);
            if (choice == 1)
            {
              return RIGHT;
            }
            else
            {
              turnLeft();
              return UP;
            }
          }
        }
        else
        {
          backup();
          turnLeft();
          return UP;
        }
      }
      
      else if (x==0 && y==0 && dir==DOWN)
      {
        if (!second)
        {
          turnLeft();
          return RIGHT;
        }
        else
        {
          backup();
          turnLeft();
          return RIGHT;
        }
      } 

      else if (x==0 && y==0 && dir==LEFT)
      {
        if (!second)
        {
          turnRight();
          return UP;
        }
        else
        {
          backup();
          turnRight();
          return UP;
        }
      }

      else if (((x==0 && y==1) || (x==0 && y==2)) && dir==UP)
      {
        if (!second)
        {
          if (sonic.detect())
          {
            turnRight();
            return RIGHT;
          }
          else
          {
            choice=random(1,3);
            if (choice == 1)
            {
              return UP;
            }
            else
            {
              turnRight();
              return RIGHT;
            }
          }
        }
        else
        {
          backup();
          turnRight();
          return LEFT;
        }
      }

      else if (((x==0 && y==1) || (x==0 && y==2)) && dir==RIGHT)
      {
        if (!second)
        {
          if (sonic.detect())
          {
            turnLeft();
            return UP;
          }
          else
          {
            choice=random(1,3);
            if (choice == 1)
            {
              return RIGHT;
             }
            else
            {
              turnLeft();
              return UP;
            }
          }
        }
        else
        {
          if (globalDir==DOWN)
          {
            backup();
            turnLeft();
            return UP;
          }
          else if (globalDir==UP)
          {
            backup();
            turnRight();
            return DOWN;
          }
        }
      }

      else if (((x==0 && y==1) || (x==0 && y==2)) && dir==DOWN)
      {
        if (!second)
        {
          if (sonic.detect())
          {
            turnLeft();
            return RIGHT;
          }
          else
          {
            turnLeft();
            return RIGHT;
          }
        }
        else
        {
          backup();
          turnLeft();
          return RIGHT;
        }
      }

      else if (((x==0 && y==1) || (x==0 && y==2)) && dir==LEFT)
      {
        turnRight();
        return UP;
      }  //Note: no turn to face left is possible here

      else if (x==0 && y==3 && dir==UP){
        turnRight();
        return RIGHT;
      }  //Note: no turn to face up is possible here

      else if (x==0 && y==3 && dir==RIGHT)
      {
        if (!second)
        {
          if (sonic.detect())
          {
            turnRight();
            return DOWN;
          }
          else
          {
            choice=random(1,3);
            if (choice==1)
            {
              return RIGHT;
            }
            else
            {
              turnRight();
              return DOWN;
            }
          }
        }
        else
        {
          backup();
          turnRight();
          return DOWN;    
        }
      }

      else if (x==0 && y==3 && dir==DOWN)
      {
        if (sonic.detect())
        {
          turnLeft();
          return RIGHT;
        }
        else
        {
          turnLeft();
          return RIGHT;
        }
      }

      else if (x==0 && y==3 && dir==LEFT){
        turnLeft();
        return DOWN;
      }  //Note: no turn to be facing left is possible here

      else if (((x==1 && y==0)  || (x==2 && y==0)) && dir==UP)
      {
        if (!second)
        {
          if (sonic.detect())
          {
            turnRight();
            return RIGHT;
          }
          else
          {
            choice=random(1,3);
            if (choice==1)
            {
              return UP;
            }
            else
            {
              turnRight();
              return RIGHT;
            }
          }
        }
        else
        {
          if (globalDir==LEFT)
          {
            backup();
            turnRight();
            return RIGHT;    
          }
          else if (globalDir==RIGHT)
          {
            backup();
            turnLeft();
            return LEFT;
          }
        }
      }

      else if (((x==1 && y==0)  || (x==2 && y==0)) && dir==RIGHT)
      {
        if (!second)
        {
          if (sonic.detect())
          {
            turnLeft();
            return UP;
          }
          else
          {
            choice=random(1,3);
            if (choice==1)
            {
              return RIGHT;
            }
            else
            {
              turnLeft();
              return UP;
            }
          }
        }
        else
        {
          backup();
          turnLeft();
          return UP;
        }
      }

      else if (((x==1 && y==0)  || (x==2 && y==0)) && dir==DOWN)
      {
        turnLeft();
        return RIGHT;
      }  //Note: not possible to turn into the down position here

      else if (((x==1 && y==0)  || (x==2 && y==0)) && dir==LEFT)  
      {
        if (!second)
        {
          if (sonic.detect())
          {
            turnRight();
            return UP;
          }
          else
          {
            turnRight();
            return UP;
          }
        }
        else
        {
          backup();
          turnRight();
          return UP;
        }
      }

      else if (((x==1 && y==1)  || (x==2 && y==1) || (x==1 && y==2) || (x==2 && y==2)) && dir==UP)
      {
        if (!second)
        {
          if (sonic.detect())
          {
            turnRight();
            return RIGHT;
          }
          else
          {
            choice=random(1,3);
            if (choice==1)
            {
              return UP;
            }
            else
            {
              turnRight();
              return RIGHT;
            }
          }
        }
        else
        {
          if (globalDir==LEFT)
          {
            backup();
            turnLeft();
            return LEFT;
          }
          else
          {
            backup();
            turnRight();
            return RIGHT;  
          }
        }
      }

      else if (((x==1 && y==1)  || (x==2 && y==1) || (x==1 && y==2) || (x==2 && y==2)) && dir==RIGHT)
      {
        if (!second)
        {
          if (sonic.detect())
          {
            turnLeft();
            return UP;
          }
          else
          {
            choice=random(1,3);
            if (choice==1)
            {
              return RIGHT;
            }
            else
            {
              turnLeft();
              return UP;
            }
          }
        }
        else
        {
          if(globalDir==DOWN)
          {
            backup();
            turnLeft();
            return UP;
          }
          else
          {
            backup();
            turnRight();
            return DOWN;
          }
        }
      }

      else if (((x==1 && y==1)  || (x==2 && y==1) || (x==1 && y==2) || (x==2 && y==2)) && dir==DOWN)
      {
        if (!second)
        {
          if (sonic.detect())
          {
            turnLeft();
            return RIGHT;
          }
          else
          {
            turnLeft();
            return RIGHT;
           }
        }
        else
        {
           if (globalDir==LEFT)
           {
              backup();
              turnLeft();
              return RIGHT;
           }
           else if (globalDir==RIGHT)
           {
              backup();
              turnRight();
              return LEFT;
           }
        }
      }

      else if (((x==1 && y==1)  || (x==2 && y==1) || (x==1 && y==2) || (x==2 && y==2)) && dir==LEFT)
      {
        if (!second)
        {
          if (sonic.detect())
          {
            turnRight();
            return UP;
          }
          else
          {
            turnRight();
            return UP;
          }
        }
        else
        {
           if (globalDir==DOWN)
           {
              backup();
              turnRight();
              return UP;
           }
           else if (globalDir==UP)
           {
              backup();
              turnLeft();
              return DOWN;
           }
        }
      }

      else if (((x==1 && y==3) || (x==2 && y==3)) && (dir==UP))
      {
        turnRight();
        return RIGHT;
      }  //Note: not possible to turn into this position

      else if (((x==1 && y==3) || (x==2 && y==3)) && (dir==RIGHT))
      {
        if (!second)
        {
          if(sonic.detect())
          {
            turnRight();
            return DOWN;
          }
          else
          {
            return RIGHT;
          }
        }
        else
        {
          backup();
          turnRight();
          return DOWN;
        }
      }

      else if (((x==1 && y==3) || (x==2 && y==3)) && (dir==DOWN))
      {
        if (!second)
        {
          if(sonic.detect())
          {
            turnLeft();
            return RIGHT;
          }
        }
        else
        {
           if (globalDir==LEFT)
           {
              backup();
              turnLeft();
              return RIGHT;
           }
           else if (globalDir==RIGHT)
           {
              backup();
              turnRight();
              return LEFT;
           }
        }
      }

      else if (((x==1 && y==3) || (x==2 && y==3)) && (dir==LEFT))
      {
        if (!second)
        {
          if(sonic.detect())
          {
            turnLeft();
            return DOWN;
          }
          else
          {
            turnLeft();
            return DOWN;
          }
        }
        else
        {
          backup();
          turnLeft();
          return DOWN;
        }
      }

      else if  (x==3 && y==0 && dir==UP)
      {
        if (!second)
        {
          if (sonic.detect())
          { 
            turnLeft();
            return LEFT;
          }
          else
          {
            return UP;
          }
        }
        else
        {
           backup();
           turnLeft();
           return LEFT;
        }
      }

      else if (x==3 && y==0 && dir==RIGHT)
      {
        turnLeft();
        return UP;
      }  //Note: not possible to turn into this position

      else if (x==3 && y==0 && dir==DOWN)
      {
        turnRight();
        return LEFT;
      }  //Note: not possible to turn into this position

      else if (x==3 && y==0 && dir==LEFT)
      {
        if (!second)
        {
          if (sonic.detect())
          {
            turnRight();
            return UP;
          }
          else
          {
            turnRight();
            return UP;
          }
        }
        else
        {
           backup();
           turnRight();
           return UP;
        }
      }

      else if (((x==3 && y==1) || (x==3 && y==2)) && dir==UP)
      {
        if (!second)
        {
          if (sonic.detect())
          {
            turnLeft();
            return LEFT;
          }
          else
          {
            return UP;
          }
        }
        else
        {
          backup();
          turnLeft();
          return LEFT;
        }
      }

      else if (((x==3 && y==1) || (x==3 && y==2)) && dir==RIGHT)
      {
        turnLeft();
        return UP;
      }        //Note: it is not possible to turn into this position

      else if (((x==3 && y==1) || (x==3 && y==2)) && dir==DOWN)
      {
        if (!second)
        {
          if (sonic.detect())
          {
            turnRight();
            return LEFT;
          }
          else
          {
            turnRight();
            return LEFT;
          }
        }
        else
        {
           backup();
           turnRight();
           return LEFT;
        }
      }

      else if (((x==3 && y==1) || (x==3 && y==2)) && dir==LEFT)     
      {
        if (!second)
        {
          if (sonic.detect())
          {
            turnRight();
            return UP;
          }
          else
          {
            turnRight();
            return UP;
          }
        }
        else
        {
           if (globalDir==DOWN)
           {
              backup();
              turnRight();
              return UP;
           }
           else if (globalDir==UP)
           {
              backup();
              turnLeft();
              return DOWN;
           }
        }
      }

      else if ((x==3 && y==3) && (dir==UP || dir==RIGHT))
      {
        //Victory Dance!!! 4 clockwise spins and then halt
        //turnRight();
        //turnRight();
        //turnRight();
        //turnRight();
        //turnRight();
        //turnRight();
        //turnRight();
        //turnRight();
        //turnRight();
        //turnRight();
        //turnRight();
        //turnRight();
        //turnRight();
        //turnRight();
        //turnRight();
        //turnRight();
        while(true)
        {
          stop();
        }
      }
      else
      {
        //Something has gone wrong - start spinning counterclockwise forever!?@#
        while(true)
        {
          turnLeft();
        }
      }
             
}



