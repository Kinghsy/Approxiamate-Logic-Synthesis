/******************************************************/
/*						      */ 
/* this is the main file for my CUDD BDD manipulator  */
/* 						      */
/******************************************************/

#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>

#include "cuddobj.hh"
#include "util.h"
#include "cuddInt.h"

// ///////////////////////////////
// GLOBAL VARIABLES and CONSTANTS
// ///////////////////////////////

int debug;	     // stores the debugging flag
const int MAXSIZE = 150; // the max num of variables we can deal with
DdManager* ddman;    // global ddmanager pointer variable
DdNode** ddnodearray; // global array of pointers to nodes 
int numfunctions;    // number of functions
int numvars;	     // number of variables in the functions

void printStats() 
{
  //if (debug) 
  //{
  //  cout << "Stats from Cudd_PrintInfo: \n\n";
  //  Cudd_PrintInfo(ddman, stdout);
  //  cout << "End of Stats from Cudd_PrintInfo \n\n";
  //}

  cout << "-----------------------------------------------------------\n";
  cout << "num variables in existence: " << Cudd_ReadSize(ddman) << "\n";
  cout << "node count: " << Cudd_ReadNodeCount(ddman) << "\n";
  cout << "num reorderings: " << Cudd_ReadReorderings(ddman) << "\n";
  cout << "memory in use: " << Cudd_ReadMemoryInUse(ddman) << " bytes \n\n";

  if (debug) 
    for (int i=0; i<numfunctions; i++)
      Cudd_PrintDebug(ddman, ddnodearray[i], 0, 1); // 4 is highest print level
  cout << "-----------------------------------------------------------\n\n"; 
}

void readCube(ifstream* infile, int* ordering)
{
  // this routine is called by createDDs for each cube in the 
  // input file.  It reads the cube, then adds to the appropriate
  // function in the DdManager

  char* inputs = new char[numvars+1];
  inputs[numvars] = '\0';
  char* outputs = new char[numfunctions+1];
  outputs[numfunctions] = '\0';
 
  // read the inputs from the file
  (*infile) >> inputs;
  if (debug) cout << "cube inputs: " << inputs << "\n";
 
  // read the outputs from the file
  (*infile) >> outputs;
  if (debug) cout << "cube outputs: " << outputs << "\n";


  // -----------------------------------------------
  // first build a DD for just this cube, call it cube
  // -----------------------------------------------

  DdNode *cube, *var, *tmp, *tmp2;

  cube = Cudd_ReadOne(ddman);
  Cudd_Ref(cube);

  for (int i=0; i<numvars; i++) {
    switch(inputs[ordering[i]-1]) 
    // the ordering list starts at 1, so subtract 1 to get a list
    // beginning with 0
    {
    case '0':
      var = Cudd_Not(Cudd_bddIthVar(ddman,i));
      break;
    case '1':
      var = Cudd_bddIthVar(ddman, i);
      break;
    case '-':
      var = Cudd_ReadOne(ddman);
      break;
    }
    tmp = Cudd_bddAnd(ddman, cube, var);
    Cudd_Ref(tmp);
    Cudd_RecursiveDeref(ddman, cube);
    //Cudd_RecursiveDeref(ddman, var);  // is this needed??? appears not.
    cube = tmp;
  }

  // ---------------------------------------
  // now add the cube to the appropriate DDs
  // (only the ones with 1 in the outputs for
  // this cube) 
  // ---------------------------------------

  for (int j=0; j<numfunctions; j++) {
    if (outputs[j] == '1') {
      tmp2 = Cudd_bddOr(ddman, ddnodearray[j], cube);
      Cudd_Ref(tmp2);
      Cudd_RecursiveDeref(ddman, ddnodearray[j]);
      //Cudd_RecursiveDeref(ddman, cube);	// do this after the for loop
      ddnodearray[j] = tmp2;
    }
  }
  Cudd_RecursiveDeref(ddman, cube);   
}

void createDDs(ifstream* infile, int* ordering) 
{

  // this routine reads the cubes from the input file pointed
  // to by f.  This file must be in espresso pla format.  

  // the global DdManager*  variable ddman and the global
  // DdNode* * variable ddnodearray (an array) are used

  // ----------------------------
  // first initialize the DdManager object
  // ----------------------------

  short numVars=0;  // num of vars; if unknown set to 0
  short numVarsZ=0; // num of vars for ZBDDs; if unknown set to 0
  int numSlots=CUDD_UNIQUE_SLOTS; // default for CUDD package
  int cacheSize=CUDD_CACHE_SLOTS; // default for CUDD package
  int maxCacheSize=10485760*2; // ditto

  ddman = Cudd_Init(numVars, numVarsZ, numSlots, cacheSize, maxCacheSize);

  // Cudd_SetLooseUpTo(ddman, 10);

  // ----------------------------
  // now read the input file...
  // ----------------------------

  if (debug) cout << "reading input file\n";

  bool keep_looping = true;
  char ch, ch2;

  while (keep_looping) 
  {
    infile->get(ch);

    while ( (ch!='1') && (ch!='0') && (ch!='-') && (ch!='.') )
      infile->get(ch);
 
    if (ch=='.') // deal with header/footer stuff
    { 
 
      infile->get(ch);
      infile->get(ch2);
 
      // if the character isn't i, o or e then
      // read over the entire line and discard it

      if ( (ch!='i') && (ch!='o') & (ch!='e') )
        while (ch != '\n') infile->get(ch);
 
	 
      if ( (ch=='i') && isspace(ch2) ) 
      {
        // read the number of inputs
        (*infile) >> numvars;
        if (debug) cout << "numvars: " << numvars << "\n";
 
      }

      else if ( (ch=='o') && isspace(ch2) ) 
      {
        // read the number of outputs
        (*infile) >> numfunctions;
        if (debug) cout << "numfunctions: " << numfunctions << "\n ";

	// now create the appropriate number of empty dds

 	ddnodearray = (DdNode**)malloc(numfunctions * sizeof(DdNode*));
 	for (int i=0; i<numfunctions; i++) 
	  ddnodearray[i] = Cudd_ReadLogicZero(ddman);

      }

      else if (ch=='e') 
      { 
        keep_looping = false;
      }
    }

    else  // the character read was NOT a '.'
    {
      // put back the char just read
      infile->putback(ch);

      // read a cube and add it to the dds
      readCube(infile, ordering);
    }

  } // end while
 
}

int* readOrder(ifstream* infile) 
{
  // this routine reads the ordering from the order file 
  // pointed to by f.
  // the format is expected to be as follows:
  // 2 1 5 4 3

  char ch;
  int a[MAXSIZE];
  int count = 0;

  infile->get(ch);
  while(!infile->eof()) {

    // if the character is a digit, put it back
    // and read the whole number into the ordering
    // array, a
 
    if (isdigit(ch)) {
	infile->putback(ch);
 	(*infile) >> a[count];
 	count++;
	if (count == MAXSIZE) {
	   cerr << "Exceeded max variables in the orderfile; exiting.\n";
	   exit(1);
 	}
    }

    // read the next char 
    infile->get(ch);
  }

  int* ar = (int*)malloc(count*sizeof(int));
  for (int i=0; i<count; i++) ar[i] = a[i]; 

  return ar;

}

void printusage() 
{
  // print instructions on how to use the program //
  cerr << "flags: \n";
  cerr << "       -d <optional> turn on debugging output\n";
  cerr << "       -f filename <required> specify file to read\n";
  cerr << "          file must be in espresso pla format\n";
  cerr << "       -o orderfile <optional> specify file containing order\n";
  cerr << "	  -s <optional> use sifting to order\n";
  cerr << "       -e <optional> use exact method for ordering\n";
  cerr << "          (-s and -e may not be used together)\n\n";

}

ifstream* openFile(int pos, char* argv[]) 
{

   if (pos == 0) 
   {
     printusage();
     return NULL;
   }
   else 
   {
     ifstream* f = new ifstream(argv[pos]);
     if (!f) 
     {
        cerr << "cannot open file " << argv[pos] << "\n";
        return NULL;
     }
     else
	return f;
  }

}

/******************************************************/
/* MAIN 					      */
/******************************************************/

int main(int argc, char* argv[]) 
{

  debug = 0;
  int fnamepos = 0;
  char ordermethod = 'n';
  int onamepos = 0;

  for (int i=1; i<argc; i++) {

    if (argv[i][0] == '-') {

	switch (argv[i][1]) {

	case 'o':
	  if (argc <= i+1)  {
	    printusage();
	    return 0;
          }
	  else onamepos = i+1;
	  break;

	case 'e': // exact method
	  ordermethod = 'e';
	  break;

	case 's': // sift
	  ordermethod = 's';
	  break;

	case 'f':
	  if (argc <= i+1)  {
	    printusage();
	    return 0;
	  }
	  else fnamepos = i+1;
	  break;
		
	case 'd':
	  debug = 1;
	  break;

	case 'h':
	  printusage();
	  return 1;
	  break;

	default:
	  printusage();
	  return 0;
	  break;

	} // end switch
     } // end if '-'


  } // end for

  // ----------------
  // open the files
  // ----------------

  if (fnamepos == 0) {
    printusage();
    return 0;
  }


  // ------------------
  // Print header
  // ------------------
  cout << "Bdd builder by J. Rice. last modified Jan 6 2000\n";
  cout << "based on CUDD package ";
  Cudd_PrintVersion(stdout);
  cout << "\n";

  if (debug) cout << "opening file " << argv[fnamepos] << "\n\n";
  ifstream* infile = openFile(fnamepos, argv);
  if (infile == NULL) return 0;
 
  // ------------------------------------------
  // read the order file and create an ordering
  // (default ordering is 0, 1, 2, ... )
  // ------------------------------------------

  int* ordering = (int*)malloc(MAXSIZE*sizeof(int));

  if (onamepos > 0) {
    if (debug) cout << "opening file " << argv[onamepos] << "\n\n";
    ifstream* orderfile = openFile(onamepos, argv);
    if (orderfile == NULL) return 0;

    if (debug) cout << "reading file " << argv[onamepos] << "\n\n";
    ordering = readOrder(orderfile);
  }
  else {
    if (debug) cout << "creating default ordering \n\n";
    for (int i=0; i<MAXSIZE; i++) ordering[i] = i+1;
    // default order starts at 1, because that's how an ordering
    // in a file would be.
  }

  if (debug) {
     cout << "ordering is:\n";
     for (int i=0; i<MAXSIZE; i++) cout << ordering[i] << " ";
     cout << "\n";
  }

  // ----------------------------------------------
  // create the DD(s)  using the specified ordering
  // ----------------------------------------------

  if (debug) cout << "creating DDs...\n\n";
  createDDs(infile, ordering);

  // ------------------------------------------
  // print stats...
  // ------------------------------------------

  cout << "\nstatistics after creating DDs using following ordering:\n";
  for (int i=0; i<numvars; i++) cout << ordering[i] << " ";
  cout << "\n\n";

  printStats();

  // ------------------------------------------
  // do reordering, if requested
  // REMOVED, as Cudd_ShuffleHeap gave seg fault.
  // now the DDs are created with ordering specified.
  // ------------------------------------------

//  if (onamepos > 0) {
//
//    cout << "\nstatistics after ordering as follows:\n";
//    for (int i=0; i<numvars; i++) cout << ordering[i] << " ";
//    cout << "\n\n";
//    int success=Cudd_ShuffleHeap(ddman, ordering);
//    if (success!=1) cout << "REORDERING FAILED!!\n";
//    else printStats();
//  }
	
  char* orderstring;
  switch (ordermethod)
  {
  case 'n': // no ordering
    orderstring = "none";
    //Cudd_ReduceHeap(ddman, CUDD_REORDER_NONE, 1);
    break;
 
  case 's': // sifting
    orderstring = "sifting";
    Cudd_ReduceHeap(ddman, CUDD_REORDER_SIFT, 1);
    break;
 
  case 'e': // exact
    orderstring = "exact";
    Cudd_ReduceHeap(ddman, CUDD_REORDER_EXACT, 1);
    break;
 
  default:
    break;
  }
 
  if (ordermethod != 'n') 
  {
    cout << "\n\nStatistics after ordering using method " << orderstring << ":\n";
 
    // ------------------------------------------
    // print after ordering stats
    // ------------------------------------------
    printStats();
  }

  // ------------------------------------------
  // destroy the ddmanager
  // ------------------------------------------

  if (debug) 
  {
    cout << "before de-refing diagrams, nodes with non-zero ref counts: ";
    cout << Cudd_CheckZeroRef(ddman) << "\n";
    for (int i=0; i<numfunctions; i++) 
      Cudd_RecursiveDeref(ddman, ddnodearray[i]);
    cout << "after deref-ing all diagrams, nodes with non-zero ref counts: ";
    cout << Cudd_CheckZeroRef(ddman) << "\n\n";
  }

  Cudd_Quit(ddman);


  return 1; 
}             
