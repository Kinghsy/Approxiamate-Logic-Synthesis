#ifndef PRINT_MFFC_H
#define PRINT_MFFC_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <climits>
#include <vector>
#include <set>
#include <map>
#include <iostream>
#include "bnet.h"

using namespace std; 

void searchMFFC(BnetNetwork *net, ostream &oufile, int num_input_min, int num_input_max);


#endif
