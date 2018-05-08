#include <IRControl.h>
#include <MotorControl.h>
#include <UltrasonicControl.h>

IRControl irL(0);
IRControl irR(1);
MotorControl motL(1);
MotorControl motR(2);
UltrasonicControl sonic (13,12);
 
boolean position[7][4];
int dir, posX,posY;
void setup() 
{
    Serial.begin(9600);
      delay(5000);
       for(int y =0;y<7;y++){
    for(int x =0;x<7;x++){
      position[x][y] = false;
      }
    }
    position[0][0] = true;
    dir = 1;
    posX=0,posY=0;
}


void loop() 
{  
  Serial.print(irL.isBlack());
  Serial.print("\t");
  Serial.print(irR.isBlack());
  Serial.print("\t");
  Serial.println(sonic.getDistance());

 moving();
}


void moving(){
  
 if (irL.isBlack() == 0 && irR.isBlack() == 0)
  {
    motL.forward(90);
    motR.forward(90);
  }
  
  else if ((irL.isBlack() == 1) && (irR.isBlack() == 0)) //moving left
  {
    motL.forward(120);  //turn up left wheel to correct to the right
    motR.forward(70);
  }

   else if ((irL.isBlack() == 0) && (irR.isBlack() == 1))//moving right
  {
    motL.forward(70);  //turn up the right wheel to correct to the left
    motR.forward(120);
  }

//   else if ((irL.isBlack() == 1) && (irR.isBlack() == 1))  //hit and intersection and stop!
//  {
//   turnLeft();
//  }
 
  }



////   if ((irL.isBlack() == 1) && (irR.isBlack() == 1)) {
////       motL.forward(left_mot_fwd_speed);
////    motR.forward(right_mot_fwd_speed);
//////    stop();
//////    if(random(10)>5){
//////      turnLeft();
//////      }
//////      else{
//////         turnRight();
//////        }
////  }else{
////    centering();
////    }
//}
//
void turnLeft() {
  int turnCounter = 0;
  kickLeft();
  while (turnCounter < 2) {
    motL.forward(0);
    motR.forward(120);
    if (irL.isBlack()&& !irR.isBlack()) {
      kickLeft();
      turnCounter++;
    }
  }
}

void kickLeft() {
  while (irL.isBlack()) {
    motL.forward(0);
    motR.forward(120);
  }
}

void turnRight() {
  int turnCounter = 0;
  kickRight();
  while (turnCounter < 2) {
    motR.forward(0);
    motL.forward(70);
    if (irR.isBlack()&& !irL.isBlack()) {
      kickRight();
      turnCounter++;
    }
  }
}

void kickRight() {
  while (irR.isBlack()) {
    motR.forward(0);
    motL.forward(70);
  }
}

void stop() {
  for (long i = 0; i < 20000; i++) {
    motL.halt();
    motR.halt();
  }
}

