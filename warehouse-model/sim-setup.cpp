/**
 * @file sim-setup.cpp
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

int NUM_ACTIONS = 9;    /* The number of neighboring cells (including the current cell)
                               that an agent can move to. */

int main() {

    int shelf_size_row;    // Row and column dimensions of a shelf
    int shelf_size_col;     

    int gap_size_row;      // Row and column distances between shelves
    int gap_size_col;    

    int num_shelves_row;   // The number of shelves along the row and column dimensions
    int num_shelves_col;

    cin >> shelf_size_row;  
    cin >> shelf_size_col;

    cin >> gap_size_row;
    cin >> gap_size_col;

    cin >> num_shelves_row;
    cin >> num_shelves_col; 

    cin.clear();

    // Calculating the required number of rows and columns in the grid
    int num_rows = num_shelves_row*(gap_size_row + shelf_size_row) + gap_size_row;
    int num_cols = num_shelves_col*(gap_size_col + shelf_size_col) + gap_size_col;
    int grid_area = num_rows*num_cols;

    // The total number of cells in the grid that denote shelving 
    int num_shelves = num_shelves_row*num_shelves_col;

    int shelf_area = num_shelves * (shelf_size_row * shelf_size_col);
    int walkable_area = grid_area - shelf_area;

    // The length of the longest side of a shelf
    int shelf_length = max(shelf_size_row, shelf_size_col); 

    // The number of locations which hold a product
    int num_tickets = 2*shelf_length*num_shelves;


    /* The grid has -1s at shelf locations and a unique id for 
       each cell denoting a walkable unit of floor space */
    int grid[num_rows][num_cols]; 

    /* Each row corresponds to a unique cell of walkable floor space 
    in the grid, with the id number of that location in the grid being the 
    index of that row. Each column holds the index of the row (if any) that 
    will be reached if an agent moves in the particular direction, with the 
    first column being "up 1, left 1". A -1 is found if a movement direction 
    is not possible from some location (i.e. a collision with either a shelf 
    or the edge of the environment would occur).*/
    int transition_table[walkable_area][NUM_ACTIONS]; // + num_tickets];
    memset(transition_table, -1, sizeof(int) * walkable_area * NUM_ACTIONS);

    /* Holds the distances between any two cells of walkable floor space in 
    the grid. The cells are indexed according to the unique id used in the grid
    i.e. distances_table[i][j] = 1 would indicate that the location i is adjacent 
    to location j (and vice-versa). */
    int distances_table[walkable_area][walkable_area];

    // Ticket locations list
    int ticket_locations[num_tickets];


    // Filling the grid with -1s where shelves are and a unique id for each
    // location without a shelf (i.e. a walkway).
    int id = 0;
    for (int row = 0; row < num_rows; row++) {
       for (int col = 0; col < num_cols; col++) {

            // Shelves exist at regular intervals just beyond each gap 
            bool shelf_in_row = (row % (gap_size_row + shelf_size_row)) >= gap_size_row;
            bool shelf_in_col = (col % (gap_size_col + shelf_size_col)) >= gap_size_col;
            
            if (shelf_in_row && shelf_in_col) {
                grid[row][col] = -1;
            } else {
                grid[row][col] = id++;
            }
        }
    }

    /* Filling the transition table such that table[i][j] = k indicates 
       that an agent at location i will move to location k if they take action j.
       The nine columns represent the at most eight possible directions in a 
       2d cell system along with the action of not moving. 
       If taking action j while at location i is impossible, then k = -1. */
    for (int row = 0; row < num_rows; row++) {
        for (int col = 0; col < num_cols; col++) {

            if (grid[row][col] >= 0) {
                int move = 0;
                for (int r = row-1; r <= row+1; r++) {
                    for (int c = col-1; c <= col+1; c++) {
                        // Ensuring actions do not lead outside the grid 
                        if ((r >= 0 && r < num_rows) && (c >= 0 && c < num_cols))  {
                            if (grid[r][c] >= 0) {
                                // grid[row][col] returns a unique indexing id 
                                transition_table[grid[row][col]][move] = grid[r][c];
                            }
                        }
                        move++; 
                    }   
                } 
            }
        }
    }

    // Filling the minimum distances table using a BFS.
    for (int source_id = 0; source_id < walkable_area; source_id++) {
        for (int destination_id = source_id; destination_id < walkable_area; destination_id++) { 
        
            queue<int> queue;
            queue.push(source_id);
          
            bool visited[walkable_area];
            memset(visited, false, sizeof(bool)*walkable_area);
            visited[source_id] = true;

            int path_length[walkable_area];
            memset(path_length, 0, sizeof(int)*walkable_area);

            int current_id;

            while (current_id != destination_id) {
                current_id = queue.front();
                queue.pop();
                if (current_id == destination_id) {break;}

                int new_id;
                for (int action_index = 0; action_index < NUM_ACTIONS; action_index++) {
                    new_id = transition_table[current_id][action_index];
                    if (new_id >= 0) {
                        if (!visited[new_id]) {
                            visited[new_id] = true;
                            queue.push(new_id);
                            path_length[new_id] = path_length[current_id] + 1;
                        }
                    }
                }
            }
            distances_table[source_id][destination_id] = path_length[destination_id];
            distances_table[destination_id][source_id] = path_length[destination_id];
        }
    }
    
    
    // Storing all ticket locations
    int num_tickets_found = 0;
    bool lateral = shelf_size_col > shelf_size_row;
    int i_max = (lateral ? num_rows : num_cols) - 1;
    int j_max = (lateral ? num_cols : num_rows) - 1;
    int index;
    bool shelf_behind;
    bool shelf_ahead;

    for (int i = 0; i <= i_max; i++) {
        for (int j = 0; j <= j_max; j++) {
            index = grid[lateral ? i:j][lateral ? j:i];
            if (index >= 0) {
                shelf_behind = grid[lateral ? max(0,i-1) : j][lateral ? j : max(0,i-1)] < 0;
                shelf_ahead = grid[lateral ? min(i_max,i+1) : j][lateral ? j : min(i_max,i+1)] < 0;
                if (shelf_behind || shelf_ahead) {
                    ticket_locations[num_tickets_found++] = index;
                } 
            }
        }
    }
    return 0;
} 