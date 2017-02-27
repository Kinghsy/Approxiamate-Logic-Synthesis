/**CHeaderFile*****************************************************************

  FileName    [bnet.h]

  PackageName [bnet]

  Synopsis    [Simple-minded package to read a blif file.]

  Description []

  SeeAlso     []

  Author      [Fabio Somenzi]

  Copyright   [Copyright (c) 1995-2004, Regents of the University of Colorado

  All rights reserved.

******************************************************************************/

#ifndef _BNET
#define _BNET


/*---------------------------------------------------------------------------*/
/* Nested includes                                                           */
/*---------------------------------------------------------------------------*/

#include "../lib/cudd-2.5.0/util/util.h"
#include "../lib/cudd-2.5.0/st/st.h"
#include "../lib/cudd-2.5.0/cudd/cudd.h"

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
/* Constant declarations                                                     */
/*---------------------------------------------------------------------------*/

/* Different types of nodes. (Used in the "BnetNode" type.) */
#define BNET_CONSTANT_NODE 0
#define BNET_INPUT_NODE 1
#define BNET_PRESENT_STATE_NODE 2
#define BNET_INTERNAL_NODE 3
#define BNET_OUTPUT_NODE 4
#define BNET_NEXT_STATE_NODE 5

/* Type of DD of a node. */
#define BNET_LOCAL_DD 0
#define BNET_GLOBAL_DD 1


/*---------------------------------------------------------------------------*/
/* Stucture declarations                                                     */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Type declarations                                                         */
/*---------------------------------------------------------------------------*/

/* The following types implement a very simple data structure for a boolean
** network. The intent is to be able to read a minimal subset of the blif
** format in a data structure from which it's easy to build DDs for the
** circuit.
*/

/* Type to store a line of the truth table of a node. The entire truth table
** implemented as a linked list of objects of this type.
*/
typedef struct BnetTabline {
    char *values;		/* string of 1, 0, and - */
    struct BnetTabline *next;	/* pointer to next table line */
} BnetTabline;

/* Node of the boolean network. There is one node in the network for each
** primary input and for each .names directive. This structure
** has a field to point to the DD of the node function. The function may
** be either in terms of primary inputs, or it may be in terms of the local
** inputs. The latter implies that each node has a variable index
** associated to it at some point in time. The field "var" stores that
** variable index, and "active" says if the association is currently valid.
** (It is indeed possible for an index to be associated to different nodes
** at different times.)
*/
typedef struct BnetNode {
    char *name;		/* name of the output signal */
    int type;		/* input, internal, constant, ... */
    int ninp;		/* number of inputs to the node */
    int nfo;		/* number of fanout nodes for this node */
    double p;
    double rp;
    char **inputs;	/* input names */
    char **fanouts; /* fanout names */
    BnetTabline *f;	/* truth table for this node */
    int polarity;	/* f is the onset (0) or the offset (1) */
    int active;		/* node has variable associated to it (1) or not (0) */
    int var;		/* DD variable index associated to this node */
    DdNode *dd;		/* decision diagram for the function of this node */
    int exdc_flag;	/* whether an exdc node or not */
    struct BnetNode *exdc; /* pointer to exdc of dd node */
    int count;		/* auxiliary field for DD dropping */
    int level;		/* maximum distance from the inputs */
    int visited;	/* flag for search */
    struct BnetNode *next; /* pointer to implement the linked list of nodes */
} BnetNode;

/* Very simple boolean network data structure. */
typedef struct BnetNetwork {
    char *name;		/* network name: from the .model directive */
    int npis;		/* number of primary inputs */
    int ninputs;	/* number of inputs */
    char **inputs;	/* primary input names: from the .inputs directive */
    int npos;		/* number of primary outputs */
    int noutputs;	/* number of outputs */
    char **outputs;	/* primary output names: from the .outputs directive */
    BnetNode *nodes;	/* linked list of the nodes */
    st_table *hash;	/* symbol table to access nodes by name */
} BnetNetwork;

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

/**AutomaticStart*************************************************************/

/*---------------------------------------------------------------------------*/
/* Function prototypes                                                       */
/*---------------------------------------------------------------------------*/

extern BnetNetwork * Bnet_ReadNetwork (FILE *fp);
extern void Bnet_PrintNetwork (BnetNetwork *net);
extern void Bnet_PrintNetwork_v2(BnetNetwork * net, FILE *fp);
extern void Bnet_FreeNetwork (BnetNetwork *net);
extern void Bnet_FreeNetwork_Bdd(BnetNetwork * net, DdManager *dd);
extern int Bnet_BuildNodeBDD (DdManager *dd, BnetNode *nd, st_table *hash, int params, int nodrop);
extern int Bnet_DfsVariableOrder (DdManager *dd, BnetNetwork *net);
extern int Bnet_bddDump (DdManager *dd, BnetNetwork *network, char *dfile, int dumpFmt, int reencoded);
extern int Bnet_bddArrayDump (DdManager *dd, BnetNetwork *network, const char *dfile, DdNode **outputs, char **onames, int noutputs, int dumpFmt);
extern int Bnet_ReadOrder (DdManager *dd, char *ordFile, BnetNetwork *net, int locGlob, int nodrop);
extern int Bnet_PrintOrder (BnetNetwork * net, DdManager *dd);

/**AutomaticEnd***************************************************************/

#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif /* _BNET */
