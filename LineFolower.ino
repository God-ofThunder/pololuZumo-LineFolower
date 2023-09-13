/*
	this program is for the pololu Zumo32U4, 
	and it will make the zumo follow a line 
	It uses the line sensors on the bottom of the zumo
*/
#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD display;
Zumo32U4Motors motor;
Zumo32U4ButtonA buttonA;
Zumo32U4LineSensors lineSensors;

#define NUM_SENSORS 5
uint16_t lineSensorValues[NUM_SENSORS];
bool useEmitters = true;
uint16_t surfaceBrightnes[NUM_SENSORS];				// an array to save the surfaceBrightnes
uint16_t defaultMotorSpeed = 200;					// the defaultMotorSpeed is the speed the zumo will drive. 
													// Max i 400 but it can drive over the line if the speed is to heigh
uint16_t motorSpeedLeft = defaultMotorSpeed;		// motorSpeedRight and left is defined so the zumo is able to turn more smooth
uint16_t motorSpeedRight = defaultMotorSpeed;
uint8_t lowMotorSpeedChange = 1;					// low and heighMotorSpeedChange is to change the motor speed so the zumo can turn
uint8_t heighMotorSpeedChange = defaultMotorSpeed/2;
uint8_t lineTreshHold = 150;						// lineTreshHold is the amount the brightnes needs to change for the zumo to se a line

void setup() {
	// prints "push A to calibrate and waits for button A to be pushed
	display.clear();
	display.gotoXY(0,0);
	display.print("push A");
	display.gotoXY(0,1);
	display.print("to calibrate");
	buttonA.waitForButton();

	// setting the surfaceBrightnes
	lineSensors.initFiveSensors();
	lineSensors.read(lineSensorValues, useEmitters ? QTR_EMITTERS_ON: QTR_EMITTERS_OFF); // reads the values from the sensors
	for(uint8_t i=0; i<NUM_SENSORS; i++){
		surfaceBrightnes[i] = lineSensorValues[i];
	}

	// prints "Ready push A" and waits for button A to be pushed
	display.clear();
	display.gotoXY(0,0);
	display.print("Ready");
	display.gotoXY(0,1);
	display.print("push A");
	buttonA.waitForButton();
	display.clear();
	delay(100);
}

// prints 2 surfaceBrightnes values and the coresponding line sensor readings
void printReadingsToDisplay(){
	display.clear();
	display.gotoXY(0,0);
	display.print(surfaceBrightnes[0]);
	display.print(" ");
	display.print(surfaceBrightnes[1]);
	display.gotoXY(0,1);
	display.print(lineSensorValues[0]);
	display.print(lineSensorValues[4]);
}

void checkForLineAndChangingMotorSpeed(){
	// check sensor 0 (the sensor in the left corner)
	if(lineSensorValues[0] < surfaceBrightnes[0]-lineTreshHold || lineSensorValues[0] > surfaceBrightnes[0]+lineTreshHold){
		// check if the zumo is turning right and if it is the set the motor to the same speed befor turning
		if(motorSpeedLeft > motorSpeedRight){
			motorSpeedLeft = defaultMotorSpeed;
			motorSpeedRight = defaultMotorSpeed;
		}
		motorSpeedLeft -= heighMotorSpeedChange; // set the motors to turn left 
		motorSpeedRight += heighMotorSpeedChange;
	}
	// chech sensor 4 (the sensor in the right corner)
	else if(lineSensorValues[4] < surfaceBrightnes[4]-lineTreshHold || lineSensorValues[4] > surfaceBrightnes[4]+lineTreshHold){
		// check if the zumo is turning left and if it is the set the motor to the same speed befor turning
		if(motorSpeedRight > motorSpeedLeft){
			motorSpeedLeft = defaultMotorSpeed;
			motorSpeedRight = defaultMotorSpeed;
		}
		motorSpeedLeft += heighMotorSpeedChange; // set the motor to turn right
		motorSpeedRight -= heighMotorSpeedChange;
	}
	// check sensor 1 (the left sensor in the middle) 
	else if(lineSensorValues[1] < surfaceBrightnes[1]-lineTreshHold || lineSensorValues[1] > surfaceBrightnes[1]+lineTreshHold){
		// check if the zumo is turning right and if it is the set the motor to the same speed befor turning
		if(motorSpeedLeft > motorSpeedRight){
			motorSpeedLeft = defaultMotorSpeed;
			motorSpeedRight = defaultMotorSpeed;
		}
		motorSpeedLeft -= lowMotorSpeedChange; // set the motors trun left
		motorSpeedRight += lowMotorSpeedChange;
	}
	// check sensor 3 (the right sensor in the middle)
	else if(lineSensorValues[3] < surfaceBrightnes[3]-lineTreshHold || lineSensorValues[3] > surfaceBrightnes[3]+lineTreshHold){
		// check if the zumo is turning left and if it is the set the motor to the same speed befor turning
		if(motorSpeedRight > motorSpeedLeft){
			motorSpeedLeft = defaultMotorSpeed;
			motorSpeedRight = defaultMotorSpeed;
		}
		motorSpeedLeft += lowMotorSpeedChange; // set the motors turn right
		motorSpeedRight -= lowMotorSpeedChange;
	}
	/*else{
		motorSpeedLeft = defaultMotorSpeed;
		motorSpeedRight = defaultMotorSpeed;
	}*/
}

void loop() {
	lineSensors.read(lineSensorValues, useEmitters ? QTR_EMITTERS_ON: QTR_EMITTERS_OFF); // reads the values from the sensors
	printReadingsToDisplay();
	motor.setSpeeds(motorSpeedLeft, motorSpeedRight); // sets the motor speed
	checkForLineAndChangingMotorSpeed();
}
