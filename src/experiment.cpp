#include "model.hpp"

int main() {

    int shelf_size_row = 7;
    int shelf_size_col = 2;
    int gap_size_row = 2;
    int gap_size_col = 3;
    int num_shelves_row = 4;
    int num_shelves_col = 10;
    int num_agents = 5;

    Model model(shelf_size_row, shelf_size_col, gap_size_row, gap_size_col,
                num_shelves_row, num_shelves_col, num_agents);


    cout << model.num_rows << "," << model.num_cols << ",";
    for(int i = 0; i < model.num_rows; i++) {
        for(int j = 0; j < model.num_cols; j++) {
            cout << model.grid[i][j] << ",";
        }
    }  
    cout << endl;    



   cout << model.walkable_area << "," << model.num_actions << ",";
    for(int i = 0; i < model.walkable_area; i++) {
        for(int j = 0; j < model.num_actions; j++) {
            cout << model.transition_table[i][j] << ",";
        }
    }  
    cout << endl;

    cout << model.walkable_area << "," << model.walkable_area << ",";
    for(int i = 0; i < model.walkable_area; i++) {
        for(int j = 0; j < model.walkable_area; j++) {
            cout << model.distances_table[i][j] << ",";
        }
    }  
    cout << endl;



  
    return 0;
}
