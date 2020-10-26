/*
    module  : do_rolldownd.c
    version : 1.2
    date    : 10/26/20
*/
#include <stdio.h>
#include <assert.h>
#include "data.h"

/*
rolldownd  :  X Y Z W  ->  Y Z X W
As if defined by:   rolldownd  ==  [rolldown] dip
*/
void do_rolldownd()
{
    data_t *cur;

    DEBUG(__FUNCTION__);
    assert(stack && stack->next && stack->next->next &&
	   stack->next->next->next);
    cur = get(4);
    cur->op = stack->op;
    cur->val = stack->val;
    cur->next = &cur[1];
    stack = stack->next;
    cur[2].op = stack->op;
    cur[2].val = stack->val;
    cur[2].next = &cur[3];
    stack = stack->next;
    cur[3].op = stack->op;
    cur[3].val = stack->val;
    stack = stack->next;
    cur[1].op = stack->op;
    cur[1].val = stack->val;
    cur[1].next = &cur[2];
    cur[3].next = stack->next;
    stack = cur;
}
