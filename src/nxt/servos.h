/**
 * @file
 * @brief servo/motor information
 */
#ifndef __SERVOS_H__
#define __SERVOS_H__

/**
 * @brief choose whether to use fuzzy logic or use hardset values
 */
#define MOTORCTRL_FUZZY

/**
 * @brief defines the largest value a motor can have
 *
 * This is the maximum value that a motor can get,
 * i.e. forward*resolution.
 */
#define MOTORCTRL_MAX_SUM 1024

/**
 * @brief the max percentage that the motor will be set to
 */
#define MOTORCTRL_PERC_MAX 100


/**
 * @brief defines the max number of chars to use for fuzzy logic
 */
#define MOTORCTRL_MAX_RES 8

/**
 * @brief minimum absolute value to send to motors
 */
#define MOTORCTRL_MIN_THESH 20

/**
 * @brief how much weight is given to a single increment
 * (call to forward or backward)
 */
#define MOTORCTRL_FULL_STEP (MOTORCTRL_MAX_SUM / MOTORCTRL_MAX_RES)


/**
 * @brief how much weight is given for a half-increment
 */
#define MOTORCTRL_HALF_STEP (MOTORCTRL_FULL_STEP / 2)



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
    // motors
    int motor_left;     //!< left motor power
    int motor_right;    //!< right motor power
    // servos
    int servo_hand;     //!< hand servo control
    int servo_thumb;    //!< thumb servo control
    int servo_fingers;  //!< finger servo control
    int servo_wrist;    //!< wrist servo control
    int servo_shoulder; //!< shoulder servo control
    int servo_5;        //!< extra servo
    int servo_6;        //!< extra servo
} motorctrl_t;

void motorctrl_forward(int *currval);
void motorctrl_backward(int *currval);
int motorctrl_bound_value(int motor_val);
int motorctrl_motor_left(const motorctrl_t& m);
int motorctrl_motor_right(const motorctrl_t& m);


/**
 * @brief initialize a motorctrl_t structure
 */
void motorctrl_init(motorctrl_t *m_ptr)
{
    memset(m_ptr, 0x0, sizeof(motorctrl_t));
}


/**
 * @brief updates a motorctrl_t structure with new motor magnitude specs
 * @param str char array returned from a bluetooth read
 * @returns
 * Returns 0 on success, positive when stop signal received,
 * negative on error
 */
int motorctrl_update(motorctrl_t *m_ptr, ubyte *str)
{

    if (NULL == m_ptr) ErrorFatal("NULL m_ptr");
    motorctrl_init(m_ptr);

    if (NULL == str)
        return 0;

    while (*str != '\0')
    {
        const ubyte motor_spec = *str;
        if ('t' == motor_spec)
        {
            // stop command
            return 1;
        }
        else if (ctrl_using_which() == CTRL_MOTOR)
        {
            // motor commands
            if ('s' == motor_spec)
            {
                // switch to arm
                LogMsg("--> Arm");
                ctrl_use_arm();
            }
            else if ('f' == motor_spec)
            {
                motorctrl_forward(&m_ptr->motor_left);
                motorctrl_forward(&m_ptr->motor_right);
            }
            else if ('b' == motor_spec)
            {
                motorctrl_backward(&m_ptr->motor_left);
                motorctrl_backward(&m_ptr->motor_right);
            }
            else if ('l' == motor_spec)
            {
                //m_ptr->motor_left += 0;
                motorctrl_forward(&m_ptr->motor_right);
            }
            else if ('r' == motor_spec)
            {
                motorctrl_forward(&m_ptr->motor_left);
                //m_ptr->motor_right += 0;
            }
        }
        else if (ctrl_using_which() == CTRL_ARM)
        {
            // arm commands
            switch (motor_spec)
            {
                case 's':
                    // switch to motor
                    LogMsg("--> Motor");
                    ctrl_use_motor();
                    break;
                case 'a':
                    // move hand
                    m_ptr->servo_hand = 100;
                    break;
                case 'e':
                    // move hand
                    m_ptr->servo_hand = -100;
                    break;
                case 'd':
                    // move wrist
                    m_ptr->servo_wrist = 10;
                    break;
                case 'g':
                    // move wrist
                    m_ptr->servo_wrist = -10;
                    break;
            }
        }
        str++;
    }
    return 0;
}

/**
 * @brief increment motor value. Doesn't apply bounding.
 * @param currval current motor value
 */
void motorctrl_forward(int *currval)
{
    *currval += MOTORCTRL_FULL_STEP;
}

/**
 * @brief decrement motor value. Doesn't apply bounding.
 * @param currval current motor value
 */
void motorctrl_backward(int *currval)
{
    *currval -= MOTORCTRL_FULL_STEP;
}

/**
 * @brief ensure motor_val is between -100 and 100
 */
int motorctrl_bound_value(int motor_val)
{
#ifdef MOTORCTRL_FUZZY
    // take the mean*(max%) of the motor value.
    // This will produce a value between -max% .. max%
    motor_val = (motor_val * MOTORCTRL_PERC_MAX) / MOTORCTRL_MAX_SUM;
#endif
    // final bounding checks
    if (motor_val < -MOTORCTRL_PERC_MAX)
        return -MOTORCTRL_PERC_MAX;
    else if (motor_val > MOTORCTRL_PERC_MAX)
        return MOTORCTRL_PERC_MAX;
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
