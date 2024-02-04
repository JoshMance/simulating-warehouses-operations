/**
 * @file floorplan-graph.cpp
 * @author Joshua Mance
 * @brief Creates a graph representing discrete locations within a warehouse.
 * @version 0.1
 * @date 2024-01-17
 * @copyright Copyright (c) 2024
 * 
 */

#include <iostream>
#include <string.h>
#include <valarray>     // USed for element-wise operations on arrays
#include <algorithm> 
#include <queue>  
//#include "../matplotlibcpp.h"
using namespace std;


int NUM_ACTIONS = 9;

int SHELF_SIZE_ROW = 1;    // Row and column dimensions of a shelf
int SHELF_SIZE_COL = 2;     

int GAP_SIZE_ROW = 2;      // Row and column distances between shelves
int GAP_SIZE_COL = 1;    

int NUM_SHELVES_ROW = 1; // The number of shelves along any given row and column
int NUM_SHELVES_COL = 1;


class Agent;
class Ticket;
class Location;

class Location {        // Holds each discrete location in a floor plan
    public:             
        bool occupied;
        Ticket* tickets;
        Location* adj_locations;

        Location(){
            occupied = false;
        } 
};

class Agent {          // An employee who picks products from shelves
    public:
        int id;
        Location* location;
};

class Ticket {          // A product that can be picked
    public:
        int number;
};

// Calculating the required row and column dimensions
int num_rows = NUM_SHELVES_ROW*(GAP_SIZE_ROW + SHELF_SIZE_ROW) + GAP_SIZE_ROW;
int num_cols = NUM_SHELVES_COL*(GAP_SIZE_COL + SHELF_SIZE_COL) + GAP_SIZE_COL;
int grid_area = num_rows*num_cols;

int num_shelves = NUM_SHELVES_ROW*NUM_SHELVES_COL;

int shelf_area = num_shelves * (SHELF_SIZE_ROW * SHELF_SIZE_COL);
int walkable_area = grid_area - shelf_area;

int shelf_length = max(SHELF_SIZE_ROW, SHELF_SIZE_COL);  // The length of the longest 
                                                         // side of a shelf
int num_tickets = 2*shelf_length*num_shelves;


int main() {

    int grid[num_rows][num_cols]; // Tables holds -1s at shelf locations 
                                  // and a unique id at walkable floor space 
    int transition_table[walkable_area][NUM_ACTIONS]; // + num_tickets];
    memset(transition_table, -1, sizeof(int) * walkable_area * NUM_ACTIONS);

    int min_distances_table[walkable_area][walkable_area];

    // Filling the grid with -1s where shelves are, and a unique id for each
    // location without a shelf (i.e. a walkway).
    int id = 0;
    for (int row = 0; row < num_rows; row++) {
       for (int col = 0; col < num_cols; col++) {

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

    // Filling the transition table such that table[i][j] = k indicates 
    // that an agent at the location with an id number of i will, if they take
    // action j, move to the location with an id number of k.
    // The nine columns represent the at most eight possible directions in a 2d cell system
    // plus the action of not moving. If action j while at location i is impossible then k = -1.
    for (int row = 0; row < num_rows; row++) {
        for (int col = 0; col < num_cols; col++) {

            if (grid[row][col] >= 0) {
                int move = 0;
                for (int r = row-1; r <= row+1; r++) {
                    for (int c = col-1; c <= col+1; c++) {

                        if ((r >= 0 && r < num_rows) && (c >= 0 && c < num_cols))  {
                            if (grid[r][c] >= 0) {
                                transition_table[grid[row][col]][move] = grid[r][c];
                            }
                        }
                        move++; 
                    }   
                } 
            }
        }
     }

    // Debug this, all above ok.

    // Filling the minimum distances table using a BFS.
    for (int source_id = 0; source_id < walkable_area; source_id++) {
        for (int destination_id = source_id; destination_id < walkable_area; destination_id++) { 
        
            queue<int> queue;
            queue.push(source_id);
          
            bool visited[walkable_area];
            visited[source_id] = true;

            int distance = 0;
            int current_id;

            while (current_id != destination_id) {
                
                current_id = queue.front();
                queue.pop();
                if (current_id == destination_id) {break;}

                for (int action_index = 0; action_index < NUM_ACTIONS; action_index++) {
                    int new_id = transition_table[current_id][action_index];
                    if (new_id >= 0) {
                        if (!visited[new_id]) {
                            visited[new_id] = true;
                            queue.push(new_id);
                        }
                    }
                }
                distance++;
            }
            min_distances_table[source_id][destination_id] = distance;
            min_distances_table[destination_id][source_id] = distance;
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

    for (int i = 0; i < walkable_area; ++i) {
        for (int j = 0; j < NUM_ACTIONS; ++j) {
            cout << transition_table[i][j] << " ";
        }
        cout << endl;
    }  
    cout << endl;

    for (int i = 0; i < walkable_area; ++i) {
        for (int j = 0; j < walkable_area; ++j) {
            cout << min_distances_table[i][j] << " ";
        }
        cout << endl;
    }  
    cout << endl;




    return 0;
} 