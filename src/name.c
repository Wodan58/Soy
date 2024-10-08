/*
    module  : name.c
    version : 1.12
    date    : 09/17/24
*/
#ifndef NAME_C
#define NAME_C

/**
Q0  OK  2170  name  :  DA  sym  ->  "sym"
For operators and combinators, the string "sym" is the name of item sym,
for literals sym the result string is its type.
*/
void name_(pEnv env)
{
    Node node;

    PARM(1, ANYTYPE);
    node = vec_pop(env->stck);
    if (node.op == USR_)
	node.u.str = vec_at(env->symtab, node.u.ent).name;
    else if (node.op == ANON_FUNCT_)
	node.u.str = opername(env, node.u.proc);
    else
	node.u.str = showname(node.op);
    node.op = STRING_;
    vec_push(env->stck, node);
}
#endif
