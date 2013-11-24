/**
 * @file
 * @brief servo/motor information
 */
#ifndef __SERVOS_H__
#define __SERVOS_H__

/**
 * @brief defines which system is being used currently
 */
typedef enum arm_motor_e
{
    CTRL_MOTOR,     //!< control the motor
    CTRL_ARM,       //!< control the arm
} arm_motor_e;


/**
 * @brief what system is currently being controlled.
 * @note internal to this file. Use associated functions
 * to change this instead
 */
static arm_motor_e CURR_SYSTEM = CTRL_MOTOR;


/**
 * @brief control the motor
 */
void ctrl_use_motor(void)
{
    CURR_SYSTEM = CTRL_MOTOR;
}


/**
 * @brief control the arm
 */
void ctrl_use_arm(void)
{
    CURR_SYSTEM = CTRL_ARM;
}

/**
 * @brief return which system is currently being used
 */
arm_motor_e ctrl_using_which(void)
{
    return CURR_SYSTEM;
}

/**
 * @brief control structure used in boolean functions
 */
typedef struct motorctrl_t
{
    int motor_d;
    int motor_e;
} motorctrl_t;

void motorctrl_forward(int *currval);
void motorctrl_backward(int *currval);
int motorctrl_bound_value(int motor_val);
int motorctrl_motor_d(const motorctrl_t& m);
int motorctrl_motor_e(const motorctrl_t& m);

/**
 * @brief return a motorctrl_t structure with motor magnitude specs
 * @param str char array returned from a bluetooth read
 * @returns Returns a structure for setting motor values
 */
void motorctrl_create(ubyte *str, motorctrl_t *m_ptr)
{
    motorctrl_t m;
    m.motor_d = 0;
    m.motor_e = 0;

    while (*str != '\0')
    {
        const ubyte motor_spec = *str;
        if (ctrl_using_which() == CTRL_MOTOR)
        {
            // motor commands
            if ('f' == motor_spec)
            {
                motorctrl_forward(&m.motor_d);
                motorctrl_forward(&m.motor_e);
            }
            else if ('b' == motor_spec)
            {
                motorctrl_backward(&m.motor_d);
                motorctrl_backward(&m.motor_e);
            }
            else if ('l' == motor_spec)
            {
                //m.motor_d += 0;
                motorctrl_forward(&m.motor_e);
            }
            else if ('r' == motor_spec)
            {
                motorctrl_forward(&m.motor_d);
                //m.motor_e += 0;
            }
        }
        else if (ctrl_using_which() == CTRL_ARM)
        {
            // arm commands
        }
        str++;
    }
    m.motor_d = motorctrl_bound_value(m.motor_d);
    m.motor_e = motorctrl_bound_value(m.motor_e);
    *m_ptr = m;
}

/**
 * @brief increment motor value. Doesn't apply bounding.
 * @param currval current motor value
 */
void motorctrl_forward(int *currval)
{
    *currval += 50;
}

/**
 * @brief decrement motor value. Doesn't apply bounding.
 * @param currval current motor value
 */
void motorctrl_backward(int *currval)
{
    *currval -= 50;
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
