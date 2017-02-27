/**CHeaderFile*****************************************************************

  FileName    [ntr.h]

  PackageName [ntr]

  Synopsis    [Simple-minded package to do traversal.]

  Description []

  SeeAlso     []

  Author      [Fabio Somenzi]

  Copyright   [Copyright (c) 1995-2004, Regents of the University of Colorado

  All rights reserved.

******************************************************************************/

#ifndef _NTR
#define _NTR

/*---------------------------------------------------------------------------*/
/* Nested includes                                                           */
/*---------------------------------------------------------------------------*/

//#include "/home/wuyi/usr/CUDD/cudd-2.5.0/dddmp/dddmp.h"
#include "bnet.h"

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
/* Constant declarations                                                     */
/*---------------------------------------------------------------------------*/

#define PI_PS_FROM_FILE	0
#define PI_PS_DFS	1
#define PI_PS_GIVEN	2

#define NTR_IMAGE_MONO 0
#define NTR_IMAGE_PART 1
#define NTR_IMAGE_CLIP 2
#define NTR_IMAGE_DEPEND 3

#define NTR_UNDER_APPROX 0
#define NTR_OVER_APPROX 1

#define NTR_FROM_NEW 0
#define NTR_FROM_REACHED 1
#define NTR_FROM_RESTRICT 2
#define NTR_FROM_COMPACT 3
#define NTR_FROM_SQUEEZE 4
#define NTR_FROM_UNDERAPPROX 5
#define NTR_FROM_OVERAPPROX 6

#define NTR_GROUP_NONE 0
#define NTR_GROUP_DEFAULT 1
#define NTR_GROUP_FIXED 2

#define NTR_SHORT_NONE 0
#define NTR_SHORT_BELLMAN 1
#define NTR_SHORT_FLOYD 2
#define NTR_SHORT_SQUARE 3

/*---------------------------------------------------------------------------*/
/* Stucture declarations                                                     */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Type declarations                                                         */
/*---------------------------------------------------------------------------*/

typedef	struct	NtrOptions {
    long	initialTime;	/* this is here for convenience */
    const char	*file;		/* first network file name */
    char	*node;		/* only node for which to build BDD */
    int		locGlob;	/* build global or local BDDs */
    int		progress;	/* report output names while building BDDs */
    int		cacheSize;	/* computed table initial size */
    int		ordering;	/* FANIN DFS ... */
    char	*orderPiPs;	/* file for externally provided order */
    Cudd_ReorderingType	reordering; /* NONE RANDOM PIVOT SIFTING ... */
    int		autoDyn;	/* ON OFF */
    Cudd_ReorderingType autoMethod; /* RANDOM PIVOT SIFTING CONVERGE ... */
    char 	*treefile;	/* file name for variable tree */
    int		firstReorder;	/* when to do first reordering */
    int		countDead;	/* count dead nodes toward triggering reordering */
    Cudd_AggregationType groupcheck; /* grouping function */

    int		recomb;		/* recombination parameter for grouping */
    int		nodrop;		/* don't drop intermediate BDDs ASAP */
    int		bdddump;	/* ON OFF */
    int		dumpFmt;	/* 0 -> dot 1 -> blif 2 ->daVinci 3 -> DDcal
				** 4 -> factored form */
   const char	*dumpfile;	/* filename for dump */
   const char    *assign;    /* the input assignment*/
} NtrOptions;

typedef struct NtrHeapSlot {
    void *item;
    int key;
} NtrHeapSlot;

typedef struct NtrHeap {
    int size;
    int nslots;
    NtrHeapSlot *slots;
} NtrHeap;

typedef struct NtrPartTR {
    int nparts;			/* number of parts */
    DdNode **part;		/* array of parts */
    DdNode **icube;		/* quantification cubes for image */
    DdNode **pcube;		/* quantification cubes for preimage */
    DdNode **nscube;		/* next state variables in each part */
    DdNode *preiabs;		/* present state vars and inputs in no part */
    DdNode *prepabs;		/* inputs in no part */
    DdNode *xw;			/* cube of all present states and PIs */
    NtrHeap *factors;		/* factors extracted from the image */
    int nlatches;		/* number of latches */
    DdNode **x;			/* array of present state variables */
    DdNode **y;			/* array of next state variables */
} NtrPartTR;

/*---------------------------------------------------------------------------*/
/* Variable declarations                                                     */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Macro declarations                                                        */
/*---------------------------------------------------------------------------*/

#ifndef TRUE
#   define TRUE 1
#endif
#ifndef FALSE
#   define FALSE 0
#endif

/**Macro***********************************************************************

  Synopsis     [Returns 1 if the two arguments are identical strings.]

  Description  []

  SideEffects  [none]

  SeeAlso      []
		   
******************************************************************************/
#define STRING_EQUAL(s1,s2) (strcmp((s1),(s2)) == 0)


/**AutomaticStart*************************************************************/

/*---------------------------------------------------------------------------*/
/* Function prototypes                                                       */
/*---------------------------------------------------------------------------*/

extern int Ntr_buildDDs (BnetNetwork *net, DdManager *dd, NtrOptions *option, BnetNetwork *net2);
extern NtrPartTR * Ntr_buildTR (DdManager *dd, BnetNetwork *net, NtrOptions *option, int image);
extern DdNode * Ntr_TransitiveClosure (DdManager *dd, NtrPartTR *TR, NtrOptions *option);
extern int Ntr_Trav (DdManager *dd, BnetNetwork *net, NtrOptions *option);
extern int Ntr_SCC (DdManager *dd, BnetNetwork *net, NtrOptions *option);
extern int Ntr_ClosureTrav (DdManager *dd, BnetNetwork *net, NtrOptions *option);
extern void Ntr_freeTR (DdManager *dd, NtrPartTR *TR);
extern NtrPartTR * Ntr_cloneTR (NtrPartTR *TR);
extern DdNode * Ntr_initState (DdManager *dd, BnetNetwork *net, NtrOptions *option);
extern DdNode * Ntr_getStateCube (DdManager *dd, BnetNetwork *net, char *filename, int pr);
extern int Ntr_Envelope (DdManager *dd, NtrPartTR *TR, FILE *dfp, NtrOptions *option);
extern int Ntr_TestMinimization (DdManager *dd, BnetNetwork *net1, BnetNetwork *net2, NtrOptions *option);
extern int Ntr_TestDensity (DdManager *dd, BnetNetwork *net1, NtrOptions *option);
extern int Ntr_TestDecomp (DdManager *dd, BnetNetwork *net1, NtrOptions *option);
extern int Ntr_VerifyEquivalence (DdManager *dd, BnetNetwork *net1, BnetNetwork *net2, NtrOptions *option);
extern int Ntr_TestCofactorEstimate (DdManager * dd, BnetNetwork * net, NtrOptions * option);
extern int Ntr_TestClipping (DdManager *dd, BnetNetwork *net1, BnetNetwork *net2, NtrOptions *option);
extern int Ntr_TestEquivAndContain (DdManager *dd, BnetNetwork *net1, BnetNetwork *net2, NtrOptions *option);
extern int Ntr_TestClosestCube (DdManager * dd, BnetNetwork * net, NtrOptions * option);
extern int Ntr_TestTwoLiteralClauses (DdManager * dd, BnetNetwork * net1, NtrOptions * option);
extern int Ntr_TestCharToVect(DdManager * dd, BnetNetwork * net1, NtrOptions * option);
extern int Ntr_maxflow (DdManager *dd, BnetNetwork *net, NtrOptions *option);
extern double Ntr_maximum01Flow (DdManager *bdd, DdNode *sx, DdNode *ty, DdNode *E, DdNode **F, DdNode **cut, DdNode **x, DdNode **y, DdNode **z, int n, int pr);
extern int Ntr_testZDD (DdManager *dd, BnetNetwork *net, NtrOptions *option);
extern int Ntr_testISOP (DdManager *dd, BnetNetwork *net, NtrOptions *option);
extern NtrHeap * Ntr_InitHeap (int size);
extern void Ntr_FreeHeap (NtrHeap *heap);
extern int Ntr_HeapInsert (NtrHeap *heap, void *item, int key);
extern int Ntr_HeapExtractMin (NtrHeap *heap, void *item, int *key);
extern int Ntr_HeapCount (NtrHeap *heap);
extern NtrHeap * Ntr_HeapClone (NtrHeap *source);
extern int Ntr_TestHeap (NtrHeap *heap, int i);
extern int Ntr_ShortestPaths (DdManager *dd, BnetNetwork *net, NtrOptions *option);

/**AutomaticEnd***************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _NTR */
