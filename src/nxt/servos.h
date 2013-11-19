/**
 * @file
 * @brief servo/motor information
 */
#ifndef __SERVOS_H__
#define __SERVOS_H__

/**
 * @brief control structure used in boolean functions
 */
typedef ubyte motorctrl_t;

/**
 * @brief return a motorctrl_t for use with comparison functions
 * @param str value returned from a bluetooth read
 * @returns Returns a value that can be used by boolean functions
 */
motorctrl_t get_motorctl(const string& str)
{
    return str[0];
}


/**
 * @brief return true if m specifies 'forward'
 */
bool is_forward(motorctrl_t m)
{
    return m == 'f';
}

/**
 * @brief return true if m specifies 'backward'
 */
bool is_backward(motorctrl_t m)
{
    return m == 'b';
}

/**
 * @brief return true if m specifies 'left'
 */
bool is_left(motorctrl_t m)
{
    return m =='l';
}

/**
 * @brief return true if m specifies 'right'
 */
bool is_right(motorctrl_t m)
{
    return m == 'r';
}

#endif
