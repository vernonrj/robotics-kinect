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

/**
 * @file
 * @brief nxt entry point
 */

#include "logging.h"
#include "bluetooth.h"
//#include "servo_config.h"
#include "servos.h"

/**
 * @brief max size of message that can be received over bluetooth
 * @FIXME: is this a hardware limitation?
 */
#define BT_MAX_MSG_SIZE 5


static int processAction(ubyte *str);



/**
 * @brief main task
 */
task main()
{
    ubyte nRcvBuffer[BT_MAX_MSG_SIZE];
    bNxtLCDStatusDisplay = true;
    wait1Msec(2000); // Give time to start the program at the far end as well

    // run until a break signal is sent (set in process_result)
    int process_result;
    do {
        if (false == checkBTLinkConnected())
            ErrorFatal("Connect");
        // wait until a message is available over bluetooth, then return it
        process_result = readMessage(nRcvBuffer, BT_MAX_MSG_SIZE);
        if (process_result < 0)
        {
            LogMsg("bt failure");
            break;
        }
        // process message read from bluetooth
        process_result = processAction(nRcvBuffer);
        if (process_result < 0)
        {
            string str;
            StringFromChars(str, (char*)nRcvBuffer);
            LogMsg("bad str");
        }
        wait1Msec(1);
    } while (process_result == 0);

    return;
}



/**
 * @brief process an action contained in str and run action.
 *
 * Currently, takes a message received over bluetooth and
 * parses characters received. Can receive multiple characters,
 * so for instance, receiving an 'ff' will say "forward *2".
 * @returns
 * On success, returns 0. On error, returns -1.
 */
static int processAction(ubyte *str)
{
    motorctrl_t mctrl;
    motorctrl_create(str, &mctrl);
    int motor_d = motorctrl_motor_d(mctrl);
    int motor_e = motorctrl_motor_e(mctrl);

    if (motor_d) motor[motorD] = motor_d;
    if (motor_e) motor[motorE] = motor_e;

    return 0;
}



