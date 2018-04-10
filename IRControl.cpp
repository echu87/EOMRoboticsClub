#include <Arduino.h>
#include "IRControl.h"
IRControl::IRControl(int pin){
	_pin = pin;
}
boolean IRControl::isBlack (){
	/*
		Usually when the sensor is reading the while board 
		the reading is in the range of 40-100 and when it is
		black, it is usually around 500. The 400 is a safe 
		point.
		I havent found a way to detect other colours, its a 
		linear sensor.
	*/
	return analogRead(_pin)>900; 
}
