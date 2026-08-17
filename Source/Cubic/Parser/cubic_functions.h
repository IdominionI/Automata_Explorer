#pragma once

#include "Source/Cubic/Cubic_Grids/cubic_grid_base.h"
#include "Source/Parser/functions.h"

// ###### HEXAGONAL GRID CELL PARSER TOKEN DEFINITIONS ######

#define CUBIC_GRID_NEIGHBOUR_C0  "C0"
#define CUBIC_GRID_NEIGHBOUR_C1  "C1"
#define CUBIC_GRID_NEIGHBOUR_C2  "C2"
#define CUBIC_GRID_NEIGHBOUR_C3  "C3"
#define CUBIC_GRID_NEIGHBOUR_C4  "C4"
#define CUBIC_GRID_NEIGHBOUR_C5  "C5"
#define CUBIC_GRID_NEIGHBOUR_C6  "C6"
#define CUBIC_GRID_NEIGHBOUR_C7  "C7"
#define CUBIC_GRID_NEIGHBOUR_C8  "C8"
#define CUBIC_GRID_NEIGHBOUR_C9  "C9"
#define CUBIC_GRID_NEIGHBOUR_C10 "C10"
#define CUBIC_GRID_NEIGHBOUR_C11 "C11"
#define CUBIC_GRID_NEIGHBOUR_C12 "C12"
#define CUBIC_GRID_NEIGHBOUR_C13 "C13"
#define CUBIC_GRID_NEIGHBOUR_C14 "C14"
#define CUBIC_GRID_NEIGHBOUR_C15 "C15"
#define CUBIC_GRID_NEIGHBOUR_C16 "C16"
#define CUBIC_GRID_NEIGHBOUR_C17 "C17"
#define CUBIC_GRID_NEIGHBOUR_C18 "C18"
#define CUBIC_GRID_NEIGHBOUR_C19 "C19"
#define CUBIC_GRID_NEIGHBOUR_C20 "C20"
#define CUBIC_GRID_NEIGHBOUR_C21 "C21"
#define CUBIC_GRID_NEIGHBOUR_C22 "C22"
#define CUBIC_GRID_NEIGHBOUR_C23 "C23"
#define CUBIC_GRID_NEIGHBOUR_C24 "C24"
#define CUBIC_GRID_NEIGHBOUR_C25 "C25"
#define CUBIC_GRID_NEIGHBOUR_C26 "C26"
#define CUBIC_GRID_NEIGHBOUR_C27 "C27"


class cubic_functions_class : public functions_base_class {
public:
    cubic_functions_class() {}
    ~cubic_functions_class() {}
    // ################ FUNCTIONS TO RETRIEVE HEXAGONAL GRID CELL VALUES ###############

    template <class T>
    bool get_cubic_grid_value(literal_node* dn, cubic_grid_base_class<T>* cubic_grid, cubic_surface_index_data_type cubic_index) {
        if (cubic_index < 0 || cubic_index >= cubic_grid->cubic_grid.size()) return false; //cubic_index is invalid

        std::string cubic_data_token = dn->function_token_definition.second;

        cubic_surface_index_data_type cubic_neighbour_index = -1;
        int cubic_neighbour_id = -1;

        if (cubic_data_token == CUBIC_GRID_NEIGHBOUR_C0)  { cubic_neighbour_id = 0; }
        if (cubic_data_token == CUBIC_GRID_NEIGHBOUR_C1)  { cubic_neighbour_id = 1; }
        if (cubic_data_token == CUBIC_GRID_NEIGHBOUR_C2)  { cubic_neighbour_id = 2; }
        if (cubic_data_token == CUBIC_GRID_NEIGHBOUR_C3)  { cubic_neighbour_id = 3; }
        if (cubic_data_token == CUBIC_GRID_NEIGHBOUR_C4)  { cubic_neighbour_id = 4; }
        if (cubic_data_token == CUBIC_GRID_NEIGHBOUR_C5)  { cubic_neighbour_id = 5; }
        if (cubic_data_token == CUBIC_GRID_NEIGHBOUR_C6)  { cubic_neighbour_id = 6; }
        if (cubic_data_token == CUBIC_GRID_NEIGHBOUR_C7)  { cubic_neighbour_id = 7; }
        if (cubic_data_token == CUBIC_GRID_NEIGHBOUR_C8)  { cubic_neighbour_id = 8; }
        if (cubic_data_token == CUBIC_GRID_NEIGHBOUR_C9)  { cubic_neighbour_id = 9; }
        if (cubic_data_token == CUBIC_GRID_NEIGHBOUR_C10) { cubic_neighbour_id = 10; }
        if (cubic_data_token == CUBIC_GRID_NEIGHBOUR_C11) { cubic_neighbour_id = 11; }
        if (cubic_data_token == CUBIC_GRID_NEIGHBOUR_C12) { cubic_neighbour_id = 12; }
		if (cubic_data_token == CUBIC_GRID_NEIGHBOUR_C13) { cubic_neighbour_id = 13; }
		if (cubic_data_token == CUBIC_GRID_NEIGHBOUR_C14) { cubic_neighbour_id = 14; }
		if (cubic_data_token == CUBIC_GRID_NEIGHBOUR_C15) { cubic_neighbour_id = 15; }
		if (cubic_data_token == CUBIC_GRID_NEIGHBOUR_C16) { cubic_neighbour_id = 16; }
		if (cubic_data_token == CUBIC_GRID_NEIGHBOUR_C17) { cubic_neighbour_id = 17; }
		if (cubic_data_token == CUBIC_GRID_NEIGHBOUR_C18) { cubic_neighbour_id = 18; }
		if (cubic_data_token == CUBIC_GRID_NEIGHBOUR_C19) { cubic_neighbour_id = 19; }
		if (cubic_data_token == CUBIC_GRID_NEIGHBOUR_C20) { cubic_neighbour_id = 20; }
		if (cubic_data_token == CUBIC_GRID_NEIGHBOUR_C21) { cubic_neighbour_id = 21; }
		if (cubic_data_token == CUBIC_GRID_NEIGHBOUR_C22) { cubic_neighbour_id = 22; }
		if (cubic_data_token == CUBIC_GRID_NEIGHBOUR_C23) { cubic_neighbour_id = 23; }
		if (cubic_data_token == CUBIC_GRID_NEIGHBOUR_C24) { cubic_neighbour_id = 24; }
		if (cubic_data_token == CUBIC_GRID_NEIGHBOUR_C25) { cubic_neighbour_id = 25; }
		if (cubic_data_token == CUBIC_GRID_NEIGHBOUR_C26) { cubic_neighbour_id = 26; }
		if (cubic_data_token == CUBIC_GRID_NEIGHBOUR_C27) { cubic_neighbour_id = 27; }

        if (cubic_neighbour_id < 0) { return false; } //have invalid hex neighbour token

        cubic_neighbour_index = get_cubic_neighbour_index(cubic_grid, cubic_index, cubic_neighbour_id);

        if (cubic_neighbour_index < 0 || cubic_neighbour_index > cubic_grid->cubic_grid.size() - 1) { // Neighbour is outside of hex grid bounds and does not exist 
            dn->value.first = cubic_grid_value_data_type_enum::nan;// This is to indicate that the neighbour is to be ignored as it does not exist
            return true;
        }

        dn->value.first = cubic_grid->cubic_grid_value_data_type;

        //printf("functions :: assign_cubic_grid_value $$$$$$$$$$$$$ : %i : %i\n", cubic_index, cubic_neighbour_index);
        switch (cubic_grid->cubic_grid_value_data_type) {
        case cubic_grid_value_data_type_enum::Boolean: dn->value.second.bvalue = cubic_grid->cubic_grid[cubic_neighbour_index]; break;
        case cubic_grid_value_data_type_enum::Float:   dn->value.second.fvalue = cubic_grid->cubic_grid[cubic_neighbour_index]; break;
        case cubic_grid_value_data_type_enum::Integer: dn->value.second.ivalue = cubic_grid->cubic_grid[cubic_neighbour_index]; break;
        case cubic_grid_value_data_type_enum::Double:  dn->value.second.dvalue = cubic_grid->cubic_grid[cubic_neighbour_index]; break;

            // for testing :: Toggle this with above case statements when not testing
            //case cubic_grid_value_data_type_enum::Boolean: dn->value.second.bvalue = cubic_grid->cubic_grid[cubic_neighbour_index];printf("Boolean:%i\n", cubic_grid->cubic_grid[cubic_neighbour_index]); break;
            //case cubic_grid_value_data_type_enum::Float:   dn->value.second.fvalue = cubic_grid->cubic_grid[cubic_neighbour_index]; printf("Float:  %f\n", cubic_grid->cubic_grid[cubic_neighbour_index]); break;
            //case cubic_grid_value_data_type_enum::Integer: dn->value.second.ivalue = cubic_grid->cubic_grid[cubic_neighbour_index]; printf("Integer:%i\n", cubic_grid->cubic_grid[cubic_neighbour_index]); break;
            //case cubic_grid_value_data_type_enum::Double:  dn->value.second.dvalue = cubic_grid->cubic_grid[cubic_neighbour_index]; printf("Double: %d\n", cubic_grid->cubic_grid[cubic_neighbour_index]); break;
        }

        return true;
    }

    cubic_surface_vec_data_type get_cubic_surface_neighbour_index_coordinates(size_t neighbour, int x_index, int y_index, int z_index) {
		glm::ivec3 neighbor_index_coordinate = { x_index, y_index, z_index };

		if (neighbour == 0) { // Get grid value of the H0 cubic voxel
			return neighbor_index_coordinate;
		}

		switch (neighbour) {
			case 1: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index + 1; break;
			case 2: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index + 1; neighbor_index_coordinate.z = z_index + 1; break;
			case 3: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index + 1; neighbor_index_coordinate.z = z_index + 1; break;
			case 4: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index;	  neighbor_index_coordinate.z = z_index + 1; break;
			case 5: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index + 1; break;
			case 6: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index + 1; break;
			case 7: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index + 1; break;
			case 8: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index + 1; break;
			case 9: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index + 1; neighbor_index_coordinate.z = z_index + 1; break;
																																			  
			case 10: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index + 1; neighbor_index_coordinate.z = z_index; break;
			case 11: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index + 1; neighbor_index_coordinate.z = z_index; break;
			case 12: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index;	   neighbor_index_coordinate.z = z_index; break;
			case 13: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index; break;
			case 14: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index; break;
			case 15: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index; break;
			case 16: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index    ; neighbor_index_coordinate.z = z_index; break;
			case 17: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index + 1; neighbor_index_coordinate.z = z_index; break;
			
			case 18: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index - 1; break;
			case 19: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index + 1; neighbor_index_coordinate.z = z_index - 1; break;
			case 20: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index + 1; neighbor_index_coordinate.z = z_index - 1; break;
			case 21: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index;	   neighbor_index_coordinate.z = z_index - 1; break;
			case 22: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index - 1; break;
			case 23: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index - 1; break;
			case 24: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index - 1; break;
			case 25: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index - 1; break;
			case 26: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index + 1; neighbor_index_coordinate.z = z_index - 1; break;
		}

		return neighbor_index_coordinate;
    }

	// Determine if cubic voxel cell index coordinate voxel_index_coordinate is valid or not
	// Ie if it is outside the volume and limits of the cubic voxel matrix
	template <class T>
	bool valid_index_coordinate(cubic_grid_base_class<T>* cubic_grid, glm::ivec3 voxel_index_coordinate) {
		//QMessageBox::information(0, "get_voxel_neighbour_index_coordinates", "here06", QMessageBox::Ok);

		if (voxel_index_coordinate.z < 0 || voxel_index_coordinate.z > cubic_grid->grid_dimension.z - 1) return false;
		if (voxel_index_coordinate.y < 0 || voxel_index_coordinate.x < 0) return false;

		if (voxel_index_coordinate.z % 2 == 0) {// even z level
			if (voxel_index_coordinate.y > cubic_grid->grid_dimension.y - 1) return false;

			if (voxel_index_coordinate.y % 2 == 0) { // even y coord
				if (voxel_index_coordinate.x > cubic_grid->grid_dimension.x - 1) return false;
			}
			else {
				if (voxel_index_coordinate.x > cubic_grid->grid_dimension.x - 2) return false;
			}
		}
		else {// odd z level
			if (voxel_index_coordinate.y > cubic_grid->grid_dimension.y - 2) return false;

			if (voxel_index_coordinate.y % 2 == 0) {// even y coord
				if (voxel_index_coordinate.x > cubic_grid->grid_dimension.x - 2) return false;
			}
			else {
				if (voxel_index_coordinate.x > cubic_grid->grid_dimension.x - 1) return false;
			}
		}

		//QMessageBox::information(0, "get_voxel_neighbour_index_coordinates", "here06A", QMessageBox::Ok);
		return true;
	}

    template <class T>
    cubic_surface_index_data_type get_cubic_neighbour_index(cubic_grid_base_class<T>* cubic_grid, cubic_surface_index_data_type cubic_index, int neighbour_id) {
		if (neighbour_id == 0) { return cubic_index; }// hex grid cell self value;

		//glm::ivec3 cubic_surface_coordinate = cubic_grid->get_matrix_coordinate(cubic_index);
		glm::ivec3 cubic_surface_coordinate = cubic_grid->get_matrix_coordinate(cubic_index);

//if(cubic_index < 500){
//printf("functions :: get_cubic_neighbour_index ######### 00000: %i : %i : %i : %i \n", cubic_surface_coordinate.x, cubic_surface_coordinate.y, cubic_surface_coordinate.z, cubic_index);
//}
		glm::ivec3 cubic_neighbour_coordinate = get_cubic_surface_neighbour_index_coordinates(neighbour_id, cubic_surface_coordinate.x, cubic_surface_coordinate.y, cubic_surface_coordinate.z);

//if(cubic_index < 500){
//printf("functions :: get_cubic_neighbour_index ######### 11111: %i : %i : %i : %i\n", cubic_neighbour_coordinate.x, cubic_neighbour_coordinate.y, cubic_neighbour_coordinate.z, cubic_index);
//}
		//if (cubic_neighbour_coordinate.y < 0 || cubic_neighbour_coordinate.y >= cubic_grid->grid_dimension.y) { return -1; }

		//if (cubic_neighbour_coordinate.y % 2 == 0) { // Even Y
		//    if (cubic_neighbour_coordinate.x < 0 || cubic_neighbour_coordinate.x >= cubic_grid->grid_dimension.x) { return -1; }
		//}
		//else {// Odd Y
		//    if (cubic_neighbour_coordinate.x < 0 || cubic_neighbour_coordinate.x >= cubic_grid->grid_dimension.x - 1) { return -1; }
		//}

		if (!valid_index_coordinate(cubic_grid, cubic_neighbour_coordinate)) {
//printf("cubic_functions_class : get_cubic_neighbour_index : !valid_index_coordinate %i : %i : %i \n", cubic_neighbour_coordinate.x, cubic_neighbour_coordinate.y, cubic_neighbour_coordinate.z);
			return -1;
		}

		cubic_surface_index_data_type cubic_neighbour_index = cubic_grid->get_index_value(cubic_neighbour_coordinate.x, cubic_neighbour_coordinate.y, cubic_neighbour_coordinate.z);

//printf("cubic_functions_class :: get_cubic_neighbour_index VALID coordinate ######### : %i : %i : %i: %i\n", cubic_neighbour_coordinate.x, cubic_neighbour_coordinate.y, cubic_neighbour_coordinate.z, cubic_neighbour_index);

		return cubic_neighbour_index;
    }

private:
};


