/*
    module  : data2.c
    version : 1.2
    date    : 10/26/20
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "data.h"

data_t *stack = 0, *dump = 0, *dump1 = 0;

void reg(__attribute__((unused)) data_t **parm)
{
}

data_t *get(int num)
{
    data_t *cur;

    cur = calloc(num, sizeof(data_t));
    assert(cur);
    return cur;
}

void mrep(long *size, long *prev, long *used)
{
    *size = 0;
    *prev = 0;
    *used = 0;
}
