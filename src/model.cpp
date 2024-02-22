#include "model.hpp" 

Model::Model(int shelf_r, int shelf_c, int gap_r, int gap_c, int num_r, int num_c, int num_pickers) {

    num_actions = 9;

    shelf_size_row  = shelf_r;
    shelf_size_col = shelf_c;

    gap_size_row = gap_r;
    gap_size_col = gap_c;

    num_shelves_row = num_r;
    num_shelves_col = num_c;

    num_pickers = num_pickers;

    // Calculating the grid's required number of rows and columns
    num_rows = num_shelves_row*(gap_size_row + shelf_size_row) + gap_size_row;
    num_cols = num_shelves_col*(gap_size_col + shelf_size_col) + gap_size_col;
    int grid_area = num_rows*num_cols;

    // The total number of cells in the grid that denote shelving 
    int num_shelves = num_shelves_row*num_shelves_col;

    shelf_area = num_shelves * (shelf_size_row * shelf_size_col);
    walkable_area = grid_area - shelf_area;

    // The length of the longest side of a shelf
    int shelf_length = max(shelf_size_row, shelf_size_col); 

    // The number of locations which hold a product
    int num_tickets = 2*shelf_length*num_shelves;

    /* Allocating memory for the grid, transition_table, 
    distances table, ticket locations list and agent locations list.*/
    
    grid = (int**) malloc(num_rows * sizeof(int*));
    for (int i = 0; i < num_rows; i++){
        grid[i] = (int*) malloc(num_cols * sizeof(int));
    }

    transition_table = (int**) malloc(walkable_area * sizeof(int*));
    for (int i = 0; i < walkable_area; i++){
        transition_table[i] = (int*) malloc(num_actions * sizeof(int));
    }

    for (int i = 0; i < (walkable_area); i++){
        for (int j = 0; j < num_actions; j++) {
            transition_table[i][j] = -1;
        }
    }   

    distances_table = (int**) malloc(walkable_area * sizeof(int*));
    for (int i = 0; i < walkable_area; i++){
        distances_table[i] = (int*) malloc(walkable_area * sizeof(int));
    }
    for (int i = 0; i < (walkable_area); i++){
        for (int j = 0; j < walkable_area; j++) {
            distances_table [i][j] = 0;
        }
    }

    tickets = (int*) malloc(num_tickets * sizeof(int));
    for (int i = 0; i < (num_tickets); i++){
        tickets[i] = -1;
    }

    locations = (Location*) malloc(walkable_area * sizeof(Location));
    for (int i = 0; i < (walkable_area); i++){
        locations[i] = Location(-1);
    }

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
    };


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
                for (int action_index = 0; action_index < num_actions; action_index++) {
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
                    tickets[num_tickets_found++] = index;
                } 
            }
        }
    }


};

