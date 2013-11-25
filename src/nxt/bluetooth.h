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
 *
 * @param data_out buffer to store message in
 *
 * @param maxSize
 * number of characters, including null terminator, that data_out can hold
 *
 * @returns
 * On success, returns 0. On error, returns -1. 
 * On unknown, returns a positive number
 */
int readMessage(ubyte *data_out, int maxSize)
{
    int msg_size;

    if (!checkBTLinkConnected())
    {
        // bluetooth not connected. Return now as error
        return -1;
    }

    while (true)
    {
        // Check to see if a message is available

        msg_size = cCmdMessageGetSize(DefaultQueue);
        if (msg_size <= 0)
        {
            wait1Msec(1);    // Give other tasks a chance to run
            continue;        // No more message this time
        }

        if (msg_size > maxSize)
        {
            // truncate message
            msg_size = maxSize;
        }
        TFileIOResult read_success;
        read_success = cCmdMessageRead(data_out, msg_size, DefaultQueue);
        switch (read_success)
        {
            case ioRsltSuccess: return 0; break;
            default: return 1; break;
        }
    }
    return 0;
}


/**
 * @brief writes a message out over Bluetooth
 *
 * @param data_out buffer message is stored in
 *
 * @param msg_size size of buffer message
 */
void writeMessage(ubyte *data_out, int msg_size)
{
    if (!checkBTLinkConnected())
    {
        return;
    }

    cCmdMessageWriteToBluetooth(DefaultQueue, data_out, msg_size);
}


#endif
