/*
 *  module  : main.c
 *  version : 1.24
 *  date    : 11/06/23
 */
#include "globals.h"

#define ERROR_ON_USRLIB 0		/* no error */

extern FILE *yyin;			/* lexr.c */
extern unsigned char mustinclude;	/* scan.c */

static jmp_buf begin;			/* restart with empty program */

char *bottom_of_stack;			/* used in gc.c */

/*
    abort execution and restart reading from yyin. In the NOBDW version the
    stack is cleared as well.
*/
PUBLIC void abortexecution_(int num)
{
    longjmp(begin, num);
}

/*
    report_clock - report the amount of time spent.
*/
#ifdef STATS
PRIVATE void report_clock(pEnv env)
{
    fflush(stdout);
    fprintf(stderr, "%ld milliseconds CPU to execute\n",
	   (clock() - env->startclock) * 1000 / CLOCKS_PER_SEC);
}
#endif

/*
 *  copyright - Print all copyright notices, even historical ones.
 *
 *  The version must be set on the commandline when compiling:
 *  -DVERS="\"alpha\"" or whatever.
 */
#ifdef COPYRIGHT
PRIVATE void copyright(char *file)
{
    int i, j = 0;
    char str[BUFFERMAX];

    static struct {
	char *file;
	time_t stamp;
	char *gc;
    } table[] = {
	{ "tutinp", 994075177, "NOBDW" },
	{ "jp-joytst", 994075177, "NOBDW" },
	{ "laztst", 1005579152, "BDW" },
	{ "symtst", 1012575285, "BDW" },
	{ "plgtst", 1012575285, "BDW" },
	{ "lsptst", 1012575285, "BDW" },
	{ "mtrtst", 1017847160, "BDW" },
	{ "grmtst", 1017847160, "BDW" },
	{ "reptst", 1047653638, "NOBDW" },
	{ "jp-reprodtst", 1047653638, "NOBDW" },
	{ "flatjoy", 1047653638, "NOBDW" },
	{ "modtst", 1047920271, "BDW" },
	{ 0, 1056113062, "NOBDW" } };

    if (strcmp(file, "stdin")) {
	for (i = 0; table[i].file; i++) {
	    if (!strncmp(file, table[i].file, strlen(table[i].file))) {
		strftime(str, sizeof(str), "%H:%M:%S on %b %d %Y",
			 gmtime(&table[i].stamp));
		printf("JOY  -  compiled at %s (%s)\n", str, table[i].gc);
		j = 1;
		break;
	    }
	}
    } else {
	printf("JOY  -  compiled at %s on %s", __TIME__, __DATE__);
#ifdef VERS
	printf(" (%s)", VERS);
#endif
	putchar('\n');
	j = 1;
    }
    if (j)
	printf("Copyright 2001 by Manfred von Thun\n");
}
#endif

/*
    dump the symbol table - accessed from quit_, because env is needed;
    do this only for user defined symbols.
*/
#ifdef SYMBOLS
PRIVATE void dump_table(pEnv env)
{
    int i;
    Entry ent;

    for (i = vec_size(env->symtab) - 1; i >= 0; i--) {
	ent = vec_at(env->symtab, i);
	if (!ent.is_user)
	    printf("(%d) %s\n", i, ent.name);
	else {
	    printf("(%d) %s == ", i, ent.name);
	    writeterm(env, ent.u.body, stdout);
	    putchar('\n');
	}
    }
}
#endif

/*
    options - print help on startup options and exit: options are those that
	      cannot be set from within the language itself.
*/
PRIVATE void options(pEnv env)
{
    char str[BUFFERMAX];

    printf("Usage: joy [options] [filename] [parameters]\n");
    printf("options, filename, parameters can be given in any order\n");
    printf("options start with '-' and are all given together\n");
    printf("parameters start with a digit\n");
    printf("the filename parameter cannot start with '-' or a digit\n");
    printf("Features included (+) or not (-):\n");
    sprintf(str, " symbols  overwrite  copyright  arity  alarm  yydebug");
#ifdef SYMBOLS
    str[0] = '+';
#else
    str[0] = '-';
#endif
#ifdef OVERWRITE
    str[9] = '+';
#else
    str[9] = '-';
#endif
#ifdef COPYRIGHT
    str[20] = '+';
#else
    str[20] = '-';
#endif
#ifdef ARITY
    str[31] = '+';
#else
    str[31] = '-';
#endif
#if ALARM
    str[38] = '+';
#else
    str[38] = '-';
#endif
#if YYDEBUG
    str[45] = '+';
#else
    str[45] = '-';
#endif
    printf("%s\n", str);
    sprintf(str, " jversion  tracing  stats  ncheck  multitask  bignums");
#ifdef VERS
    str[0] = '+';
#else
    str[0] = '-';
#endif
#ifdef TRACING
    str[10] = '+';
#else
    str[10] = '-';
#endif
#ifdef STATS
    str[19] = '+';
#else
    str[19] = '-';
#endif
#ifdef NCHECK
    str[26] = '+';
#else
    str[26] = '-';
#endif
#ifdef USE_MULTI_THREADS_JOY
    str[34] = '+';
#else
    str[34] = '-';
#endif
#ifdef USE_BIGNUM_ARITHMETIC
    str[45] = '+';
#else
    str[45] = '-';
#endif
    printf("%s\n", str);
    sprintf(str, " compiler  tokens  ndebug  debug  nobdw  tracegc");
#ifdef COMPILER
    str[0] = '+';
#else
    str[0] = '-';
#endif
#ifdef TOKENS
    str[10] = '+';
#else
    str[10] = '-';
#endif
#ifdef NDEBUG
    str[18] = '+';
#else
    str[18] = '-';
#endif
#ifdef DEBUG
    str[26] = '+';
#else
    str[26] = '-';
#endif
#ifdef NOBDW
    str[33] = '+';
#else
    str[33] = '-';
#endif
#ifdef ENABLE_TRACEGC
    str[40] = '+';
#else
    str[40] = '-';
#endif
    printf("%s\n", str);
#ifdef COMP
    printf("Compile flags: %s\n", COMP);
#endif
#ifdef LINK
    printf("Linker flags: %s\n", LINK);
#endif
    printf("Options:\n");
    printf("  -h : print this help text and exit\n");
#ifdef COMPILER
    printf("  -c : compile joy source into C source\n");
#endif
#ifdef TRACING
    printf("  -d : print a trace of stack development\n");
#endif
#ifdef SYMBOLS
    printf("  -s : dump symbol table functions after execution\n");
#endif
#ifdef TRACING
    printf("  -t : print a trace of program execution\n");
#endif
#ifdef COPYRIGHT
    printf("  -v : do not print a copyright notice\n");
#endif
#ifdef OVERWRITE
    printf("  -w : suppress warnings: overwriting, arities\n");
#endif
#if YYDEBUG
    printf("  -y : print a trace of parser execution\n");
#endif
    quit_(env);
}

PRIVATE int my_main(int argc, char **argv)
{
    char *ptr;
    int i, j, ch;
    unsigned char helping = 0;
#ifdef COPYRIGHT
    unsigned char verbose = 1;
#endif
#ifdef SYMBOLS
    unsigned char symdump = 0;
#endif

    Env env; /* global variables */
    memset(&env, 0, sizeof(env));
    /*
     *  Start the clock. my_atexit is called from quit_ that is called in
     *  scan.c after reading EOF on the first input file.
     */
    env.startclock = clock();
#ifdef STATS
    my_atexit(report_clock);
#endif
    vec_init(env.tokens);
    vec_init(env.symtab);
#ifdef USE_MULTI_THREADS_JOY
    vec_init(env.context);
    vec_init(env.channel);
#endif
    /*
     *  Initialize yyin and other environmental parameters.
     */
    yyin = stdin;
    env.filename = "stdin";
    env.overwrite = INIWARNING;
    if ((ptr = strrchr(env.pathname = argv[0], '/')) != 0) {
	*ptr++ = 0;
	argv[0] = ptr;
    } else if ((ptr = strrchr(env.pathname, '\\')) != 0) {
	*ptr++ = 0;
	argv[0] = ptr;
    } else
	env.pathname = ".";
    /*
     *  First look for options. They start with -.
     */
    for (i = 1; i < argc; i++)
	if (argv[i][0] == '-') {
	    for (j = 1; argv[i][j]; j++)
		switch (argv[i][j]) {
		case 'h' : helping = 1; break;
#ifdef COMPILER
		case 'c' : env.compiling = 1; break;
#endif
#ifdef TRACING
		case 'd' : env.debugging = 1; break;
#endif
#ifdef SYMBOLS
		case 's' : symdump = 1; break;
#endif
#ifdef TRACING
		case 't' : env.debugging = 2; break;
#endif
#ifdef COPYRIGHT
		case 'v' : verbose = 0; break;
#endif
#ifdef OVERWRITE
		case 'w' : env.overwrite = 0; break;
#endif
#if YYDEBUG
		case 'y' : yydebug = 1; break;
#endif
		}
	    /*
		Overwrite the options with subsequent parameters.
	    */
	    for (--argc; i < argc; i++)
		argv[i] = argv[i + 1];
	    break;
	}
    /*
     *  Look for a possible filename parameter. Filenames cannot start with -
     *  and cannot start with a digit, unless preceded by a path: e.g. './'.
     */
    for (i = 1; i < argc; i++) {
	ch = argv[i][0];
	if (!isdigit(ch)) {
	    if ((yyin = freopen(env.filename = argv[i], "r", stdin)) == 0) {
		fprintf(stderr, "failed to open the file '%s'.\n",
			env.filename);
		return 0;
	    }
	    /*
	     *  Overwrite argv[0] with the filename and shift subsequent
	     *  parameters.
	     */
	    if ((ptr = strrchr(argv[0] = env.filename, '/')) != 0) {
		*ptr++ = 0;
		argv[0] = env.filename = ptr;
	    }
	    for (--argc; i < argc; i++)
		argv[i] = argv[i + 1];
	    break;
	}
    }
    env.g_argc = argc;
    env.g_argv = argv;
#ifdef COPYRIGHT
    if (verbose)
	copyright(env.filename);
#endif
#ifdef SYMBOLS
    if (symdump)
	my_atexit(dump_table);
#endif
    env.echoflag = INIECHOFLAG;
    env.autoput = INIAUTOPUT;
    env.undeferror = INIUNDEFERROR;
    inilinebuffer(env.filename);
    inisymboltable(&env);
    if (helping)
	options(&env);		/* might print symbol table */
#ifdef COMPILER
    if (env.compiling)
	initcompile(&env);	/* uses symtab and filename */
#endif
    env.stck = pvec_init();	/* start with an empty stack */
    setjmp(begin);		/* return here after error or abort */
    env.prog = pvec_init();	/* restart with an empty program */
    if (mustinclude) {
	mustinclude = include(&env, "usrlib.joy", ERROR_ON_USRLIB);
	fflush(stdout);		/* flush include messages */
    }
    return yyparse(&env);
}

int main(int argc, char **argv)
{
    int (*volatile m)(int, char **) = my_main;

    bottom_of_stack = (char *)&argc;
    GC_INIT();
    return (*m)(argc, argv);
}
