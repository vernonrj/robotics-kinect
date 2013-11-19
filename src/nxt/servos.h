/**
 * @file
 * @brief servo/motor information
 */
#ifndef __SERVOS_H__
#define __SERVOS_H__

/**
 * @brief control structure used in boolean functions
 */
typedef struct motorctrl_t
{
    str m_message;
    int m_motor_d;
    int m_motor_e;
} motorctrl_t;


/**
 * @brief return a motorctrl_t with values to plug into motors
 * @param str value returned from a bluetooth read
 * @returns Returns a structure for setting motor values
 */
motorctrl_t motorctrl_create(const string& str)
{
    motorctrl_t m =
    {
        .m_message = str[0],
        .m_motor_d = 0,
        .m_motor_e = 0,
    };

    if ('f' == m.m_message)
    {
        m.motor_d += 50;
        m.motor_e += 50;
    }
    if ('b' == m.m_message)
    {
        m.motor_d += -50;
        m.motor_e += -50;
    }
    if ('l' == m.m_message)
    {
        m.motor_d += 0;
        m.motor_e += 50;
    }
    if ('r' == m.m_message)
    {
        m.motor_d += 50;
        m.motor_e += 0;
    }
    return m;
}


/**
 * @brief return the motor value for motorD
 */
int motorctrl_motor_d(const motorctrl_t& m)
{
    return m.m_motor_d;
}

/**
 * @brief return the motor value for motorE
 */
int motorctrl_motor_e(const motorctrl_t& m)
{
    return m.m_motor_e;
}

#endif
