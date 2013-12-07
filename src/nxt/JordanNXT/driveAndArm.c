#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     motorD,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     motorE,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     motorF,        tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     motorG,        tmotorTetrix, PIDControl, encoder)
#pragma config(Servo,  srvo_S1_C3_1,    hand_oc,              tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    wrist_lr,             tServoStandard)
#pragma config(Servo,  srvo_S1_C3_3,    wrist_ud,             tServoStandard)
#pragma config(Servo,  srvo_S1_C3_4,    shoulder_lr,          tServoStandard)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                    Bluetooth Receiving
//
// Modified from ROBOTC NXT BT Messaging No Error Checking.c
// Functions for receiving data over bluetooth
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma platform(NXT)
//#include "bluetooth.h"
//long nLastXmitTimeStamp = nPgmTime;
//long nDeltaTime         = 0;

//const int kTimeBetweenXmit = 30;
const int kMaxSizeOfMessage = 5;
const TMailboxIDs kQueueID = mailbox1;


void ErrorFatal(const string& errmsg)
{
    PlaySound(soundLowBuzz);
    PlaySound(soundLowBuzz);
    eraseDisplay();
    nxtDisplayCenteredTextLine(3, "ERROR");
    nxtDisplayCenteredTextLine(4, errmsg);
    wait1Msec(3000);
    StopAllTasks();
}

void LogMsg(const string& errmsg)
{
    PlaySound(soundLowBuzz);
    eraseDisplay();
    nxtDisplayCenteredTextLine(3, "LOG");
    nxtDisplayCenteredTextLine(4, errmsg);
    wait1Msec(3000);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                           Check to See if Bluetooth Link is Connection
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

bool checkBTLinkConnected()
{
    return nBTCurrentStreamIndex >= 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                        Receive Messages Task
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

int readMessage(ubyte *nRcvBuffer, int maxSize)
{
    TFileIOResult nBTCmdRdErrorStatus;
    int nSizeOfMessage;

    while (true)
    {
        // Check to see if a message is available

        nSizeOfMessage = cCmdMessageGetSize(kQueueID);
        if (nSizeOfMessage <= 0)
        {
            wait1Msec(1);    // Give other tasks a chance to run
            break;           // No more message this time
        }

        if (nSizeOfMessage > maxSize)
            nSizeOfMessage = maxSize;
        nBTCmdRdErrorStatus = cCmdMessageRead(nRcvBuffer, nSizeOfMessage, kQueueID);
        switch (nBTCmdRdErrorStatus)
        {
            case ioRsltSuccess: return 0; break;
            default: return 1; break;
        }
    }
    return -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                        Main Task
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

task main()
{
	ubyte nRcvBuffer[kMaxSizeOfMessage];
	bNxtLCDStatusDisplay = true;
	wait1Msec(2000); // Give time to start the program at the far end as well
	string str;
	bool drive;
	bool arm;
	drive = true;
	arm = true;

	//servoChangeRate[hand_oc] = 2;
	//servoChangeRate[wrist_lr] = 2;
	//servoChangeRate[wrist_ud] = 2;
	//servoChangeRate[shoulder_lr] = 2;
	servoTarget[hand_oc] = 200;
	servoTarget[wrist_lr] = 150;
	servoTarget[wrist_ud] = 80;
	servoTarget[shoulder_lr] = 127;

	//
	// Send and receive 1M messages
	//
	for (long nSendTotal = 0; nSendTotal < 1000000; ++nSendTotal)
	{
		if (false == checkBTLinkConnected())
			ErrorFatal("Connect");
		//sendDataMsg();
		int success = readMessage(nRcvBuffer, kMaxSizeOfMessage);
		if (success == 0)
		{
			StringFromChars(str, (char*)nRcvBuffer);
			if (drive)
			{
				if (str == "f")
				{
					motor[motorD] = 50;
					motor[motorE] = 50;
				}
				else if (str == "q")
				{
					motor[motorD] = 100;
					motor[motorE] = 100;
				}

				else if (str == "b")
				{
					motor[motorD] = -50;
					motor[motorE] = -50;
				}
				else if (str == "l")
				{
					motor[motorD] = 0;
					motor[motorE] = 50;
				}
				else if (str == "r")
				{
					motor[motorD] = 50;
					motor[motorE] = 0;
				}

				else if (str == "t")
				{
					motor[motorD] = 0;
					motor[motorE] = 0;
				}

				else if (str == "s")
					drive = false;
			}

			else if (!drive)
			{
				if (!arm)
				{
					if (str == "a")
						servo[hand_oc] = ServoValue(hand_oc) + 5;

					else if (str == "e")
						servo[hand_oc] = ServoValue(hand_oc) - 5;

					else if(str == "c")
						servo[wrist_lr] = ServoValue(wrist_lr) + 5;

					else if (str == "d")
						servo[wrist_lr] = ServoValue(wrist_lr) - 5;

					else if (str == "g")
						servo[wrist_ud] = ServoValue(wrist_ud) + 5;

					else if (str == "h")
						servo[wrist_ud] = ServoValue(wrist_ud) - 5;

					else if (str == "s")
						drive = true;

					else if (str == "w")
						arm = true;
				}

				else if (arm)
				{
					if ((str == "i") && (ServoValue(hand_oc) <= 220))
						servo[shoulder_lr] = ServoValue(shoulder_lr) + 5;

					else if ((str == "j") && (ServoValue(hand_oc) >= 20))
						servo[shoulder_lr] = ServoValue(shoulder_lr) - 5;

					else if (str == "k")
					{
						motor[motorF] = 25;
						wait1Msec(100);
						motor[motorF] = 0;
					}

					else if (str == "m")
					{
						motor[motorF] = -25;
						wait1Msec(100);
						motor[motorF] = 0;
					}

					else if (str == "x")
					{
						motor[motorG] = 15;
						wait1Msec(100);
						motor[motorG] = 0;
						motor[motorG] = 10;
						wait1Msec(100);
						motor[motorG] = 0;
					}


					else if (str == "z")
					{
						motor[motorG] = -30;
						wait1Msec(300);
						motor[motorG] = 0;
					}

					else if (str == "s")
						drive = true;

					else if (str == "w")
						arm = false;
				}
			}
						//nSendTotal = 0;
        }
        wait1Msec(1);
    }
    return;
}
