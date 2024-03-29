/*
    module  : ytreegenrec.c
    version : 1.9
    date    : 11/06/23
*/
#ifndef YTREEGENREC_C
#define YTREEGENREC_C

/**
OK 3150  (treegenrec)  :  DDDDDA	T [O1] [O2] [C]  ->  ...
T is a tree. If T is a leaf, executes O1.
Else executes O2 and then [[[O1] [O2] C] treegenrec] C.
*/
void ytreegenrec_(pEnv env)
{
    unsigned size;
    Node list, node;

    PARM(1, DIP);
    env->stck = pvec_pop(env->stck, &list);	/* item on top of the stack */
    node = pvec_lst(env->stck);			/* 2nd item on the stack */
    if (node.op == LIST_) {			/* list = [[O1] [O2] C] */
	prog(env, list.u.lis);			/* C */
	(void)pop(env);
	(void)pop(env);

	code(env, cons_);
	code(env, cons_);

	node.u.lis = 0;
	prime(env, node);

	node.u.proc = ytreegenrec_;
	node.op = ANON_PRIME_;
	prime(env, node);

	env->prog = pvec_add(env->prog, list);	/* list = [[O1] [O2] C] */

	size = pvec_cnt(list.u.lis);		/* list = [[O1] [O2] C] */
	list = pvec_nth(list.u.lis, size - 2);	/* list = [O2] */
	prog(env, list.u.lis);
    } else {					/* list = [[O1] [O2] C] */
	list = pvec_lst(list.u.lis);		/* list = [O1] */
	prog(env, list.u.lis);
    }
}
#endif
