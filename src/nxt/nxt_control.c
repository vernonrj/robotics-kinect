/**
 * @file
 * @brief nxt entry point
 */
#include "logging.h"
#include "bluetooth.h"
#include "servo_config.h"
#include "servos.h"


static int processAction(const string& str);


/**
 * @brief main task
 */
task main()
{
    ubyte nRcvBuffer[kMaxSizeOfMessage];
    bNxtLCDStatusDisplay = true;
    wait1Msec(2000); // Give time to start the program at the far end as well

    // run forever or until a break signal is sent
    while (true)
    {
        if (false == checkBTLinkConnected())
            ErrorFatal("Connect");
        int success = readMessage(nRcvBuffer, kMaxSizeOfMessage);
        if (success == 0)
        {
            string str;
            StringFromChars(str, (char*)nRcvBuffer);
            //LogMsg(str);
            int action_success = processAction(str);
            if (action_success > 0)
                break;
            else if (action_success < 0)
            {
                log("bad str: %s", str);
            }
        }
        wait1Msec(1);
    }

    return;
}



/**
 * @brief process an action contained in str and run action
 * @returns
 * On success, returns 0. On error, returns -1.
 */
int processAction(const string& str)
{
    motorctrl_t message = get_motorctl(str);
    
    if (is_forward(message))
    {
      motor[motorD] = 50;
      motor[motorE] = 50;
    }
    else if (is_backward(message))
    {
      motor[motorD] = -50;
      motor[motorE] = -50;
    }
    else if (is_left(message))
    {
      motor[motorD] = 0;
      motor[motorE] = 50;
    }
    else if (is_right(message))
    {
      motor[motorD] = 50;
      motor[motorE] = 0;
    }
    else
    {
        return -1;
    }
    return 0;
}



