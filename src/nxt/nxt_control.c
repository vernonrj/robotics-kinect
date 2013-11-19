/**
 * @file
 * @brief nxt entry point
 */
#include "servo_config.h"


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


