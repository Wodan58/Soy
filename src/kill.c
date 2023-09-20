/*
    module  : kill.c
    version : 1.3
    date    : 09/15/23
*/
#ifndef KILL_C
#define KILL_C

/**
OK 3270  kill  :  N	->
[MT] Make all threads eligible for garbage collection.
*/
void kill_(pEnv env)
{
#ifdef MULTI_TASK_THREAD_JOY
    kill_thread(env);			/* exit a thread */
#endif
}
#endif
