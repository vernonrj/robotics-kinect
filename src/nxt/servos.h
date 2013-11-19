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
    int motor_d;
    int motor_e;
} motorctrl_t;


/**
 * @brief return a motorctrl_t structure with motor magnitude specs
 * @param str char array returned from a bluetooth read
 * @returns Returns a structure for setting motor values
 */
motorctrl_t motorctrl_create(ubyte *str)
{
    motorctrl_t m =
    {
        .motor_d = 0,
        .motor_e = 0,
    };
    while (*str != '\0')
    {
        const ubyte motor_spec = *str;
        if ('f' == motor_spec)
        {
            m.motor_d += 50;
            m.motor_e += 50;
        }
        else if ('b' == motor_spec)
        {
            m.motor_d += -50;
            m.motor_e += -50;
        }
        else if ('l' == motor_spec)
        {
            m.motor_d += 0;
            m.motor_e += 50;
        }
        else if ('r' == motor_spec)
        {
            m.motor_d += 50;
            m.motor_e += 0;
        }
        str++;
    }
    m.motor_d = motorctrl_bound_value(m.motor_d);
    m.motor_e = motorctrl_bound_value(m.motor_e);
    return m;
}

/**
 * @brief ensure motor_val is between -100 and 100
 */
int motorctrl_bound_value(int motor_val)
{
    if (motor_val < -100)
        return -100;
    else if (motor_val > 100)
        return 100;
    else
        return motor_val;
}

/**
 * @brief return the motor value for motorD
 */
int motorctrl_motor_d(const motorctrl_t& m)
{
    return m.motor_d;
}

/**
 * @brief return the motor value for motorE
 */
int motorctrl_motor_e(const motorctrl_t& m)
{
    return m.motor_e;
}

#endif
