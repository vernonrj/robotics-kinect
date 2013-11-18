/**
 * @file
 * @brief logging utilities
 */
#ifndef __LOGGING_H__
#define __LOGGING_H__
/**
 * @brief logs a fatal error and terminates function
 */
void ErrorFatal(const string& errmsg);

/**
 * @brief logs a message stored in msg
 */
void LogMsg(const string& msg);

#endif
