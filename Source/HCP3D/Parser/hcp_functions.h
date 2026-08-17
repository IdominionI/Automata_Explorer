#pragma once

#include "Source/HCP3D/HCP_Grids/hcp_grid_base.h"
#include "Source/Parser/functions.h"

// ###### HEXAGONAL GRID CELL PARSER TOKEN DEFINITIONS ######

#define HCP_GRID_NEIGHBOUR_H0  "H0"
#define HCP_GRID_NEIGHBOUR_H1  "H1"
#define HCP_GRID_NEIGHBOUR_H2  "H2"
#define HCP_GRID_NEIGHBOUR_H3  "H3"
#define HCP_GRID_NEIGHBOUR_H4  "H4"
#define HCP_GRID_NEIGHBOUR_H5  "H5"
#define HCP_GRID_NEIGHBOUR_H6  "H6"
#define HCP_GRID_NEIGHBOUR_H7  "H7"
#define HCP_GRID_NEIGHBOUR_H8  "H8"
#define HCP_GRID_NEIGHBOUR_H9  "H9"
#define HCP_GRID_NEIGHBOUR_H10 "H10"
#define HCP_GRID_NEIGHBOUR_H11 "H11"
#define HCP_GRID_NEIGHBOUR_H12 "H12"

class hcp_functions_class : public functions_base_class {
public:
    hcp_functions_class() {}
    ~hcp_functions_class() {}
    // ################ FUNCTIONS TO RETRIEVE HEXAGONAL GRID CELL VALUES ###############

    template <class T>
    bool get_hcp_grid_value(literal_node* dn, hcp_grid_base_class<T>* hcp_grid, hcp_surface_index_data_type hcp_index) {
        if (hcp_index < 0 || hcp_index >= hcp_grid->hcp_grid.size()) return false; //hcp_index is invalid

        std::string hcp_data_token = dn->function_token_definition.second;

        hcp_surface_index_data_type hcp_neighbour_index = -1;
        int hcp_neighbour_id = -1;

        if (hcp_data_token == HCP_GRID_NEIGHBOUR_H0)  { hcp_neighbour_id = 0; }
        if (hcp_data_token == HCP_GRID_NEIGHBOUR_H1)  { hcp_neighbour_id = 1; }
        if (hcp_data_token == HCP_GRID_NEIGHBOUR_H2)  { hcp_neighbour_id = 2; }
        if (hcp_data_token == HCP_GRID_NEIGHBOUR_H3)  { hcp_neighbour_id = 3; }
        if (hcp_data_token == HCP_GRID_NEIGHBOUR_H4)  { hcp_neighbour_id = 4; }
        if (hcp_data_token == HCP_GRID_NEIGHBOUR_H5)  { hcp_neighbour_id = 5; }
        if (hcp_data_token == HCP_GRID_NEIGHBOUR_H6)  { hcp_neighbour_id = 6; }
        if (hcp_data_token == HCP_GRID_NEIGHBOUR_H7 ) { hcp_neighbour_id = 7; }
        if (hcp_data_token == HCP_GRID_NEIGHBOUR_H8 ) { hcp_neighbour_id = 8; }
        if (hcp_data_token == HCP_GRID_NEIGHBOUR_H9 ) { hcp_neighbour_id = 9; }
        if (hcp_data_token == HCP_GRID_NEIGHBOUR_H10) { hcp_neighbour_id = 10; }
        if (hcp_data_token == HCP_GRID_NEIGHBOUR_H11) { hcp_neighbour_id = 11; }
        if (hcp_data_token == HCP_GRID_NEIGHBOUR_H12) { hcp_neighbour_id = 12; }

        if (hcp_neighbour_id < 0) { return false; } //have invalid hex neighbour token

        hcp_neighbour_index = get_hcp_neighbour_index(hcp_grid, hcp_index, hcp_neighbour_id);

        if (hcp_neighbour_index < 0 || hcp_neighbour_index > hcp_grid->hcp_grid.size() - 1) { // Neighbour is outside of hex grid bounds and does not exist 
            dn->value.first = hcp_grid_value_data_type_enum::nan;// This is to indicate that the neighbour is to be ignored as it does not exist
            return true;
        }

        dn->value.first = hcp_grid->hcp_grid_value_data_type;

        //printf("functions :: assign_hcp_grid_value $$$$$$$$$$$$$ : %i : %i\n", hcp_index, hcp_neighbour_index);
        switch (hcp_grid->hcp_grid_value_data_type) {
        case hcp_grid_value_data_type_enum::Boolean: dn->value.second.bvalue = hcp_grid->hcp_grid[hcp_neighbour_index]; break;
        case hcp_grid_value_data_type_enum::Float:   dn->value.second.fvalue = hcp_grid->hcp_grid[hcp_neighbour_index]; break;
        case hcp_grid_value_data_type_enum::Integer: dn->value.second.ivalue = hcp_grid->hcp_grid[hcp_neighbour_index]; break;
        case hcp_grid_value_data_type_enum::Double:  dn->value.second.dvalue = hcp_grid->hcp_grid[hcp_neighbour_index]; break;

            // for testing :: Toggle this with above case statements when not testing
            //case hcp_grid_value_data_type_enum::Boolean: dn->value.second.bvalue = hcp_grid->hcp_grid[hcp_neighbour_index];printf("Boolean:%i\n", hcp_grid->hcp_grid[hcp_neighbour_index]); break;
            //case hcp_grid_value_data_type_enum::Float:   dn->value.second.fvalue = hcp_grid->hcp_grid[hcp_neighbour_index]; printf("Float:  %f\n", hcp_grid->hcp_grid[hcp_neighbour_index]); break;
            //case hcp_grid_value_data_type_enum::Integer: dn->value.second.ivalue = hcp_grid->hcp_grid[hcp_neighbour_index]; printf("Integer:%i\n", hcp_grid->hcp_grid[hcp_neighbour_index]); break;
            //case hcp_grid_value_data_type_enum::Double:  dn->value.second.dvalue = hcp_grid->hcp_grid[hcp_neighbour_index]; printf("Double: %d\n", hcp_grid->hcp_grid[hcp_neighbour_index]); break;
        }

        return true;
    }

    hcp_surface_vec_data_type get_hcp_surface_neighbour_index_coordinates(size_t neighbour, int x_index, int y_index, int z_index) {
		glm::ivec3 neighbor_index_coordinate = { x_index, y_index, z_index };

		if (neighbour == 0) { // Get grid value of the H0 hcp voxel
			return neighbor_index_coordinate;
		}

		if (z_index % 2 == 0) {  // Even Z 
			if (y_index % 2 == 0) { // Even Y 
				switch (neighbour) {
				case 1: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index + 1; break;
				case 2: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index + 1; break;
				case 3: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index + 1; break;

				case 4: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index + 1; neighbor_index_coordinate.z = z_index;    break;
				case 5: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index;    break;
				case 6: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index;    break;
				case 7: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index;    break;
				case 8: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index;    break;
				case 9: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index + 1; neighbor_index_coordinate.z = z_index;    break;

				case 10: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index - 1; break;
				case 11: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index - 1; break;
				case 12: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index - 1; break;
				}
			}
			else {// Odd Y 
				switch (neighbour) {
				case 1: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index + 1; break;
				case 2: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index + 1; break;
				case 3: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index + 1; break;

				case 4: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index + 1; neighbor_index_coordinate.z = z_index;    break;
				case 5: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index;    break;
				case 6: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index;    break;
				case 7: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index;    break;
				case 8: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index;    break;
				case 9: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index + 1; neighbor_index_coordinate.z = z_index;    break;

				case 10:  neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index - 1; break;
				case 11: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index - 1; break;
				case 12: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index - 1; break;
				}
			}
		}
		else { // Odd Z 
			if (y_index % 2 == 0) { // Even Y 
				switch (neighbour) {
				case 1: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index + 1; break;
				case 2: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index + 1; neighbor_index_coordinate.z = z_index + 1; break;
				case 3: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index + 1; break;

				case 4: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index + 1; neighbor_index_coordinate.z = z_index;    break;
				case 5: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index;    break;
				case 6: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index;    break;
				case 7: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index;    break;
				case 8: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index;    break;
				case 9: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index + 1; neighbor_index_coordinate.z = z_index;    break;

				case 10:  neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index - 1; break;
				case 11: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index + 1; neighbor_index_coordinate.z = z_index - 1; break;
				case 12: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index - 1; break;
				}
			}
			else {// Odd Y
				switch (neighbour) {
				case 1: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index + 1; break;
				case 2: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index + 1; neighbor_index_coordinate.z = z_index + 1; break;
				case 3: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index + 1; break;

				case 4: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index + 1; neighbor_index_coordinate.z = z_index;    break;
				case 5: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index;    break;
				case 6: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index;    break;
				case 7: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index - 1; neighbor_index_coordinate.z = z_index;    break;
				case 8: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index;    break;
				case 9: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index + 1; neighbor_index_coordinate.z = z_index;    break;

				case 10:  neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index - 1; break;
				case 11: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index + 1; neighbor_index_coordinate.z = z_index - 1; break;
				case 12: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index;     neighbor_index_coordinate.z = z_index - 1; break;
				}
			}
		}

		return neighbor_index_coordinate;
    }

	// Determine if hcp voxel cell index coordinate voxel_index_coordinate is valid or not
	// Ie if it is outside the volume and limits of the hcp voxel matrix
	template <class T>
	bool valid_index_coordinate(hcp_grid_base_class<T>* hcp_grid, glm::ivec3 voxel_index_coordinate) {
		//QMessageBox::information(0, "get_voxel_neighbour_index_coordinates", "here06", QMessageBox::Ok);

		if (voxel_index_coordinate.z < 0 || voxel_index_coordinate.z > hcp_grid->grid_dimension.z - 1) return false;
		if (voxel_index_coordinate.y < 0 || voxel_index_coordinate.x < 0) return false;

		if (voxel_index_coordinate.z % 2 == 0) {// even z level
			if (voxel_index_coordinate.y > hcp_grid->grid_dimension.y - 1) return false;

			if (voxel_index_coordinate.y % 2 == 0) { // even y coord
				if (voxel_index_coordinate.x > hcp_grid->grid_dimension.x - 1) return false;
			}
			else {
				if (voxel_index_coordinate.x > hcp_grid->grid_dimension.x - 2) return false;
			}
		}
		else {// odd z level
			if (voxel_index_coordinate.y > hcp_grid->grid_dimension.y - 2) return false;

			if (voxel_index_coordinate.y % 2 == 0) {// even y coord
				if (voxel_index_coordinate.x > hcp_grid->grid_dimension.x - 2) return false;
			}
			else {
				if (voxel_index_coordinate.x > hcp_grid->grid_dimension.x - 1) return false;
			}
		}

		//QMessageBox::information(0, "get_voxel_neighbour_index_coordinates", "here06A", QMessageBox::Ok);
		return true;
	}

    template <class T>
    hcp_surface_index_data_type get_hcp_neighbour_index(hcp_grid_base_class<T>* hcp_grid, hcp_surface_index_data_type hcp_index, int neighbour_id) {
		if (neighbour_id == 0) { return hcp_index; }// hex grid cell self value;

		//glm::ivec3 hcp_surface_coordinate = hcp_grid->get_matrix_coordinate(hcp_index);
		glm::ivec3 hcp_surface_coordinate = hcp_grid->get_matrix_coordinate(hcp_index);

//if(hcp_index < 500){
//printf("functions :: get_hcp_neighbour_index ######### 00000: %i : %i : %i : %i \n", hcp_surface_coordinate.x, hcp_surface_coordinate.y, hcp_surface_coordinate.z, hcp_index);
//}
		glm::ivec3 hcp_neighbour_coordinate = get_hcp_surface_neighbour_index_coordinates(neighbour_id, hcp_surface_coordinate.x, hcp_surface_coordinate.y, hcp_surface_coordinate.z);

//if(hcp_index < 500){
//printf("functions :: get_hcp_neighbour_index ######### 11111: %i : %i : %i : %i\n", hcp_neighbour_coordinate.x, hcp_neighbour_coordinate.y, hcp_neighbour_coordinate.z, hcp_index);
//}

		if (!valid_index_coordinate(hcp_grid, hcp_neighbour_coordinate)) {
//printf("hcp_functions_class : get_hcp_neighbour_index : !valid_index_coordinate %i : %i : %i \n", hcp_neighbour_coordinate.x, hcp_neighbour_coordinate.y, hcp_neighbour_coordinate.z);
			return -1;
		}

		hcp_surface_index_data_type hcp_neighbour_index = hcp_grid->get_index_value(hcp_neighbour_coordinate.x, hcp_neighbour_coordinate.y, hcp_neighbour_coordinate.z);

//printf("hcp_functions_class :: get_hcp_neighbour_index VALID coordinate ######### : %i : %i : %i: %i\n", hcp_neighbour_coordinate.x, hcp_neighbour_coordinate.y, hcp_neighbour_coordinate.z, hcp_neighbour_index);

		return hcp_neighbour_index;
    }

private:
};


