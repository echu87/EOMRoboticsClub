#include <IRControl.h>
#include <MotorControl.h>
#include <UltrasonicControl.h>

IRControl irL(0);
IRControl irR(1);
MotorControl motL(1);
MotorControl motR(2);
UltrasonicControl sonic (13,12);

boolean position[7][4];
int dir, posX,posY,right_mot_fwd_speed =70;
void setup() 
{
    Serial.begin(9600);
      delay(5000);
}


void loop() 
{  
  Serial.print(irL.isBlack());
  Serial.print("\t");
  Serial.print(irR.isBlack());
  Serial.print("\t");
  Serial.println(sonic.getDistance());
  
  centering();
 
}


void centering(){
  
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
  
   else if ((irL.isBlack() == 1) && (irR.isBlack() == 1))  //hit and intersection and stop!
  {
    turnLeft();
  }
 
  }

//void setup() {
//  for(int y =0;y<7;y++){
//    for(int x =0;x<7;x++){
//      position[x][y] = false;
//      }
//    }
//    position[0][0] = true;
//    dir = 1;
//    posX=0,posY=0;
//    
//}
//
//void loop() {
//if (irL.isBlack() == 0 && irR.isBlack() == 0)
//  {
//    motL.forward(90);
//    motR.forward(90);
//  }
//  
//  else if ((irL.isBlack() == 1) && (irR.isBlack() == 0)) //moving left
//  {
//    motL.forward(120);  //turn up left wheel to correct to the right
//    motR.forward(70);
//  }
//
//   else if ((irL.isBlack() == 0) && (irR.isBlack() == 1))//moving right
//  {
//    motL.forward(70);  //turn up the right wheel to correct to the left
//    motR.forward(120);
//  }
//  
//   else if ((irL.isBlack() == 1) && (irR.isBlack() == 1))  //hit and intersection and stop!
//  {
//    for (long i=0; i<10000 ; i++)  //Note: Needs to be long for 4 byte storage
//    {                               //Rather than 2 byte storage of int
//      motL.halt();
//      motR.halt();
//    }
//
//    for (long j=0; j<20000; j++)
//    {
//      motL.forward(120);
//      motR.forward(0);
//    }
//  }
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
  int turnCounter = 1;
  kickLeft();
  while (turnCounter < 2) {
    motL.forward(0);
    motR.forward(right_mot_fwd_speed);
    if (irL.isBlack()) {
      kickLeft();
      turnCounter++;
    }
  }
}

void kickLeft() {
  while (irL.isBlack()) {
    motL.forward(0);
    motR.forward(right_mot_fwd_speed);
  }
}

//void turnRight() {
//  int turnCounter = 0;
//  kickRight();
//  while (turnCounter < 2) {
//    motR.forward(0);
//    motL.forward(right_mot_fwd_speed);
//    if (irR.isBlack()&& !irL.isBlack()) {
//      kickRight();
//      turnCounter++;
//    }
//  }
//}
//
//void kickRight() {
//  while (irR.isBlack()) {
//    motR.forward(0);
//    motL.forward(right_mot_fwd_speed);
//  }
//}
//
//void stop() {
//  for (long i = 0; i < 20000; i++) {
//    motL.halt();
//    motR.halt();
//  }
//}

