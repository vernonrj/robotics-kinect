/////////////////////////////////// SETUP SERVOS AND MOTORS ////////////////////////////////////////////
#pragma config(Hubs,  S1, HTMotor,  HTServo,  none,     none)
#pragma config(Motor,  mtr_S1_C1_1,     motorD,        tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     motorE,        tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Servo,  srvo_S1_C2_1,    servo1,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_3,    fingers,              tServoStandard)
#pragma config(Servo,  srvo_S1_C2_4,    thumb,                tServoStandard)
#pragma config(Servo,  srvo_S1_C2_5,    wrist,                tServoStandard)
#pragma config(Servo,  srvo_S1_C2_6,    shoulder,             tServoStandard)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//
////////////////////////////////////////////////////////////////////////////////////////////////////////

