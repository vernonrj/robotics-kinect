/**
 * @file
 * @brief bluetooth processing for Robotics NXT
 */
#ifndef __NXT_BLUETOOTH_H__
#define __NXT_BLUETOOTH_H__

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
int readMessage(ubyte *nRcvBuffer, int maxSize);

#endif
