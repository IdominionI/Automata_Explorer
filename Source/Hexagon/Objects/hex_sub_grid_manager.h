#pragma once

#include "hex_grid_base.h"

class hex_sub_grid_manager_class {
public:
	hex_sub_grid_manager_class() {}
	~hex_sub_grid_manager_class() {}

	bool hex_sub_grids_overlap(hex_sub_grid_struct_type hex_sub_grid01, hex_sub_grid_struct_type hex_sub_grid02) {
		if (hex_sub_grid01.lower_range.x > hex_sub_grid02.upper_range.x || hex_sub_grid02.lower_range.x > hex_sub_grid01.upper_range.x) { return false; }
		if (hex_sub_grid01.lower_range.y > hex_sub_grid02.upper_range.y || hex_sub_grid02.lower_range.y > hex_sub_grid01.upper_range.y) { return false; }

		if (hex_sub_grid01.lower_range.x <= hex_sub_grid02.upper_range.x && hex_sub_grid01.lower_range.x >= hex_sub_grid02.lower_range.x) {
			if (hex_sub_grid01.lower_range.y <= hex_sub_grid02.upper_range.y && hex_sub_grid01.lower_range.y >= hex_sub_grid02.lower_range.y)
				{return true;}

			if (hex_sub_grid01.upper_range.y <= hex_sub_grid02.upper_range.y && hex_sub_grid01.upper_range.y >= hex_sub_grid02.lower_range.y)
				{return true;}

			return false;
		}

		if (hex_sub_grid01.upper_range.x <= hex_sub_grid02.upper_range.x && hex_sub_grid01.upper_range.x >= hex_sub_grid02.lower_range.x) {
			if (hex_sub_grid01.lower_range.y <= hex_sub_grid02.upper_range.y && hex_sub_grid01.lower_range.y >= hex_sub_grid02.lower_range.y)
				{return true;}

			if (hex_sub_grid01.upper_range.y <= hex_sub_grid02.upper_range.y && hex_sub_grid01.upper_range.y >= hex_sub_grid02.lower_range.y)
				{return true;}

			return false;
		}

		return false;
	}

	bool hex_sub_grids_neighbours(hex_sub_grid_struct_type hex_sub_grid01, hex_sub_grid_struct_type hex_sub_grid02) {
		hex_surface_vec_data_type difference01 = hex_sub_grid02.lower_range - hex_sub_grid01.upper_range;
		if (difference01.x == 1 || difference01.y == 1) { return true; }

		hex_surface_vec_data_type difference02 = hex_sub_grid01.lower_range - hex_sub_grid02.upper_range;
		if (difference02.x == 1 || difference02.y == 1) { return true; }

		return false;
	}

	bool merge_hex_sub_grids(hex_sub_grid_struct_type hex_sub_grid01, hex_sub_grid_struct_type hex_sub_grid02, hex_sub_grid_struct_type &merged_hex_sub_grid) {
		hex_surface_vec_data_type min_grid_range = { 0,0 };
		hex_surface_vec_data_type max_grid_range = { 0,0 };

		// define min hax grid range
		if (hex_sub_grid01.lower_range.x <= hex_sub_grid02.lower_range.x){
			min_grid_range.x = hex_sub_grid01.lower_range.x;
		} else {
			min_grid_range.x = hex_sub_grid02.lower_range.x;
		}

		if (hex_sub_grid01.lower_range.y <= hex_sub_grid02.lower_range.y){
			min_grid_range.y = hex_sub_grid01.lower_range.y;
		} else {
			min_grid_range.y = hex_sub_grid02.lower_range.y;
		}

		// define max hax grid range
		if (hex_sub_grid01.upper_range.x >= hex_sub_grid02.upper_range.x) {
			max_grid_range.x = hex_sub_grid01.upper_range.x;
		} else {
			max_grid_range.x = hex_sub_grid02.upper_range.x;
		}

		if (hex_sub_grid01.upper_range.y >= hex_sub_grid02.upper_range.y) {
			max_grid_range.y = hex_sub_grid01.upper_range.y;
		} else {
			max_grid_range.y = hex_sub_grid02.upper_range.y;
		}

		merged_hex_sub_grid.lower_range = min_grid_range;
		merged_hex_sub_grid.upper_range = max_grid_range;

		return true;
	}

	bool hex_sub_grid_partition_x(hex_sub_grid_struct_type &hex_sub_grid, hex_surface_vec_data_type partition_coord, hex_sub_grid_struct_type &partition_hex_sub_grid) {
		if (partition_coord.x < hex_sub_grid.lower_range.x || partition_coord.x > hex_sub_grid.upper_range.x) { return false; }

		partition_hex_sub_grid.lower_range.x = partition_coord.x;
		partition_hex_sub_grid.lower_range.y = hex_sub_grid.lower_range.y;
		partition_hex_sub_grid.upper_range = hex_sub_grid.upper_range;

		hex_sub_grid.upper_range.x = partition_coord.x;

		return true;
	}

	bool hex_sub_grid_partition_y(hex_sub_grid_struct_type &hex_sub_grid, hex_surface_vec_data_type partition_coord, hex_sub_grid_struct_type& partition_hex_sub_grid) {
		if (partition_coord.y < hex_sub_grid.lower_range.y || partition_coord.y > hex_sub_grid.upper_range.y) { return false; }

		partition_hex_sub_grid.lower_range.y = partition_coord.y;
		partition_hex_sub_grid.lower_range.x = hex_sub_grid.lower_range.x;
		partition_hex_sub_grid.upper_range = hex_sub_grid.upper_range;

		hex_sub_grid.upper_range.y = partition_coord.y;

		return true;
	}



protected:


private:


};