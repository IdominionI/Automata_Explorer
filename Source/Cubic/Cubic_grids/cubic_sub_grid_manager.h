#pragma once

#include "cubic_grid_base.h"

class cubic_sub_grid_manager_class {
public:
	cubic_sub_grid_manager_class() {}
	~cubic_sub_grid_manager_class() {}

	bool cubic_sub_grids_overlap(cubic_sub_grid_struct_type cubic_sub_grid01, cubic_sub_grid_struct_type cubic_sub_grid02) {
		if (cubic_sub_grid01.lower_range.x > cubic_sub_grid02.upper_range.x || cubic_sub_grid02.lower_range.x > cubic_sub_grid01.upper_range.x) { return false; }
		if (cubic_sub_grid01.lower_range.y > cubic_sub_grid02.upper_range.y || cubic_sub_grid02.lower_range.y > cubic_sub_grid01.upper_range.y) { return false; }
		if (cubic_sub_grid01.lower_range.z > cubic_sub_grid02.upper_range.z || cubic_sub_grid02.lower_range.z > cubic_sub_grid01.upper_range.z) { return false; }

		if (cubic_sub_grid01.lower_range.x <= cubic_sub_grid02.upper_range.x && cubic_sub_grid01.lower_range.x >= cubic_sub_grid02.lower_range.x) {
			if (cubic_sub_grid01.lower_range.y <= cubic_sub_grid02.upper_range.y && cubic_sub_grid01.lower_range.y >= cubic_sub_grid02.lower_range.y) {return true;}
			if (cubic_sub_grid01.upper_range.y <= cubic_sub_grid02.upper_range.y && cubic_sub_grid01.upper_range.y >= cubic_sub_grid02.lower_range.y) {return true;}

			if (cubic_sub_grid01.lower_range.z <= cubic_sub_grid02.upper_range.z && cubic_sub_grid01.lower_range.z >= cubic_sub_grid02.lower_range.z) { return true; }
			if (cubic_sub_grid01.upper_range.z <= cubic_sub_grid02.upper_range.z && cubic_sub_grid01.upper_range.z >= cubic_sub_grid02.lower_range.z) { return true; }

			return false;
		}

		if (cubic_sub_grid01.upper_range.x <= cubic_sub_grid02.upper_range.x && cubic_sub_grid01.upper_range.x >= cubic_sub_grid02.lower_range.x) {
			if (cubic_sub_grid01.lower_range.y <= cubic_sub_grid02.upper_range.y && cubic_sub_grid01.lower_range.y >= cubic_sub_grid02.lower_range.y) {return true;}
			if (cubic_sub_grid01.upper_range.y <= cubic_sub_grid02.upper_range.y && cubic_sub_grid01.upper_range.y >= cubic_sub_grid02.lower_range.y) {return true;}

			if (cubic_sub_grid01.lower_range.z <= cubic_sub_grid02.upper_range.z && cubic_sub_grid01.lower_range.z >= cubic_sub_grid02.lower_range.z) { return true; }
			if (cubic_sub_grid01.upper_range.z <= cubic_sub_grid02.upper_range.z && cubic_sub_grid01.upper_range.z >= cubic_sub_grid02.lower_range.z) { return true; }

			return false;
		}

		return false;
	}

	bool cubic_sub_grids_neighbours(cubic_sub_grid_struct_type cubic_sub_grid01, cubic_sub_grid_struct_type cubic_sub_grid02) {
		cubic_surface_vec_data_type difference01 = cubic_sub_grid02.lower_range - cubic_sub_grid01.upper_range;
		if (difference01.x == 1 || difference01.y == 1 || difference01.z == 1) { return true; }

		cubic_surface_vec_data_type difference02 = cubic_sub_grid01.lower_range - cubic_sub_grid02.upper_range;
		if (difference02.x == 1 || difference02.y == 1 || difference01.z == 1) { return true; }

		return false;
	}

	bool merge_cubic_sub_grids(cubic_sub_grid_struct_type cubic_sub_grid01, cubic_sub_grid_struct_type cubic_sub_grid02, cubic_sub_grid_struct_type &merged_cubic_sub_grid) {
		cubic_surface_vec_data_type min_grid_range = { 0,0,0 };
		cubic_surface_vec_data_type max_grid_range = { 0,0,0 };

		// define min hax grid range
		if (cubic_sub_grid01.lower_range.x <= cubic_sub_grid02.lower_range.x){
			min_grid_range.x = cubic_sub_grid01.lower_range.x;
		} else {
			min_grid_range.x = cubic_sub_grid02.lower_range.x;
		}

		if (cubic_sub_grid01.lower_range.y <= cubic_sub_grid02.lower_range.y){
			min_grid_range.y = cubic_sub_grid01.lower_range.y;
		} else {
			min_grid_range.y = cubic_sub_grid02.lower_range.y;
		}

		if (cubic_sub_grid01.lower_range.z <= cubic_sub_grid02.lower_range.z){
			min_grid_range.z = cubic_sub_grid01.lower_range.z;
		} else {
			min_grid_range.z = cubic_sub_grid02.lower_range.z;
		}

		// define max hax grid range
		if (cubic_sub_grid01.upper_range.x >= cubic_sub_grid02.upper_range.x) {
			max_grid_range.x = cubic_sub_grid01.upper_range.x;
		} else {
			max_grid_range.x = cubic_sub_grid02.upper_range.x;
		}

		if (cubic_sub_grid01.upper_range.y >= cubic_sub_grid02.upper_range.y) {
			max_grid_range.y = cubic_sub_grid01.upper_range.y;
		} else {
			max_grid_range.y = cubic_sub_grid02.upper_range.y;
		}

		if (cubic_sub_grid01.upper_range.z >= cubic_sub_grid02.upper_range.z) {
			max_grid_range.z = cubic_sub_grid01.upper_range.z;
		} else {
			max_grid_range.z = cubic_sub_grid02.upper_range.z;
		}

		merged_cubic_sub_grid.lower_range = min_grid_range;
		merged_cubic_sub_grid.upper_range = max_grid_range;

		return true;
	}

	bool cubic_sub_grid_partition_x(cubic_sub_grid_struct_type &cubic_sub_grid, cubic_surface_vec_data_type partition_coord, cubic_sub_grid_struct_type &partition_cubic_sub_grid) {
		if (partition_coord.x < cubic_sub_grid.lower_range.x || partition_coord.x > cubic_sub_grid.upper_range.x) { return false; }

		partition_cubic_sub_grid.lower_range.x = partition_coord.x;
		partition_cubic_sub_grid.lower_range.y = cubic_sub_grid.lower_range.y;
		partition_cubic_sub_grid.lower_range.z = cubic_sub_grid.lower_range.z;
		partition_cubic_sub_grid.upper_range   = cubic_sub_grid.upper_range;

		cubic_sub_grid.upper_range.x = partition_coord.x;

		return true;
	}

	bool cubic_sub_grid_partition_y(cubic_sub_grid_struct_type &cubic_sub_grid, cubic_surface_vec_data_type partition_coord, cubic_sub_grid_struct_type& partition_cubic_sub_grid) {
		if (partition_coord.y < cubic_sub_grid.lower_range.y || partition_coord.y > cubic_sub_grid.upper_range.y) { return false; }

		partition_cubic_sub_grid.lower_range.y = partition_coord.y;
		partition_cubic_sub_grid.lower_range.x = cubic_sub_grid.lower_range.x;
		partition_cubic_sub_grid.lower_range.z = cubic_sub_grid.lower_range.z;
		partition_cubic_sub_grid.upper_range   = cubic_sub_grid.upper_range;

		cubic_sub_grid.upper_range.y = partition_coord.y;

		return true;
	}

	bool cubic_sub_grid_partition_z(cubic_sub_grid_struct_type& cubic_sub_grid, cubic_surface_vec_data_type partition_coord, cubic_sub_grid_struct_type& partition_cubic_sub_grid) {
		if (partition_coord.z < cubic_sub_grid.lower_range.z || partition_coord.z > cubic_sub_grid.upper_range.z) { return false; }

		partition_cubic_sub_grid.lower_range.z = partition_coord.z;
		partition_cubic_sub_grid.lower_range.x = cubic_sub_grid.lower_range.x;
		partition_cubic_sub_grid.lower_range.y = cubic_sub_grid.lower_range.y;
		partition_cubic_sub_grid.upper_range   = cubic_sub_grid.upper_range;

		cubic_sub_grid.upper_range.z = partition_coord.z;

		return true;
	}

protected:


private:


};