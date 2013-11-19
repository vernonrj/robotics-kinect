/**
 * @file
 * @brief bluetooth processing for Robotics NXT
 * @FIXME move main() out of this header
 */
#ifndef __NXT_BLUETOOTH_H__
#define __NXT_BLUETOOTH_H__

#pragma platform(NXT)
#include "servo_config.h"
#include "logging.h"


const int kMaxSizeOfMessage = 5;
const TMailboxIDs kQueueID = mailbox1;


/**
 * @brief returns True if bluetooth is connected
 */
bool checkBTLinkConnected()
{
    return nBTCurrentStreamIndex >= 0;
}


/**
 * @brief returns a message received over Bluetooth
 * @param nRcvBuffer
 * buffer to store message in
 * @param maxSize
 * number of characters, including null terminator, that nRcvBuffer can hold
 * @returns
 * On success, returns 0.
 * On error, returns -1.
 * On unknown, returns a positive number
 */
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


/**
 * @brief main task
 */
task main()
{
    ubyte nRcvBuffer[kMaxSizeOfMessage];
    bNxtLCDStatusDisplay = true;
    wait1Msec(2000); // Give time to start the program at the far end as well

    //
    // Send and receive 1M messages
    //
    for (long nSendTotal = 0; nSendTotal < 1000000; ++nSendTotal)
    {
        if (false == checkBTLinkConnected())
            ErrorFatal("Connect");
        int success = readMessage(nRcvBuffer, kMaxSizeOfMessage);
        if (success == 0)
        {
            string str;
            StringFromChars(str, (char*)nRcvBuffer);
            LogMsg(str);
        }
        wait1Msec(1);
    }

    return;
}

// if (message == 'f'){
//   motor[motorD] = 50;
//   motor[motorE] = 50;
// }
// else if (message == 'b'){
//   motor[motorD] = -50;
//   motor[motorE] = -50;
// }
// else if (message == 'l'){
//   motor[motorD] = 0;
//   motor[motorE] = 50;
// }
// else if (message == 'r'){
//   motor[motorD] = 50;
//   motor[motorE] = 0;


#endif
