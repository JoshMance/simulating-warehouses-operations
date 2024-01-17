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
#include <valarray> // USed for element-wise operations on arrays
using namespace std;


valarray<int> SHELF_SIZE {2, 2};    // Row and column dimensions of a shelf
valarray<int> GAP_SIZE {1, 2};       // Row and column distances between shelves
valarray<int> NUM_SHELVES {3, 2};   // The number of shelves along any given row and column

// Calculating the required row and column dimensions
valarray<int> GRID_SIZE = NUM_SHELVES*(GAP_SIZE + SHELF_SIZE) + GAP_SIZE;

int GRID_AREA = GRID_SIZE[0]*GRID_SIZE[1];

int main() {

    // Creating a grid floor plan and putting 1s at shelf locations and 
    // 0s elsewhere
    int grid[GRID_SIZE[0]][GAP_SIZE[1]];
    memset(grid, 0, sizeof(grid));

    for (int row = 0; row < GRID_SIZE[0]; row++) {
        bool shelf_in_row = (row % (GAP_SIZE[0] + SHELF_SIZE[0])) >= GAP_SIZE[0];

        for (int col = 0; col < GRID_SIZE[1]; col++) {
             bool shelf_in_col = (col % (GAP_SIZE[1] + SHELF_SIZE[1])) >= GAP_SIZE[1];
            
            if (shelf_in_row && shelf_in_col) {
                cout << "here!" << row << " " << col << endl;
                grid[row][col] = 1;
            }
        }
    }

    cout << "Initialization Complete"; 
    return 0;
} 