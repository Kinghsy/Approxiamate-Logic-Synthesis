#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <climits>
#include <map>
#include <cassert>
#include <ctime>
#include <sys/timeb.h>
#include <vector>
#include "bnet.h"
#include "searchMFFC.h"

using namespace std;

int main(int argc, char **argv)
{
    if(argc != 5)
    {
        cout << "Usage: ./main inFile outFile minInput maxInput" << endl;
        exit(1);
    }
    string infile = argv[1];
    string outfile = argv[2];
    int num_input_min = atoi(argv[3]);
    int num_input_max = atoi(argv[4]);


    struct timeb startTime, endTime;
    //Record the computing time.
    ftime(&startTime);

    //*************************************//
    //*****Initialize the circuit**********//
    //*************************************//
    cout << "**********************************" << endl;
    cout << "Read the Boolean network: " << endl;
    FILE *fp;
    fp = fopen(infile.c_str(), "r");
    BnetNetwork *net = Bnet_ReadNetwork(fp);
    if (net == NULL) {
        cout << "Syntax error in " << infile << endl;
		exit(2);
    }
    Bnet_PrintNetwork(net);
    fclose(fp);

    //*************************************//
    //*****Get the MFFC and print them*****//
    //*************************************//
    stringstream s;
    searchMFFC(net, s, num_input_min, num_input_max);

    ftime(&endTime);
    double runtime_main = ((endTime.time - startTime.time)*1000
                           + (endTime.millitm - startTime.millitm))/1000.0;
    cout << "total runtime: " << runtime_main << endl;
    Bnet_FreeNetwork(net);

    string MFFCStr = s.str();

    cout << MFFCStr;

    string tFile = "temp.blif";

    ofstream ofile(tFile);
    if (!ofile.is_open()) {
        cerr << "Cannot open file." << endl;
        exit(1);
    }
    ofile << MFFCStr;
    ofile.close();

    vector<int> v= getTruthTable(tFile);

    for (auto it = v.cbegin(); it != v.cend(); it++) {
        cout << *it << endl;
    }

    return 0;
}
