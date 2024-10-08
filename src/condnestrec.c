/*
    module  : condnestrec.c
    version : 1.9
    date    : 09/17/24
*/
#ifndef CONDNESTREC_C
#define CONDNESTREC_C

/**
Q1  OK  2750  condnestrec  :  DDA  [ [C1] [C2] .. [D] ]  ->  ...
A generalisation of condlinrec.
Each [Ci] is of the form [[B] [R1] [R2] .. [Rn]] and [D] is of the form
[[R1] [R2] .. [Rn]]. Tries each B, or if all fail, takes the default [D].
For the case taken, executes each [Ri] but recurses between any two
consecutive [Ri] (n > 3 would be exceptional.)
*/
void condnestrec_(pEnv env)
{
    unsigned size, size2;
    int i, j, k, l, limit;
    Node aggr, elem, node;

    PARM(1, CASE);
    aggr = vec_pop(env->stck);
    size2 = vec_size(env->prog);
    for (k = 0, l = vec_size(aggr.u.lis); k < l; k++) {
	/*
	    jump address of the next cond line, or the end of it
	*/
	size = vec_size(env->prog);
	/*
	    push the jump address onto the program stack
	*/
	push(env, size2);
	/*
	    jump on true to the end of the program
	*/
	code(env, jump_);
	/*
	    read a cond line
	*/
	elem = vec_at(aggr.u.lis, k);
	limit = k ? 2 : 1;
	for (i = 0, j = vec_size(elem.u.lis) - limit; i < j; i++) {
	    node = vec_at(elem.u.lis, i);
	    prog(env, node.u.lis);
	    /*
		push the continuation
	    */
	    code(env, condnestrec_);
	    node.u.lis = aggr.u.lis;
	    node.op = LIST_;
	    prime(env, node);
	}
	node = vec_at(elem.u.lis, i);
	prog(env, node.u.lis);
	if (k) {
	    /*
		push the jump address onto the program stack
	    */
	    push(env, size);
	    /*
		jump on false to the next cond line; remove condition code
	    */
	    code(env, pfalse_);
	    /*
		save and restore the stack, if needed
	    */
	    save(env, node.u.lis, 0, 0);
	    /*
		push the condition of the cond line
	    */
	    node = vec_back(elem.u.lis);
	    prog(env, node.u.lis);
	}
    }
}
#endif
