/**
 * @file
 * @brief logging utilities
 */
#ifndef __LOGGING_H__
#define __LOGGING_H__
#pragma platform(NXT)

/**
 * @brief logs a nonfatal error message
 */
void ErrorMsg(const string& errmsg)
{
    eraseDisplay();
    PlaySound(soundLowBuzz);
    nxtDisplayCenteredTextLine(3, "ERROR");
    nxtDisplayCenteredTextLine(4, errmsg);
}


/**
 * @brief logs a fatal error and terminates function
 * @note
 * there is a substantial pause in execution during this function.
 * call resetMotors() before calling this to keep robot from going crazy
 */
void ErrorFatal(const string& errmsg)
{
    eraseDisplay();
    PlaySound(soundLowBuzz);
    PlaySound(soundLowBuzz);
    nxtDisplayCenteredTextLine(3, "FATAL");
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
