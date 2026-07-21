#pragma once

#include "Source/Cart2D/Objects/cart2D_grid_base.h"
#include "Source/Parser/functions.h"

// ###### HEXAGONAL GRID CELL PARSER TOKEN DEFINITIONS ######

#define CART2D_GRID_NEIGHBOUR_N0  "N0"
#define CART2D_GRID_NEIGHBOUR_N1  "N1"
#define CART2D_GRID_NEIGHBOUR_N2  "N2"
#define CART2D_GRID_NEIGHBOUR_N3  "N3"
#define CART2D_GRID_NEIGHBOUR_N4  "N4"
#define CART2D_GRID_NEIGHBOUR_N5  "N5"
#define CART2D_GRID_NEIGHBOUR_N6  "N6"
#define CART2D_GRID_NEIGHBOUR_N7  "N7"
#define CART2D_GRID_NEIGHBOUR_N8  "N8"

class cart2D_functions_class : public functions_base_class {
public:
    cart2D_functions_class() {}
    ~cart2D_functions_class() {}
    // ################ FUNCTIONS TO RETRIEVE HEXAGONAL GRID CELL VALUES ###############

    template <class T>
    bool get_cart2D_grid_value(literal_node* dn, cart2D_grid_base_class<T>* cart2D_grid, cart2D_surface_index_data_type cart2D_index) {
        if (cart2D_index < 0 || cart2D_index >= cart2D_grid->cart2D_grid.size()) return false; //cart2D_index is invalid

        std::string cart2D_data_token = dn->function_token_definition.second;

        cart2D_surface_index_data_type cart2D_neighbour_index = -1;
        int cart2D_neighbour_id = -1;

        if (cart2D_data_token == CART2D_GRID_NEIGHBOUR_N0) { cart2D_neighbour_id = 0; }
        if (cart2D_data_token == CART2D_GRID_NEIGHBOUR_N1) { cart2D_neighbour_id = 1; }
        if (cart2D_data_token == CART2D_GRID_NEIGHBOUR_N2) { cart2D_neighbour_id = 2; }
        if (cart2D_data_token == CART2D_GRID_NEIGHBOUR_N3) { cart2D_neighbour_id = 3; }
        if (cart2D_data_token == CART2D_GRID_NEIGHBOUR_N4) { cart2D_neighbour_id = 4; }
        if (cart2D_data_token == CART2D_GRID_NEIGHBOUR_N5) { cart2D_neighbour_id = 5; }
        if (cart2D_data_token == CART2D_GRID_NEIGHBOUR_N6) { cart2D_neighbour_id = 6; }
        if (cart2D_data_token == CART2D_GRID_NEIGHBOUR_N7) { cart2D_neighbour_id = 7; }
        if (cart2D_data_token == CART2D_GRID_NEIGHBOUR_N8) { cart2D_neighbour_id = 8; }

        if (cart2D_neighbour_id < 0) { return false; } //have invalid hex neighbour token

        cart2D_neighbour_index = get_cart2D_neighbour_index(cart2D_grid, cart2D_index, cart2D_neighbour_id);

        if (cart2D_neighbour_index < 0 || cart2D_neighbour_index > cart2D_grid->cart2D_grid.size() - 1) { // Neighbour is outside of hex grid bounds and does not exist 
            dn->value.first = cart2D_grid_value_data_type_enum::nan;// This is to indicate that the neighbour is to be ignored as it does not exist
            return true;
        }

        dn->value.first = cart2D_grid->cart2D_grid_value_data_type;

        //printf("functions :: assign_cart2D_grid_value $$$$$$$$$$$$$ : %i : %i\n", cart2D_index, cart2D_neighbour_index);
        switch (cart2D_grid->cart2D_grid_value_data_type) {
        case cart2D_grid_value_data_type_enum::Boolean: dn->value.second.bvalue = cart2D_grid->cart2D_grid[cart2D_neighbour_index]; break;
        case cart2D_grid_value_data_type_enum::Float:   dn->value.second.fvalue = cart2D_grid->cart2D_grid[cart2D_neighbour_index]; break;
        case cart2D_grid_value_data_type_enum::Integer: dn->value.second.ivalue = cart2D_grid->cart2D_grid[cart2D_neighbour_index]; break;
        case cart2D_grid_value_data_type_enum::Double:  dn->value.second.dvalue = cart2D_grid->cart2D_grid[cart2D_neighbour_index]; break;

            // for testing :: Toggle this with above case statements when not testing
            //case cart2D_grid_value_data_type_enum::Boolean: dn->value.second.bvalue = cart2D_grid->cart2D_grid[cart2D_neighbour_index];printf("Boolean:%i\n", cart2D_grid->cart2D_grid[cart2D_neighbour_index]); break;
            //case cart2D_grid_value_data_type_enum::Float:   dn->value.second.fvalue = cart2D_grid->cart2D_grid[cart2D_neighbour_index]; printf("Float:  %f\n", cart2D_grid->cart2D_grid[cart2D_neighbour_index]); break;
            //case cart2D_grid_value_data_type_enum::Integer: dn->value.second.ivalue = cart2D_grid->cart2D_grid[cart2D_neighbour_index]; printf("Integer:%i\n", cart2D_grid->cart2D_grid[cart2D_neighbour_index]); break;
            //case cart2D_grid_value_data_type_enum::Double:  dn->value.second.dvalue = cart2D_grid->cart2D_grid[cart2D_neighbour_index]; printf("Double: %d\n", cart2D_grid->cart2D_grid[cart2D_neighbour_index]); break;
        }

        return true;
    }

    cart2D_surface_vec_data_type get_cart2D_surface_neighbour_index_coordinates(size_t neighbour, int x_index, int y_index) {
        cart2D_surface_vec_data_type neighbor_index_coordinate;
        //if (y_index % 2 == 0) { // Even Y 
            switch (neighbour) {
                case 1: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index + 1; break;
                case 2: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index + 1; break;
                case 3: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index ;    break;
                case 4: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index - 1; break;
                case 5: neighbor_index_coordinate.x = x_index ;    neighbor_index_coordinate.y = y_index - 1; break;
                case 6: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index - 1; break;

                case 7: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index;     break;
                case 8: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index + 1; break;
            }
        //}
        //else {// Odd Y 
        //    switch (neighbour) {
        //        case 1: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index + 1; break;
        //        case 2: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index;     break;
        //        case 3: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index - 1; break;
        //        case 4: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index - 1; break;
        //        case 5: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index;     break;
        //        case 6: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index + 1;  break;
        //    }
        //}
        return neighbor_index_coordinate;
    }

    template <class T>
    cart2D_surface_index_data_type get_cart2D_neighbour_index(cart2D_grid_base_class<T>* cart2D_grid, cart2D_surface_index_data_type cart2D_index, int neighbour_id) {
        if (neighbour_id == 0) { return cart2D_index; }// hex grid cell self value;

        cart2D_surface_vec_data_type cart2D_surface_coordinate = cart2D_grid->get_matrix_coordinate(cart2D_index);
//printf("functions :: get_cart2D_neighbour_index ######### 00000: %i : %i : %i\n", cart2D_surface_coordinate.x, cart2D_surface_coordinate.y, cart2D_index);
        cart2D_surface_vec_data_type cart2D_neighbour_coordinate = get_cart2D_surface_neighbour_index_coordinates(neighbour_id, cart2D_surface_coordinate.x, cart2D_surface_coordinate.y);

//printf("functions :: get_cart2D_neighbour_index ######### 11111: %i : %i : %i\n", cart2D_neighbour_coordinate.x, cart2D_neighbour_coordinate.y, cart2D_index);

        if (cart2D_neighbour_coordinate.y < 0 || cart2D_neighbour_coordinate.y >= cart2D_grid->grid_dimension.y) { return -1; }

        if (cart2D_neighbour_coordinate.y % 2 == 0) { // Even Y
            if (cart2D_neighbour_coordinate.x < 0 || cart2D_neighbour_coordinate.x >= cart2D_grid->grid_dimension.x) { return -1; }
        }
        else {// Odd Y
            if (cart2D_neighbour_coordinate.x < 0 || cart2D_neighbour_coordinate.x >= cart2D_grid->grid_dimension.x - 1) { return -1; }
        }

        cart2D_surface_index_data_type cart2D_neighbour_index = cart2D_grid->get_index_value(cart2D_neighbour_coordinate.x, cart2D_neighbour_coordinate.y, 0);

// printf("functions :: get_cart2D_neighbour_index ######### : %i : %i : %i\n", cart2D_neighbour_coordinate.x, cart2D_neighbour_coordinate.y, cart2D_neighbour_index);

        return cart2D_neighbour_index;
    }

private:
};


