/****************************************************************
 * 
 * Head file for blif package
 *
 ****************************************************************/

FILE * open_file(  char * filename, char * mode);
DdManager * startCudd(NtrOptions * option, int  nvars);
NtrOptions * mainInit();
int ntrReadTree(DdManager * dd, char * treefile, int  nvars);
int printBdd (BnetNetwork *net1, DdManager *dd);
 
