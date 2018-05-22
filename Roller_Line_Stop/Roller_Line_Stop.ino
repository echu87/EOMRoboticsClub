#include <IRControl.h>
#include <MotorControl.h>
#include <UltrasonicControl.h>

IRControl ir1(0);
IRControl ir2(1);
MotorControl mot1(1);
MotorControl mot2(2);
UltrasonicControl sonic (13,12);
   
void setup() 
{
    Serial.begin(9600);
      delay(5000);
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
    mot1.forward(90);
    mot2.forward(90);
  }
  
  else if ((ir1.isBlack() == 1) && (ir2.isBlack() == 0)) //moving left
  {
    mot1.forward(120);  //turn up left wheel to correct to the right
    mot2.forward(70);
  }

   else if ((ir1.isBlack() == 0) && (ir2.isBlack() == 1))//moving right
  {
    mot1.forward(70);  //turn up the right wheel to correct to the left
    mot2.forward(120);
  }
  
   else if ((ir1.isBlack() == 1) && (ir2.isBlack() == 1))  //hit and intersection and stop!
  {
    for (long i=0; i<10000 ; i++)  //Note: Needs to be long for 4 byte storage
    {                               //Rather than 2 byte storage of int
      mot1.halt();
      mot2.halt();
    }

    for (long j=0; j<20000; j++)
    {
      mot1.forward(120);
      mot2.forward(0);
    }
  }
 
}
