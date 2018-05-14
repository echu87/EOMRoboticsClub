#include <IRControl.h>
#include <MotorControl.h>
#include <UltrasonicControl.h>

#include <IRControl.h>
#include <MotorControl.h>
#include <UltrasonicControl.h>

IRControl irL(0);
IRControl irR(1);
MotorControl motL(1);
MotorControl motR(2);
UltrasonicControl sonic (13,12);
const int LEFT=1, UP=2, RIGHT=3, DOWN=0;
boolean moveableSpot[4];
int dir, posX,posY,right_mot_fwd_speed =80;
void setup() 
{
    Serial.begin(9600);
      delay(5000);
    dir = UP;
    posX=0,posY=0;
    
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
  motL.forward(90);
    motR.forward(90);
  }
 
  }


void turnLeft() {
  int turnCounter = 0;
  kickLeft();
  while (turnCounter < 2) {
    motL.forward(0);
    motR.forward(right_mot_fwd_speed);
    if (irL.isBlack()) {
      kickLeft();
      turnCounter++;
    }
  }
 for(int i =0;i <300;i++){
  kickRight();
  }
}

void kickLeft() {
  while (irL.isBlack()) {
    motL.forward(0);
    motR.forward(right_mot_fwd_speed);
  }
}
void kickRight() {
  while (irR.isBlack()) {
    motR.forward(0);
    motL.forward(right_mot_fwd_speed);
  }
}

void turnRight() {
  int turnCounter = 0;
  kickRight();
  
  while (turnCounter < 1) {
    motR.forward(0);
    motL.forward(right_mot_fwd_speed*4);
    if (irR.isBlack()) {
      kickRight();
      turnCounter++;
    }
  }
   for(int i =0;i <300;i++){
  kickLeft();
  }
  
}

void node()
{
  clearPresets();
 switch(dir){
    case UP: posY++;
    break;
    case DOWN: posY--;
    break;
    case RIGHT: posX++;
    break;
    case LEFT: posX--;
    }
    if(sonic.detect()){
    moveableSpot[dir] =false;
    }
    if(!moveableSpot[dir]){
      for(int i =1; i < 4; i++){
        if(moveableSpot[i]){
          turn(i);
          }
        }
      }else{
        turnRight();
        turnRight();
        
        }
}

void clearPresets(){
  for(int i =0; i < 4 ; i++)
  {
    moveableSpot[i] =true;  
  }
  switch(posX){
    case 0: moveableSpot[LEFT] =false;
    break;
    case 3: moveableSpot[RIGHT] = false;
    }
   switch(posY){
    case 6: moveableSpot[UP] =false;
    break;
    case 0: moveableSpot[DOWN] = false;
    }
  
  }
  void turn(int rotation){
    rotation = rotation - dir;
    if(rotation>0){
    turnRight();
    dir += rotation;
    }
    else if(rotation<0){
    turnLeft();
    dir += rotation;
    }
    else{
      turnRight();
      turnRight();
      if(dir+2 > 3){
        dir = dir+2 -3;
      }
        else
        dir += 2;
        }
      
    
    }



