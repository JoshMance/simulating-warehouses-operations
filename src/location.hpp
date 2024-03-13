#include <stdlib.h>
#pragma once
using namespace std;

class Location {
    
    public:

    int index;
    int num_adj;
    Location **neighbours;

    Location(int index_val);
};