/*
    module  : filter.c
    version : 1.12
    date    : 09/17/24
*/
#ifndef FILTER_C
#define FILTER_C

/**
Q1  OK  2830  filter  :  DDA  A [B]  ->  A1
Uses test B to filter aggregate A producing sametype aggregate A1.
*/
void filter_(pEnv env)
{
    int64_t i, j;
    unsigned size;
    Node list, aggr, node;

    PARM(2, STEP);
    list = vec_pop(env->stck);
    aggr = vec_pop(env->stck);
    /*
	register the location of the result aggregate
    */
    size = vec_size(env->prog);
    /*
	build a result aggregate of the correct type
    */
    node.op = aggr.op;
    switch (aggr.op) {
    case LIST_:
	vec_init(node.u.lis);
	prime(env, node);
	for (i = vec_size(aggr.u.lis) - 1; i >= 0; i--) {
	    /*
		push the element that may be added to the result
	    */
	    node = vec_at(aggr.u.lis, i);
	    prime(env, node);
	    /*
		push the location of the result types
	    */
	    push(env, size);
	    /*
		add an instruction that builds the result types
	    */
	    code(env, fpush_);
	    /*
		save and restore the stack, if needed
	    */
	    save(env, list.u.lis, 1, 0);
	    /*
		push the program to be executed for each element
	    */
	    prog(env, list.u.lis);
	    /*
		push the element to be split
	    */
	    prime(env, node);
	}
	break;

    case STRING_:
    case BIGNUM_:
    case USR_STRING_:
	node.u.str = GC_strdup(aggr.u.str);
	node.u.str[0] = 0;
	prime(env, node);
	node.op = CHAR_;
	for (i = strlen(aggr.u.str) - 1; i >= 0; i--) {
	    /*
		push the element that may be added to the result
	    */
	    node.u.num = aggr.u.str[i];
	    prime(env, node);
	    /*
		push the location of the result types
	    */
	    push(env, size);
	    /*
		add an instruction that builds the result types
	    */
	    code(env, fpush_);
	    /*
		save and restore the stack, if needed
	    */
	    save(env, list.u.lis, 1, 0);
	    /*
		push the program to be executed for each element
	    */
	    prog(env, list.u.lis);
	    /*
		push the element to be split
	    */
	    prime(env, node);
	}
	break;

    case SET_:
	node.u.set = 0;
	prime(env, node);
	node.op = INTEGER_;
	for (j = 1, i = 0; i < SETSIZE; i++, j <<= 1)
	    if (aggr.u.set & j) {
		/*
		    push the element that may be added to the result
		*/
		node.u.num = i;
		prime(env, node);
		/*
		    push the location of the result types
		*/
		push(env, size);
		/*
		    add an instruction that builds the result types
		*/
		code(env, fpush_);
		/*
		    save and restore the stack, if needed
		*/
		save(env, list.u.lis, 1, 0);
		/*
		    push the program to be executed for each element
		*/
		prog(env, list.u.lis);
		/*
		    push the element to be split
		*/
		prime(env, node);
	    }
	break;
    }
}
#endif
