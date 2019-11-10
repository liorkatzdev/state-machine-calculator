/*****************************************************************************
 * File name  : calc.h
 * Developer  : Lior Katz
 * Date       : 2019-03-19
 * Description: VERSION 1.0
 *****************************************************************************/
#include <stdlib.h>       /* malloc */
#include <assert.h>	      /* assert */
#include <string.h>       /* strtod */
#include <ctype.h>        /* isspace, isdigit */

#include "calc.h"
#include "stack_v1.h"

/*********************************** MACROS ***********************************/

#define UNUSED(x) (void)(x)
#define LUT_SIZE 6
#define ERROR_VALUE 0
/*********************************** enums ************************************/

typedef enum states {
    WAIT_FOR_NUMBER_STATE,
    WAIT_FOR_OPERATOR_STATE,
    END_STATE,
    ERROR_STATE,

    MAX_STATES
} state;

typedef enum events {
    NUMBER_EVENT,
    PLUS_EVENT,
    MINUS_EVENT,
    MULTIPLE_EVENT,
    DIV_EVENT,
    OPEN_BRACE_EVENT,
    CLOSE_BRACE_EVENT,
    END_EVENT,         /* in case of '\0' */
    OTHER_EVENT,       /* in case of char that is not one of the events above */

    MAX_EVENTS

} event;

/*********************************** typedef **********************************/

/*  function_event type, receives:
 *  numbers_stack - stack for numbers,
 *  operators_stack - stack for operators,
 *  result - to hold result.
 */
typedef state(*function_event)(stack_t *numbers_stack,
                                    stack_t *operators_stack, result_t *result);

/*********************************** Inner Functions **************************/

/* Initialize event handler array */
static void InitializeEventHandler(void);

/* Gets an event from string */
static event GetEvent(char **str, double *num);

/* Analyze the result of the Calc function */
static void CalcAnalyzer(stack_t *numbers_stack, stack_t *operators_stack,
                          result_t *calc_result, char **stack_operator
                                                    , state *new_state);

/* Analyze the result of the Calc function */
static result_t Calc(stack_t *numbers_stack, char opeator);

/* pushs number into numbers_stack */
static state GotNumber(stack_t *numbers_stack,
                                    stack_t *operators_stack, result_t *result);

/* handles the operators +, -, *, / and higher precedence operator */
static state OperationHandle(stack_t *numbers_stack,
                                    stack_t *operators_stack, result_t *result);

/* handles close brace event  */
static state CloseBrace(stack_t *numbers_stack,
                                    stack_t *operators_stack, result_t *result);

/* handles '/0' event  */
static state End(stack_t *numbers_stack,
                                    stack_t *operators_stack, result_t *result);

/* handles error event  */
static state Error(stack_t *numbers_stack,
                                    stack_t *operators_stack, result_t *result);

/* checks whether new_operator has higher precidence then stack_opeator */
static int IsNewOperatorHasHigherPrecedence(char stack_opeator,
                                                             char new_operator);

/*********************************** global variables *************************/

function_event event_handler[MAX_STATES][MAX_EVENTS] = {{NULL}};

/*********************************** Calculator *******************************/
result_t Calculator(char *str)
{
    size_t length = 0;
    stack_t *numbers_stack = NULL;
    stack_t *operators_stack = NULL;
    result_t result = {ERROR_VALUE, APPLICATION_ERROR};
    state state = WAIT_FOR_NUMBER_STATE;
    event event = NUMBER_EVENT;

    assert(str);

    InitializeEventHandler();

    length = strlen(str);

    numbers_stack = StackCreate(length, sizeof(double));
    if(NULL != numbers_stack)
    {
        operators_stack = StackCreate(length, sizeof(char));
        if(NULL != operators_stack)
        {
            result.status = CALC_SUCCESS;
            while (state != END_STATE && state != ERROR_STATE)
            {
                event = GetEvent(&str, &result.result);

                state = event_handler[state][event]
                                      (numbers_stack, operators_stack, &result);
            }

            StackDestroy(operators_stack);
            operators_stack = NULL;
        }

        StackDestroy(numbers_stack);
        numbers_stack = NULL;
    }

    return (result);
}

/*********************************** InitializeEventHandler *******************/
static void InitializeEventHandler(void)
{
    event_handler[WAIT_FOR_NUMBER_STATE]       [NUMBER_EVENT] = GotNumber;
    event_handler[WAIT_FOR_NUMBER_STATE]         [PLUS_EVENT] = Error;
    event_handler[WAIT_FOR_NUMBER_STATE]        [MINUS_EVENT] = Error;
    event_handler[WAIT_FOR_NUMBER_STATE]     [MULTIPLE_EVENT] = Error;
    event_handler[WAIT_FOR_NUMBER_STATE]          [DIV_EVENT] = Error;
    event_handler[WAIT_FOR_NUMBER_STATE]   [OPEN_BRACE_EVENT] = OperationHandle;
    event_handler[WAIT_FOR_NUMBER_STATE]  [CLOSE_BRACE_EVENT] = Error;
    event_handler[WAIT_FOR_NUMBER_STATE]          [END_EVENT] = Error;
    event_handler[WAIT_FOR_NUMBER_STATE]        [OTHER_EVENT] = Error;

    event_handler[WAIT_FOR_OPERATOR_STATE]     [NUMBER_EVENT] = Error;
    event_handler[WAIT_FOR_OPERATOR_STATE]       [PLUS_EVENT] = OperationHandle;
    event_handler[WAIT_FOR_OPERATOR_STATE]      [MINUS_EVENT] = OperationHandle;
    event_handler[WAIT_FOR_OPERATOR_STATE]   [MULTIPLE_EVENT] = OperationHandle;
    event_handler[WAIT_FOR_OPERATOR_STATE]        [DIV_EVENT] = OperationHandle;
    event_handler[WAIT_FOR_OPERATOR_STATE] [OPEN_BRACE_EVENT] = Error;
    event_handler[WAIT_FOR_OPERATOR_STATE][CLOSE_BRACE_EVENT] = CloseBrace;
    event_handler[WAIT_FOR_OPERATOR_STATE]        [END_EVENT] = End;
    event_handler[WAIT_FOR_OPERATOR_STATE]      [OTHER_EVENT] = Error;
}

/*********************************** GetEvent *********************************/
static event GetEvent(char **str, double *num)
{
    char *result_str = NULL;
    event returned_event = NUMBER_EVENT;

    assert(str);

    /* skip to next non space charactetrs */
    while (isspace(**str))
    {
        ++(*str);
    }

    if (!isdigit(**str))
    {
        /* extract operator from string and update the event*/
        switch (**str)
        {
        case ('+'):
            returned_event = PLUS_EVENT;
            ++(*str);
            break;

        case ('-'):
            returned_event = MINUS_EVENT;
            ++(*str);
            break;

        case ('*'):
            returned_event = MULTIPLE_EVENT;
            ++(*str);
            break;

        case ('/'):
            returned_event = DIV_EVENT;
            ++(*str);
            break;

        case ('('):
            returned_event = OPEN_BRACE_EVENT;
            ++(*str);
            break;

        case (')'):
            returned_event = CLOSE_BRACE_EVENT;
            ++(*str);
            break;

        case ('\0'):
            returned_event = END_EVENT;
            ++(*str);
            break;

        default:
            returned_event = OTHER_EVENT;
        }

        *num = returned_event;
    }
    else
    {
        /* extract number from string and update the event */
        *num = strtod(*str, &result_str);
        *str = result_str;
        returned_event = NUMBER_EVENT;
    }

    return (returned_event);
}

/*********************************** GotNumber ********************************/
static state GotNumber(stack_t *numbers_stack,
                                     stack_t *operators_stack, result_t *result)
{
    assert(numbers_stack);
    assert(result);

    UNUSED(operators_stack);

    StackPush(numbers_stack, &result->result);

    return (WAIT_FOR_OPERATOR_STATE);
}

/*********************************** OperationHandle **************************/
static state OperationHandle(stack_t *numbers_stack,
                                     stack_t *operators_stack, result_t *result)
{
    char new_operator = ' ';
    char LUT[LUT_SIZE] = {' ','+', '-', '*', '/', '('};
    char *operator = NULL;
    result_t calc_result = {0, CALC_SUCCESS};
    state new_state = WAIT_FOR_NUMBER_STATE;

    assert(numbers_stack);
    assert(operators_stack);
    assert(result);

    new_operator = LUT[(int)result->result];

    operator = StackPeek(operators_stack);

    /* performs and analyze calculation according to math precedence */
    while(NULL != operator &&
            (!IsNewOperatorHasHigherPrecedence(*operator, new_operator)))
    {
        CalcAnalyzer(numbers_stack, operators_stack,
                                     &calc_result , &operator, &new_state);
    }

    StackPush(operators_stack, &new_operator);

    return (new_state);
}

/*********************************** CloseBrace *******************************/
static state CloseBrace(stack_t *numbers_stack,
                                 stack_t *operators_stack, result_t *result)
{
    char *operator = NULL;
    result_t calc_result = {0, CALC_SUCCESS};
    state new_state = WAIT_FOR_OPERATOR_STATE;

    assert(numbers_stack);
    assert(operators_stack);
    assert(result);

    operator = StackPeek(operators_stack);

    /* caclculate until operator stack is empty of found opening brace */
    while (0 != StackSize(operators_stack) &&
                        MATH_ERROR != calc_result.status && '(' != *operator)
    {
        CalcAnalyzer(numbers_stack, operators_stack,
                                     &calc_result , &operator, &new_state);
    }

    /* in case opening brace found */
    if(0 != StackSize(operators_stack))
    {
        StackPop(operators_stack);
        if(MATH_ERROR == calc_result.status)
        {
            result->status = MATH_ERROR;
            new_state = ERROR_STATE;
            result->result = ERROR_VALUE;
        }
        else
        {
            result->result = *(double *)StackPeek(numbers_stack);
        }
    }
    else
    {
        result->status = SYNTAX_ERROR;
        new_state = ERROR_STATE;
        result->result = ERROR_VALUE;
    }

    return (new_state);
}

/*********************************** End **************************************/
static state End(stack_t *numbers_stack,
                                     stack_t *operators_stack, result_t *result)
{
    char *operator = NULL;
    result_t calc_result = {0, CALC_SUCCESS};
    state new_state = END_STATE;

    assert(numbers_stack);
    assert(operators_stack);
    assert(result);

    /* operator in stack */
    operator = StackPeek(operators_stack);

    /* Caclulate all that remains */
    while (0 != StackSize(operators_stack)  &&
           MATH_ERROR != calc_result.status && '(' != *operator)
    {
        CalcAnalyzer(numbers_stack, operators_stack,
                                     &calc_result , &operator, &new_state);
    }

    /* single closing brace left after all caclulations = syntax error*/
    if(0 != StackSize(operators_stack))
    {
        new_state = ERROR_STATE;

        result->result = ERROR_VALUE;
        result->status = SYNTAX_ERROR;
    }
    else if(CALC_SUCCESS == result->status)
    {
        result->result = *(double *)StackPeek(numbers_stack);
    }

    return (new_state);
}

/*********************************** Error ************************************/
static state Error(stack_t *numbers_stack,
                                     stack_t *operators_stack, result_t *result)
{
    assert(result);
    UNUSED(numbers_stack);
    UNUSED(operators_stack);

    result->result = ERROR_VALUE;
    result->status = SYNTAX_ERROR;

    return (ERROR_STATE);
}

/*********************************** CalcAnalyzer *****************************/
static void CalcAnalyzer(stack_t *numbers_stack, stack_t *operators_stack,
                          result_t *calc_result, char **stack_operator
                                                    , state *new_state)
{
    assert(numbers_stack);
    assert(operators_stack);
    assert(calc_result);
    assert(stack_operator);
    assert(new_state);

    StackPop(operators_stack);

    *calc_result = Calc(numbers_stack, **stack_operator);

    /* check for math error */
    if(MATH_ERROR != calc_result->status)
    {
        StackPush(numbers_stack, &(calc_result->result));
    }
    else
    {
        /* if error - update new_state and result to math error event */
        *new_state = ERROR_STATE;
    }

    *stack_operator = StackPeek(operators_stack);
}
/*********************************** Calc *************************************/
static result_t Calc(stack_t *numbers_stack, char opeator)
{
    double *first = NULL;
    double *second = NULL;
    result_t calc_result = {0,CALC_SUCCESS};
    assert(numbers_stack);

    second = StackPeek(numbers_stack);
    StackPop(numbers_stack);

    first = StackPeek(numbers_stack);
    StackPop(numbers_stack);

    switch (opeator)
    {
        case ('+'):
            calc_result.result = *first + *second;
            break;

        case ('-'):
            calc_result.result = *first - *second;
            break;

        case ('*'):
            calc_result.result = (*first) * (*second);
            break;

        case ('/'):
            if(0 != *second)
            {
                calc_result.result = (*first) / (*second);
            }
            else
            {
                calc_result.status = MATH_ERROR;
            }
            break;

        default:
            ;
    }

    return(calc_result);
}

/****************** IsStackOperatorHasHigherPrecidence ************************/
static int IsNewOperatorHasHigherPrecedence(char stack_opeator,
                                                              char new_operator)
{
    int result = 0;

    if((('+' == stack_opeator) || ('-' == stack_opeator)) &&
                               (('*' == new_operator) || ('/' == new_operator)))
    {
        result = 1;
    }
    else if('(' == stack_opeator || '(' == new_operator)
    {
            result = 1;
    }

    return (result);
}
