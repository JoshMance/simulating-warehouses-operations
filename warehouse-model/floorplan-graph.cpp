/**
 * @file floorplan-graph.cpp
 * @author Joshua Mance
 * @brief Creates 2d grid representing discrete locations a given warehouse, a transition table that encodes
 *        all possible movements within this grid and a matrix storing the distances between all locations.
 * @version 1
 * @date 2024-02-05
 * @copyright Copyright (c) 2024
 */

#include <iostream>
#include <string.h>
#include <algorithm> 
#include <queue>  
//#include "../matplotlibcpp.h"
using namespace std;


int NUM_ACTIONS = 9;       // The number of neighboring cells (including the current cell)
                           // that an agent can move to.

int SHELF_SIZE_ROW = 1;    // Row and column dimensions of a shelf
int SHELF_SIZE_COL = 2;     

int GAP_SIZE_ROW = 2;      // Row and column distances between shelves
int GAP_SIZE_COL = 1;    

int NUM_SHELVES_ROW = 1;   // The number of shelves along the row and column dimensions
int NUM_SHELVES_COL = 1;


// Calculating the required number of rows and columns in the grid
int num_rows = NUM_SHELVES_ROW*(GAP_SIZE_ROW + SHELF_SIZE_ROW) + GAP_SIZE_ROW;
int num_cols = NUM_SHELVES_COL*(GAP_SIZE_COL + SHELF_SIZE_COL) + GAP_SIZE_COL;
int grid_area = num_rows*num_cols;

// The total number of cells in the grid that denote shelving 
int num_shelves = NUM_SHELVES_ROW*NUM_SHELVES_COL;

int shelf_area = num_shelves * (SHELF_SIZE_ROW * SHELF_SIZE_COL);
int walkable_area = grid_area - shelf_area;

// The length of the longest side of a shelf
int shelf_length = max(SHELF_SIZE_ROW, SHELF_SIZE_COL); 

//int num_tickets = 2*shelf_length*num_shelves;


int main() {

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

    // Filling the grid with -1s where shelves are and a unique id for each
    // location without a shelf (i.e. a walkway).
    int id = 0;
    for (int row = 0; row < num_rows; row++) {
       for (int col = 0; col < num_cols; col++) {

            // Shelves exist at regular intervals just beyond each gap 
            bool shelf_in_row = (row % (GAP_SIZE_ROW + SHELF_SIZE_ROW)) >= GAP_SIZE_ROW;
            bool shelf_in_col = (col % (GAP_SIZE_COL + SHELF_SIZE_COL)) >= GAP_SIZE_COL;
            
            if (shelf_in_row && shelf_in_col) {
                grid[row][col] = -1;
            }
            else {
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
    


    // Displaying the grid and transition matrix.
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }  
    cout << endl;

    // for (int i = 0; i < walkable_area; ++i) {
    //     for (int j = 0; j < NUM_ACTIONS; ++j) {
    //         cout << transition_table[i][j] << " ";
    //     }
    //     cout << endl;
    // }  
    // cout << endl;

    cout << "     ";
    for (int i = 0; i < walkable_area; ++i) {
        cout << i;
        if (i < 10) {
            cout << "   ";
        }
        else {
            cout << "  ";
        }
    }
    cout << endl << endl;

    for (int i = 0; i < walkable_area; ++i) {
        cout << i;
        if (i < 10) {
            cout << "    ";
        }
        else {
            cout << "   ";
        }
        for (int j = 0; j < walkable_area; ++j) {
            cout << distances_table[i][j] << "   ";
        }
        cout << endl;
    }  
    cout << endl;




    return 0;
} 