#include "location.hpp"

Location::Location(int index_val) {
    index = index_val;
    num_adj = 0;
    neighbours = (Location **) malloc(sizeof(Location *));
};