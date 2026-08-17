#pragma once

#include <vector>
#include <string>
#include <type_traits>

#include <Framework/AFW/Application/afw_global.h>
#include <FrameWork/AFW/Tools/afw_id_key_manager.h>

#include "cart2D_grid_parameters.h"

#include <ThirdParty/ImGui/implot/implot.h>

// Data structure to store and manage unordered hexagonal automata grid data and display that data 
// as a specific shape, size and color in the main hexagonal automata grid display. 

struct cart2D_grid_cell_data_struct_type {
	int cart2D_grid_cell_data_id = -1;
	bool display_data = false;

	std::vector<cart2D_surface_index_data_type> cart2D_grid_cells_index = {}; // Index of Hex grid cells to display data for
	std::vector<float> cart2D_cell_centers_x = {};							// x coordinates of Hex grid cells to display data for
	std::vector<float> cart2D_cell_centers_y = {};							// y coordinates of Hex grid cells to display data for

	ImVec4 cart2D_grid_cells_display_color = { 1.0f,1.0f,1.0f,1.0f };		// colors of Hex grid cells to display data for
	int    display_shape_id = 1;										// shape id of Hex grid cells to display data for
	float  display_shape_size = 6.0f;									// shape size of Hex grid cells to display data for
	ImPlotMarker_ cart2D_grid_cells_display_shape = ImPlotMarker_::ImPlotMarker_Circle;// ImPlot shape of Hex grid cells to display data for

	// Clear all hexagonal grid data as being empty and ready the dynamic vector arrays to be reused
	void clear_cart2D_grid_cell_data() {
//printf("cart2D_grid_cell_data_struct_type::clear_cart2D_grid_cell_data 000 %i : %i : %i |%i : %i : %i \n", cart2D_grid_cells_index.size(), cart2D_cell_centers_x.size(), cart2D_cell_centers_y.size(), cart2D_grid_cells_index.capacity(), cart2D_cell_centers_x.capacity(), cart2D_cell_centers_y.capacity());
		cart2D_grid_cells_index.clear();
		cart2D_cell_centers_x.clear();
		cart2D_cell_centers_y.clear();

//printf("cart2D_grid_cell_data_struct_type::clear_cart2D_grid_cell_data 111 %i : %i : %i|%i : %i : %i\n", cart2D_grid_cells_index.size(), cart2D_cell_centers_x.size(), cart2D_cell_centers_y.size(), cart2D_grid_cells_index.capacity(), cart2D_cell_centers_x.capacity(), cart2D_cell_centers_y.capacity());
		cart2D_grid_cells_index.shrink_to_fit();
		cart2D_cell_centers_x.shrink_to_fit();
		cart2D_cell_centers_y.shrink_to_fit();

//printf("cart2D_grid_cell_data_struct_type::clear_cart2D_grid_cell_data 222 %i : %i : %i|%i : %i : %i\n", cart2D_grid_cells_index.size(), cart2D_cell_centers_x.size(), cart2D_cell_centers_y.size(), cart2D_grid_cells_index.capacity(), cart2D_cell_centers_x.capacity(), cart2D_cell_centers_y.capacity());
	}

	void add_cart2D_grid_cell(cart2D_surface_index_data_type cart2D_grid_cell_index, float x_coord, float y_coord) {
		add_cart2D_grid_cell(cart2D_grid_cell_index);
		add_cart2D_grid_cell_cartesian_coordinates(x_coord, y_coord);
	}

	void add_cart2D_grid_cell(cart2D_surface_index_data_type cart2D_grid_cell_index) {
		cart2D_grid_cells_index.push_back(cart2D_grid_cell_index);
	}

	void add_cart2D_grid_cell_cartesian_coordinates(float x_coord, float y_coord) {
		cart2D_cell_centers_x.push_back(x_coord);
		cart2D_cell_centers_y.push_back(y_coord);
	}

	bool display_cart2D_cell_data() {
		// This display_cart2D_cell_data() function must be called within a ImGui code block similar to
		//      
		// ImGui::Begin("Int Hex Grid");// If Have name of ImGui::Begin then have ability to minimise and dock 
		//      if (ImPlot::BeginPlot("Test Hex Grid")) {
		//
		//			for (cart2D_grid_cell_data_struct_type cart2D_grid_cell : cart2D_grid_cells) {
		//				cart2D_grid_cell.display_cart2D_cell_data();
		//			}
		// 
		//            ImPlot::EndPlot();
		//	}
		// ImGui::End();
//printf("cart2D_grid_cell_data_struct_type :: display_cart2D_cell_data 00 %i : %i : %i \n", cart2D_grid_cell_data_id, cart2D_grid_cells_index.size(), cart2D_cell_centers_x.size());

		if (!display_data) {
			//printf("cart2D_grid_cell_data_struct_type :: display_cart2D_cell_data 00 !display_data \n");
			return false;
		}

		if (cart2D_grid_cell_data_id < 0) {
			//printf("cart2D_grid_cell_data_struct_type :: display_cart2D_cell_data 11 cart2D_grid_cell_data_id < 0 \n");
			return false;
		}
		if (cart2D_cell_centers_x.size() < 1 || cart2D_cell_centers_y.size() < 1) {
			//printf("cart2D_grid_cell_data_struct_type :: display_cart2D_cell_data 22 cart2D_cell_centers_x.size() < 1 || cart2D_cell_centers_y.size() < 1 \n");		
			return false;
		}

//for (int i = 0; i < cart2D_cell_centers_x.size(); i++) {
//	printf("cart2D_grid_cell_data_struct_type :: display_cart2D_cell_data 00 %i : %f : %f \n", cart2D_grid_cell_data_id, cart2D_cell_centers_x[i], cart2D_cell_centers_y[i]);
//}

		std::string scatter_plot_id = "Result Grid" + std::to_string(cart2D_grid_cell_data_id);

		ImPlot::PlotScatter(scatter_plot_id.c_str(), cart2D_cell_centers_x.data(), cart2D_cell_centers_y.data(), cart2D_cell_centers_x.size(), {
			//ImPlotProp_Marker, ImPlotMarker_Circle, 
			//ImPlotProp_Marker, ImPlotMarker_Square,// use this when testing
			ImPlotProp_Marker, cart2D_grid_cells_display_shape,
			ImPlotProp_MarkerSize, display_shape_size,
			ImPlotProp_LineColor, cart2D_grid_cells_display_color,
			ImPlotProp_FillColor, cart2D_grid_cells_display_color,
			ImPlotProp_FillAlpha, 0.25f,
			ImPlotProp_Flags, ImPlotFlags_Equal // Does not work
			});

		return true;
	}
};

// Data structure to store and manage ordered hexagonal automata sub grid data of the main hexagonal grid and display that data 

struct cart2D_sub_grid_struct_type {
	int cart2D_grid_range_id = -1;

	cart2D_sub_grid_struct_type() {}
	cart2D_sub_grid_struct_type(cart2D_surface_vec_data_type lr, cart2D_surface_vec_data_type ur) : lower_range(lr), upper_range(ur) {}

	cart2D_surface_vec_data_type lower_range = { 0,0 }; // Lower left bounds of sub grid
	cart2D_surface_vec_data_type upper_range = { 0,0 }; // upper right bounds of sub grid

	std::vector<float> sub_cart2D_centers_x = {}; // x coordinates of Hex grid cells to display data for
	std::vector<float> sub_cart2D_centers_y = {}; // y coordinates of Hex grid cells to display data for

	cart2D_surface_index_data_type child_cart2D_origin_index = -1; // Index of the main hexagonal grid that the origin 0 index that the sub grid references

	// Function to test if sub hexagonal grid has a valid range over a hexagonal grid of passed origin and grid extent 
	bool valid_range(cart2D_surface_vec_data_type origin, cart2D_surface_vec_data_type extent) {
		if (lower_range.x < origin.x || lower_range.x > extent.x) { return false; }
		if (lower_range.y < origin.y || lower_range.y > extent.y) { return false; }

		if (upper_range.x < origin.x || upper_range.x > extent.x) { return false; }
		if (upper_range.y < origin.y || upper_range.y > extent.y) { return false; }

		return true;
	}

	// Function to test if sub hexagonal grid is of a null or zero dimension
	bool null_dimension() {
		if (upper_range.x == lower_range.x || upper_range.y == lower_range.y) { return false; }

		return true;
	}

	// Function to test if hexagonal grid corrdinate is within the hexagonal sub grid bounds
	bool grid_coordinate_in_sub_cart2D_bounds(cart2D_surface_vec_data_type grid_coordinate) {
		if (grid_coordinate.x < lower_range.x || grid_coordinate.x > upper_range.x) { return false; }
		if (grid_coordinate.y < lower_range.y || grid_coordinate.y > upper_range.y) { return false; }

		return true;
	}

	bool display_sub_cart2D_grid() {
		// This display_sub_cart2D_grid() function must be called within a ImGui code block similar to
		//      
		// ImGui::Begin("Int Hex Grid");// If Have name of ImGui::Begin then have ability to minimise and dock 
		//      if (ImPlot::BeginPlot("Test Hex Grid")) {
		//
		//			for (cart2D_sub_grid_struct_type cart2D_sub_grid : cart2D_sub_grids) {
		//				cart2D_sub_grid.display_sub_cart2D_grid();
		//			}
		// 
		//            ImPlot::EndPlot();
		//	}
		// ImGui::End();

//printf("cart2D_sub_grid_struct_type :: display_sub_cart2D_grid 00"); printf(" %i :: %i \n", cart2D_grid_range_id, sub_cart2D_centers_x.size());

		if (cart2D_grid_range_id < 0) { return false; }
		if (sub_cart2D_centers_x.size() < 1 || sub_cart2D_centers_y.size() < 1) { return false; }

		std::string scatter_plot_id = "sub Grid" + std::to_string(cart2D_grid_range_id); // Change this for user to define sub grid description

		ImPlot::PlotScatter(scatter_plot_id.c_str(), sub_cart2D_centers_x.data(), sub_cart2D_centers_y.data(), sub_cart2D_centers_x.size(), {
			ImPlotProp_Marker, ImPlotMarker_Square,
			ImPlotProp_MarkerSize, 6,
			ImPlotProp_LineColor, ImPlot::GetColormapColor(cart2D_grid_range_id), // Change this for user to define sub grid color
			ImPlotProp_FillColor, ImPlot::GetColormapColor(cart2D_grid_range_id), // Change this for user to define sub grid color
			ImPlotProp_FillAlpha, 0.25f,
			ImPlotProp_Flags, ImPlotFlags_Equal // Does not work
			});

		return true;
	}
};

// Base class upon which all hexagonal automata grids use to define, display and manage hexagonal grid data, 

template <class T>
class cart2D_grid_base_class {
public:
	cart2D_grid_base_class() {}
	cart2D_grid_base_class(T initial_cart2D_value, cart2D_surface_vec_data_type dimension) {
		define_cart2D_grid(initial_cart2D_value, dimension);
	}
	~cart2D_grid_base_class() { delete_cart2D_grid(); }

	std::vector<cart2D_sub_grid_struct_type> cart2D_sub_grids = {}; // List of hexagonal sub grids that are bound to this cart2D_grid_base_class object
	cart2D_grid_value_data_type_enum		  cart2D_grid_value_data_type = cart2D_grid_value_data_type_enum::nan; // The datatype of the hexagonal grid of this cart2D_grid_base_class object

	int cart2D_grid_id = -1;			// Hex grid id

	// ------------- Hexagonal Grid definition --------------------
	glm::vec2				  global_cart_origin = { 0.0f,0.0f }; // Global Cartesian coordinate of the grid origin 
	cart2D_surface_vec_data_type global_grid_origin = { 0,0 };		// Global Index origin that is of the top parent hexagonal grid.

	cart2D_surface_vec_data_type grid_dimension = { 0,0 };     // Dimensions of the cartesian 2D grid

	// ------------- Hexagonal Grid data Storage --------------------
	std::vector<T> cart2D_grid = {};			// hexgrid cell data

	float cart2D_size = 1.0f;					// cartesian 2D grid cell display size

	std::vector<float> cart2D_centers_x = {};   // Hex grid cell Cartesian x coordinates
	std::vector<float> cart2D_centers_y = {};   // Hex grid cell Cartesian y coordinates

	ImVec4 cart2D_grid_display_color = { 0.5f,0.5f,1.0f,1.0f };
	std::vector<ImU32> cart2D_colors = {};		// Hex grid cell color data
	// --------------------------------------------------------

	bool define_cart2D_grid_coordinates() {
		if (cart2D_grid.size() == 0) { return false; }

		ImU32 icu32 = ImGui::GetColorU32(cart2D_grid_display_color);

		for (int i = 0; i < cart2D_grid.size(); ++i) {
			glm::vec2 cart2D_center = get_cart2D_surface_world_cartesian_coordinate(i);
			cart2D_centers_x.push_back(cart2D_center.x);
			cart2D_centers_y.push_back(cart2D_center.y);

			cart2D_colors.push_back(icu32);
			//printf("define_cart2D_grid_coordinates : %f : %f \n", cart2D_center.x, cart2D_center.y);
		}

		return true;
	}

	// Virtual functions that all derived classes must have defined accordint to the datatype that they are defined as
	virtual void delete_cart2D_grid_data(cart2D_surface_index_data_type i) = 0;
	virtual void invalidate_cart2D_grid_data_value(cart2D_surface_index_data_type index) = 0;
	virtual bool display_cart2D_grid() = 0;

	T get_initial_cart2D_grid_value() { return initial_cart2D_data_value; }

	void define_initial_cart2D_data_value(T value) { initial_cart2D_data_value = value; }

	void reinitialise_cart2D_grid_data_values() {
		delete_cart2D_grid();
		define_cart2D_grid();
	}

	void reinitialise_cart2D_grid_data_values(T value) {
		delete_cart2D_grid();
		define_cart2D_grid(value, grid_dimension);
	}

	bool null_dimension() {// Test to determin if have grid of zero dimension on one axis
		if (grid_dimension.x < 1 || grid_dimension.y < 1) {
			return false;
		}
		else {
			return true;
		}
	}

	// ---------------------------------------------------------------------------------
	// Functions to calculate/retrieve hexagonal grid index from x-y grid index coordinates 
	cart2D_surface_index_data_type get_index_value(cart2D_surface_index_data_type iX, cart2D_surface_index_data_type iY, cart2D_surface_index_data_type iZ) {
		return iY * grid_dimension.x + iX;
	}

	cart2D_surface_index_data_type get_z_layer_total(cart2D_surface_index_data_type xdim, cart2D_surface_index_data_type ydim) {
		return xdim * ydim;
	}

	cart2D_surface_index_data_type get_z_layer_total(cart2D_surface_index_data_type iZ) {
		return get_z_layer_total(grid_dimension.x, grid_dimension.y);
	}

	cart2D_surface_index_data_type calculate_cart2D_surface_matrix_data_size(cart2D_surface_index_data_type xdim, cart2D_surface_index_data_type ydim) {
		return   get_z_layer_total(xdim, ydim);
	}

	cart2D_surface_index_data_type calculate_cart2D_surface_matrix_data_size() {
		return calculate_cart2D_surface_matrix_data_size(grid_dimension.x, grid_dimension.y);
	}

	// END cartesian 2D surface matrix index 
	// ---------------------------------------------------------------------------------

	// ###################
	//Create Hex grid of x-y index coordinate range with specified initial value and cartesian 2D grid size
	bool create_cart2D_grid(T initial_cart2D_data_value, cart2D_surface_vec_data_type min_range, cart2D_surface_vec_data_type max_range, float cart2D_size) {
		float x_iextent = max_range.x - min_range.x;
		float y_iextent = max_range.y - min_range.y;

		grid_dimension = { x_iextent,y_iextent };

		global_grid_origin = min_range;
		global_cart_origin = get_cart2D_surface_cartesian_coordinate(min_range, cart2D_size);

		return define_cart2D_grid(initial_cart2D_data_value, grid_dimension);
	}

	//Create Hex grid of x-y Cartesian coordinate range with specified initial value and cartesian 2D grid size
	bool create_cart2D_grid(T initial_cart2D_data_value, glm::vec2 min_range, glm::vec2 max_range, float cart2D_size) {
		float x_extent = max_range.x - min_range.x;
		float y_extent = max_range.y - min_range.y;

		float x_res_step = cart2D_size * 2.0f;
		float y_res_step = cart2D_size * 2.0f;

		cart2D_surface_index_data_type data_set_x_size, data_set_y_size;

		data_set_x_size = (cart2D_surface_index_data_type)(x_extent / x_res_step);
		data_set_y_size = (cart2D_surface_index_data_type)(y_extent / y_res_step);

		grid_dimension = { data_set_x_size,data_set_y_size };

		global_cart_origin = min_range;
		global_grid_origin = min_range;
		global_cart_origin = get_cart2D_surface_cartesian_coordinate(min_range, cart2D_size);

		return define_cart2D_grid(initial_cart2D_data_value, grid_dimension);
	}

	// create cartesian 2D sub grid of specified x-y index coordinate range
	int create_cart2D_sub_grid(cart2D_surface_vec_data_type min_range, cart2D_surface_vec_data_type max_range) {
		cart2D_sub_grid_struct_type cart2D_sub_grid(min_range, max_range);

		if (!cart2D_sub_grid.valid_range(global_grid_origin, global_grid_origin + grid_dimension)) {
			return -1;
		}

		cart2D_sub_grid.cart2D_grid_range_id = cart2D_sub_grid_id_key.get_available_id_key();

		cart2D_sub_grids.push_back(cart2D_sub_grid);

		return cart2D_sub_grid.cart2D_grid_range_id;
	}

	// create cartesian 2D sub grid of specified x-y index coordinate range and value
	bool create_cart2D_sub_grid(T initial_cart2D_value, cart2D_surface_vec_data_type min_range, cart2D_surface_vec_data_type max_range) {

		if (create_cart2D_sub_grid(min_range, max_range) < 0) {
			return false;
		}

		size_t num_rows = max_range.y - min_range.y + 1;
		size_t num_cols = max_range.x - min_range.x + 1;

		cart2D_surface_index_data_type child_cart2D_origin_index = get_cart2D_surface_matrix_data_index(min_range);

		size_t index_offset = 0;
		for (size_t row = 0; row < num_rows; row++) {
			for (size_t col = 0; col < num_cols; col++) {
				cart2D_surface_index_data_type cart2D_index = child_cart2D_origin_index + index_offset + col;
				cart2D_grid[cart2D_index] = initial_cart2D_value;
			}

			index_offset += grid_dimension.x;
		}

		return true;
	}

	//Create Hex grid of x-y dimension with specified initial value
	bool define_cart2D_grid(T initial_cart2D_data_value, cart2D_surface_vec_data_type dimension) {
		define_initial_cart2D_data_value(initial_cart2D_data_value);
		return define_cart2D_grid(dimension);
	}

	//Create Hex grid of x-y dimension with currently defined initial value
	bool define_cart2D_grid(cart2D_surface_vec_data_type dimension) {
		if (!dimension.x || !dimension.y) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "Define cartesian 2D grid :: Cannot define cartesian 2D grid having a zero dimension\n");
			return false;
		}

		size_t surface_grid_data_size = calculate_cart2D_surface_matrix_data_size(dimension.x, dimension.y);

		if (surface_grid_data_size > MAX_PEMISSIBLE_CART2D_GRID_SIZE) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "Define cartesian 2D grid :: Cannot define cartesian 2D grid. Hex grid has dimensions that are constitute a cartesian 2D grid beyond permisible max size.\n");
			return false;
		}

		cart2D_grid = std::vector<T>(surface_grid_data_size, initial_cart2D_data_value);
		grid_dimension = dimension;

		if (std::is_same<T, bool>::value) { cart2D_grid_value_data_type = cart2D_grid_value_data_type_enum::Boolean; }
		if (std::is_same<T, int>::value) { cart2D_grid_value_data_type = cart2D_grid_value_data_type_enum::Integer; }
		if (std::is_same<T, float>::value) { cart2D_grid_value_data_type = cart2D_grid_value_data_type_enum::Float; }
		if (std::is_same<T, double>::value) { cart2D_grid_value_data_type = cart2D_grid_value_data_type_enum::Double; }

		return true;
	}

	// Clear cartesian 2D grid of all data values and initilaise it ready to be reused
	// Need to rename it to clear_cart2D_grid()??????
	void delete_cart2D_grid() {
		// Including any of the following crashes application in certain circumstances with error ocurring in the
		// for  or delete_cart2D_grid_data(f) statements.
		// Do know why since nothing seems to be wrong with the for statement and the delete_cart2D_grid_data();
		// statement is a dummy C++ stub for possible future implementation. Very strange !!!!!!!
		// Left commented out as has no impact or need. 
		//for (size_t i = 0; i < cart2D_grid.size();i++) {
		//for (cart2D_surface_index_data_type i = cart2D_grid.size()-1; i>=0;i--) {
		//for (cart2D_surface_index_data_type f = 5; f>=0;f--) {
			//delete_cart2D_grid_data(f);
		//}
		printf("cart2D_grid_base_class :: delete_cart2D_grid 000"); printf(" %zu\n", cart2D_grid.size());

		cart2D_centers_x.clear();
		cart2D_centers_y.clear();
		cart2D_colors.clear();

		cart2D_grid.clear();

		cart2D_centers_x.shrink_to_fit();
		cart2D_centers_y.shrink_to_fit();
		cart2D_colors.shrink_to_fit();

		cart2D_grid.shrink_to_fit();
		printf("cart2D_grid_base_class :: delete_cart2D_grid 111"); printf(" %zu\n", cart2D_grid.size());
	}

	//bool resize_cart2D_grid() {
	//	return resize_cart2D_grid(grid_dimension);
	//}

	//bool resize_cart2D_grid(cart2D_surface_vec_data_type dimension) {
	//	if (!dimension.x || !dimension.y) {
	//	// Error message that have zero dimension grid
	//		return false;
	//	}

	//	size_t surface_grid_data_size = calculate_cart2D_surface_matrix_data_size(dimension.x, dimension.y);

	//	if (surface_grid_data_size > MAX_PEMISSIBLE_HEX2D_GRID_SIZE) {
	//		// Error message that have cartesian 2D grid size too large
	//		return false;
	//	}

	//	cart2D_grid.resize(surface_grid_data_size);
	//	grid_dimension = dimension;

	//	return true;
	//}

	// ###########################################################################

	// Retreive the 3 dim x,y,z coordinate that the matrix index corresponds to
	cart2D_surface_vec_data_type get_matrix_coordinate(cart2D_surface_index_data_type matrix_index) {
		cart2D_surface_vec_data_type matrix_coord;

		// Get the array matrix index value of the x=0, y=0 z= matrix_coord.z cartesian 2D surface coordinate
		cart2D_surface_index_data_type zeroth_index = matrix_index;
		cart2D_surface_index_data_type yt = grid_dimension.x; // Even row number of hexs + odd row of hexs of any z level

//printf("get_matrix_coordinate 000 : %i : %i : %i  : %i\n", matrix_index, grid_dimension.x, zeroth_index, yt);

		cart2D_surface_index_data_type yt_index01, yt_index02;

		yt_index01 = cart2D_surface_index_data_type(zeroth_index / yt);

		matrix_coord.y = yt_index01;
		matrix_coord.x = zeroth_index - yt_index01 * grid_dimension.x;

		return matrix_coord;
	}

	// Retrieve the Cartesian coord of the cartesian 2D grid x-y index coordinate cart2D_surface_coord and cartesian 2D grid cell size cart2D_size
	glm::vec2 get_cart2D_surface_cartesian_coordinate(cart2D_surface_vec_data_type cart2D_surface_coord, float cart2D_size) {
		cart2D_surface_index_data_type i = cart2D_surface_coord.x, j = cart2D_surface_coord.y, k = 0;

		glm::vec2 cart2D_cartesian_coordinate;

		cart2D_cartesian_coordinate.x = (float(i) * 2) * cart2D_size + cart2D_size;
		cart2D_cartesian_coordinate.y = (2 * float(j)) * cart2D_size + cart2D_size;

		return cart2D_cartesian_coordinate;
	}

	// Retrieve the Cartesian coord of the cartesian 2D grid x-y index coordinate cart2D_surface_coord
	glm::vec2 get_cart2D_surface_cartesian_coordinate(cart2D_surface_vec_data_type cart2D_surface_coord) {
		return get_cart2D_surface_cartesian_coordinate(cart2D_surface_coord, cart2D_size);
	}

	// Retrieve the global Cartesian coord of the cartesian 2D grid x-y index coordinate cart2D_surface_coord
	glm::vec2 get_cart2D_surface_world_cartesian_coordinate(cart2D_surface_vec_data_type voxel_matrix_coord) {
		glm::vec2  voxel_cart_coord = get_cart2D_surface_cartesian_coordinate(voxel_matrix_coord) + global_cart_origin;

		return voxel_cart_coord;
	}

	// Retrieve the Cartesian coord of the cartesian 2D grid cell of index
	glm::vec2 get_cart2D_surface_world_cartesian_coordinate(cart2D_surface_index_data_type index) {
		cart2D_surface_vec_data_type voxel_matrix_coord = get_matrix_coordinate(index);
//printf("get_cart2D_surface_world_cartesian_coordinate index 000 : %i : %i : %i \n",index, voxel_matrix_coord.x, voxel_matrix_coord.y);

		glm::vec2  voxel_cart_coord = get_cart2D_surface_cartesian_coordinate(voxel_matrix_coord) + global_cart_origin;
//printf("get_cart2D_surface_world_cartesian_coordinate index 111 : %f : %f : %f : %f\n", voxel_cart_coord.x, voxel_cart_coord.y, global_cart_origin.x, global_cart_origin.y);

		return voxel_cart_coord;
	}

	// Retrieve the cartesian 2D grid index coordinate that corresponds to the cartesian 2D x-y index coordinate matrix_coord
	cart2D_surface_index_data_type get_cart2D_surface_matrix_data_index(cart2D_surface_vec_data_type matrix_coord) {// y in matrix_coord must be the corrected_y as defined in get_cart2D_surface_matrix_bit_location
		return get_index_value(matrix_coord.x, matrix_coord.y, 0);
	}
	// ###########################################################################
	// ++++++++++++++++++++ FUNCTIONS TO FIND HEX STORAGE INDEX LOCATIONS OF   +++++++++++++++++++++
	// ++++++++++++++++++++    A CARTESIAN COORDINATE POINT P IN 2D SPACE      +++++++++++++++++++++

	/*
		Following code is to find which cartesian 2D cell of a defined cartesian 2D size, origin and dimensions
		that a point P of Cartesian coordinate (x,y) is to occupy.

		!!!!!!!!!!!!!! NOT YET FULLY TESTED !!!!!!!!!!!!!!!

		Usage

		//Get the index of the one dimensiona vector array that the point P of coordinate (x,y)
		//is designated to be assigned to
		cart2D_surface_index_data_type index = index_of_cart2D_cell_with_cartesian_coord(x,y);

		if index > -1 then perform whatever task is required to assign a value to the vector array element
		cart2D_surface_matrix_data[index];
	*/

	// Find the index of the one dimensional vertex vector array that a point P of cartesian coordinte
	// (x,y) will be within the bounds of a 2D hexagon voxel cell.
	cart2D_surface_index_data_type index_of_cart2D_cell_with_cartesian_coord(float x, float y) {
		cart2D_surface_vec_data_type cart2D_coord = hexagon_cell_coord_from_cartesian(x, y);

		if (cartesian_coord_within_grid_bounds(cart2D_coord))
		{
			return get_index_value(cart2D_coord.x, cart2D_coord.y, 0);
		}
		else
		{
			return -1;
		}
	}

	// Determine if a point P of cartesian coordinte (x,y) is within the limits of
	// the dimensions of the hexagonal grid that is stored in the computer memory
	bool cartesian_coord_within_grid_bounds(float x, float y) {
		glm::ivec3 cart2D_coord = hexagon_cell_coord_from_cartesian(x, y);

		return cartesian_coord_within_grid_bounds(cart2D_coord);
	}

	// Determine if a point P of cartesian 2D grid x-y index coordinates is within the limits of
	// the dimensions of the hexagonal grid that is stored in the computer memory
	bool cartesian_coord_within_grid_bounds(cart2D_surface_vec_data_type cart2D_coord) {
		if (cart2D_coord.x < 0 || cart2D_coord.y < 0) { return false; }

		if (cart2D_coord.x > grid_dimension.x) { return false; }
		if (cart2D_coord.y > grid_dimension.y) { return false; }

		return true;
	}

	// Obtain the cartesian 2D grid index coordinates of the cartesian 2D grid that a point P of cartesian coordinte
	// (x,y) will be found to be within the bounds of a 2D hexgagon voxel cell.
	cart2D_surface_vec_data_type hexagon_cell_coord_from_cartesian(float x, float y) {
		float grid_radius = cart2D_size;
		float grid_height = grid_radius;

//printf("hexagon_cell_coord_from_cartesian 000 voxel_size : %f :grid_height %f : grid_radius %f : c %f \n", cart2D_size, grid_height, grid_radius,c);
//printf("hexagon_cell_coord_from_cartesian 111 grid_origin :x %f :y %f \n", grid_origin.x, grid_origin.y);

		float grid_x = x - global_grid_origin.x;
		float grid_y = y - global_grid_origin.y;

//printf("hexagon_cell_coord_from_cartesian 222 :x %f :y %f \n", grid_x, grid_y);

		int row, column;

		row    = (int)floor(grid_y / (cart2D_size * 2.0f));
		column = (int)floor(grid_x / (cart2D_size * 2.0f));

		cart2D_surface_vec_data_type cart2D_coord;
		cart2D_coord.x = column;
		cart2D_coord.y = row;

		return cart2D_coord;
	}

	// ###########################################################################

	// Test  to see if cartesian 2D grid cell of x-y index coordinate grid_coordinate is within a cartesian 2D grid cart2D_grid
	bool grid_coordinate_in_cart2D_grid_bounds(cart2D_grid_base_class* cart2D_grid, cart2D_surface_vec_data_type grid_coordinate) {
		if (!cart2D_grid) return false;
		if (cart2D_grid->grid_dimension.x < 1 || cart2D_grid->grid_dimension.y < 1) { return false; }

		if (grid_coordinate.y < cart2D_grid->global_grid_origin.y || grid_coordinate.y > cart2D_grid->global_grid_origin.y + cart2D_grid->grid_dimension.y) { return false; }
		if (grid_coordinate.x < cart2D_grid->global_grid_origin.x || grid_coordinate.x > cart2D_grid->global_grid_origin.x + cart2D_grid->grid_dimension.x) { return false; }

		return true;
	}

	// Test  to see if cartesian 2D grid cell of x-y index coordinate grid_coordinate is within this class cartesian 2D grid
	bool grid_coordinate_in_cart2D_grid_bounds(cart2D_surface_vec_data_type grid_coordinate) {
		if (grid_dimension.x < 1 || grid_dimension.y < 1) { return false; }

		if (grid_coordinate.y < global_grid_origin.y || grid_coordinate.y > global_grid_origin.y + grid_dimension.y) { return false; }
		if (grid_coordinate.x < global_grid_origin.x || grid_coordinate.x > global_grid_origin.x + grid_dimension.x) { return false; }

		return true;
	}

	// Return the index value of the first cartesian 2D sub grid that exists within the list of cartesian 2D sub grids for
	// this class that has a coordinate global_grid_coordinate existing with this class cartesian 2D grid bounds
	cart2D_surface_index_data_type get_cart2D_sub_grid_index_with_global_grid_coord(cart2D_surface_vec_data_type global_grid_coordinate) {
		if (cart2D_sub_grids.size() < 1) { return -1; }

		for (int i = 0; i < cart2D_sub_grids.size(); i++) {
			cart2D_sub_grid_struct_type cart2D_sub_grid = cart2D_sub_grids[i];

			if (cart2D_sub_grid.grid_coordinate_in_sub_cart2D_bounds(global_grid_coordinate)) {
				return i;
				break;
			}
		}

		return -1;
	}


	void define_sub_grid_plot_data(cart2D_sub_grid_struct_type& cart2D_sub_grid) {
		cart2D_sub_grid.sub_cart2D_centers_x.clear();
		cart2D_sub_grid.sub_cart2D_centers_x.shrink_to_fit();
		cart2D_sub_grid.sub_cart2D_centers_y.clear();
		cart2D_sub_grid.sub_cart2D_centers_y.shrink_to_fit();

		cart2D_surface_index_data_type child_cart2D_origin_index = get_cart2D_surface_matrix_data_index(cart2D_sub_grid.lower_range);

		size_t num_rows = cart2D_sub_grid.upper_range.y - cart2D_sub_grid.lower_range.y + 1;
		size_t num_cols = cart2D_sub_grid.upper_range.x - cart2D_sub_grid.lower_range.x + 1;

		size_t index_offset = 0;

		for (size_t row = 0; row < num_rows; row++) {
			for (size_t col = 0; col < num_cols; col++) {
				cart2D_surface_index_data_type cart2D_index = child_cart2D_origin_index + index_offset + col;

				cart2D_sub_grid.sub_cart2D_centers_x.push_back(cart2D_centers_x[cart2D_index]);
				cart2D_sub_grid.sub_cart2D_centers_y.push_back(cart2D_centers_y[cart2D_index]);
			}

			index_offset += grid_dimension.x;
		}

	}

protected:
	T initial_cart2D_data_value; // Initial value that all hexagonal grid cells are assigned to


private:
	id_key_manager_class<unsigned int> cart2D_sub_grid_id_key;


};