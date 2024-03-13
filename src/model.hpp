/**
 * @file model.hpp
 * @author Joshua Mance
 * @brief Returns four data structures that define the simulation environment. 
 *        These are: (1) a 2d grid of discrete locations (including shelving) in a warehouse,
*                    (2) a transition table encoding all possible movements within the grid,
*                    (3) a matrix storing the distances between all (non-shelving) locations and
*                    (4) a list of the location index numbers for each product. 
 * @version 1
 * @date 2024-02-06
 * @copyright Copyright (c) 2024
 */

#include <iostream>
#include <string.h>
#include <algorithm> 
#include <queue>  

#include "location.hpp"
#include "order.hpp"
#include "picker.hpp"

using namespace std;

class Model {
    public:
        
        int num_actions;    /* The number of neighboring cells (including the current cell)
                                that an agent can move to. */

        int shelf_size_row;    // Row and column dimensions of a shelf
        int shelf_size_col;     

        int gap_size_row;      // Row and column distances between shelves
        int gap_size_col;    

        int num_shelves_row;   // The number of shelves along the row and column dimensions
        int num_shelves_col;

        int num_rows;
        int num_cols;

        int shelf_area;
        int num_locations;

        int num_pickers;

        /* Holds the distances between any two cells of walkable floor space in 
            the grid. The cells are indexed according to the unique id used in the grid
            i.e. distances_table[i][j] = 1 would indicate that the location i is adjacent 
            to location j (and vice-versa). */
        int **distances_table;

        Location *locations;

        Model(int shelf_r, int shelf_c, int gap_r, int gap_c, int num_r, int num_c, int num_pickers);
};

