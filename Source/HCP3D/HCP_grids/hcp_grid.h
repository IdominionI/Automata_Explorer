#pragma once

#include <ThirdParty/ImGui/imgui_docking/imgui_internal.h>

#include <FrameWork/AFW/ImGUI/ImGuiEx/imgui_custom.h>
#include <FrameWork/AFW/ImGUI/ImGuiEx/imgui_widgets.h>
#include <FrameWork/AFW/Interface/IconsFontAwesome4.h>

#include "Source/Widgets/ImGradientHDR.h"
#include "Source/Application/random_engine.h"

#include "hcp_sub_grid_manager.h"
#include "hcp_grid_brush.h"
#include "hcp_surface_import_export.h"

// Central main hcpagonal automata grid class that defines, stores, creates, displays and manages all aspects
// of the hcpagonal automata grid data to be used in the application
// This class has incorporated classes or structures that are used to modify, display or define the hcp grid 
//   gradient
//   random engine
//   hcp grid brush
//   hcp surface import/export

template <class T>
class hcp_grid_class : public hcp_grid_base_class<T> , public hcp_sub_grid_manager_class{
	using HGBC = hcp_grid_base_class<T>; // Need to define this so as to have access to the hcp_grid_base_class class : Due to using template
public:
	hcp_grid_class() {
        initialise_gradient();
        //random_engine.initialise_distribution_input_parameters();
    }
	hcp_grid_class(T initial_hcp_value, hcp_surface_vec_data_type dimension) : hcp_grid_base_class<T>(initial_hcp_value, dimension){
        initialise_gradient();
        //random_engine.initialise_distribution_input_parameters();
    }
	~hcp_grid_class() {}

    std::string gradient_dir_path = "./Gradients"; // Path name to the default directory that the gradient class exists within

    //bool first = true;

    std::vector<hcp_grid_cell_data_struct_type> hcp_grid_cells_data_list = {};// vector list of hcp grid data to display as an overlay on main grid display

	void delete_hcp_grid_data(hcp_surface_index_data_type i) override {
		// integer value does not need to be deleted
	}

	void invalidate_hcp_grid_data_value(hcp_surface_index_data_type index) override {
        HGBC::hcp_grid.at(index) = INVALID_HCP_INT_DATA_VALUE;// Need to cater for float, double and boolean data types !!!!
	}

    // The central main core function that manages the display of the hcp grid window panels that
    // the user interacts with to display and edit hcp grid data values.
	bool display_hcp_grid() override {
        if (HGBC::hcp_centers_x.size() == 0) { return false; }
//printf("hcp_grid_class::display_hcp_grid 000\n");

        display_grid_parameters_panel();

//printf("hcp_grid_class::display_hcp_grid 1111\n");
        if (edit_ahcp_grid) {
            display_grid_edit_panel();
        } else{
            // Enable plot inputs and window interaction when not in edit mode and disable edit functions
            plot_flags = ImPlot3DFlags_Equal | ImPlot3DFlags_NoMenus | ImPlot3DFlags_NoLegend;// | ImPlotFlags_Crosshairs not included
            plot_window_flags = ImGuiWindowFlags_None;
        }
//printf("hcp_grid_class::display_hcp_grid 2222\n");

        display_automata_rule_results();// Display the current main hcpagonal grid data which the user can interact with

//printf("hcp_grid_class::display_hcp_grid 4444\n");
        return true;
	}

    // Default initialisation of hcp grid data value gradient 
    void initialise_gradient() {
        state.AddColorMarker(0.0f, { 1.0f, 1.0f, 1.0f }, 1.0f);
        state.AddColorMarker(1.0f, { 0.0f, 1.0f, 0.0f }, 1.0f);
        //state.AddAlphaMarker(0.0f, 1.0f);
        //state.AddAlphaMarker(1.0f, 1.0f);
    }

    int define_new_hcp_grid_cell_data() {
        hcp_grid_cell_data_id++;
        hcp_grid_cell_data_struct_type hcp_grid_cell_data;
        hcp_grid_cell_data.hcp_grid_cell_data_id = hcp_grid_cell_data_id;

        hcp_grid_cells_data_list.push_back(hcp_grid_cell_data);

        return hcp_grid_cell_data.hcp_grid_cell_data_id;
    }

    // Clear all the hcp grid cell data of the hcp_grid_cells_data_list to be reused
    void clear_all_hcp_grid_cell_data() {
        hcp_grid_cell_data_id = -1;
        for (hcp_grid_cell_data_struct_type &hcp_grid_cell : hcp_grid_cells_data_list) {
            hcp_grid_cell.clear_hcp_grid_cell_data();
        }
    }

    // Clear hcp grid cell data of the hcp_grid_cells_data_list of id number hcp_grid_cell_data_id
    bool clear_hcp_grid_cell_data(int  hcp_grid_cell_data_id) {
        for (size_t i = 0; i < hcp_grid_cells_data_list.size(); i++) {
            if (hcp_grid_cells_data_list[i].hcp_grid_cell_data_id == hcp_grid_cell_data_id) {
                hcp_grid_cells_data_list[i].clear_hcp_grid_cell_data();
                return true;
            }
        }

        return false;
    }

    // Delete all the hcp grid cell data of the hcp_grid_cells_data_list
    bool delete_hcp_grid_cell_data(int hcp_grid_cell_data_id) {
        for (size_t i = hcp_grid_cells_data_list.size()-1; i > -1;i--) {
            if (hcp_grid_cells_data_list[i].hcp_grid_cell_data_id == hcp_grid_cell_data_id) {
                hcp_grid_cells_data_list[i].clear_hcp_grid_cell_data();
                hcp_grid_cells_data_list.erase(hcp_grid_cells_data_list.begin() + i);
                return true;
            }
        }

        return false;
    }

    void delete_all_hcp_grid_cell_data() {
        clear_all_hcp_grid_cell_data();
        hcp_grid_cells_data_list.clear();
        hcp_grid_cells_data_list.shrink_to_fit();
    }

    // Get the hcp grid cell data entry within the hcp_grid_cells_data_list that has an ID value of hcp_grid_cell_data_id
    int get_hcp_grid_cell_data_index(int hcp_grid_cell_data_id) {
//printf("hcp_grid_class :: get_hcp_grid_cell_data_index 0000: %i : %i\n" , hcp_grid_cells_data_list.size(), hcp_grid_cell_data_id);
        for (size_t i = 0; i < hcp_grid_cells_data_list.size(); i++) {
//printf("hcp_grid_class :: get_hcp_grid_cell_data_index 1111: %i :  %i : %i\n",i , hcp_grid_cells_data_list[i].hcp_grid_cell_data_id, hcp_grid_cell_data_id);
            if (hcp_grid_cells_data_list[i].hcp_grid_cell_data_id == hcp_grid_cell_data_id) {
                return i;
            }
        }

        return -1;
    }

    // Add the hcp grid index of the main hcpagonal hcp grid hcp_grid_index to the hcp_grid_cells_data_list entry with index hcp_grid_cell_index
    bool add_hcp_grid_cell_index(int hcp_grid_cell_index, hcp_surface_index_data_type hcp_grid_index) {
        if (hcp_grid_cell_index < 0 || hcp_grid_cell_index >= hcp_grid_cells_data_list.size()) {
//printf("hcp_grid_class :: add_hcp_grid_cell_index %i : %i 000\n", hcp_grid_cell_index, hcp_grid_cells_data_list.size());
            return false;
        }
//printf("hcp_grid_class :: add_hcp_grid_cell_index %i : %i 1111\n", hcp_grid_cell_index, hcp_grid_cells_data_list.size());
        glm::vec3 hcp_cell_cart_coord = HGBC::get_hcp_surface_world_cartesian_coordinate(hcp_grid_index);
//printf("hcp_grid_class :: add_hcp_grid_cell_index %i : %i 2222\n", hcp_grid_cell_index, hcp_grid_cells_data_list.size());
        hcp_grid_cells_data_list[hcp_grid_cell_index].add_hcp_grid_cell(hcp_grid_index,hcp_cell_cart_coord.x, hcp_cell_cart_coord.y, hcp_cell_cart_coord.z);
//printf("hcp_grid_class :: add_hcp_grid_cell_index %i : %i 3333\n", hcp_grid_cell_index, hcp_grid_cells_data_list.size());
        return true;
    }

    // Add the hcp grid index of the main hcpagonal hcp grid hcp_grid_index to the hcp_grid_cells_data_list entry with a id number of  hcp_grid_cell_data_id
    bool add_hcp_grid_cell_id(int hcp_grid_cell_data_id, hcp_surface_index_data_type hcp_grid_index) {
        int hcp_grid_cell_index = get_hcp_grid_cell_data_index(hcp_grid_cell_data_id);
        if (hcp_grid_cell_index < 0) { return false; }

        return (add_hcp_grid_cell_index(hcp_grid_cell_index, hcp_grid_index));
    }

    bool define_hcp_grid_cell_coordinate() {

    }

    // Sub grid Test functions
    /*
    void logical_test_hcp_sub_grids() {
//printf("test_hcp_sub_grids::display 00000 \n");

        if (HGBC::hcp_sub_grids.size() > 1) {
// printf("test_hcp_sub_grids::display 11111 \n");           
            for (size_t i = 0; i < HGBC::hcp_sub_grids.size(); i++) {
                for (size_t j = 0; j < HGBC::hcp_sub_grids.size(); j++) {
                    if (i!=j) {
//printf("test_hcp_sub_grids::display 22222 %i : %i  \n",i,j);
                        hcp_sub_grid_struct_type hcp_sub_grid01 = HGBC::hcp_sub_grids[i];
                        hcp_sub_grid_struct_type hcp_sub_grid02 = HGBC::hcp_sub_grids[j];

                        if (hcp_sub_grids_overlap(hcp_sub_grid01, hcp_sub_grid02)) {
                            std::string info = "test_hcp_sub_grids :: HCP sub grid : " + std::to_string(hcp_sub_grid01.hcp_grid_range_id) + " overlaps with HCP sub grid : " + std::to_string(hcp_sub_grid02.hcp_grid_range_id) + "\n";
                            afw_globalc::get_current_logger()->log(LogLevel::INFO, info);
                        }

                        if (hcp_sub_grids_neighbours(hcp_sub_grid01, hcp_sub_grid02)) {
                            std::string info = "test_hcp_sub_grids :: HCP sub grid : " + std::to_string(hcp_sub_grid01.hcp_grid_range_id) + " is neighbours with HCP sub grid : " + std::to_string(hcp_sub_grid02.hcp_grid_range_id) + "\n";
                            afw_globalc::get_current_logger()->log(LogLevel::INFO, info);
                        }

                    }
                }
            }

        }
    }

    bool operation_testhcp_sub_grids() {
        if (!tested) {
            //if (HGBC::hcp_sub_grids.size() > 1) {
            //    hcp_sub_grid_struct_type merged_hcp_sub_grid;

            //    if (!merge_hcp_sub_grids(HGBC::hcp_sub_grids[0], HGBC::hcp_sub_grids[1], merged_hcp_sub_grid)) return false;

            //    HGBC::create_hcp_sub_grid(merged_hcp_sub_grid.lower_range, merged_hcp_sub_grid.upper_range);
            //    HGBC::define_sub_grid_plot_data(HGBC::hcp_sub_grids[HGBC::hcp_sub_grids.size() - 1]);
            //    tested = true;
            //}

            if (HGBC::hcp_sub_grids.size() > 0) {
                hcp_sub_grid_struct_type test_hcp_sub_grid1, test_hcp_sub_grid2;

                if (!hcp_sub_grid_partition_x(HGBC::hcp_sub_grids[0], { 10,10 }, test_hcp_sub_grid1)) { return false; }

                HGBC::define_sub_grid_plot_data(HGBC::hcp_sub_grids[0]);

                HGBC::create_hcp_sub_grid(test_hcp_sub_grid1.lower_range, test_hcp_sub_grid1.upper_range);
                HGBC::define_sub_grid_plot_data(HGBC::hcp_sub_grids[HGBC::hcp_sub_grids.size() - 1]);
               
                if (!hcp_sub_grid_partition_y(HGBC::hcp_sub_grids[0], { 10,10 }, test_hcp_sub_grid2)) { return false; }

                HGBC::define_sub_grid_plot_data(HGBC::hcp_sub_grids[0]);

                HGBC::create_hcp_sub_grid(test_hcp_sub_grid2.lower_range, test_hcp_sub_grid2.upper_range);
                HGBC::define_sub_grid_plot_data(HGBC::hcp_sub_grids[HGBC::hcp_sub_grids.size() - 1]);

                tested = true;
            }
        }

        return true;
    }
    */

    // Update the main hcpagonal grid color data with a value that corresponds to the color of the current hcp grid gradient display for that value
    // If the data value is out of range and beyond the bounds of the defied gradient, assign the min or max color value depending if the value is 
    // below or above the gradient min max display value
    void update_hcp_grid_colors() {

        for (size_t i = 0; i < HGBC::hcp_colors.size(); i++) {
            if ((float)HGBC::hcp_grid[i] < (float)state.min_grad_value || (float)HGBC::hcp_grid[i] > (float)state.max_grad_value) {
                ImU32 hcp_grid_edit_color = ImGui::GetColorU32(HGBC::hcp_grid_display_color);
                HGBC::hcp_colors[i] = hcp_grid_edit_color;
            }
            else {
                //float grid_value_pos = (float)HGBC::hcp_grid[i] / ((float)state.max_grad_value - (float)state.min_grad_value);
                float grid_value_pos = ((float)HGBC::hcp_grid[i] - (float)state.min_grad_value) / ((float)state.max_grad_value - (float)state.min_grad_value);
                std::array<float, 4> c = state.GetColorAndIntensity(grid_value_pos);
                ImU32 hcp_grid_edit_color = ImGui::GetColorU32({ c[0], c[1], c[2],c[3] });
                HGBC::hcp_colors[i] = hcp_grid_edit_color;
            }
        }
    }

    // Edit the main hcp grid data with the hcp grid brush data of edit_brush_grid
    // x_coord,y_coord is the x-y coordinate index of the main hcp grid that is to be referencd for editing
    // b_x_coord,b_y_coord  is the x-y index coordinate index of the hcp grid brush that is to be referencd for changing the main hcp grid to.
    // If hcp grid brush cell at x-y index coordinate  b_x_coord,b_y_coord has a value greater than 0, then the current main hcp grid at  x-y index coordinate  x_coord,y_coord is changed
    void draw_hcp_grid_brush(hcp_grid_class<T> *edit_brush_grid, hcp_surface_index_data_type x_coord, hcp_surface_index_data_type y_coord, hcp_surface_index_data_type z_coord, hcp_surface_index_data_type b_x_coord, hcp_surface_index_data_type b_y_coord, hcp_surface_index_data_type b_z_coord) {
// NOT YET IMPLEMENTED !!!!!!!!!!!!!
        hcp_surface_index_data_type grid_index  = HGBC::get_hcp_surface_matrix_data_index({ x_coord ,y_coord,z_coord });               // Get main hcp grid cell index that corresponds to the main hcp grid x-y index coordinate 
        hcp_surface_index_data_type brush_index = edit_brush_grid->get_hcp_surface_matrix_data_index({ b_x_coord ,b_y_coord,b_z_coord });// Get hcp brush grid cell index that corresponds to the hcp brush grid cell x-y index coordinate 
        if (grid_index > -1 && grid_index < HGBC::hcp_grid.size()) { // check that retrieved main hcp grid cell index is in the bounds of the current hcp grid array size
            if (brush_index > -1 && brush_index < edit_brush_grid->hcp_grid.size()) {// check that retrieved hcp grid brush cell index is in the bounds of the current hcp grid brush array size
//printf("hcp_grid_class::draw_hcp_grid_brush brush 11111 : brush value :%i\n", edit_brush_grid->hcp_grid[brush_index]);
                if (edit_brush_grid->hcp_grid[brush_index] > 0) {// If the hcp grid brush cell of index brush_index has a non zero value then edit main hcp grid cell data and color values
//printf("hcp_grid_class::draw_hcp_grid_brush00000  grid index: %i : brush index %i : \n", grid_index, brush_index);
//printf("hcp_grid_class::draw_hcp_grid_brush00000  grid coord: %i | %i | %i :: brush coord %i | %i | %i: \n", x_coord, y_coord, z_coord, b_x_coord, b_y_coord, b_z_coord);
                    update_hcp_grid_brush_colors(grid_index);
                }
            }
        }
    }

    // Change the current main hcp grid cell value with an array index grid_index to the current hcp_grid_edit_value
    // and update the color data with the color of the colour gradient that corresponds to the hcp_grid_edit_value
    void update_hcp_grid_brush_colors(hcp_surface_index_data_type grid_index) {
        HGBC::hcp_grid[grid_index]   = hcp_grid_edit_value; // Change hcp grid value
        float grid_value_pos         = hcp_grid_edit_value / ((float)state.max_grad_value - (float)state.min_grad_value); // get position in the gradient that corresponds to hcp_grid_edit_value
        std::array<float, 4> c       = state.GetColorAndIntensity(grid_value_pos);// get color data as a color array  from the gradient that is at position grid_value_pos in the gradient 
        ImU32 hcp_grid_edit_color    = ImGui::GetColorU32({ c[0], c[1], c[2],c[3] }); // convert color array data to an ImGui color U32 format
        HGBC::hcp_colors[grid_index] = hcp_grid_edit_color; // Update hcp grid cell color data 
    }

    // !!!!!!!!!!!!!!!!!!!!!!!! GET NEAREST 3D GRID MATRIX CELL TO MOUSE CURSOR !!!!!!!!!!!!!!!!!!!

    bool get_display_nearest_hcp_matrix_grid_cell_to_cursor(ImDrawList* draw_list, ImVec2 mouse_pos, ImVec2 plot_pos, ImVec2 plot_size, bool display_tooltip = false, bool display_line = true) {
        //void get_display_nearest_hcp_matrix_grid_cell_to_cursor(bool display_tooltip = false, bool display_line = true) {
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

            for (int i = 0; i < HGBC::hcp_centers_x.size(); i++) {
                ImVec2 point_px = ImPlot3D::PlotToPixels(HGBC::hcp_centers_x[i], HGBC::hcp_centers_y[i], HGBC::hcp_centers_z[i]);
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
                    ImGui::Text("Position: (%.3f, %.3f, %.3f)", HGBC::hcp_centers_x[closest_idx], HGBC::hcp_centers_y[closest_idx], HGBC::hcp_centers_z[closest_idx]);
                    ImGui::Text("Position: (%.3f, %.3f, %.3f)", HGBC::hcp_centers_x[closest_idx], HGBC::hcp_centers_y[closest_idx], HGBC::hcp_centers_z[closest_idx]);
                    ImGui::Text("Distance: %.1f px", ImSqrt(min_dist_sq));
                    ImGui::EndTooltip();
                }

                hcp_grid_coord = HGBC::hcp_cell_coord_from_cartesian(HGBC::hcp_centers_x[closest_idx], HGBC::hcp_centers_y[closest_idx], HGBC::hcp_centers_z[closest_idx]);
                hcp_grid_index = closest_idx;
                return true;
            }
        }

        return false;
    }
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    void display_orientation_gizmo(ImDrawList* draw_list, ImVec2 mouse_pos, ImVec2 plot_pos, ImVec2 plot_size) {
        //void display_orientation_gizmo() {
            //ImDrawList* draw_list = ImPlot3D::GetPlotDrawList();
            //ImVec2 mouse_pos = ImGui::GetMousePos();
            //ImVec2 plot_pos = ImPlot3D::GetPlotRectPos();
            //ImVec2 plot_size = ImPlot3D::GetPlotRectSize();

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

    hcp_surface_index_data_type get_hcp_grid_index() {
        return hcp_grid_index;
    }


//    bool test_hcp_grid_cell_list() {
////printf("hcp_grid_class :: test_hcp_grid_cell_list 00\n");
//
//        if (!tested2) {
//printf("hcp_grid_class :: test_hcp_grid_cell_list 11\n");
//            int id1 = define_new_hcp_grid_cell_data();
//            int id2 = define_new_hcp_grid_cell_data();
//
//            int index1 = get_hcp_grid_cell_data_index(id1);
//            int index2 = get_hcp_grid_cell_data_index(id2);
//
//printf("hcp_grid_class :: test_hcp_grid_cell_list 22 %i : %i : %i : %i : %i\n", id1, id2, index1, index2, hcp_grid_cells_data_list.size());
//            if (hcp_grid_cells_data_list.size() > 0) {
//
//                for (int i = 0; i < 10; i++) {
//                    add_hcp_grid_cell_index(index1, i);
//                    add_hcp_grid_cell_index(index2, i+200);
//                }
//            }
//
//            hcp_grid_cells_data_list[index1].hcp_grid_cells_display_color = { 1.0f,0.0f,0.0f,1.0f };
//            hcp_grid_cells_data_list[index2].hcp_grid_cells_display_color = { 0.0f,1.0f,0.0f,1.0f };
//
//            tested2 = true;
//
//            for (hcp_grid_cell_data_struct_type &hcp_grid_cells_data : hcp_grid_cells_data_list) {
//               hcp_grid_cells_data.display_data = true;
//            }
//        }
//
//        return true;
//    }

protected:
    // Main ImGui window widget or panel that displays and manages the widgets used as inputs 
    // for user interaction to query and edit the main hcpagonal grid data values and its display 
    void display_grid_parameters_panel() {
        ImGui::Begin("HCP Automata grid display parameters");
        {
            //###### interactively change main hcp grid display color of hcp grid cell locations ######
            ImGui::Text("Color : ");
            ImGui::SameLine();
            if (ImGui::ColorEdit4("##hcpgc", (float*)&hcp_grid_display_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_None)) {
                HGBC::hcp_grid_display_color = hcp_grid_display_color;
            }
            //ImGui::ColorEdit4("##hcpgc", (float*)HGBC::hcp_grid_display_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_None); //This generates compilor error
            ImGui::SameLine();
            // interactively change main hcp grid display shape of hcp grid cell locations
            ImGui::Text("Shape : ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(60);
            const char* combo_preview_value = shapes[hcp_grid_display_shape_id];
            if (ImGui::BeginCombo("##hcpgsh", combo_preview_value, shapes_combo_flags))
            {
                for (int n = 0; n < IM_COUNTOF(shapes); n++)
                {
                    const bool is_selected = (hcp_grid_display_shape_id == n);
                    if (ImGui::Selectable(shapes[n], is_selected)) {
                        hcp_grid_display_shape_id = n;
                        define_imgui_shape();
                    }

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected) { ImGui::SetItemDefaultFocus(); }
                }
                ImGui::EndCombo();
            }
            ImGui::SameLine();
            // interactively change main hcp grid display shape size of hcp grid cell locations
            ImGui::Text("Size : ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(60);
            ImGui::DragFloat("##hcpgsi", &hcp_grid_display_shape_size, 0.1f, 1.0f, 50.0f, "%2.2f");
            ImGui::SameLine();
            // interactively change main hcp grid display of hcp grid cell locations on or off
            ImGui::Text("Display : ");
            ImGui::SameLine();
            ImGui::Checkbox("##hcpgad", &display_ahcp_grid);
            ImGui::SameLine();
            // interactively change main hcp grid display of hcp grid cell locations on or off
            ImGui::Text("Auto Fit : ");
            ImGui::SameLine();
            ImGui::Checkbox("##hcpgaf", &autofit_ahcp_grid);
            ImGui::Separator();


            // --------------------------------------------------------

            // interactively change the range of main hcp grid cells displayed
            //ImGui::Text("HCP Grid Extents Displayed");
            //ImGui::SameLine();
            // interactively change main hcp grid display of hcp grid cell locations on or off
            ImGui::Text("Use HCP Grid Extents : ");
            ImGui::SameLine();
            ImGui::Checkbox("##hcpuge", &use_grid_extents);
            ImGui::SameLine();
            ImGui::Text("Beware This has zoom problems");

            // interactively change x axis display limits
            ImGui::Text("Min X: ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(60);
            //if (ImGui::SliderFloat("Min X##hcpgmnx", &plot_min_x, -1.0f, 1000.0f, "%3.1f")) {
            if (ImGui::DragFloat("##hcpgmnx", &plot_min_x, 1.0f, -1.0f, 1000.0f, "%3.1f")) {
                plot_axis_limit_changed = IMGUI_WINDOW_BORDER_LEFT;
            }
            ImGui::SameLine();
            ImGui::Text("Max X: ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(60);
            //if (ImGui::SliderFloat("Max X##hcpgmxx", &plot_max_x, -1.0f, 1000.0f, "%3.1f")) {
            if (ImGui::DragFloat("##hcpgmxx", &plot_max_x, 1.0f, -1.0f, 1000.0f, "%3.1f")) {
                plot_axis_limit_changed = IMGUI_WINDOW_BORDER_RIGHT;
            }
            ImGui::SameLine();
            // interactively change y axis display limits
            ImGui::Text("Min Y: ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(60);
            // if (ImGui::SliderFloat("Min Y##hcpgmny", &plot_min_y, -1.0f, 1000.0f, "%3.1f")) {
            if (ImGui::DragFloat("##hcpgmny", &plot_min_y, 1.0f, -1.0f, 1000.0f, "%3.1f")) {
                plot_axis_limit_changed = IMGUI_WINDOW_BORDER_LEFT;
            }
            ImGui::SameLine();
            ImGui::Text("Max Y: ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(60);
            //if (ImGui::SliderFloat("Max Y##hcpgmxy", &plot_max_y, -1.0f, 1000.0f, "%3.1f")) {
            if (ImGui::DragFloat("##hcpgmxy", &plot_max_y, 1.0f, -1.0f, 1000.0f, "%3.1f")) {
                plot_axis_limit_changed = IMGUI_WINDOW_BORDER_RIGHT;
            }
            ImGui::SameLine();
            // interactively change y axis display limits
            ImGui::Text("Min Z: ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(60);
            // if (ImGui::SliderFloat("Min Y##hcpgmny", &plot_min_y, -1.0f, 1000.0f, "%3.1f")) {
            if (ImGui::DragFloat("##hcpgmnz", &plot_min_z, 1.0f, -1.0f, 1000.0f, "%3.1f")) {
                plot_axis_limit_changed = IMGUI_WINDOW_BORDER_LEFT;
            }
            ImGui::SameLine();
            ImGui::Text("Max Z: ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(60);
            //if (ImGui::SliderFloat("Max Y##hcpgmxy", &plot_max_y, -1.0f, 1000.0f, "%3.1f")) {
            if (ImGui::DragFloat("##hcpgmxz", &plot_max_z, 1.0f, -1.0f, 1000.0f, "%3.1f")) {
                plot_axis_limit_changed = IMGUI_WINDOW_BORDER_RIGHT;
            }

            ImGui::Separator();
            // ################# HCP matrix grid orientation setting #####################
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

            // ################# HCP Grid Value Display Color Gradient #####################
            // Inputs to define gradient min max values to be represented as the min max color markers
            ImGui::Text("         Grid Value display Gradient limits");
            ImGui::Text("           Min                       Max");
            ImGui::SetNextItemWidth(200);
            ImGui::InputInt("##hcpgmngv", &state.min_grad_value);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(200);
            ImGui::InputInt("##hcpgmxgv", &state.max_grad_value);// Had a crash from this point after pressing return !!!!!!

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

            // Widget button to update main hcp grid color data to that of the color gradient
            // after changes or loading of gradient file
            ImGui::SetCursorPosX(140);
            if (ImGui::Button("Update HCP Grid Colors")) {
                update_hcp_grid_colors();
            }

            // Widget to activate/deactivate the hcp edit window flag variable
            // and display or not display the panel to edit hcp grid cell data values
            ImGui::Separator();
            ImGui::Text("Edit HCP grid: ");
            ImGui::SameLine();
            ImGui::Checkbox("##hcpged", &edit_ahcp_grid);

        }
        ImGui::End();
    }

    // ImGui window widget or panel that displays and manages the widgets used as inputs 
    // for user interaction to query and edit the main hcpagonal grid cell data values
    // Only active and displayed if user has selected the edit hcp grid checkbox widget into active mode
    void display_grid_edit_panel(){
        ImGui::Begin("Edit HCP Automata Grid"); {

            // ###### This section of code is relevant to the main hcpagonal Automata grid        ###########
            // ###### display window and if the mouse cusor is within it to perform editing tasks ###########
            // Test if the mouse cursor is within the bounds of the main hcp grid display window and if it is
            // test for any valid user interactions with the hcp grid data and perform tasks according to what
            // keyboard or mouse interaction are initiated.
            ImGuiContext* cec = ImGui::GetCurrentContext();// Get current Imgui context of what is happening 
            bool edit_mode = true; // default mode of interaction

            // ++++++++++++++++
            //ImDrawList* draw_list = ImPlot3D::GetPlotDrawList();
            //ImVec2 mouse_pos = ImGui::GetMousePos();
            //ImVec2 plot_pos = ImPlot3D::GetPlotRectPos();
            //ImVec2 plot_size = ImPlot3D::GetPlotRectSize();

            //get_display_nearest_hcp_matrix_grid_cell_to_cursor(draw_list, mouse_pos, plot_pos, plot_size);
            // ++++++++++++++++

            if (cec && cec->HoveredWindow) {// Have mouse cursor hovering within an ImGui window
                if (cec->HoveredWindow->ID == plot_window->ID) { // If the mouse cursor is hovering over the window that the hcp grid is being displayed in
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

                    // If mouse cursor is within the bounds of the hcp grid display then can perform hcp grid edit functions
                    if (got_nearest_hcp_grid_cell) {
                        //printf("got_nearest_hcp_grid_cell\n");
                      //}

                    //if (mouse_plot_pos.x > plot_min_x && mouse_plot_pos.x < plot_max_x && mouse_plot_pos.y > plot_min_y && mouse_plot_pos.y < plot_max_y) {
                        // Mouse cursor position is given in Cartesian coordinates of the hcp grid ImPlot being displayed, so need to convert mouse Cartesian
                        // coordinates to hcp grid index and hcp grid x-y index coordinates to be able to edit the hcp grid cell data values and retrieve them for display
                        //hcp_grid_index = HGBC::index_of_hcp_cell_with_cartesian_coord(mouse_plot_pos.x, mouse_plot_pos.y);
                        //hcp_grid_coord = HGBC::get_matrix_coordinate(hcp_grid_index);

                        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {// Left mouse click interaction
//printf("hcp_grid_class:display_grid_edit_panel: edit_grid::mouse button left : %i\n", hcp_grid_index);
                            if(hcp_grid_index > -1 && hcp_grid_index < HGBC::hcp_grid.size()){
                                if (use_hcp_grid_brush && hcp_grid_brush.hcp_grid_brush_selection.second) { // Using a hcp grid brush to edit hcp grid cell data values and have a valid hcp grid brush selected
//printf("hcp_grid_class:display_grid_edit_panel brush 0000 : %i\n", hcp_grid_index);

                                   // hcp_grid_brush_edit();
                                    hcp_grid_class<T> *edit_brush_grid = hcp_grid_brush.hcp_grid_brush_selection.second;// Get the current selected hcp grid brush data
                                    if (!edit_brush_grid) { // Test have a valid hcp grid brush selected
                                        return;
                                    }
//printf("hcp_grid_class:display_grid_edit_panel brush 1111 : %i\n", hcp_grid_index);
                                    // Define hcp brush center in x-y index coordinate and calculate hcp brush grid extents from that center location
                                    hcp_surface_vec_data_type brush_dim = edit_brush_grid->grid_dimension;
                                    hcp_surface_vec_data_type brush_center = { 0, 0, 0 };
//printf("hcp_grid_class:display_grid_edit_panel brush 111AAA : %i : %i : %i \n", brush_dim.x, brush_dim.y, brush_dim.z);

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

                                    hcp_surface_vec_data_type hcp_grid_brush_origin = { hcp_grid_coord.x - brush_center.x ,hcp_grid_coord.y - brush_center.y,hcp_grid_coord.z - brush_center.z }; // main hcp grid x-y index coordinate where hcp grid brush (0,0) origin index lies
//printf("hcp_grid_class:display_grid_edit_panel brush 2222 : %i : %i : %i origin %i : %i : %i\n", brush_center.x, brush_center.y, brush_center.z, hcp_grid_brush_origin.x, hcp_grid_brush_origin.y, hcp_grid_brush_origin.z);
                                    // Iterate through each hcp brush grid x-y-z index coordinate and corresponding overlapped main hcp grid x-y-z index coordinate
                                    // and perform the edit the main hcp grid cell data value and colors function
 
                                    //if(hcp_grid_coord.z % 2 == 0){// Have even HCP matrix Level that brush is centered upon
                                        for (int b_z_coord = 0; b_z_coord < brush_dim.z; b_z_coord++) {// current Brush level to modify hcp matrix
                                            hcp_surface_index_data_type z_coord = hcp_grid_brush_origin.z + b_z_coord; // hcp matrix z coordinate that curent brush z corrdinate is upon
                                            if (z_coord >= 0 && z_coord < HGBC::grid_dimension.z) { // Brush cell is within hcp grid matrix z bounds
                                                if (z_coord % 2 == 0) {// have even HCP matrix Level that brush cell is upon
                                                    for (int b_y_coord = 0; b_y_coord < brush_dim.y; b_y_coord++) {
                                                        hcp_surface_index_data_type y_coord = hcp_grid_brush_origin.y + b_y_coord; //hcp matrix y coordinate that curent brush y corrdinate is upon
                                                        if (y_coord >= 0 && y_coord < HGBC::grid_dimension.y){// Brush cell is within hcp grid matrix y bounds
                                                            if (y_coord % 2 == 0) { // have even HCP matrix row that brush cell is upon
                                                                if (b_z_coord % 2 == 0) {// Have even brush level
                                                                    if (b_y_coord % 2 == 0 ) {// have even brush row
                                                                        for (int b_x_coord = 0; b_x_coord < brush_dim.x; b_x_coord++) {
                                                                            hcp_surface_index_data_type x_coord = hcp_grid_brush_origin.x + b_x_coord; //hcp matrix x coordinate that curent brush x corrdinate is upon
                                                                            if (x_coord >= 0 && x_coord < HGBC::grid_dimension.x) {// Brush cell is within hcp grid matrix x bounds
                                                                                draw_hcp_grid_brush(edit_brush_grid, x_coord, y_coord, z_coord, b_x_coord, b_y_coord, b_z_coord);// Edit main hcp grid cell data and color value according to hcp brush cell data value
                                                                            }
                                                                        }
                                                                    } else {// have odd brush row
                                                                        for (int b_x_coord = 0; b_x_coord < brush_dim.x - 1; b_x_coord++) {
                                                                            hcp_surface_index_data_type x_coord = hcp_grid_brush_origin.x + b_x_coord; //hcp matrix x coordinate that curent brush x corrdinate is upon
                                                                            if (x_coord >= 0 && x_coord < HGBC::grid_dimension.x) {// Brush cell is within hcp grid matrix x bounds
                                                                                draw_hcp_grid_brush(edit_brush_grid, x_coord, y_coord, z_coord, b_x_coord, b_y_coord, b_z_coord);// Edit main hcp grid cell data and color value according to hcp brush cell data value
                                                                            }
                                                                        }
                                                                    }
                                                                } else {// Have odd brush level
                                                                    if (b_y_coord < brush_dim.y - 1) {
                                                                        if (b_y_coord % 2 == 0) {// have even brush row
                                                                            for (int b_x_coord = 0; b_x_coord < brush_dim.x - 1; b_x_coord++) {
                                                                                hcp_surface_index_data_type x_coord = hcp_grid_brush_origin.x + b_x_coord; //hcp matrix x coordinate that curent brush x corrdinate is upon
                                                                                if (x_coord >= 0 && x_coord < HGBC::grid_dimension.x) {// Brush cell is within hcp grid matrix x bounds
                                                                                    draw_hcp_grid_brush(edit_brush_grid, x_coord, y_coord, z_coord, b_x_coord, b_y_coord, b_z_coord);// Edit main hcp grid cell data and color value according to hcp brush cell data value
                                                                                }
                                                                            }
                                                                        }
                                                                        else {// have odd brush row
                                                                            for (int b_x_coord = 0; b_x_coord < brush_dim.x; b_x_coord++) {
                                                                                hcp_surface_index_data_type x_coord = hcp_grid_brush_origin.x + b_x_coord; //hcp matrix x coordinate that curent brush x corrdinate is upon
                                                                                if (x_coord >= 0 && x_coord < HGBC::grid_dimension.x) {// Brush cell is within hcp grid matrix x bounds
                                                                                    draw_hcp_grid_brush(edit_brush_grid, x_coord, y_coord, z_coord, b_x_coord, b_y_coord, b_z_coord);// Edit main hcp grid cell data and color value according to hcp brush cell data value
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }

                                                            } else { // have odd HCP matrix row that brush cell is upon
                                                                if (b_z_coord % 2 == 0) {// Have even brush level
                                                                    if (b_y_coord % 2 == 0) {// have even brush row
                                                                        for (int b_x_coord = 0; b_x_coord < brush_dim.x; b_x_coord++) {
                                                                            hcp_surface_index_data_type x_coord = hcp_grid_brush_origin.x + b_x_coord; //hcp matrix x coordinate that curent brush x corrdinate is upon
                                                                            if (x_coord >= 0 && x_coord < HGBC::grid_dimension.x - 1) {// Brush cell is within hcp grid matrix x bounds
                                                                                draw_hcp_grid_brush(edit_brush_grid, x_coord, y_coord, z_coord, b_x_coord, b_y_coord, b_z_coord);// Edit main hcp grid cell data and color value according to hcp brush cell data value
                                                                            }
                                                                        }
                                                                    } else {// have odd brush row
                                                                        for (int b_x_coord = 0; b_x_coord < brush_dim.x - 1; b_x_coord++) {
                                                                            hcp_surface_index_data_type x_coord = hcp_grid_brush_origin.x + b_x_coord; //hcp matrix x coordinate that curent brush x corrdinate is upon
                                                                            if (x_coord >= 0 && x_coord < HGBC::grid_dimension.x - 1) {// Brush cell is within hcp grid matrix x bounds
                                                                                draw_hcp_grid_brush(edit_brush_grid, x_coord, y_coord, z_coord, b_x_coord, b_y_coord, b_z_coord);// Edit main hcp grid cell data and color value according to hcp brush cell data value
                                                                            }
                                                                        }
                                                                    }
                                                                } else {// Have odd brush level
                                                                    if (b_y_coord < brush_dim.y - 1) {
                                                                        if (b_y_coord % 2 == 0) {// have even brush row
                                                                            for (int b_x_coord = 0; b_x_coord < brush_dim.x - 1; b_x_coord++) {
                                                                                hcp_surface_index_data_type x_coord = hcp_grid_brush_origin.x + b_x_coord; //hcp matrix x coordinate that curent brush x corrdinate is upon
                                                                                if (x_coord >= 0 && x_coord < HGBC::grid_dimension.x - 1) {// Brush cell is within hcp grid matrix x bounds
                                                                                    draw_hcp_grid_brush(edit_brush_grid, x_coord, y_coord, z_coord, b_x_coord, b_y_coord, b_z_coord);// Edit main hcp grid cell data and color value according to hcp brush cell data value
                                                                                }
                                                                            }
                                                                        }
                                                                        else {// have odd brush row
                                                                            for (int b_x_coord = 0; b_x_coord < brush_dim.x; b_x_coord++) {
                                                                                hcp_surface_index_data_type x_coord = hcp_grid_brush_origin.x + b_x_coord; //hcp matrix x coordinate that curent brush x corrdinate is upon
                                                                                if (x_coord >= 0 && x_coord < HGBC::grid_dimension.x - 1) {// Brush cell is within hcp grid matrix x bounds
                                                                                    draw_hcp_grid_brush(edit_brush_grid, x_coord, y_coord, z_coord, b_x_coord, b_y_coord, b_z_coord);// Edit main hcp grid cell data and color value according to hcp brush cell data value
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                } else {// have odd HCP matrix Level that brush cell is upon
                                                    for (int b_y_coord = 0; b_y_coord < brush_dim.y; b_y_coord++) {
                                                        hcp_surface_index_data_type y_coord = hcp_grid_brush_origin.y + b_y_coord; //hcp matrix y coordinate that curent brush y corrdinate is upon
                                                        if (y_coord >= 0 && y_coord < HGBC::grid_dimension.y-1) {// Brush cell is within hcp grid matrix y bounds
                                                            if (y_coord % 2 == 0) { // have even HCP matrix row that brush cell is upon
                                                                if (b_z_coord % 2 == 0) {// have even brush level
                                                                    if (b_y_coord % 2 == 0) {// have even brush row
                                                                        for (int b_x_coord = 0; b_x_coord < brush_dim.x; b_x_coord++) {
                                                                            hcp_surface_index_data_type x_coord = hcp_grid_brush_origin.x + b_x_coord; //hcp matrix x coordinate that curent brush x corrdinate is upon
                                                                            if (x_coord >= 0 && x_coord < HGBC::grid_dimension.x - 1) {// Brush cell is within hcp grid matrix x bounds
                                                                                draw_hcp_grid_brush(edit_brush_grid, x_coord, y_coord, z_coord, b_x_coord, b_y_coord, b_z_coord);// Edit main hcp grid cell data and color value according to hcp brush cell data value
                                                                            }
                                                                        }
                                                                    }
                                                                    else {// have odd brush row
                                                                        for (int b_x_coord = 0; b_x_coord < brush_dim.x - 1; b_x_coord++) {
                                                                            hcp_surface_index_data_type x_coord = hcp_grid_brush_origin.x + b_x_coord; //hcp matrix x coordinate that curent brush x corrdinate is upon
                                                                            if (x_coord >= 0 && x_coord < HGBC::grid_dimension.x - 1) {// Brush cell is within hcp grid matrix x bounds
                                                                                draw_hcp_grid_brush(edit_brush_grid, x_coord, y_coord, z_coord, b_x_coord, b_y_coord, b_z_coord);// Edit main hcp grid cell data and color value according to hcp brush cell data value
                                                                            }
                                                                        }
                                                                    }
                                                                } 
                                                                else{// have odd brush level
                                                                    if (b_y_coord < brush_dim.y - 1) {
                                                                        if (b_y_coord % 2 == 0) {// have even brush row
                                                                            for (int b_x_coord = 0; b_x_coord < brush_dim.x - 1; b_x_coord++) {
                                                                                hcp_surface_index_data_type x_coord = hcp_grid_brush_origin.x + b_x_coord; //hcp matrix x coordinate that curent brush x corrdinate is upon
                                                                                if (x_coord >= 0 && x_coord < HGBC::grid_dimension.x - 1) {// Brush cell is within hcp grid matrix x bounds
                                                                                    draw_hcp_grid_brush(edit_brush_grid, x_coord, y_coord, z_coord, b_x_coord, b_y_coord, b_z_coord);// Edit main hcp grid cell data and color value according to hcp brush cell data value
                                                                                }
                                                                            }
                                                                        }
                                                                        else {// have odd brush row
                                                                            for (int b_x_coord = 0; b_x_coord < brush_dim.x; b_x_coord++) {
                                                                                hcp_surface_index_data_type x_coord = hcp_grid_brush_origin.x + b_x_coord; //hcp matrix x coordinate that curent brush x corrdinate is upon
                                                                                if (x_coord >= 0 && x_coord < HGBC::grid_dimension.x - 1) {// Brush cell is within hcp grid matrix x bounds
                                                                                    draw_hcp_grid_brush(edit_brush_grid, x_coord, y_coord, z_coord, b_x_coord, b_y_coord, b_z_coord);// Edit main hcp grid cell data and color value according to hcp brush cell data value
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            } else { // have odd HCP matrix row that brush cell is upon
                                                                
                                                                if (b_z_coord % 2 == 0) {// have even brush level
                                                                    if (b_y_coord % 2 == 0) {// have even brush row
                                                                        for (int b_x_coord = 0; b_x_coord < brush_dim.x-1; b_x_coord++) {
                                                                            hcp_surface_index_data_type x_coord = hcp_grid_brush_origin.x + b_x_coord; //hcp matrix x coordinate that curent brush x corrdinate is upon
                                                                            if (x_coord >= 0 && x_coord < HGBC::grid_dimension.x) {// Brush cell is within hcp grid matrix x bounds
                                                                                draw_hcp_grid_brush(edit_brush_grid, x_coord, y_coord, z_coord, b_x_coord, b_y_coord, b_z_coord);// Edit main hcp grid cell data and color value according to hcp brush cell data value
                                                                            }
                                                                        }
                                                                    }
                                                                    else {// have odd brush row
                                                                        for (int b_x_coord = 0; b_x_coord < brush_dim.x; b_x_coord++) {
                                                                            hcp_surface_index_data_type x_coord = hcp_grid_brush_origin.x + b_x_coord; //hcp matrix x coordinate that curent brush x corrdinate is upon
                                                                            if (x_coord >= 0 && x_coord < HGBC::grid_dimension.x) {// Brush cell is within hcp grid matrix x bounds
                                                                                draw_hcp_grid_brush(edit_brush_grid, x_coord, y_coord, z_coord, b_x_coord, b_y_coord, b_z_coord);// Edit main hcp grid cell data and color value according to hcp brush cell data value
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                                else {// have odd brush level
                                                                    if (b_y_coord < brush_dim.y - 1) {
                                                                        if (b_y_coord % 2 == 0) {// have even brush row
                                                                            for (int b_x_coord = 0; b_x_coord < brush_dim.x; b_x_coord++) {
                                                                                hcp_surface_index_data_type x_coord = hcp_grid_brush_origin.x + b_x_coord; //hcp matrix x coordinate that curent brush x corrdinate is upon
                                                                                if (x_coord >= 0 && x_coord < HGBC::grid_dimension.x) {// Brush cell is within hcp grid matrix x bounds
                                                                                    draw_hcp_grid_brush(edit_brush_grid, x_coord, y_coord, z_coord, b_x_coord, b_y_coord, b_z_coord);// Edit main hcp grid cell data and color value according to hcp brush cell data value
                                                                                }
                                                                            }
                                                                        }
                                                                        else {// have odd brush row
                                                                            for (int b_x_coord = 0; b_x_coord < brush_dim.x - 1; b_x_coord++) {
                                                                                hcp_surface_index_data_type x_coord = hcp_grid_brush_origin.x + b_x_coord; //hcp matrix x coordinate that curent brush x corrdinate is upon
                                                                                if (x_coord >= 0 && x_coord < HGBC::grid_dimension.x) {// Brush cell is within hcp grid matrix x bounds
                                                                                    draw_hcp_grid_brush(edit_brush_grid, x_coord, y_coord, z_coord, b_x_coord, b_y_coord, b_z_coord);// Edit main hcp grid cell data and color value according to hcp brush cell data value
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }

                                                }
                                            }
                                        }
                                    //} else{// Have odd HCP matrix Level

                                    //}
                                }
                                else {
//printf("hcp_grid_class::display_grid_edit_panel: brush 3333  mouse button left :index %i | value %i \n", hcp_grid_index, HGBC::hcp_grid[hcp_grid_index]);
                                    update_hcp_grid_brush_colors(hcp_grid_index);// Edit the hovered main hcp grid cell data and color value
                                } 
//printf("hcp_grid_class::display_grid_edit_panel: brush 4444 mouse button left :index %i | value %i \n", hcp_grid_index, HGBC::hcp_grid[hcp_grid_index]);
                            }
                        }

                       // if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
//printf("hcp_grid_class:display_grid_edit_panel::mouse button right : %i\n", hcp_grid_index);
                      //  }
                    }

                } else {// Enable plot inputs and window interaction when hcp plot edit window is not hovered over in edit mode and disable edit functions
                    plot_flags = ImPlot3DFlags_Equal | ImPlot3DFlags_NoMenus | ImPlot3DFlags_NoLegend;// | ImPlot3DFlags_Crosshairs not included
                    plot_window_flags = ImGuiWindowFlags_None;
                    edit_mode = false;
                }
 
            }
   
            // ##################################################################################################

            // ###### This section of code is relevant to the display of the hcpagonal Automata grid edit ###########
            // ###### widgets  to define the hcp grid edit inputs and perform hcp grid edit processes     ###########
            // Display the current hcp grid data and coordinate values that the mouse cursor is hovering over in the
            // main hcpagonal automata grid display window
            ImGui::Text("HCP Grid Coordinate ::   ");
            ImGui::SameLine();
            ImGui::Text("X : %i | ", hcp_grid_coord.x);
            ImGui::SameLine();
            ImGui::Text("Y : %i | ", hcp_grid_coord.y);
            ImGui::SameLine();
            ImGui::Text("Z : %i", hcp_grid_coord.z);

            ImGui::Text("HCP Grid Index : %i  ||", hcp_grid_index);
            ImGui::SameLine();
                
            if (hcp_grid_index > -1 && hcp_grid_index < HGBC::hcp_grid.size()) { 
                hcp_grid_value = HGBC::hcp_grid[hcp_grid_index]; 
            }

            ImGui::Text(" HCP Grid value : %i", hcp_grid_value); // Need to accomidate all data types of hcp grid here !!!!
            ImGui::Separator();

            display_random_hcp_generator(); // Display the random engine window widgets to create a hcp grid of random generated numbers

            ImGui::Separator();
            // Display ImGui widgets to enter value to edit/change hovered hcp grid cell to either as an individual hcp grid cell or with a hcp grid brush
            ImGui::Text("Edit HCP Automata Grid Values");
            ImGui::Text("Value : ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(60);
            // interactively change result display shape size
            ImGui::DragFloat("##hcpgsi", &hcp_grid_edit_value, 0.1f, 1.0f, 50.0f, "%2.2f");  // Need to accomidate all data types of hcp grid here !!!!
            ImGui::SameLine();
            ImGui::Checkbox("Use HCP Grid Brush", &use_hcp_grid_brush);// flag to use a hcp grid brush to perform edits

            if (use_hcp_grid_brush) {// If use hcp grid brush
                hcp_grid_brush.display_hcp_grid_brush_panel();// display hcp grid brush window widget panel
            }

            // Widget inputs to create a border of a given value at the edge of the current hcp grid
            ImGui::Separator();
            ImGui::Text("HCP Grid Border");
            ImGui::Text("Border Value: ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(60);
            ImGui::DragFloat("##hcpgbv", &hcp_grid_edit_border_value, 0.1f, 1.0f, 50.0f, "%2.2f");
            ImGui::SameLine();
            if (ImGui::Button("Add HCP Grid Border")) {
                add_hcp_grid_border();
            }
        }
        ImGui::End();
    }

    void  add_hcp_grid_border() {
        // NOT YET IMPLEMENTED :: NEEDS TO BE DEFINED !!!!!!!!!!!!!!!!!!!!
        //for (size_t iz = 0; iz < HGBC::grid_dimension.z; iz++) {
        //    // Bottom border
        //    for (size_t i = 0; i < HGBC::grid_dimension.x; i++) {
        //        hcp_surface_index_data_type index = HGBC::get_hcp_surface_matrix_data_index({ i,0,iz });
        //        HGBC::hcp_grid[index] = hcp_grid_edit_border_value;
        //    }

        //    // top border
        //    if (HGBC::grid_dimension.y % 2 == 0) {// even y dimension
        //        for (size_t i = 0; i < HGBC::grid_dimension.x; i++) {
        //            hcp_surface_index_data_type index = HGBC::get_hcp_surface_matrix_data_index({ i,HGBC::grid_dimension.y - 1 });
        //            HGBC::hcp_grid[index] = hcp_grid_edit_border_value;
        //        }
        //    }
        //    else { // odd y dimension
        //        for (size_t i = 0; i < HGBC::grid_dimension.x - 1; i++) {
        //            hcp_surface_index_data_type index = HGBC::get_hcp_surface_matrix_data_index({ i,HGBC::grid_dimension.y - 1 });
        //            HGBC::hcp_grid[index] = hcp_grid_edit_border_value;
        //        }
        //    }

        //    // left border
        //    for (size_t j = 0; j < HGBC::grid_dimension.y; j++) {
        //        hcp_surface_index_data_type index = HGBC::get_hcp_surface_matrix_data_index({ 0,j, });
        //        HGBC::hcp_grid[index] = hcp_grid_edit_border_value;
        //    }

        //    // right border
        //    for (size_t j = 0; j < HGBC::grid_dimension.y; j++) {
        //        if (j % 2 == 0) {// even row
        //            hcp_surface_index_data_type index = HGBC::get_hcp_surface_matrix_data_index({ HGBC::grid_dimension.x - 1,j });
        //            HGBC::hcp_grid[index] = hcp_grid_edit_border_value;
        //        }
        //        else { // odd row
        //            hcp_surface_index_data_type index = HGBC::get_hcp_surface_matrix_data_index({ HGBC::grid_dimension.x - 2,j });
        //            HGBC::hcp_grid[index] = hcp_grid_edit_border_value;
        //        }
        //    }
        //}
    }

    void save_gradienthdr_data() {
//printf("hcp_grid:save_gradienthdr_data000\n");
        char const* patterns[] = { "*.ghd" };
        //char const* file_pathname = vwDialogs::save_file(nullptr, patterns, 1);
        char const* file_pathname = vwDialogs::save_file(gradient_dir_path.c_str(), patterns, 1);
//printf("hcp_grid:save_gradienthdr_data1111\n");
        if (file_pathname == nullptr) {
            afw_globalc::get_current_logger()->log(LogLevel::INFO, "ERROR : No gradient file defined to save gradient data to\n");
            return;
        }
//printf("hcp_grid:save_gradienthdr_data222\n");
        export_gradient(file_pathname, state);
    }

    void load_gradienthdr() {
//printf("hcp_grid:load_gradienthdr_data000\n");
        char const* patterns[] = { "*.ghd" };
        char const* file_pathname = vwDialogs::open_file(gradient_dir_path.c_str(), patterns, 1);
//printf("hcp_grid:load_gradienthdr_data111\n");
        if (file_pathname == nullptr) {
            afw_globalc::get_current_logger()->log(LogLevel::INFO, "ERROR : No gradient file defined to import gradient data to\n");
            return;
        }
//printf("hcp_grid:load_gradienthdr_data222\n");
        import_gradient(file_pathname,state);
//printf("hcp_grid:load_gradienthdr_data333\n");
        update_hcp_grid_colors();
    }

    // Display the random number generator and distribution ImGui widgets to select the random number generator
    // and number ditribution function and enter the parameter data required to generate a random number and
    // generate the random number.
    void display_random_hcp_generator(){
        random_engine.display_generation_inputs();

        ImGui::SetCursorPosX(100);
        if (ImGui::Button("Generate Random HCP Grid")) {// button widget to activate the creation of a random number
printf("hcp_grid : display_random_hcp_generator 0000 \n");
            random_engine.define_random_engine(); // Define the random engine input data to generate a randome number

            // Following testing only: Comment out or delete when finished
            //for (int i = 0; i < 20; i++) {
            //    random_engine.uniform_int_distribution.generate_random_number();
                //printf("random_engine_class : generate_random_value gen_number : %i\n", uniform_int_distribution.generated_number_value.integer_p);
            //    printf("hcp_grid_class : display_random_hcp_generator random number : %i\n", random_engine.uniform_int_distribution.generated_number_value);
            //}

printf("hcp_grid : display_random_hcp_generator 1111 : %i \n", random_engine.random_distribution_index);
            generate_random_hcp_grid();
            update_hcp_grid_colors();
        }

    }

    // CRITICAL : The random_distribution_index assignment must match the index of the random_distributions array list that
    //            the user has selected in the ImGui combo widget of random number distributions.
    // Generate a random number for every hcp grid cell based upon the selections made in the random engine GUI
    // One function exists for each possible number distribution type.
    void generate_random_hcp_grid() {
         switch (random_engine.random_distribution_index) {
            case 0:  { uniform_int_distribution_hcp_grid_random_generation(); break; }
            case 1:  { uniform_real_distribution_hcp_grid_random_generation(); break; }
            case 2:  { bernoulli_distribution_hcp_grid_random_generation(); break; }
            case 3:  { binomial_distribution_hcp_grid_random_generation(); break; }
            case 4:  { geometric_distribution_hcp_grid_random_generation(); break; }
            case 5:  { negative_binomial_distribution_hcp_grid_random_generation(); break; }
            case 6:  { poisson_distribution_hcp_grid_random_generation(); break; }
            case 7:  { exponential_distribution_hcp_grid_random_generation(); break; }
            case 8:  { gamma_distribution_hcp_grid_random_generation(); break; }
            case 9:  { weibull_distribution_hcp_grid_random_generation(); break; }
            case 10: { extreme_value_distribution_hcp_grid_random_generation(); break; }
            case 11: { normal_distribution_hcp_grid_random_generation(); break; }
            case 12: { lognormal_distribution_hcp_grid_random_generation(); break; }
            case 13: { chi_squared_distribution_hcp_grid_random_generation(); break; }
            case 14: { cauchy_distribution_hcp_grid_random_generation(); break; }
            case 15: { fisher_f_distribution_hcp_grid_random_generation(); break; }
            case 16: { student_t_distribution_hcp_grid_random_generation(); break; }
        }
    }


    // ##################################################################
    // ## Functions to generate a random number for each hcp grid cell ##
    // ## One function is defined for each number ditribution type     ##

    // NOTE : The
    // if (random_engine.rng_add_to_existing_grid) {
    //      rn = HGBC::hcp_grid[i] + rn;
    // }
    // to add random distribution numbers to the existing grid may not
    // be appropriate for all random distributons and has only been tested
    // for the bernoulli distribution. Use With Caution.

    void uniform_int_distribution_hcp_grid_random_generation() {
        for (size_t i = 0; i < HGBC::hcp_grid.size(); i++) {
            random_engine.uniform_int_distribution.generate_random_number();

            T rn = (T) random_engine.uniform_int_distribution.generated_number_value;
            
            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::hcp_grid[i] + rn;
            }

            HGBC::hcp_grid[i] = rn;

            // following for testing only commant out or delete when no longer used!!!!!!
//printf("hcp_grid_class : uniform_int_distribution_hcp_grid_random_generation random number : %i\n", random_engine.uniform_int_distribution.generated_number_value);
            //if(rn > 50)
            //     HGBC::hcp_grid[i] = 1;
            //else
            //    HGBC::hcp_grid[i] = 0;
        }
    }

    void uniform_real_distribution_hcp_grid_random_generation(){
        for (size_t i = 0; i < HGBC::hcp_grid.size(); i++) {
            random_engine.uniform_real_distribution.generate_random_number();

            T rn = (T)random_engine.uniform_real_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::hcp_grid[i] + rn;
            }

            HGBC::hcp_grid[i] = rn;
        }
    }
    void bernoulli_distribution_hcp_grid_random_generation(){
        for (size_t i = 0; i < HGBC::hcp_grid.size(); i++) {
            random_engine.bernoulli_distribution.generate_random_number();

            T rn = (T)random_engine.bernoulli_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::hcp_grid[i] + rn;
            }

            HGBC::hcp_grid[i] = rn;
//printf("hcp_grid : bernoulli_distribution_hcp_grid_random_generation : %i \n", HGBC::hcp_grid[i]);
        }
    }
    void binomial_distribution_hcp_grid_random_generation() {
        for (size_t i = 0; i < HGBC::hcp_grid.size(); i++) {
            random_engine.binomial_distribution.generate_random_number();

            T rn = (T)random_engine.binomial_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::hcp_grid[i] + rn;
            }

            HGBC::hcp_grid[i] = rn;
        }
    }
    void geometric_distribution_hcp_grid_random_generation(){
        for (size_t i = 0; i < HGBC::hcp_grid.size(); i++) {
            random_engine.geometric_distribution.generate_random_number();

            T rn = (T)random_engine.geometric_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::hcp_grid[i] + rn;
            }

            HGBC::hcp_grid[i] = rn;
        }
    }
    void negative_binomial_distribution_hcp_grid_random_generation(){
        for (size_t i = 0; i < HGBC::hcp_grid.size(); i++) {
            random_engine.negative_binomial_distribution.generate_random_number();

            T rn = (T)random_engine.negative_binomial_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::hcp_grid[i] + rn;
            }

            HGBC::hcp_grid[i] = rn;
        }
    }
    void poisson_distribution_hcp_grid_random_generation(){
        for (size_t i = 0; i < HGBC::hcp_grid.size(); i++) {
            random_engine.poisson_distribution.generate_random_number();

            T rn = (T)random_engine.poisson_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::hcp_grid[i] + rn;
            }

            HGBC::hcp_grid[i] = rn;
        }
    }
    void exponential_distribution_hcp_grid_random_generation(){
        for (size_t i = 0; i < HGBC::hcp_grid.size(); i++) {
            random_engine.exponential_distribution.generate_random_number();

            T rn = (T)random_engine.exponential_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::hcp_grid[i] + rn;
            }

            HGBC::hcp_grid[i] = rn;
        }
    }
    void gamma_distribution_hcp_grid_random_generation(){
        for (size_t i = 0; i < HGBC::hcp_grid.size(); i++) {
            random_engine.gamma_distribution.generate_random_number();

            T rn = (T)random_engine.gamma_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::hcp_grid[i] + rn;
            }

            HGBC::hcp_grid[i] = rn;
        }
    }
    void weibull_distribution_hcp_grid_random_generation(){
        for (size_t i = 0; i < HGBC::hcp_grid.size(); i++) {
            random_engine.weibull_distribution.generate_random_number();

            T rn = (T)random_engine.weibull_distribution.generated_number_value;
            
            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::hcp_grid[i] + rn;
            }

            HGBC::hcp_grid[i] = rn;
        }
    }
    void extreme_value_distribution_hcp_grid_random_generation(){
        for (size_t i = 0; i < HGBC::hcp_grid.size(); i++) {
            random_engine.extreme_value_distribution.generate_random_number();

            T rn = (T)random_engine.extreme_value_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::hcp_grid[i] + rn;
            }

            HGBC::hcp_grid[i] = rn;
        }
    }
    void normal_distribution_hcp_grid_random_generation(){
        for (size_t i = 0; i < HGBC::hcp_grid.size(); i++) {
            random_engine.normal_distribution.generate_random_number();

            T rn = (T)random_engine.normal_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::hcp_grid[i] + rn;
            }

            HGBC::hcp_grid[i] = rn;
        }
    }
    void lognormal_distribution_hcp_grid_random_generation(){
        for (size_t i = 0; i < HGBC::hcp_grid.size(); i++) {
            random_engine.lognormal_distribution.generate_random_number();

            T rn = (T)random_engine.lognormal_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::hcp_grid[i] + rn;
            }

            HGBC::hcp_grid[i] = rn;
        }
    }
    void chi_squared_distribution_hcp_grid_random_generation(){
        for (size_t i = 0; i < HGBC::hcp_grid.size(); i++) {
            random_engine.chi_squared_distribution.generate_random_number();

            T rn = (T)random_engine.chi_squared_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::hcp_grid[i] + rn;
            }

            HGBC::hcp_grid[i] = rn;
        }
    }
    void cauchy_distribution_hcp_grid_random_generation(){
        for (size_t i = 0; i < HGBC::hcp_grid.size(); i++) {
            random_engine.cauchy_distribution.generate_random_number();

            T rn = (T)random_engine.cauchy_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::hcp_grid[i] + rn;
            }

            HGBC::hcp_grid[i] = rn;
        }
    }
    void fisher_f_distribution_hcp_grid_random_generation(){
        for (size_t i = 0; i < HGBC::hcp_grid.size(); i++) {
            random_engine.fisher_f_distribution.generate_random_number();

            T rn = (T)random_engine.fisher_f_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::hcp_grid[i] + rn;
            }

            HGBC::hcp_grid[i] = rn;
        }
    }
    void student_t_distribution_hcp_grid_random_generation(){
        for (size_t i = 0; i < HGBC::hcp_grid.size(); i++) {
            random_engine.student_t_distribution.generate_random_number();

            T rn = (T)random_engine.student_t_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::hcp_grid[i] + rn;
            }

            HGBC::hcp_grid[i] = rn;
        }
    }

    // ImGui window widget to display the hcp automata grid cell data and overlying hcp_grid_cells_data_list list of 
    // hcp sub grid data using the ImPlot::ScatterPlot function. 
    void display_automata_rule_results(){
        ImGui::Begin("HCP3D Automata", nullptr, plot_window_flags);// If Have name of ImGui::Begin then have ability to minimise and dock 

        if (ImPlot3D::BeginPlot("HCP3D Automata Grid",ImVec2(-1, -1), plot_flags | ImPlot3DFlags_NoClip)) {
        //if (ImPlot3D::BeginPlot("HCP3D Automata Grid")) {

            //ImPlotStyle& style = ImPlot::GetStyle();
            ImPlot3DStyle& style = ImPlot3D::GetStyle();
            ImGuiContext *cpc = ImGui::GetCurrentContext();// Strange compilor does not complain about ImGuiContext without including imgui_internal.h
                                                           // But does complain about ImGuiContext not being defined when using cpc in the next lines.

           //float resized_width  = cpc->CurrentWindow->Size.x;
           //float resized_height = cpc->CurrentWindow->Size.y;

            // Test and functions to maintain the display aspect ratio of the ImGui-ImPlot window
            // when performing windo resizing using the window borders. This is performed 
            // so as to avoid strange disparity of the display of hcp grid where the grid X-Y axis
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
//    printf("display_hcp_grid : px %f : py %f: x %f : y %f \n", prev_width, prev_height,cpc->CurrentWindow->Size.x, cpc->CurrentWindow->Size.y);


//testing only delete/comment out when not needed
if(HGBC::hcp_centers_x.empty()){
    printf("hcp_grid_class::display_automata_rule_results:: ERROR \n", HGBC::hcp_centers_x.empty());
}

//printf("hcp_grid_class::display_automata_rule_results:: AAAAA %i\n", HGBC::hcp_centers_x.size());
            // ImPlot::ScatterPlot function to display main hcp grid cell data
            if (display_ahcp_grid) {// If display hcp grid flag is true

                // Constrain the hcp grid display to a min max Cartesian coordinate range of coordinates
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

                if (autofit_ahcp_grid) {
                    x_axis_flags |= ImPlot3DAxisFlags_AutoFit;
                    y_axis_flags |= ImPlot3DAxisFlags_AutoFit;
                    z_axis_flags |= ImPlot3DAxisFlags_AutoFit;
                }

                // !!!! Add more axis options here !!!!!
                //x_axis_flags |= ImPlot3DAxisFlags_NoDecorations | ImPlot3DAxisFlags_NoTickMarks;
                //y_axis_flags |= ImPlot3DAxisFlags_NoDecorations | ImPlot3DAxisFlags_NoTickMarks;
                //z_axis_flags |= ImPlot3DAxisFlags_NoDecorations | ImPlot3DAxisFlags_NoTickMarks;

                ImPlot3D::SetupAxes("###hcpx", "###hcpy", "###hcpz", x_axis_flags, y_axis_flags, z_axis_flags);

                ImPlot3D::PlotScatter("hcp", HGBC::hcp_centers_x.data(), HGBC::hcp_centers_y.data(), HGBC::hcp_centers_z.data(), HGBC::hcp_centers_x.size(), {
                    ImPlot3DProp_MarkerSize, hcp_grid_display_shape_size,
                    ImPlot3DProp_Marker, hcp_grid_display_shape,
                    ImPlot3DProp_FillAlpha,0.25f,
                    ImPlot3DProp_MarkerFillColors,HGBC::hcp_colors.data(),
                    ImPlot3DProp_MarkerLineColors, HGBC::hcp_colors.data()
                });
            }

            ImDrawList* draw_list = ImPlot3D::GetPlotDrawList();
            ImVec2 mouse_pos = ImGui::GetMousePos();
            ImVec2 plot_pos = ImPlot3D::GetPlotRectPos();
            ImVec2 plot_size = ImPlot3D::GetPlotRectSize();

            if(edit_ahcp_grid){ got_nearest_hcp_grid_cell =  get_display_nearest_hcp_matrix_grid_cell_to_cursor(draw_list, mouse_pos, plot_pos, plot_size); }
            
            display_orientation_gizmo(draw_list, mouse_pos, plot_pos, plot_size);


 
            //for (hcp_sub_grid_struct_type hcp_sub_grid : HGBC::hcp_sub_grids) {
            //    hcp_sub_grid.display_sub_hcp_grid();
            //}

            for (hcp_grid_cell_data_struct_type hcp_grid_cell : hcp_grid_cells_data_list) {
//printf("hcp_grid_class::display_hcp_grid 3333 :"); printf(" %i : %i\n", hcp_grid_cell.hcp_grid_cell_data_id, hcp_grid_cell.display_shape_id);
               hcp_grid_cell.display_hcp_cell_data();// This is the hcp grid automata rules display data
            }
     
            ImPlot3D::EndPlot();
        }
        plot_axis_limit_changed = IMGUI_WINDOW_BORDER_NONE;

        ImGui::End();
    }

    /*
    // Flags for axis

    */


private:
    ImGuiWindow* plot_window = nullptr;
    ImGuiWindowFlags plot_window_flags = ImGuiWindowFlags_None;
    ImPlot3DFlags plot_flags = ImPlot3DFlags_Equal | ImPlot3DFlags_NoMenus | ImPlot3DFlags_NoLegend;

    ImPlot3DPoint mouse_plot_pos = { 0.0f,0.0f,0.0f };
    hcp_surface_index_data_type hcp_grid_index = -1;
    hcp_surface_vec_data_type hcp_grid_coord = { 0,0,0 };
    T hcp_grid_value = 0;

    bool change_extents_popup = false;

    int hcp_grid_cell_data_id = -1;

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
    
    // HCP grid display paramters 
    bool display_ahcp_grid               = true;
    bool edit_ahcp_grid                  = false;

    const char* shapes[9]                = { "Circle", "Square", "Diamond", "Up", "Down", "Left", "Right", "Cross", "Asterix" };
    ImGuiComboFlags shapes_combo_flags   = ImGuiComboFlags_NoArrowButton;
    signed char plot_axis_limit_changed  = -1;
    ImVec4 hcp_grid_display_color        = HGBC::hcp_grid_display_color;
    int hcp_grid_display_shape_id        = 1;
    float hcp_grid_display_shape_size    = 6.0f;
    ImPlot3DMarker_ hcp_grid_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Circle;

    // HCP grid value display color gradient variables
    ImGradientHDRState::ColorMarker* cm = nullptr;
    ImGradientHDRState::ColorMarker* selectedColorMarker = nullptr;
    int32_t stateID = 10;
    ImGradientHDRState state;
    ImGradientHDRTemporaryState tempState;
    bool isMarkerShown = true;

    int min_grad_value_i = 0;
    int max_grad_value_i = 100;

    bool got_nearest_hcp_grid_cell = false;

    // --------------- Random number generator variables -----------------------------

    random_engine_class random_engine;

    // ---------------- Edit HCP Grid Variables ------------------------------
    float hcp_grid_edit_value = 1.0f;
    float hcp_grid_edit_border_value = 100.0f;

    // --------------- HCP Grid Brush variables ----------------
    bool use_hcp_grid_brush = false;
    hcp_grid_brush_class<T> hcp_grid_brush;


    // ------------------------- Display box parameters  ---------------------------------------
    bool autofit_ahcp_grid = false;
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
        switch (hcp_grid_display_shape_id) {
            case 0:  hcp_grid_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Circle; break;
            case 1:  hcp_grid_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Square; break;
            case 2:  hcp_grid_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Diamond; break;
            case 3:  hcp_grid_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Up; break;
            case 4:  hcp_grid_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Down; break;
            case 5:  hcp_grid_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Left; break;
            case 6:  hcp_grid_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Right; break;
            case 7:  hcp_grid_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Cross; break;
            case 8:  hcp_grid_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Plus; break;
            case 9:  hcp_grid_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Asterisk; break;
            default: hcp_grid_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Circle;
        }
    }
};
