#pragma once

#include "Source/Hex2D/Objects/hex_grid_base.h"
#include "Source/Parser/functions.h"

// ###### HEXAGONAL GRID CELL PARSER TOKEN DEFINITIONS ######

#define HEX_GRID_NEIGHBOUR_H0  "H0"
#define HEX_GRID_NEIGHBOUR_H1  "H1"
#define HEX_GRID_NEIGHBOUR_H2  "H2"
#define HEX_GRID_NEIGHBOUR_H3  "H3"
#define HEX_GRID_NEIGHBOUR_H4  "H4"
#define HEX_GRID_NEIGHBOUR_H5  "H5"
#define HEX_GRID_NEIGHBOUR_H6  "H6"

class hex_functions_class : public functions_base_class {
public:
    hex_functions_class() {}
    ~hex_functions_class() {}
    // ################ FUNCTIONS TO RETRIEVE HEXAGONAL GRID CELL VALUES ###############

    template <class T>
    bool get_hex_grid_value(literal_node* dn, hex_grid_base_class<T>* hex_grid, hex_surface_index_data_type hex_index) {
        if (hex_index < 0 || hex_index >= hex_grid->hex_grid.size()) return false; //hex_index is invalid

        std::string hex_data_token = dn->function_token_definition.second;

        hex_surface_index_data_type hex_neighbour_index = -1;
        int hex_neighbour_id = -1;

        if (hex_data_token == HEX_GRID_NEIGHBOUR_H0) { hex_neighbour_id = 0; }
        if (hex_data_token == HEX_GRID_NEIGHBOUR_H1) { hex_neighbour_id = 1; }
        if (hex_data_token == HEX_GRID_NEIGHBOUR_H2) { hex_neighbour_id = 2; }
        if (hex_data_token == HEX_GRID_NEIGHBOUR_H3) { hex_neighbour_id = 3; }
        if (hex_data_token == HEX_GRID_NEIGHBOUR_H4) { hex_neighbour_id = 4; }
        if (hex_data_token == HEX_GRID_NEIGHBOUR_H5) { hex_neighbour_id = 5; }
        if (hex_data_token == HEX_GRID_NEIGHBOUR_H6) { hex_neighbour_id = 6; }

        if (hex_neighbour_id < 0) { return false; } //have invalid hex neighbour token

        hex_neighbour_index = get_hex_neighbour_index(hex_grid, hex_index, hex_neighbour_id);

        if (hex_neighbour_index < 0 || hex_neighbour_index > hex_grid->hex_grid.size() - 1) { // Neighbour is outside of hex grid bounds and does not exist 
            dn->value.first = hex_grid_value_data_type_enum::nan;// This is to indicate that the neighbour is to be ignored as it does not exist
            return true;
        }

        dn->value.first = hex_grid->hex_grid_value_data_type;

        //printf("functions :: assign_hex_grid_value $$$$$$$$$$$$$ : %i : %i\n", hex_index, hex_neighbour_index);
        switch (hex_grid->hex_grid_value_data_type) {
        case hex_grid_value_data_type_enum::Boolean: dn->value.second.bvalue = hex_grid->hex_grid[hex_neighbour_index]; break;
        case hex_grid_value_data_type_enum::Float:   dn->value.second.fvalue = hex_grid->hex_grid[hex_neighbour_index]; break;
        case hex_grid_value_data_type_enum::Integer: dn->value.second.ivalue = hex_grid->hex_grid[hex_neighbour_index]; break;
        case hex_grid_value_data_type_enum::Double:  dn->value.second.dvalue = hex_grid->hex_grid[hex_neighbour_index]; break;

            // for testing :: Toggle this with above case statements when not testing
            //case hex_grid_value_data_type_enum::Boolean: dn->value.second.bvalue = hex_grid->hex_grid[hex_neighbour_index];printf("Boolean:%i\n", hex_grid->hex_grid[hex_neighbour_index]); break;
            //case hex_grid_value_data_type_enum::Float:   dn->value.second.fvalue = hex_grid->hex_grid[hex_neighbour_index]; printf("Float:  %f\n", hex_grid->hex_grid[hex_neighbour_index]); break;
            //case hex_grid_value_data_type_enum::Integer: dn->value.second.ivalue = hex_grid->hex_grid[hex_neighbour_index]; printf("Integer:%i\n", hex_grid->hex_grid[hex_neighbour_index]); break;
            //case hex_grid_value_data_type_enum::Double:  dn->value.second.dvalue = hex_grid->hex_grid[hex_neighbour_index]; printf("Double: %d\n", hex_grid->hex_grid[hex_neighbour_index]); break;
        }

        return true;
    }

    hex_surface_vec_data_type get_hex_surface_neighbour_index_coordinates(size_t neighbour, int x_index, int y_index) {
        hex_surface_vec_data_type neighbor_index_coordinate;
        if (y_index % 2 == 0) { // Even Y 
            switch (neighbour) {
                case 1: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index + 1;  break;
                case 2: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index;      break;
                case 3: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index - 1;  break;
                case 4: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index - 1;  break;
                case 5: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index;      break;
                case 6: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index + 1;  break;
            }
        }
        else {// Odd Y 
            switch (neighbour) {
                case 1: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index + 1; break;
                case 2: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index;     break;
                case 3: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index - 1; break;
                case 4: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index - 1; break;
                case 5: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index;     break;
                case 6: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index + 1;  break;
            }
        }
        return neighbor_index_coordinate;
    }

    template <class T>
    hex_surface_index_data_type get_hex_neighbour_index(hex_grid_base_class<T>* hex_grid, hex_surface_index_data_type hex_index, int neighbour_id) {
        if (neighbour_id == 0) { return hex_index; }// hex grid cell self value;

        hex_surface_vec_data_type hex_surface_coordinate = hex_grid->get_matrix_coordinate(hex_index);
//printf("functions :: get_hex_neighbour_index ######### 00000: %i : %i : %i\n", hex_surface_coordinate.x, hex_surface_coordinate.y, hex_index);
        hex_surface_vec_data_type hex_neighbour_coordinate = get_hex_surface_neighbour_index_coordinates(neighbour_id, hex_surface_coordinate.x, hex_surface_coordinate.y);

//printf("functions :: get_hex_neighbour_index ######### 11111: %i : %i : %i\n", hex_neighbour_coordinate.x, hex_neighbour_coordinate.y, hex_index);

        if (hex_neighbour_coordinate.y < 0 || hex_neighbour_coordinate.y >= hex_grid->grid_dimension.y) { return -1; }

        if (hex_neighbour_coordinate.y % 2 == 0) { // Even Y
            if (hex_neighbour_coordinate.x < 0 || hex_neighbour_coordinate.x >= hex_grid->grid_dimension.x) { return -1; }
        }
        else {// Odd Y
            if (hex_neighbour_coordinate.x < 0 || hex_neighbour_coordinate.x >= hex_grid->grid_dimension.x - 1) { return -1; }
        }

        hex_surface_index_data_type hex_neighbour_index = hex_grid->get_index_value(hex_neighbour_coordinate.x, hex_neighbour_coordinate.y, 0);

// printf("functions :: get_hex_neighbour_index ######### : %i : %i : %i\n", hex_neighbour_coordinate.x, hex_neighbour_coordinate.y, hex_neighbour_index);

        return hex_neighbour_index;
    }

private:
};


