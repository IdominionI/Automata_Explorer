#pragma once

#include <ThirdParty/ImGui/imgui_docking/imgui_internal.h>

#include <FrameWork/AFW/ImGUI/ImGuiEx/imgui_custom.h>
#include <FrameWork/AFW/ImGUI/ImGuiEx/imgui_widgets.h>
#include <FrameWork/AFW/Interface/IconsFontAwesome4.h>

#include "Source/Widgets/ImGradientHDR.h"
#include "Source/Application/random_engine.h"

#include "cart2D_sub_grid_manager.h"
#include "cart2D_grid_brush.h"
#include "cart2D_surface_import_export.h"

// Central main hexagonal automata grid class that defines, stores, creates, displays and manages all aspects
// of the hexagonal automata grid data to be used in the application
// This class has incorporated classes or structures that are used to modify, display or define the Cartesian 2D  
//   gradient
//   random engine
//   Cartesian 2D  brush
//   hex surface import/export

template <class T>
class cart2D_grid_class : public cart2D_grid_base_class<T>, public cart2D_sub_grid_manager_class {
    using HGBC = cart2D_grid_base_class<T>; // Need to define this so as to have access to the cart2D_grid_base_class class : Due to using template
public:
    cart2D_grid_class() {
        initialise_gradient();
    }
    cart2D_grid_class(T initial_cart2D_value, cart2D_surface_vec_data_type dimension) : cart2D_grid_base_class<T>(initial_cart2D_value, dimension) {
        initialise_gradient();
    }
    ~cart2D_grid_class() {}

    std::string gradient_dir_path = "./Gradients"; // Path name to the default directory that the gradient class exists within

    std::vector<cart2D_grid_cell_data_struct_type> cart2D_grid_cells_data_list = {};// vector list of Cartesian 2D  data to display as an overlay on main grid display

    void delete_cart2D_grid_data(cart2D_surface_index_data_type i) override {
        // integer value does not need to be deleted
    }

    void invalidate_cart2D_grid_data_value(cart2D_surface_index_data_type index) override {
        HGBC::cart2D_grid.at(index) = INVALID_CART2D_INT_DATA_VALUE;// Need to cater for float, double and boolean data types !!!!
    }

    // The central main core function that manages the display of the Cartesian 2D  window panels that
    // the user interacts with to display and edit Cartesian grid data values.
    bool display_cart2D_grid() override {
        if (HGBC::cart2D_centers_x.size() == 0) { return false; }
//printf("cart2D_grid_class::display_cart2D_grid 000\n");

        display_grid_parameters_panel();

//printf("cart2D_grid_class::display_cart2D_grid 1111\n");
        if (edit_acart2D_grid) {
            display_grid_edit_panel();
        }
        else {
            // Enable plot inputs and window interaction when not in edit mode and disable edit functions
            plot_flags = ImPlotFlags_Equal | ImPlotFlags_NoMenus | ImPlotFlags_Crosshairs | ImPlotFlags_NoLegend;
            plot_window_flags = ImGuiWindowFlags_None;
        }
//printf("cart2D_grid_class::display_cart2D_grid 2222\n");

        display_automata_rule_results();// Display the current main hexagonal grid data which the user can interact with

//printf("cart2D_grid_class::display_cart2D_grid 4444\n");
        return true;
    }

    // Default initialisation of Cartesian 2D  data value gradient 
    void initialise_gradient() {
        state.AddColorMarker(0.0f, { 1.0f, 1.0f, 1.0f }, 1.0f);
        state.AddColorMarker(1.0f, { 0.0f, 1.0f, 0.0f }, 1.0f);
    }

    int define_new_cart2D_grid_cell_data() {
        cart2D_grid_cell_data_id++;
        cart2D_grid_cell_data_struct_type cart2D_grid_cell_data;
        cart2D_grid_cell_data.cart2D_grid_cell_data_id = cart2D_grid_cell_data_id;

        cart2D_grid_cells_data_list.push_back(cart2D_grid_cell_data);

        return cart2D_grid_cell_data.cart2D_grid_cell_data_id;
    }

    // Clear all the Cartesian 2D  cell data of the cart2D_grid_cells_data_list to be reused
    void clear_all_cart2D_grid_cell_data() {
        cart2D_grid_cell_data_id = -1;
        for (cart2D_grid_cell_data_struct_type& cart2D_grid_cell : cart2D_grid_cells_data_list) {
            cart2D_grid_cell.clear_cart2D_grid_cell_data();
        }
    }

    // Clear Cartesian 2D  cell data of the cart2D_grid_cells_data_list of id number cart2D_grid_cell_data_id
    bool clear_cart2D_grid_cell_data(int  cart2D_grid_cell_data_id) {
        for (size_t i = 0; i < cart2D_grid_cells_data_list.size(); i++) {
            if (cart2D_grid_cells_data_list[i].cart2D_grid_cell_data_id == cart2D_grid_cell_data_id) {
                cart2D_grid_cells_data_list[i].clear_cart2D_grid_cell_data();
                return true;
            }
        }

        return false;
    }

    // Delete all the Cartesian 2D  cell data of the cart2D_grid_cells_data_list
    bool delete_cart2D_grid_cell_data(int cart2D_grid_cell_data_id) {
        for (size_t i = cart2D_grid_cells_data_list.size() - 1; i > -1; i--) {
            if (cart2D_grid_cells_data_list[i].cart2D_grid_cell_data_id == cart2D_grid_cell_data_id) {
                cart2D_grid_cells_data_list[i].clear_cart2D_grid_cell_data();
                cart2D_grid_cells_data_list.erase(cart2D_grid_cells_data_list.begin() + i);
                return true;
            }
        }

        return false;
    }

    void delete_all_cart2D_grid_cell_data() {
        clear_all_cart2D_grid_cell_data();
        cart2D_grid_cells_data_list.clear();
        cart2D_grid_cells_data_list.shrink_to_fit();
    }

    // Get the Cartesian 2D  cell data entry within the cart2D_grid_cells_data_list that has an ID value of cart2D_grid_cell_data_id
    int get_cart2D_grid_cell_data_index(int cart2D_grid_cell_data_id) {
//printf("cart2D_grid_class :: get_cart2D_grid_cell_data_index 0000: %i : %i\n" , cart2D_grid_cells_data_list.size(), cart2D_grid_cell_data_id);
        for (size_t i = 0; i < cart2D_grid_cells_data_list.size(); i++) {
//printf("cart2D_grid_class :: get_cart2D_grid_cell_data_index 1111: %i :  %i : %i\n",i , cart2D_grid_cells_data_list[i].cart2D_grid_cell_data_id, cart2D_grid_cell_data_id);
            if (cart2D_grid_cells_data_list[i].cart2D_grid_cell_data_id == cart2D_grid_cell_data_id) {
                return i;
            }
        }

        return -1;
    }

    // Add the Cartesian 2D  index of the main hexagonal Cartesian 2D  cart2D_grid_index to the cart2D_grid_cells_data_list entry with index cart2D_grid_cell_index
    bool add_cart2D_grid_cell_index(int cart2D_grid_cell_index, cart2D_surface_index_data_type cart2D_grid_index) {
        if (cart2D_grid_cell_index < 0 || cart2D_grid_cell_index >= cart2D_grid_cells_data_list.size()) {
//printf("cart2D_grid_class :: add_cart2D_grid_cell_index %i : %i 000\n", cart2D_grid_cell_index, cart2D_grid_cells_data_list.size());
            return false;
        }
//printf("cart2D_grid_class :: add_cart2D_grid_cell_index %i : %i 1111\n", cart2D_grid_cell_index, cart2D_grid_cells_data_list.size());
        glm::vec2 cart2D_cell_cart_coord = HGBC::get_cart2D_surface_world_cartesian_coordinate(cart2D_grid_index);
//printf("cart2D_grid_class :: add_cart2D_grid_cell_index %i : %i 2222\n", cart2D_grid_cell_index, cart2D_grid_cells_data_list.size());
        cart2D_grid_cells_data_list[cart2D_grid_cell_index].add_cart2D_grid_cell(cart2D_grid_index, cart2D_cell_cart_coord.x, cart2D_cell_cart_coord.y);
//printf("cart2D_grid_class :: add_cart2D_grid_cell_index %i : %i 3333\n", cart2D_grid_cell_index, cart2D_grid_cells_data_list.size());
        return true;
    }

    // Add the Cartesian 2D  index of the main hexagonal Cartesian 2D  cart2D_grid_index to the cart2D_grid_cells_data_list entry with a id number of  cart2D_grid_cell_data_id
    bool add_cart2D_grid_cell_id(int cart2D_grid_cell_data_id, cart2D_surface_index_data_type cart2D_grid_index) {
        int cart2D_grid_cell_index = get_cart2D_grid_cell_data_index(cart2D_grid_cell_data_id);
        if (cart2D_grid_cell_index < 0) { return false; }

        return (add_cart2D_grid_cell_index(cart2D_grid_cell_index, cart2D_grid_index));
    }

    bool define_cart2D_grid_cell_coordinate() {

    }

    // Sub grid Test functions
    /*
    void logical_test_cart2D_sub_grids() {
//printf("test_cart2D_sub_grids::display 00000 \n");

        if (HGBC::cart2D_sub_grids.size() > 1) {
// printf("test_cart2D_sub_grids::display 11111 \n");
            for (size_t i = 0; i < HGBC::cart2D_sub_grids.size(); i++) {
                for (size_t j = 0; j < HGBC::cart2D_sub_grids.size(); j++) {
                    if (i!=j) {
//printf("test_cart2D_sub_grids::display 22222 %i : %i  \n",i,j);
                        cart2D_sub_grid_struct_type cart2D_sub_grid01 = HGBC::cart2D_sub_grids[i];
                        cart2D_sub_grid_struct_type cart2D_sub_grid02 = HGBC::cart2D_sub_grids[j];

                        if (cart2D_sub_grids_overlap(cart2D_sub_grid01, cart2D_sub_grid02)) {
                            std::string info = "test_cart2D_sub_grids :: Hex sub grid : " + std::to_string(cart2D_sub_grid01.cart2D_grid_range_id) + " overlaps with Hex sub grid : " + std::to_string(cart2D_sub_grid02.cart2D_grid_range_id) + "\n";
                            afw_globalc::get_current_logger()->log(LogLevel::INFO, info);
                        }

                        if (cart2D_sub_grids_neighbours(cart2D_sub_grid01, cart2D_sub_grid02)) {
                            std::string info = "test_cart2D_sub_grids :: Hex sub grid : " + std::to_string(cart2D_sub_grid01.cart2D_grid_range_id) + " is neighbours with Hex sub grid : " + std::to_string(cart2D_sub_grid02.cart2D_grid_range_id) + "\n";
                            afw_globalc::get_current_logger()->log(LogLevel::INFO, info);
                        }

                    }
                }
            }

        }
    }

    bool operation_testcart2D_sub_grids() {
        if (!tested) {
            //if (HGBC::cart2D_sub_grids.size() > 1) {
            //    cart2D_sub_grid_struct_type merged_cart2D_sub_grid;

            //    if (!merge_cart2D_sub_grids(HGBC::cart2D_sub_grids[0], HGBC::cart2D_sub_grids[1], merged_cart2D_sub_grid)) return false;

            //    HGBC::create_cart2D_sub_grid(merged_cart2D_sub_grid.lower_range, merged_cart2D_sub_grid.upper_range);
            //    HGBC::define_sub_grid_plot_data(HGBC::cart2D_sub_grids[HGBC::cart2D_sub_grids.size() - 1]);
            //    tested = true;
            //}

            if (HGBC::cart2D_sub_grids.size() > 0) {
                cart2D_sub_grid_struct_type test_cart2D_sub_grid1, test_cart2D_sub_grid2;

                if (!cart2D_sub_grid_partition_x(HGBC::cart2D_sub_grids[0], { 10,10 }, test_cart2D_sub_grid1)) { return false; }

                HGBC::define_sub_grid_plot_data(HGBC::cart2D_sub_grids[0]);

                HGBC::create_cart2D_sub_grid(test_cart2D_sub_grid1.lower_range, test_cart2D_sub_grid1.upper_range);
                HGBC::define_sub_grid_plot_data(HGBC::cart2D_sub_grids[HGBC::cart2D_sub_grids.size() - 1]);

                if (!cart2D_sub_grid_partition_y(HGBC::cart2D_sub_grids[0], { 10,10 }, test_cart2D_sub_grid2)) { return false; }

                HGBC::define_sub_grid_plot_data(HGBC::cart2D_sub_grids[0]);

                HGBC::create_cart2D_sub_grid(test_cart2D_sub_grid2.lower_range, test_cart2D_sub_grid2.upper_range);
                HGBC::define_sub_grid_plot_data(HGBC::cart2D_sub_grids[HGBC::cart2D_sub_grids.size() - 1]);

                tested = true;
            }
        }

        return true;
    }
    */

    // Update the main hexagonal grid color data with a value that corresponds to the color of the current Cartesian 2D  gradient display for that value
    // If the data value is out of range and beyond the bounds of the defied gradient, assign the min or max color value depending if the value is 
    // below or above the gradient min max display value
    void update_cart2D_grid_colors() {
        for (size_t i = 0; i < HGBC::cart2D_colors.size(); i++) {
            if ((float)HGBC::cart2D_grid[i] < (float)state.min_grad_value || (float)HGBC::cart2D_grid[i] > (float)state.max_grad_value) {
                ImU32 cart2D_grid_edit_color = ImGui::GetColorU32(HGBC::cart2D_grid_display_color);
                HGBC::cart2D_colors[i] = cart2D_grid_edit_color;
            }
            else {
                //float grid_value_pos = (float)HGBC::cart2D_grid[i] / ((float)state.max_grad_value - (float)state.min_grad_value);
                float grid_value_pos = ((float)HGBC::cart2D_grid[i] - (float)state.min_grad_value) / ((float)state.max_grad_value - (float)state.min_grad_value);
                std::array<float, 4> c = state.GetColorAndIntensity(grid_value_pos);
                ImU32 cart2D_grid_edit_color = ImGui::GetColorU32({ c[0], c[1], c[2],c[3] });
                HGBC::cart2D_colors[i] = cart2D_grid_edit_color;
            }
        }
    }

    // Edit the main Cartesian 2D  data with the Cartesian 2D  brush data of edit_brush_grid
    // x_coord,y_coord is the x-y coordinate index of the main Cartesian 2D  that is to be referencd for editing
    // b_x_coord,b_y_coord  is the x-y index coordinate index of the Cartesian 2D  brush that is to be referencd for changing the main Cartesian 2D  to.
    // If Cartesian 2D  brush cell at x-y index coordinate  b_x_coord,b_y_coord has a value greater than 0, then the current main Cartesian 2D  at  x-y index coordinate  x_coord,y_coord is changed
    void draw_cart2D_grid_brush(cart2D_grid_class<T>* edit_brush_grid, cart2D_surface_index_data_type x_coord, cart2D_surface_index_data_type y_coord, cart2D_surface_index_data_type b_x_coord, cart2D_surface_index_data_type b_y_coord) {
        cart2D_surface_index_data_type grid_index = HGBC::get_cart2D_surface_matrix_data_index({ x_coord ,y_coord });               // Get main Cartesian 2D  cell index that corresponds to the main Cartesian 2D  x-y index coordinate 
        cart2D_surface_index_data_type brush_index = edit_brush_grid->get_cart2D_surface_matrix_data_index({ b_x_coord ,b_y_coord });// Get hex brush grid cell index that corresponds to the hex brush grid cell x-y index coordinate 
//printf("edit_grid:draw_cart2D_grid_brush brush 3333 : %i : brush %i\n", grid_index, brush_index);
        if (grid_index > -1 && grid_index < HGBC::cart2D_grid.size()) { // check that retrieved main Cartesian 2D  cell index is in the bounds of the current Cartesian 2D  array size
            if (brush_index > -1 && brush_index < edit_brush_grid->cart2D_grid.size()) {// check that retrieved Cartesian 2D  brush cell index is in the bounds of the current Cartesian 2D  brush array size
//printf("edit_grid:draw_cart2D_grid_brush brush 33AA : brush value :%i\n", edit_brush_grid->cart2D_grid[brush_index]);
                if (edit_brush_grid->cart2D_grid[brush_index] > 0) {// If the Cartesian 2D  brush cell of index brush_index has a non zero value then edit main Cartesian 2D  cell data and color values 
                    update_cart2D_grid_brush_colors(grid_index);
                }
            }
        }
    }

    // Change the current main Cartesian 2D  cell value with an array index grid_index to the current cart2D_grid_edit_value
    // and update the color data with the color of the colour gradient that corresponds to the cart2D_grid_edit_value
    void update_cart2D_grid_brush_colors(cart2D_surface_index_data_type grid_index) {
        HGBC::cart2D_grid[grid_index] = cart2D_grid_edit_value; // Change Cartesian 2D  value
        float grid_value_pos = cart2D_grid_edit_value / ((float)state.max_grad_value - (float)state.min_grad_value); // get position in the gradient that corresponds to cart2D_grid_edit_value
        std::array<float, 4> c = state.GetColorAndIntensity(grid_value_pos);// get color data as a color array  from the gradient that is at position grid_value_pos in the gradient 
        ImU32 cart2D_grid_edit_color = ImGui::GetColorU32({ c[0], c[1], c[2],c[3] }); // convert color array data to an ImGui color U32 format
        HGBC::cart2D_colors[grid_index] = cart2D_grid_edit_color; // Update Cartesian 2D  cell color data 
    }

    //    bool test_cart2D_grid_cell_list() {
    ////printf("cart2D_grid_class :: test_cart2D_grid_cell_list 00\n");
    //
    //        if (!tested2) {
    //printf("cart2D_grid_class :: test_cart2D_grid_cell_list 11\n");
    //            int id1 = define_new_cart2D_grid_cell_data();
    //            int id2 = define_new_cart2D_grid_cell_data();
    //
    //            int index1 = get_cart2D_grid_cell_data_index(id1);
    //            int index2 = get_cart2D_grid_cell_data_index(id2);
    //
    //printf("cart2D_grid_class :: test_cart2D_grid_cell_list 22 %i : %i : %i : %i : %i\n", id1, id2, index1, index2, cart2D_grid_cells_data_list.size());
    //            if (cart2D_grid_cells_data_list.size() > 0) {
    //
    //                for (int i = 0; i < 10; i++) {
    //                    add_cart2D_grid_cell_index(index1, i);
    //                    add_cart2D_grid_cell_index(index2, i+200);
    //                }
    //            }
    //
    //            cart2D_grid_cells_data_list[index1].cart2D_grid_cells_display_color = { 1.0f,0.0f,0.0f,1.0f };
    //            cart2D_grid_cells_data_list[index2].cart2D_grid_cells_display_color = { 0.0f,1.0f,0.0f,1.0f };
    //
    //            tested2 = true;
    //
    //            for (cart2D_grid_cell_data_struct_type &cart2D_grid_cells_data : cart2D_grid_cells_data_list) {
    //               cart2D_grid_cells_data.display_data = true;
    //            }
    //        }
    //
    //        return true;
    //    }

protected:
    // Main ImGui window widget or panel that displays and manages the widgets used as inputs 
    // for user interaction to query and edit the main hexagonal grid data values and its display 
    void display_grid_parameters_panel() {
        ImGui::Begin("Hex Automata grid display parameters");
        {
            //###### interactively change main Cartesian 2D  display color of Cartesian 2D  cell locations ######
            ImGui::Text("Color : ");
            ImGui::SameLine();
            if (ImGui::ColorEdit4("##hgc", (float*)&cart2D_grid_display_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_None)) {
                HGBC::cart2D_grid_display_color = cart2D_grid_display_color;
            }
            //ImGui::ColorEdit4("##hgc", (float*)HGBC::cart2D_grid_display_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_None); //This generates compilor error
            ImGui::SameLine();
            // interactively change main Cartesian 2D  display shape of Cartesian 2D  cell locations
            ImGui::Text("Shape : ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(60);
            const char* combo_preview_value = shapes[cart2D_grid_display_shape_id];
            if (ImGui::BeginCombo("##hgsh", combo_preview_value, shapes_combo_flags))
            {
                for (int n = 0; n < IM_COUNTOF(shapes); n++)
                {
                    const bool is_selected = (cart2D_grid_display_shape_id == n);
                    if (ImGui::Selectable(shapes[n], is_selected)) {
                        cart2D_grid_display_shape_id = n;
                        define_imgui_shape();
                    }

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected) { ImGui::SetItemDefaultFocus(); }
                }
                ImGui::EndCombo();
            }
            ImGui::SameLine();
            // interactively change main Cartesian 2D  display shape size of Cartesian 2D  cell locations
            ImGui::Text("Size : ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(60);
            ImGui::DragFloat("##hgsi", &cart2D_grid_display_shape_size, 0.1f, 1.0f, 50.0f, "%2.2f");
            ImGui::SameLine();
            // interactively change main Cartesian 2D  display of Cartesian 2D  cell locations on or off
            ImGui::Text("Display : ");
            ImGui::SameLine();
            ImGui::Checkbox("##hgad", &display_acart2D_grid);
            ImGui::Separator();
            // interactively change the range of main Cartesian 2D  cells displayed
            ImGui::Text("Cartesian 2D  Extents Displayed");
            // interactively change x axis display limits
            ImGui::Text("Min X: ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(60);

            if (ImGui::DragFloat("##hgmnx", &plot_min_x, 1.0f, -1.0f, 1000.0f, "%3.1f")) {
                plot_axis_limit_changed = IMGUI_WINDOW_BORDER_LEFT;
            }
            ImGui::SameLine();
            ImGui::Text("Max X: ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(60);

            if (ImGui::DragFloat("##hgmxx", &plot_max_x, 1.0f, -1.0f, 1000.0f, "%3.1f")) {
                plot_axis_limit_changed = IMGUI_WINDOW_BORDER_RIGHT;
            }
            ImGui::SameLine();
            // interactively change y axis display limits
            ImGui::Text("Min Y: ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(60);

            if (ImGui::DragFloat("##hgmny", &plot_min_y, 1.0f, -1.0f, 1000.0f, "%3.1f")) {
                plot_axis_limit_changed = IMGUI_WINDOW_BORDER_LEFT;
            }
            ImGui::SameLine();
            ImGui::Text("Max Y: ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(60);

            if (ImGui::DragFloat("##hgmxy", &plot_max_y, 1.0f, -1.0f, 1000.0f, "%3.1f")) {
                plot_axis_limit_changed = IMGUI_WINDOW_BORDER_RIGHT;
            }

            ImGui::Separator();

            // ################# Cartesian 2D  Value Display Color Gradient #####################
            // Inputs to define gradient min max values to be represented as the min max color markers
            ImGui::Text("         Grid Value display Gradient limits");
            ImGui::Text("           Min                       Max");
            ImGui::SetNextItemWidth(200);
            ImGui::InputInt("##hgmngv", &state.min_grad_value);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(200);
            ImGui::InputInt("##hgmxgv", &state.max_grad_value);// Had a crash from this point after pressing return !!!!!!

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
                ImGui::DragFloat("Intensity", &cm->Intensity, 0.1f, 0.0f, 100.0f, "%f", 1.0f);
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

            // Widget button to update main Cartesian 2D  color data to that of the color gradient
            // after changes or loading of gradient file
            ImGui::SetCursorPosX(140);
            if (ImGui::Button("Update Cartesian 2D  Colors")) {
                update_cart2D_grid_colors();
            }

            // Widget to activate/deactivate the hex edit window flag variable
            // and display or not display the panel to edit Cartesian grid cell data values
            ImGui::Separator();
            ImGui::Text("Edit Cartesian 2D grid: ");
            ImGui::SameLine();
            ImGui::Checkbox("##hged", &edit_acart2D_grid);

        }
        ImGui::End();
    }

    // ImGui window widget or panel that displays and manages the widgets used as inputs 
    // for user interaction to query and edit the main hexagonal grid cell data values
    // Only active and displayed if user has selected the edit Cartesian 2D grid checkbox widget into active mode
    void display_grid_edit_panel() {
        ImGui::Begin("Edit Hex Automata Grid"); {

            // ###### This section of code is relevant to the main Hexagonal Automata grid        ###########
            // ###### display window and if the mouse cusor is within it to perform editing tasks ###########
            // Test if the mouse cursor is within the bounds of the main Cartesian 2D  display window and if it is
            // test for any valid user interactions with the Cartesian 2D  data and perform tasks according to what
            // keyboard or mouse interaction are initiated.
            ImGuiContext* cec = ImGui::GetCurrentContext();// Get current Imgui context of what is happening 
            bool edit_mode = true; // default mode of interaction 

            if (cec && cec->HoveredWindow) {// Have mouse cursor hovering within an ImGui window
                if (cec->HoveredWindow->ID == plot_window->ID) { // If the mouse cursor is hovering over the window that the Cartesian 2D  is being displayed in
                    if (ImGui::GetIO().KeyShift) { // Enable plot inputs and window interaction when shift key is pressed in edit mode and disable edit functions
                        plot_flags = ImPlotFlags_Equal | ImPlotFlags_NoMenus | ImPlotFlags_Crosshairs | ImPlotFlags_NoLegend;
                        plot_window_flags = ImGuiWindowFlags_None;
                        edit_mode = false;
                    }
                    else {// disable plot inputs and window interaction when shift key not pressed in edit mode and enable edit functions
                        plot_flags = ImPlotFlags_Equal | ImPlotFlags_NoMenus | ImPlotFlags_Crosshairs | ImPlotFlags_NoLegend | ImPlotFlags_NoInputs;
                        plot_window_flags = ImGuiWindowFlags_NoMove;
                    }

//printf("ImGui::GetCurrentContext().CurrentWindow.ID!= ImGui::GetCurrentContext().CurrentWindow.GetID(plot_window_id.c_str())\n");
//printf("current context %i : %i\n", ImGui::GetCurrentContext()->HoveredWindow->ID, plot_window->ID);
                    // If mouse cursor is within the bounds of the Cartesian 2D  display then can perform Cartesian 2D  edit functions
                    if (mouse_plot_pos.x > plot_min_x && mouse_plot_pos.x < plot_max_x && mouse_plot_pos.y > plot_min_y && mouse_plot_pos.y < plot_max_y) {
                        // Mouse cursor position is given in Cartesian coordinates of the Cartesian 2D  ImPlot being displayed, so need to convert mouse Cartesian
                        // coordinates to Cartesian 2D  index and Cartesian 2D  x-y index coordinates to be able to edit the Cartesian 2D  cell data values and retrieve them for display
                        cart2D_grid_index = HGBC::index_of_cart2D_cell_with_cartesian_coord(mouse_plot_pos.x, mouse_plot_pos.y);
                        cart2D_grid_coord = HGBC::get_matrix_coordinate(cart2D_grid_index);

                        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {// Left mouse click interaction
//printf("edit_grid::mouse button left : %i\n", cart2D_grid_index);
                            if (cart2D_grid_index > -1 && cart2D_grid_index < HGBC::cart2D_grid.size()) {
                                if (use_cart2D_grid_brush && cart2D_grid_brush.cart2D_grid_brush_selection.second) { // Using a Cartesian 2D  brush to edit Cartesian grid cell data values and have a valid Cartesian 2D  brush selected
//printf("edit_grid:display_grid_edit_panel brush 0000 : %i\n", cart2D_grid_index);
                                    // cart2D_grid_brush_edit();
                                    cart2D_grid_class<T>* edit_brush_grid = cart2D_grid_brush.cart2D_grid_brush_selection.second;// Get the current selected Cartesian 2D  brush data
                                    if (!edit_brush_grid) { // Test have a valid Cartesian 2D  brush selected
                                        return;
                                    }
//printf("edit_grid:display_grid_edit_panel brush 1111 : %i\n", cart2D_grid_index);
                                    // Define hex brush center in x-y index coordinate and calculate hex brush grid extents from that center location
                                    cart2D_surface_vec_data_type brush_dim = edit_brush_grid->grid_dimension;
                                    cart2D_surface_vec_data_type brush_center = { 0, 0 };

                                    // Define Brush center and +,- extents
                                    if (brush_dim.y % 2 == 1) {// odd row y center
                                        brush_center.y = (int)(brush_dim.y / 2);
                                    }
                                    else {// even row y center
                                        brush_center.y = (int)(brush_dim.y / 2) - 1;
                                    }

                                    if ((brush_dim.x) % 2 == 1) { // odd column center
                                        brush_center.x = (int)(brush_dim.x / 2);
                                    }
                                    else {
                                        brush_center.x = (int)(brush_dim.x / 2) - 1;
                                    }

                                    cart2D_surface_vec_data_type cart2D_grid_brush_origin = { cart2D_grid_coord.x - brush_center.x ,cart2D_grid_coord.y - brush_center.y }; // main Cartesian 2D  x-y index coordinate where Cartesian 2D  brush (0,0) origin index lies
//printf("edit_grid:display_grid_edit_panel brush 2222 : %i : %i : origin %i : %i \n", brush_center.x, brush_center.y, cart2D_grid_brush_origin.x, cart2D_grid_brush_origin.y);
                                    // Iterate through each hex brush grid x-y index coordinate and corresponding overlapped main Cartesian 2D  x-y index coordinate
                                    // and perform the edit the main Cartesian 2D  cell data value and colors function
                                    for (int b_y_coord = 0; b_y_coord < brush_dim.y; b_y_coord++) {
                                        cart2D_surface_index_data_type y_coord = cart2D_grid_brush_origin.y + b_y_coord; // main Cartesian 2D  y index coordinate that corresonds to the Cartesian 2D  brush y index coordinate
                                        if (y_coord >= 0 && y_coord < HGBC::grid_dimension.y) {// brush element is within the Cartesian 2D  bounds
                                            for (int b_x_coord = 0; b_x_coord < brush_dim.x; b_x_coord++) {
                                                cart2D_surface_index_data_type x_coord = cart2D_grid_brush_origin.x + b_x_coord;// main Cartesian 2D  x index coordinate that corresonds to the Cartesian 2D  brush x index coordinate
                                                if ((y_coord % 2 == 0 && x_coord >= 0 && x_coord < HGBC::grid_dimension.x) || ((y_coord % 2 == 1 && x_coord >= 0 && x_coord < HGBC::grid_dimension.x - 1))) {// brush element is within the Cartesian 2D  bounds
                                                    draw_cart2D_grid_brush(edit_brush_grid, x_coord, y_coord, b_x_coord, b_y_coord);// Edit main Cartesian 2D  cell data and color value according to hex brush cell data value 
                                                }
                                            }
                                        }
                                    }
                                }
                                else {
                                    update_cart2D_grid_brush_colors(cart2D_grid_index);// Edit the hovered main Cartesian 2D  cell data and color value
                                }
//printf("edit_grid::mouse button left :index %i | value %i ::color | %0.3f| %0.3f| %0.3f| %0.3f\n", cart2D_grid_index, HGBC::cart2D_grid[cart2D_grid_index], c[0], c[1], c[2], c[3]);
                            }
                        }

                        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
//printf("edit_grid::mouse button right : %i\n", cart2D_grid_index);
                        }
                    }
                }
                else {// Enable plot inputs and window interaction when hex plot edit window is not hovered over in edit mode and disable edit functions
                    plot_flags = ImPlotFlags_Equal | ImPlotFlags_NoMenus | ImPlotFlags_Crosshairs | ImPlotFlags_NoLegend;
                    plot_window_flags = ImGuiWindowFlags_None;
                    edit_mode = false;
                }
            }
            // ##################################################################################################

            // ###### This section of code is relevant to the display of the Hexagonal Automata grid edit ###########
            // ###### widgets  to define the Cartesian 2D  edit inputs and perform Cartesian 2D  edit processes     ###########
            // Display the current Cartesian 2D  data and coordinate values that the mouse cursor is hovering over in the
            // main hexagonal automata grid display window
            ImGui::Text("Cartesian Grid Coordinate ::   ");
            ImGui::SameLine();
            ImGui::Text("X : %i | ", cart2D_grid_coord.x);
            ImGui::SameLine();
            ImGui::Text("Y : %i", cart2D_grid_coord.y);

            ImGui::Text("Cart 2D Grid Index : %i  ||", cart2D_grid_index);
            ImGui::SameLine();

            if (cart2D_grid_index > -1 && cart2D_grid_index < HGBC::cart2D_grid.size()) {
                cart2D_grid_value = HGBC::cart2D_grid[cart2D_grid_index];
            }

            ImGui::Text(" Cartesian Grid value : %i", cart2D_grid_value); // Need to accomidate all data types of Cartesian 2D  here !!!!
            ImGui::Separator();

            display_random_cart2D_generator(); // Display the random engine window widgets to create a Cartesian 2D  of random generated numbers

            ImGui::Separator();
            // Display ImGui widgets to enter value to edit/change hovered Cartesian 2D  cell to either as an individual Cartesian 2D  cell or with a Cartesian 2D  brush
            ImGui::Text("Edit Cart 2D Automata Grid Values");
            ImGui::Text("Value : ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(60);
            // interactively change result display shape size
            ImGui::DragFloat("##hgsi", &cart2D_grid_edit_value, 0.1f, 1.0f, 50.0f, "%2.2f");  // Need to accomidate all data types of Cartesian 2D  here !!!!
            ImGui::SameLine();
            ImGui::Checkbox("Use Cart 2D Grid Brush", &use_cart2D_grid_brush);// flag to use a Cartesian 2D  brush to perform edits

            if (use_cart2D_grid_brush) {// If use Cartesian 2D  brush
                cart2D_grid_brush.display_cart2D_grid_brush_panel();// display Cartesian 2D  brush window widget panel
            }

            // Widget inputs to create a border of a given value at the edge of the current Cartesian 2D 
            ImGui::Separator();
            ImGui::Text("Cartesian 2D Grid Border");
            ImGui::Text("Border Value: ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(60);
            ImGui::DragFloat("##hgbv", &cart2D_grid_edit_border_value, 0.1f, 1.0f, 50.0f, "%2.2f");
            ImGui::SameLine();
            if (ImGui::Button("Add Cart2D Grid Border")) {
                add_cart2D_grid_border();
            }
        }
        ImGui::End();
    }

    void  add_cart2D_grid_border() {
        // Bottom border
        for (size_t i = 0; i < HGBC::grid_dimension.x; i++) {
            cart2D_surface_index_data_type index = HGBC::get_cart2D_surface_matrix_data_index({ i,0 });
            HGBC::cart2D_grid[index] = cart2D_grid_edit_border_value;
        }

        // top border
        if (HGBC::grid_dimension.y % 2 == 0) {// even y dimension
            for (size_t i = 0; i < HGBC::grid_dimension.x; i++) {
                cart2D_surface_index_data_type index = HGBC::get_cart2D_surface_matrix_data_index({ i,HGBC::grid_dimension.y - 1 });
                HGBC::cart2D_grid[index] = cart2D_grid_edit_border_value;
            }
        }
        else { // odd y dimension
            for (size_t i = 0; i < HGBC::grid_dimension.x - 1; i++) {
                cart2D_surface_index_data_type index = HGBC::get_cart2D_surface_matrix_data_index({ i,HGBC::grid_dimension.y - 1 });
                HGBC::cart2D_grid[index] = cart2D_grid_edit_border_value;
            }
        }

        // left border
        for (size_t j = 0; j < HGBC::grid_dimension.y; j++) {
            cart2D_surface_index_data_type index = HGBC::get_cart2D_surface_matrix_data_index({ 0,j });
            HGBC::cart2D_grid[index] = cart2D_grid_edit_border_value;
        }

        // right border
        for (size_t j = 0; j < HGBC::grid_dimension.y; j++) {
            if (j % 2 == 0) {// even row
                cart2D_surface_index_data_type index = HGBC::get_cart2D_surface_matrix_data_index({ HGBC::grid_dimension.x - 1,j });
                HGBC::cart2D_grid[index] = cart2D_grid_edit_border_value;
            }
            else { // odd row
                cart2D_surface_index_data_type index = HGBC::get_cart2D_surface_matrix_data_index({ HGBC::grid_dimension.x - 2,j });
                HGBC::cart2D_grid[index] = cart2D_grid_edit_border_value;
            }
        }
    }

    void save_gradienthdr_data() {
//printf("cart2D_grid:save_gradienthdr_data000\n");
        char const* patterns[] = { "*.ghd" };
        char const* file_pathname = vwDialogs::save_file(gradient_dir_path.c_str(), patterns, 1);
//printf("cart2D_grid:save_gradienthdr_data1111\n");
        if (file_pathname == nullptr) {
            afw_globalc::get_current_logger()->log(LogLevel::INFO, "ERROR : No gradient file defined to save gradient data to\n");
            return;
        }
//printf("cart2D_grid:save_gradienthdr_data222\n");
        export_gradient(file_pathname, state);
    }

    void load_gradienthdr() {
//printf("cart2D_grid:load_gradienthdr_data000\n");
        char const* patterns[] = { "*.ghd" };
        char const* file_pathname = vwDialogs::open_file(gradient_dir_path.c_str(), patterns, 1);
//printf("cart2D_grid:load_gradienthdr_data111\n");
        if (file_pathname == nullptr) {
            afw_globalc::get_current_logger()->log(LogLevel::INFO, "ERROR : No gradient file defined to import gradient data to\n");
            return;
        }
//printf("cart2D_grid:load_gradienthdr_data222\n");
        import_gradient(file_pathname, state);
//printf("cart2D_grid:load_gradienthdr_data333\n");
        update_cart2D_grid_colors();
    }

    // Display the random number generator and distribution ImGui widgets to select the random number generator
    // and number ditribution function and enter the parameter data required to generate a random number and
    // generate the random number.
    void display_random_cart2D_generator() {
        random_engine.display_generation_inputs();

        ImGui::SetCursorPosX(100);
        if (ImGui::Button("Generate Random Cartesian 2D ")) {// button widget to activate the creation of a random number
            random_engine.define_random_engine(); // Define the random engine input data to generate a randome number

// Following testing only: Comment out or delete when finished
//for (int i = 0; i < 20; i++) {
//    random_engine.uniform_int_distribution.generate_random_number();
    //printf("random_engine_class : generate_random_value gen_number : %i\n", uniform_int_distribution.generated_number_value.integer_p);
//    printf("cart2D_grid_class : display_random_cart2D_generator random number : %i\n", random_engine.uniform_int_distribution.generated_number_value);
//}

            generate_random_cart2D_grid();
            update_cart2D_grid_colors();
        }

    }

    // CRITICAL : The random_distribution_index assignment must match the index of the random_distributions array list that
    //            the user has selected in the ImGui combo widget of random number distributions.
    // Generate a random number for every Cartesian 2D  cell based upon the selections made in the random engine GUI
    // One function exists for each possible number distribution type.
    void generate_random_cart2D_grid() {
        switch (random_engine.random_distribution_index) {
        case 0: { uniform_int_distribution_cart2D_grid_random_generation(); break; }
        case 1: { uniform_real_distribution_cart2D_grid_random_generation(); break; }
        case 2: { bernoulli_distribution_cart2D_grid_random_generation(); break; }
        case 3: { binomial_distribution_cart2D_grid_random_generation(); break; }
        case 4: { geometric_distribution_cart2D_grid_random_generation(); break; }
        case 5: { negative_binomial_distribution_cart2D_grid_random_generation(); break; }
        case 6: { poisson_distribution_cart2D_grid_random_generation(); break; }
        case 7: { exponential_distribution_cart2D_grid_random_generation(); break; }
        case 8: { gamma_distribution_cart2D_grid_random_generation(); break; }
        case 9: { weibull_distribution_cart2D_grid_random_generation(); break; }
        case 10: { extreme_value_distribution_cart2D_grid_random_generation(); break; }
        case 11: { normal_distribution_cart2D_grid_random_generation(); break; }
        case 12: { lognormal_distribution_cart2D_grid_random_generation(); break; }
        case 13: { chi_squared_distribution_cart2D_grid_random_generation(); break; }
        case 14: { cauchy_distribution_cart2D_grid_random_generation(); break; }
        case 15: { fisher_f_distribution_cart2D_grid_random_generation(); break; }
        case 16: { student_t_distribution_cart2D_grid_random_generation(); break; }
        }
    }


    // ##################################################################
    // ## Functions to generate a random number for each Cartesian 2D  cell ##
    // ## One function is defined for each number ditribution type     ##

    // NOTE : The
    // if (random_engine.rng_add_to_existing_grid) {
    //      rn = HGBC::cart2D_grid[i] + rn;
    // }
    // to add random distribution numbers to the existing grid may not
    // be appropriate for all random distributons and has only been tested
    // for the bernoulli distribution. Use With Caution.

    void uniform_int_distribution_cart2D_grid_random_generation() {
        for (size_t i = 0; i < HGBC::cart2D_grid.size(); i++) {
            random_engine.uniform_int_distribution.generate_random_number();

            T rn = (T)random_engine.uniform_int_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::cart2D_grid[i] + rn;
            }

            HGBC::cart2D_grid[i] = rn;

            // following for testing only commant out or delete when no longer used!!!!!!
//printf("cart2D_grid_class : uniform_int_distribution_cart2D_grid_random_generation random number : %i\n", random_engine.uniform_int_distribution.generated_number_value);
        }
    }

    void uniform_real_distribution_cart2D_grid_random_generation() {
        for (size_t i = 0; i < HGBC::cart2D_grid.size(); i++) {
            random_engine.uniform_real_distribution.generate_random_number();

            T rn = (T)random_engine.uniform_real_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::cart2D_grid[i] + rn;
            }

            HGBC::cart2D_grid[i] = rn;
        }
    }
    void bernoulli_distribution_cart2D_grid_random_generation() {
        for (size_t i = 0; i < HGBC::cart2D_grid.size(); i++) {
            random_engine.bernoulli_distribution.generate_random_number();

            T rn = (T)random_engine.bernoulli_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::cart2D_grid[i] + rn;
            }

            HGBC::cart2D_grid[i] = rn;
        }
    }
    void binomial_distribution_cart2D_grid_random_generation() {
        for (size_t i = 0; i < HGBC::cart2D_grid.size(); i++) {
            random_engine.binomial_distribution.generate_random_number();

            T rn = (T)random_engine.binomial_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::cart2D_grid[i] + rn;
            }

            HGBC::cart2D_grid[i] = rn;
        }
    }
    void geometric_distribution_cart2D_grid_random_generation() {
        for (size_t i = 0; i < HGBC::cart2D_grid.size(); i++) {
            random_engine.geometric_distribution.generate_random_number();

            T rn = (T)random_engine.geometric_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::cart2D_grid[i] + rn;
            }

            HGBC::cart2D_grid[i] = rn;
        }
    }
    void negative_binomial_distribution_cart2D_grid_random_generation() {
        for (size_t i = 0; i < HGBC::cart2D_grid.size(); i++) {
            random_engine.negative_binomial_distribution.generate_random_number();

            T rn = (T)random_engine.negative_binomial_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::cart2D_grid[i] + rn;
            }

            HGBC::cart2D_grid[i] = rn;
        }
    }
    void poisson_distribution_cart2D_grid_random_generation() {
        for (size_t i = 0; i < HGBC::cart2D_grid.size(); i++) {
            random_engine.poisson_distribution.generate_random_number();

            T rn = (T)random_engine.poisson_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::cart2D_grid[i] + rn;
            }

            HGBC::cart2D_grid[i] = rn;
        }
    }
    void exponential_distribution_cart2D_grid_random_generation() {
        for (size_t i = 0; i < HGBC::cart2D_grid.size(); i++) {
            random_engine.exponential_distribution.generate_random_number();

            T rn = (T)random_engine.exponential_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::cart2D_grid[i] + rn;
            }

            HGBC::cart2D_grid[i] = rn;
        }
    }
    void gamma_distribution_cart2D_grid_random_generation() {
        for (size_t i = 0; i < HGBC::cart2D_grid.size(); i++) {
            random_engine.gamma_distribution.generate_random_number();

            T rn = (T)random_engine.gamma_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::cart2D_grid[i] + rn;
            }

            HGBC::cart2D_grid[i] = rn;
        }
    }
    void weibull_distribution_cart2D_grid_random_generation() {
        for (size_t i = 0; i < HGBC::cart2D_grid.size(); i++) {
            random_engine.weibull_distribution.generate_random_number();

            T rn = (T)random_engine.weibull_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::cart2D_grid[i] + rn;
            }

            HGBC::cart2D_grid[i] = rn;
        }
    }
    void extreme_value_distribution_cart2D_grid_random_generation() {
        for (size_t i = 0; i < HGBC::cart2D_grid.size(); i++) {
            random_engine.extreme_value_distribution.generate_random_number();

            T rn = (T)random_engine.extreme_value_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::cart2D_grid[i] + rn;
            }

            HGBC::cart2D_grid[i] = rn;
        }
    }
    void normal_distribution_cart2D_grid_random_generation() {
        for (size_t i = 0; i < HGBC::cart2D_grid.size(); i++) {
            random_engine.normal_distribution.generate_random_number();

            T rn = (T)random_engine.normal_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::cart2D_grid[i] + rn;
            }

            HGBC::cart2D_grid[i] = rn;
        }
    }
    void lognormal_distribution_cart2D_grid_random_generation() {
        for (size_t i = 0; i < HGBC::cart2D_grid.size(); i++) {
            random_engine.lognormal_distribution.generate_random_number();

            T rn = (T)random_engine.lognormal_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::cart2D_grid[i] + rn;
            }

            HGBC::cart2D_grid[i] = rn;
        }
    }
    void chi_squared_distribution_cart2D_grid_random_generation() {
        for (size_t i = 0; i < HGBC::cart2D_grid.size(); i++) {
            random_engine.chi_squared_distribution.generate_random_number();

            T rn = (T)random_engine.chi_squared_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::cart2D_grid[i] + rn;
            }

            HGBC::cart2D_grid[i] = rn;
        }
    }
    void cauchy_distribution_cart2D_grid_random_generation() {
        for (size_t i = 0; i < HGBC::cart2D_grid.size(); i++) {
            random_engine.cauchy_distribution.generate_random_number();

            T rn = (T)random_engine.cauchy_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::cart2D_grid[i] + rn;
            }

            HGBC::cart2D_grid[i] = rn;
        }
    }
    void fisher_f_distribution_cart2D_grid_random_generation() {
        for (size_t i = 0; i < HGBC::cart2D_grid.size(); i++) {
            random_engine.fisher_f_distribution.generate_random_number();

            T rn = (T)random_engine.fisher_f_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::cart2D_grid[i] + rn;
            }

            HGBC::cart2D_grid[i] = rn;
        }
    }
    void student_t_distribution_cart2D_grid_random_generation() {
        for (size_t i = 0; i < HGBC::cart2D_grid.size(); i++) {
            random_engine.student_t_distribution.generate_random_number();

            T rn = (T)random_engine.student_t_distribution.generated_number_value;

            if (random_engine.rng_add_to_existing_grid) {
                rn = HGBC::cart2D_grid[i] + rn;
            }

            HGBC::cart2D_grid[i] = rn;
        }
    }

    // ImGui window widget to display the hex automata grid cell data and overlying cart2D_grid_cells_data_list list of 
    // hex sub grid data using the ImPlot::ScatterPlot function. 
    void display_automata_rule_results() {
        ImGui::Begin("Hex Automata", nullptr, plot_window_flags);// If Have name of ImGui::Begin then have ability to minimise and dock 

        if (ImPlot::BeginPlot("Hex Automata Grid", ImVec2(-1, -1), plot_flags)) {

            ImPlotStyle& style = ImPlot::GetStyle();
            ImGuiContext* cpc = ImGui::GetCurrentContext();// Strange compilor does not complain about ImGuiContext without including imgui_internal.h
            // But does complain about ImGuiContext not being defined when using cpc in the next lines.

 // Test and functions to maintain the display aspect ratio of the ImGui-ImPlot window
 // when performing windo resizing using the window borders. This is performed 
 // so as to avoid strange disparity of the display of Cartesian 2D  where the grid X-Y axis
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
                    case IMGUI_WINDOW_BORDER_LEFT:
                    case IMGUI_WINDOW_BORDER_RIGHT: {
                        resize_plot_window_width(cpc, style);
                        border_resized = IMGUI_WINDOW_BORDER_NONE;
                        break;
                    }
                    case IMGUI_WINDOW_BORDER_TOP:
                    case IMGUI_WINDOW_BORDER_BOTTOM: {
                        resize_plot_window_height(cpc, style);
                        border_resized = IMGUI_WINDOW_BORDER_NONE;
                        break;
                    }
                    }
                }
            }

            //if(plot_axis_limit_changed> IMGUI_WINDOW_BORDER_NONE) printf("Axis Extents  %i\n", plot_axis_limit_changed);

            //if (cpc)
            //    printf("display_cart2D_grid : px %f : py %f: x %f : y %f \n", prev_width, prev_height,cpc->CurrentWindow->Size.x, cpc->CurrentWindow->Size.y);

            // Constrain the Cartesian 2D  display to a min max Cartesian coordinate range of coordinates
            // Problem using this is that when panning, unwanted zooming in occurs
            //ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, plot_min_x, plot_max_x);
            //ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, plot_min_y, plot_max_y);

            // Define axis limits of the display
            ImPlot::SetupAxisLimits(ImAxis_X1, plot_min_x, plot_max_x);
            ImPlot::SetupAxisLimits(ImAxis_Y1, plot_min_y, plot_max_y);

//testing only delete/comment out when not needed
            if (HGBC::cart2D_centers_x.empty()) {
                printf("cart2D_grid_class::display_automata_rule_results:: ERROR \n", HGBC::cart2D_centers_x.empty());
            }
            // ImPlot::ScatterPlot function to display main Cartesian 2D  cell data
            if (display_acart2D_grid) {// If display Cartesian 2D  flag is true
                ImPlot::PlotScatter("hex", HGBC::cart2D_centers_x.data(), HGBC::cart2D_centers_y.data(), HGBC::cart2D_centers_x.size(), {
                    ImPlotProp_Marker, cart2D_grid_display_shape,// use this when testing
                    ImPlotProp_MarkerSize, cart2D_grid_display_shape_size,
                    //ImPlotProp_LineColor, cart2D_grid_display_color,
                    //ImPlotProp_FillColor, cart2D_grid_display_color,
                    ImPlotProp_MarkerFillColors, HGBC::cart2D_colors.data(),
                    ImPlotProp_MarkerLineColors, HGBC::cart2D_colors.data(),
                    ImPlotProp_FillAlpha, 0.25f,
                    ImPlotProp_Flags,
                    ImPlotFlags_Equal
                    });
            }

            mouse_plot_pos = ImPlot::GetPlotMousePos(); // This must be placed within the BeginPlot : EndPlot block or application will crash 

            for (cart2D_sub_grid_struct_type cart2D_sub_grid : HGBC::cart2D_sub_grids) {
                cart2D_sub_grid.display_sub_cart2D_grid();
            }

            for (cart2D_grid_cell_data_struct_type cart2D_grid_cell : cart2D_grid_cells_data_list) {
//printf("cart2D_grid_class::display_cart2D_grid 3333 :"); printf(" %i : %i\n", cart2D_grid_cell.cart2D_grid_cell_data_id, cart2D_grid_cell.display_shape_id);
                cart2D_grid_cell.display_cart2D_cell_data();// This is the Cartesian 2D  automata rules display data
            }

            ImPlot::EndPlot();
        }
        plot_axis_limit_changed = IMGUI_WINDOW_BORDER_NONE;

        ImGui::End();
    }


private:
    ImGuiWindow* plot_window = nullptr;
    ImGuiWindowFlags plot_window_flags = ImGuiWindowFlags_None;
    ImPlotFlags plot_flags = ImPlotFlags_Equal | ImPlotFlags_NoMenus | ImPlotFlags_Crosshairs | ImPlotFlags_NoLegend;

    ImPlotPoint mouse_plot_pos = { 0.0f,0.0f };
    cart2D_surface_index_data_type cart2D_grid_index = -1;
    cart2D_surface_vec_data_type cart2D_grid_coord = { 0,0 };
    T cart2D_grid_value = 0;

    bool change_extents_popup = false;

    int cart2D_grid_cell_data_id = -1;

    int tested = false;
    int tested2 = false;

    // Plot resize variables
    float plot_min_x = -1.0f;
    float plot_max_x = 50.0f;
    float plot_min_y = -1.0f;
    float plot_max_y = 50.0f;

    float prev_plot_min_x = -1.0f;
    float prev_plot_max_x = 50.0f;
    float prev_plot_min_y = -1.0f;
    float prev_plot_max_y = 50.0f;

    float plot_min_zoom = 1.0f;
    float plot_max_zoom = 40.0f;

    float prev_width = 300.0f;
    float prev_height = 300.0f;

    float prev_style_width = 0.0f;
    float prev_style_height = 0.0f;

    float new_width = 0.0f;
    float new_height = 0.0f;

    float new_style_width = 0.0f;
    float new_style_height = 0.0f;

    signed char border_resized_held = -1;
    signed char border_resized = -1;

    // Cartesian 2D  display paramters 
    bool display_acart2D_grid = true;
    bool edit_acart2D_grid = false;
    const char* shapes[9] = { "Circle", "Square", "Diamond", "Up", "Down", "Left", "Right", "Cross", "Asterix" };
    ImGuiComboFlags shapes_combo_flags = ImGuiComboFlags_NoArrowButton;
    signed char plot_axis_limit_changed = -1;
    ImVec4 cart2D_grid_display_color = HGBC::cart2D_grid_display_color;
    int cart2D_grid_display_shape_id = 1;
    float cart2D_grid_display_shape_size = 6.0f;
    ImPlotMarker_ cart2D_grid_display_shape = ImPlotMarker_::ImPlotMarker_Circle;

    // Cartesian 2D  value display color gradient variables
    ImGradientHDRState::ColorMarker* cm = nullptr;
    ImGradientHDRState::ColorMarker* selectedColorMarker = nullptr;
    int32_t stateID = 10;
    ImGradientHDRState state;
    ImGradientHDRTemporaryState tempState;
    bool isMarkerShown = true;

    int min_grad_value_i = 0;
    int max_grad_value_i = 100;

    // --------------- Random number generator variables -----------------------------

    random_engine_class random_engine;

    // ---------------- Edit Cartesian 2D Grid Variables ------------------------------
    float cart2D_grid_edit_value = 1.0f;
    float cart2D_grid_edit_border_value = 100.0f;

    // --------------- Cartesian Grid Brush variables ----------------
    bool use_cart2D_grid_brush = false;
    cart2D_grid_brush_class<T> cart2D_grid_brush;


    // ------------------------------------------------------------------------------

    // Plot resize functions
    void set_plot_window_resize_initial(ImGuiContext* cpc, ImPlotStyle& style) {
        prev_width = cpc->CurrentWindow->Size.x;
        prev_height = cpc->CurrentWindow->Size.y;

        prev_style_width = style.PlotDefaultSize.x;
        prev_style_height = style.PlotDefaultSize.y;
    }

    void resize_plot_window_width(ImGuiContext* cpc, ImPlotStyle& style) {
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

    void  resize_plot_window_height(ImGuiContext* cpc, ImPlotStyle& style) {
        float resized_height = cpc->CurrentWindow->Size.y;
        float y_ratio = resized_height / prev_height;

        new_width = (prev_width * y_ratio);
        new_height = resized_height;

        new_style_width = prev_style_width * y_ratio;
        new_style_height = resized_height;

        ImGui::SetWindowSize({ new_width,new_height });

        style.PlotDefaultSize.x = new_style_width;
        style.PlotDefaultSize.y = new_style_height;
    }

    void define_imgui_shape() {// default shapes to display
        switch (cart2D_grid_display_shape_id) {
        case 0:  cart2D_grid_display_shape = ImPlotMarker_::ImPlotMarker_Circle; break;
        case 1:  cart2D_grid_display_shape = ImPlotMarker_::ImPlotMarker_Square; break;
        case 2:  cart2D_grid_display_shape = ImPlotMarker_::ImPlotMarker_Diamond; break;
        case 3:  cart2D_grid_display_shape = ImPlotMarker_::ImPlotMarker_Up; break;
        case 4:  cart2D_grid_display_shape = ImPlotMarker_::ImPlotMarker_Down; break;
        case 5:  cart2D_grid_display_shape = ImPlotMarker_::ImPlotMarker_Left; break;
        case 6:  cart2D_grid_display_shape = ImPlotMarker_::ImPlotMarker_Right; break;
        case 7:  cart2D_grid_display_shape = ImPlotMarker_::ImPlotMarker_Cross; break;
        case 8:  cart2D_grid_display_shape = ImPlotMarker_::ImPlotMarker_Plus; break;
        case 9:  cart2D_grid_display_shape = ImPlotMarker_::ImPlotMarker_Asterisk; break;
        default: cart2D_grid_display_shape = ImPlotMarker_::ImPlotMarker_Circle;
        }
    }
};
