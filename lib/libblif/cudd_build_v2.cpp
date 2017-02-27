/**CFile***********************************************************************

  FileName    [cudd_build_v2.c]

  SeeAlso     []

  Author      [Fabio Somenzi]

  Copyright   [Copyright (c) 1995-2004, Regents of the University of Colorado

  All rights reserved.

******************************************************************************/

#include "ntr.h"
#include "../lib/cudd-2.5.0/cudd/cuddInt.h"

/*---------------------------------------------------------------------------*/
/* Constant declarations                                                     */
/*---------------------------------------------------------------------------*/

#define BUFLENGTH 8192
static  char    buffer[BUFLENGTH];
#ifdef DD_DEBUG
extern  st_table *checkMinterms (BnetNetwork *net, DdManager *dd, st_table *previous);
#endif

/*---------------------------------------------------------------------------*/
/* Static function prototypes                                                */
/*---------------------------------------------------------------------------*/

static NtrOptions * mainInit ();
static void ntrReadOptions (const char *filename, NtrOptions * option);
static FILE * open_file (const char *filename);
static int reorder (BnetNetwork *net, DdManager *dd, NtrOptions *option);
static void freeOption (NtrOptions *option);
//static DdManager * startCudd (FILE*fp, NtrOptions *option, int nvars);
static DdManager *startCudd(NtrOptions *option, int nvars);
static int ntrReadTree (DdManager *dd, char *treefile, int nvars);

/**Function********************************************************************

  Synopsis    [Main program for ntr.]

  Description [Main program for ntr. Performs initialization. Reads command
  line options and network(s). Builds BDDs with reordering, and optionally
  does reachability analysis. Prints stats.]

  SideEffects [None]

  SeeAlso     []

******************************************************************************/
void cudd_build_v2(BnetNetwork *net, DdManager **dd, const char *filename, int params)
{
    NtrOptions	*option;	/* options */
    FILE	*fp;		/* input network file pointer */
    int i;
    int		exitval;	/* return value of Cudd_CheckZeroRef */
    int		ok;		/* overall return value from main() */
    int		result;		/* stores the return value of functions */
    BnetNode	*node;		/* auxiliary pointer to network node */
    
    /* Initialize. */
    option = mainInit();
    option->locGlob        = params;
    ntrReadOptions(filename, option);
    
    /* First assign variables to inputs if the order is provided.
	** (Either in the .blif file or in an order file.)
	*/
	if (option->ordering == PI_PS_FROM_FILE) {
	    /* Follow order given in input file. First primary inputs
	    ** and then present state variables.
	    */
	    for (i = 0; i < net->npis; i++) {
//	 	printf("PI node: %s\n", net->inputs[i]);
		if (!st_lookup(net->hash,net->inputs[i],&node)) {
		    return;
		}
		result = Bnet_BuildNodeBDD(*dd,node,net->hash,
					   option->locGlob,option->nodrop);
		if (result == 0) return;
	    }
	} else if (option->ordering == PI_PS_GIVEN) {
	    result = Bnet_ReadOrder(*dd,option->orderPiPs,net,option->locGlob,
				    option->nodrop);
	    if (result == 0) return;
	} else {
	    result = Bnet_DfsVariableOrder(*dd,net);
	    if (result == 0) return;
	}
    /* At this point the BDDs of all primary inputs and present state
    ** variables have been built. */
    		

    /* Specify the node to build to BDD */
    char **outputs = net->outputs;
//    printf("number of outputs: %d\n", net->noutputs);
	for(i = 0; i < net->noutputs; i++)
    {
		char *po_line = net->outputs[i];    
    	option->node = po_line; 
//    	printf("build bdd for output %s\n", po_line);

    	/* Build the BDDs for the nodes of the input network. */
    	result = Ntr_buildDDs(net, *dd, option, NULL);
    	if (result == 0) { printf("After ntr, result = 0!\n"); exit(2); }
	
	}
	
#ifdef DD_DEBUG
    Cudd_CheckKeys(*dd);
#endif
  
    //(void) printf("total time = %s\n", util_print_time(util_cpu_time() - option->initialTime));
    freeOption(option);
//    util_print_cpu_stats(stdout);

} /* end of cudd_build_v2 */


/*---------------------------------------------------------------------------*/
/* Definition of internal functions                                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Definition of static functions                                            */
/*---------------------------------------------------------------------------*/


/**Function********************************************************************

  Synopsis    [Allocates the option structure and initializes it.]

  Description []

  SideEffects [none]

  SeeAlso     [ntrReadOptions]

******************************************************************************/
static NtrOptions *
mainInit()
{
    NtrOptions	*option;

    /* Initialize option structure. */
    option = ALLOC(NtrOptions,1);
    option->initialTime    = util_cpu_time();
    option->file          = NULL;
    option->node           = NULL;
//    option->locGlob        = BNET_GLOBAL_DD;
    option->progress       = FALSE;
    option->cacheSize      = 32768;
    option->ordering       = PI_PS_FROM_FILE;
    option->orderPiPs      = NULL;
    option->reordering     = CUDD_REORDER_NONE;
    option->autoDyn        = 0;
    option->autoMethod     = CUDD_REORDER_SIFT;
    option->treefile       = NULL;
    option->firstReorder   = DD_FIRST_REORDER;
    option->countDead      = FALSE;
    option->groupcheck     = CUDD_GROUP_CHECK7;
    option->recomb         = DD_DEFAULT_RECOMB;
    option->nodrop         = FALSE;
    option->bdddump	   = FALSE;
    option->dumpFmt	   = 0;	/* dot */
    option->dumpfile	   = NULL;

    return(option);

} /* end of mainInit */


/**Function********************************************************************

  Synopsis    [Reads the command line options.]

  Description [Reads the command line options. Scans the command line
  one argument at a time and performs a switch on each arguement it
  hits.  Some arguemnts also read in the following arg from the list
  (i.e., -f also gets the filename which should folow.)
  Gives a usage message and exits if any unrecognized args are found.]

  SideEffects [May initialize the random number generator.]

  SeeAlso     [mainInit ntrReadOptionsFile]

******************************************************************************/
static void
ntrReadOptions(const char *filename, NtrOptions * option)
{
    option->file = filename;
    return;
} /* end of ntrReadOptions */



/**Function********************************************************************

  Synopsis    [Opens a file.]

  Description [Opens a file, or fails with an error message and exits.
  Allows '-' as a synonym for standard input.]

  SideEffects [None]

  SeeAlso     []

******************************************************************************/
static FILE *open_file(const char * filename)
{
    FILE *fp;
    fp = fopen(filename, "r");
    if(fp == NULL)
    {
        perror(filename);
        exit(1);
    }
    return(fp);
} /* end of open_file */


/**Function********************************************************************

  Synopsis    [Applies reordering to the DDs.]

  Description [Explicitly applies reordering to the DDs. Returns 1 if
  successful; 0 otherwise.]

  SideEffects [None]

  SeeAlso     []

*****************************************************************************/
static int
reorder(
  BnetNetwork * net,
  DdManager * dd /* DD Manager */,
  NtrOptions * option)
{
#ifdef DD_DEBUG
    st_table	*mintermTable;	/* minterm counts for each output */
#endif
    int result;			/* return value from functions */

    (void) printf("Number of inputs = %d\n",net->ninputs);

    /* Perform the final reordering */
    if (option->reordering != CUDD_REORDER_NONE) {
#ifdef DD_DEBUG
	result = Cudd_DebugCheck(dd);
	if (result != 0) {
	    (void) fprintf(stderr,"Error reported by Cudd_DebugCheck\n");
	    return(0);
	}
	result = Cudd_CheckKeys(dd);
	if (result != 0) {
	    (void) fprintf(stderr,"Error reported by Cudd_CheckKeys\n");
	    return(0);
	}
	mintermTable = checkMinterms(net,dd,NULL);
	if (mintermTable == NULL) exit(2);
#endif

	dd->siftMaxVar = 1000000;
	dd->siftMaxSwap = 1000000000;
	result = Cudd_ReduceHeap(dd,option->reordering,1);
	if (result == 0) return(0);
#ifdef DD_DEBUG
	result = Cudd_DebugCheck(dd);
	if (result != 0) {
	    (void) fprintf(stderr,"Error reported by Cudd_DebugCheck\n");
	    return(0);
	}
	result = Cudd_CheckKeys(dd);
	if (result != 0) {
	    (void) fprintf(stderr,"Error reported by Cudd_CheckKeys\n");
	    return(0);
	}
	mintermTable = checkMinterms(net,dd,mintermTable);
#endif

	/* Print symmetry stats if pertinent */
	if (dd->tree == NULL &&
	    (option->reordering == CUDD_REORDER_SYMM_SIFT ||
	    option->reordering == CUDD_REORDER_SYMM_SIFT_CONV))
	    Cudd_SymmProfile(dd,0,dd->size - 1);
    }

    return(1);

} /* end of reorder */


/**Function********************************************************************

  Synopsis    [Frees the option structure and its appendages.]

  Description []

  SideEffects [None]

  SeeAlso     []

*****************************************************************************/
static void
freeOption(
  NtrOptions * option)
{
/*    if (option->file != NULL) FREE(option->file);
    if (option->orderPiPs != NULL) FREE(option->orderPiPs);
    if (option->treefile != NULL) FREE(option->treefile);
    if (option->dumpfile != NULL) FREE(option->dumpfile);
    if (option->node != NULL) FREE(option->node);
*/
    FREE(option);

} /* end of freeOption */


/**Function********************************************************************

  Synopsis    [Starts the CUDD manager with the desired options.]

  Description [Starts the CUDD manager with the desired options.
  We start with 0 variables, because Ntr_buildDDs will create new
  variables rather than using whatever already exists.]

  SideEffects [None]

  SeeAlso     []

*****************************************************************************/
//static DdManager *startCudd(FILE *fp, NtrOptions * option, int nvars)
static DdManager *startCudd(NtrOptions * option, int nvars)
{
    DdManager *dd;
    int result;

    dd = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
    if (dd == NULL) return(NULL);

    Cudd_SetGroupcheck(dd,option->groupcheck);
    if (option->autoDyn & 1) {
	Cudd_AutodynEnable(dd,option->autoMethod);
    }
    dd->nextDyn = option->firstReorder;
    dd->countDead = (option->countDead == FALSE) ? ~0 : 0;
    dd->recomb = option->recomb;
//    FILE *fp;
//    fp = fopen("red_addition_v1.log", "a+");
//    dd->out = fp;
    result = ntrReadTree(dd,option->treefile,nvars);
    if (result == 0) {
	Cudd_Quit(dd);
	return(NULL);
    }
#ifndef DD_STATS
    result = Cudd_EnableReorderingReporting(dd);
    if (result == 0) {
	(void) fprintf(stderr, "Error reported by Cudd_EnableReorderingReporting\n");
	Cudd_Quit(dd);
	return(NULL);
    }
#endif

    return(dd);

} /* end of startCudd */


/**Function********************************************************************

  Synopsis    [Reads the variable group tree from a file.]

  Description [Reads the variable group tree from a file.
  Returns 1 if successful; 0 otherwise.]

  SideEffects [None]

  SeeAlso     []

*****************************************************************************/
static int
ntrReadTree(
  DdManager * dd,
  char * treefile,
  int  nvars)
{
    FILE *fp;
    MtrNode *root;

    if (treefile == NULL) {
	return(1);
    }

    if ((fp = fopen(treefile,"r")) == NULL) {
	(void) fprintf(stderr,"Unable to open %s\n",treefile);
	return(0);
    }

    root = Mtr_ReadGroups(fp,ddMax(Cudd_ReadSize(dd),nvars));
    if (root == NULL) {
	return(0);
    }

    Cudd_SetTree(dd,root);

    return(1);

} /* end of ntrReadTree */


