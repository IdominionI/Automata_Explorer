#pragma once

#include <vector>
#include <string>
#include <type_traits>

#include <Framework/AFW/Application/afw_global.h>

#include <FrameWork/AFW/Tools/afw_id_key_manager.h>

#include "hex_grid_parameters.h"

#include <ThirdParty/ImGui/implot/implot.h>


struct hex_grid_cell_data_struct_type {
	int hex_grid_cell_data_id = -1;
	ImVec4 hex_grid_cells_display_color = { 1.0f,1.0f,1.0f,1.0f };
	int    display_shape_id = 1;
	float  display_shape_size = 6.0f;
	ImPlotMarker_ hex_grid_cells_display_shape = ImPlotMarker_::ImPlotMarker_Circle;

	std::vector<hex_surface_index_data_type> hex_grid_cells_index = {};
	std::vector<float> hex_cell_centers_x = {};
	std::vector<float> hex_cell_centers_y = {};

	bool display_data = false;

	void clear_hex_grid_cell_data() {
//printf("hex_grid_cell_data_struct_type::clear_hex_grid_cell_data 000 %i : %i : %i |%i : %i : %i \n", hex_grid_cells_index.size(), hex_cell_centers_x.size(), hex_cell_centers_y.size(), hex_grid_cells_index.capacity(), hex_cell_centers_x.capacity(), hex_cell_centers_y.capacity());
		hex_grid_cells_index.clear();
		hex_cell_centers_x.clear();
		hex_cell_centers_y.clear();

//printf("hex_grid_cell_data_struct_type::clear_hex_grid_cell_data 111 %i : %i : %i|%i : %i : %i\n", hex_grid_cells_index.size(), hex_cell_centers_x.size(), hex_cell_centers_y.size(), hex_grid_cells_index.capacity(), hex_cell_centers_x.capacity(), hex_cell_centers_y.capacity());
		hex_grid_cells_index.shrink_to_fit();
		hex_cell_centers_x.shrink_to_fit();
		hex_cell_centers_y.shrink_to_fit();

//printf("hex_grid_cell_data_struct_type::clear_hex_grid_cell_data 222 %i : %i : %i|%i : %i : %i\n", hex_grid_cells_index.size(), hex_cell_centers_x.size(), hex_cell_centers_y.size(), hex_grid_cells_index.capacity(), hex_cell_centers_x.capacity(), hex_cell_centers_y.capacity());
	}

	void add_hex_grid_cell(hex_surface_index_data_type hex_grid_cell_index, float x_coord, float y_coord) {
		add_hex_grid_cell(hex_grid_cell_index);
		add_hex_grid_cell_cartesian_coordinates(x_coord,y_coord);
	}

	void add_hex_grid_cell(hex_surface_index_data_type hex_grid_cell_index) {
		hex_grid_cells_index.push_back(hex_grid_cell_index);
	}

	void add_hex_grid_cell_cartesian_coordinates(float x_coord,float y_coord) {
		hex_cell_centers_x.push_back(x_coord);
		hex_cell_centers_y.push_back(y_coord);
	}

	bool display_hex_cell_data() {
		// This display_hex_cell_data() function must be called within a ImGui code block similar to
		//      
		// ImGui::Begin("Int Hex Grid");// If Have name of ImGui::Begin then have ability to minimise and dock 
		//      if (ImPlot::BeginPlot("Test Hex Grid")) {
		//
		//			for (hex_grid_cell_data_struct_type hex_grid_cell : hex_grid_cells) {
		//				hex_grid_cell.display_hex_cell_data();
		//			}
		// 
		//            ImPlot::EndPlot();
		//	}
		// ImGui::End();
//printf("hex_grid_cell_data_struct_type :: display_hex_cell_data 00 %i : %i : %i \n", hex_grid_cell_data_id, hex_grid_cells_index.size(), hex_cell_centers_x.size());

		if (!display_data) {
//printf("hex_grid_cell_data_struct_type :: display_hex_cell_data 00 !display_data \n");
			return false;
		}

		if (hex_grid_cell_data_id < 0) {
//printf("hex_grid_cell_data_struct_type :: display_hex_cell_data 11 hex_grid_cell_data_id < 0 \n");
			return false;
		}
		if (hex_cell_centers_x.size() < 1 || hex_cell_centers_y.size() < 1) {
//printf("hex_grid_cell_data_struct_type :: display_hex_cell_data 22 hex_cell_centers_x.size() < 1 || hex_cell_centers_y.size() < 1 \n");		
			return false;
		}

//for (int i = 0; i < hex_cell_centers_x.size(); i++) {
//	printf("hex_grid_cell_data_struct_type :: display_hex_cell_data 00 %i : %f : %f \n", hex_grid_cell_data_id, hex_cell_centers_x[i], hex_cell_centers_y[i]);
//}

		std::string scatter_plot_id = "Result Grid" + std::to_string(hex_grid_cell_data_id);

		ImPlot::PlotScatter(scatter_plot_id.c_str(), hex_cell_centers_x.data(), hex_cell_centers_y.data(), hex_cell_centers_x.size(), {
			//ImPlotProp_Marker, ImPlotMarker_Circle, 
			//ImPlotProp_Marker, ImPlotMarker_Square,// use this when testing
			ImPlotProp_Marker, hex_grid_cells_display_shape,
			ImPlotProp_MarkerSize, display_shape_size,
			ImPlotProp_LineColor, hex_grid_cells_display_color,
			ImPlotProp_FillColor, hex_grid_cells_display_color,
			ImPlotProp_FillAlpha, 0.25f,
			ImPlotProp_Flags, ImPlotFlags_Equal // Does not work
		});

		return true;
	}
};

struct hex_sub_grid_struct_type {
	int hex_grid_range_id = -1;

	hex_sub_grid_struct_type(){}
	hex_sub_grid_struct_type(hex_surface_vec_data_type lr, hex_surface_vec_data_type ur) : lower_range(lr), upper_range(ur){}

	hex_surface_vec_data_type lower_range = { 0,0 };
	hex_surface_vec_data_type upper_range = { 0,0 };

	bool valid_range(hex_surface_vec_data_type origin, hex_surface_vec_data_type extent) {
		if (lower_range.x < origin.x || lower_range.x > extent.x) { return false; }
		if (lower_range.y < origin.y || lower_range.y > extent.y) { return false; }

		if (upper_range.x < origin.x || upper_range.x > extent.x) { return false; }
		if (upper_range.y < origin.y || upper_range.y > extent.y) { return false; }

		return true;
	}

	bool null_dimension() {
		if (upper_range.x == lower_range.x || upper_range.y == lower_range.y) { return false; }

		return true;
	}

	bool grid_coordinate_in_sub_hex_bounds(hex_surface_vec_data_type grid_coordinate) {
		if (grid_coordinate.x < lower_range.x || grid_coordinate.x > upper_range.x) { return false; }
		if (grid_coordinate.y < lower_range.y || grid_coordinate.y > upper_range.y) { return false; }

		return true;
	}

	// ###################################################################
	std::vector<float> sub_hex_centers_x = {};
	std::vector<float> sub_hex_centers_y = {};

	hex_surface_index_data_type child_hex_origin_index = -1;

	bool display_sub_hex_grid() {
		// This display_sub_hex_grid() function must be called within a ImGui code block similar to
		//      
		// ImGui::Begin("Int Hex Grid");// If Have name of ImGui::Begin then have ability to minimise and dock 
		//      if (ImPlot::BeginPlot("Test Hex Grid")) {
		//
		//			for (hex_sub_grid_struct_type hex_sub_grid : hex_sub_grids) {
		//				hex_sub_grid.display_sub_hex_grid();
		//			}
		// 
		//            ImPlot::EndPlot();
		//	}
		// ImGui::End();

printf("hex_sub_grid_struct_type :: display_sub_hex_grid 00"); printf(" %i :: %i \n", hex_grid_range_id, sub_hex_centers_x.size());

		if (hex_grid_range_id < 0) {return false;}
		if (sub_hex_centers_x.size() < 1 || sub_hex_centers_y.size() < 1) { return false; }

		std::string scatter_plot_id = "sub Grid" + std::to_string(hex_grid_range_id);

		ImPlot::PlotScatter(scatter_plot_id.c_str(), sub_hex_centers_x.data(), sub_hex_centers_y.data(), sub_hex_centers_x.size(), {
			ImPlotProp_Marker, ImPlotMarker_Square,
			ImPlotProp_MarkerSize, 6,
			ImPlotProp_LineColor, ImPlot::GetColormapColor(hex_grid_range_id),
			ImPlotProp_FillColor, ImPlot::GetColormapColor(hex_grid_range_id),
			ImPlotProp_FillAlpha, 0.25f,
			ImPlotProp_Flags, ImPlotFlags_Equal // Does not work
			});

		return true;
	}

};


template <class T>
class hex_grid_base_class {
public:
	hex_grid_base_class(){}
	hex_grid_base_class(T initial_hex_value, hex_surface_vec_data_type dimension) {
		define_hex_grid(initial_hex_value,dimension);
	}
	~hex_grid_base_class() { delete_hex_grid(); }

	std::vector<hex_sub_grid_struct_type> hex_sub_grids = {};

	int hex_grid_id = -1;

	glm::vec2				  global_cart_origin = {0.0f,0.0f}; // Global Cartesian coordinate of the grid origin 
	hex_surface_vec_data_type global_grid_origin = {0,0};		// Global Index origin that is of the top parent hexagonal grid.

	hex_surface_vec_data_type grid_dimension	 = { 0,0 };     // Dimensions of the hex grid

	hex_grid_value_data_type_enum hex_grid_value_data_type = hex_grid_value_data_type_enum::nan;
	std::vector<T> hex_grid	= {};		  //hexgrid data

	float hex_size = 1.0f;

	// THIS PART TO BE DEFINED AS A HEX GRID BUFFER
	// +++++++++++++++++++++++++++++++++++++++++++++++++++
	std::vector<float> hex_centers_x = {};
	std::vector<float> hex_centers_y = {};

	ImVec4 hex_grid_display_color = { 0.5f,0.5f,1.0f,1.0f };
	std::vector<ImU32> hex_colors = {};

	bool define_hex_grid_coordinates() {
		if (hex_grid.size() == 0) { return false; }
		
		ImU32 icu32 = ImGui::GetColorU32(hex_grid_display_color);

		for (int i = 0; i < hex_grid.size(); ++i) {
			glm::vec2 hex_center = get_hex_surface_world_cartesian_coordinate(i);
			hex_centers_x.push_back(hex_center.x);
			hex_centers_y.push_back(hex_center.y);


			hex_colors.push_back(icu32);
//printf("define_hex_grid_coordinates : %f : %f \n", hex_center.x, hex_center.y);
		}

		return true;
	}
	
	// +++++++++++++++++++++++++++++++++++++++++++++++++++

	virtual void delete_hex_grid_data(hex_surface_index_data_type i) = 0;
	virtual void invalidate_hex_grid_data_value(hex_surface_index_data_type index) = 0;
	virtual bool display_hex_grid() = 0;

	T get_initial_hex_grid_value() { return initial_hex_data_value; }

	void define_initial_hex_data_value(T value) { initial_hex_data_value = value; }

	void reinitialise_hex_grid_data_values() {
		delete_hex_grid();
		define_hex_grid();
	}

	void reinitialise_hex_grid_data_values(T value) {
		delete_hex_grid();
		define_hex_grid(value, grid_dimension);
	}

	bool null_dimension() {
		if (grid_dimension.x < 1 || grid_dimension.y < 1){
			return false;
		}else{
			return true;
		}
	}
	
	// ---------------------------------------------------------------------------------
	hex_surface_index_data_type get_index_value(hex_surface_index_data_type iX, hex_surface_index_data_type iY, hex_surface_index_data_type iZ) {
		// Even z level
		return (hex_surface_index_data_type(iY / 2) + iY % 2) * grid_dimension.x + hex_surface_index_data_type(iY / 2) * (grid_dimension.x - 1) + iX;
	}

	hex_surface_index_data_type get_z_layer_total(hex_surface_index_data_type xdim, hex_surface_index_data_type ydim) {
		// Even z level
		return (hex_surface_index_data_type(ydim / 2) + ydim % 2) * xdim + hex_surface_index_data_type(ydim / 2) * (xdim - 1);
	}

	hex_surface_index_data_type get_z_layer_total(hex_surface_index_data_type iZ) {
		return get_z_layer_total(grid_dimension.x, grid_dimension.y);
	}

	hex_surface_index_data_type calculate_hex_surface_matrix_data_size(hex_surface_index_data_type xdim, hex_surface_index_data_type ydim) {
		return   get_z_layer_total(xdim, ydim);
	}

	hex_surface_index_data_type calculate_hex_surface_matrix_data_size() {
		return calculate_hex_surface_matrix_data_size(grid_dimension.x, grid_dimension.y);
	}

	// END hex surface matrix index 
	// ---------------------------------------------------------------------------------

	// ###################

	bool create_hex_grid(T initial_hex_data_value, hex_surface_vec_data_type min_range, hex_surface_vec_data_type max_range, float hex_size) {
		float x_iextent = max_range.x - min_range.x;
		float y_iextent = max_range.y - min_range.y;

		grid_dimension = { x_iextent,y_iextent };

		global_grid_origin = min_range;
		global_cart_origin = get_hex_surface_cartesian_coordinate(min_range, hex_size);

		return define_hex_grid(initial_hex_data_value,grid_dimension);
	}

	bool create_hex_grid(T initial_hex_data_value,glm::vec2 min_range, glm::vec2 max_range, float hex_size) {
		float x_extent = max_range.x - min_range.x;
		float y_extent = max_range.y - min_range.y;

		float x_res_step = hex_size * 2.0f;
		float y_res_step = hex_size * (3.0f / sqrt(3.0f));

		hex_surface_index_data_type data_set_x_size, data_set_y_size;

		if (x_extent / x_res_step - float((hex_surface_index_data_type)(x_extent / x_res_step)) > 0.0f)
			{data_set_x_size = (hex_surface_index_data_type)(x_extent / x_res_step) + 1;}
		else
			{data_set_x_size = (hex_surface_index_data_type)(x_extent / x_res_step);}

		if (x_extent / y_res_step - float((hex_surface_index_data_type)(y_extent / y_res_step)) > 0.0f)
			{data_set_y_size = (hex_surface_index_data_type)(y_extent / y_res_step) + 1;}
		else
			{data_set_y_size = (hex_surface_index_data_type)(y_extent / y_res_step);}

		grid_dimension = { data_set_x_size,data_set_y_size };

		global_cart_origin = min_range;
		global_grid_origin = min_range;
		global_cart_origin = get_hex_surface_cartesian_coordinate(min_range, hex_size);

		return define_hex_grid(initial_hex_data_value,grid_dimension);
	}

	int create_hex_sub_grid(hex_surface_vec_data_type min_range, hex_surface_vec_data_type max_range) {
		hex_sub_grid_struct_type hex_sub_grid(min_range,max_range);

		if (!hex_sub_grid.valid_range(global_grid_origin, global_grid_origin+grid_dimension)) {
			return -1;
		}

		hex_sub_grid.hex_grid_range_id = hex_sub_grid_id_key.get_available_id_key();

		hex_sub_grids.push_back(hex_sub_grid);

		return hex_sub_grid.hex_grid_range_id;
	}

	bool create_hex_sub_grid(T initial_hex_value, hex_surface_vec_data_type min_range, hex_surface_vec_data_type max_range) {

		if (create_hex_sub_grid(min_range, max_range) < 0) {
			return false;
		}

		size_t num_rows = max_range.y - min_range.y + 1;
		size_t num_cols = max_range.x - min_range.x + 1;

		hex_surface_index_data_type child_hex_origin_index = get_hex_surface_matrix_data_index(min_range);

		size_t index_offset = 0;
		for (size_t row = 0; row < num_rows; row++) {
			size_t max_cols = 0;
			if (row % 2 == 0) {// even row
				if (num_cols == 1)
					{max_cols = 1;}
				else
					{max_cols = num_cols - 1;}
			}
			else { // odd row
				if (num_cols <= 2)
					{max_cols = 1;}
				else
					{max_cols = num_cols - 2;}
			}

			for (size_t col = 0; col < max_cols; col++) {
				hex_surface_index_data_type hex_index = child_hex_origin_index + index_offset + col;

				hex_grid[hex_index] = initial_hex_value;
			}

			if (row % 2 == 0) {// even row
				index_offset += grid_dimension.x;
			} else { // odd row
				index_offset += grid_dimension.x - 1;
			}

		}

		return true;
	}


	bool define_hex_grid(T initial_hex_data_value, hex_surface_vec_data_type dimension) {
		define_initial_hex_data_value(initial_hex_data_value);
		return define_hex_grid(dimension);
	}

	bool define_hex_grid(hex_surface_vec_data_type dimension) {
		if (!dimension.x || !dimension.y) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "Define hex grid :: Cannot define hex grid having a zero dimension\n");
			return false;
		}

		size_t surface_grid_data_size = calculate_hex_surface_matrix_data_size(dimension.x, dimension.y);

		if (surface_grid_data_size > MAX_PEMISSIBLE_HEX_GRID_SIZE) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "Define hex grid :: Cannot define hex grid. Hex grid has dimensions that are constitute a hex grid beyond permisible max size.\n");
			return false;
		}

		hex_grid = std::vector<T>(surface_grid_data_size, initial_hex_data_value);
		grid_dimension = dimension;

		if (std::is_same<T, bool>::value)  { hex_grid_value_data_type = hex_grid_value_data_type_enum::Boolean;}
		if (std::is_same<T, int>::value)   { hex_grid_value_data_type = hex_grid_value_data_type_enum::Integer;}
		if (std::is_same<T, float>::value) { hex_grid_value_data_type = hex_grid_value_data_type_enum::Float;  }
		if (std::is_same<T, double>::value){ hex_grid_value_data_type = hex_grid_value_data_type_enum::Double; }

		return true;
	}

	void delete_hex_grid() {
		// Including any of the following crashes application in certain circumstances with error ocurring in the
		// for  or delete_hex_grid_data(f) statements.
		// Do know why since nothing seems to be wrong with the for statement and the delete_hex_grid_data();
		// statement is a dummy C++ stub for possible future implementation. Very strange !!!!!!!
		// Left commented out as has no impact or need. 
		//for (size_t i = 0; i < hex_grid.size();i++) {
		//for (hex_surface_index_data_type i = hex_grid.size()-1; i>=0;i--) {
		//for (hex_surface_index_data_type f = 5; f>=0;f--) {
			//delete_hex_grid_data(f);
		//}
printf("hex_grid_base_class :: delete_hex_grid 000"); printf(" %i\n", hex_grid.size());

		hex_centers_x.clear();
		hex_centers_y.clear();
		hex_colors.clear();

		hex_grid.clear();

		hex_centers_x.shrink_to_fit();
		hex_centers_y.shrink_to_fit();
		hex_colors.shrink_to_fit();

		hex_grid.shrink_to_fit();
printf("hex_grid_base_class :: delete_hex_grid 111"); printf(" %i\n", hex_grid.size());
	}

	//bool resize_hex_grid() {
	//	return resize_hex_grid(grid_dimension);
	//}

	//bool resize_hex_grid(hex_surface_vec_data_type dimension) {
	//	if (!dimension.x || !dimension.y) {
	//	// Error message that have zero dimension grid
	//		return false;
	//	}

	//	size_t surface_grid_data_size = calculate_hex_surface_matrix_data_size(dimension.x, dimension.y);

	//	if (surface_grid_data_size > MAX_PEMISSIBLE_HEX_GRID_SIZE) {
	//		// Error message that have hex grid size too large
	//		return false;
	//	}

	//	hex_grid.resize(surface_grid_data_size);
	//	grid_dimension = dimension;

	//	return true;
	//}

	// ###########################################################################

	// Retreive the 3 dim x,y,z coordinate that the matrix index corresponds to
	hex_surface_vec_data_type get_matrix_coordinate(hex_surface_index_data_type matrix_index) {
		hex_surface_vec_data_type matrix_coord;

		// Get the array matrix index value of the x=0, y=0 z= matrix_coord.z hex surface coordinate
		hex_surface_index_data_type zeroth_index = matrix_index - get_index_value(0, 0, 0);
		hex_surface_index_data_type yt = 2 * grid_dimension.x - 1; // Even row number of hexs + odd row of hexs of any z level

//printf("get_matrix_coordinate 000 : %i : %i : %i  : %i\n", matrix_index, grid_dimension.x, zeroth_index, yt);

		hex_surface_index_data_type yt_index01, yt_index02;

		yt_index01 = hex_surface_index_data_type(zeroth_index / yt);

//printf("get_matrix_coordinate 111 : %i : %i \n", matrix_index, yt_index01);

		if (((zeroth_index - yt_index01 * yt) % yt) < grid_dimension.x){
			yt_index02 = 0;
//printf("get_matrix_coordinate 222 yt_index02 = 0\n");
			}
		else {
			yt_index02 = 1;
//printf("get_matrix_coordinate 333 yt_index02 = 1 \n");
		}

		matrix_coord.y = yt_index01 * 2 + yt_index02;
		matrix_coord.x = zeroth_index - ((hex_surface_index_data_type(matrix_coord.y / 2) + matrix_coord.y % 2) * grid_dimension.x + hex_surface_index_data_type(matrix_coord.y / 2) * (grid_dimension.x - 1));

		return matrix_coord;
	}

	glm::vec2 get_hex_surface_cartesian_coordinate(hex_surface_vec_data_type hex_surface_coord, float hex_size) {
		hex_surface_index_data_type i = hex_surface_coord.x, j = hex_surface_coord.y, k = 0;

		float sqrt3 = sqrt(3.0f), third = 1.0f / 3.0f, z_mult = 2.0f * sqrt(6.0f) / 3.0f, sqrt3_2 = sqrt(1.5f);
		glm::vec2 hex_cartesian_coordinate;

		if (j % 2 == 0) {
			hex_cartesian_coordinate.x = (float(i) * 2 + float(k % 2)) * hex_size;
			hex_cartesian_coordinate.y = (sqrt3 * float(j) + sqrt3 * third * float(k % 2)) * hex_size;
		} else {
			hex_cartesian_coordinate.x = (-1.0f + float(i) * 2.0f + float(k % 2) + 2.0f * float((k + 1) % 2)) * hex_size;
			hex_cartesian_coordinate.y = (sqrt3 + sqrt3 * (float(j) - 1) + sqrt3 * third * float(k % 2)) * hex_size;
		}

		return hex_cartesian_coordinate;
	}

	glm::vec2 get_hex_surface_cartesian_coordinate(hex_surface_vec_data_type hex_surface_coord) {
		return get_hex_surface_cartesian_coordinate(hex_surface_coord, hex_size);
	}

	glm::vec2 get_hex_surface_world_cartesian_coordinate(hex_surface_vec_data_type voxel_matrix_coord) {
		glm::vec2  voxel_cart_coord = get_hex_surface_cartesian_coordinate(voxel_matrix_coord) + global_cart_origin;

		return voxel_cart_coord;
	}

	glm::vec2 get_hex_surface_world_cartesian_coordinate(hex_surface_index_data_type index) {
		hex_surface_vec_data_type voxel_matrix_coord = get_matrix_coordinate(index);
//printf("get_hex_surface_world_cartesian_coordinate index 000 : %i : %i : %i \n",index, voxel_matrix_coord.x, voxel_matrix_coord.y);

		glm::vec2  voxel_cart_coord = get_hex_surface_cartesian_coordinate(voxel_matrix_coord) + global_cart_origin;
//printf("get_hex_surface_world_cartesian_coordinate index 111 : %f : %f : %f : %f\n", voxel_cart_coord.x, voxel_cart_coord.y, global_cart_origin.x, global_cart_origin.y);

		return voxel_cart_coord;
	}

	hex_surface_index_data_type get_hex_surface_matrix_data_index(hex_surface_vec_data_type matrix_coord) {// y in matrix_coord must be the corrected_y as defined in get_hex_surface_matrix_bit_location
		return get_index_value(matrix_coord.x, matrix_coord.y, 0);
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
		hex_surface_index_data_type index = index_of_hex_cell_with_cartesian_coord(x,y);

		if index > -1 then perform whatever task is required to assign a value to the vector array element
		hex_surface_matrix_data[index];
	*/

	// Find the index of the one dimensional vertex vector array that a point P of cartesian coordinte
	// (x,y) will be within the bounds of a 2D hegagon voxel cell.
	hex_surface_index_data_type index_of_hex_cell_with_cartesian_coord(float x, float y) {
		hex_surface_vec_data_type hex_coord = hexagon_cell_coord_from_cartesian(x,y);

		if (cartesian_coord_within_grid_bounds(hex_coord))
			{return get_index_value(hex_coord.x, hex_coord.y, 0);}
		else
			{return - 1;}
	}

	// Determine if a point P of cartesian coordinte (x,y) is within the limits of
	// the dimensions of the hexagonal grid that is stored in the computer memory
	bool cartesian_coord_within_grid_bounds(float x, float y) {
		glm::ivec3 hex_coord = hexagon_cell_coord_from_cartesian(x,y);

		return cartesian_coord_within_grid_bounds(hex_coord);
	}

	///bool cartesian_coord_within_grid_bounds(glm::ivec3 hex_coord) {
	bool cartesian_coord_within_grid_bounds(hex_surface_vec_data_type hex_coord) {
		if (hex_coord.x < 0 || hex_coord.y < 0) { return false; }

		if (hex_coord.y % 2 == 0){// even row
			if (hex_coord.x > grid_dimension.x) { return false; }
		}
		else {// odd row
			if (hex_coord.x > grid_dimension.x - 1) { return false; }
		}

		if (hex_coord.y > grid_dimension.y) { return false; }

		return true;
	}

	// Obtain the hex grid index coordinates of the hex grid that a point P of cartesian coordinte
	// (x,y) will be found to be within the bounds of a 2D hexgagon voxel cell.
	hex_surface_vec_data_type hexagon_cell_coord_from_cartesian(float x, float y) {
		float grid_radius = hex_size ;
		float grid_height = grid_radius * (sqrt(3.0f));
		float c           = grid_radius / (sqrt(3.0f));
		
//printf("hexagon_cell_coord_from_cartesian 000 voxel_size : %f :grid_height %f : grid_radius %f : c %f \n", hex_size, grid_height, grid_radius,c);
//printf("hexagon_cell_coord_from_cartesian 111 grid_origin :x %f :y %f \n", grid_origin.x, grid_origin.y);

		float grid_x = x - global_grid_origin.x;
		float grid_y = y - global_grid_origin.y;

//printf("hexagon_cell_coord_from_cartesian 222 :x %f :y %f \n", grid_x, grid_y);

		int row, column;

		if (grid_y < -1.0 / sqrt(3.0))
			{row = (int)((grid_y - grid_height) / grid_height);}
		else
			{row = (int)(grid_y / grid_height);}

		bool row_is_odd = abs(row % 2) == 1;

		if (row_is_odd)
			{column = (int)floor(grid_x / (hex_size*2.0f));}
		else
			{column = (int)floor((grid_x + grid_radius) / (hex_size * 2.0f));}

//printf("hexagon_cell_coord_from_cartesian 333 :row %i :col %i \n",row, column);
		// Position of point relative to box it is in
		float rel_y = grid_y - (row * grid_height);
		float rel_x;

		if (row_is_odd)
			{rel_x = (grid_x - ((column * (hex_size * 2.0f)) + grid_radius));}
		else
			{rel_x = grid_x - (column * (hex_size * 2.0f));}

		float m = 1.0f / sqrt(3.0f);

//printf("hexagon_cell_coord_from_cartesian 444 :rel_x %f :rel_y %f : m %f : line %f :%f \n", rel_x,rel_y, m, m * rel_x + 2.0*c, -m * rel_x + 2.0 * c);
		// Work out if the point is above either of the hexagon's top edges
		if (rel_y >= (m * rel_x + 2.0 * c) && rel_x < 0){ // LEFT edge
//printf("hexagon_cell_coord_from_cartesian 555 :row %i :col %i \n",row, column);
			row++;
			if (!row_is_odd)
				{column--;}
		} else {
			if (rel_y >= (-m * rel_x) + 2.0 * c && rel_x >= 0) { // RIGHT edge
//printf("hexagon_cell_coord_from_cartesian 666 :row %i :col %i \n", row, column);
				row++;
				if (row_is_odd)
					{column++;}
			}
		}

		hex_surface_vec_data_type hex_coord;
		hex_coord.x = column;
		hex_coord.y = row;

		return hex_coord;
	}

	// ###########################################################################

	bool grid_coordinate_in_hex_grid_bounds(hex_grid_base_class *hex_grid, hex_surface_vec_data_type grid_coordinate) {
		if (!hex_grid) return false;
		if (hex_grid->grid_dimension.x < 1 || hex_grid->grid_dimension.y < 1) { return false; }

		if (grid_coordinate.y < hex_grid->global_grid_origin.y || grid_coordinate.y > hex_grid->global_grid_origin.y + hex_grid->grid_dimension.y) { return false; }

		if (grid_coordinate.y % 2 == 0) { // have even row
			if (grid_coordinate.x < hex_grid->global_grid_origin.x || grid_coordinate.x > hex_grid->global_grid_origin.x + hex_grid->grid_dimension.x) { return false; }
		} else{ // have odd row
			if (grid_coordinate.x < hex_grid->global_grid_origin.x || grid_coordinate.x > hex_grid->global_grid_origin.x + hex_grid->grid_dimension.x - 1) { return false; }
		}

		return true;
	}

	bool grid_coordinate_in_hex_grid_bounds(hex_surface_vec_data_type grid_coordinate) {
		if (grid_dimension.x < 1 || grid_dimension.y < 1) { return false; }

		if (grid_coordinate.y < global_grid_origin.y || grid_coordinate.y > global_grid_origin.y + grid_dimension.y) { return false; }

		if (grid_coordinate.y % 2 == 0) { // have even row
			if (grid_coordinate.x < global_grid_origin.x || grid_coordinate.x > global_grid_origin.x + grid_dimension.x) { return false; }
		}
		else { // have odd row
			if (grid_coordinate.x < global_grid_origin.x || grid_coordinate.x > global_grid_origin.x + grid_dimension.x - 1) { return false; }
		}

		return true;
	}

	hex_surface_index_data_type get_hex_sub_grid_index_with_global_grid_coord(hex_surface_vec_data_type global_grid_coordinate) {
		if (hex_sub_grids.size() < 1) { return -1; }

		for (int i = 0; i < hex_sub_grids.size(); i++ ) {
			hex_sub_grid_struct_type hex_sub_grid = hex_sub_grids[i];

			if (hex_sub_grid.grid_coordinate_in_sub_hex_bounds(global_grid_coordinate)) {
				return i;
				break;
			}
		}

		return -1;
	}


	void define_sub_grid_plot_data(hex_sub_grid_struct_type &hex_sub_grid) {
		hex_sub_grid.sub_hex_centers_x.clear();
		hex_sub_grid.sub_hex_centers_x.shrink_to_fit();
		hex_sub_grid.sub_hex_centers_y.clear();
		hex_sub_grid.sub_hex_centers_y.shrink_to_fit();

		hex_surface_index_data_type child_hex_origin_index = get_hex_surface_matrix_data_index(hex_sub_grid.lower_range);

		size_t num_rows = hex_sub_grid.upper_range.y - hex_sub_grid.lower_range.y + 1;
		size_t num_cols = hex_sub_grid.upper_range.x - hex_sub_grid.lower_range.x + 1;

		if (hex_sub_grid.upper_range.x % 2 == 0) { num_cols += 1; } // even max row
		size_t index_offset = 0;

		for (size_t row = 0; row < num_rows; row++) {
			size_t max_cols = 0;
			if (row % 2 == 0) {// even row
				if (num_cols == 1)
					{max_cols = 1;}
				else
					{max_cols = num_cols - 1;}
			}
			else { // odd row
				if (num_cols <= 2)
					{max_cols = 1;}
				else
					{max_cols = num_cols - 2;}
			}

			for (size_t col = 0; col < max_cols; col++) {
				hex_surface_index_data_type hex_index = child_hex_origin_index + index_offset + col;

				hex_sub_grid.sub_hex_centers_x.push_back(hex_centers_x[hex_index]);
				hex_sub_grid.sub_hex_centers_y.push_back(hex_centers_y[hex_index]);
			}

			if (row % 2 == 0) {// even row
				index_offset += grid_dimension.x;
			}
			else { // odd row
				index_offset += grid_dimension.x - 1;
			}

		}

	}

protected:
	T initial_hex_data_value;


private:
	id_key_manager_class<unsigned int> hex_sub_grid_id_key;


};