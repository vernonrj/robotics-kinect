#pragma config(Hubs,  S1, HTMotor,  HTServo,  none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     motorD,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     motorE,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C2_1,    thumb,                tServoStandard)
#pragma config(Servo,  srvo_S1_C2_2,    fingers,              tServoStandard)
#pragma config(Servo,  srvo_S1_C2_3,    wrist,                tServoStandard)
#pragma config(Servo,  srvo_S1_C2_4,    shoulder,             tServoStandard)
#pragma config(Servo,  srvo_S1_C2_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(NXT)

/**
 * @file
 * @brief nxt entry point
 */

#include "logging.h"
#include "bluetooth.h"
#include "servos.h"

/**
 * @brief max size of message that can be received over bluetooth
 */
#define BT_MAX_MSG_SIZE MOTORCTRL_MAX_RES


/**
 * @brief timeout before motors are stopped, or disabled if zero
 *
 * If nonzero, motors are stopped if a bluetooth message hasn't been
 * received in a specified period of time.
 *
 * If zero, timeout is disabled.
 */
#define BT_MSG_TIMEOUT 0

static void resetMotors(void);
static int processAction(ubyte *str);
static void setMotorValues(void);


static motorctrl_t MotorCfg;

/**
 * @brief main task
 */
task main()
{
    // clear the TETRIX encoders in motors D and E
    resetMotors();
    memset(&MotorCfg, 0x0, sizeof(MotorCfg));
    // Bluetooth receive buffer
    ubyte nRcvBuffer[BT_MAX_MSG_SIZE+1];

    // set LCD to display messages
    bNxtLCDStatusDisplay = true;
    // Give time to start the program at the far end as well
    wait1Msec(2000);

    // run until a break signal is sent (set in process_result)
    int process_result = 0;
    do {
        if (false == checkBTLinkConnected())
        {
            ErrorFatal("Disconnected");
            return;
        }
        // wait until a message is available over bluetooth, then return it
        process_result = readMessage(nRcvBuffer, BT_MAX_MSG_SIZE+1);
        if (process_result < 0)
        {
            ErrorFatal("bt read failure");
        }
        else if (process_result > 0)
        {
            // No messages to read.
#if BT_MSG_TIMEOUT != 0
            // Check time since last message was received
            if (time1[T1] >= 100)
            {
                // zero out if running for too long without an update
                //resetMotors();
            }
#endif
            wait1Msec(1);
            process_result = 0;
        }
        else
        {
            // uncomment to send message back to confirm received
            // TODO: this didn't seem to work when it was uncommented
            //writeMessage(nRcvBuffer, BT_MAX_MSG_SIZE);
            // process message read from bluetooth
            process_result = processAction(nRcvBuffer);
            if (process_result < 0)
            {
                resetMotors();
                ErrorMsg("bad str");
                wait1Msec(1);
                process_result = 0;
            }
            else if (process_result > 0)
                break;
        }
    } while (true);

    resetMotors();
    LogMsg("Shutdown");
    wait1Msec(5000);

    return;
}


/**
 * @brief shut off all running motors and servos
 */
static void resetMotors(void)
{
    MotorCfg.motor_left = 0;
    MotorCfg.motor_right = 0;
    setMotorValues();
    // restart heartbeat
    ClearTimer(T1);
}


/**
 * @brief process an action contained in str and run action.
 *
 * Currently, takes a message received over bluetooth and
 * parses characters received. Can receive multiple characters,
 * so for instance, receiving an 'ff' will say "forward *2".
 * @returns
 * On success, returns 0.
 * On shutdown signal, returns 1.
 * On error, returns -1.
 */
static int processAction(ubyte *str)
{
    int remote_status = motorctrl_update(&MotorCfg, str);
    if (remote_status == -1)
        return 0;
    setMotorValues();
    int motor_left = motorctrl_motor_left(MotorCfg);
    int motor_right = motorctrl_motor_right(MotorCfg);

    string log_str;
    if (motor_left > 0 && motor_right > 0) LogMsg("Forward");
    else if (motor_left > 0) LogMsg("Right");
    else if (motor_right > 0) LogMsg("Left");
    else if (motor_left < 0 && motor_right < 0) LogMsg("Back");
    else LogMsg("Stop");


    //sprintf(log_str, "%d | %d", motor_left, motor_right);
    //LogMsg(log_str);
    ClearTimer(T1);

    if (remote_status > 0)
    {
        // shutdown system
        return 1;
    }

    return 0;
}


/**
 * @brief set all motors and servos based on values in MotorCfg
 */
static void setMotorValues()
{
    motor[motorD] = motorctrl_motor_left(MotorCfg);
    motor[motorE] = motorctrl_motor_right(MotorCfg);
}


