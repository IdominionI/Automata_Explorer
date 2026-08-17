#pragma once

#include <vector>
#include <string>
#include <type_traits>

#include <Framework/AFW/Application/afw_global.h>
#include <FrameWork/AFW/Tools/afw_id_key_manager.h>

#include "cubic_grid_parameters.h"

#include <ThirdParty/ImGui/implot3D/implot3d.h>
#include <ThirdParty/ImGui/implot3D/implot3d_internal.h>

// Data structure to store and manage unordered hexagonal automata grid data and display that data 
// as a specific shape, size and color in the main hexagonal automata grid display. 

struct cubic_grid_cell_data_struct_type {
	int cubic_grid_cell_data_id = -1;
	bool display_data = false;

	std::vector<cubic_surface_index_data_type> cubic_grid_cells_index = {}; // Index of Hex grid cells to display data for
	std::vector<float> cubic_cell_centers_x = {};							// x coordinates of Hex grid cells to display data for
	std::vector<float> cubic_cell_centers_y = {};							// y coordinates of Hex grid cells to display data for
	std::vector<float> cubic_cell_centers_z = {};							// y coordinates of Hex grid cells to display data for

	ImVec4 cubic_grid_cells_display_color = { 1.0f,1.0f,1.0f,1.0f };		// colors of Hex grid cells to display data for
	int    display_shape_id = 1;										// shape id of Hex grid cells to display data for
	float  display_shape_size = 6.0f;									// shape size of Hex grid cells to display data for
	ImPlot3DMarker_ cubic_grid_cells_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Circle;// ImPlot shape of Hex grid cells to display data for

	// Clear all hexagonal grid data as being empty and ready the dynamic vector arrays to be reused
	void clear_cubic_grid_cell_data() {
//printf("cubic_grid_cell_data_struct_type::clear_cubic_grid_cell_data 000 %i : %i : %i |%i : %i : %i \n", cubic_grid_cells_index.size(), cubic_cell_centers_x.size(), cubic_cell_centers_y.size(), cubic_grid_cells_index.capacity(), cubic_cell_centers_x.capacity(), cubic_cell_centers_y.capacity());
		cubic_grid_cells_index.clear();
		cubic_cell_centers_x.clear();
		cubic_cell_centers_y.clear();
		cubic_cell_centers_z.clear();

//printf("cubic_grid_cell_data_struct_type::clear_cubic_grid_cell_data 111 %i : %i : %i|%i : %i : %i\n", cubic_grid_cells_index.size(), cubic_cell_centers_x.size(), cubic_cell_centers_y.size(), cubic_grid_cells_index.capacity(), cubic_cell_centers_x.capacity(), cubic_cell_centers_y.capacity());
		cubic_grid_cells_index.shrink_to_fit();
		cubic_cell_centers_x.shrink_to_fit();
		cubic_cell_centers_y.shrink_to_fit();
		cubic_cell_centers_z.shrink_to_fit();

//printf("cubic_grid_cell_data_struct_type::clear_cubic_grid_cell_data 222 %i : %i : %i|%i : %i : %i\n", cubic_grid_cells_index.size(), cubic_cell_centers_x.size(), cubic_cell_centers_y.size(), cubic_grid_cells_index.capacity(), cubic_cell_centers_x.capacity(), cubic_cell_centers_y.capacity());
	}

	void add_cubic_grid_cell(cubic_surface_index_data_type cubic_grid_cell_index, float x_coord, float y_coord, float z_coord) {
		add_cubic_grid_cell(cubic_grid_cell_index);
		add_cubic_grid_cell_cartesian_coordinates(x_coord, y_coord, z_coord);
	}

	void add_cubic_grid_cell(cubic_surface_index_data_type cubic_grid_cell_index) {
		cubic_grid_cells_index.push_back(cubic_grid_cell_index);
	}

	void add_cubic_grid_cell_cartesian_coordinates(float x_coord, float y_coord, float z_coord) {
		cubic_cell_centers_x.push_back(x_coord);
		cubic_cell_centers_y.push_back(y_coord);
		cubic_cell_centers_z.push_back(z_coord);
	}

	bool display_cubic_cell_data() {
		// This display_cubic_cell_data() function must be called within a ImGui code block similar to
		//      
		// ImGui::Begin("Int Hex Grid");// If Have name of ImGui::Begin then have ability to minimise and dock 
		//      if (ImPlot::BeginPlot("Test Hex Grid")) {
		//
		//			for (cubic_grid_cell_data_struct_type cubic_grid_cell : cubic_grid_cells) {
		//				cubic_grid_cell.display_cubic_cell_data();
		//			}
		// 
		//            ImPlot::EndPlot();
		//	}
		// ImGui::End();
//printf("cubic_grid_cell_data_struct_type :: display_cubic_cell_data 00 %i : %i : %i \n", cubic_grid_cell_data_id, cubic_grid_cells_index.size(), cubic_cell_centers_x.size());

		if (!display_data) {
//printf("cubic_grid_cell_data_struct_type :: display_cubic_cell_data 00 !display_data \n");
			return false;
		}

		if (cubic_grid_cell_data_id < 0) {
//printf("cubic_grid_cell_data_struct_type :: display_cubic_cell_data 11 cubic_grid_cell_data_id < 0 \n");
			return false;
		}
		if (cubic_cell_centers_x.size() < 1 || cubic_cell_centers_y.size() < 1 || cubic_cell_centers_z.size() < 1) {
//printf("cubic_grid_cell_data_struct_type :: display_cubic_cell_data 22 cubic_cell_centers_x.size() < 1 || cubic_cell_centers_y.size() < 1 \n");		
			return false;
		}

//for (int i = 0; i < cubic_cell_centers_x.size(); i++) {
//	printf("cubic_grid_cell_data_struct_type :: display_cubic_cell_data 00 %i : %f : %f \n", cubic_grid_cell_data_id, cubic_cell_centers_x[i], cubic_cell_centers_y[i]);
//}

		std::string scatter_plot_id = "Result Grid" + std::to_string(cubic_grid_cell_data_id);

		ImPlot3DSpec spec;
		//ImPlotProp_Marker, ImPlotMarker_Circle, 
		//ImPlotProp_Marker, ImPlotMarker_Square,// use this when testing
		spec.Marker			 = cubic_grid_cells_display_shape;
		spec.MarkerSize		 = display_shape_size;
		spec.MarkerLineColor = cubic_grid_cells_display_color;
		spec.MarkerFillColor = cubic_grid_cells_display_color;
		spec.FillAlpha = 0.25f;
		ImPlot3D::PlotScatter(scatter_plot_id.c_str(), cubic_cell_centers_x.data(), cubic_cell_centers_y.data(), cubic_cell_centers_z.data(), cubic_cell_centers_x.size(),spec);

		return true;
	}
};

// Data structure to store and manage ordered hexagonal automata sub grid data of the main hexagonal grid and display that data 

struct cubic_sub_grid_struct_type {
	int cubic_grid_range_id = -1;

	cubic_sub_grid_struct_type() {}
	cubic_sub_grid_struct_type(cubic_surface_vec_data_type lr, cubic_surface_vec_data_type ur) : lower_range(lr), upper_range(ur) {}

	cubic_surface_vec_data_type lower_range = { 0,0,0 }; // Lower left bounds of sub grid
	cubic_surface_vec_data_type upper_range = { 0,0,0 }; // upper right bounds of sub grid

	std::vector<float> sub_cubic_centers_x = {}; // x coordinates of Hex grid cells to display data for
	std::vector<float> sub_cubic_centers_y = {}; // y coordinates of Hex grid cells to display data for
	std::vector<float> sub_cubic_centers_z = {}; // y coordinates of Hex grid cells to display data for

	cubic_surface_index_data_type child_cubic_origin_index = -1; // Index of the main hexagonal grid that the origin 0 index that the sub grid references

	// Function to test if sub hexagonal grid has a valid range over a hexagonal grid of passed origin and grid extent 
	bool valid_range(cubic_surface_vec_data_type origin, cubic_surface_vec_data_type extent) {
		if (lower_range.x < origin.x || lower_range.x > extent.x) { return false; }
		if (lower_range.y < origin.y || lower_range.y > extent.y) { return false; }
		if (lower_range.z < origin.z || lower_range.z > extent.z) { return false; }

		if (upper_range.x < origin.x || upper_range.x > extent.x) { return false; }
		if (upper_range.y < origin.y || upper_range.y > extent.y) { return false; }
		if (upper_range.z < origin.z || upper_range.z > extent.z) { return false; }

		return true;
	}

	// Function to test if sub hexagonal grid is of a null or zero dimension
	bool null_dimension() {
		if (upper_range.x == lower_range.x || upper_range.y == lower_range.y || upper_range.z == lower_range.z) { return false; }

		return true;
	}

	// Function to test if hexagonal grid corrdinate is within the hexagonal sub grid bounds
	bool grid_coordinate_in_sub_cubic_bounds(cubic_surface_vec_data_type grid_coordinate) {
		if (grid_coordinate.x < lower_range.x || grid_coordinate.x > upper_range.x) { return false; }
		if (grid_coordinate.y < lower_range.y || grid_coordinate.y > upper_range.y) { return false; }
		if (grid_coordinate.z < lower_range.z || grid_coordinate.z > upper_range.z) { return false; }

		return true;
	}

	bool display_sub_cubic_grid() {
		// This display_sub_cubic_grid() function must be called within a ImGui code block similar to
		//      
		// ImGui::Begin("Int Hex Grid");// If Have name of ImGui::Begin then have ability to minimise and dock 
		//      if (ImPlot::BeginPlot("Test Hex Grid")) {
		//
		//			for (cubic_sub_grid_struct_type cubic_sub_grid : cubic_sub_grids) {
		//				cubic_sub_grid.display_sub_cubic_grid();
		//			}
		// 
		//            ImPlot::EndPlot();
		//	}
		// ImGui::End();

//printf("cubic_sub_grid_struct_type :: display_sub_cubic_grid 00"); printf(" %i :: %i \n", cubic_grid_range_id, sub_cubic_centers_x.size());

		if (cubic_grid_range_id < 0) { return false; }
		if (sub_cubic_centers_x.size() < 1 || sub_cubic_centers_y.size() < 1 || sub_cubic_centers_z.size() < 1) { return false; }

		std::string scatter_plot_id = "sub Grid" + std::to_string(cubic_grid_range_id); // Change this for user to define sub grid description

		ImPlot3DSpec spec;
		//ImPlotProp_Marker, ImPlotMarker_Circle, 
		//ImPlotProp_Marker, ImPlotMarker_Square,// use this when testing
		spec.Marker			 = ImPlot3DMarker_Square;
		spec.MarkerSize		 = 6;
		spec.MarkerLineColor = ImPlot3D::GetColormapColor(cubic_grid_range_id);
		spec.MarkerFillColor = ImPlot3D::GetColormapColor(cubic_grid_range_id);
		spec.FillAlpha = 0.25f;
		ImPlot3D::PlotScatter(scatter_plot_id.c_str(), sub_cubic_centers_x.data(), sub_cubic_centers_y.data(), sub_cubic_centers_z.data(), sub_cubic_centers_x.size(),spec);

		return true;
	}
};

// Base class upon which all hexagonal automata grids use to define, display and manage hexagonal grid data, 

template <class T>
class cubic_grid_base_class {
public:
	cubic_grid_base_class() {}
	cubic_grid_base_class(T initial_cubic_value, cubic_surface_vec_data_type dimension) {
		define_cubic_grid(initial_cubic_value, dimension);
	}
	~cubic_grid_base_class() { delete_cubic_grid(); }

	std::vector<cubic_sub_grid_struct_type> cubic_sub_grids = {}; // List of hexagonal sub grids that are bound to this cubic_grid_base_class object
	cubic_grid_value_data_type_enum		  cubic_grid_value_data_type = cubic_grid_value_data_type_enum::nan; // The datatype of the hexagonal grid of this cubic_grid_base_class object

	int cubic_grid_id = -1;			// Hex grid id

	// ------------- Hexagonal Grid definition --------------------
	glm::vec3				  global_cart_origin = { 0.0f,0.0f,0.0f }; // Global Cartesian coordinate of the grid origin 
	cubic_surface_vec_data_type global_grid_origin = { 0,0,0 };		// Global Index origin that is of the top parent hexagonal grid.

	cubic_surface_vec_data_type grid_dimension = { 0,0,0 };     // Dimensions of the hex grid

	// ------------- Hexagonal Grid data Storage --------------------
	std::vector<T> cubic_grid = {};			// hexgrid cell data

	float cubic_size = 1.0f;					// hex grid cell display size

	std::vector<float> cubic_centers_x = {};   // Hex grid cell Cartesian x coordinates
	std::vector<float> cubic_centers_y = {};   // Hex grid cell Cartesian y coordinates
	std::vector<float> cubic_centers_z = {};   // Hex grid cell Cartesian y coordinates

	ImVec4 cubic_grid_display_color = { 0.5f,0.5f,1.0f,1.0f };
	std::vector<ImU32> cubic_colors = {};		// Hex grid cell color data
	// --------------------------------------------------------

	bool define_cubic_grid_coordinates() {
		if (cubic_grid.size() == 0) { return false; }

		ImU32 icu32 = ImGui::GetColorU32(cubic_grid_display_color);

//printf("cubic_grid_base_class :: define_cubic_grid_coordinates : %i\n", cubic_grid.size());
		for (int i = 0; i < cubic_grid.size(); i++) {
			glm::vec3 cubic_center = get_cubic_surface_world_cartesian_coordinate(i);
			cubic_centers_x.push_back(cubic_center.x);
			cubic_centers_y.push_back(cubic_center.y);
			cubic_centers_z.push_back(cubic_center.z);

			cubic_colors.push_back(icu32);
//printf("cubic_grid_base_class :: define_cubic_grid_coordinates %i : %f: %f : %f \n",i , cubic_center.x, cubic_center.y, cubic_center.z);
		}

		return true;
	}

	// Virtual functions that all derived classes must have defined accordint to the datatype that they are defined as
	virtual void delete_cubic_grid_data(cubic_surface_index_data_type i) = 0;
	virtual void invalidate_cubic_grid_data_value(cubic_surface_index_data_type index) = 0;
	virtual bool display_cubic_grid() = 0;

	T get_initial_cubic_grid_value() { return initial_cubic_data_value; }

	void define_initial_cubic_data_value(T value) { initial_cubic_data_value = value; }

	void reinitialise_cubic_grid_data_values() {
		delete_cubic_grid();
		define_cubic_grid();
	}

	void reinitialise_cubic_grid_data_values(T value) {
		delete_cubic_grid();
		define_cubic_grid(value, grid_dimension);
	}

	bool null_dimension() {// Test to determin if have grid of zero dimension on one axis
		if (grid_dimension.x < 1 || grid_dimension.y < 1 || grid_dimension.z < 1) {
			return false;
		}
		else {
			return true;
		}
	}

	// ---------------------------------------------------------------------------------
	// Functions to calculate the voxel matrix index of the storage vector that is a container or storage of a voxel cell of grid matrix coordinate (i,j,k)
	// See documentation vw_CUBIC_voxel_draft_01 for more details and explaination how these alogorithims were derived

	// Calculate the index of the  C++ vector modelling the voxel matrix value of layer iZ of a voxel that has coordinate (iX,iY,iZ)
	cubic_surface_index_data_type get_z_layer_index_value(cubic_surface_index_data_type iX, cubic_surface_index_data_type iY, cubic_surface_index_data_type iZ) {
		return (grid_dimension.x * iY) + iX;
	}

	// Functions to calculate/retrieve hexagonal grid index from x-y grid index coordinates 
	cubic_surface_index_data_type get_index_value(cubic_surface_index_data_type iX, cubic_surface_index_data_type iY, cubic_surface_index_data_type iZ) {
		return  get_z_layer_total(iZ)*iZ + get_z_layer_index_value(iX, iY, iZ);
	}

	cubic_surface_index_data_type get_z_layer_total(cubic_surface_index_data_type iZ, cubic_surface_index_data_type xdim, cubic_surface_index_data_type ydim) {
		return xdim * ydim;
	}

	cubic_surface_index_data_type get_z_layer_total(cubic_surface_index_data_type iZ) {
		return get_z_layer_total(iZ,grid_dimension.x, grid_dimension.y);
	}

	// calculate the total number of voxels cells that exist within a voxel matrix
	cubic_surface_index_data_type calculate_cubic_surface_matrix_data_size(cubic_surface_index_data_type xdim, cubic_surface_index_data_type ydim, cubic_surface_index_data_type zdim) {
		return  xdim * ydim * zdim;
	}

	//cubic_surface_index_data_type calculate_cubic_surface_matrix_data_size() {
	//	return calculate_cubic_surface_matrix_data_size(grid_dimension.x, grid_dimension.y, grid_dimension.z);
	//}

	// END hex surface matrix index 
	// ---------------------------------------------------------------------------------

	// ###################
	//Create Hex grid of x-y index coordinate range with specified initial value and hex grid size
	bool create_cubic_grid(T initial_cubic_data_value, cubic_surface_vec_data_type min_range, cubic_surface_vec_data_type max_range, float cubic_size) {
		float x_iextent = max_range.x - min_range.x;
		float y_iextent = max_range.y - min_range.y;
		float z_iextent = max_range.z - min_range.z;

		grid_dimension = { x_iextent,y_iextent,z_iextent };

		global_grid_origin = min_range;
		global_cart_origin = get_cubic_surface_cartesian_coordinate(min_range, cubic_size);

		return define_cubic_grid(initial_cubic_data_value, grid_dimension);
	}

	// For a given set of limits of a Cartesian rectangular volume, calculate the dimensions of a voxel
// matrix that fits within the limits of this rectangular volume.

	glm::ivec3 calc_voxel_volume_dimensions(T initial_cubic_data_value, glm::vec3 min_range, glm::vec3 max_range, float cubic_size) {
		float x_extent = max_range.x - min_range.x;
		float y_extent = max_range.y - min_range.y;
		float z_extent = max_range.z - min_range.z;
		float z_mult = 2.0f * sqrt(6.0f) / 3.0f;

		float x_res_step = cubic_size * 2.0f;
		float y_res_step = cubic_size * (3.0f / sqrt(3.0f));
		//float z_res_step = voxel_generator_parameters.resolution_step * z_mult;
		float z_res_step = z_mult;

		cubic_surface_index_data_type data_set_x_size, data_set_y_size, data_set_z_size;

		if (x_extent / x_res_step - float((cubic_surface_index_data_type)(x_extent / x_res_step)) > 0.0f)
		{
			data_set_x_size = (cubic_surface_index_data_type)(x_extent / x_res_step) + 1;
		}
		else
		{
			data_set_x_size = (cubic_surface_index_data_type)(x_extent / x_res_step);
		}

		if (y_extent / y_res_step - float((cubic_surface_index_data_type)(y_extent / y_res_step)) > 0.0f)
		{
			data_set_y_size = (cubic_surface_index_data_type)(y_extent / y_res_step) + 1;
		}
		else
		{
			data_set_y_size = (cubic_surface_index_data_type)(y_extent / y_res_step);
		}

		if (z_extent / z_res_step - float((cubic_surface_index_data_type)(z_extent / z_res_step)) > 0.0)
			data_set_z_size = (cubic_surface_index_data_type)(z_extent / z_res_step) + 1;
		else
			data_set_z_size = (cubic_surface_index_data_type)(z_extent / z_res_step);

		return { data_set_x_size,data_set_y_size };
	}

	//Create Hex grid of x-y Cartesian coordinate range with specified initial value and hex grid size
	bool create_cubic_grid(T initial_cubic_data_value, glm::vec3 min_range, glm::vec3 max_range, float cubic_size) {
		grid_dimension = calc_voxel_volume_dimensions(initial_cubic_data_value, min_range, max_range, cubic_size);

		global_cart_origin = min_range;
		global_grid_origin = min_range;
		global_cart_origin = get_cubic_surface_cartesian_coordinate(min_range, cubic_size);

		return define_cubic_grid(initial_cubic_data_value, grid_dimension);
	}

	// create hex sub grid of specified x-y index coordinate range
	int create_cubic_sub_grid(cubic_surface_vec_data_type min_range, cubic_surface_vec_data_type max_range) {
		cubic_sub_grid_struct_type cubic_sub_grid(min_range, max_range);

		if (!cubic_sub_grid.valid_range(global_grid_origin, global_grid_origin + grid_dimension)) {
			return -1;
		}

		cubic_sub_grid.cubic_grid_range_id = cubic_sub_grid_id_key.get_available_id_key();

		cubic_sub_grids.push_back(cubic_sub_grid);

		return cubic_sub_grid.cubic_grid_range_id;
	}

	// create hex sub grid of specified x-y-z index coordinate range and value
	bool create_cubic_sub_grid(T initial_cubic_value, cubic_surface_vec_data_type min_range, cubic_surface_vec_data_type max_range) {
/* TO BE IMPLEMENTED !!!!!!!!!!!!!!!
		if (create_cubic_sub_grid(min_range, max_range) < 0) {
			return false;
		}

		size_t num_rows = max_range.y - min_range.y + 1;
		size_t num_cols = max_range.x - min_range.x + 1;

		cubic_surface_index_data_type child_cubic_origin_index = get_cubic_surface_matrix_data_index(min_range);

		size_t index_offset = 0;
		for (size_t row = 0; row < num_rows; row++) {
			size_t max_cols = 0;
			if (row % 2 == 0) {// even row
				if (num_cols == 1)
				{
					max_cols = 1;
				}
				else
				{
					max_cols = num_cols - 1;
				}
			}
			else { // odd row
				if (num_cols <= 2)
				{
					max_cols = 1;
				}
				else
				{
					max_cols = num_cols - 2;
				}
			}

			for (size_t col = 0; col < max_cols; col++) {
				cubic_surface_index_data_type cubic_index = child_cubic_origin_index + index_offset + col;

				cubic_grid[cubic_index] = initial_cubic_value;
			}

			if (row % 2 == 0) {// even row
				index_offset += grid_dimension.x;
			}
			else { // odd row
				index_offset += grid_dimension.x - 1;
			}

		}
*/
		return true;
	}

	//Create Hex grid of x-y dimension with specified initial value
	bool define_cubic_grid(T initial_cubic_data_value, cubic_surface_vec_data_type dimension) {
		define_initial_cubic_data_value(initial_cubic_data_value);
		return define_cubic_grid(dimension);
	}

	//Create Hex grid of x-y dimension with currently defined initial value
	bool define_cubic_grid(cubic_surface_vec_data_type dimension) {
		if (!dimension.x || !dimension.y || !dimension.z) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "Define hex grid :: Cannot define hex grid having a zero dimension\n");
			return false;
		}

		cubic_surface_index_data_type surface_grid_data_size = calculate_cubic_surface_matrix_data_size(dimension.x, dimension.y, dimension.z);
printf("cubic_grid_base_class :: define_cubic_grid 0000 %i:%i:%i:%i\n", dimension.x, dimension.y, dimension.z, surface_grid_data_size);

		if (surface_grid_data_size > MAX_PEMISSIBLE_CUBIC_GRID_SIZE) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "Define hCP grid :: Cannot define hex grid. CUBIC grid has dimensions that are constitute a cubic grid beyond permisible max size.\n");
			return false;
		}

		cubic_grid = std::vector<T>(surface_grid_data_size, initial_cubic_data_value);
		//cubic_grid = std::vector<T>(7800, initial_cubic_data_value);
		grid_dimension = dimension;
printf("cubic_grid_base_class :: define_cubic_grid 11111 %i : %i\n", surface_grid_data_size, cubic_grid.size());

		if (std::is_same<T, bool>::value) { cubic_grid_value_data_type = cubic_grid_value_data_type_enum::Boolean; }
		if (std::is_same<T, int>::value) { cubic_grid_value_data_type = cubic_grid_value_data_type_enum::Integer; }
		if (std::is_same<T, float>::value) { cubic_grid_value_data_type = cubic_grid_value_data_type_enum::Float; }
		if (std::is_same<T, double>::value) { cubic_grid_value_data_type = cubic_grid_value_data_type_enum::Double; }

		return true;
	}

	// Clear hex grid of all data values and initilaise it ready to be reused
	// Need to rename it to clear_cubic_grid()??????
	void delete_cubic_grid() {
		// Including any of the following crashes application in certain circumstances with error ocurring in the
		// for  or delete_cubic_grid_data(f) statements.
		// Do know why since nothing seems to be wrong with the for statement and the delete_cubic_grid_data();
		// statement is a dummy C++ stub for possible future implementation. Very strange !!!!!!!
		// Left commented out as has no impact or need. 
		//for (size_t i = 0; i < cubic_grid.size();i++) {
		//for (cubic_surface_index_data_type i = cubic_grid.size()-1; i>=0;i--) {
		//for (cubic_surface_index_data_type f = 5; f>=0;f--) {
			//delete_cubic_grid_data(f);
		//}
printf("cubic_grid_base_class :: delete_cubic_grid 000"); printf(" %zu\n", cubic_grid.size());

		cubic_centers_x.clear();
		cubic_centers_y.clear();
		cubic_centers_z.clear();
		cubic_colors.clear();

		cubic_grid.clear();

		cubic_centers_x.shrink_to_fit();
		cubic_centers_y.shrink_to_fit();
		cubic_centers_z.shrink_to_fit();
		cubic_colors.shrink_to_fit();

		cubic_grid.shrink_to_fit();
printf("cubic_grid_base_class :: delete_cubic_grid 111"); printf(" %zu\n", cubic_grid.size());
	}

	//bool resize_cubic_grid() {
	//	return resize_cubic_grid(grid_dimension);
	//}

	//bool resize_cubic_grid(cubic_surface_vec_data_type dimension) {
	//	if (!dimension.x || !dimension.y) {
	//	// Error message that have zero dimension grid
	//		return false;
	//	}

	//	size_t surface_grid_data_size = calculate_cubic_surface_matrix_data_size(dimension.x, dimension.y, dimension.z);

	//	if (surface_grid_data_size > MAX_PEMISSIBLE_HEX_GRID_SIZE) {
	//		// Error message that have hex grid size too large
	//		return false;
	//	}

	//	cubic_grid.resize(surface_grid_data_size);
	//	grid_dimension = dimension;

	//	return true;
	//}

	// ###########################################################################

	// Retreive the 3 dim x,y,z coordinate that the matrix index corresponds to
	cubic_surface_vec_data_type get_matrix_coordinate(cubic_surface_index_data_type matrix_index) {
		cubic_surface_index_data_type lt = get_z_layer_total(0);// total number for even z level

		cubic_surface_index_data_type z_index = cubic_surface_index_data_type((matrix_index / lt));
		cubic_surface_index_data_type y_index = cubic_surface_index_data_type((matrix_index - (z_index * lt))/grid_dimension.x);
		cubic_surface_index_data_type x_index = cubic_surface_index_data_type(matrix_index - ((z_index * lt) + (y_index * grid_dimension.x)));

		return { x_index ,y_index ,z_index };
	}

	// Retrieve the Cartesian coord of the hex grid x-y index coordinate cubic_surface_coord and hex grid cell size cubic_size
	glm::vec3 get_cubic_surface_cartesian_coordinate(cubic_surface_vec_data_type cubic_surface_coord, float cubic_size) {
		cubic_surface_index_data_type i = cubic_surface_coord.x, j = cubic_surface_coord.y, k = cubic_surface_coord.z;

		glm::vec3 cubic_cartesian_coordinate;

		cubic_cartesian_coordinate.x = float(i) * cubic_size;
		cubic_cartesian_coordinate.y = float(j) * cubic_size;
		cubic_cartesian_coordinate.z = float(k) * cubic_size;

		return cubic_cartesian_coordinate;
	}

	// Retrieve the Cartesian coord of the hex grid x-y index coordinate cubic_surface_coord
	glm::vec3 get_cubic_surface_cartesian_coordinate(cubic_surface_vec_data_type cubic_surface_coord) {
		return get_cubic_surface_cartesian_coordinate(cubic_surface_coord, cubic_size);
	}

	// Retrieve the global Cartesian coord of the hex grid x-y index coordinate cubic_surface_coord
	glm::vec3 get_cubic_surface_world_cartesian_coordinate(cubic_surface_vec_data_type voxel_matrix_coord) {
		glm::vec3  voxel_cart_coord = get_cubic_surface_cartesian_coordinate(voxel_matrix_coord) + global_cart_origin;

		return voxel_cart_coord;
	}

	// Retrieve the Cartesian coord of the hex grid cell of index
	glm::vec3 get_cubic_surface_world_cartesian_coordinate(cubic_surface_index_data_type index) {
		cubic_surface_vec_data_type voxel_matrix_coord = get_matrix_coordinate(index);
//printf("get_cubic_surface_world_cartesian_coordinate index 000 : %i : %i : %i \n",index, voxel_matrix_coord.x, voxel_matrix_coord.y);

		glm::vec3  voxel_cart_coord = get_cubic_surface_cartesian_coordinate(voxel_matrix_coord) + global_cart_origin;
//printf("get_cubic_surface_world_cartesian_coordinate index 111 : %f : %f : %f : %f\n", voxel_cart_coord.x, voxel_cart_coord.y, global_cart_origin.x, global_cart_origin.y);

		return voxel_cart_coord;
	}

	// Retrieve the hex grid index coordinate that corresponds to the hex x-y index coordinate matrix_coord
	cubic_surface_index_data_type get_cubic_surface_matrix_data_index(cubic_surface_vec_data_type matrix_coord) {// y in matrix_coord must be the corrected_y as defined in get_cubic_surface_matrix_bit_location
		return get_index_value(matrix_coord.x, matrix_coord.y, matrix_coord.z);
	}
	// ###########################################################################
	// ++++++++++++++++++++ FUNCTIONS TO FIND HEX STORAGE INDEX LOCATIONS OF   +++++++++++++++++++++
	// ++++++++++++++++++++    A CARTESIAN COORDINATE POINT P IN 2D SPACE      +++++++++++++++++++++

	/*
		Following code is to find which hex cell of a defined hex size, origin and dimensions
		that a point P of Cartesian coordinate (x,y) is to occupy.

		!!!!!!!!!!!!!! NOT YET FULLY TESTED !!!!!!!!!!!!!!!

		Usage

		//Get the index of the one dimensiona vector array that the point P of coordinate (x,y)
		//is designated to be assigned to
		cubic_surface_index_data_type index = index_of_cubic_cell_with_cartesian_coord(x,y);

		if index > -1 then perform whatever task is required to assign a value to the vector array element
		cubic_surface_matrix_data[index];
	*/

	// Find the index of the one dimensional vertex vector array that a point P of cartesian coordinte
	// (x,y) will be within the bounds of a 2D hexagon voxel cell.
	cubic_surface_index_data_type index_of_cubic_cell_with_cartesian_coord(float x, float y, float z) {
		cubic_surface_vec_data_type cubic_coord = cubic_cell_coord_from_cartesian(x, y, z);

		if (cartesian_coord_within_grid_bounds(cubic_coord))
		{
			return get_index_value(cubic_coord.x, cubic_coord.y, cubic_coord.z);
		}
		else
		{
			return -1;
		}
	}

	// Determine if a point P of cartesian coordinte (x,y) is within the limits of
	// the dimensions of the hexagonal grid that is stored in the computer memory
	bool cartesian_coord_within_grid_bounds(float x, float y, float z) {
		glm::ivec3 cubic_coord = cubic_cell_coord_from_cartesian(x, y, z);

		return cartesian_coord_within_grid_bounds(cubic_coord);
	}

	// Determine if a point P of hex grid x-y index coordinates is within the limits of
	// the dimensions of the hexagonal grid that is stored in the computer memory
	bool cartesian_coord_within_grid_bounds(cubic_surface_vec_data_type cubic_coord) {
		if (cubic_coord.x < 0 || cubic_coord.y < 0 || cubic_coord.z < 0) { return false; }

		if (cubic_coord.x >= grid_dimension.x || cubic_coord.y >= grid_dimension.y || cubic_coord.z >= grid_dimension.z) return false;

		return true;
	}

	// Obtain the hex grid index coordinates of the hex grid that a point P of cartesian coordinte
	// (x,y,z) will be found to be within the bounds of a 2D hexgagon voxel cell.
	cubic_surface_vec_data_type cubic_cell_coord_from_cartesian(float x, float y, float z) {
		float voxel_radius = cubic_size/2.0f;

		float grid_x = x - global_cart_origin.x;
		float grid_y = y - global_cart_origin.y;
		float grid_z = z - global_cart_origin.z;

		if (grid_x < -voxel_radius || grid_y < -voxel_radius || grid_z < -voxel_radius) { return { -1,-1,-1 }; } // No negative values permited in cubic automata

		cubic_surface_index_data_type level  = (int) std::floor((grid_z + voxel_radius) / cubic_size);
		cubic_surface_index_data_type row    = (int) std::floor((grid_y + voxel_radius) / cubic_size);
		cubic_surface_index_data_type column = (int) std::floor((grid_x + voxel_radius) / cubic_size);

//printf("cubic_grid_base:cubic_cell_coord_from_cartesian : %.3f : %i : %i\n", grid_z, level, level_is_even);

		return { column, row, level };

	}

	// ###########################################################################

	// Calculate the distance to a plane in 3D space from a point P with directional vector pq_vector to a point
	// Q on a plane that has a normal vector on the plane normal_vector
	float distance_to_plane(glm::vec3 normal_vector, glm::vec3 pq_vector) {
		float normal_vector_length = sqrt(normal_vector.x* normal_vector.x+normal_vector.y* normal_vector.y+ normal_vector.z* normal_vector.z);
		float normal_dot_pq        = dot(pq_vector, normal_vector);

//printf("distance_to_plane : %f : %f :%f \n", normal_vector_length, normal_dot_pq,normal_dot_pq / normal_vector_length);
		return normal_dot_pq / normal_vector_length;
	}


	// Test  to see if hex grid cell of x-y index coordinate grid_coordinate is within a hex grid cubic_grid
	bool grid_coordinate_in_cubic_grid_bounds(cubic_grid_base_class* cubic_grid, cubic_surface_vec_data_type grid_coordinate) {
		if (!cubic_grid) return false;
		if (cubic_grid->grid_dimension.x < 1 || cubic_grid->grid_dimension.y < 1 || cubic_grid->grid_dimension.z < 1) { return false; }

		if (grid_coordinate.y < cubic_grid->global_grid_origin.y || grid_coordinate.y > cubic_grid->global_grid_origin.y + cubic_grid->grid_dimension.y) { return false; }
		if (grid_coordinate.x < cubic_grid->global_grid_origin.x || grid_coordinate.x > cubic_grid->global_grid_origin.x + cubic_grid->grid_dimension.x) { return false; }
		if (grid_coordinate.z < cubic_grid->global_grid_origin.z || grid_coordinate.z > cubic_grid->global_grid_origin.z + cubic_grid->grid_dimension.z) { return false; }

		return true;
	}

	// Test  to see if hex grid cell of x-y index coordinate grid_coordinate is within this class hex grid
	bool grid_coordinate_in_cubic_grid_bounds(cubic_surface_vec_data_type grid_coordinate) {
		if (grid_dimension.x < 1 || grid_dimension.y < 1 || grid_dimension.y < 1) { return false; }

		if (grid_coordinate.y < global_grid_origin.y || grid_coordinate.y > global_grid_origin.y + grid_dimension.y) { return false; }
		if (grid_coordinate.x < global_grid_origin.x || grid_coordinate.x > global_grid_origin.x + grid_dimension.x) { return false; }
		if (grid_coordinate.z < global_grid_origin.z || grid_coordinate.z > global_grid_origin.z + grid_dimension.z) { return false; }

		return true;
	}

	// Retrun the index value of the first hex sub grid that exists within the list of hex sub grids for
	//  this class that has a coordinate global_grid_coordinate existing with this class hex grid bounds
	cubic_surface_index_data_type get_cubic_sub_grid_index_with_global_grid_coord(cubic_surface_vec_data_type global_grid_coordinate) {
		if (cubic_sub_grids.size() < 1) { return -1; }

		for (int i = 0; i < cubic_sub_grids.size(); i++) {
			cubic_sub_grid_struct_type cubic_sub_grid = cubic_sub_grids[i];

			if (cubic_sub_grid.grid_coordinate_in_sub_cubic_bounds(global_grid_coordinate)) {
				return i;
				break;
			}
		}

		return -1;
	}


	void define_sub_grid_plot_data(cubic_sub_grid_struct_type& cubic_sub_grid) {
		cubic_sub_grid.sub_cubic_centers_x.clear();
		cubic_sub_grid.sub_cubic_centers_x.shrink_to_fit();
		cubic_sub_grid.sub_cubic_centers_y.clear();
		cubic_sub_grid.sub_cubic_centers_y.shrink_to_fit();
		cubic_sub_grid.sub_cubic_centers_z.clear();
		cubic_sub_grid.sub_cubic_centers_z.shrink_to_fit();

		cubic_surface_index_data_type child_cubic_origin_index = get_cubic_surface_matrix_data_index(cubic_sub_grid.lower_range);

		size_t num_rows   = cubic_sub_grid.upper_range.y - cubic_sub_grid.lower_range.y + 1;
		size_t num_cols   = cubic_sub_grid.upper_range.x - cubic_sub_grid.lower_range.x + 1;
		size_t num_levels = cubic_sub_grid.upper_range.z - cubic_sub_grid.lower_range.z + 1;

		//if (cubic_sub_grid.upper_range.x % 2 == 0) { num_cols += 1; } // even max row
		//size_t index_offset = 0;
		cubic_surface_index_data_type cubic_index = child_cubic_origin_index;

		for (size_t level = 0; level < num_levels; level++) {
			for (size_t row = 0; row < num_rows; row++) {
				for (size_t col = 0; col < num_cols; col++) {
					cubic_sub_grid.sub_cubic_centers_x.push_back(cubic_centers_x[cubic_index]);
					cubic_sub_grid.sub_cubic_centers_y.push_back(cubic_centers_y[cubic_index]);
					cubic_sub_grid.sub_cubic_centers_z.push_back(cubic_centers_z[cubic_index]);
					cubic_index += 1;
				}
			}
		}
	}

protected:
	T initial_cubic_data_value; // Initial value that all hexagonal grid cells are assigned to


private:
	id_key_manager_class<unsigned int> cubic_sub_grid_id_key;


};