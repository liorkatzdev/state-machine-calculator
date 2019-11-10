/*******************************************************************************
 * File name:   stack_v1.h
 * Developer:   Lior Katz
 * Description: stack head file
 ******************************************************************************/

#ifndef _STACK_V1_H_
#define _STACK_V1_H_

#include <stddef.h> /* size_t */ 

/* This is the stack variable. Stack contains elements which are the size
 * defined by the user in the CreateStack functions.
 */
typedef struct stack stack_t;

/*  Stack Create function creates a data structure of type stack.
 *  in case of success the create function returns a pointer to the address of   
 *  struct base.
 *  in case of failure NULL will be returned.
 *  The function receives two arguments: capacity (total number of elements),
 *  and element_size - which is the size of each element.
 */
stack_t *StackCreate(size_t capacity, size_t element_size);

/*  StackDestroy function will release the memory pointed to by stack.       
 */
void StackDestroy(stack_t *stack);

/*  StackPush function inserts element into the top of stack. 
 *  the function will not alter the value of element.    
 *  the function returns 0 in case of success and -1 in case of failure.
 */
int StackPush(stack_t *stack, const void *element);

/*  StackPop removes the element at the top of the stack. 
 *  the function returns 0 in case of success and -1 in case of failure.     
 */
int StackPop(stack_t *stack);

/*  StackPeek function return the element in the top of stack. 
 *  in case of failure NULL will be returned.
 *  The user must create his own copy on the returend element, since one the
 *  element will be popped in will no longer be available. 
 *  In case stack is empty - returns 1.
 *  if pop succeeds - return 0; */
void *StackPeek(const stack_t *stack);

/*  StackSize function returns the number of elements currently exist in stack.    
 */
size_t StackSize(const stack_t *stack);

#endif     /* _STACK_V1_H_ */
