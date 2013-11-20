/**
 * @file
 * @brief bluetooth processing for Robotics NXT
 * @note only tested on NXT devices
 */
#ifndef __NXT_BLUETOOTH_H__
#define __NXT_BLUETOOTH_H__

#pragma platform(NXT)

#include "logging.h"



/**
 * @brief mailbox that message will be sent to
 * @TODO
 * this might need to be changed to work in a setup other than NXT <--> NXT
 */
const TMailboxIDs DefaultQueue = mailbox1;


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

    if (!checkBTLinkConnected())
    {
        // bluetooth not connected. Return now as error
        return -1;
    }

    while (true)
    {
        // Check to see if a message is available

        nSizeOfMessage = cCmdMessageGetSize(DefaultQueue);
        if (nSizeOfMessage <= 0)
        {
            wait1Msec(1);    // Give other tasks a chance to run
            break;           // No more message this time
        }

        if (nSizeOfMessage > maxSize)
            nSizeOfMessage = maxSize;
        nBTCmdRdErrorStatus = cCmdMessageRead(nRcvBuffer,
                                              nSizeOfMessage,
                                              DefaultQueue);
        switch (nBTCmdRdErrorStatus)
        {
            case ioRsltSuccess: return 0; break;
            default: return 1; break;
        }
    }
    return 0;
}



#endif
