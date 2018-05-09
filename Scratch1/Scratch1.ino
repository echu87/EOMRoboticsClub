#include <IRControl.h>
#include <MotorControl.h>
#include <UltrasonicControl.h>

IRControl irL(0);
IRControl irR(1);
MotorControl motL(1);
MotorControl motR(2);
UltrasonicControl sonic (13,12);
const int UP=0, RIGHT=1, DOWN=2, LEFT=3;
boolean moveableSpot[4];
int dir, posX,posY,right_mot_fwd_speed =900,left_mot_fwd_speed = 900;
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
   if(sonic.detect())
   turnLeft();
   else
   turnRight();
  }
 
  }


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
  kickRight();
}

void kickLeft() {
  while (irL.isBlack()) {
    motL.forward(0);
    motR.forward(right_mot_fwd_speed);
  }
}
void pushLeft() {
  while (!irL.isBlack()) {
    motL.forward(0);
    motR.forward(right_mot_fwd_speed);
  }
}
void kickRight() {
  while (irR.isBlack()) {
    motR.forward(0);
    motL.forward(left_mot_fwd_speed);
  }
}
void pushRight() {
  while (!irR.isBlack()) {
    motR.forward(0);
    motL.forward(left_mot_fwd_speed);
  }
}
void turnRight() {
  int turnCounter = 1;
  kickRight();
  while (turnCounter < 2) {
    motR.forward(0);
    motL.forward(left_mot_fwd_speed);
    if (irR.isBlack()) {
      kickRight();
      turnCounter++;
    }
  }
  kickLeft();
}

void node(int i)
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
    moveableSpot[dir] = false;
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



