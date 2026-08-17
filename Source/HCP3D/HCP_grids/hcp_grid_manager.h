#pragma once

#include "hcp_grid_base.h"

template <class T>
class hcp_grid_manager_class {
public:
	hcp_grid_manager_class() {}
	~hcp_grid_manager_class() {}

	bool hcp_grids_overlap(hcp_grid_base_class<T> &hcp_grid01, hcp_grid_base_class<T> &hcp_grid02) {
		if (!hcp_grid01 || !hcp_grid02) { return false; }
		if (hcp_grid01.null_dimension() || hcp_grid02.null_dimension()) { return false; }

		define_hcp_grid_range(hcp_grid01, hcp_grid02);
		
		if (hcp_grid01_min_range.x > hcp_grid02_max_range.x || hcp_grid02_min_range.x > hcp_grid01_max_range.x) { return false; }
		if (hcp_grid01_min_range.y > hcp_grid02_max_range.y || hcp_grid02_min_range.y > hcp_grid01_max_range.y) { return false; }
		if (hcp_grid01_min_range.z > hcp_grid02_max_range.z || hcp_grid02_min_range.z > hcp_grid01_max_range.z) { return false; }

		if (hcp_grid01_min_range.x <= hcp_grid02_max_range.x && hcp_grid01_min_range.x >= hcp_grid02_min_range.x){
			if (hcp_grid01_min_range.y <= hcp_grid02_max_range.y && hcp_grid01_min_range.y >= hcp_grid02_min_range.y) {return true;}
			if (hcp_grid01_max_range.y <= hcp_grid02_max_range.y && hcp_grid01_max_range.y >= hcp_grid02_min_range.y) {return true;}

			if (hcp_grid01_min_range.z <= hcp_grid02_max_range.z && hcp_grid01_min_range.z >= hcp_grid02_min_range.z) { return true; }
			if (hcp_grid01_max_range.z <= hcp_grid02_max_range.z && hcp_grid01_max_range.z >= hcp_grid02_min_range.z) { return true; }

			return false;
		}

		if (hcp_grid01_max_range.x <= hcp_grid02_max_range.x && hcp_grid01_max_range.x >= hcp_grid02_min_range.x) {
			if (hcp_grid01_min_range.y <= hcp_grid02_max_range.y && hcp_grid01_min_range.y >= hcp_grid02_min_range.y) {return true;}
			if (hcp_grid01_max_range.y <= hcp_grid02_max_range.y && hcp_grid01_max_range.y >= hcp_grid02_min_range.y) {return true;}

			if (hcp_grid01_min_range.z <= hcp_grid02_max_range.z && hcp_grid01_min_range.z >= hcp_grid02_min_range.z) { return true; }
			if (hcp_grid01_max_range.z <= hcp_grid02_max_range.z && hcp_grid01_max_range.z >= hcp_grid02_min_range.z) { return true; }

			return false;
		}

		return false;
	}

	bool hcp_grids_neighbours(hcp_grid_base_class<T> &hcp_grid01, hcp_grid_base_class<T> &hcp_grid02) {
		if (!hcp_grid01 || !hcp_grid02) { return false; }
		if (hcp_grid01.null_dimension() || hcp_grid02.null_dimension()) { return false; }

		define_hcp_grid_range(hcp_grid01,hcp_grid02);

		hcp_surface_index_data_type difference01 = hcp_grid02_min_range - hcp_grid01_max_range;
		if (difference01.x == 1 || difference01.y == 1 || difference01.z == 1) { return true; }

		hcp_surface_index_data_type difference02 = hcp_grid01_min_range - hcp_grid02_max_range;
		if (difference02.x == 1 || difference02.y == 1 || difference02.z == 1) { return true; }

		return false;
	}

	hcp_grid_base_class<T>* merge_hcp_grids(hcp_grid_base_class<T> *hcp_grid01, hcp_grid_base_class<T> *hcp_grid02) {
		if (!hcp_grid01 || !hcp_grid02) { return nullptr; }
		if (hcp_grid01->null_dimension() || hcp_grid02->null_dimension()) { return nullptr; }
		if (hcp_grid01->hcp_size != hcp_grid02->hcp_size) { return nullptr; }

		define_hcp_grid_range(hcp_grid01, hcp_grid02);

		hcp_surface_vec_data_type min_grid_range = { 0,0 };
		hcp_surface_vec_data_type max_grid_range = { 0,0 };

		// define min hax grid range
		if (hcp_grid01_min_range.x <= hcp_grid02_min_range.x) {
			min_grid_range.x = hcp_grid01_min_range.x;
		} else{
			min_grid_range.x = hcp_grid02_min_range.x;
		}

		if (hcp_grid01_min_range.y <= hcp_grid02_min_range.y) {
			min_grid_range.y = hcp_grid01_min_range.y;
		} else {
			min_grid_range.y = hcp_grid02_min_range.y;
		}

		if (hcp_grid01_min_range.z <= hcp_grid02_min_range.z) {
			min_grid_range.z = hcp_grid01_min_range.z;
		} else {
			min_grid_range.z = hcp_grid02_min_range.z;
		}

		// define max hax grid range
		if (hcp_grid01_max_range.x >= hcp_grid02_max_range.x) {
			max_grid_range.x = hcp_grid01_max_range.x;
		} else {
			max_grid_range.x = hcp_grid02_max_range.x;
		}

		if (hcp_grid01_max_range.y >= hcp_grid02_max_range.y) {
			max_grid_range.y = hcp_grid01_max_range.y;
		} else {
			max_grid_range.y = hcp_grid02_max_range.y;
		}

		if (hcp_grid01_max_range.z >= hcp_grid02_max_range.z) {
			max_grid_range.z = hcp_grid01_max_range.z;
		} else {
			max_grid_range.z = hcp_grid02_max_range.z;
		}

		// Create grid to merge grids into
		hcp_grid_base_class<T> *hcp_grid_merged = new hcp_grid_base_class<T>;
		if (!hcp_grid_merged) { return nullptr; }
		if (!hcp_grid_merged->create_hcp_grid(min_grid_range, max_grid_range, hcp_grid01->hcp_size)) { return false; }

		// Merge grids in order of least to highest grid index
		// Get indexes at which to merge grids
		//hcp_surface_index_data_type start_index_hcp_grid01 = hcp_grid01->origin_parent_hcp_grid_index - hcp_grid_merged->origin_parent_hcp_grid_index;
		//hcp_surface_index_data_type start_index_hcp_grid02 = hcp_grid02->origin_parent_hcp_grid_index - hcp_grid_merged->origin_parent_hcp_grid_index;

		for (hcp_surface_index_data_type i = 0; i < hcp_grid01->hcp_grid.size(); i++) {
			hcp_surface_vec_data_type gmc = hcp_grid01->get_matrix_coordinate(i) + hcp_grid01->global_grid_origin;

			hcp_surface_index_data_type mgi = hcp_grid_merged->get_hcp_surface_matrix_data_index(gmc);

			hcp_grid_merged[mgi] = hcp_grid01->hcp_grid[i];
		}

		for (hcp_surface_index_data_type i = 0; i < hcp_grid02->hcp_grid.size(); i++) {
			hcp_surface_vec_data_type gmc = hcp_grid02->get_matrix_coordinate(i) + hcp_grid02->global_grid_origin;

			hcp_surface_index_data_type mgi = hcp_grid_merged->get_hcp_surface_matrix_data_index(gmc);

			hcp_grid_merged[mgi] = hcp_grid02->hcp_grid[i];
		}

		return hcp_grid_merged;
	}

	hcp_grid_base_class<T>* create_hcp_grid_partition(hcp_grid_base_class<T>* hcp_grid, hcp_surface_vec_data_type min_grid_range, hcp_surface_vec_data_type max_grid_range) {
		if (!hcp_grid) { return nullptr; }

		// Create partitial grid to partition grids into
		hcp_grid_base_class<T>* hcp_grid_part = new hcp_grid_base_class<T>;
		if (!hcp_grid_part) { return nullptr; }
		if (!hcp_grid_part->create_hcp_grid(min_grid_range, max_grid_range, hcp_grid->hcp_size)) { return false; }

		for (hcp_surface_index_data_type i = 0; i < hcp_grid_part->hcp_grid.size(); i++) {
			hcp_surface_vec_data_type   gmc = hcp_grid_part->get_matrix_coordinate(i) + hcp_grid_part->global_grid_origin;

			hcp_surface_index_data_type mgi = hcp_grid->get_hcp_surface_matrix_data_index(gmc);

			hcp_grid_part[i] = hcp_grid->hcp_grid[mgi];
			hcp_grid->invalidate_hcp_grid_data_value(mgi);
		}

		return hcp_grid_part;
	}

	bool merge_sub_hcp_grid_to_parent(hcp_grid_base_class<T>* hcp_grid) {
		if (!hcp_grid) { return false; }
		if (!hcp_grid->parent_hcp_grid) { return false; }

		for (hcp_surface_index_data_type i = 0; i < hcp_grid->hcp_grid.size(); i++) {
			hcp_surface_vec_data_type gmc = hcp_grid->get_matrix_coordinate(i) + hcp_grid->global_grid_origin;

			hcp_surface_index_data_type mgi = hcp_grid->parent->get_hcp_surface_matrix_data_index(gmc);

			hcp_grid->parent_hcp_grid[mgi] = hcp_grid->hcp_grid[i];
		}

		delete_child_hcp_grid(hcp_grid->parent_hcp_grid, hcp_grid);
		hcp_grid = nullptr;

		return true;
	}

	bool delete_child_hcp_grid(hcp_grid_base_class<T> *hcp_grid, hcp_grid_base_class<T> *child_hcp_grid_to_delete) {
		if (!hcp_grid || !child_hcp_grid_to_delete) { return false; }

		for (hcp_grid_base_class<T>* child_hcp_grid : hcp_grid->child_hcp_grids) {
			if (child_hcp_grid == child_hcp_grid_to_delete) {
				delete(child_hcp_grid_to_delete);
				return true;
				break;
			}
		}

		return false;
	}

	hcp_grid_base_class<T>* get_child_grid_with_global_grid_coord(hcp_grid_base_class<T>* hcp_grid, hcp_surface_vec_data_type global_grid_coordinate) {
		if (!hcp_grid) { return nullptr; }
		if (hcp_grid->child_hcp_grids.size() == 0) { return nullptr; }

		for (hcp_grid_base_class<T>* child_hcp_grid : hcp_grid->child_hcp_grids) {
			if (child_hcp_grid->grid_coordinate_in_hcp_grid_bounds(global_grid_coordinate)) {
				return true;
				break;
			}
		}

		return nullptr;
	}

protected:


private:
	hcp_surface_vec_data_type hcp_grid01_min_range = {0,0};
	hcp_surface_vec_data_type hcp_grid01_max_range = {0,0};

	hcp_surface_vec_data_type hcp_grid02_min_range = {0,0,0};
	hcp_surface_vec_data_type hcp_grid02_max_range = {0,0,0};

	template <class T>
	void define_hcp_grid_range(hcp_grid_base_class<T>& hcp_grid01, hcp_grid_base_class<T>& hcp_grid02) {
		hcp_grid01_min_range = hcp_grid01.global_grid_origin;
		hcp_grid01_max_range = hcp_grid01.global_grid_origin + hcp_grid01.grid_dimension;

		hcp_grid02_min_range = hcp_grid02.global_grid_origin;
		hcp_grid02_max_range = hcp_grid02.global_grid_origin + hcp_grid02.grid_dimension;
	}

};