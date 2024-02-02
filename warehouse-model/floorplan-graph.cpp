/**
 * @file floorplan-graph.cpp
 * @author Joshua Mance
 * @brief Creates a graph representing discrete locations within a warehouse.
 * @version 0.1
 * @date 2024-01-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <iostream>
#include <string.h>
#include <valarray>     // USed for element-wise operations on arrays
#include <list>
using namespace std;

class Agent;
class Ticket;
class Location;

class Location {        // Holds each discrete location in a floor plan
    public:             // Access specifier
        int row;
        int col;
        Ticket* tickets[12];
        Location* adj_locations[4];

        Location(){} 

        Location(int row, int col) {
            row = row;
            col = col;

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
        Location* locations[12];
};

valarray<int> SHELF_SIZE {1, 1};    // Row and column dimensions of a shelf
valarray<int> GAP_SIZE {1, 1};       // Row and column distances between shelves
valarray<int> NUM_SHELVES {2, 2};   // The number of shelves along any given row and column

// Calculating the required row and column dimensions
valarray<int> GRID_SIZE = NUM_SHELVES*(GAP_SIZE + SHELF_SIZE) + GAP_SIZE;
int GRID_AREA = GRID_SIZE[0]*GRID_SIZE[1];

int SHELF_AREA = NUM_SHELVES[0]*NUM_SHELVES[1]*SHELF_SIZE[0]*SHELF_SIZE[1];
int NUM_LOCATIONS = GRID_AREA - SHELF_AREA;

list<Ticket> tickets;
list<Location> locations;

int main() {

    // Creating a grid floor plan. Putting 1s at shelf locations and 0s elsewhere.
    int grid[GRID_SIZE[0]][GAP_SIZE[1]];
    memset(grid, 0, sizeof(grid));

    for (int row = 0; row < GRID_SIZE[0]; row++) {
        bool shelf_in_row = (row % (GAP_SIZE[0] + SHELF_SIZE[0])) >= GAP_SIZE[0];
        
        for (int col = 0; col < GRID_SIZE[1]; col++) {
             bool shelf_in_col = (col % (GAP_SIZE[1] + SHELF_SIZE[1])) >= GAP_SIZE[1];
            
            if (shelf_in_row && shelf_in_col) {
             
                grid[row][col] = 1;
    
            }
        }
    }

    // Running loop again - fix this inefficiency moving forward
    for (int row = 0; row < GRID_SIZE[0]; row++) {
        bool shelf_in_row = (row % (GAP_SIZE[0] + SHELF_SIZE[0])) >= GAP_SIZE[0];
        
        for (int col = 0; col < GRID_SIZE[1]; col++) {
             bool shelf_in_col = (col % (GAP_SIZE[1] + SHELF_SIZE[1])) >= GAP_SIZE[1];
            
            if (shelf_in_row && shelf_in_col) {
             
                Location new_location;
                new_location.row = row;
                new_location.col = col;
                locations.push_back(new_location);

            }
        }
    }

    // Checking all locations
    for (list<Location>::iterator it = locations.begin(); it != locations.end(); ++it){
        cout << it->row << " " << it->col << endl;
    }


    return 0;
} 