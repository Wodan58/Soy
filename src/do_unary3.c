/*
    module  : do_unary3.c
    version : 1.2
    date    : 10/26/20
*/
#include <stdio.h>
#include <assert.h>
#include "data.h"
#include "code.h"
#include "ident.h"

/*
unary3  :  X1 X2 X3 [P]  ->  R1 R2 R3
Executes P three times, with Xi, returns Ri (i = 1..3).
*/
void do_unary3()
{
    data_t *cur;

    DEBUG(__FUNCTION__);
    assert(stack && stack->next && stack->next->next &&
	   stack->next->next->next && stack->op == typ_list);
    cur = get(1);
    cur->op = typ_list;
    cur->list = stack;
    cur->next = dump;
    dump = cur;
    stack = stack->next->next->next;
    exeterm(dump->list->list);
    cur = get(1);
    cur->op = stack->op;
    cur->val = stack->val;
    cur->next = dump1;
    dump1 = cur;
    cur = get(1);
    cur->op = dump->list->next->next->op;
    cur->val = dump->list->next->next->val;
    cur->next = dump->list->next->next->next->next;
    stack = cur;
    exeterm(dump->list->list);
    cur = get(1);
    cur->op = stack->op;
    cur->val = stack->val;
    cur->next = dump1;
    dump1 = cur;
    cur = get(1);
    cur->op = dump->list->next->op;
    cur->val = dump->list->next->val;
    cur->next = dump->list->next->next->next->next;
    stack = cur;
    exeterm(dump->list->list);
    stack->next = dump1;
    dump1 = dump1->next->next;
    stack->next->next->next = dump->list->next->next->next->next;
    dump = dump->next;
}
