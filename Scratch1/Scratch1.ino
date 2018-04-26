#include <IRControl.h>
#include <MotorControl.h>
#include <UltrasonicControl.h>

IRControl irL(0);
IRControl irR(1);
MotorControl motL(1);
MotorControl motR(2);
UltrasonicControl sonic(13,12);

const int left_mot_fwd_speed=74, right_mot_fwd_speed=76;
const int correct_mot_speed=100;
const int left_turn_speed=85, right_turn_speed=85;

void setup() {

}

void loop() {
  
  if (irL.isBlack() == 0 && irR.isBlack() == 0) {
    motL.forward(left_mot_fwd_speed);
    motR.forward(right_mot_fwd_speed);
  }

  else if (irL.isBlack() == 1 && irR.isBlack() == 0) {
    motL.forward(correct_mot_speed);
    motR.forward(right_motor_foward_speed);
  }

  else if (irL.isBlack() == 0 && irR.isBlack() == 1) {
    motL.forward(left_motor_foward_speed);
    motR.forward(correct_mot_speed);
  }

  else if (irL.isBlack() == 1 && irR.isBlack() == 1) {
    stop();
    turnLeft();
  }
  
}

void turnLeft() {
  int turnCounter = 0;
  kickLeft();
  while (turnCounter < 2) {
    motL.forward(0);
    motR.forward(right_motor_forward_speed);
    if (irR.isBlack()) {
      kickLeft();
      turnCounter++;
    }
  }
}

void kickLeft() {
  while (irR.isBlack()) {
    motL.forward(0);
    motR.forward(right_motor_forward_speed);
  }
}

void stop() {
  for (long i = 0; i < 20000; i++) {
    motL.halt();
    motR.halt();
  }
}

