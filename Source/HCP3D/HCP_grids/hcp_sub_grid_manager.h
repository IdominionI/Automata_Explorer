#pragma once

#include "hcp_grid_base.h"

class hcp_sub_grid_manager_class {
public:
	hcp_sub_grid_manager_class() {}
	~hcp_sub_grid_manager_class() {}

	bool hcp_sub_grids_overlap(hcp_sub_grid_struct_type hcp_sub_grid01, hcp_sub_grid_struct_type hcp_sub_grid02) {
		if (hcp_sub_grid01.lower_range.x > hcp_sub_grid02.upper_range.x || hcp_sub_grid02.lower_range.x > hcp_sub_grid01.upper_range.x) { return false; }
		if (hcp_sub_grid01.lower_range.y > hcp_sub_grid02.upper_range.y || hcp_sub_grid02.lower_range.y > hcp_sub_grid01.upper_range.y) { return false; }
		if (hcp_sub_grid01.lower_range.z > hcp_sub_grid02.upper_range.z || hcp_sub_grid02.lower_range.z > hcp_sub_grid01.upper_range.z) { return false; }

		if (hcp_sub_grid01.lower_range.x <= hcp_sub_grid02.upper_range.x && hcp_sub_grid01.lower_range.x >= hcp_sub_grid02.lower_range.x) {
			if (hcp_sub_grid01.lower_range.y <= hcp_sub_grid02.upper_range.y && hcp_sub_grid01.lower_range.y >= hcp_sub_grid02.lower_range.y) {return true;}
			if (hcp_sub_grid01.upper_range.y <= hcp_sub_grid02.upper_range.y && hcp_sub_grid01.upper_range.y >= hcp_sub_grid02.lower_range.y) {return true;}

			if (hcp_sub_grid01.lower_range.z <= hcp_sub_grid02.upper_range.z && hcp_sub_grid01.lower_range.z >= hcp_sub_grid02.lower_range.z) { return true; }
			if (hcp_sub_grid01.upper_range.z <= hcp_sub_grid02.upper_range.z && hcp_sub_grid01.upper_range.z >= hcp_sub_grid02.lower_range.z) { return true; }

			return false;
		}

		if (hcp_sub_grid01.upper_range.x <= hcp_sub_grid02.upper_range.x && hcp_sub_grid01.upper_range.x >= hcp_sub_grid02.lower_range.x) {
			if (hcp_sub_grid01.lower_range.y <= hcp_sub_grid02.upper_range.y && hcp_sub_grid01.lower_range.y >= hcp_sub_grid02.lower_range.y) {return true;}
			if (hcp_sub_grid01.upper_range.y <= hcp_sub_grid02.upper_range.y && hcp_sub_grid01.upper_range.y >= hcp_sub_grid02.lower_range.y) {return true;}

			if (hcp_sub_grid01.lower_range.z <= hcp_sub_grid02.upper_range.z && hcp_sub_grid01.lower_range.z >= hcp_sub_grid02.lower_range.z) { return true; }
			if (hcp_sub_grid01.upper_range.z <= hcp_sub_grid02.upper_range.z && hcp_sub_grid01.upper_range.z >= hcp_sub_grid02.lower_range.z) { return true; }

			return false;
		}

		return false;
	}

	bool hcp_sub_grids_neighbours(hcp_sub_grid_struct_type hcp_sub_grid01, hcp_sub_grid_struct_type hcp_sub_grid02) {
		hcp_surface_vec_data_type difference01 = hcp_sub_grid02.lower_range - hcp_sub_grid01.upper_range;
		if (difference01.x == 1 || difference01.y == 1 || difference01.z == 1) { return true; }

		hcp_surface_vec_data_type difference02 = hcp_sub_grid01.lower_range - hcp_sub_grid02.upper_range;
		if (difference02.x == 1 || difference02.y == 1 || difference01.z == 1) { return true; }

		return false;
	}

	bool merge_hcp_sub_grids(hcp_sub_grid_struct_type hcp_sub_grid01, hcp_sub_grid_struct_type hcp_sub_grid02, hcp_sub_grid_struct_type &merged_hcp_sub_grid) {
		hcp_surface_vec_data_type min_grid_range = { 0,0,0 };
		hcp_surface_vec_data_type max_grid_range = { 0,0,0 };

		// define min hax grid range
		if (hcp_sub_grid01.lower_range.x <= hcp_sub_grid02.lower_range.x){
			min_grid_range.x = hcp_sub_grid01.lower_range.x;
		} else {
			min_grid_range.x = hcp_sub_grid02.lower_range.x;
		}

		if (hcp_sub_grid01.lower_range.y <= hcp_sub_grid02.lower_range.y){
			min_grid_range.y = hcp_sub_grid01.lower_range.y;
		} else {
			min_grid_range.y = hcp_sub_grid02.lower_range.y;
		}

		if (hcp_sub_grid01.lower_range.z <= hcp_sub_grid02.lower_range.z){
			min_grid_range.z = hcp_sub_grid01.lower_range.z;
		} else {
			min_grid_range.z = hcp_sub_grid02.lower_range.z;
		}

		// define max hax grid range
		if (hcp_sub_grid01.upper_range.x >= hcp_sub_grid02.upper_range.x) {
			max_grid_range.x = hcp_sub_grid01.upper_range.x;
		} else {
			max_grid_range.x = hcp_sub_grid02.upper_range.x;
		}

		if (hcp_sub_grid01.upper_range.y >= hcp_sub_grid02.upper_range.y) {
			max_grid_range.y = hcp_sub_grid01.upper_range.y;
		} else {
			max_grid_range.y = hcp_sub_grid02.upper_range.y;
		}

		if (hcp_sub_grid01.upper_range.z >= hcp_sub_grid02.upper_range.z) {
			max_grid_range.z = hcp_sub_grid01.upper_range.z;
		} else {
			max_grid_range.z = hcp_sub_grid02.upper_range.z;
		}

		merged_hcp_sub_grid.lower_range = min_grid_range;
		merged_hcp_sub_grid.upper_range = max_grid_range;

		return true;
	}

	bool hcp_sub_grid_partition_x(hcp_sub_grid_struct_type &hcp_sub_grid, hcp_surface_vec_data_type partition_coord, hcp_sub_grid_struct_type &partition_hcp_sub_grid) {
		if (partition_coord.x < hcp_sub_grid.lower_range.x || partition_coord.x > hcp_sub_grid.upper_range.x) { return false; }

		partition_hcp_sub_grid.lower_range.x = partition_coord.x;
		partition_hcp_sub_grid.lower_range.y = hcp_sub_grid.lower_range.y;
		partition_hcp_sub_grid.lower_range.z = hcp_sub_grid.lower_range.z;
		partition_hcp_sub_grid.upper_range   = hcp_sub_grid.upper_range;

		hcp_sub_grid.upper_range.x = partition_coord.x;

		return true;
	}

	bool hcp_sub_grid_partition_y(hcp_sub_grid_struct_type &hcp_sub_grid, hcp_surface_vec_data_type partition_coord, hcp_sub_grid_struct_type& partition_hcp_sub_grid) {
		if (partition_coord.y < hcp_sub_grid.lower_range.y || partition_coord.y > hcp_sub_grid.upper_range.y) { return false; }

		partition_hcp_sub_grid.lower_range.y = partition_coord.y;
		partition_hcp_sub_grid.lower_range.x = hcp_sub_grid.lower_range.x;
		partition_hcp_sub_grid.lower_range.z = hcp_sub_grid.lower_range.z;
		partition_hcp_sub_grid.upper_range   = hcp_sub_grid.upper_range;

		hcp_sub_grid.upper_range.y = partition_coord.y;

		return true;
	}

	bool hcp_sub_grid_partition_z(hcp_sub_grid_struct_type& hcp_sub_grid, hcp_surface_vec_data_type partition_coord, hcp_sub_grid_struct_type& partition_hcp_sub_grid) {
		if (partition_coord.z < hcp_sub_grid.lower_range.z || partition_coord.z > hcp_sub_grid.upper_range.z) { return false; }

		partition_hcp_sub_grid.lower_range.z = partition_coord.z;
		partition_hcp_sub_grid.lower_range.x = hcp_sub_grid.lower_range.x;
		partition_hcp_sub_grid.lower_range.y = hcp_sub_grid.lower_range.y;
		partition_hcp_sub_grid.upper_range   = hcp_sub_grid.upper_range;

		hcp_sub_grid.upper_range.z = partition_coord.z;

		return true;
	}

protected:


private:


};