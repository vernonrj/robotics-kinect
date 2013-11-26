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
		drive = true;

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
           if (drive){
		         if (str == "f"){ motor[motorD] = 50; motor[motorE] = 50; }

						 else if (str == "b"){motor[motorD] = -50; motor[motorE] = -50; }

						 else if (str == "l"){motor[motorD] = 0; motor[motorE] = 50; }

						 else if (str == "r"){motor[motorD] = 50; motor[motorE] = 0; }

						 else if (str == "t"){motor[motorD] = 0; motor[motorE] = 0; }

						 else if (str == "s"){drive = false;}}

					  else if (!drive){
					  	if (str == "a"){servo[hand_oc] = 100;LogMsg(str);}

					  	else if (str == "e"){servo[hand_oc] = -100;}

					  	else if(str == "c"){servo[wrist_lr] = 10;}

					  	else if (str == "d"){servo[wrist_lr] = -10;}

					  	else if (str == "g"){servo[wrist_ud] = 10;}

					  	else if (str == "h"){servo[wrist_ud] = -10;}

					  	//else if (str == "i"){servo[shoulder_lr] = -10;}

					  	//else if (str == "j"){servo[shoulder_lr] = -10;}

					  	//else if (str == "k"){servo[wrist] = -10;}

					  	//else if (str == "m"){servo[wrist] = -10;}

					  	else if (str == "s"){drive = true;}}
						//nSendTotal = 0;
        }
        wait1Msec(1);
    }

    return;
}