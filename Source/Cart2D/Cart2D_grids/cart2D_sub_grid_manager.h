#pragma once

#include "cart2D_grid_base.h"

class cart2D_sub_grid_manager_class {
public:
	cart2D_sub_grid_manager_class() {}
	~cart2D_sub_grid_manager_class() {}

	bool cart2D_sub_grids_overlap(cart2D_sub_grid_struct_type cart2D_sub_grid01, cart2D_sub_grid_struct_type cart2D_sub_grid02) {
		if (cart2D_sub_grid01.lower_range.x > cart2D_sub_grid02.upper_range.x || cart2D_sub_grid02.lower_range.x > cart2D_sub_grid01.upper_range.x) { return false; }
		if (cart2D_sub_grid01.lower_range.y > cart2D_sub_grid02.upper_range.y || cart2D_sub_grid02.lower_range.y > cart2D_sub_grid01.upper_range.y) { return false; }

		if (cart2D_sub_grid01.lower_range.x <= cart2D_sub_grid02.upper_range.x && cart2D_sub_grid01.lower_range.x >= cart2D_sub_grid02.lower_range.x) {
			if (cart2D_sub_grid01.lower_range.y <= cart2D_sub_grid02.upper_range.y && cart2D_sub_grid01.lower_range.y >= cart2D_sub_grid02.lower_range.y)
				{return true;}

			if (cart2D_sub_grid01.upper_range.y <= cart2D_sub_grid02.upper_range.y && cart2D_sub_grid01.upper_range.y >= cart2D_sub_grid02.lower_range.y)
				{return true;}

			return false;
		}

		if (cart2D_sub_grid01.upper_range.x <= cart2D_sub_grid02.upper_range.x && cart2D_sub_grid01.upper_range.x >= cart2D_sub_grid02.lower_range.x) {
			if (cart2D_sub_grid01.lower_range.y <= cart2D_sub_grid02.upper_range.y && cart2D_sub_grid01.lower_range.y >= cart2D_sub_grid02.lower_range.y)
				{return true;}

			if (cart2D_sub_grid01.upper_range.y <= cart2D_sub_grid02.upper_range.y && cart2D_sub_grid01.upper_range.y >= cart2D_sub_grid02.lower_range.y)
				{return true;}

			return false;
		}

		return false;
	}

	bool cart2D_sub_grids_neighbours(cart2D_sub_grid_struct_type cart2D_sub_grid01, cart2D_sub_grid_struct_type cart2D_sub_grid02) {
		cart2D_surface_vec_data_type difference01 = cart2D_sub_grid02.lower_range - cart2D_sub_grid01.upper_range;
		if (difference01.x == 1 || difference01.y == 1) { return true; }

		cart2D_surface_vec_data_type difference02 = cart2D_sub_grid01.lower_range - cart2D_sub_grid02.upper_range;
		if (difference02.x == 1 || difference02.y == 1) { return true; }

		return false;
	}

	bool merge_cart2D_sub_grids(cart2D_sub_grid_struct_type cart2D_sub_grid01, cart2D_sub_grid_struct_type cart2D_sub_grid02, cart2D_sub_grid_struct_type &merged_cart2D_sub_grid) {
		cart2D_surface_vec_data_type min_grid_range = { 0,0 };
		cart2D_surface_vec_data_type max_grid_range = { 0,0 };

		// define min cart2D grid range
		if (cart2D_sub_grid01.lower_range.x <= cart2D_sub_grid02.lower_range.x){
			min_grid_range.x = cart2D_sub_grid01.lower_range.x;
		} else {
			min_grid_range.x = cart2D_sub_grid02.lower_range.x;
		}

		if (cart2D_sub_grid01.lower_range.y <= cart2D_sub_grid02.lower_range.y){
			min_grid_range.y = cart2D_sub_grid01.lower_range.y;
		} else {
			min_grid_range.y = cart2D_sub_grid02.lower_range.y;
		}

		// define max cart2D grid range
		if (cart2D_sub_grid01.upper_range.x >= cart2D_sub_grid02.upper_range.x) {
			max_grid_range.x = cart2D_sub_grid01.upper_range.x;
		} else {
			max_grid_range.x = cart2D_sub_grid02.upper_range.x;
		}

		if (cart2D_sub_grid01.upper_range.y >= cart2D_sub_grid02.upper_range.y) {
			max_grid_range.y = cart2D_sub_grid01.upper_range.y;
		} else {
			max_grid_range.y = cart2D_sub_grid02.upper_range.y;
		}

		merged_cart2D_sub_grid.lower_range = min_grid_range;
		merged_cart2D_sub_grid.upper_range = max_grid_range;

		return true;
	}

	bool cart2D_sub_grid_partition_x(cart2D_sub_grid_struct_type &cart2D_sub_grid, cart2D_surface_vec_data_type partition_coord, cart2D_sub_grid_struct_type &partition_cart2D_sub_grid) {
		if (partition_coord.x < cart2D_sub_grid.lower_range.x || partition_coord.x > cart2D_sub_grid.upper_range.x) { return false; }

		partition_cart2D_sub_grid.lower_range.x = partition_coord.x;
		partition_cart2D_sub_grid.lower_range.y = cart2D_sub_grid.lower_range.y;
		partition_cart2D_sub_grid.upper_range = cart2D_sub_grid.upper_range;

		cart2D_sub_grid.upper_range.x = partition_coord.x;

		return true;
	}

	bool cart2D_sub_grid_partition_y(cart2D_sub_grid_struct_type &cart2D_sub_grid, cart2D_surface_vec_data_type partition_coord, cart2D_sub_grid_struct_type& partition_cart2D_sub_grid) {
		if (partition_coord.y < cart2D_sub_grid.lower_range.y || partition_coord.y > cart2D_sub_grid.upper_range.y) { return false; }

		partition_cart2D_sub_grid.lower_range.y = partition_coord.y;
		partition_cart2D_sub_grid.lower_range.x = cart2D_sub_grid.lower_range.x;
		partition_cart2D_sub_grid.upper_range = cart2D_sub_grid.upper_range;

		cart2D_sub_grid.upper_range.y = partition_coord.y;

		return true;
	}



protected:


private:


};