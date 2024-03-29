/*
    module  : treegenrec.c
    version : 1.6
    date    : 11/06/23
*/
#ifndef TREEGENREC_C
#define TREEGENREC_C

/**
OK 2890  treegenrec  :  DDDDA	T [O1] [O2] [C]  ->  ...
T is a tree. If T is a leaf, executes O1.
Else executes O2 and then [[[O1] [O2] C] treegenrec] C.
*/
void treegenrec_(pEnv env)
{	/* T [O1] [O2] [C]	*/
    PARM(4, IFTE);
    cons_(env);
    cons_(env);
    ytreegenrec_(env);
}
#endif
