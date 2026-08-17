#pragma once

#include <vector>
#include <string>
#include <type_traits>

#include <Framework/AFW/Application/afw_global.h>
#include <FrameWork/AFW/Tools/afw_id_key_manager.h>

#include "hcp_grid_parameters.h"

#include <ThirdParty/ImGui/implot3D/implot3d.h>
#include <ThirdParty/ImGui/implot3D/implot3d_internal.h>

// Data structure to store and manage unordered hexagonal automata grid data and display that data 
// as a specific shape, size and color in the main hexagonal automata grid display. 

struct hcp_grid_cell_data_struct_type {
	int hcp_grid_cell_data_id = -1;
	bool display_data = false;

	std::vector<hcp_surface_index_data_type> hcp_grid_cells_index = {}; // Index of Hex grid cells to display data for
	std::vector<float> hcp_cell_centers_x = {};							// x coordinates of Hex grid cells to display data for
	std::vector<float> hcp_cell_centers_y = {};							// y coordinates of Hex grid cells to display data for
	std::vector<float> hcp_cell_centers_z = {};							// y coordinates of Hex grid cells to display data for

	ImVec4 hcp_grid_cells_display_color = { 1.0f,1.0f,1.0f,1.0f };		// colors of Hex grid cells to display data for
	int    display_shape_id = 1;										// shape id of Hex grid cells to display data for
	float  display_shape_size = 6.0f;									// shape size of Hex grid cells to display data for
	ImPlot3DMarker_ hcp_grid_cells_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Circle;// ImPlot shape of Hex grid cells to display data for

	// Clear all hexagonal grid data as being empty and ready the dynamic vector arrays to be reused
	void clear_hcp_grid_cell_data() {
//printf("hcp_grid_cell_data_struct_type::clear_hcp_grid_cell_data 000 %i : %i : %i |%i : %i : %i \n", hcp_grid_cells_index.size(), hcp_cell_centers_x.size(), hcp_cell_centers_y.size(), hcp_grid_cells_index.capacity(), hcp_cell_centers_x.capacity(), hcp_cell_centers_y.capacity());
		hcp_grid_cells_index.clear();
		hcp_cell_centers_x.clear();
		hcp_cell_centers_y.clear();
		hcp_cell_centers_z.clear();

//printf("hcp_grid_cell_data_struct_type::clear_hcp_grid_cell_data 111 %i : %i : %i|%i : %i : %i\n", hcp_grid_cells_index.size(), hcp_cell_centers_x.size(), hcp_cell_centers_y.size(), hcp_grid_cells_index.capacity(), hcp_cell_centers_x.capacity(), hcp_cell_centers_y.capacity());
		hcp_grid_cells_index.shrink_to_fit();
		hcp_cell_centers_x.shrink_to_fit();
		hcp_cell_centers_y.shrink_to_fit();
		hcp_cell_centers_z.shrink_to_fit();

//printf("hcp_grid_cell_data_struct_type::clear_hcp_grid_cell_data 222 %i : %i : %i|%i : %i : %i\n", hcp_grid_cells_index.size(), hcp_cell_centers_x.size(), hcp_cell_centers_y.size(), hcp_grid_cells_index.capacity(), hcp_cell_centers_x.capacity(), hcp_cell_centers_y.capacity());
	}

	void add_hcp_grid_cell(hcp_surface_index_data_type hcp_grid_cell_index, float x_coord, float y_coord, float z_coord) {
		add_hcp_grid_cell(hcp_grid_cell_index);
		add_hcp_grid_cell_cartesian_coordinates(x_coord, y_coord, z_coord);
	}

	void add_hcp_grid_cell(hcp_surface_index_data_type hcp_grid_cell_index) {
		hcp_grid_cells_index.push_back(hcp_grid_cell_index);
	}

	void add_hcp_grid_cell_cartesian_coordinates(float x_coord, float y_coord, float z_coord) {
		hcp_cell_centers_x.push_back(x_coord);
		hcp_cell_centers_y.push_back(y_coord);
		hcp_cell_centers_z.push_back(z_coord);
	}

	bool display_hcp_cell_data() {
		// This display_hcp_cell_data() function must be called within a ImGui code block similar to
		//      
		// ImGui::Begin("Int Hex Grid");// If Have name of ImGui::Begin then have ability to minimise and dock 
		//      if (ImPlot::BeginPlot("Test Hex Grid")) {
		//
		//			for (hcp_grid_cell_data_struct_type hcp_grid_cell : hcp_grid_cells) {
		//				hcp_grid_cell.display_hcp_cell_data();
		//			}
		// 
		//            ImPlot::EndPlot();
		//	}
		// ImGui::End();
//printf("hcp_grid_cell_data_struct_type :: display_hcp_cell_data 00 %i : %i : %i \n", hcp_grid_cell_data_id, hcp_grid_cells_index.size(), hcp_cell_centers_x.size());

		if (!display_data) {
//printf("hcp_grid_cell_data_struct_type :: display_hcp_cell_data 00 !display_data \n");
			return false;
		}

		if (hcp_grid_cell_data_id < 0) {
//printf("hcp_grid_cell_data_struct_type :: display_hcp_cell_data 11 hcp_grid_cell_data_id < 0 \n");
			return false;
		}
		if (hcp_cell_centers_x.size() < 1 || hcp_cell_centers_y.size() < 1 || hcp_cell_centers_z.size() < 1) {
//printf("hcp_grid_cell_data_struct_type :: display_hcp_cell_data 22 hcp_cell_centers_x.size() < 1 || hcp_cell_centers_y.size() < 1 \n");		
			return false;
		}

//for (int i = 0; i < hcp_cell_centers_x.size(); i++) {
//	printf("hcp_grid_cell_data_struct_type :: display_hcp_cell_data 00 %i : %f : %f \n", hcp_grid_cell_data_id, hcp_cell_centers_x[i], hcp_cell_centers_y[i]);
//}

		std::string scatter_plot_id = "Result Grid" + std::to_string(hcp_grid_cell_data_id);

		ImPlot3DSpec spec;
		//ImPlotProp_Marker, ImPlotMarker_Circle, 
		//ImPlotProp_Marker, ImPlotMarker_Square,// use this when testing
		spec.Marker			 = hcp_grid_cells_display_shape;
		spec.MarkerSize		 = display_shape_size;
		spec.MarkerLineColor = hcp_grid_cells_display_color;
		spec.MarkerFillColor = hcp_grid_cells_display_color;
		spec.FillAlpha = 0.25f;
		ImPlot3D::PlotScatter(scatter_plot_id.c_str(), hcp_cell_centers_x.data(), hcp_cell_centers_y.data(), hcp_cell_centers_z.data(), hcp_cell_centers_x.size(),spec);

		//void DemoScatterPlots() {
		//	IMGUI_DEMO_MARKER("Plots/Scatter Plots");
		//	srand(0);
		//	static float xs1[100], ys1[100], zs1[100];
		//	for (int i = 0; i < 100; i++) {
		//		xs1[i] = i * 0.01f;
		//		ys1[i] = xs1[i] + 0.1f * ((float)rand() / (float)RAND_MAX);
		//		zs1[i] = xs1[i] + 0.1f * ((float)rand() / (float)RAND_MAX);
		//	}
		//	static float xs2[50], ys2[50], zs2[50];
		//	for (int i = 0; i < 50; i++) {
		//		xs2[i] = 0.25f + 0.2f * ((float)rand() / (float)RAND_MAX);
		//		ys2[i] = 0.50f + 0.2f * ((float)rand() / (float)RAND_MAX);
		//		zs2[i] = 0.75f + 0.2f * ((float)rand() / (float)RAND_MAX);
		//	}

		//	if (ImPlot3D::BeginPlot("Scatter Plots")) {
		//		ImPlot3D::PlotScatter("Data 1", xs1, ys1, zs1, 100);
		//		ImPlot3DSpec spec;
		//		spec.Marker = ImPlot3DMarker_Square;
		//		spec.MarkerSize = 6;
		//		spec.MarkerLineColor = ImPlot3D::GetColormapColor(1);
		//		spec.MarkerFillColor = ImPlot3D::GetColormapColor(1);
		//		spec.FillAlpha = 0.25f;
		//		ImPlot3D::PlotScatter("Data 2", xs2, ys2, zs2, 50, spec);
		//		ImPlot3D::EndPlot();
		//	}
		//}

		return true;
	}
};

// Data structure to store and manage ordered hexagonal automata sub grid data of the main hexagonal grid and display that data 

struct hcp_sub_grid_struct_type {
	int hcp_grid_range_id = -1;

	hcp_sub_grid_struct_type() {}
	hcp_sub_grid_struct_type(hcp_surface_vec_data_type lr, hcp_surface_vec_data_type ur) : lower_range(lr), upper_range(ur) {}

	hcp_surface_vec_data_type lower_range = { 0,0,0 }; // Lower left bounds of sub grid
	hcp_surface_vec_data_type upper_range = { 0,0,0 }; // upper right bounds of sub grid

	std::vector<float> sub_hcp_centers_x = {}; // x coordinates of Hex grid cells to display data for
	std::vector<float> sub_hcp_centers_y = {}; // y coordinates of Hex grid cells to display data for
	std::vector<float> sub_hcp_centers_z = {}; // y coordinates of Hex grid cells to display data for

	hcp_surface_index_data_type child_hcp_origin_index = -1; // Index of the main hexagonal grid that the origin 0 index that the sub grid references

	// Function to test if sub hexagonal grid has a valid range over a hexagonal grid of passed origin and grid extent 
	bool valid_range(hcp_surface_vec_data_type origin, hcp_surface_vec_data_type extent) {
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
	bool grid_coordinate_in_sub_hcp_bounds(hcp_surface_vec_data_type grid_coordinate) {
		if (grid_coordinate.x < lower_range.x || grid_coordinate.x > upper_range.x) { return false; }
		if (grid_coordinate.y < lower_range.y || grid_coordinate.y > upper_range.y) { return false; }
		if (grid_coordinate.z < lower_range.z || grid_coordinate.z > upper_range.z) { return false; }

		return true;
	}

	bool display_sub_hcp_grid() {
		// This display_sub_hcp_grid() function must be called within a ImGui code block similar to
		//      
		// ImGui::Begin("Int Hex Grid");// If Have name of ImGui::Begin then have ability to minimise and dock 
		//      if (ImPlot::BeginPlot("Test Hex Grid")) {
		//
		//			for (hcp_sub_grid_struct_type hcp_sub_grid : hcp_sub_grids) {
		//				hcp_sub_grid.display_sub_hcp_grid();
		//			}
		// 
		//            ImPlot::EndPlot();
		//	}
		// ImGui::End();

//printf("hcp_sub_grid_struct_type :: display_sub_hcp_grid 00"); printf(" %i :: %i \n", hcp_grid_range_id, sub_hcp_centers_x.size());

		if (hcp_grid_range_id < 0) { return false; }
		if (sub_hcp_centers_x.size() < 1 || sub_hcp_centers_y.size() < 1 || sub_hcp_centers_z.size() < 1) { return false; }

		std::string scatter_plot_id = "sub Grid" + std::to_string(hcp_grid_range_id); // Change this for user to define sub grid description

		ImPlot3DSpec spec;
		//ImPlotProp_Marker, ImPlotMarker_Circle, 
		//ImPlotProp_Marker, ImPlotMarker_Square,// use this when testing
		spec.Marker			 = ImPlot3DMarker_Square;
		spec.MarkerSize		 = 6;
		spec.MarkerLineColor = ImPlot3D::GetColormapColor(hcp_grid_range_id);
		spec.MarkerFillColor = ImPlot3D::GetColormapColor(hcp_grid_range_id);
		spec.FillAlpha = 0.25f;
		ImPlot3D::PlotScatter(scatter_plot_id.c_str(), sub_hcp_centers_x.data(), sub_hcp_centers_y.data(), sub_hcp_centers_z.data(), sub_hcp_centers_x.size(),spec);

		//ImPlot::PlotScatter(scatter_plot_id.c_str(), sub_hcp_centers_x.data(), sub_hcp_centers_y.data(), sub_hcp_centers_x.size(), {
		//	ImPlotProp_Marker, ImPlotMarker_Square,
		//	ImPlotProp_MarkerSize, 6,
		//	ImPlotProp_LineColor, ImPlot::GetColormapColor(hcp_grid_range_id), // Change this for user to define sub grid color
		//	ImPlotProp_FillColor, ImPlot::GetColormapColor(hcp_grid_range_id), // Change this for user to define sub grid color
		//	ImPlotProp_FillAlpha, 0.25f,
		//	ImPlotProp_Flags, ImPlotFlags_Equal // Does not work
		//	});

		return true;
	}
};

// Base class upon which all hexagonal automata grids use to define, display and manage hexagonal grid data, 

template <class T>
class hcp_grid_base_class {
public:
	hcp_grid_base_class() {}
	hcp_grid_base_class(T initial_hcp_value, hcp_surface_vec_data_type dimension) {
		define_hcp_grid(initial_hcp_value, dimension);
	}
	~hcp_grid_base_class() { delete_hcp_grid(); }

	std::vector<hcp_sub_grid_struct_type> hcp_sub_grids = {}; // List of hexagonal sub grids that are bound to this hcp_grid_base_class object
	hcp_grid_value_data_type_enum		  hcp_grid_value_data_type = hcp_grid_value_data_type_enum::nan; // The datatype of the hexagonal grid of this hcp_grid_base_class object

	int hcp_grid_id = -1;			// Hex grid id

	// ------------- Hexagonal Grid definition --------------------
	glm::vec3				  global_cart_origin = { 0.0f,0.0f,0.0f }; // Global Cartesian coordinate of the grid origin 
	hcp_surface_vec_data_type global_grid_origin = { 0,0,0 };		// Global Index origin that is of the top parent hexagonal grid.

	hcp_surface_vec_data_type grid_dimension = { 0,0,0 };     // Dimensions of the hex grid

	// ------------- Hexagonal Grid data Storage --------------------
	std::vector<T> hcp_grid = {};			// hexgrid cell data

	float hcp_size = 1.0f;					// hex grid cell display size

	std::vector<float> hcp_centers_x = {};   // Hex grid cell Cartesian x coordinates
	std::vector<float> hcp_centers_y = {};   // Hex grid cell Cartesian y coordinates
	std::vector<float> hcp_centers_z = {};   // Hex grid cell Cartesian y coordinates

	ImVec4 hcp_grid_display_color = { 0.5f,0.5f,1.0f,1.0f };
	std::vector<ImU32> hcp_colors = {};		// Hex grid cell color data
	// --------------------------------------------------------

	bool define_hcp_grid_coordinates() {
		if (hcp_grid.size() == 0) { return false; }

		ImU32 icu32 = ImGui::GetColorU32(hcp_grid_display_color);

printf("hcp_grid_base_class :: define_hcp_grid_coordinates : %i\n", hcp_grid.size());
		for (int i = 0; i < hcp_grid.size(); i++) {
			glm::vec3 hcp_center = get_hcp_surface_world_cartesian_coordinate(i);
			hcp_centers_x.push_back(hcp_center.x);
			hcp_centers_y.push_back(hcp_center.y);
			hcp_centers_z.push_back(hcp_center.z);

			hcp_colors.push_back(icu32);
//printf("hcp_grid_base_class :: define_hcp_grid_coordinates %i : %f: %f : %f \n",i , hcp_center.x, hcp_center.y, hcp_center.z);
		}

		return true;
	}

	// Virtual functions that all derived classes must have defined accordint to the datatype that they are defined as
	virtual void delete_hcp_grid_data(hcp_surface_index_data_type i) = 0;
	virtual void invalidate_hcp_grid_data_value(hcp_surface_index_data_type index) = 0;
	virtual bool display_hcp_grid() = 0;

	T get_initial_hcp_grid_value() { return initial_hcp_data_value; }

	void define_initial_hcp_data_value(T value) { initial_hcp_data_value = value; }

	void reinitialise_hcp_grid_data_values() {
		delete_hcp_grid();
		define_hcp_grid();
	}

	void reinitialise_hcp_grid_data_values(T value) {
		delete_hcp_grid();
		define_hcp_grid(value, grid_dimension);
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
	// See documentation vw_HCP_voxel_draft_01 for more details and explaination how these alogorithims were derived

	// Calculate the index of the  C++ vector modelling the voxel matrix value of layer iZ of a voxel that has coordinate (iX,iY,iZ)
	hcp_surface_index_data_type get_z_layer_index_value(hcp_surface_index_data_type iX, hcp_surface_index_data_type iY, hcp_surface_index_data_type iZ) {
		if (iZ % 2 == 0) // Even z level
			{return (hcp_surface_index_data_type(iY / 2) + iY % 2) * grid_dimension.x + hcp_surface_index_data_type(iY / 2) * (grid_dimension.x - 1) + iX;}
		else // Odd z level
			{return (hcp_surface_index_data_type(iY / 2) + iY % 2) * (grid_dimension.x - 1) + hcp_surface_index_data_type(iY / 2) * grid_dimension.x + iX;}
	}

	// Functions to calculate/retrieve hexagonal grid index from x-y grid index coordinates 
	hcp_surface_index_data_type get_index_value(hcp_surface_index_data_type iX, hcp_surface_index_data_type iY, hcp_surface_index_data_type iZ) {
		return (hcp_surface_index_data_type(iZ / 2) + iZ % 2) * get_z_layer_total(0) + hcp_surface_index_data_type(iZ / 2) * get_z_layer_total(1) + get_z_layer_index_value(iX, iY, iZ);
	}

	hcp_surface_index_data_type get_z_layer_total(hcp_surface_index_data_type iZ, hcp_surface_index_data_type xdim, hcp_surface_index_data_type ydim) {
		if (iZ % 2 == 0) { // Even z level
			return (hcp_surface_index_data_type(ydim / 2) + ydim % 2) * xdim + hcp_surface_index_data_type(ydim / 2) * (xdim - 1);
		} else {// Odd z level
			return (hcp_surface_index_data_type((ydim - 1) / 2) + (ydim - 1) % 2) * (xdim - 1) + hcp_surface_index_data_type((ydim - 1) / 2) * xdim;
		}
	}

	hcp_surface_index_data_type get_z_layer_total(hcp_surface_index_data_type iZ) {
		return get_z_layer_total(iZ,grid_dimension.x, grid_dimension.y);
	}

	// calculate the total number of voxels cells that exist within a voxel matrix
	hcp_surface_index_data_type calculate_hcp_surface_matrix_data_size(hcp_surface_index_data_type xdim, hcp_surface_index_data_type ydim, hcp_surface_index_data_type zdim) {
		return  (hcp_surface_index_data_type(zdim / 2) + zdim % 2) * get_z_layer_total(0, xdim, ydim) + hcp_surface_index_data_type(zdim / 2) * get_z_layer_total(1, xdim, ydim);
	}

	//hcp_surface_index_data_type calculate_hcp_surface_matrix_data_size() {
	//	return calculate_hcp_surface_matrix_data_size(grid_dimension.x, grid_dimension.y, grid_dimension.z);
	//}

	// END hex surface matrix index 
	// ---------------------------------------------------------------------------------

	// ###################
	//Create Hex grid of x-y index coordinate range with specified initial value and hex grid size
	bool create_hcp_grid(T initial_hcp_data_value, hcp_surface_vec_data_type min_range, hcp_surface_vec_data_type max_range, float hcp_size) {
		float x_iextent = max_range.x - min_range.x;
		float y_iextent = max_range.y - min_range.y;
		float z_iextent = max_range.z - min_range.z;

		grid_dimension = { x_iextent,y_iextent,z_iextent };

		global_grid_origin = min_range;
		global_cart_origin = get_hcp_surface_cartesian_coordinate(min_range, hcp_size);

		return define_hcp_grid(initial_hcp_data_value, grid_dimension);
	}

	// For a given set of limits of a Cartesian rectangular volume, calculate the dimensions of a voxel
// matrix that fits within the limits of this rectangular volume.

	glm::ivec3 calc_voxel_volume_dimensions(T initial_hcp_data_value, glm::vec3 min_range, glm::vec3 max_range, float hcp_size) {
		float x_extent = max_range.x - min_range.x;
		float y_extent = max_range.y - min_range.y;
		float z_extent = max_range.z - min_range.z;
		float z_mult = 2.0f * sqrt(6.0f) / 3.0f;

		float x_res_step = hcp_size * 2.0f;
		float y_res_step = hcp_size * (3.0f / sqrt(3.0f));
		//float z_res_step = voxel_generator_parameters.resolution_step * z_mult;
		float z_res_step = z_mult;

		hcp_surface_index_data_type data_set_x_size, data_set_y_size, data_set_z_size;

		if (x_extent / x_res_step - float((hcp_surface_index_data_type)(x_extent / x_res_step)) > 0.0f)
		{
			data_set_x_size = (hcp_surface_index_data_type)(x_extent / x_res_step) + 1;
		}
		else
		{
			data_set_x_size = (hcp_surface_index_data_type)(x_extent / x_res_step);
		}

		if (y_extent / y_res_step - float((hcp_surface_index_data_type)(y_extent / y_res_step)) > 0.0f)
		{
			data_set_y_size = (hcp_surface_index_data_type)(y_extent / y_res_step) + 1;
		}
		else
		{
			data_set_y_size = (hcp_surface_index_data_type)(y_extent / y_res_step);
		}

		if (z_extent / z_res_step - float((hcp_surface_index_data_type)(z_extent / z_res_step)) > 0.0)
			data_set_z_size = (hcp_surface_index_data_type)(z_extent / z_res_step) + 1;
		else
			data_set_z_size = (hcp_surface_index_data_type)(z_extent / z_res_step);

		return { data_set_x_size,data_set_y_size };
	}

	//Create Hex grid of x-y Cartesian coordinate range with specified initial value and hex grid size
	bool create_hcp_grid(T initial_hcp_data_value, glm::vec3 min_range, glm::vec3 max_range, float hcp_size) {
		//float x_extent = max_range.x - min_range.x;
		//float y_extent = max_range.y - min_range.y;
		//float y_extent = max_range.z - min_range.z;
		//float z_mult = 2.0f * sqrt(6.0f) / 3.0f;

		//float x_res_step = hcp_size * 2.0f;
		//float y_res_step = hcp_size * (3.0f / sqrt(3.0f));
		//float z_res_step = voxel_generator_parameters.resolution_step * z_mult;

		//hcp_surface_index_data_type data_set_x_size, data_set_y_size, data_set_z_size;

		//if (x_extent / x_res_step - float((hcp_surface_index_data_type)(x_extent / x_res_step)) > 0.0f)
		//{
		//	data_set_x_size = (hcp_surface_index_data_type)(x_extent / x_res_step) + 1;
		//}
		//else
		//{
		//	data_set_x_size = (hcp_surface_index_data_type)(x_extent / x_res_step);
		//}

		//if (x_extent / y_res_step - float((hcp_surface_index_data_type)(y_extent / y_res_step)) > 0.0f)
		//{
		//	data_set_y_size = (hcp_surface_index_data_type)(y_extent / y_res_step) + 1;
		//}
		//else
		//{
		//	data_set_y_size = (hcp_surface_index_data_type)(y_extent / y_res_step);
		//}

		//if (z_size / z_res_step - float((hcp_surface_index_data_type)(z_size / z_res_step)) > 0.0)
		//	data_set_z_size = (hcp_surface_index_data_type)(z_size / z_res_step) + 1;
		//else
		//	data_set_z_size = (hcp_surface_index_data_type)(z_size / z_res_step);

		grid_dimension = calc_voxel_volume_dimensions(initial_hcp_data_value, min_range, max_range, hcp_size);

		global_cart_origin = min_range;
		global_grid_origin = min_range;
		global_cart_origin = get_hcp_surface_cartesian_coordinate(min_range, hcp_size);

		return define_hcp_grid(initial_hcp_data_value, grid_dimension);
	}

	// create hex sub grid of specified x-y index coordinate range
	int create_hcp_sub_grid(hcp_surface_vec_data_type min_range, hcp_surface_vec_data_type max_range) {
		hcp_sub_grid_struct_type hcp_sub_grid(min_range, max_range);

		if (!hcp_sub_grid.valid_range(global_grid_origin, global_grid_origin + grid_dimension)) {
			return -1;
		}

		hcp_sub_grid.hcp_grid_range_id = hcp_sub_grid_id_key.get_available_id_key();

		hcp_sub_grids.push_back(hcp_sub_grid);

		return hcp_sub_grid.hcp_grid_range_id;
	}

	// create hex sub grid of specified x-y-z index coordinate range and value
	bool create_hcp_sub_grid(T initial_hcp_value, hcp_surface_vec_data_type min_range, hcp_surface_vec_data_type max_range) {
/* TO BE IMPLEMENTED !!!!!!!!!!!!!!!
		if (create_hcp_sub_grid(min_range, max_range) < 0) {
			return false;
		}

		size_t num_rows = max_range.y - min_range.y + 1;
		size_t num_cols = max_range.x - min_range.x + 1;

		hcp_surface_index_data_type child_hcp_origin_index = get_hcp_surface_matrix_data_index(min_range);

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
				hcp_surface_index_data_type hcp_index = child_hcp_origin_index + index_offset + col;

				hcp_grid[hcp_index] = initial_hcp_value;
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
	bool define_hcp_grid(T initial_hcp_data_value, hcp_surface_vec_data_type dimension) {
		define_initial_hcp_data_value(initial_hcp_data_value);
		return define_hcp_grid(dimension);
	}

	//Create Hex grid of x-y dimension with currently defined initial value
	bool define_hcp_grid(hcp_surface_vec_data_type dimension) {
		if (!dimension.x || !dimension.y || !dimension.z) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "Define hex grid :: Cannot define hex grid having a zero dimension\n");
			return false;
		}

		hcp_surface_index_data_type surface_grid_data_size = calculate_hcp_surface_matrix_data_size(dimension.x, dimension.y, dimension.z);
printf("hcp_grid_base_class :: define_hcp_grid 0000 %i:%i:%i:%i\n", dimension.x, dimension.y, dimension.z, surface_grid_data_size);

		if (surface_grid_data_size > MAX_PEMISSIBLE_HCP_GRID_SIZE) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "Define hCP grid :: Cannot define hex grid. HCP grid has dimensions that are constitute a hcp grid beyond permisible max size.\n");
			return false;
		}

		hcp_grid = std::vector<T>(surface_grid_data_size, initial_hcp_data_value);
		//hcp_grid = std::vector<T>(7800, initial_hcp_data_value);
		grid_dimension = dimension;
printf("hcp_grid_base_class :: define_hcp_grid 11111 %i : %i\n", surface_grid_data_size, hcp_grid.size());

		if (std::is_same<T, bool>::value) { hcp_grid_value_data_type = hcp_grid_value_data_type_enum::Boolean; }
		if (std::is_same<T, int>::value) { hcp_grid_value_data_type = hcp_grid_value_data_type_enum::Integer; }
		if (std::is_same<T, float>::value) { hcp_grid_value_data_type = hcp_grid_value_data_type_enum::Float; }
		if (std::is_same<T, double>::value) { hcp_grid_value_data_type = hcp_grid_value_data_type_enum::Double; }

		return true;
	}

	// Clear hex grid of all data values and initilaise it ready to be reused
	// Need to rename it to clear_hcp_grid()??????
	void delete_hcp_grid() {
		// Including any of the following crashes application in certain circumstances with error ocurring in the
		// for  or delete_hcp_grid_data(f) statements.
		// Do know why since nothing seems to be wrong with the for statement and the delete_hcp_grid_data();
		// statement is a dummy C++ stub for possible future implementation. Very strange !!!!!!!
		// Left commented out as has no impact or need. 
		//for (size_t i = 0; i < hcp_grid.size();i++) {
		//for (hcp_surface_index_data_type i = hcp_grid.size()-1; i>=0;i--) {
		//for (hcp_surface_index_data_type f = 5; f>=0;f--) {
			//delete_hcp_grid_data(f);
		//}
printf("hcp_grid_base_class :: delete_hcp_grid 000"); printf(" %zu\n", hcp_grid.size());

		hcp_centers_x.clear();
		hcp_centers_y.clear();
		hcp_centers_z.clear();
		hcp_colors.clear();

		hcp_grid.clear();

		hcp_centers_x.shrink_to_fit();
		hcp_centers_y.shrink_to_fit();
		hcp_centers_z.shrink_to_fit();
		hcp_colors.shrink_to_fit();

		hcp_grid.shrink_to_fit();
printf("hcp_grid_base_class :: delete_hcp_grid 111"); printf(" %zu\n", hcp_grid.size());
	}

	//bool resize_hcp_grid() {
	//	return resize_hcp_grid(grid_dimension);
	//}

	//bool resize_hcp_grid(hcp_surface_vec_data_type dimension) {
	//	if (!dimension.x || !dimension.y) {
	//	// Error message that have zero dimension grid
	//		return false;
	//	}

	//	size_t surface_grid_data_size = calculate_hcp_surface_matrix_data_size(dimension.x, dimension.y, dimension.z);

	//	if (surface_grid_data_size > MAX_PEMISSIBLE_HEX_GRID_SIZE) {
	//		// Error message that have hex grid size too large
	//		return false;
	//	}

	//	hcp_grid.resize(surface_grid_data_size);
	//	grid_dimension = dimension;

	//	return true;
	//}

	// ###########################################################################

	// Retreive the 3 dim x,y,z coordinate that the matrix index corresponds to
	hcp_surface_vec_data_type get_matrix_coordinate(hcp_surface_index_data_type matrix_index) {
	glm::ivec3 matrix_coord;

		hcp_surface_index_data_type t0 = get_z_layer_total(0);// total number for even z level
		hcp_surface_index_data_type t1 = get_z_layer_total(1);// total number for odd  z level

		hcp_surface_index_data_type zt = t0 + t1; // total number for even + odd z levels

		bool even_z_level;

		if (matrix_index < t0) { // Voxel is in the even z=0 layer
			matrix_coord.z = 0;
			even_z_level = true;
		}
		else {
			hcp_surface_index_data_type zt_index01 = hcp_surface_index_data_type(matrix_index / zt);
			hcp_surface_index_data_type zt_index02 = hcp_surface_index_data_type((matrix_index - zt_index01 * zt) / t0);

			if (zt_index02 > 0) { // Have matrix_index in the odd  z level
				matrix_coord.z = zt_index01 * 2 + 1;
				even_z_level = false;
			}
			else {// Have matrix_index in the even z level
				matrix_coord.z = zt_index01 * 2;
				even_z_level = true;
			}
		}

		// Get the array matrix index value of the x=0, y=0 z= matrix_coord.z voxel coordinate
		hcp_surface_index_data_type zeroth_index = matrix_index - get_index_value(0, 0, matrix_coord.z);

		hcp_surface_index_data_type yt = 2 * grid_dimension.x - 1; // Even row number of voxels + odd row of voxels of any z level

		if (even_z_level) {
			hcp_surface_index_data_type yt_index01, yt_index02;

			yt_index01 = hcp_surface_index_data_type(zeroth_index / yt);

			if (((zeroth_index - yt_index01 * yt) % yt) < grid_dimension.x)
				yt_index02 = 0;
			else
				yt_index02 = 1;

			matrix_coord.y = yt_index01 * 2 + yt_index02;
			matrix_coord.x = zeroth_index - ((hcp_surface_index_data_type(matrix_coord.y / 2) + matrix_coord.y % 2) * grid_dimension.x + hcp_surface_index_data_type(matrix_coord.y / 2) * (grid_dimension.x - 1));
		}
		else {
			hcp_surface_index_data_type yt_index01, yt_index02;

			yt_index01 = hcp_surface_index_data_type(zeroth_index / yt);

			if (((zeroth_index - yt_index01 * yt) % yt) < grid_dimension.x - 1)
				yt_index02 = 0;
			else
				yt_index02 = 1;

			matrix_coord.y = yt_index01 * 2 + yt_index02;

//if(matrix_coord.z > 0){
//QMessageBox::information(0, "get_matrix_coordinate", "get_matrix_coordinate 0BBB: " + QString::number(grid_dimension.x) + " : " + QString::number(even_z_level) + ":" + QString::number(yt) + ":" + QString::number(zeroth_index) + ":" +
//							"\n " + QString::number(yt_index01) + " : " + QString::number(yt_index02) + ":" + QString::number((zeroth_index - (yt_index01 * yt + yt_index02 * grid_dimension.x - 1)))), QMessageBox::Ok);
//			}

			matrix_coord.x = zeroth_index - ((hcp_surface_index_data_type(matrix_coord.y / 2) + matrix_coord.y % 2) * (grid_dimension.x - 1) + hcp_surface_index_data_type(matrix_coord.y / 2) * grid_dimension.x);
		}

//if(matrix_coord.z == 0){
//QMessageBox::information(0, "get_matrix_coordinate", "get_matrix_coordinate 01: " + QString::number(grid_dimension.x) + " : " + QString::number(even_z_level) + ":" + QString::number(yt) + ":" + QString::number(zeroth_index) + ":" +
//							"\n " + QString::number(matrix_coord.x) + " : " + QString::number(matrix_coord.y) + ":" + QString::number(matrix_coord.z), QMessageBox::Ok);
//}

		return matrix_coord;
	}

	// Retrieve the Cartesian coord of the hex grid x-y index coordinate hcp_surface_coord and hex grid cell size hcp_size
	glm::vec3 get_hcp_surface_cartesian_coordinate(hcp_surface_vec_data_type hcp_surface_coord, float hcp_size) {
		hcp_surface_index_data_type i = hcp_surface_coord.x, j = hcp_surface_coord.y, k = hcp_surface_coord.z;

		float sqrt3 = sqrt(3.0f), third = 1.0f / 3.0f, z_mult = 2.0f * sqrt(6.0f) / 3.0f, sqrt3_2 = sqrt(1.5f);
		glm::vec3 hcp_cartesian_coordinate;

		//if (j % 2 == 0) {
		//	hcp_cartesian_coordinate.x = (float(i) * 2 + float(k % 2)) * hcp_size;
		//	hcp_cartesian_coordinate.y = (sqrt3 * float(j) + sqrt3 * third * float(k % 2)) * hcp_size;
		//}
		//else {
		//	hcp_cartesian_coordinate.x = (-1.0f + float(i) * 2.0f + float(k % 2) + 2.0f * float((k + 1) % 2)) * hcp_size;
		//	hcp_cartesian_coordinate.y = (sqrt3 + sqrt3 * (float(j) - 1) + sqrt3 * third * float(k % 2)) * hcp_size;
		//}

		hcp_cartesian_coordinate.x = (2 * float(i) + float((j + k) % 2)) * hcp_size; // ++++
		hcp_cartesian_coordinate.y = (sqrt3 * (float(j) + third * float(k % 2))) * hcp_size; // ++++

		hcp_cartesian_coordinate.z = (z_mult * float(k) * hcp_size);

		return hcp_cartesian_coordinate;
	}

	// Retrieve the Cartesian coord of the hex grid x-y index coordinate hcp_surface_coord
	glm::vec3 get_hcp_surface_cartesian_coordinate(hcp_surface_vec_data_type hcp_surface_coord) {
		return get_hcp_surface_cartesian_coordinate(hcp_surface_coord, hcp_size);
	}

	// Retrieve the global Cartesian coord of the hex grid x-y index coordinate hcp_surface_coord
	glm::vec3 get_hcp_surface_world_cartesian_coordinate(hcp_surface_vec_data_type voxel_matrix_coord) {
		glm::vec3  voxel_cart_coord = get_hcp_surface_cartesian_coordinate(voxel_matrix_coord) + global_cart_origin;

		return voxel_cart_coord;
	}

	// Retrieve the Cartesian coord of the hex grid cell of index
	glm::vec3 get_hcp_surface_world_cartesian_coordinate(hcp_surface_index_data_type index) {
		hcp_surface_vec_data_type voxel_matrix_coord = get_matrix_coordinate(index);
//printf("get_hcp_surface_world_cartesian_coordinate index 000 : %i : %i : %i \n",index, voxel_matrix_coord.x, voxel_matrix_coord.y);

		glm::vec3  voxel_cart_coord = get_hcp_surface_cartesian_coordinate(voxel_matrix_coord) + global_cart_origin;
//printf("get_hcp_surface_world_cartesian_coordinate index 111 : %f : %f : %f : %f\n", voxel_cart_coord.x, voxel_cart_coord.y, global_cart_origin.x, global_cart_origin.y);

		return voxel_cart_coord;
	}

	// Retrieve the hex grid index coordinate that corresponds to the hex x-y index coordinate matrix_coord
	hcp_surface_index_data_type get_hcp_surface_matrix_data_index(hcp_surface_vec_data_type matrix_coord) {// y in matrix_coord must be the corrected_y as defined in get_hcp_surface_matrix_bit_location
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
		hcp_surface_index_data_type index = index_of_hcp_cell_with_cartesian_coord(x,y);

		if index > -1 then perform whatever task is required to assign a value to the vector array element
		hcp_surface_matrix_data[index];
	*/

	// Find the index of the one dimensional vertex vector array that a point P of cartesian coordinte
	// (x,y) will be within the bounds of a 2D hexagon voxel cell.
	hcp_surface_index_data_type index_of_hcp_cell_with_cartesian_coord(float x, float y, float z) {
		hcp_surface_vec_data_type hcp_coord = hcp_cell_coord_from_cartesian(x, y, z);

		if (cartesian_coord_within_grid_bounds(hcp_coord))
		{
			return get_index_value(hcp_coord.x, hcp_coord.y, hcp_coord.z);
		}
		else
		{
			return -1;
		}
	}

	// Determine if a point P of cartesian coordinte (x,y) is within the limits of
	// the dimensions of the hexagonal grid that is stored in the computer memory
	bool cartesian_coord_within_grid_bounds(float x, float y, float z) {
		glm::ivec3 hcp_coord = hcp_cell_coord_from_cartesian(x, y, z);

		return cartesian_coord_within_grid_bounds(hcp_coord);
	}

	// Determine if a point P of hex grid x-y index coordinates is within the limits of
	// the dimensions of the hexagonal grid that is stored in the computer memory
	bool cartesian_coord_within_grid_bounds(hcp_surface_vec_data_type hcp_coord) {
		if (hcp_coord.x < 0 || hcp_coord.y < 0 || hcp_coord.z < 0) { return false; }

		//if (hcp_coord.y % 2 == 0) {// even row
		//	if (hcp_coord.x > grid_dimension.x) { return false; }
		//}
		//else {// odd row
		//	if (hcp_coord.x > grid_dimension.x - 1) { return false; }
		//}

		//if (hcp_coord.y > grid_dimension.y) { return false; }

		if (hcp_coord.z % 2 == 0) {// even level
			if (hcp_coord.y % 2 == 0){// even row
				if (hcp_coord.x >= grid_dimension.x) return false;
			} else {// odd row
				if (hcp_coord.x >= grid_dimension.x - 1) return false;
			}
		} else {// odd level
			if(hcp_coord.y % 2 == 0){// even row
				if (hcp_coord.x >= grid_dimension.x-1) return false;
			}
			else {// odd row
				if (hcp_coord.x >= grid_dimension.x) return false;
			}
		}

		if (hcp_coord.y >= grid_dimension.y || hcp_coord.z >= grid_dimension.z) return false;

		return true;
	}

	// Obtain the hex grid index coordinates of the hex grid that a point P of cartesian coordinte
	// (x,y,z) will be found to be within the bounds of a 2D hexgagon voxel cell.
	hcp_surface_vec_data_type hcp_cell_coord_from_cartesian(float x, float y, float z) {
		float voxel_radius = hcp_size;
		//float voxel_height = voxel_radius * (sqrt(1.5f) + sqrt(6.0f) / 3.0f);
		float voxel_height = voxel_radius *  (sqrt(6.0f) / 3.0f)*2.0f;
		float grid_x, grid_y;

		float grid_z = z - global_cart_origin.z;

		int level = -1;

		if (grid_z < -voxel_height/2.0f) { return { -1,-1,-1 }; } // No negative values permited in hcp automata

		level = (int) std::floor((grid_z + voxel_height/2.0f) / voxel_height);

		bool level_is_even = level % 2 == 0;

//printf("hcp_grid_base:hcp_cell_coord_from_cartesian : %.3f : %i : %i\n", grid_z, level, level_is_even);

		if (level_is_even) {
			grid_x = x - global_cart_origin.x;
			grid_y = y - global_cart_origin.y;
			return get_hcp_voxel_cell_coord_even_level(grid_x, grid_y, grid_z, voxel_height, level);
		}
		else {
			grid_x = x - (global_cart_origin.x + voxel_radius);
			grid_y = y - (global_cart_origin.y + voxel_radius / sqrt(3.0f));
			return get_hcp_voxel_cell_coord_odd_level(grid_x, grid_y, grid_z, voxel_height, level);
		}

	}

	// Calculate and return the voxel matrix coordinate of a voxel cell for a given 
	// Cartesian coordinate (grid_x, grid_y, grid_z) within an even voxel grid matrix level
	glm::ivec3 get_hcp_voxel_cell_coord_even_level(float grid_x, float grid_y, float grid_z, float voxel_height, int level) {
		float grid_radius = hcp_size;
		float grid_height = grid_radius * (sqrt(3.0f));
		float c           = grid_radius / (sqrt(3.0f));

//printf("hcp_grid_base : get_hcp_voxel_cell_coord_even_level 000 hex_size : %f :grid_height %f : grid_radius %f : c %f \n", hcp_size, grid_height, grid_radius,c);
//printf("get_hcp_voxel_cell_coord_even_level 111 grid_origin :x %f :y %f :z %f \n", global_cart_origin.x, global_cart_origin.y, global_cart_origin.z);
//printf("get_hcp_voxel_cell_coord_even_level 222 :x %f :y %f :z %f \n", grid_x, grid_y, grid_z);

		int row, column;

		if (grid_y < -1.0 / sqrt(3.0))
			row = (int)((grid_y - grid_height) / grid_height);
		else
			row = (int)(grid_y / grid_height);

		bool row_is_odd = abs(row % 2) == 1;

		if (row_is_odd) {
			column = (int)floor(grid_x / (hcp_size*2));
		} else {
			column = (int)floor((grid_x + grid_radius) / (hcp_size * 2));
		}

		
//printf("hcp_grid_base : get_hcp_voxel_cell_coord_even_level 333 :row %i :col %i :level %i \n", row, column, level);
		// Position of point relative to box it is in
		float rel_y = grid_y - (row * grid_height);
		float rel_x;
		float rel_z = grid_z - (level * voxel_height);

		if (row_is_odd){
			rel_x = grid_x - ((column * (hcp_size*2)) + grid_radius);
		} else {
			rel_x = grid_x - (column * (hcp_size * 2));
		}

		float m = 1.0f / sqrt(3.0f);

//printf("hcp_grid_base : get_hcp_voxel_cell_coord_even_level 444 :rel_x %f :rel_y %f : m %f : line %f :%f \n", rel_x,rel_y, m, m * rel_x + 2.0*c, -m * rel_x + 2.0 * c);
		// Work out if the point is above either of the hexagon's top edges
		if (rel_y >= (m * rel_x + 2.0 * c) && rel_x < 0) { // LEFT edge
			row++;
			if (!row_is_odd) { // Have even row
				column--;
				row_is_odd = true;
				rel_x = grid_x - ((column * (hcp_size * 2)) + grid_radius);
			}else
				rel_x = rel_x + grid_radius;
			
			rel_y = grid_y - (row * grid_height);
		} else {
			if (rel_y >= (-m * rel_x) + 2.0 * c && rel_x >= 0) {// RIGHT edge
				row++;
				if (row_is_odd) {
					column++;
					row_is_odd = false;
					rel_x = grid_x - (column * (hcp_size * 2));
				} else
					rel_x = rel_x - grid_radius;
				
				rel_y = grid_y - (row * grid_height);
			}
		}

		glm::ivec3 voxel_coord;
		voxel_coord.x = column;
		voxel_coord.y = row;
		voxel_coord.z = level;// +++++

//printf("hcp_grid_base : get_hcp_voxel_cell_coord_even_level 4AA :rel_x %f :rel_y %f : rel_z %f \n", rel_x,rel_y,rel_z);
//printf("get_hcp_voxel_cell_coord_even_level 555 :row %i :col %i \n",voxel_coord.y, column);
		// Get voxel z coord
		// Find distance to upper plane of the zone that the point exists in of the voxel
		// and if it is below the plane (ie distance negative) it is in the voxel, otherwise
		// it is in the neighbouring voxel of the upper level.
/*
		float m0 = -1 / sqrt(3.0f);//slope of line in xy plane from point point 6 to point 3
		float m1 =  1 / sqrt(3.0f);//slope of line in xy plane from point point 5 to point 2

		glm::vec3 pq_vector = { rel_x,rel_y,rel_z - sqrt(1.5f) }; //distance vector from rel_z to top voxel vertex point.

		if (rel_x >= 0 && rel_y > m0 * rel_x) {// region 0 
			glm::vec3 normal_vector = { 1.0f,1.0f / sqrt(3.0f),(2.0f * sqrt(6.0f)) / 3.0f };

			if (distance_to_plane(normal_vector, pq_vector) < 0)
				voxel_coord.z = level;
			else {
				voxel_coord.z = level + 1;
				if (row_is_odd)	voxel_coord.x++;
			}
printf("hcp_grid_base : get_hcp_voxel_cell_coord_even_level 666 :col %i :row %i :level %i \n", voxel_coord.x, voxel_coord.y, voxel_coord.z);
			return voxel_coord;
		}

		if (rel_y <= m0*rel_x && rel_y < m1 * rel_x) {// region 1 
			glm::vec3 normal_vector = { 0.0f,-1.0f / sqrt(3.0f),(2.0f * sqrt(6.0f)) / 3.0f };

			if (distance_to_plane(normal_vector, pq_vector) < 0)
				voxel_coord.z = level;
			else {
				voxel_coord.y--;
				voxel_coord.z = level + 1;
			}
printf("hcp_grid_base : get_hcp_voxel_cell_coord_even_level 777 :col %i :row %i :level %i \n", voxel_coord.x, voxel_coord.y, voxel_coord.z);
			return voxel_coord;
		}

		if (rel_x < 0 && rel_y >= m1 * rel_x) {// region 2 
			glm::vec3 normal_vector = {-1.0f,-1.0f / sqrt(3.0f),(2.0 * sqrt(6.0f)) / 3.0f };

			if (distance_to_plane(normal_vector, pq_vector) < 0)
				voxel_coord.z = level;
			else {
				voxel_coord.z = level + 1;
				if (!row_is_odd) voxel_coord.x--; 
			}
printf("hcp_grid_base : get_hcp_voxel_cell_coord_even_level 888 :col %i :row %i :level %i \n", voxel_coord.x, voxel_coord.y, voxel_coord.z);
			return voxel_coord;
		}
*/
		return voxel_coord; // Possible bug created by this ?????
	}

	// Calculate and return the voxel matrix coordinate of a voxel cell for a given 
	// Cartesian coordinate (grid_x, grid_y, grid_z) within an odd voxel grid matrix level
	glm::ivec3 get_hcp_voxel_cell_coord_odd_level(float grid_x, float grid_y, float grid_z, float voxel_height, int level) {
		float grid_radius = hcp_size;
		float grid_height = grid_radius * (sqrt(3.0f));
		float c           = grid_radius / (sqrt(3.0f));

//printf("get_hcp_voxel_cell_coord_odd_level 000 hex_size : %f :grid_height %f : grid_radius %f : c %f \n", hcp_size, grid_height, grid_radius,c);
//printf("get_hcp_voxel_cell_coord_odd_level 111 grid_origin :x %f :y %f :z %f \n", global_cart_origin.x, global_cart_origin.y, global_cart_origin.z);
//printf("get_hcp_voxel_cell_coord_odd_level 222 :x %f :y %f :z %f \n", grid_x, grid_y, grid_z);

		int row;
		int column;

		if (grid_y < -1.0 / sqrt(3.0))
			row = (int)((grid_y - grid_height) / grid_height);
		else
			row = (int)(grid_y / grid_height);

		bool row_is_odd = abs(row % 2) == 1;

		column = (int)floor((grid_x + grid_radius) / (hcp_size * 2));

//printf("get_hcp_voxel_cell_coord_odd_level 333 :row %i :col %i :level %i \n", row, column, level);

		// Position of point relative to box it is in
		float rel_y = grid_y - (row * grid_height);
		float rel_x;
		float rel_z = grid_z - (level * voxel_height);

		if (row_is_odd){
			rel_x = grid_x - (column * (hcp_size * 2)) + grid_radius;
		} else {
			rel_x = grid_x - (column * (hcp_size * 2));
		}

		float m = 1.0f / sqrt(3.0f);

//printf("get_hcp_voxel_cell_coord_odd_level 444 :rel_x %f :rel_y %f : m %f : line %f :%f \n", rel_x,rel_y, m, m * rel_x + 2.0*c, -m * rel_x + 2.0 * c);
		// Work out if the point is above either of the hexagon's top edges
		if (rel_y >= (m * rel_x + 2.0 * c) && rel_x < 0) { // LEFT edge
			row++;
			if (row_is_odd) { // Have even row
				column--;
				row_is_odd = true;
				rel_x = grid_x - ((column * (hcp_size * 2)) + grid_radius);
			}else
				rel_x = rel_x + grid_radius;
			
			rel_y = grid_y - (row * grid_height);
		} else {
			if (rel_y >= (-m * rel_x) + 2.0 * c && rel_x >= 0) {// RIGHT edge
				row++;
				if (!row_is_odd) {
					column++;
					row_is_odd = false;
					rel_x = grid_x - (column * (hcp_size * 2));
				} else
					rel_x = rel_x - grid_radius;
				
				rel_y = grid_y - (row * grid_height);
			}
		}

		glm::ivec3 voxel_coord;
		voxel_coord.x = column;
		voxel_coord.y = row;
		voxel_coord.z = level;

//printf("get_hcp_voxel_cell_coord_odd_level 4AA :rel_x %f :rel_y %f : rel_z %f \n", rel_x,rel_y,rel_z);
//printf("get_hcp_voxel_cell_coord_odd_level 555 :row %i :col %i \n",row, column);
		
		// Get voxel z coord
		// Find distance to upper plane of the zone that the point exists in of the voxel
		// and if it is below the plane (ie distance negative) it is in the voxel, otherwise
		// it is in the neighbouring voxel of the upper level.
/*
		float m0 = -1 / sqrt(3.0f);//slope of line in xy plane from point point 6 to point 3
		float m1 =  1 / sqrt(3.0f);//slope of line in xy plane from point point 5 to point 2

		glm::vec3 pq_vector = { rel_x,rel_y,rel_z - sqrt(1.5f) }; //distance vector from rel_z to top voxel vertex point.

		if (rel_x <= 0 && rel_y < m0 * rel_x) { //region 0
			glm::vec3 normal_vector = { -1.0f,-1.0f / sqrt(3.0f),(2.0 * sqrt(6.0f)) / 3.0f };

			if (distance_to_plane(normal_vector, pq_vector) < 0)
				voxel_coord.z = level;
			else {
				voxel_coord.z = level + 1;
				if (row_is_odd) voxel_coord.x--;
			}
//printf("get_hcp_voxel_cell_coord_odd_level 666 :col %i :row %i :level %i \n", voxel_coord.x, voxel_coord.y, voxel_coord.z);
			return voxel_coord;
		}

		if (rel_y >= m0 * rel_x && rel_y > m * rel_x) { //region 1
			glm::vec3 normal_vector = { 0.0f,1.0f / sqrt(3.0f),(2.0 * sqrt(6.0f)) / 3.0f };

			if (distance_to_plane(normal_vector, pq_vector) < 0)
				voxel_coord.z = level;
			else {
				voxel_coord.y++;
				voxel_coord.z = level + 1;
			}
//printf("get_hcp_voxel_cell_coord_odd_level 777 :col %i :row %i :level %i \n", voxel_coord.x, voxel_coord.y, voxel_coord.z);
			return voxel_coord;
		}

		if (rel_x > 0 && rel_y <= m1 * rel_x) {// region 2 
			glm::vec3 normal_vector = { 1.0f,1.0f / sqrt(3.0f),(2.0 * sqrt(6.0f)) / 3.0f };
			
			if (distance_to_plane(normal_vector, pq_vector) < 0)
				voxel_coord.z = level;
			else {
				voxel_coord.z = level + 1;
				if (!row_is_odd) voxel_coord.x++;
			}
//printf("get_hcp_voxel_cell_coord_odd_level 888 :col %i :row %i:level %i \n", voxel_coord.x, voxel_coord.y, voxel_coord.z);
			return voxel_coord;
		}
*/
		return voxel_coord; // Possible bug created by this ?????
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


	// Test  to see if hex grid cell of x-y index coordinate grid_coordinate is within a hex grid hcp_grid
	bool grid_coordinate_in_hcp_grid_bounds(hcp_grid_base_class* hcp_grid, hcp_surface_vec_data_type grid_coordinate) {
		if (!hcp_grid) return false;
		if (hcp_grid->grid_dimension.x < 1 || hcp_grid->grid_dimension.y < 1) { return false; }

		if (grid_coordinate.y < hcp_grid->global_grid_origin.y || grid_coordinate.y > hcp_grid->global_grid_origin.y + hcp_grid->grid_dimension.y) { return false; }

		if (grid_coordinate.y % 2 == 0) { // have even row
			if (grid_coordinate.x < hcp_grid->global_grid_origin.x || grid_coordinate.x > hcp_grid->global_grid_origin.x + hcp_grid->grid_dimension.x) { return false; }
		}
		else { // have odd row
			if (grid_coordinate.x < hcp_grid->global_grid_origin.x || grid_coordinate.x > hcp_grid->global_grid_origin.x + hcp_grid->grid_dimension.x - 1) { return false; }
		}

		return true;
	}

	// Test  to see if hex grid cell of x-y index coordinate grid_coordinate is within this class hex grid
	bool grid_coordinate_in_hcp_grid_bounds(hcp_surface_vec_data_type grid_coordinate) {
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

	// Retrun the index value of the first hex sub grid that exists within the list of hex sub grids for
	//  this class that has a coordinate global_grid_coordinate existing with this class hex grid bounds
	hcp_surface_index_data_type get_hcp_sub_grid_index_with_global_grid_coord(hcp_surface_vec_data_type global_grid_coordinate) {
		if (hcp_sub_grids.size() < 1) { return -1; }

		for (int i = 0; i < hcp_sub_grids.size(); i++) {
			hcp_sub_grid_struct_type hcp_sub_grid = hcp_sub_grids[i];

			if (hcp_sub_grid.grid_coordinate_in_sub_hcp_bounds(global_grid_coordinate)) {
				return i;
				break;
			}
		}

		return -1;
	}


	void define_sub_grid_plot_data(hcp_sub_grid_struct_type& hcp_sub_grid) {
		hcp_sub_grid.sub_hcp_centers_x.clear();
		hcp_sub_grid.sub_hcp_centers_x.shrink_to_fit();
		hcp_sub_grid.sub_hcp_centers_y.clear();
		hcp_sub_grid.sub_hcp_centers_y.shrink_to_fit();
		hcp_sub_grid.sub_hcp_centers_z.clear();
		hcp_sub_grid.sub_hcp_centers_z.shrink_to_fit();

		hcp_surface_index_data_type child_hcp_origin_index = get_hcp_surface_matrix_data_index(hcp_sub_grid.lower_range);

		size_t num_rows   = hcp_sub_grid.upper_range.y - hcp_sub_grid.lower_range.y + 1;
		size_t num_cols   = hcp_sub_grid.upper_range.x - hcp_sub_grid.lower_range.x + 1;
		size_t num_levels = hcp_sub_grid.upper_range.z - hcp_sub_grid.lower_range.z + 1;

		if (hcp_sub_grid.upper_range.x % 2 == 0) { num_cols += 1; } // even max row
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
				hcp_surface_index_data_type hcp_index = child_hcp_origin_index + index_offset + col;

				hcp_sub_grid.sub_hcp_centers_x.push_back(hcp_centers_x[hcp_index]);
				hcp_sub_grid.sub_hcp_centers_y.push_back(hcp_centers_y[hcp_index]);
				hcp_sub_grid.sub_hcp_centers_z.push_back(hcp_centers_z[hcp_index]);
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
	T initial_hcp_data_value; // Initial value that all hexagonal grid cells are assigned to


private:
	id_key_manager_class<unsigned int> hcp_sub_grid_id_key;


};