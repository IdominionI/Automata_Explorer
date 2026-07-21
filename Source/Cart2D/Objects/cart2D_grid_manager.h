#pragma once

#include "cart2D_grid_base.h"

template <class T>
class cart2D_grid_manager_class {
public:
	cart2D_grid_manager_class() {}
	~cart2D_grid_manager_class() {}

	bool cart2D_grids_overlap(cart2D_grid_base_class<T> &cart2D_grid01, cart2D_grid_base_class<T> &cart2D_grid02) {
		if (!cart2D_grid01 || !cart2D_grid02) { return false; }
		if (cart2D_grid01.null_dimension() || cart2D_grid02.null_dimension()) { return false; }

		define_cart2D_grid_range(cart2D_grid01, cart2D_grid02);
		
		if (cart2D_grid01_min_range.x > cart2D_grid02_max_range.x || cart2D_grid02_min_range.x > cart2D_grid01_max_range.x) { return false; }
		if (cart2D_grid01_min_range.y > cart2D_grid02_max_range.y || cart2D_grid02_min_range.y > cart2D_grid01_max_range.y) { return false; }

		if (cart2D_grid01_min_range.x <= cart2D_grid02_max_range.x && cart2D_grid01_min_range.x >= cart2D_grid02_min_range.x){
			if (cart2D_grid01_min_range.y <= cart2D_grid02_max_range.y && cart2D_grid01_min_range.y >= cart2D_grid02_min_range.y)
				{return true;}

			if (cart2D_grid01_max_range.y <= cart2D_grid02_max_range.y && cart2D_grid01_max_range.y >= cart2D_grid02_min_range.y)
				{return true;}

			return false;
		}

		if (cart2D_grid01_max_range.x <= cart2D_grid02_max_range.x && cart2D_grid01_max_range.x >= cart2D_grid02_min_range.x) {
			if (cart2D_grid01_min_range.y <= cart2D_grid02_max_range.y && cart2D_grid01_min_range.y >= cart2D_grid02_min_range.y)
				{return true;}

			if (cart2D_grid01_max_range.y <= cart2D_grid02_max_range.y && cart2D_grid01_max_range.y >= cart2D_grid02_min_range.y)
				{return true;}

			return false;
		}

		return false;
	}

	bool cart2D_grids_neighbours(cart2D_grid_base_class<T> &cart2D_grid01, cart2D_grid_base_class<T> &cart2D_grid02) {
		if (!cart2D_grid01 || !cart2D_grid02) { return false; }
		if (cart2D_grid01.null_dimension() || cart2D_grid02.null_dimension()) { return false; }

		define_cart2D_grid_range(cart2D_grid01,cart2D_grid02);

		cart2D_surface_index_data_type difference01 = cart2D_grid02_min_range - cart2D_grid01_max_range;
		if (difference01.x == 1 || difference01.y == 1) { return true; }

		cart2D_surface_index_data_type difference02 = cart2D_grid01_min_range - cart2D_grid02_max_range;
		if (difference02.x == 1 || difference02.y == 1) { return true; }

		return false;
	}

	cart2D_grid_base_class<T>* merge_cart2D_grids(cart2D_grid_base_class<T> *cart2D_grid01, cart2D_grid_base_class<T> *cart2D_grid02) {
		if (!cart2D_grid01 || !cart2D_grid02) { return nullptr; }
		if (cart2D_grid01->null_dimension() || cart2D_grid02->null_dimension()) { return nullptr; }
		if (cart2D_grid01->cart2D_size != cart2D_grid02->cart2D_size) { return nullptr; }

		define_cart2D_grid_range(cart2D_grid01, cart2D_grid02);

		cart2D_surface_vec_data_type min_grid_range = { 0,0 };
		cart2D_surface_vec_data_type max_grid_range = { 0,0 };

		// define min hax grid range
		if (cart2D_grid01_min_range.x <= cart2D_grid02_min_range.x) {
			min_grid_range.x = cart2D_grid01_min_range.x;
		} else{
			min_grid_range.x = cart2D_grid02_min_range.x;
		}

		if (cart2D_grid01_min_range.y <= cart2D_grid02_min_range.y) {
			min_grid_range.y = cart2D_grid01_min_range.y;
		} else {
			min_grid_range.y = cart2D_grid02_min_range.y;
		}

		// define max hax grid range
		if (cart2D_grid01_max_range.x >= cart2D_grid02_max_range.x) {
			max_grid_range.x = cart2D_grid01_max_range.x;
		} else {
			max_grid_range.x = cart2D_grid02_max_range.x;
		}

		if (cart2D_grid01_max_range.y >= cart2D_grid02_max_range.y) {
			max_grid_range.y = cart2D_grid01_max_range.y;
		} else {
			max_grid_range.y = cart2D_grid02_max_range.y;
		}

		// Create grid to merge grids into
		cart2D_grid_base_class<T> *cart2D_grid_merged = new cart2D_grid_base_class<T>;
		if (!cart2D_grid_merged) { return nullptr; }
		if (!cart2D_grid_merged->create_cart2D_grid(min_grid_range, max_grid_range, cart2D_grid01->cart2D_size)) { return false; }

		// Merge grids in order of least to highest grid index
		// Get indexes at which to merge grids
		//cart2D_surface_index_data_type start_index_cart2D_grid01 = cart2D_grid01->origin_parent_cart2D_grid_index - cart2D_grid_merged->origin_parent_cart2D_grid_index;
		//cart2D_surface_index_data_type start_index_cart2D_grid02 = cart2D_grid02->origin_parent_cart2D_grid_index - cart2D_grid_merged->origin_parent_cart2D_grid_index;

		for (cart2D_surface_index_data_type i = 0; i < cart2D_grid01->cart2D_grid.size(); i++) {
			cart2D_surface_vec_data_type gmc = cart2D_grid01->get_matrix_coordinate(i) + cart2D_grid01->global_grid_origin;

			cart2D_surface_index_data_type mgi = cart2D_grid_merged->get_cart2D_surface_matrix_data_index(gmc);

			cart2D_grid_merged[mgi] = cart2D_grid01->cart2D_grid[i];
		}

		for (cart2D_surface_index_data_type i = 0; i < cart2D_grid02->cart2D_grid.size(); i++) {
			cart2D_surface_vec_data_type gmc = cart2D_grid02->get_matrix_coordinate(i) + cart2D_grid02->global_grid_origin;

			cart2D_surface_index_data_type mgi = cart2D_grid_merged->get_cart2D_surface_matrix_data_index(gmc);

			cart2D_grid_merged[mgi] = cart2D_grid02->cart2D_grid[i];
		}

		return cart2D_grid_merged;
	}

	cart2D_grid_base_class<T>* create_cart2D_grid_partition(cart2D_grid_base_class<T>* cart2D_grid, cart2D_surface_vec_data_type min_grid_range, cart2D_surface_vec_data_type max_grid_range) {
		if (!cart2D_grid) { return nullptr; }

		// Create partitial grid to partition grids into
		cart2D_grid_base_class<T>* cart2D_grid_part = new cart2D_grid_base_class<T>;
		if (!cart2D_grid_part) { return nullptr; }
		if (!cart2D_grid_part->create_cart2D_grid(min_grid_range, max_grid_range, cart2D_grid->cart2D_size)) { return false; }

		for (cart2D_surface_index_data_type i = 0; i < cart2D_grid_part->cart2D_grid.size(); i++) {
			cart2D_surface_vec_data_type   gmc = cart2D_grid_part->get_matrix_coordinate(i) + cart2D_grid_part->global_grid_origin;

			cart2D_surface_index_data_type mgi = cart2D_grid->get_cart2D_surface_matrix_data_index(gmc);

			cart2D_grid_part[i] = cart2D_grid->cart2D_grid[mgi];
			cart2D_grid->invalidate_cart2D_grid_data_value(mgi);
		}

		return cart2D_grid_part;
	}

	bool merge_sub_cart2D_grid_to_parent(cart2D_grid_base_class<T>* cart2D_grid) {
		if (!cart2D_grid) { return false; }
		if (!cart2D_grid->parent_cart2D_grid) { return false; }

		for (cart2D_surface_index_data_type i = 0; i < cart2D_grid->cart2D_grid.size(); i++) {
			cart2D_surface_vec_data_type gmc = cart2D_grid->get_matrix_coordinate(i) + cart2D_grid->global_grid_origin;

			cart2D_surface_index_data_type mgi = cart2D_grid->parent->get_cart2D_surface_matrix_data_index(gmc);

			cart2D_grid->parent_cart2D_grid[mgi] = cart2D_grid->cart2D_grid[i];
		}

		delete_child_cart2D_grid(cart2D_grid->parent_cart2D_grid, cart2D_grid);
		cart2D_grid = nullptr;

		return true;
	}

	bool delete_child_cart2D_grid(cart2D_grid_base_class<T> *cart2D_grid, cart2D_grid_base_class<T> *child_cart2D_grid_to_delete) {
		if (!cart2D_grid || !child_cart2D_grid_to_delete) { return false; }

		for (cart2D_grid_base_class<T>* child_cart2D_grid : cart2D_grid->child_cart2D_grids) {
			if (child_cart2D_grid == child_cart2D_grid_to_delete) {
				delete(child_cart2D_grid_to_delete);
				return true;
				break;
			}
		}

		return false;
	}

	cart2D_grid_base_class<T>* get_child_grid_with_global_grid_coord(cart2D_grid_base_class<T>* cart2D_grid, cart2D_surface_vec_data_type global_grid_coordinate) {
		if (!cart2D_grid) { return nullptr; }
		if (cart2D_grid->child_cart2D_grids.size() == 0) { return nullptr; }

		for (cart2D_grid_base_class<T>* child_cart2D_grid : cart2D_grid->child_cart2D_grids) {
			if (child_cart2D_grid->grid_coordinate_in_cart2D_grid_bounds(global_grid_coordinate)) {
				return true;
				break;
			}
		}

		return nullptr;
	}

protected:


private:
	cart2D_surface_vec_data_type cart2D_grid01_min_range = {0,0};
	cart2D_surface_vec_data_type cart2D_grid01_max_range = {0,0};

	cart2D_surface_vec_data_type cart2D_grid02_min_range = {0,0};
	cart2D_surface_vec_data_type cart2D_grid02_max_range = {0,0};

	template <class T>
	void define_cart2D_grid_range(cart2D_grid_base_class<T>& cart2D_grid01, cart2D_grid_base_class<T>& cart2D_grid02) {
		cart2D_grid01_min_range = cart2D_grid01.global_grid_origin;
		cart2D_grid01_max_range = cart2D_grid01.global_grid_origin + cart2D_grid01.grid_dimension;

		cart2D_grid02_min_range = cart2D_grid02.global_grid_origin;
		cart2D_grid02_max_range = cart2D_grid02.global_grid_origin + cart2D_grid02.grid_dimension;
	}

};