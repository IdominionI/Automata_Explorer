#pragma once

#include <ThirdParty/ImGui/imgui_docking/imgui_internal.h>

#include <FrameWork/AFW/ImGUI/ImGuiEx/imgui_custom.h>
#include <FrameWork/AFW/ImGUI/ImGuiEx/imgui_widgets.h>
#include <FrameWork/AFW/Interface/IconsFontAwesome4.h>

#include "Source/Widgets/ImGradientHDR.h"
#include "Source/Application/random_engine.h"

#include "cubic_sub_grid_manager.h"
#include "cubic_grid_brush.h"
#include "cubic_surface_import_export.h"

// Central main cubicagonal automata grid class that defines, stores, creates, displays and manages all aspects
// of the cubicagonal automata grid data to be used in the application
// This class has incorporated classes or structures that are used to modify, display or define the cubic grid 
//   gradient
//   random engine
//   cubic grid brush
//   cubic surface import/export

template <class T>
class cubic_grid_class : public cubic_grid_base_class<T> , public cubic_sub_grid_manager_class{
	using HGBC = cubic_grid_base_class<T>; // Need to define this so as to have access to the cubic_grid_base_class class : Due to using template
public:
	cubic_grid_class() {
        initialise_gradient();
        //random_engine.initialise_distribution_input_parameters();
    }
	cubic_grid_class(T initial_cubic_value, cubic_surface_vec_data_type dimension) : cubic_grid_base_class<T>(initial_cubic_value, dimension){
        initialise_gradient();
        //random_engine.initialise_distribution_input_parameters();
    }
	~cubic_grid_class() {}

    std::string gradient_dir_path = "./Gradients"; // Path name to the default directory that the gradient class exists within

    //bool first = true;

    std::vector<cubic_grid_cell_data_struct_type> cubic_grid_cells_data_list = {};// vector list of cubic grid data to display as an overlay on main grid display

	void delete_cubic_grid_data(cubic_surface_index_data_type i) override {
		// integer value does not need to be deleted
	}

	void invalidate_cubic_grid_data_value(cubic_surface_index_data_type index) override {
        HGBC::cubic_grid.at(index) = INVALID_CUBIC_INT_DATA_VALUE;// Need to cater for float, double and boolean data types !!!!
	}

    // The central main core function that manages the display of the cubic grid window panels that
    // the user interacts with to display and edit cubic grid data values.
	bool display_cubic_grid() override {
        if (HGBC::cubic_centers_x.size() == 0) { return false; }
//printf("cubic_grid_class::display_cubic_grid 000\n");

        display_grid_parameters_panel();

//printf("cubic_grid_class::display_cubic_grid 1111\n");
        if (edit_acubic_grid) {
            display_grid_edit_panel();
        } else{
            // Enable plot inputs and window interaction when not in edit mode and disable edit functions
            plot_flags = ImPlot3DFlags_Equal | ImPlot3DFlags_NoMenus | ImPlot3DFlags_NoLegend;// | ImPlotFlags_Crosshairs not included
            plot_window_flags = ImGuiWindowFlags_None;
        }
//printf("cubic_grid_class::display_cubic_grid 2222\n");

        display_automata_rule_results();// Display the current main cubicagonal grid data which the user can interact with

//printf("cubic_grid_class::display_cubic_grid 4444\n");
        return true;
	}

    // Default initialisation of cubic grid data value gradient 
    void initialise_gradient() {
        state.AddColorMarker(0.0f, { 1.0f, 1.0f, 1.0f }, 1.0f);
        state.AddColorMarker(1.0f, { 0.0f, 1.0f, 0.0f }, 1.0f);
        //state.AddAlphaMarker(0.0f, 1.0f);
        //state.AddAlphaMarker(1.0f, 1.0f);
    }

    int define_new_cubic_grid_cell_data() {
        cubic_grid_cell_data_id++;
        cubic_grid_cell_data_struct_type cubic_grid_cell_data;
        cubic_grid_cell_data.cubic_grid_cell_data_id = cubic_grid_cell_data_id;

        cubic_grid_cells_data_list.push_back(cubic_grid_cell_data);

        return cubic_grid_cell_data.cubic_grid_cell_data_id;
    }

    // Clear all the cubic grid cell data of the cubic_grid_cells_data_list to be reused
    void clear_all_cubic_grid_cell_data() {
        cubic_grid_cell_data_id = -1;
        for (cubic_grid_cell_data_struct_type &cubic_grid_cell : cubic_grid_cells_data_list) {
            cubic_grid_cell.clear_cubic_grid_cell_data();
        }
    }

    // Clear cubic grid cell data of the cubic_grid_cells_data_list of id number cubic_grid_cell_data_id
    bool clear_cubic_grid_cell_data(int  cubic_grid_cell_data_id) {
        for (size_t i = 0; i < cubic_grid_cells_data_list.size(); i++) {
            if (cubic_grid_cells_data_list[i].cubic_grid_cell_data_id == cubic_grid_cell_data_id) {
                cubic_grid_cells_data_list[i].clear_cubic_grid_cell_data();
                return true;
            }
        }

        return false;
    }

    // Delete all the cubic grid cell data of the cubic_grid_cells_data_list
    bool delete_cubic_grid_cell_data(int cubic_grid_cell_data_id) {
        for (size_t i = cubic_grid_cells_data_list.size()-1; i > -1;i--) {
            if (cubic_grid_cells_data_list[i].cubic_grid_cell_data_id == cubic_grid_cell_data_id) {
                cubic_grid_cells_data_list[i].clear_cubic_grid_cell_data();
                cubic_grid_cells_data_list.erase(cubic_grid_cells_data_list.begin() + i);
                return true;
            }
        }

        return false;
    }

    void delete_all_cubic_grid_cell_data() {
        clear_all_cubic_grid_cell_data();
        cubic_grid_cells_data_list.clear();
        cubic_grid_cells_data_list.shrink_to_fit();
    }

    // Get the cubic grid cell data entry within the cubic_grid_cells_data_list that has an ID value of cubic_grid_cell_data_id
    int get_cubic_grid_cell_data_index(int cubic_grid_cell_data_id) {
//printf("cubic_grid_class :: get_cubic_grid_cell_data_index 0000: %i : %i\n" , cubic_grid_cells_data_list.size(), cubic_grid_cell_data_id);
        for (size_t i = 0; i < cubic_grid_cells_data_list.size(); i++) {
//printf("cubic_grid_class :: get_cubic_grid_cell_data_index 1111: %i :  %i : %i\n",i , cubic_grid_cells_data_list[i].cubic_grid_cell_data_id, cubic_grid_cell_data_id);
            if (cubic_grid_cells_data_list[i].cubic_grid_cell_data_id == cubic_grid_cell_data_id) {
                return i;
            }
        }

        return -1;
    }

    // Add the cubic grid index of the main cubic grid cubic_grid_index to the cubic_grid_cells_data_list entry with index cubic_grid_cell_index
    bool add_cubic_grid_cell_index(int cubic_grid_cell_index, cubic_surface_index_data_type cubic_grid_index) {
        if (cubic_grid_cell_index < 0 || cubic_grid_cell_index >= cubic_grid_cells_data_list.size()) {
//printf("cubic_grid_class :: add_cubic_grid_cell_index %i : %i 000\n", cubic_grid_cell_index, cubic_grid_cells_data_list.size());
            return false;
        }
//printf("cubic_grid_class :: add_cubic_grid_cell_index %i : %i 1111\n", cubic_grid_cell_index, cubic_grid_cells_data_list.size());
        glm::vec3 cubic_cell_cart_coord = HGBC::get_cubic_surface_world_cartesian_coordinate(cubic_grid_index);
//printf("cubic_grid_class :: add_cubic_grid_cell_index %i : %i 2222\n", cubic_grid_cell_index, cubic_grid_cells_data_list.size());
        cubic_grid_cells_data_list[cubic_grid_cell_index].add_cubic_grid_cell(cubic_grid_index,cubic_cell_cart_coord.x, cubic_cell_cart_coord.y, cubic_cell_cart_coord.z);
//printf("cubic_grid_class :: add_cubic_grid_cell_index %i : %i 3333\n", cubic_grid_cell_index, cubic_grid_cells_data_list.size());
        return true;
    }

    // Add the cubic grid index of the main cubicagonal cubic grid cubic_grid_index to the cubic_grid_cells_data_list entry with a id number of  cubic_grid_cell_data_id
    bool add_cubic_grid_cell_id(int cubic_grid_cell_data_id, cubic_surface_index_data_type cubic_grid_index) {
        int cubic_grid_cell_index = get_cubic_grid_cell_data_index(cubic_grid_cell_data_id);
        if (cubic_grid_cell_index < 0) { return false; }

        return (add_cubic_grid_cell_index(cubic_grid_cell_index, cubic_grid_index));
    }

    bool define_cubic_grid_cell_coordinate() {

    }

    // Sub grid Test functions
    /*
    void logical_test_cubic_sub_grids() {
//printf("test_cubic_sub_grids::display 00000 \n");

        if (HGBC::cubic_sub_grids.size() > 1) {
// printf("test_cubic_sub_grids::display 11111 \n");           
            for (size_t i = 0; i < HGBC::cubic_sub_grids.size(); i++) {
                for (size_t j = 0; j < HGBC::cubic_sub_grids.size(); j++) {
                    if (i!=j) {
//printf("test_cubic_sub_grids::display 22222 %i : %i  \n",i,j);
                        cubic_sub_grid_struct_type cubic_sub_grid01 = HGBC::cubic_sub_grids[i];
                        cubic_sub_grid_struct_type cubic_sub_grid02 = HGBC::cubic_sub_grids[j];

                        if (cubic_sub_grids_overlap(cubic_sub_grid01, cubic_sub_grid02)) {
                            std::string info = "test_cubic_sub_grids :: CUBIC sub grid : " + std::to_string(cubic_sub_grid01.cubic_grid_range_id) + " overlaps with CUBIC sub grid : " + std::to_string(cubic_sub_grid02.cubic_grid_range_id) + "\n";
                            afw_globalc::get_current_logger()->log(LogLevel::INFO, info);
                        }

                        if (cubic_sub_grids_neighbours(cubic_sub_grid01, cubic_sub_grid02)) {
                            std::string info = "test_cubic_sub_grids :: CUBIC sub grid : " + std::to_string(cubic_sub_grid01.cubic_grid_range_id) + " is neighbours with CUBIC sub grid : " + std::to_string(cubic_sub_grid02.cubic_grid_range_id) + "\n";
                            afw_globalc::get_current_logger()->log(LogLevel::INFO, info);
                        }

                    }
                }
            }

        }
    }

    bool operation_testcubic_sub_grids() {
        if (!tested) {
            //if (HGBC::cubic_sub_grids.size() > 1) {
            //    cubic_sub_grid_struct_type merged_cubic_sub_grid;

            //    if (!merge_cubic_sub_grids(HGBC::cubic_sub_grids[0], HGBC::cubic_sub_grids[1], merged_cubic_sub_grid)) return false;

            //    HGBC::create_cubic_sub_grid(merged_cubic_sub_grid.lower_range, merged_cubic_sub_grid.upper_range);
            //    HGBC::define_sub_grid_plot_data(HGBC::cubic_sub_grids[HGBC::cubic_sub_grids.size() - 1]);
            //    tested = true;
            //}

            if (HGBC::cubic_sub_grids.size() > 0) {
                cubic_sub_grid_struct_type test_cubic_sub_grid1, test_cubic_sub_grid2;

                if (!cubic_sub_grid_partition_x(HGBC::cubic_sub_grids[0], { 10,10 }, test_cubic_sub_grid1)) { return false; }

                HGBC::define_sub_grid_plot_data(HGBC::cubic_sub_grids[0]);

                HGBC::create_cubic_sub_grid(test_cubic_sub_grid1.lower_range, test_cubic_sub_grid1.upper_range);
                HGBC::define_sub_grid_plot_data(HGBC::cubic_sub_grids[HGBC::cubic_sub_grids.size() - 1]);
               
                if (!cubic_sub_grid_partition_y(HGBC::cubic_sub_grids[0], { 10,10 }, test_cubic_sub_grid2)) { return false; }

                HGBC::define_sub_grid_plot_data(HGBC::cubic_sub_grids[0]);

                HGBC::create_cubic_sub_grid(test_cubic_sub_grid2.lower_range, test_cubic_sub_grid2.upper_range);
                HGBC::define_sub_grid_plot_data(HGBC::cubic_sub_grids[HGBC::cubic_sub_grids.size() - 1]);

                tested = true;
            }
        }

        return true;
    }
    */

    // Update the main cubicagonal grid color data with a value that corresponds to the color of the current cubic grid gradient display for that value
    // If the data value is out of range and beyond the bounds of the defied gradient, assign the min or max color value depending if the value is 
    // below or above the gradient min max display value
    void update_cubic_grid_colors() {

        for (size_t i = 0; i < HGBC::cubic_colors.size(); i++) {
            if ((float)HGBC::cubic_grid[i] < (float)state.min_grad_value || (float)HGBC::cubic_grid[i] > (float)state.max_grad_value) {
                ImU32 cubic_grid_edit_color = ImGui::GetColorU32(HGBC::cubic_grid_display_color);
                HGBC::cubic_colors[i] = cubic_grid_edit_color;
            }
            else {
                //float grid_value_pos = (float)HGBC::cubic_grid[i] / ((float)state.max_grad_value - (float)state.min_grad_value);
                float grid_value_pos = ((float)HGBC::cubic_grid[i] - (float)state.min_grad_value) / ((float)state.max_grad_value - (float)state.min_grad_value);
                std::array<float, 4> c = state.GetColorAndIntensity(grid_value_pos);
                ImU32 cubic_grid_edit_color = ImGui::GetColorU32({ c[0], c[1], c[2],c[3] });
                HGBC::cubic_colors[i] = cubic_grid_edit_color;
            }
        }
    }

    // Edit the main cubic grid data with the cubic grid brush data of edit_brush_grid
    // x_coord,y_coord is the x-y coordinate index of the main cubic grid that is to be referencd for editing
    // b_x_coord,b_y_coord  is the x-y index coordinate index of the cubic grid brush that is to be referencd for changing the main cubic grid to.
    // If cubic grid brush cell at x-y index coordinate  b_x_coord,b_y_coord has a value greater than 0, then the current main cubic grid at  x-y index coordinate  x_coord,y_coord is changed
    void draw_cubic_grid_brush(cubic_grid_class<T> *edit_brush_grid, cubic_surface_index_data_type x_coord, cubic_surface_index_data_type y_coord, cubic_surface_index_data_type z_coord, cubic_surface_index_data_type b_x_coord, cubic_surface_index_data_type b_y_coord, cubic_surface_index_data_type b_z_coord) {
// NOT YET IMPLEMENTED !!!!!!!!!!!!!
        cubic_surface_index_data_type grid_index  = HGBC::get_cubic_surface_matrix_data_index({ x_coord ,y_coord,z_coord });               // Get main cubic grid cell index that corresponds to the main cubic grid x-y index coordinate 
        cubic_surface_index_data_type brush_index = edit_brush_grid->get_cubic_surface_matrix_data_index({ b_x_coord ,b_y_coord,b_z_coord });// Get cubic brush grid cell index that corresponds to the cubic brush grid cell x-y index coordinate 
        if (grid_index > -1 && grid_index < HGBC::cubic_grid.size()) { // check that retrieved main cubic grid cell index is in the bounds of the current cubic grid array size
            if (brush_index > -1 && brush_index < edit_brush_grid->cubic_grid.size()) {// check that retrieved cubic grid brush cell index is in the bounds of the current cubic grid brush array size
//printf("cubic_grid_class::draw_cubic_grid_brush brush 11111 : brush value :%i\n", edit_brush_grid->cubic_grid[brush_index]);
                if (edit_brush_grid->cubic_grid[brush_index] > 0) {// If the cubic grid brush cell of index brush_index has a non zero value then edit main cubic grid cell data and color values
//printf("cubic_grid_class::draw_cubic_grid_brush00000  grid index: %i : brush index %i : \n", grid_index, brush_index);
//printf("cubic_grid_class::draw_cubic_grid_brush00000  grid coord: %i | %i | %i :: brush coord %i | %i | %i: \n", x_coord, y_coord, z_coord, b_x_coord, b_y_coord, b_z_coord);
                    update_cubic_grid_brush_colors(grid_index);
                }
            }
        }
    }

    // Change the current main cubic grid cell value with an array index grid_index to the current cubic_grid_edit_value
    // and update the color data with the color of the colour gradient that corresponds to the cubic_grid_edit_value
    void update_cubic_grid_brush_colors(cubic_surface_index_data_type grid_index) {
        HGBC::cubic_grid[grid_index]   = cubic_grid_edit_value; // Change cubic grid value
        float grid_value_pos         = cubic_grid_edit_value / ((float)state.max_grad_value - (float)state.min_grad_value); // get position in the gradient that corresponds to cubic_grid_edit_value
        std::array<float, 4> c       = state.GetColorAndIntensity(grid_value_pos);// get color data as a color array  from the gradient that is at position grid_value_pos in the gradient 
        ImU32 cubic_grid_edit_color    = ImGui::GetColorU32({ c[0], c[1], c[2],c[3] }); // convert color array data to an ImGui color U32 format
        HGBC::cubic_colors[grid_index] = cubic_grid_edit_color; // Update cubic grid cell color data 
    }

    // !!!!!!!!!!!!!!!!!!!!!!!! GET NEAREST 3D GRID MATRIX CELL TO MOUSE CURSOR !!!!!!!!!!!!!!!!!!!

    bool get_display_nearest_cubic_matrix_grid_cell_to_cursor(ImDrawList* draw_list, ImVec2 mouse_pos, ImVec2 plot_pos, ImVec2 plot_size, bool display_tooltip = false, bool display_line = true) {
        //void get_display_nearest_cubic_matrix_grid_cell_to_cursor(bool display_tooltip = false, bool display_line = true) {
            //ImDrawList* draw_list = ImPlot3D::GetPlotDrawList();
            //ImVec2 mouse_pos = ImGui::GetMousePos();
            //ImVec2 plot_pos = ImPlot3D::GetPlotRectPos();
            //ImVec2 plot_size = ImPlot3D::GetPlotRectSize();

            // Check if mouse is over plot
        bool is_hovered = ImGui::IsItemHovered();

        if (is_hovered) {
            // Find closest point to mouse in screen space
            int closest_idx = -1;
            float min_dist_sq = 1e10f; // Large value instead of FLT_MAX
            ImVec2 closest_px;

            for (int i = 0; i < HGBC::cubic_centers_x.size(); i++) {
                ImVec2 point_px = ImPlot3D::PlotToPixels(HGBC::cubic_centers_x[i], HGBC::cubic_centers_y[i], HGBC::cubic_centers_z[i]);
                float dx = point_px.x - mouse_pos.x;
                float dy = point_px.y - mouse_pos.y;
                float dist_sq = dx * dx + dy * dy;
                if (dist_sq < min_dist_sq) {
                    min_dist_sq = dist_sq;
                    closest_idx = i;
                    closest_px = point_px;
                }
            }

            // Draw line to closest point
            if (closest_idx >= 0) {
                if (display_line) { draw_list->AddLine(mouse_pos, closest_px, IM_COL32(255, 255, 0, 255), 2.0f); }

                // Draw tooltip
                if (display_tooltip) {
                    ImGui::BeginTooltip();
                    ImGui::Text("Mouse: (%.1f, %.1f)", mouse_pos.x, mouse_pos.y);
                    ImGui::Text("Closest Point #%d", closest_idx);
                    ImGui::Text("Position: (%.3f, %.3f, %.3f)", HGBC::cubic_centers_x[closest_idx], HGBC::cubic_centers_y[closest_idx], HGBC::cubic_centers_z[closest_idx]);
                    ImGui::Text("Position: (%.3f, %.3f, %.3f)", HGBC::cubic_centers_x[closest_idx], HGBC::cubic_centers_y[closest_idx], HGBC::cubic_centers_z[closest_idx]);
                    ImGui::Text("Distance: %.1f px", ImSqrt(min_dist_sq));
                    ImGui::EndTooltip();
                }

                cubic_grid_coord = HGBC::cubic_cell_coord_from_cartesian(HGBC::cubic_centers_x[closest_idx], HGBC::cubic_centers_y[closest_idx], HGBC::cubic_centers_z[closest_idx]);
                cubic_grid_index = closest_idx;
                return true;
            }
        }

        return false;
    }
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    void display_orientation_gizmo(ImDrawList* draw_list, ImVec2 mouse_pos, ImVec2 plot_pos, ImVec2 plot_size) {
        ImPlot3DContext& gp = *ImPlot3D::GetCurrentContext();
        ImPlot3DPlot* plot = gp.CurrentPlot;
        if (plot) {
            ImVec2 gizmo_center = ImVec2(plot_pos.x + plot_size.x - 50, plot_pos.y + plot_size.y - 50);
            float gizmo_size = 30.0f;

            // Get rotation quaternion
            ImPlot3DQuat rot = plot->Rotation;

            // Define axis directions in plot space
            ImPlot3DPoint axes[3] = {
                ImPlot3DPoint(1, 0, 0), // X-axis (red)
                ImPlot3DPoint(0, 1, 0), // Y-axis (green)
                ImPlot3DPoint(0, 0, 1)  // Z-axis (blue)
            };

            ImU32 colors[3] = {
                IM_COL32(200, 50, 50, 255), // Red
                IM_COL32(50, 200, 50, 255), // Green
                IM_COL32(50, 50, 200, 255)  // Blue
            };

            const char* labels[3] = { "X", "Y", "Z" };

            // Draw gizmo background circle
            draw_list->AddCircleFilled(gizmo_center, gizmo_size + 5, IM_COL32(0, 0, 0, 100));

            // Draw each axis
            for (int i = 0; i < 3; i++) {
                // Rotate axis by quaternion
                ImPlot3DPoint rotated = rot * axes[i];

                // Project to 2D gizmo space (simple orthographic projection)
                ImVec2 axis_end = ImVec2(gizmo_center.x + float(rotated.x) * gizmo_size,
                    gizmo_center.y - float(rotated.y) * gizmo_size // Flip Y for screen coords
                );

                // Draw line
                draw_list->AddLine(gizmo_center, axis_end, colors[i], 2.0f);

                // Draw circle at end
                draw_list->AddCircleFilled(axis_end, 4.0f, colors[i]);

                // Draw label
                ImVec2 label_pos = ImVec2(axis_end.x + 8, axis_end.y - 8);
                draw_list->AddText(label_pos, colors[i], labels[i]);
            }
        }
    }

    cubic_surface_index_data_type get_cubic_grid_index() {
        return cubic_grid_index;
    }

protected:
    // Main ImGui window widget or panel that displays and manages the widgets used as inputs 
    // for user interaction to query and edit the main cubicagonal grid data values and its display 
    void display_grid_parameters_panel() {
        ImGui::Begin("CUBIC Automata grid display parameters");
        {
            //###### interactively change main cubic grid display color of cubic grid cell locations ######
            ImGui::Text("Color : ");
            ImGui::SameLine();
            if (ImGui::ColorEdit4("##cugc", (float*)&cubic_grid_display_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_None)) {
                HGBC::cubic_grid_display_color = cubic_grid_display_color;
            }
            //ImGui::ColorEdit4("##cugc", (float*)HGBC::cubic_grid_display_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_None); //This generates compilor error
            ImGui::SameLine();
            // interactively change main cubic grid display shape of cubic grid cell locations
            ImGui::Text("Shape : ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(60);
            const char* combo_preview_value = shapes[cubic_grid_display_shape_id];
            if (ImGui::BeginCombo("##cugsh", combo_preview_value, shapes_combo_flags))
            {
                for (int n = 0; n < IM_COUNTOF(shapes); n++)
                {
                    const bool is_selected = (cubic_grid_display_shape_id == n);
                    if (ImGui::Selectable(shapes[n], is_selected)) {
                        cubic_grid_display_shape_id = n;
                        define_imgui_shape();
                    }

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected) { ImGui::SetItemDefaultFocus(); }
                }
                ImGui::EndCombo();
            }
            ImGui::SameLine();
            // interactively change main cubic grid display shape size of cubic grid cell locations
            ImGui::Text("Size : ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(60);
            ImGui::DragFloat("##cugsi", &cubic_grid_display_shape_size, 0.1f, 1.0f, 50.0f, "%2.2f");
            ImGui::SameLine();
            // interactively change main cubic grid display of cubic grid cell locations on or off
            ImGui::Text("Display : ");
            ImGui::SameLine();
            ImGui::Checkbox("##cugad", &display_acubic_grid);
            ImGui::SameLine();
            // interactively change main cubic grid display of cubic grid cell locations on or off
            ImGui::Text("Auto Fit : ");
            ImGui::SameLine();
            ImGui::Checkbox("##cugaf", &autofit_acubic_grid);
            ImGui::Separator();


            // --------------------------------------------------------

            // interactively change the range of main cubic grid cells displayed
            //ImGui::Text("CUBIC Grid Extents Displayed");
            //ImGui::SameLine();
            // interactively change main cubic grid display of cubic grid cell locations on or off
            ImGui::Text("Use CUBIC Grid Extents : ");
            ImGui::SameLine();
            ImGui::Checkbox("##cuuge", &use_grid_extents);
            ImGui::SameLine();
            ImGui::Text("Beware This has zoom problems");

            // interactively change x axis display limits
            ImGui::Text("Min X: ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(60);
            //if (ImGui::SliderFloat("Min X##cugmnx", &plot_min_x, -1.0f, 1000.0f, "%3.1f")) {
            if (ImGui::DragFloat("##cugmnx", &plot_min_x, 1.0f, -1.0f, 1000.0f, "%3.1f")) {
                plot_axis_limit_changed = IMGUI_WINDOW_BORDER_LEFT;
            }
            ImGui::SameLine();
            ImGui::Text("Max X: ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(60);
            //if (ImGui::SliderFloat("Max X##cugmxx", &plot_max_x, -1.0f, 1000.0f, "%3.1f")) {
            if (ImGui::DragFloat("##cugmxx", &plot_max_x, 1.0f, -1.0f, 1000.0f, "%3.1f")) {
                plot_axis_limit_changed = IMGUI_WINDOW_BORDER_RIGHT;
            }
            ImGui::SameLine();
            // interactively change y axis display limits
            ImGui::Text("Min Y: ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(60);
            // if (ImGui::SliderFloat("Min Y##cugmny", &plot_min_y, -1.0f, 1000.0f, "%3.1f")) {
            if (ImGui::DragFloat("##cugmny", &plot_min_y, 1.0f, -1.0f, 1000.0f, "%3.1f")) {
                plot_axis_limit_changed = IMGUI_WINDOW_BORDER_LEFT;
            }
            ImGui::SameLine();
            ImGui::Text("Max Y: ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(60);
            //if (ImGui::SliderFloat("Max Y##cugmxy", &plot_max_y, -1.0f, 1000.0f, "%3.1f")) {
            if (ImGui::DragFloat("##cugmxy", &plot_max_y, 1.0f, -1.0f, 1000.0f, "%3.1f")) {
                plot_axis_limit_changed = IMGUI_WINDOW_BORDER_RIGHT;
            }
            ImGui::SameLine();
            // interactively change y axis display limits
            ImGui::Text("Min Z: ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(60);
            // if (ImGui::SliderFloat("Min Y##cugmny", &plot_min_y, -1.0f, 1000.0f, "%3.1f")) {
            if (ImGui::DragFloat("##cugmnz", &plot_min_z, 1.0f, -1.0f, 1000.0f, "%3.1f")) {
                plot_axis_limit_changed = IMGUI_WINDOW_BORDER_LEFT;
            }
            ImGui::SameLine();
            ImGui::Text("Max Z: ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(60);
            //if (ImGui::SliderFloat("Max Y##cugmxy", &plot_max_y, -1.0f, 1000.0f, "%3.1f")) {
            if (ImGui::DragFloat("##cugmxz", &plot_max_z, 1.0f, -1.0f, 1000.0f, "%3.1f")) {
                plot_axis_limit_changed = IMGUI_WINDOW_BORDER_RIGHT;
            }

            ImGui::Separator();
            // ################# CUBIC matrix grid orientation setting #####################
            // Inputs to define gradient min max values to be represented as the min max color markers
            ImGui::Text("         Matrix grid orientation view");
            if (ImGui::Button("X-Y", { 50,20 })) {
                elevation   = 90.0f;
                azimuth     = 0.0f;
                change_view = true;
            }
            ImGui::SameLine();
            if (ImGui::Button("X-Z", { 50,20 })) {
                elevation   = 0.0f;
                azimuth     = 0.0f;
                change_view = true;
            }
            ImGui::SameLine();
            if (ImGui::Button("Y-Z", { 50,20 })) {
                elevation   = 0.0f;
                azimuth     = -90.0f;
                change_view = true;
            }
            ImGui::SameLine();
            ImGui::SetNextItemWidth(200);
            if (ImGui::SliderFloat("Elevation", &elevation, -90.0f, 90.0f, "%.1f degrees")) { change_view = true; }
            ImGui::SameLine();
            ImGui::SetNextItemWidth(200);
            if (ImGui::SliderFloat("Azimuth", &azimuth, -180.0f, 180.0f, "%.1f degrees")) { change_view = true; }


            ImGui::Separator();

            // ################# CUBIC Grid Value Display Color Gradient #####################
            // Inputs to define gradient min max values to be represented as the min max color markers
            ImGui::Text("         Grid Value display Gradient limits");
            ImGui::Text("           Min                       Max");
            ImGui::SetNextItemWidth(200);
            ImGui::InputInt("##cugmngv", &state.min_grad_value);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(200);
            ImGui::InputInt("##cugmxgv", &state.max_grad_value);// Had a crash from this point after pressing return !!!!!!

            // Display the current value and color that the mouse cursor is hovering above the gradient display
            ImGui::Text("Inquired Gradient Value : %4.3f ", state.current_gradient_value);
            ImGui::Text("Inquired Gradient color : %4.2f | %4.2f | %4.2f | %4.2f  ", state.current_gradient_color[0], state.current_gradient_color[1], state.current_gradient_color[2], state.current_gradient_color[3]);

            ImGradientHDR(stateID, state, tempState, isMarkerShown);// Gradient display widget that changes values of the state, tempState, isMarkerShown as the user interacts with this widget

            // Tests to perform actions on the gradient and gradient display widget to manage gradient marker creation, deletion and modification of position and color
            if (tempState.selectedMarkerType == ImGradientHDRMarkerType::Color)
            {
                selectedColorMarker = state.GetColorMarker(tempState.selectedIndex);
                if (selectedColorMarker != nullptr) {
                    cm = selectedColorMarker;
                }
            }

            if (cm != nullptr) {
                ImGui::ColorEdit3("Color", (float*)cm->Color.data(), ImGuiColorEditFlags_Float);
                ImGui::DragFloat("Intensity", &cm->Intensity, 0.01f, 0.0f, 1.0f, "%1.3f", 1.0f);
            }

            if (tempState.selectedMarkerType != ImGradientHDRMarkerType::Unknown) {
                if (ImGui::Button("Delete Gradient Marker")) {
                    if (tempState.selectedMarkerType == ImGradientHDRMarkerType::Color) {
                        state.RemoveColorMarker(tempState.selectedIndex);
                        tempState = ImGradientHDRTemporaryState{};
                    }
                    else {
                        if (tempState.selectedMarkerType == ImGradientHDRMarkerType::Alpha) {
                            state.RemoveAlphaMarker(tempState.selectedIndex);
                            tempState = ImGradientHDRTemporaryState{};
                        }
                    }
                }
            }

            // Widget buttons to save and load gradient data 
            ImGui::SetCursorPosX(100);
            if (ImGui::Button("Save Gradient")) {
                save_gradienthdr_data();
            }

            ImGui::SameLine();
            ImGui::SetCursorPosX(250);
            if (ImGui::Button("Load Gradient")) {
                load_gradienthdr();
            }

            // Widget button to update main cubic grid color data to that of the color gradient
            // after changes or loading of gradient file
            ImGui::SetCursorPosX(140);
            if (ImGui::Button("Update CUBIC Grid Colors")) {
                update_cubic_grid_colors();
            }

            // Widget to activate/deactivate the cubic edit window flag variable
            // and display or not display the panel to edit cubic grid cell data values
            ImGui::Separator();
            ImGui::Text("Edit Cubic grid: ");
            ImGui::SameLine();
            ImGui::Checkbox("##cuged", &edit_acubic_grid);

        }
        ImGui::End();
    }

    // ImGui window widget or panel that displays and manages the widgets used as inputs 
    // for user interaction to query and edit the main cubicagonal grid cell data values
    // Only active and displayed if user has selected the edit cubic grid checkbox widget into active mode
    void display_grid_edit_panel(){
        ImGui::Begin("Edit Cubic Automata Grid"); {

            // ###### This section of code is relevant to the main cubicagonal Automata grid        ###########
            // ###### display window and if the mouse cusor is within it to perform editing tasks ###########
            // Test if the mouse cursor is within the bounds of the main cubic grid display window and if it is
            // test for any valid user interactions with the cubic grid data and perform tasks according to what
            // keyboard or mouse interaction are initiated.
            ImGuiContext* cec = ImGui::GetCurrentContext();// Get current Imgui context of what is happening 
            bool edit_mode = true; // default mode of interaction

            if (cec && cec->HoveredWindow) {// Have mouse cursor hovering within an ImGui window
                if (cec->HoveredWindow->ID == plot_window->ID) { // If the mouse cursor is hovering over the window that the cubic grid is being displayed in
                    if (ImGui::GetIO().KeyShift) { // Enable plot inputs and window interaction when shift key is pressed in edit mode and disable edit functions
                        plot_flags = ImPlot3DFlags_Equal | ImPlot3DFlags_NoMenus | ImPlot3DFlags_NoLegend;//not included| ImPlotFlags_Crosshairs
                        plot_window_flags = ImGuiWindowFlags_None;
                        edit_mode = false;
                    }
                    else {// disable plot inputs and window interaction when shift key not pressed in edit mode and enable edit functions
                        plot_flags = ImPlot3DFlags_Equal | ImPlot3DFlags_NoMenus  | ImPlot3DFlags_NoLegend | ImPlot3DFlags_NoInputs;//not included| ImPlotFlags_Crosshairs
                        plot_window_flags = ImGuiWindowFlags_NoMove;
                    }

//printf("ImGui::GetCurrentContext().CurrentWindow.ID!= ImGui::GetCurrentContext().CurrentWindow.GetID(plot_window_id.c_str())\n");
//printf("current context %i : %i\n", ImGui::GetCurrentContext()->HoveredWindow->ID, plot_window->ID);

                    // If mouse cursor is within the bounds of the cubic grid display then can perform cubic grid edit functions
                    if (got_nearest_cubic_grid_cell) {
                        //printf("got_nearest_cubic_grid_cell\n");
                      //}

                    //if (mouse_plot_pos.x > plot_min_x && mouse_plot_pos.x < plot_max_x && mouse_plot_pos.y > plot_min_y && mouse_plot_pos.y < plot_max_y) {
                        // Mouse cursor position is given in Cartesian coordinates of the cubic grid ImPlot being displayed, so need to convert mouse Cartesian
                        // coordinates to cubic grid index and cubic grid x-y index coordinates to be able to edit the cubic grid cell data values and retrieve them for display
                        //cubic_grid_index = HGBC::index_of_cubic_cell_with_cartesian_coord(mouse_plot_pos.x, mouse_plot_pos.y);
                        //cubic_grid_coord = HGBC::get_matrix_coordinate(cubic_grid_index);

                        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {// Left mouse click interaction
//printf("cubic_grid_class:display_grid_edit_panel: edit_grid::mouse button left : %i\n", cubic_grid_index);
                            if(cubic_grid_index > -1 && cubic_grid_index < HGBC::cubic_grid.size()){
                                if (use_cubic_grid_brush && cubic_grid_brush.cubic_grid_brush_selection.second) { // Using a cubic grid brush to edit cubic grid cell data values and have a valid cubic grid brush selected
//printf("cubic_grid_class:display_grid_edit_panel brush 0000 : %i\n", cubic_grid_index);

                                   // cubic_grid_brush_edit();
                                    cubic_grid_class<T> *edit_brush_grid = cubic_grid_brush.cubic_grid_brush_selection.second;// Get the current selected cubic grid brush data
                                    if (!edit_brush_grid) { // Test have a valid cubic grid brush selected
                                        return;
                                    }
//printf("cubic_grid_class:display_grid_edit_panel brush 1111 : %i\n", cubic_grid_index);
                                    // Define cubic brush center in x-y index coordinate and calculate cubic brush grid extents from that center location
                                    cubic_surface_vec_data_type brush_dim = edit_brush_grid->grid_dimension;
                                    cubic_surface_vec_data_type brush_center = { 0, 0, 0 };
//printf("cubic_grid_class:display_grid_edit_panel brush 111AAA : %i : %i : %i \n", brush_dim.x, brush_dim.y, brush_dim.z);

                                    // Define Brush center and +,- extents
                                    if (brush_dim.y % 2 == 1) {// odd row y center
                                        brush_center.y    = (int)(brush_dim.y / 2);
                                    } else{// even row y center
                                        brush_center.y    = (int)(brush_dim.y / 2)-1;
                                    }

                                    if ((brush_dim.x) % 2 == 1) { // odd column center
                                        brush_center.x = (int)(brush_dim.x / 2);
                                    } else {
                                        brush_center.x = (int)(brush_dim.x / 2) - 1;
                                    }

                                    if ((brush_dim.z) % 2 == 1) { // odd level center
                                        brush_center.z = (int)(brush_dim.z / 2);
                                    }
                                    else {
                                        brush_center.z = (int)(brush_dim.z / 2) - 1;
                                    }

                                    cubic_surface_vec_data_type cubic_grid_brush_origin = { cubic_grid_coord.x - brush_center.x ,cubic_grid_coord.y - brush_center.y,cubic_grid_coord.z - brush_center.z }; // main cubic grid x-y index coordinate where cubic grid brush (0,0) origin index lies
//printf("cubic_grid_class:display_grid_edit_panel brush 2222 : %i : %i : %i origin %i : %i : %i\n", brush_center.x, brush_center.y, brush_center.z, cubic_grid_brush_origin.x, cubic_grid_brush_origin.y, cubic_grid_brush_origin.z);
                                    // Iterate through each cubic brush grid x-y-z index coordinate and corresponding overlapped main cubic grid x-y-z index coordinate
                                    // and perform the edit the main cubic grid cell data value and colors function
 
                                    for (int b_z_coord = 0; b_z_coord < brush_dim.z; b_z_coord++) {// current Brush level to modify cubic matrix
                                        cubic_surface_index_data_type z_coord = cubic_grid_brush_origin.z + b_z_coord; // cubic matrix z coordinate that curent brush z corrdinate is upon
                                        if (z_coord >= 0 && z_coord < HGBC::grid_dimension.z) { // Brush cell is within cubic grid matrix z bounds
                                            for (int b_y_coord = 0; b_y_coord < brush_dim.y; b_y_coord++) {
                                                cubic_surface_index_data_type y_coord = cubic_grid_brush_origin.y + b_y_coord; //cubic matrix y coordinate that curent brush y corrdinate is upon
                                                if (y_coord >= 0 && y_coord < HGBC::grid_dimension.y){// Brush cell is within cubic grid matrix y bounds
                                                    for (int b_x_coord = 0; b_x_coord < brush_dim.x; b_x_coord++) {
                                                        cubic_surface_index_data_type x_coord = cubic_grid_brush_origin.x + b_x_coord; //cubic matrix x coordinate that curent brush x corrdinate is upon
                                                        if (x_coord >= 0 && x_coord < HGBC::grid_dimension.x) {// Brush cell is within cubic grid matrix x bounds
                                                            draw_cubic_grid_brush(edit_brush_grid, x_coord, y_coord, z_coord, b_x_coord, b_y_coord, b_z_coord);// Edit main cubic grid cell data and color value according to cubic brush cell data value
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                                else {
//printf("cubic_grid_class::display_grid_edit_panel: brush 3333  mouse button left :index %i | value %i \n", cubic_grid_index, HGBC::cubic_grid[cubic_grid_index]);
                                    update_cubic_grid_brush_colors(cubic_grid_index);// Edit the hovered main cubic grid cell data and color value
                                } 
//printf("cubic_grid_class::display_grid_edit_panel: brush 4444 mouse button left :index %i | value %i \n", cubic_grid_index, HGBC::cubic_grid[cubic_grid_index]);
                            }
                        }

                       // if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
//printf("cubic_grid_class:display_grid_edit_panel::mouse button right : %i\n", cubic_grid_index);
                      //  }
                    }

                } else {// Enable plot inputs and window interaction when cubic plot edit window is not hovered over in edit mode and disable edit functions
                    plot_flags = ImPlot3DFlags_Equal | ImPlot3DFlags_NoMenus | ImPlot3DFlags_NoLegend;// | ImPlot3DFlags_Crosshairs not included
                    plot_window_flags = ImGuiWindowFlags_None;
                    edit_mode = false;
                }
 
            }

            // ##################################################################################################

            // ###### This section of code is relevant to the display of the cubicagonal Automata grid edit ###########
            // ###### widgets  to define the cubic grid edit inputs and perform cubic grid edit processes     ###########
            // Display the current cubic grid data and coordinate values that the mouse cursor is hovering over in the
            // main cubicagonal automata grid display window
            ImGui::Text("CUBIC Grid Coordinate ::   ");
            ImGui::SameLine();
            ImGui::Text("X : %i | ", cubic_grid_coord.x);
            ImGui::SameLine();
            ImGui::Text("Y : %i | ", cubic_grid_coord.y);
            ImGui::SameLine();
            ImGui::Text("Z : %i", cubic_grid_coord.z);

            ImGui::Text("CUBIC Grid Index : %i  ||", cubic_grid_index);
            ImGui::SameLine();
                
            if (cubic_grid_index > -1 && cubic_grid_index < HGBC::cubic_grid.size()) { 
                cubic_grid_value = HGBC::cubic_grid[cubic_grid_index]; 
            }

            ImGui::Text(" CUBIC Grid value : %i", cubic_grid_value); // Need to accomidate all data types of cubic grid here !!!!
            ImGui::Separator();

            display_random_cubic_generator(); // Display the random engine window widgets to create a cubic grid of random generated numbers

            ImGui::Separator();
            // Display ImGui widgets to enter value to edit/change hovered cubic grid cell to either as an individual cubic grid cell or with a cubic grid brush
            ImGui::Text("Edit CUBIC Automata Grid Values");
            ImGui::Text("Value : ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(60);
            // interactively change result display shape size
            ImGui::DragFloat("##cugsi", &cubic_grid_edit_value, 0.1f, 1.0f, 50.0f, "%2.2f");  // Need to accomidate all data types of cubic grid here !!!!
            ImGui::SameLine();
            ImGui::Checkbox("Use CUBIC Grid Brush", &use_cubic_grid_brush);// flag to use a cubic grid brush to perform edits

            if (use_cubic_grid_brush) {// If use cubic grid brush
                cubic_grid_brush.display_cubic_grid_brush_panel();// display cubic grid brush window widget panel
            }

            // Widget inputs to create a border of a given value at the edge of the current cubic grid
            ImGui::Separator();
            ImGui::Text("CUBIC Grid Border");
            ImGui::Text("Border Value: ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(60);
            ImGui::DragFloat("##cugbv", &cubic_grid_edit_border_value, 0.1f, 1.0f, 50.0f, "%2.2f");
            ImGui::SameLine();
            if (ImGui::Button("Add CUBIC Grid Border")) {
                add_cubic_grid_border();
            }
        }
        ImGui::End();
    }

    void  add_cubic_grid_border() {
        // NOT YET IMPLEMENTED :: NEEDS TO BE DEFINED !!!!!!!!!!!!!!!!!!!!
        // Bottom border : z = 0 
        for (size_t iy = 0; iy < HGBC::grid_dimension.y; iy++) {
            for (size_t ix = 0; ix < HGBC::grid_dimension.x; ix++) {
                cubic_surface_index_data_type index = HGBC::get_cubic_surface_matrix_data_index({ ix,iy,0 });
                HGBC::cubic_grid[index] = cubic_grid_edit_border_value;
            }
        }

        // top border : z = 0 grid_dimension.z-1
        for (size_t iy = 0; iy < HGBC::grid_dimension.y; iy++) {
            for (size_t ix = 0; ix < HGBC::grid_dimension.x; ix++) {
                cubic_surface_index_data_type index = HGBC::get_cubic_surface_matrix_data_index({ ix,iy,HGBC::grid_dimension.z-1});
                HGBC::cubic_grid[index] = cubic_grid_edit_border_value;
            }
        }

       // left border : x = 0
        for (size_t iz = 0; iz < HGBC::grid_dimension.z; iz++) {
            for (size_t iy = 0; iy < HGBC::grid_dimension.y; iy++) {
                cubic_surface_index_data_type index = HGBC::get_cubic_surface_matrix_data_index({ 0,iy,iz });
                HGBC::cubic_grid[index] = cubic_grid_edit_border_value;
            }
        }

        // right border : x = grid_dimension.x - 1
        for (size_t iz = 0; iz < HGBC::grid_dimension.z; iz++) {
            for (size_t iy = 0; iy < HGBC::grid_dimension.y; iy++) {
                cubic_surface_index_data_type index = HGBC::get_cubic_surface_matrix_data_index({ HGBC::grid_dimension.x - 1,iy,iz });
                HGBC::cubic_grid[index] = cubic_grid_edit_border_value;
            }
        }

        // front border : y = 0
        for (size_t iz = 0; iz < HGBC::grid_dimension.z; iz++) {
            for (size_t ix = 0; ix < HGBC::grid_dimension.x; ix++) {
                cubic_surface_index_data_type index = HGBC::get_cubic_surface_matrix_data_index({ ix,0,iz });
                HGBC::cubic_grid[index] = cubic_grid_edit_border_value;
            }
        }

        // rear border : y = grid_dimension.y - 1
        for (size_t iz = 0; iz < HGBC::grid_dimension.z; iz++) {
            for (size_t ix = 0; ix < HGBC::grid_dimension.x; ix++) {
                cubic_surface_index_data_type index = HGBC::get_cubic_surface_matrix_data_index({ ix,HGBC::grid_dimension.y - 1,iz });
                HGBC::cubic_grid[index] = cubic_grid_edit_border_value;
            }
        }
    }

    void save_gradienthdr_data() {
//printf("cubic_grid:save_gradienthdr_data000\n");
        char const* patterns[] = { "*.ghd" };
        //char const* file_pathname = vwDialogs::save_file(nullptr, patterns, 1);
        char const* file_pathname = vwDialogs::save_file(gradient_dir_path.c_str(), patterns, 1);
//printf("cubic_grid:save_gradienthdr_data1111\n");
        if (file_pathname == nullptr) {
            afw_globalc::get_current_logger()->log(LogLevel::INFO, "ERROR : No gradient file defined to save gradient data to\n");
            return;
        }
//printf("cubic_grid:save_gradienthdr_data222\n");
        export_gradient(file_pathname, state);
    }

    void load_gradienthdr() {
//printf("cubic_grid:load_gradienthdr_data000\n");
        char const* patterns[] = { "*.ghd" };
        char const* file_pathname = vwDialogs::open_file(gradient_dir_path.c_str(), patterns, 1);
//printf("cubic_grid:load_gradienthdr_data111\n");
        if (file_pathname == nullptr) {
            afw_globalc::get_current_logger()->log(LogLevel::INFO, "ERROR : No gradient file defined to import gradient data to\n");
            return;
        }
//printf("cubic_grid:load_gradienthdr_data222\n");
        import_gradient(file_pathname,state);
//printf("cubic_grid:load_gradienthdr_data333\n");
        update_cubic_grid_colors();
    }

    // Display the random number generator and distribution ImGui widgets to select the random number generator
    // and number ditribution function and enter the parameter data required to generate a random number and
    // generate the random number.
    void display_random_cubic_generator(){
        random_engine.display_generation_inputs();

        ImGui::SetCursorPosX(100);
        if (ImGui::Button("Generate Random CUBIC Grid")) {// button widget to activate the creation of a random number
printf("cubic_grid : display_random_cubic_generator 0000 \n");
            random_engine.define_random_engine(); // Define the random engine input data to generate a randome number

printf("cubic_grid : display_random_cubic_generator 1111 : %i \n", random_engine.random_distribution_index);
            generate_random_cubic_grid();
            update_cubic_grid_colors();
        }

    }

    // CRITICAL : The random_distribution_index assignment must match the index of the random_distributions array list that
    //            the user has selected in the ImGui combo widget of random number distributions.
    // Generate a random number for every cubic grid cell based upon the selections made in the random engine GUI
    // One function exists for each possible number distribution type.
    void generate_random_cubic_grid() {
         switch (random_engine.random_distribution_index) {
            case 0:  { uniform_int_distribution_cubic_grid_random_generation(); break; }
            case 1:  { uniform_real_distribution_cubic_grid_random_generation(); break; }
            case 2:  { bernoulli_distribution_cubic_grid_random_generation(); break; }
            case 3:  { binomial_distribution_cubic_grid_random_generation(); break; }
            case 4:  { geometric_distribution_cubic_grid_random_generation(); break; }
            case 5:  { negative_binomial_distribution_cubic_grid_random_generation(); break; }
            case 6:  { poisson_distribution_cubic_grid_random_generation(); break; }
            case 7:  { exponential_distribution_cubic_grid_random_generation(); break; }
            case 8:  { gamma_distribution_cubic_grid_random_generation(); break; }
            case 9:  { weibull_distribution_cubic_grid_random_generation(); break; }
            case 10: { extreme_value_distribution_cubic_grid_random_generation(); break; }
            case 11: { normal_distribution_cubic_grid_random_generation(); break; }
            case 12: { lognormal_distribution_cubic_grid_random_generation(); break; }
            case 13: { chi_squared_distribution_cubic_grid_random_generation(); break; }
            case 14: { cauchy_distribution_cubic_grid_random_generation(); break; }
            case 15: { fisher_f_distribution_cubic_grid_random_generation(); break; }
            case 16: { student_t_distribution_cubic_grid_random_generation(); break; }
        }
    }


    // ##################################################################
    // ## Functions to generate a random number for each cubic grid cell ##
    // ## One function is defined for each number ditribution type     ##

    // NOTE : The
    // if (random_engine.rng_add_to_existing_grid) {
    //      rn = HGBC::cubic_grid[i] + rn;
    // }
    // to add random distribution numbers to the existing grid may not
    // be appropriate for all random distributons and has only been tested
    // for the bernoulli distribution. Use With Caution.

    void uniform_int_distribution_cubic_grid_random_generation() {
        for (size_t i = 0; i < HGBC::cubic_grid.size(); i++) {
            random_engine.uniform_int_distribution.generate_random_number();

            T rn = (T) random_engine.uniform_int_distribution.generated_number_value;
            
            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::cubic_grid[i] + rn;
            }

            HGBC::cubic_grid[i] = rn;
        }
    }

    void uniform_real_distribution_cubic_grid_random_generation(){
        for (size_t i = 0; i < HGBC::cubic_grid.size(); i++) {
            random_engine.uniform_real_distribution.generate_random_number();

            T rn = (T)random_engine.uniform_real_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::cubic_grid[i] + rn;
            }

            HGBC::cubic_grid[i] = rn;
        }
    }
    void bernoulli_distribution_cubic_grid_random_generation(){
        for (size_t i = 0; i < HGBC::cubic_grid.size(); i++) {
            random_engine.bernoulli_distribution.generate_random_number();

            T rn = (T)random_engine.bernoulli_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::cubic_grid[i] + rn;
            }

            HGBC::cubic_grid[i] = rn;
//printf("cubic_grid : bernoulli_distribution_cubic_grid_random_generation : %i \n", HGBC::cubic_grid[i]);
        }
    }
    void binomial_distribution_cubic_grid_random_generation() {
        for (size_t i = 0; i < HGBC::cubic_grid.size(); i++) {
            random_engine.binomial_distribution.generate_random_number();

            T rn = (T)random_engine.binomial_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::cubic_grid[i] + rn;
            }

            HGBC::cubic_grid[i] = rn;
        }
    }
    void geometric_distribution_cubic_grid_random_generation(){
        for (size_t i = 0; i < HGBC::cubic_grid.size(); i++) {
            random_engine.geometric_distribution.generate_random_number();

            T rn = (T)random_engine.geometric_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::cubic_grid[i] + rn;
            }

            HGBC::cubic_grid[i] = rn;
        }
    }
    void negative_binomial_distribution_cubic_grid_random_generation(){
        for (size_t i = 0; i < HGBC::cubic_grid.size(); i++) {
            random_engine.negative_binomial_distribution.generate_random_number();

            T rn = (T)random_engine.negative_binomial_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::cubic_grid[i] + rn;
            }

            HGBC::cubic_grid[i] = rn;
        }
    }
    void poisson_distribution_cubic_grid_random_generation(){
        for (size_t i = 0; i < HGBC::cubic_grid.size(); i++) {
            random_engine.poisson_distribution.generate_random_number();

            T rn = (T)random_engine.poisson_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::cubic_grid[i] + rn;
            }

            HGBC::cubic_grid[i] = rn;
        }
    }
    void exponential_distribution_cubic_grid_random_generation(){
        for (size_t i = 0; i < HGBC::cubic_grid.size(); i++) {
            random_engine.exponential_distribution.generate_random_number();

            T rn = (T)random_engine.exponential_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::cubic_grid[i] + rn;
            }

            HGBC::cubic_grid[i] = rn;
        }
    }
    void gamma_distribution_cubic_grid_random_generation(){
        for (size_t i = 0; i < HGBC::cubic_grid.size(); i++) {
            random_engine.gamma_distribution.generate_random_number();

            T rn = (T)random_engine.gamma_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::cubic_grid[i] + rn;
            }

            HGBC::cubic_grid[i] = rn;
        }
    }
    void weibull_distribution_cubic_grid_random_generation(){
        for (size_t i = 0; i < HGBC::cubic_grid.size(); i++) {
            random_engine.weibull_distribution.generate_random_number();

            T rn = (T)random_engine.weibull_distribution.generated_number_value;
            
            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::cubic_grid[i] + rn;
            }

            HGBC::cubic_grid[i] = rn;
        }
    }
    void extreme_value_distribution_cubic_grid_random_generation(){
        for (size_t i = 0; i < HGBC::cubic_grid.size(); i++) {
            random_engine.extreme_value_distribution.generate_random_number();

            T rn = (T)random_engine.extreme_value_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::cubic_grid[i] + rn;
            }

            HGBC::cubic_grid[i] = rn;
        }
    }
    void normal_distribution_cubic_grid_random_generation(){
        for (size_t i = 0; i < HGBC::cubic_grid.size(); i++) {
            random_engine.normal_distribution.generate_random_number();

            T rn = (T)random_engine.normal_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::cubic_grid[i] + rn;
            }

            HGBC::cubic_grid[i] = rn;
        }
    }
    void lognormal_distribution_cubic_grid_random_generation(){
        for (size_t i = 0; i < HGBC::cubic_grid.size(); i++) {
            random_engine.lognormal_distribution.generate_random_number();

            T rn = (T)random_engine.lognormal_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::cubic_grid[i] + rn;
            }

            HGBC::cubic_grid[i] = rn;
        }
    }
    void chi_squared_distribution_cubic_grid_random_generation(){
        for (size_t i = 0; i < HGBC::cubic_grid.size(); i++) {
            random_engine.chi_squared_distribution.generate_random_number();

            T rn = (T)random_engine.chi_squared_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::cubic_grid[i] + rn;
            }

            HGBC::cubic_grid[i] = rn;
        }
    }
    void cauchy_distribution_cubic_grid_random_generation(){
        for (size_t i = 0; i < HGBC::cubic_grid.size(); i++) {
            random_engine.cauchy_distribution.generate_random_number();

            T rn = (T)random_engine.cauchy_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::cubic_grid[i] + rn;
            }

            HGBC::cubic_grid[i] = rn;
        }
    }
    void fisher_f_distribution_cubic_grid_random_generation(){
        for (size_t i = 0; i < HGBC::cubic_grid.size(); i++) {
            random_engine.fisher_f_distribution.generate_random_number();

            T rn = (T)random_engine.fisher_f_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::cubic_grid[i] + rn;
            }

            HGBC::cubic_grid[i] = rn;
        }
    }
    void student_t_distribution_cubic_grid_random_generation(){
        for (size_t i = 0; i < HGBC::cubic_grid.size(); i++) {
            random_engine.student_t_distribution.generate_random_number();

            T rn = (T)random_engine.student_t_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::cubic_grid[i] + rn;
            }

            HGBC::cubic_grid[i] = rn;
        }
    }

    // ImGui window widget to display the cubic automata grid cell data and overlying cubic_grid_cells_data_list list of 
    // cubic sub grid data using the ImPlot::ScatterPlot function. 
    void display_automata_rule_results(){
        ImGui::Begin("CUBIC3D Automata", nullptr, plot_window_flags);// If Have name of ImGui::Begin then have ability to minimise and dock 

        if (ImPlot3D::BeginPlot("CUBIC3D Automata Grid",ImVec2(-1, -1), plot_flags | ImPlot3DFlags_NoClip)) {
            ImPlot3DStyle& style = ImPlot3D::GetStyle();
            ImGuiContext *cpc = ImGui::GetCurrentContext();// Strange compilor does not complain about ImGuiContext without including imgui_internal.h
                                                           // But does complain about ImGuiContext not being defined when using cpc in the next lines.

            // Test and functions to maintain the display aspect ratio of the ImGui-ImPlot window
            // when performing windo resizing using the window borders. This is performed 
            // so as to avoid strange disparity of the display of cubic grid where the grid X-Y axis
            // are not displayed on a 1:1 basis.
            // NOTE : This does not apply using corner window resizing : Need to find flags that 
            //        indicate when widow is being resized via corner and implement this method. 

            if (cpc) {
                plot_window = cpc->CurrentWindow;

                border_resized_held = cpc->CurrentWindow->ResizeBorderHeld;
                if (border_resized_held != IMGUI_WINDOW_BORDER_NONE) {
                    if (border_resized == IMGUI_WINDOW_BORDER_NONE) {
                        set_plot_window_resize_initial(cpc, style);
                        border_resized = cpc->CurrentWindow->ResizeBorderHeld;
                    }
                }

                if (border_resized_held == IMGUI_WINDOW_BORDER_NONE && border_resized != IMGUI_WINDOW_BORDER_NONE) {
                    switch (border_resized) {
                        case IMGUI_WINDOW_BORDER_LEFT  :
                        case IMGUI_WINDOW_BORDER_RIGHT : {
                                                            resize_plot_window_width(cpc, style);
                                                            border_resized = IMGUI_WINDOW_BORDER_NONE;
                                                            break;
                                                         }
                        case IMGUI_WINDOW_BORDER_TOP    :
                        case IMGUI_WINDOW_BORDER_BOTTOM : {
                                                            resize_plot_window_height(cpc, style);
                                                            border_resized = IMGUI_WINDOW_BORDER_NONE;
                                                            break;
                                                         }
                    }
                }     
            }

if(plot_axis_limit_changed> IMGUI_WINDOW_BORDER_NONE) printf("Axis Extents  %i\n", plot_axis_limit_changed);

//if (cpc)
//    printf("display_cubic_grid : px %f : py %f: x %f : y %f \n", prev_width, prev_height,cpc->CurrentWindow->Size.x, cpc->CurrentWindow->Size.y);


//testing only delete/comment out when not needed
if(HGBC::cubic_centers_x.empty()){
    printf("cubic_grid_class::display_automata_rule_results:: ERROR \n", HGBC::cubic_centers_x.empty());
}

//printf("cubic_grid_class::display_automata_rule_results:: AAAAA %i\n", HGBC::cubic_centers_x.size());
            // ImPlot::ScatterPlot function to display main cubic grid cell data
            if (display_acubic_grid) {// If display cubic grid flag is true

                // Constrain the cubic grid display to a min max Cartesian coordinate range of coordinates
                // Problem using this is that when panning, unwanted zooming in occurs
                if (use_grid_extents) {
                    ImPlot3D::SetupAxisLimitsConstraints(ImAxis3D_X, plot_min_x, plot_max_x);
                    ImPlot3D::SetupAxisLimitsConstraints(ImAxis3D_Y, plot_min_y, plot_max_y);
                    ImPlot3D::SetupAxisLimitsConstraints(ImAxis3D_Z, plot_min_z, plot_max_z);
                } else {
                    // Define axis limits of the display box
                    ImPlot3D::SetupAxisLimits(ImAxis3D_X, plot_min_x, plot_max_x);
                    ImPlot3D::SetupAxisLimits(ImAxis3D_Y, plot_min_y, plot_max_y);
                    ImPlot3D::SetupAxisLimits(ImAxis3D_Z, plot_min_z, plot_max_z);
                }

                //ImPlot3D::SetupBoxScale(display_box_scale, display_box_scale, display_box_scale);

                // Set the rotation using the specified elevation and azimuth
                if (change_view) {
                    ImPlot3D::SetupBoxRotation(elevation, azimuth, false, ImPlot3DCond_Always);
                    change_view = false;
                }

                ImPlot3DAxisFlags x_axis_flags = axis_flags;
                ImPlot3DAxisFlags y_axis_flags = axis_flags;
                ImPlot3DAxisFlags z_axis_flags = axis_flags;

                if (autofit_acubic_grid) {
                    x_axis_flags |= ImPlot3DAxisFlags_AutoFit;
                    y_axis_flags |= ImPlot3DAxisFlags_AutoFit;
                    z_axis_flags |= ImPlot3DAxisFlags_AutoFit;
                }

                // !!!! Add more axis options here !!!!!
                //x_axis_flags |= ImPlot3DAxisFlags_NoDecorations | ImPlot3DAxisFlags_NoTickMarks;
                //y_axis_flags |= ImPlot3DAxisFlags_NoDecorations | ImPlot3DAxisFlags_NoTickMarks;
                //z_axis_flags |= ImPlot3DAxisFlags_NoDecorations | ImPlot3DAxisFlags_NoTickMarks;

                ImPlot3D::SetupAxes("###cux", "###cuy", "###cuz", x_axis_flags, y_axis_flags, z_axis_flags);

                ImPlot3D::PlotScatter("cubic", HGBC::cubic_centers_x.data(), HGBC::cubic_centers_y.data(), HGBC::cubic_centers_z.data(), HGBC::cubic_centers_x.size(), {
                    ImPlot3DProp_MarkerSize, cubic_grid_display_shape_size,
                    ImPlot3DProp_Marker, cubic_grid_display_shape,
                    ImPlot3DProp_FillAlpha,0.25f,
                    ImPlot3DProp_MarkerFillColors,HGBC::cubic_colors.data(),
                    ImPlot3DProp_MarkerLineColors, HGBC::cubic_colors.data()
                });
            }

            ImDrawList* draw_list = ImPlot3D::GetPlotDrawList();
            ImVec2 mouse_pos = ImGui::GetMousePos();
            ImVec2 plot_pos = ImPlot3D::GetPlotRectPos();
            ImVec2 plot_size = ImPlot3D::GetPlotRectSize();

            if(edit_acubic_grid){ got_nearest_cubic_grid_cell =  get_display_nearest_cubic_matrix_grid_cell_to_cursor(draw_list, mouse_pos, plot_pos, plot_size); }
            
            display_orientation_gizmo(draw_list, mouse_pos, plot_pos, plot_size);

            // Uncomment when ready to test and use
            //for (cubic_sub_grid_struct_type cubic_sub_grid : HGBC::cubic_sub_grids) {
            //    cubic_sub_grid.display_sub_cubic_grid();
            //}

            for (cubic_grid_cell_data_struct_type cubic_grid_cell : cubic_grid_cells_data_list) {
//printf("cubic_grid_class::display_cubic_grid 3333 :"); printf(" %i : %i\n", cubic_grid_cell.cubic_grid_cell_data_id, cubic_grid_cell.display_shape_id);
               cubic_grid_cell.display_cubic_cell_data();// This is the cubic grid automata rules display data
            }
     
            ImPlot3D::EndPlot();
        }
        plot_axis_limit_changed = IMGUI_WINDOW_BORDER_NONE;

        ImGui::End();
    }

private:
    ImGuiWindow* plot_window = nullptr;
    ImGuiWindowFlags plot_window_flags = ImGuiWindowFlags_None;
    ImPlot3DFlags plot_flags = ImPlot3DFlags_Equal | ImPlot3DFlags_NoMenus | ImPlot3DFlags_NoLegend;

    ImPlot3DPoint mouse_plot_pos = { 0.0f,0.0f,0.0f };
    cubic_surface_index_data_type cubic_grid_index = -1;
    cubic_surface_vec_data_type cubic_grid_coord = { 0,0,0 };
    T cubic_grid_value = 0;

    bool change_extents_popup = false;

    int cubic_grid_cell_data_id = -1;

    int tested = false;
    int tested2 = false;

    // Plot resize variables
    float plot_min_x = -1.0f;
    float plot_max_x = 50.0f;
    float plot_min_y = -1.0f;
    float plot_max_y = 50.0f;
    float plot_min_z = -1.0f;
    float plot_max_z = 50.0f;

    float prev_plot_min_x = -1.0f;
    float prev_plot_max_x = 50.0f;
    float prev_plot_min_y = -1.0f;
    float prev_plot_max_y = 50.0f;
    float prev_plot_min_z = -1.0f;
    float prev_plot_max_z = 50.0f;

    float plot_min_zoom = 1.0f;
    float plot_max_zoom = 40.0f;

    float prev_width  = 300.0f;
    float prev_height = 300.0f;

    float prev_style_width  = 0.0f;
    float prev_style_height = 0.0f;

    float new_width  = 0.0f;
    float new_height = 0.0f;

    float new_style_width = 0.0f;
    float new_style_height = 0.0f;

    signed char border_resized_held = -1;
    signed char border_resized      = -1;
    
    // CUBIC grid display paramters 
    bool display_acubic_grid               = true;
    bool edit_acubic_grid                  = false;

    const char* shapes[9]                = { "Circle", "Square", "Diamond", "Up", "Down", "Left", "Right", "Cross", "Asterix" };
    ImGuiComboFlags shapes_combo_flags   = ImGuiComboFlags_NoArrowButton;
    signed char plot_axis_limit_changed  = -1;
    ImVec4 cubic_grid_display_color        = HGBC::cubic_grid_display_color;
    int cubic_grid_display_shape_id        = 1;
    float cubic_grid_display_shape_size    = 6.0f;
    ImPlot3DMarker_ cubic_grid_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Circle;

    // CUBIC grid value display color gradient variables
    ImGradientHDRState::ColorMarker* cm = nullptr;
    ImGradientHDRState::ColorMarker* selectedColorMarker = nullptr;
    int32_t stateID = 10;
    ImGradientHDRState state;
    ImGradientHDRTemporaryState tempState;
    bool isMarkerShown = true;

    int min_grad_value_i = 0;
    int max_grad_value_i = 100;

    bool got_nearest_cubic_grid_cell = false;

    // --------------- Random number generator variables -----------------------------

    random_engine_class random_engine;

    // ---------------- Edit CUBIC Grid Variables ------------------------------
    float cubic_grid_edit_value = 1.0f;
    float cubic_grid_edit_border_value = 100.0f;

    // --------------- CUBIC Grid Brush variables ----------------
    bool use_cubic_grid_brush = false;
    cubic_grid_brush_class<T> cubic_grid_brush;


    // ------------------------- Display box parameters  ---------------------------------------
    bool autofit_acubic_grid = false;
    bool use_grid_extents  = true;

    ImPlot3DAxisFlags axis_flags = ImPlot3DAxisFlags_None;
    //float display_box_scale = 1.15f;

    bool  change_view = false;
    float elevation   = 45.0f;
    float azimuth     = -135.0f;
    // ------------------------------------------------------------------------------


    // ******************** Plot resize functions **********************************
    //void set_plot_window_resize_initial(ImGuiContext *cpc, ImPlotStyle &style) {
    void set_plot_window_resize_initial(ImGuiContext *cpc, ImPlot3DStyle &style) {
        prev_width = cpc->CurrentWindow->Size.x;
        prev_height = cpc->CurrentWindow->Size.y;

        prev_style_width  = style.PlotDefaultSize.x;
        prev_style_height = style.PlotDefaultSize.y;
    }

    void resize_plot_window_width(ImGuiContext *cpc, ImPlot3DStyle&style) {
        float resized_width = cpc->CurrentWindow->Size.x;
        float x_ratio = resized_width / prev_width;

        new_width = resized_width;
        new_height = prev_height * x_ratio;

        new_style_width = resized_width;
        new_style_height = prev_style_height * x_ratio;

        ImGui::SetWindowSize({ new_width,new_height });

        style.PlotDefaultSize.x = new_style_width;
        style.PlotDefaultSize.y = new_style_height;
    }

    void  resize_plot_window_height(ImGuiContext* cpc, ImPlot3DStyle& style) {
        float resized_height = cpc->CurrentWindow->Size.y;
        float y_ratio = resized_height / prev_height;

        new_width = (prev_width * y_ratio);
        new_height = resized_height;

        new_style_width  = prev_style_width * y_ratio;
        new_style_height = resized_height;

        ImGui::SetWindowSize({ new_width,new_height });

        style.PlotDefaultSize.x = new_style_width;
        style.PlotDefaultSize.y = new_style_height;
    }

    void define_imgui_shape() {// default shapes to display
        switch (cubic_grid_display_shape_id) {
            case 0:  cubic_grid_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Circle; break;
            case 1:  cubic_grid_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Square; break;
            case 2:  cubic_grid_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Diamond; break;
            case 3:  cubic_grid_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Up; break;
            case 4:  cubic_grid_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Down; break;
            case 5:  cubic_grid_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Left; break;
            case 6:  cubic_grid_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Right; break;
            case 7:  cubic_grid_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Cross; break;
            case 8:  cubic_grid_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Plus; break;
            case 9:  cubic_grid_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Asterisk; break;
            default: cubic_grid_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Circle;
        }
    }
};
