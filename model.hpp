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
using namespace std;

class Model {
    public:
        
        int NUM_ACTIONS = 9;    /* The number of neighboring cells (including the current cell)
                                that an agent can move to. */

        int shelf_size_row;    // Row and column dimensions of a shelf
        int shelf_size_col;     

        int gap_size_row;      // Row and column distances between shelves
        int gap_size_col;    

        int num_shelves_row;   // The number of shelves along the row and column dimensions
        int num_shelves_col;

        int num_agents;

        /* Each row corresponds to a unique cell of walkable floor space 
            in the grid, with the id number of that location in the grid being the 
            index of that row. Each column holds the index of the row (if any) that 
            will be reached if an agent moves in the particular direction, with the 
            first column being "up 1, left 1". A -1 is found if a movement direction 
            is not possible from some location (i.e. a collision with either a shelf 
            or the edge of the environment would occur).*/
        int transition_table[0][0]; 

        /* Holds the distances between any two cells of walkable floor space in 
            the grid. The cells are indexed according to the unique id used in the grid
            i.e. distances_table[i][j] = 1 would indicate that the location i is adjacent 
            to location j (and vice-versa). */
        int distances_table[0][0];

        /* Ticket locations list. The i^th index having value v indicates that
            that ticket number i is can be found at the location with an index of v.*/
        int ticket_locations[0]; 

        /* The j^th index having value u indicates that the j^th location has the agent 
           with an id of v there. A value of -1 indicates that a location is empty. */
        int agents[0];

        Model(int shelf_r, int shelf_c, int gap_r, int gap_c, int num_r, int num_c, int num_agents);
};

