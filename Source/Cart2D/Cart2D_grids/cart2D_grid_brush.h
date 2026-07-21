#pragma once

#include <filesystem>
#include <sys/stat.h>

#include <ThirdParty/ImGui/imgui_docking/imgui_internal.h>
#include <ImGui/imgui_docking/misc/cpp/imgui_stdlib.h>// Need This so as to be able to input text as a std::string variable

#include <FrameWork/AFW/ImGUI/ImGuiEx/imgui_custom.h>
#include <FrameWork/AFW/ImGUI/ImGuiEx/imgui_widgets.h>
#include <FrameWork/AFW/Interface/IconsFontAwesome4.h>
#include <FrameWork/AFW/tools/afw_string_tools.h>

#include "cart2D_surface_import_export.h"

template <typename T>
class cart2D_grid_class; // Need to forward declare this as have a case of circular referencing 

// Cartesian 2D grid brush class to display a user interface to create, store and manage cart2D grid
// brushes that are used to edit the main cart2Dagonal automata grid cell data values. 
// Similar to the maim cart2D_grid_class except does not inherit cart2D_grid_base_class.
// Each cart2D Brush is a cart2D grid class object and are stored as a vector list of these 
// cart2D grid class objects.

template <class T>
class cart2D_grid_brush_class {
    //using HGBC = cart2D_grid_base_class<T>;
public:
    cart2D_grid_brush_class() {}
    ~cart2D_grid_brush_class() {}

    std::string brush_pathname = "Brushes/Cart2d_Brushes";// default pathname to application sub directory 
    std::filesystem::path item_path;

    typedef std::pair < std::string, cart2D_grid_class<T>*> cart2D_grid_brush_list_data_type; // basic cart2D grid brush datatype
    cart2D_grid_brush_list_data_type cart2D_grid_brush_selection;                             // Current selected cart2D grid brush datatype
    std::vector <cart2D_grid_brush_list_data_type > cart2D_grid_brush_selections = {};        // vector list of cart2D grid brushes to select

    cart2D_grid_brush_list_data_type cart2D_grid_brush_edit;                                  // Flag to edit a cart2D grid brush
    cart2D_grid_class<T>* cart2D_grid_brush = nullptr;                                        // pointer to current selected cart2D grid brush

    std::vector<cart2D_grid_cell_data_struct_type> cart2D_grid_cells_data_list = {};          // vector list of cart2D grid brushes diaplay data to display on screen

    // Function to load existing cart2D grid brush data from disc files into a list of
    //  cart2D grid brushes and list of display color shape and size data
    void initialise_cart2D_grid_brushes() {
        //folowing for testing only : comment out or delete when finished
        //cart2D_grid_brush_list_data_type brush0;
        //cart2D_grid_brush_list_data_type brush1;
        //cart2D_grid_brush_list_data_type brush2;
        //cart2D_grid_brush_list_data_type brush3;
        //cart2D_grid_brush_list_data_type brush4;
        //cart2D_grid_brush_list_data_type brush5;

        //brush0.first = "brush0"; brush0.second = nullptr;
        //brush1.first = "brush1"; brush1.second = nullptr;
        //brush2.first = "brush2"; brush2.second = nullptr;
        //brush3.first = "brush3"; brush3.second = nullptr;
        //brush4.first = "brush4"; brush4.second = nullptr;
        //brush5.first = "brush5"; brush5.second = nullptr;

        //cart2D_grid_brush_selections.push_back(brush0);
        //cart2D_grid_brush_selections.push_back(brush1);
        //cart2D_grid_brush_selections.push_back(brush2);
        //cart2D_grid_brush_selections.push_back(brush3);
        //cart2D_grid_brush_selections.push_back(brush4);
        //cart2D_grid_brush_selections.push_back(brush5);


        // following not testing
        // ++++++++++++++++++++++++++++++++++

        // This structure would distinguish a file from a directory
        struct stat sb;

        // Looping until all the items of the directory are exhausted
        for (const auto& entry : std::filesystem::directory_iterator(brush_pathname)) {

            // Converting the path to const char * in the subsequent lines
            std::filesystem::path outfilename = entry.path();
            std::string outfilename_str = outfilename.string();
            const char* path = outfilename_str.c_str();

            // Test whether the path points to a non-directory or not 
            if (stat(path, &sb) == 0 && !(sb.st_mode & S_IFDIR)) { // have a data file
                std::cout << "0 : " << path << std::endl;

                std::string bpn = path;
                // ----------- Import cart2D grid brush data ------------------
 //printf("cart2D_grid_brush_class::initialise_cart2D_grid_brushes 11\n");
                cart2D_grid_class<T>* hgrid_brush = create_cart2D_grid_brush(2, 2, cart2D_grid_value);// Create a dummy cart2D grid brush to import cart2D grid brudh data into
//printf("cart2D_grid_brush_class::initialise_cart2D_grid_brushes 22\n");
                if (!hgrid_brush) {
                    afw_globalc::get_current_logger()->log(LogLevel::INFO, "Create cart2D grid brush:: Unable to define cart2D grid brush" + bpn + "\n");
                    continue;
                }
//printf("cart2D_grid_brush_class::initialise_cart2D_grid_brushes 33\n");
                hgrid_brush->clear_all_cart2D_grid_cell_data();
                hgrid_brush->delete_cart2D_grid();
//printf("cart2D_grid_brush_class::initialise_cart2D_grid_brushes 44\n");
                cart2D_surface_import_export.import_cart2D_surface_automata_grid(bpn, hgrid_brush);
                hgrid_brush->define_cart2D_grid_coordinates();
//printf("cart2D_grid_brush_class::initialise_cart2D_grid_brushes 55\n");
                for (size_t i = 0; i < hgrid_brush->cart2D_colors.size(); i++) {
                    if (hgrid_brush->cart2D_grid[i] > 0)
                        hgrid_brush->cart2D_colors[i] = ImGui::GetColorU32(cart2D_grid_edit_color);
                }

                // -------------- Define grid brush name to displsy in the brush list ---------------------
                FW::stringtools::removeSubstring(bpn, "C2GB.txt");
                size_t pos1 = bpn.find_last_of('\\');
//                std::cout << "1 : " << bpn << " : " << pos1 << std::endl;
                bpn.erase(0, pos1 + 1);
//                std::cout << "2 : " << bpn << std::endl;
                FW::stringtools::replace(bpn, "_", " ");
//                std::cout << "3 : " << bpn << std::endl;

                cart2D_grid_brush_list_data_type new_brush;
                new_brush.first = bpn;
                new_brush.second = hgrid_brush;

                add_brush_to_list(new_brush);

            }// else { // have a directory
                // not to be used but left here for reference
            //}
        }

        // Initialise default cart2D brush slection
        cart2D_grid_brush_edit.first = "null";
        cart2D_grid_brush_edit.second = nullptr;

        if (cart2D_grid_brush_selections.size() > 0) {
            cart2D_grid_brush_selection = cart2D_grid_brush_selections[0];
        }
        else {
            cart2D_grid_brush_selection.first = "null";
            cart2D_grid_brush_selection.second = nullptr;
        }
    }

    bool add_brush_to_list(cart2D_grid_brush_list_data_type cart2D_grid_brush) {
        //Check for cart2D grid brush of same name
        for (size_t i = 0; i < cart2D_grid_brush_selections.size(); i++) {
            if (cart2D_grid_brush.first == cart2D_grid_brush_selections[i].first) {
                vwDialogs::display_error_message("Add Brush ERROR:", "Could not add brush to selection list. \nHave an existing brush of the same name.");
                return false;
            }
        }

        // Check if have brush grid undefined
        if (!cart2D_grid_brush.second) {
            vwDialogs::display_error_message("Add Brush ERROR:", "Could not add brush to selection list. \nBrush to add is undefined.");
            return false;
        }

        cart2D_grid_brush_selections.push_back(cart2D_grid_brush);

        return true;
    }

    bool save_brush_to_list(cart2D_grid_brush_list_data_type& cart2D_grid_brush) {
        // Check to see if have brush of same name
        for (cart2D_grid_brush_list_data_type& cart2D_grid_brush_selection : cart2D_grid_brush_selections) {
            if (cart2D_grid_brush.first == cart2D_grid_brush_selection.first) {// Have brush of the same name : overwrite it
                if (!cart2D_grid_brush.second) {// Check if have brush grid undefined
                    vwDialogs::display_error_message("Save Brush ERROR:", "Could not Save brush to selection list. \nBrush to save is undefined.");
                    return false;
                }
                else {
//printf("cart2D_grid_brush_class::save_brush_to_list AAAA %s %s\n", cart2D_grid_brush_selection.first.c_str(), cart2D_grid_brush.first.c_str());
//if(!cart2D_grid_brush_selection.second) printf("cart2D_grid_brush_class::save_brush_to_list ABABAB !cart2D_grid_brush_selection.second\n");
                    if (cart2D_grid_brush.second != cart2D_grid_brush_selection.second) {
//printf("cart2D_grid_brush_class::save_brush_to_list BBBB\n");
                        delete cart2D_grid_brush_selection.second; // delete cart2D grid data as it is to be replaced and no longer used
//printf("cart2D_grid_brush_class::save_brush_to_list CCCC\n");
                        cart2D_grid_brush_selection.second = cart2D_grid_brush.second;
                    }
//printf("cart2D_grid_brush_class::save_brush_to_list DDDD\n");
                    return true;
                }
            }
        }

        return add_brush_to_list(cart2D_grid_brush);// No brush to overwrite : add brush to list
    }


    // The main ImGui window widget panel to display widgets for user to interact with to select
    // existing cart2D brush to use for editing the main cart2D automata grid, or to create/edit/save/load
    // cart2D brush grid. If the editing of a cart2D brush is activarted, a cart2D brush edit window will
    // appear that operates similarly to the main cart2D grid automata display window widget.
    void display_cart2D_grid_brush_panel() {
        if (!brushes_list_initialised) { // need this or brushes will be added at infenitum every ImGui frame call
            initialise_cart2D_grid_brushes();
            brushes_list_initialised = true;
        }

        // For some unknown reason naming the ImGui::Begin window below other than Hex Grid Brush causes ImPlot::BeginPlot to crash application with error
        // [01120] [imgui-error] (current settings: Assert=1, Log=1, Tooltip=1)
        // [01120][imgui - error] In window 'Cart2D Grid Brush ': Setup needs to be called after BeginPlot and before any setup locking functions(e.g.PlotX)!
        // 
        ImGui::Begin("Hex Grid Brush", nullptr, plot_window_flags);// If Have name of ImGui::Begin then have ability to minimise and dock 
        //ImGui::Begin("Cart2D Grid Brush ", nullptr, plot_window_flags);// If Have name of ImGui::Begin then have ability to minimise and dock 

        // ###### This section of code is relevant to the main Hex brush grid display ###########
        // ###### window and if the mouse cusor is within it to perform editing tasks ###########
        // Test if the mouse cursor is within the bounds of the cart2D brush display window and if it is
        // test for any valid user interactions with the cart2D brush data and perform tasks according to what
        // keyboard or mouse interaction are initiated.
        if (cart2D_grid_brush && display_cart2D_grid_brush_edit) {
            ImGuiContext* cec = ImGui::GetCurrentContext();// Get current Imgui context of what is happening 
            bool edit_mode = true;// default mode of interaction 

            if (cec && cec->HoveredWindow) {// Have mouse cursor hovering within an ImGui window
                if (cec->HoveredWindow->ID == plot_window->ID) { // If the mouse cursor is hovering over the window that the cart2D brush is being displayed in
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
                                        // If mouse cursor is within the bounds of the cart2D brush display then can perform cart2D brush edit functions
                    if (mouse_plot_pos.x > plot_min_x && mouse_plot_pos.x < plot_max_x && mouse_plot_pos.y > plot_min_y && mouse_plot_pos.y < plot_max_y) {
                        // Mouse cursor position is given in Cartesian coordinates of the cart2D brush grid ImPlot being displayed, so need to convert mouse Cartesian
                        // coordinates to cart2D brush grid index and cart2D brush grid x-y index coordinates to be able to edit the cart2D brush grid cell data values and retrieve them for display
                        cart2D_grid_index = cart2D_grid_brush->index_of_cart2D_cell_with_cartesian_coord(mouse_plot_pos.x, mouse_plot_pos.y);
                        cart2D_grid_brush_coord = cart2D_grid_brush->get_matrix_coordinate(cart2D_grid_index);

                        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) { //On left mouse click flip set the cart2D brush call value to 1 and change the cart2D brush grid display data
//printf("edit_grid::mouse button left : %i\n", cart2D_grid_index);
                            if (cart2D_grid_index > -1 && cart2D_grid_index < cart2D_grid_brush->cart2D_grid.size()) {
                                cart2D_grid_brush->cart2D_grid[cart2D_grid_index] = cart2D_grid_edit_value;
                                cart2D_grid_brush->cart2D_colors[cart2D_grid_index] = ImGui::GetColorU32(cart2D_grid_edit_color);
//printf("edit_grid::mouse button left :index %i | value %i ::color | %0.3f| %0.3f| %0.3f| %0.3f\n", cart2D_grid_index, cart2D_grid_brush->cart2D_grid[cart2D_grid_index], c[0], c[1], c[2], c[3]);
                            }

                        }

                        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {//On right mouse click flip set the cart2D brush call value to 0 and change the cart2D brush grid display data
//printf("edit_grid::mouse button left : %i\n", cart2D_grid_index);
                            if (cart2D_grid_index > -1 && cart2D_grid_index < cart2D_grid_brush->cart2D_grid.size()) {
                                cart2D_grid_brush->cart2D_grid[cart2D_grid_index] = 0.0f;
                                cart2D_grid_brush->cart2D_colors[cart2D_grid_index] = ImGui::GetColorU32(cart2D_grid_display_color);

//printf("edit_grid::mouse button left :index %i | value %i ::color | %0.3f| %0.3f| %0.3f| %0.3f\n", cart2D_grid_index, cart2D_grid_brush->cart2D_grid[cart2D_grid_index], c[0], c[1], c[2], c[3]);
                            }

                        }

                        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
//printf("edit_grid::mouse button right : %i\n", cart2D_grid_index);
                        }
                    }
                }
                else {// Enable plot inputs and window interaction when cart2D plot edit window is not hovered over in edit mode and disable edit functions
                    plot_flags = ImPlotFlags_Equal | ImPlotFlags_NoMenus | ImPlotFlags_Crosshairs | ImPlotFlags_NoLegend;
                    plot_window_flags = ImGuiWindowFlags_None;
                    edit_mode = false;
                }
            }
        }

        // ##################################################################################################

        // ###### This section of code is relevant to the display of the Hex brush grid edit widgets ###########
        // ###### to define the cart2D brush grid edit inputs and perform cart2D brush grid edit processes ###########
        // Display the current cart2D brush grid data and coordinate values that the mouse cursor is hovering over in the
        // cart2Da brush grid display window

        float x_pos = 10.0f, y_pos = 20.0f;

        text("Cartesian 2D grid Brush", x_pos, y_pos);


        ImGui::Text("Select Brush To Use");
        static int item_selected_idx = 0; // Here we store our selected data as an index.

        //static bool item_highlight = false;
        int item_highlighted_idx = -1; // Here we store our highlighted data as an index.

        // Display list of available cart2D brush grids to select for use or edit
        ImGui::SetNextItemWidth(300);
        if (ImGui::BeginListBox("##hbslb"))
        {
            for (int n = 0; n < cart2D_grid_brush_selections.size(); n++)
            {
                const bool is_selected = (item_selected_idx == n);
                if (ImGui::Selectable(cart2D_grid_brush_selections[n].first.c_str(), is_selected)) {
                    item_selected_idx = n;
                    //printf("cart2D_grid_brush_class::listbox 0000 selection %i %s \n", item_selected_idx, items[item_selected_idx].c_str());
                    //printf("cart2D_grid_brush_class::listbox 0000 selection %i %s \n", item_selected_idx, cart2D_grid_brush_selections[item_selected_idx].first.c_str());
                }

                if (ImGui::IsItemHovered()) {
                    item_highlighted_idx = n;
                    //printf("cart2D_grid_brush_class::listbox 1111 hovered %i %s \n", item_highlighted_idx, items[item_highlighted_idx].c_str());
                    //printf("cart2D_grid_brush_class::listbox 1111 hovered %i %s \n", item_highlighted_idx, cart2D_grid_brush_selections[item_highlighted_idx].first.c_str());
                }

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected) {
                    ImGui::SetItemDefaultFocus();
                    cart2D_grid_brush_selection = cart2D_grid_brush_selections[item_selected_idx];
                    //printf("cart2D_grid_brush_class::listbox 2222 is_selected %i %s \n", item_selected_idx, items[item_selected_idx].c_str());
                    //printf("cart2D_grid_brush_class::listbox 2222 is_selected %i %s \n", item_selected_idx, cart2D_grid_brush_selections[item_selected_idx].first.c_str());
                }
            }
            ImGui::EndListBox();
        }

        ImGui::SameLine();
        ImGui::Text("Current Selected Brush :\n %s", cart2D_grid_brush_selection.first.c_str());

//if (!cart2D_grid_brush_selection.second) printf("!cart2D_grid_brush_selection.second\n");

        // Display a preview of the selected cart2D brush in a ImPlot::ScatterPlot widget that the user cannot interact with
        if (cart2D_grid_brush_selection.second) {// Test that have a valid cart2D brush grid selected
//printf("cart2D_grid_brush_class::display_cart2D_grid_brush_panel cart2D_grid_brush_selection.second : %i : %i : %i\n", cart2D_grid_brush_selection.second->cart2D_centers_x.size(), cart2D_grid_brush_selection.second->cart2D_centers_y.size(),cart2D_grid_brush_selection.second->cart2D_colors.size());

            int brush_max_dim = std::max(cart2D_grid_brush_selection.second->grid_dimension.x, cart2D_grid_brush_selection.second->grid_dimension.y);
            float brush_preview_size = ((brush_preview_range_size / brush_preview_window_size) / float(brush_max_dim)) * (0.5f * brush_preview_scale) * brush_preview_range_size;

            //ImPlot::BeginPlot("Cart2D Grid Brush", ImVec2(brush_preview_window_size, brush_preview_window_size), brush_preview_flags);
            ImGui::SetCursorPosX(320);
            ImGui::SetCursorPosY(y_pos + 60);
            ImPlot::BeginPlot("Cart2D Grid Brush", ImVec2(brush_preview_window_size, brush_preview_window_size), brush_preview_flags);
            ImPlot::SetupAxes(nullptr, nullptr, brush_preview_axes_flags, brush_preview_axes_flags);
            ImPlot::SetupAxesLimits(-1.25f, float(brush_max_dim) * 2.0f, -1.25f, float(brush_max_dim) * 2.0f, ImGuiCond_None);

            ImPlot::PlotScatter("Selected cart2D brush", cart2D_grid_brush_selection.second->cart2D_centers_x.data(), cart2D_grid_brush_selection.second->cart2D_centers_y.data(), cart2D_grid_brush_selection.second->cart2D_centers_x.size(), {
                ImPlotProp_Marker, cart2D_grid_display_shape,// use this when testing
                ImPlotProp_MarkerSize, brush_preview_size,
                ImPlotProp_MarkerFillColors, cart2D_grid_brush_selection.second->cart2D_colors.data(),
                ImPlotProp_MarkerLineColors, cart2D_grid_brush_selection.second->cart2D_colors.data(),
                ImPlotProp_FillAlpha, 0.25f,
                ImPlotProp_Flags,
                ImPlotFlags_Equal
                });
            ImPlot::EndPlot();
        }

        // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        // ###### Widget to display the cart2D brush edit window to edit selected or create a new cart2D brush #####
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        ImGui::Text("Edit/Create Brush : ");
        ImGui::SameLine();
        // interactively display edit cart2D grid brush
        ImGui::Checkbox("##hgebd", &display_cart2D_grid_brush_edit);

        // Display cart2D brush widgets that the user can interact with to edit a cart2D brush grid
        if (display_cart2D_grid_brush_edit) {// Display cart2D brush edit widgets if flag to edit cart2D grid brush is set to true
            ImGui::Separator();
            ImGui::Text("Edit/Create cart2D grid Brush");

            ImGui::Text("Cart 2D Grid Coordinate ::   ");
            ImGui::SameLine();
            ImGui::Text("X : %i | ", cart2D_grid_brush_coord.x);
            ImGui::SameLine();
            ImGui::Text("Y : %i", cart2D_grid_brush_coord.y);

            ImGui::Text("Cart 2D Index : %i  ||", cart2D_grid_index);
            ImGui::SameLine();

            if (cart2D_grid_index > -1 && cart2D_grid_index < cart2D_grid_brush->cart2D_grid.size()) cart2D_grid_value = cart2D_grid_brush->cart2D_grid[cart2D_grid_index];

            ImGui::Text(" Cart 2D value : %i", cart2D_grid_value); // Need to accomidate all data types of cart2D grid here !!!!
            ImGui::Separator();

            // ------------------------------------------------------
            ImGui::Text("Grid Color : ");
            ImGui::SameLine();
            // interactively change result display color
            if (ImGui::ColorEdit4("##hbgc", (float*)&cart2D_grid_display_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_None)) {
                if (cart2D_grid_brush) {
                    cart2D_grid_base_class<T>* bc = cart2D_grid_brush;// This is to access the base class of cart2D_grid_class
                    bc->cart2D_grid_display_color = cart2D_grid_display_color;

                    for (size_t i = 0; i < cart2D_grid_brush->cart2D_colors.size(); i++) {
                        if (cart2D_grid_brush->cart2D_colors[i] != ImGui::GetColorU32(cart2D_grid_edit_color)) {
                            cart2D_grid_brush->cart2D_colors[i] = ImGui::GetColorU32(cart2D_grid_display_color);
                        }
                    }
                }
            }
            //ImGui::ColorEdit4("##hgc", (float*)cart2D_grid_brush->cart2D_grid_display_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_None); //This generates compilor error
            ImGui::SameLine();
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
                    if (is_selected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::SameLine();
            ImGui::Text("Size : ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(60);
            // interactively change result display shape size
            ImGui::DragFloat("##hgsi", &cart2D_grid_display_shape_size, 0.1f, 1.0f, 50.0f, "%2.2f");
            ImGui::SameLine();
            ImGui::Text("Brush Color : ");
            ImGui::SameLine();
            // interactively change edit display color
            if (ImGui::ColorEdit4("##hgbc", (float*)&cart2D_grid_edit_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_None)) {
                if (cart2D_grid_brush) {
                    //cart2D_grid_base_class<T>* bc = cart2D_grid_brush;
                    //bc->cart2D_grid_display_color = cart2D_grid_display_color;
                    for (size_t i = 0; i < cart2D_grid_brush->cart2D_colors.size(); i++) {
                        if (cart2D_grid_brush->cart2D_colors[i] != ImGui::GetColorU32(cart2D_grid_display_color)) {
                            cart2D_grid_brush->cart2D_colors[i] = ImGui::GetColorU32(cart2D_grid_edit_color);
                        }
                    }
                }
            }
            // ----------------------------------        

            ImGui::Separator();

            x_pos += 55;
            y_pos += 300;
            if (ex_button("Edit Current selected Brush", x_pos + 100, y_pos, 200, 20)) {
                //printf("create_cart2D_grid_panel_class::display 1111 \n");
                if (!cart2D_grid_brush_selection.second) {
                    vwDialogs::display_error_message("Edit Current Brush ERROR:", "Can not edit current selected brush. \nIt is undefined.");
                }
                else {
                    cart2D_grid_brush_edit = cart2D_grid_brush_selection;
                    cart2D_grid_brush_defined = true;
                }
            }

            y_pos += 30;
            if (ex_button("Create New Cart 2D Grid Brush", x_pos + 100, y_pos, 200, 20)) {
                //printf("create_cart2D_grid_panel_class::display 1111 \n");
                create_cart2D_grid_brush_popup = true;
            }

            /* !!!!!!! A Future feature for a more advanced brush editing suite implementation !!!!!!!!!!!!!!!!
                        y_pos += 30;
                        if (ex_button("Change Hex Brush size", x_pos + 100, y_pos, 180, 20)) {
                            show_change_brush_size_widget = true;
                        }

                        if (show_change_brush_size_widget) {
                            bool canceled_change_brush_size = false;

                            change_cart2D_grid_brush_size(x_pos,y_pos,canceled_change_brush_size);

                            if (canceled_change_brush_size) show_change_brush_size_widget = false;
                        }
            */

            y_pos += 30;
            if (ex_button("Add Brush To List", x_pos + 100, y_pos, 200, 20)) {
                add_brush_to_list(cart2D_grid_brush_edit);
            }

            y_pos += 30;
            if (ex_button("Save Brush To List", x_pos + 100, y_pos, 200, 20)) {
                save_brush_to_list(cart2D_grid_brush_edit);
            }

            y_pos += 30;
            if (ex_button("Import Cart 2D Grid Brush", x_pos + 10, y_pos, 180, 20)) {
                import_cart2D_grid_brush();
            }

            if (ex_button("Export New Cart 2D Grid Brush", x_pos + 210, y_pos, 180, 20)) {
                export_cart2D_grid_brush();
            }

            if (create_cart2D_grid_brush_popup) {
                bool canceled_cart2D_brush = false;
                cart2D_grid_brush_list_data_type new_cart2D_grid_brush = display_create_cart2D_grid_brush_popup(canceled_cart2D_brush);
                if (new_cart2D_grid_brush.second) {
                    cart2D_grid_brush = new_cart2D_grid_brush.second;
                    cart2D_grid_brush_edit = new_cart2D_grid_brush;
                    cart2D_grid_brush_defined = true;
                    create_cart2D_grid_brush_popup = false;
                }

                if (canceled_cart2D_brush) { create_cart2D_grid_brush_popup = false; }
            }


        }

        if (cart2D_grid_brush_defined && display_cart2D_grid_brush_edit) {// Display cart2D brush edit window widget of cart2D brush grid cell data
            //printf("acart2D_application_class: display_acart2D_main_gui_panel : 0000  \n");
            if (cart2D_grid_brush_edit.second) {
                // cart2D_grid_brush->display_cart2D_grid(); do not use this
                if (!cart2D_grid_brush_edit_display()) { // Display cart2D brush edit window widget of cart2D brush grid cell data
                    vwDialogs::display_error_message("Edit Brush ERROR:", "Can not edit current selected brush. \nIt is undefined.");
                    cart2D_grid_brush_defined = false;
                }
            }
        }

        ImGui::End();
    }

    // Popup window widget function to define inputs to initiate and create a new cart2D brush grid to edit
    cart2D_grid_brush_list_data_type display_create_cart2D_grid_brush_popup(bool& cancel = false) {
        //printf("cart2D_grid_brush_class::display_create_cart2D_grid_brush_popup AAAAA \n");

        ImGui::OpenPopup("Create Cart2D Grid Brush");

        // Always center this window when appearing
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        cart2D_grid_brush_list_data_type return_value;
        return_value.first = "Brush";
        return_value.second = nullptr;

        if (ImGui::BeginPopupModal("Create Cart2D Grid Brush", NULL, ImGuiWindowFlags_AlwaysAutoResize))// Must have same id name as ImGui::OpenPopup to display
        {
            //printf("cart2D_grid_brush_class::display_create_cart2D_grid_brush_popup 00000 \n");

            float x_pos = 10.0f, y_pos = 40.0f;

            text("Define Global Cart2D grid", x_pos, y_pos);

            x_pos = 20; y_pos += 30;
            text("Brush Name : ", x_pos, y_pos);
            ImGui::SameLine();
            ImGui::InputText("##nhgbn", &cart2D_grid_brush_name, ImGuiInputTextFlags_CallbackCharFilter, Input_Filters::name);// ++++

            x_pos = 20; y_pos += 30;
            text("Brush Width :", x_pos, y_pos);
            integer_input("###cart2Dgxd", xdim, x_pos + 100, y_pos - 3, 100.0f);

            y_pos += 30;
            text("Brush Height :", x_pos, y_pos);
            integer_input("###cart2Dgyd", ydim, x_pos + 100, y_pos - 3, 100.0f);

            // for possible future implementation
            //y_pos += 30;
            //text("Initial Hex cell value:", x_pos, y_pos);
            //float_input("###cart2Dghs", cart2D_grid_value, x_pos + 250, y_pos - 3, 100.0f);

            y_pos += 30;
            if (ex_button("Create Cart 2D Grid Brush", x_pos + 10, y_pos, 180, 20)) {
                //printf("create_cart2D_grid_panel_class::display 1111 \n");
                return_value.first = cart2D_grid_brush_name;
                return_value.second = create_cart2D_grid_brush(xdim, ydim, cart2D_grid_value);
            }

            if (ex_button("Cancel###chgcb", x_pos + 210, y_pos, 100, 20)) {
                cancel = true;
            }

            //printf("cart2D_grid_brush_class::display_create_cart2D_grid_brush_popup 22222 \n");		

            ImGui::EndPopup();

        }
        //printf("cart2D_grid_brush_class::display_create_cart2D_grid_brush_popup 3333 \n");

        return return_value;
    }

    // ImGui window widget to display the cart2D brush grid cell data using the ImPlot::ScatterPlot function. 
    bool cart2D_grid_brush_edit_display() {
        cart2D_grid_class<T>* cart2D_grid_brush = cart2D_grid_brush_edit.second;

        if (!cart2D_grid_brush) {// comment out when confident this function will not be called if cart2D_grid_brush_edit.second is a nullptr
//printf("cart2D_grid_brush_class::cart2D_grid_brush_edit_display ERROR: Cart 2D grid brush to edit is a nullptr\n");
            return false;
        }

        ImGui::Begin("Cart2D Grid Brush", nullptr, plot_window_flags);// If Have name of ImGui::Begin then have ability to minimise and dock 

        if (ImPlot::BeginPlot("Cart2D Grid Brush", ImVec2(-1, -1), plot_flags)) {

            ImPlotStyle& style = ImPlot::GetStyle();
            ImGuiContext* cpc = ImGui::GetCurrentContext();// Strange compilor does not complain about ImGuiContext without including imgui_internal.h
            // But does complain about ImGuiContext not being defined when using cpc in the next lines.

//float resized_width  = cpc->CurrentWindow->Size.x;
//float resized_height = cpc->CurrentWindow->Size.y;

 // Test and functions to maintain the display aspect ratio of the ImGui-ImPlot window
 // when performing windo resizing using the window borders. This is performed 
 // so as to avoid strange disparity of the display of cart2D grid where the grid X-Y axis
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

                        // Constrain the cart2D grid display to a min max Cartesian coordinate range of coordinates
            ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, plot_min_x, plot_max_x);
            //ImPlot::SetupAxisZoomConstraints(ImAxis_X1, plot_min_zoom, plot_max_zoom);
            ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, plot_min_y, plot_max_y);
            //ImPlot::SetupAxisZoomConstraints(ImAxis_Y1, plot_min_zoom, plot_max_zoom);

            // ImPlot::ScatterPlot function to display main cart2D grid cell data if flag is set to display cart2D brush grid
            if (display_acart2D_grid) {
                // ImPlot::PlotScatter("cart2D brush", cart2D_grid_brush->cart2D_centers_x.data(), cart2D_grid_brush->cart2D_centers_y.data(), cart2D_grid_brush->cart2D_centers_x.size(), {
                ImPlot::PlotScatter("cart2D brush", cart2D_grid_brush->cart2D_centers_x.data(), cart2D_grid_brush->cart2D_centers_y.data(), cart2D_grid_brush->cart2D_centers_x.size(), {
                    ImPlotProp_Marker, cart2D_grid_display_shape,// use this when testing
                    ImPlotProp_MarkerSize, cart2D_grid_display_shape_size,
                    //ImPlotProp_LineColor, cart2D_grid_display_color,
                    //ImPlotProp_FillColor, cart2D_grid_display_color,
                    ImPlotProp_MarkerFillColors, cart2D_grid_brush->cart2D_colors.data(),
                    ImPlotProp_MarkerLineColors, cart2D_grid_brush->cart2D_colors.data(),
                    ImPlotProp_FillAlpha, 0.25f,
                    ImPlotProp_Flags,
                    ImPlotFlags_Equal
                    });
            }

            mouse_plot_pos = ImPlot::GetPlotMousePos(); // This must be placed within the BeginPlot : EndPlot block or application will crash

            for (cart2D_grid_cell_data_struct_type cart2D_grid_cell : cart2D_grid_cells_data_list) {
                //printf("cart2D_grid_class::display_cart2D_grid 3333\n");
                cart2D_grid_cell.display_cart2D_cell_data();
            }

            ImPlot::EndPlot();
        }

        plot_axis_limit_changed = IMGUI_WINDOW_BORDER_NONE;

        ImGui::End();

        return true;
    }


    cart2D_grid_class<T>* create_cart2D_grid_brush(cart2D_surface_index_data_type xdim, cart2D_surface_index_data_type ydim, float cart2D_grid_value) {
//printf("create_cart2D_grid_panel_class::create_global_grid 00000 \n");

        if (xdim < 1 || ydim < 1) {
            //imgui_message_model_popup("Cart 2D Grid Error Message", "Create Cart 2D grid", "Cannot create cart2D grid : \n Cart 2D grid xdimension is of zero size", dummy);
            return nullptr;
        }

        cart2D_grid_class<T>* new_cart2D_grid_brush = new cart2D_grid_class<T>(cart2D_grid_value, { xdim, ydim });

        if (!new_cart2D_grid_brush) {
            afw_globalc::get_current_logger()->log(LogLevel::INFO, "Create cart2D grid brush:: Unable to define cart2D grid brush.\n");
            return nullptr;
        }

        new_cart2D_grid_brush->define_cart2D_grid_coordinates();

        return new_cart2D_grid_brush;
    }

    /* !!!!!!! A Future feature for a more advanced brush editing suite implementation !!!!!!!!!!!!!!!!
    void change_cart2D_grid_brush_size(cart2D_grid_class<T> *cart2D_grid_brush,float &x_pos,float &y_pos, bool &cancel) {

       // ImGui::Begin("Change Cart 2D Grid Brush size");// Must have same id name as ImGui::OpenPopup to display

//printf("cart2D_grid_brush_class::display_create_cart2D_grid_brush_popup 00000 \n");

        if (!cart2D_grid_brush) {
            vwDialogs::display_error_message("Change cart2D grid brush size", "ERROR ::\n No Cart 2D grid brush defined to change brush size.\n");
            cancel = true;
            return;
        }

        if (cart2D_grid_brush->cart2D_grid.size() < 1) {
            vwDialogs::display_error_message("Change cart2D grid brush size", "ERROR ::\n No Cart 2D grid brush defined to change brush size.\n");
            cancel = true;
            return;
        }

        cart2D_surface_index_data_type new_xdim = 5, new_ydim = 5;
        std::vector<T> new_cart2D_grid = {};

        ImGui::Separator();

        x_pos = 20; y_pos += 40;
        text("New Brush Width  :", x_pos+120, y_pos);
        integer_input("###cart2Dgbcxd", nxdim, x_pos + 250, y_pos - 3, 100.0f);

        y_pos += 30;
        text("New Brush Height :", x_pos+120, y_pos);
        integer_input("###cart2Dgbcyd", nydim, x_pos + 250, y_pos - 3, 100.0f);

        y_pos += 30;
        if (ex_button("Change Cart 2D Grid Brush Size", x_pos + 10, y_pos, 190, 20)) {
//printf("create_cart2D_grid_panel_class::display 1111 \n");
            ncart2D_grid = create_cart2D_grid_brush(new_xdim, new_ydim, float cart2D_grid_value);
            if (!new_cart2D_grid) {
                vwDialogs::display_error_message("Change cart2D grid brush size", "ERROR ::\n Could not define new cart2D grid brush of specified size.\n");
            } else{
                cart2D_surface_index_data_type old_xdim = cart2D_grid_brush->grid_dimension.x, old_ydim = cart2D_grid_brush->grid_dimension.y;

                cart2D_surface_index_data_type x_shift = (new_xdim - old_xdim)/2;
                cart2D_surface_index_data_type y_shift = (new_ydim - old_ydim)/2;



                if (new_ydim < old_ydim) {

                } else {

                }
            }
        }

        if (ex_button("Finish Brush Size Change###chgbccb", x_pos + 210, y_pos, 190, 20)) {
            cancel = true;
        }

        ImGui::Separator();
//printf("cart2D_grid_brush_class::display_create_cart2D_grid_brush_popup 22222 \n");

         //   ImGui::End();
    }
*/

    void import_cart2D_grid_brush() {
        //printf("cart2D_grid_brush_class::import_cart2D_grid_brush 00\n");
        char const* patterns[] = { "*C2GB.txt" };
        char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

        if (file_pathname == nullptr) {
            //if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No automata rules file defined to import cart2D surface generation parameter data from.\n");
            return;
        }

        if (!cart2D_grid_brush) {
            //printf("cart2D_grid_brush_class::import_cart2D_grid_brush 11\n");
            cart2D_grid_brush = create_cart2D_grid_brush(2, 2, cart2D_grid_value);// Create a dummy cart2D grid brush to import cart2D grid brudh data into
        }

        //printf("cart2D_grid_brush_class::import_cart2D_grid_brush 22\n");
        if (!cart2D_grid_brush) {
            afw_globalc::get_current_logger()->log(LogLevel::INFO, "Create cart2D grid brush:: Unable to define cart2D grid brush.\n");
            return;
        }

        //printf("cart2D_grid_brush_class::import_cart2D_grid_brush 33\n");
        cart2D_grid_brush->clear_all_cart2D_grid_cell_data();
        cart2D_grid_brush->delete_cart2D_grid();

        //printf("cart2D_grid_brush_class::import_cart2D_grid_brush 44\n");
        cart2D_surface_import_export.import_cart2D_surface_automata_grid(file_pathname, cart2D_grid_brush);
        cart2D_grid_brush->define_cart2D_grid_coordinates();

        //printf("cart2D_grid_brush_class::import_cart2D_grid_brush 55\n");

        for (size_t i = 0; i < cart2D_grid_brush->cart2D_colors.size(); i++) {
            if (cart2D_grid_brush->cart2D_grid[i] > 0) {
                cart2D_grid_brush->cart2D_colors[i] = ImGui::GetColorU32(cart2D_grid_edit_color);
            }
        }

        cart2D_grid_brush_defined = true;

        //printf("cart2D_grid_brush_class::import_cart2D_grid_brush 66\n");

    }

    void export_cart2D_grid_brush() {

        //printf("cart2D_surface_automata_widget_class :: save_automata_rules\n");// replace with clear variables
        char const* patterns[] = { "*C2GB.txt" };
        char const* file_pathname = vwDialogs::save_file(nullptr, patterns, 1);

        if (file_pathname == nullptr) {
            //if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No automata rules file defined to save cart2D surface automata rules data to\n");
            vwDialogs::display_error_message("Save Cart 2D Grid Brush", "ERROR : \n No cart2D grid brush data file defined to save cart2D grid brush data to\nSave cart2D grid Brush data aborted");
            return;
        }
        //else
        //printf("save_generation_parameters != NULL %s \n", file_pathname);
        //printf("save_automata_rules != NULL  \n");

        cart2D_surface_import_export.export_cart2D_surface_automata_grid(file_pathname, cart2D_grid_brush);
    }

private:
    bool cart2D_grid_brush_defined = false;
    bool create_cart2D_grid_brush_popup = false;

    bool display_cart2D_grid_brush_edit = false;
    bool show_change_brush_size_widget = false;
    bool brushes_list_initialised = false;

    float brush_preview_window_size = 100.0f;
    float brush_preview_range_size = (100.0f / 120.0f) * 100;
    float brush_preview_scale = 1.0f;

    cart2D_surface_import_export_class<T> cart2D_surface_import_export;

    cart2D_surface_index_data_type xdim = 5, ydim = 5;

    ImGuiWindow* plot_window = nullptr;
    ImGuiWindowFlags plot_window_flags = ImGuiWindowFlags_None;
    ImPlotFlags plot_flags = ImPlotFlags_Equal | ImPlotFlags_NoMenus | ImPlotFlags_Crosshairs | ImPlotFlags_NoLegend;
    ImPlotFlags brush_preview_flags = ImPlotFlags_CanvasOnly | ImPlotFlags_Equal | ImPlotFlags_NoInputs | ImPlotFlags_NoFrame;
    ImPlotAxisFlags brush_preview_axes_flags = ImPlotAxisFlags_NoDecorations;

    ImPlotPoint mouse_plot_pos = { 0.0f,0.0f };
    cart2D_surface_index_data_type cart2D_grid_index = -1;
    std::string cart2D_grid_brush_name = "Brush";
    cart2D_surface_vec_data_type cart2D_grid_brush_coord = { 0,0 };
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

    // Cartesian 2D grid display paramters 
    bool display_acart2D_grid = true;
    bool edit_acart2D_grid = false;
    const char* shapes[9] = { "Circle", "Square", "Diamond", "Up", "Down", "Left", "Right", "Cross", "Asterix" };
    ImGuiComboFlags shapes_combo_flags = ImGuiComboFlags_NoArrowButton;
    signed char plot_axis_limit_changed = -1;
    ImVec4 cart2D_grid_display_color = { 0.5f,0.5f,1.0f,1.0f };
    int cart2D_grid_display_shape_id = 1;
    float cart2D_grid_display_shape_size = 6.0f;
    float cart2D_brush_preview_display_shape_size = 6.0f;
    ImPlotMarker_ cart2D_grid_display_shape = ImPlotMarker_::ImPlotMarker_Circle;

    float cart2D_grid_edit_value = 1.0f;
    ImVec4 cart2D_grid_edit_color = { 1.0f,1.0f,1.0f,1.0f };

    int min_grad_value_i = 0;
    int max_grad_value_i = 100;


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

    void define_imgui_shape() {
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