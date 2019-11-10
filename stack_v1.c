/*******************************************************************************
 * File name:   stack_v1.c
 * Developer:   Lior Katz
 * Description: stack source file
 ******************************************************************************/

#include <stdio.h>	   /* printf */
#include <stdlib.h>     /* malloc, free */
#include <assert.h>	   /* assert */
#include <string.h>	   /* memcpy */

#include "stack_v1.h"

#define FAILURE (-1)

/******************************* global definition ****************************/
struct stack
{
	void *base;
	void *current;
	void *top;
	size_t element_size;
};

/********************************* StackCreate *******************************/
stack_t *StackCreate(size_t capacity, size_t element_size)
{
	 /* one more byte to top pointer to point allocated memory */
	stack_t *ptr_new_stack = (stack_t *)malloc(sizeof(stack_t) +
						 (capacity * element_size) + 1);
	if (NULL == ptr_new_stack)
	{
		return (NULL);
	}

	ptr_new_stack->element_size = element_size;
	ptr_new_stack->base = ptr_new_stack + 1;
	ptr_new_stack->current = ptr_new_stack + 1;
	ptr_new_stack->top = (void *)((size_t)ptr_new_stack->base
	                                        + (capacity * element_size));
/* can also be :
 *ptr_new_stack->top = (char*)ptr_new_stack->base + (capacity * element_size);*/
	return (ptr_new_stack);
}

/********************************* StackDestroy ******************************/
void StackDestroy(stack_t *stack)
{
	assert(stack);

	free(stack);
	stack = NULL;
}

/********************************* StackPush *********************************/
int StackPush(stack_t *stack, const void *element)
{
	assert(stack);
	assert(element);

	 /* if stack is full return (-1); */
	if ( stack->current == stack->top)
	{
		return (FAILURE);
	}

	memcpy(stack->current, element, stack->element_size);

	/* move current pointer to next location */
	stack->current = (void *)((size_t)stack->current + stack->element_size);


	return (0);
}

/********************************* StackPop **********************************/
int StackPop(stack_t *stack)
{
	assert(stack);

	/* if empty -> return (-1) */
	if ( stack->current == stack->base )
	{
		printf("stack is empty!\n");

		return (FAILURE);
	}

	/* Update current */
	stack->current = (void *)((size_t)stack->current - stack->element_size);

	return (0);
}

/********************************* StackPeek *********************************/
void *StackPeek(const stack_t *stack)
{
	assert(stack);

	/* if empty -> return NULL */
	if ( stack->current == stack->base )
	{
		return (NULL);
	}

	/* current element */
	return ((void *)((size_t)stack->current - stack->element_size));
}

/********************************* StackSize *********************************/
size_t StackSize(const stack_t *stack)
{
	assert(stack);

	return (((size_t)stack->current - (size_t)stack->base)
	                                    / stack->element_size);
}
