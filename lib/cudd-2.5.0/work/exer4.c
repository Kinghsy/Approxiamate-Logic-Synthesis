/*
 * A test program to read blif file
 */
 
#include "ntr.h"
#include "cuddInt.h"
#include "blif.h"

int 
main(  
     int  argc,
     char ** argv)
{

 NtrOptions	*option;	/* options */
 FILE	        *fp1;		/* Blif file pointer*/
 BnetNetwork	*net1 = NULL;	/* Network created from blif file */
 int		pr;		/* verbosity level */
 DdManager	*dd;		/* pointer to DD manager */
 int            result;

 
 
 if (argc!=2)
  {
   printf("Missing Blif file name.\n");
   exit(1);
  }
 else
  {
   /* Initialize. */
   option = mainInit();
   
   // Read Blif file and store information into BnetNetwork net1
   fp1 = open_file(argv[1], "r");
   net1 = Bnet_ReadNetwork(fp1,pr);
   
   // Build BDD from BnetNetwork net1
   dd = startCudd(option,net1->ninputs);
   result = Ntr_buildDDs(net1,dd,option,NULL);
   
   // Exit if errors occur during the process of building BDD
   if (result == 0) { exit(2); }
   
   // print BDD structures for each primary output in blif file
   printBdd(net1, dd);
     
  }
 return 0;
}
