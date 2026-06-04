#pragma once

#include "hex_grid_base.h"

//#include "hex_grid_parameters.h"

template <class T>
class hex_grid_manager_class {
public:
	hex_grid_manager_class() {}
	~hex_grid_manager_class() {}

	bool hex_grids_overlap(hex_grid_base_class<T> &hex_grid01, hex_grid_base_class<T> &hex_grid02) {
		if (!hex_grid01 || !hex_grid02) { return false; }
		if (hex_grid01.null_dimension() || hex_grid02.null_dimension()) { return false; }

		define_hex_grid_range(hex_grid01, hex_grid02);
		
		if (hex_grid01_min_range.x > hex_grid02_max_range.x || hex_grid02_min_range.x > hex_grid01_max_range.x) { return false; }
		if (hex_grid01_min_range.y > hex_grid02_max_range.y || hex_grid02_min_range.y > hex_grid01_max_range.y) { return false; }

		if (hex_grid01_min_range.x <= hex_grid02_max_range.x && hex_grid01_min_range.x >= hex_grid02_min_range.x){
			if (hex_grid01_min_range.y <= hex_grid02_max_range.y && hex_grid01_min_range.y >= hex_grid02_min_range.y)
				{return true;}

			if (hex_grid01_max_range.y <= hex_grid02_max_range.y && hex_grid01_max_range.y >= hex_grid02_min_range.y)
				{return true;}

			return false;
		}

		if (hex_grid01_max_range.x <= hex_grid02_max_range.x && hex_grid01_max_range.x >= hex_grid02_min_range.x) {
			if (hex_grid01_min_range.y <= hex_grid02_max_range.y && hex_grid01_min_range.y >= hex_grid02_min_range.y)
				{return true;}

			if (hex_grid01_max_range.y <= hex_grid02_max_range.y && hex_grid01_max_range.y >= hex_grid02_min_range.y)
				{return true;}

			return false;
		}

		return false;
	}

	bool hex_grids_neighbours(hex_grid_base_class<T> &hex_grid01, hex_grid_base_class<T> &hex_grid02) {
		if (!hex_grid01 || !hex_grid02) { return false; }
		if (hex_grid01.null_dimension() || hex_grid02.null_dimension()) { return false; }

		define_hex_grid_range(hex_grid01,hex_grid02);

		hex_surface_index_data_type difference01 = hex_grid02_min_range - hex_grid01_max_range;
		if (difference01.x == 1 || difference01.y == 1) { return true; }

		hex_surface_index_data_type difference02 = hex_grid01_min_range - hex_grid02_max_range;
		if (difference02.x == 1 || difference02.y == 1) { return true; }

		return false;
	}

	hex_grid_base_class<T>* merge_hex_grids(hex_grid_base_class<T> *hex_grid01, hex_grid_base_class<T> *hex_grid02) {
		if (!hex_grid01 || !hex_grid02) { return nullptr; }
		if (hex_grid01->null_dimension() || hex_grid02->null_dimension()) { return nullptr; }
		if (hex_grid01->hex_size != hex_grid02->hex_size) { return nullptr; }

		define_hex_grid_range(hex_grid01, hex_grid02);

		hex_surface_vec_data_type min_grid_range = { 0,0 };
		hex_surface_vec_data_type max_grid_range = { 0,0 };

		// define min hax grid range
		if (hex_grid01_min_range.x <= hex_grid02_min_range.x) {
			min_grid_range.x = hex_grid01_min_range.x;
		} else{
			min_grid_range.x = hex_grid02_min_range.x;
		}

		if (hex_grid01_min_range.y <= hex_grid02_min_range.y) {
			min_grid_range.y = hex_grid01_min_range.y;
		} else {
			min_grid_range.y = hex_grid02_min_range.y;
		}

		// define max hax grid range
		if (hex_grid01_max_range.x >= hex_grid02_max_range.x) {
			max_grid_range.x = hex_grid01_max_range.x;
		} else {
			max_grid_range.x = hex_grid02_max_range.x;
		}

		if (hex_grid01_max_range.y >= hex_grid02_max_range.y) {
			max_grid_range.y = hex_grid01_max_range.y;
		} else {
			max_grid_range.y = hex_grid02_max_range.y;
		}

		// Create grid to merge grids into
		hex_grid_base_class<T> *hex_grid_merged = new hex_grid_base_class<T>;
		if (!hex_grid_merged) { return nullptr; }
		if (!hex_grid_merged->create_hex_grid(min_grid_range, max_grid_range, hex_grid01->hex_size)) { return false; }

		// Merge grids in order of least to highest grid index
		// Get indexes at which to merge grids
		//hex_surface_index_data_type start_index_hex_grid01 = hex_grid01->origin_parent_hex_grid_index - hex_grid_merged->origin_parent_hex_grid_index;
		//hex_surface_index_data_type start_index_hex_grid02 = hex_grid02->origin_parent_hex_grid_index - hex_grid_merged->origin_parent_hex_grid_index;

		for (hex_surface_index_data_type i = 0; i < hex_grid01->hex_grid.size(); i++) {
			hex_surface_vec_data_type gmc = hex_grid01->get_matrix_coordinate(i) + hex_grid01->global_grid_origin;

			hex_surface_index_data_type mgi = hex_grid_merged->get_hex_surface_matrix_data_index(gmc);

			hex_grid_merged[mgi] = hex_grid01->hex_grid[i];
		}

		for (hex_surface_index_data_type i = 0; i < hex_grid02->hex_grid.size(); i++) {
			hex_surface_vec_data_type gmc = hex_grid02->get_matrix_coordinate(i) + hex_grid02->global_grid_origin;

			hex_surface_index_data_type mgi = hex_grid_merged->get_hex_surface_matrix_data_index(gmc);

			hex_grid_merged[mgi] = hex_grid02->hex_grid[i];
		}

		return hex_grid_merged;
	}

	hex_grid_base_class<T>* create_hex_grid_partition(hex_grid_base_class<T>* hex_grid, hex_surface_vec_data_type min_grid_range, hex_surface_vec_data_type max_grid_range) {
		if (!hex_grid) { return nullptr; }

		// Create partitial grid to partition grids into
		hex_grid_base_class<T>* hex_grid_part = new hex_grid_base_class<T>;
		if (!hex_grid_part) { return nullptr; }
		if (!hex_grid_part->create_hex_grid(min_grid_range, max_grid_range, hex_grid->hex_size)) { return false; }

		for (hex_surface_index_data_type i = 0; i < hex_grid_part->hex_grid.size(); i++) {
			hex_surface_vec_data_type   gmc = hex_grid_part->get_matrix_coordinate(i) + hex_grid_part->global_grid_origin;

			hex_surface_index_data_type mgi = hex_grid->get_hex_surface_matrix_data_index(gmc);

			hex_grid_part[i] = hex_grid->hex_grid[mgi];
			hex_grid->invalidate_hex_grid_data_value(mgi);
		}

		return hex_grid_part;
	}

	bool merge_sub_hex_grid_to_parent(hex_grid_base_class<T>* hex_grid) {
		if (!hex_grid) { return false; }
		if (!hex_grid->parent_hex_grid) { return false; }

		for (hex_surface_index_data_type i = 0; i < hex_grid->hex_grid.size(); i++) {
			hex_surface_vec_data_type gmc = hex_grid->get_matrix_coordinate(i) + hex_grid->global_grid_origin;

			hex_surface_index_data_type mgi = hex_grid->parent->get_hex_surface_matrix_data_index(gmc);

			hex_grid->parent_hex_grid[mgi] = hex_grid->hex_grid[i];
		}

		delete_child_hex_grid(hex_grid->parent_hex_grid, hex_grid);
		hex_grid = nullptr;

		return true;
	}

	bool delete_child_hex_grid(hex_grid_base_class<T> *hex_grid, hex_grid_base_class<T> *child_hex_grid_to_delete) {
		if (!hex_grid || !child_hex_grid_to_delete) { return false; }

		for (hex_grid_base_class<T>* child_hex_grid : hex_grid->child_hex_grids) {
			if (child_hex_grid == child_hex_grid_to_delete) {
				delete(child_hex_grid_to_delete);
				return true;
				break;
			}
		}

		return false;
	}

	hex_grid_base_class<T>* get_child_grid_with_global_grid_coord(hex_grid_base_class<T>* hex_grid, hex_surface_vec_data_type global_grid_coordinate) {
		if (!hex_grid) { return nullptr; }
		if (hex_grid->child_hex_grids.size() == 0) { return nullptr; }

		for (hex_grid_base_class<T>* child_hex_grid : hex_grid->child_hex_grids) {
			if (child_hex_grid->grid_coordinate_in_hex_grid_bounds(global_grid_coordinate)) {
				return true;
				break;
			}
		}

		return nullptr;
	}

protected:


private:
	hex_surface_vec_data_type hex_grid01_min_range = {0,0};
	hex_surface_vec_data_type hex_grid01_max_range = {0,0};

	hex_surface_vec_data_type hex_grid02_min_range = {0,0};
	hex_surface_vec_data_type hex_grid02_max_range = {0,0};

	template <class T>
	void define_hex_grid_range(hex_grid_base_class<T>& hex_grid01, hex_grid_base_class<T>& hex_grid02) {
		hex_grid01_min_range = hex_grid01.global_grid_origin;
		hex_grid01_max_range = hex_grid01.global_grid_origin + hex_grid01.grid_dimension;

		hex_grid02_min_range = hex_grid02.global_grid_origin;
		hex_grid02_max_range = hex_grid02.global_grid_origin + hex_grid02.grid_dimension;
	}

};