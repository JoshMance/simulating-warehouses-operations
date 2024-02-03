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
#include <list>
using namespace std;

valarray<int> SHELF_SIZE {1, 1};    // Row and column dimensions of a shelf
valarray<int> GAP_SIZE {1, 1};       // Row and column distances between shelves
valarray<int> NUM_SHELVES {2, 2};   // The number of shelves along any given row and column

class Agent;
class Ticket;
class Location;

class Location {        // Holds each discrete location in a floor plan
    public:             
        bool occupied;
        Ticket* tickets[12];
        Location* adj_locations[4];

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
valarray<int> grid_size = NUM_SHELVES*(GAP_SIZE + SHELF_SIZE) + GAP_SIZE;
int grid_area = grid_size[0]*grid_size[1];

int total_shelf_area = NUM_SHELVES[0]*NUM_SHELVES[1]*SHELF_SIZE[0]*SHELF_SIZE[1];

int num_locations = grid_area - total_shelf_area;

int main() {

    // Creating a grid floor plan. 
    int grid[grid_size[0]][grid_size[1]];

    for (int row = 0; row < grid_size[0]; row++) {
        bool shelf_in_row = (row % (GAP_SIZE[0] + SHELF_SIZE[0])) >= GAP_SIZE[0];
        
        for (int col = 0; col < grid_size[1]; col++) {
             bool shelf_in_col = (col % (GAP_SIZE[1] + SHELF_SIZE[1])) >= GAP_SIZE[1];
            
            if (shelf_in_row && shelf_in_col) {
                Location new_location = Location();
                grid[row][col] = &new_location;
                cout << row << " " << col << endl; 
            }
            else {
                grid[row][col] = nullptr;
            }
        }
    }



    return 0;
} 