/**
 * @file
 * @brief servo/motor information
 */
#ifndef __SERVOS_H__
#define __SERVOS_H__

/**
 * @brief defines the largest value a motor can have
 *
 * This is the maximum value that a motor can get,
 * i.e. forward*(max resolution).
 */
#define MOTORCTRL_MAX_MAGNITUDE 1024


/**
 * @brief defines the max number of chars to use for fuzzy logic
 *
 *
 */
#define MOTORCTRL_MAX_RESOLUTION 8



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
    int motor_left;
    int motor_right;
} motorctrl_t;

void motorctrl_forward(int *currval);
void motorctrl_backward(int *currval);
int motorctrl_bound_value(int motor_val);
int motorctrl_motor_left(const motorctrl_t& m);
int motorctrl_motor_right(const motorctrl_t& m);



/**
 * @brief updates a motorctrl_t structure with new motor magnitude specs
 * @param str char array returned from a bluetooth read
 * @returns Returns a structure for setting motor values
 */
void motorctrl_update(motorctrl_t *m_ptr, ubyte *str)
{
    motorctrl_t m;
    m.motor_left = 0;
    m.motor_right = 0;

    while (*str != '\0')
    {
        const ubyte motor_spec = *str;
        if (ctrl_using_which() == CTRL_MOTOR)
        {
            // motor commands
            if ('f' == motor_spec)
            {
                motorctrl_forward(&m.motor_left);
                motorctrl_forward(&m.motor_right);
            }
            else if ('b' == motor_spec)
            {
                motorctrl_backward(&m.motor_left);
                motorctrl_backward(&m.motor_right);
            }
            else if ('l' == motor_spec)
            {
                //m.motor_left += 0;
                motorctrl_forward(&m.motor_right);
            }
            else if ('r' == motor_spec)
            {
                motorctrl_forward(&m.motor_left);
                //m.motor_right += 0;
            }
        }
        else if (ctrl_using_which() == CTRL_ARM)
        {
            // arm commands
        }
        str++;
    }
    m_ptr->motor_left = motorctrl_bound_value(m.motor_left);
    m_ptr->motor_right = motorctrl_bound_value(m.motor_right);
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
int motorctrl_motor_left(const motorctrl_t& m)
{
    return m.motor_left;
}

/**
 * @brief return the motor value for motorE
 */
int motorctrl_motor_right(const motorctrl_t& m)
{
    return m.motor_right;
}

#endif
