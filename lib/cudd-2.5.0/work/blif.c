/**********************************************************
 *
 * package for reading blif
 *
 **********************************************************/

#include "ntr.h"
#include "cuddInt.h"
#include "blif.h"
 
/******************************************************************************/
FILE *
open_file(
  char * filename,
  char * mode)
{
    FILE *fp;

    if (strcmp(filename, "-") == 0) {
        return mode[0] == 'r' ? stdin : stdout;
    } else if ((fp = fopen(filename, mode)) == NULL) {
        perror(filename);
        exit(1);
    }
    return(fp);

} /* end of open_file */

/*****************************************************************************/
DdManager *
startCudd(
  NtrOptions * option,
  int  nvars)
{
    DdManager *dd;
    int result;

    dd = Cudd_Init(0, 0, option->slots, option->cacheSize, option->maxMemory);
    if (dd == NULL) return(NULL);

    if (option->maxMemHard != 0) {
	Cudd_SetMaxMemory(dd,option->maxMemHard);
    }
    Cudd_SetMaxLive(dd,option->maxLive);
    Cudd_SetGroupcheck(dd,option->groupcheck);
    if (option->autoDyn & 1) {
	Cudd_AutodynEnable(dd,option->autoMethod);
    }
    dd->nextDyn = option->firstReorder;
    dd->countDead = (option->countDead == FALSE) ? ~0 : 0;
    dd->maxGrowth = 1.0 + ((float) option->maxGrowth / 100.0);
    dd->recomb = option->recomb;
    dd->arcviolation = option->arcviolation;
    dd->symmviolation = option->symmviolation;
    dd->populationSize = option->populationSize;
    dd->numberXovers = option->numberXovers;
    result = ntrReadTree(dd,option->treefile,nvars);
    if (result == 0) {
	Cudd_Quit(dd);
	return(NULL);
    }
#ifndef DD_STATS
    result = Cudd_EnableReorderingReporting(dd);
    if (result == 0) {
	(void) fprintf(stderr,
		       "Error reported by Cudd_EnableReorderingReporting\n");
	Cudd_Quit(dd);
	return(NULL);
    }
#endif

    return(dd);

} /* end of startCudd */


NtrOptions *
mainInit(
   )
{
    NtrOptions	*option;

    /* Initialize option structure. */
    option = ALLOC(NtrOptions,1);
    option->initialTime    = util_cpu_time();
    option->verify         = FALSE;
    option->second         = FALSE;
    option->file1          = NULL;
    option->file2          = NULL;
    option->traverse       = FALSE;
    option->depend         = FALSE;
    option->image          = NTR_IMAGE_MONO;
    option->imageClip      = 1.0;
    option->approx         = NTR_UNDER_APPROX;
    option->threshold      = -1;
    option->from	   = NTR_FROM_NEW;
    option->groupnsps      = NTR_GROUP_NONE;
    option->closure        = FALSE;
    option->closureClip    = 1.0;
    option->envelope       = FALSE;
    option->scc            = FALSE;
    option->maxflow        = FALSE;
    option->shortPath      = NTR_SHORT_NONE;
    option->selectiveTrace = FALSE;
    option->zddtest        = FALSE;
    option->printcover     = FALSE;
    option->sinkfile       = NULL;
    option->partition      = FALSE;
    option->char2vect      = FALSE;
    option->density        = FALSE;
    option->quality        = 1.0;
    option->decomp         = FALSE;
    option->cofest         = FALSE;
    option->clip           = -1.0;
    option->dontcares      = FALSE;
    option->closestCube    = FALSE;
    option->noBuild        = FALSE;
    option->stateOnly      = FALSE;
    option->node           = NULL;
    option->locGlob        = BNET_GLOBAL_DD;
    option->progress       = FALSE;
    option->cacheSize      = 32768;
    option->maxMemory      = 0;	/* set automatically */
    option->maxMemHard     = 0; /* don't set */
    option->maxLive        = ~0; /* very large number */
    option->slots          = CUDD_UNIQUE_SLOTS;
    option->ordering       = PI_PS_FROM_FILE;
    option->orderPiPs      = NULL;
    option->reordering     = CUDD_REORDER_NONE;
    option->autoMethod     = CUDD_REORDER_SIFT;
    option->autoDyn        = 0;
    option->treefile       = NULL;
    option->firstReorder   = DD_FIRST_REORDER;
    option->countDead      = FALSE;
    option->maxGrowth      = 20;
    option->groupcheck     = CUDD_GROUP_CHECK7;
    option->arcviolation   = 10;
    option->symmviolation  = 10;
    option->recomb         = DD_DEFAULT_RECOMB;
    option->nodrop         = TRUE;
    option->signatures     = FALSE;
    option->verb           = 0;
    option->gaOnOff        = 0;
    option->populationSize = 0;	/* use default */
    option->numberXovers   = 0;	/* use default */
    option->bdddump	   = FALSE;
    option->dumpFmt	   = 0;	/* dot */
    option->dumpfile	   = NULL;
    option->store          = -1; /* do not store */
    option->storefile      = NULL;
    option->load           = FALSE;
    option->loadfile       = NULL;

    return(option);

} /* end of mainInit */


/*****************************************************************************/
int
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


/******************************************************************************
 * 
 * Print the BDD representation of the function
 *
 ******************************************************************************/
int 
printBdd (
  BnetNetwork      *net1,        // Circuit network created from blif file
  DdManager	   *dd	        // BDD of the circuit network
  )
{
   int            noutput;      // number of output node
   char           **outputs;    // array of primary variable name
   char           *name;        // a primary output name
   BnetNode       *bnode;       // 
   BnetNode       *xnode;
   int            nvars;         // number of variables
   int            i;
   DdNode         *dnode;   

   
   // print variable order
   printf(" Variable Order: \n");
   Bnet_PrintOrder(net1, dd);
   printf(" \n");
   
   // get variable number
   nvars = Cudd_ReadSize(dd);
   
   
   noutput = net1->noutputs;
   outputs = net1->outputs;
   for(i=0; i<noutput; i++)
    {
     name = outputs[i];
     printf(" Primary output: %s \n",  name);
     
     // traversal the linked list
     xnode = NULL;
     
     for(bnode=net1->nodes; bnode!=NULL; bnode=bnode->next)
     {
      if(strcmp(bnode->name, name)==0)
      {
       xnode =bnode;
       break;
      }
     }  
      
     if(xnode!=NULL)
      {
        dnode = xnode->dd;
	// print
	Cudd_PrintDebug(dd, dnode, nvars, 4); 
      }
      
    }
   
   return 0;
  } 


