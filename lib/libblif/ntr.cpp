/**CFile***********************************************************************

  FileName    [ntr.c]

  PackageName [ntr]

  Synopsis    [A very simple reachability analysis program.]

  Description []

  SeeAlso     []

  Author      [Fabio Somenzi]

  Copyright   [Copyright (c) 1995-2004, Regents of the University of Colorado

  All rights reserved.

******************************************************************************/

#include "ntr.h"

/*---------------------------------------------------------------------------*/
/* Constant declarations                                                     */
/*---------------------------------------------------------------------------*/

#define NTR_MAX_DEP_SIZE 20

/*---------------------------------------------------------------------------*/
/* Stucture declarations                                                     */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Type declarations                                                         */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Variable declarations                                                     */
/*---------------------------------------------------------------------------*/

#ifndef lint
static char rcsid[] UTIL_UNUSED = "$Id: ntr.c,v 1.25 2004/08/13 18:28:28 fabio Exp fabio $";
#endif


/*---------------------------------------------------------------------------*/
/* Macro declarations                                                        */
/*---------------------------------------------------------------------------*/

/**AutomaticStart*************************************************************/

/*---------------------------------------------------------------------------*/
/* Static function prototypes                                                */
/*---------------------------------------------------------------------------*/

static void ntrInitializeCount (BnetNetwork *net, NtrOptions *option);
static void ntrCountDFS (BnetNetwork *net, BnetNode *node);

/**AutomaticEnd***************************************************************/

/*---------------------------------------------------------------------------*/
/* Definition of exported functions                                          */
/*---------------------------------------------------------------------------*/


/**Function********************************************************************

  Synopsis    [Builds DDs for a network outputs and next state
  functions.]

  Description [Builds DDs for a network outputs and next state
  functions. The method is really brain-dead, but it is very simple.
  Returns 1 in case of success; 0 otherwise. Some inputs to the network
  may be shared with another network whose DDs have already been built.
  In this case we want to share the DDs as well.]

  SideEffects [the dd fields of the network nodes are modified. Uses the
  count fields of the nodes.]

  SeeAlso     []

******************************************************************************/
int
Ntr_buildDDs(
  BnetNetwork * net /* network for which DDs are to be built */,
  DdManager * dd /* DD manager */,
  NtrOptions * option /* option structure */,
  BnetNetwork * net2 /* companion network with which inputs may be shared */)
{
    int result;
    int i;
    BnetNode *node;
    
    option->locGlob == BNET_GLOBAL_DD;
	/* Create BDDs with DFS from the primary outputs and the next
	** state functions. If the inputs had not been ordered yet,
	** this would result in a DFS order for the variables.
	*/

//	ntrInitializeCount(net,option);


	if (option->node != NULL) {
	    if (!st_lookup(net->hash,option->node, &node)) {
		return(0);
	    }
		result = Bnet_BuildNodeBDD(dd,node,net->hash, option->locGlob,
				       option->nodrop);
	    if (result == 0) return(0);
	} 
	
	/* Make sure all inputs have a DD and dereference the DDs of
	** the nodes that are not reachable from the outputs.
	*/
/*	for (i = 0; i < net->npis; i++) {
	    if (!st_lookup(net->hash,net->inputs[i],&node)) {
		return(0);
	    }
		result = Bnet_BuildNodeBDD(dd,node,net->hash, option->locGlob,
				       option->nodrop);
	    if (result == 0) return(0);
	    if (node->count == -1) Cudd_RecursiveDeref(dd,node->dd);
	}
*/
	/* Dispose of the BDDs of the internal nodes if they have not
	** been dropped already.
	*/
/*	if (option->nodrop == TRUE) {
	    for (node = net->nodes; node != NULL; node = node->next) {
		if (node->dd != NULL && node->count != -1 &&
		    (node->type == BNET_INTERNAL_NODE ||
		    node->type == BNET_INPUT_NODE ||
		    node->type == BNET_PRESENT_STATE_NODE)) {
		    Cudd_RecursiveDeref(dd,node->dd);
		    if (node->type == BNET_INTERNAL_NODE) node->dd = NULL;
		}
	    }
	}
*/	
    return(1);

} /* end of buildDD */


/*---------------------------------------------------------------------------*/
/* Definition of internal functions                                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Definition of static functions                                            */
/*---------------------------------------------------------------------------*/

/**Function********************************************************************

  Synopsis    [Initializes the count fields used to drop DDs.]

  Description [Initializes the count fields used to drop DDs.
  Before actually building the BDDs, we perform a DFS from the outputs
  to initialize the count fields of the nodes.  The initial value of the
  count field will normally coincide with the fanout of the node.
  However, if there are nodes with no path to any primary output or next
  state variable, then the initial value of count for some nodes will be
  less than the fanout. For primary outputs and next state functions we
  add 1, so that we will never try to free their DDs. The count fields
  of the nodes that are not reachable from the outputs are set to -1.]

  SideEffects [Changes the count fields of the network nodes. Uses the
  visited fields.]

  SeeAlso     []

******************************************************************************/
static void
ntrInitializeCount(
  BnetNetwork * net,
  NtrOptions * option)
{
    BnetNode *node;
    int i;

    if (option->node != NULL) {
	if (!st_lookup(net->hash,option->node,&node)) {
	    (void) fprintf(stdout, "Warning: node %s not found!\n",
			   option->node);
	} else {
	    ntrCountDFS(net,node);
	    node->count++;
	}
    } 
    
    /* Clear visited flags. */
    node = net->nodes;
    while (node != NULL) {
	if (node->visited == 0) {
	    node->count = -1;
	} else {
	    node->visited = 0;
	}
	node = node->next;
    }

} /* end of ntrInitializeCount */


/**Function********************************************************************

  Synopsis    [Does a DFS from a node setting the count field.]

  Description []

  SideEffects [Changes the count and visited fields of the nodes it
  visits.]

  SeeAlso     [ntrLevelDFS]

******************************************************************************/
static void
ntrCountDFS(
  BnetNetwork * net,
  BnetNode * node)
{
    int i;
    BnetNode *auxnd;

    node->count++;

    if (node->visited == 1) {
	return;
    }

    node->visited = 1;

    for (i = 0; i < node->ninp; i++) {
	if (!st_lookup(net->hash, node->inputs[i], &auxnd)) {
	    exit(2);
	}
	ntrCountDFS(net,auxnd);
    }

} /* end of ntrCountDFS */

