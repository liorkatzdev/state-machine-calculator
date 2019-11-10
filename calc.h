/*****************************************************************************
 * File name  : calc.h
 * Developer  : Asaf Halbani Batan
 * Date       : 2019-03-19
 * Description: VERSION 1.0
 *****************************************************************************/

#ifndef __CALC_H__
#define __CALC_H__

struct result_s
{
    double result;
    int status;
};

typedef struct result_s result_t;

enum calc_status
{
    APPLICATION_ERROR = -3,
    SYNTAX_ERROR,
    MATH_ERROR,
    CALC_SUCCESS
};

/*********************************** Calculator *******************************/
/*	Description      : Receives a string and calculates result.
 *
 *	Input            : char * str = string. string should contains a valid
 *	                   arithmetic expression.
 *
 *	Return Values    : result_t -
 *	                   If calculation succeeds, member 'result' will
 *	                   contain the result, err_status will return .
 *	                   If fails - result will contain -1 and err_str will
 *	                   one of the following error status:
 *                     application error : (-3 )
 *                     syntax error      : (-2 )
 *                     math error        : (-1 )
 *                     success           : ( 0 )
 *
 *	Time Complexity  : O(n)
 *
 *  Space Complexity : O(n)
 */
result_t Calculator(char *str);

#endif     /* __CALC_H__ */
