/*******************************************************************************
*	Filename    :	bst_test.c
*	Developer	:	Lior Katz
*	Last Update :	2019-03-13
*	Description :	bst test file.
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "calc.h"

/*********************************** Inner Functions **************************/

void TestAddition(void);
void TestSubtraction(void);
void TestDivide(void);
void TestMultiple(void);
void TestBraces(void);
void TestSyntaxError(void);

void TestValidInput(void);
void TestInvalidInput(void);
void TestDivisionByZero(void);


/********************************** Main **************************************/

int main (void)
{
    TestAddition();
    TestSubtraction();
    TestDivide();
    TestMultiple();
    TestBraces();
    TestSyntaxError();
    TestValidInput();
    TestInvalidInput();
    TestDivisionByZero();

    return (0);
}

/*********************************** TestAddition *****************************/
void TestAddition(void)
{
    result_t result1 = Calculator(" 3 + 10 + 5 + 2");
    result_t result2 = Calculator(" 3 + 5 + 10 + 2");
    result_t result3 = Calculator(" 3 + 2 + 5 + 10");
    result_t result4 = Calculator(" 10 + 3 + 5 + 2");
    result_t result5 = Calculator(" 3  60 + 5 + 2");

    printf("Test Addition-> ");
    if(20 == result1.result && 20 == result2.result &&
        20 == result3.result && 20 == result4.result &&
        SYNTAX_ERROR == result5.status)
        {
            printf("SUCCESS\n");
        }
        else
        {
            printf("FAIL\n");
        }
    }

    /*********************************** TestSubtraction **************************/
    void TestSubtraction(void)
    {
        result_t result1 = Calculator(" 20 - 10 - 5 - 2");
        result_t result2 = Calculator(" 3 + 5 + 10 - 2");
        result_t result3 = Calculator(" 3 + 2 - 5 - 10");
        result_t result4 = Calculator(" 10 + 3 - 5 + 2");
        result_t result5 = Calculator(" 5 - 60 - 5 ");

        printf("Test Subtraction-> ");
        if(3 == result1.result && 16 == result2.result &&
            (-10) == result3.result && 10 == result4.result &&
            (-60) == result5.result)
            {
                printf("SUCCESS\n");
            }
            else
            {
                printf("FAIL\n");
            }
        }

        /*********************************** TestDivide *******************************/
        void TestDivide(void)
        {
            result_t result1 = Calculator(" 20 + 100 / 10 ");
            result_t result2 = Calculator(" 10 + 20 - 20 / 5");
            result_t result3 = Calculator(" 3 + 2 + 5 + 10 / 10");
            result_t result4 = Calculator(" 60 / 3 + 30 / 6");
            result_t result5 = Calculator(" 60 / 5 ");

            printf("Test Divide-> ");
            if(30 == result1.result && 26 == result2.result &&
                11 == result3.result && 25 == result4.result &&
                12 == result5.result)
                {
                    printf("SUCCESS\n");
                }
                else
                {
                    printf("FAIL\n");
                }
            }

            /*********************************** TestMultiple *****************************/
            void TestMultiple(void)
            {
                result_t result1 = Calculator(" 20 * 100 / 10 ");
                result_t result2 = Calculator(" 10 + 20 * 20 / 5");
                result_t result3 = Calculator(" 3 + 2 * 5 + 10 / 10");
                result_t result4 = Calculator(" 60 * 3 + 30 / 6");
                result_t result5 = Calculator(" 60 * 5 ");

                printf("Test Multiple-> ");
                if(200 == result1.result && 90 == result2.result &&
                    14 == result3.result && 185 == result4.result &&
                    300 == result5.result)
                    {
                        printf("SUCCESS\n");
                    }
                    else
                    {
                        printf("FAIL\n");
                    }
                }

                /*********************************** TestBraces *******************************/
                void TestBraces(void)
                {
                    result_t result1 = Calculator(" (20 + 100) / 10 ");
                    result_t result2 = Calculator(" 10 + 20 * 60 / (50 + 10)");
                    result_t result3 = Calculator(" ((3 + 2) * 5 + 10) / 10");
                    result_t result4 = Calculator(" 60 * (3 + 30) / 6");
                    result_t result5 = Calculator(" 60 + 5 +60 / (10 - 20)");

                    printf("Test Multiple-> ");
                    if(12 == result1.result && 30 == result2.result &&
                        3.5 == result3.result && 330 == result4.result &&
                        59 == result5.result)
                        {
                            printf("SUCCESS\n");
                        }
                        else
                        {
                            printf("FAIL\n");
                        }
                    }

                    /*********************************** TestSyntaxError **************************/
                    void TestSyntaxError(void)
                    {
                        result_t result1 = Calculator(" (20 + a100) / 10 ");
                        result_t result2 = Calculator(" 10 + 20 * 20 / ((50 + 10)");
                        result_t result3 = Calculator(" ((3 + 2) * 5 + 10)) / 10)");
                        result_t result4 = Calculator(" 60 ** (3 + 30) / 6");
                        result_t result5 = Calculator(" 60 + 5 +60 / (10 10 - 20)");

                        printf("Test Syntax Error-> ");
                        if(SYNTAX_ERROR == result1.status && SYNTAX_ERROR == result2.status &&
                            SYNTAX_ERROR == result3.status && SYNTAX_ERROR == result4.status &&
                            SYNTAX_ERROR == result5.status)
                            {
                                printf("SUCCESS\n");
                            }
                            else
                            {
                                printf("FAIL\n");
                            }
                        }
/*********************************** TestValidInput ***************************/
void TestValidInput(void)
{
    result_t result = Calculator(" 3 + 4 - 5/6*33+999 +3 + 4 - 5/6*33+999");
    if (result.status == CALC_SUCCESS)
    {
        printf ("Result is %f\n", result.result);
    }
    else if (result.status == MATH_ERROR)
    {
        printf ("MATH ERROR\n");
    }
    else if (result.status == APPLICATION_ERROR)
    {
        printf ("APPLICATION ERROR\n");
    }
    else if (result.status == SYNTAX_ERROR)
    {
        printf ("SYNTAX ERROR\n");
    }
}

/*********************************** TestInvalidInput *************************/
void TestInvalidInput(void)
{
    result_t result = Calculator(" 3 + s)4 - 5/d6*33+999 +3 + 4f 9");
    if (result.status == CALC_SUCCESS)
    {
        printf ("Result is %f\n", result.result);
    }
    else if (result.status == MATH_ERROR)
    {
        printf ("MATH ERROR\n");
    }
    else if (result.status == APPLICATION_ERROR)
    {
        printf ("APPLICATION ERROR\n");
    }
    else if (result.status == SYNTAX_ERROR)
    {
        printf ("SYNTAX ERROR\n");
    }
}

/*********************************** TestDivisionByZero ***********************/
void TestDivisionByZero(void)
{
    result_t result = Calculator(" 3 + 4 - 5/6*33+999 +3 + 4 - 5/0*33+999");
    if (result.status == CALC_SUCCESS)
    {
        printf ("Result is %f\n", result.result);
    }
    else if (result.status == MATH_ERROR)
    {
        printf ("MATH ERROR\n");
    }
    else if (result.status == APPLICATION_ERROR)
    {
        printf ("APPLICATION ERROR\n");
    }
    else if (result.status == SYNTAX_ERROR)
    {
        printf ("SYNTAX ERROR\n");
    }
}
