#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <climits>
#include <vector>
#include <map>
#include <cassert>
#include <ctime>
#include <sys/timeb.h>
#include "head/bnet.h"
#include "head/find_print_candidate_MFFC.h"

using namespace std;

int numPI_ini, numPO_ini;


int main(int argc, char **argv)
{
    struct timeb startTime, endTime;                         //Record the computing time.
    
    if(argc != 5)
    {
        cout << "Correct usage: ./main input_file outfile_name_MFFCs_chosen_info outfile_name_other_parts num_input > log_file " << endl;
        exit(1);
    }
    
    ftime(&startTime);    

    //*************************************//
    //*****Initialize the circuit**********//
    //*************************************//
    cout << endl << "**************************************************************" << endl;
    cout << "Read the Boolean network: " << endl;
    string infile = argv[1]; 
    FILE *fp;
    fp = fopen(infile.c_str(), "r");
    BnetNetwork *net = Bnet_ReadNetwork(fp);
    numPI_ini = net->npis;
    numPO_ini = net->npos;
//    Bnet_PrintNetwork(net);
    fclose(fp);
    
    
    if (net == NULL) {
        cout << "Syntax error in " << infile << endl;
		exit(2);
    }


	//*************************************//
    //*****Get the MFFC and print them*****//
    //*************************************//
    string outfile = argv[2];
    int num_input_min = atoi(argv[3]);    
    int num_input_max = atoi(argv[4]);
    find_print_candidate_MFFC(net, outfile, num_input_min, num_input_max);
    
    ftime(&endTime);
    double runtime_main = ((endTime.time - startTime.time)*1000 + (endTime.millitm - startTime.millitm))/1000.0;
    cout << "total runtime: " << runtime_main << endl;
    
    Bnet_FreeNetwork(net);
	
    return 0;
}
