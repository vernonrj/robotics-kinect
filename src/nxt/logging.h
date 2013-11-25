/**
 * @file
 * @brief logging utilities
 */
#ifndef __LOGGING_H__
#define __LOGGING_H__
#pragma platform(NXT)


/**
 * @brief logs a fatal error and terminates function
 */
void ErrorFatal(const string& errmsg)
{
    eraseDisplay();
    nxtDisplayCenteredTextLine(3, "ERROR");
    nxtDisplayCenteredTextLine(4, errmsg);
    wait1Msec(3000);
    StopAllTasks();
}


/**
 * @brief logs a message stored in msg
 */
void LogMsg(const string& msg)
{
    eraseDisplay();
    nxtDisplayCenteredTextLine(3, "LOG");
    nxtDisplayCenteredTextLine(4, msg);
}



#endif
