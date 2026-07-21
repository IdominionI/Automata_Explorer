#pragma once

#include <filesystem>
#include <sys/stat.h>

#include <ThirdParty/ImGui/imgui_docking/imgui_internal.h>
#include <ImGui/imgui_docking/misc/cpp/imgui_stdlib.h>// Need This so as to be able to input text as a std::string variable

#include <FrameWork/AFW/ImGUI/ImGuiEx/imgui_custom.h>
#include <FrameWork/AFW/ImGUI/ImGuiEx/imgui_widgets.h>
#include <FrameWork/AFW/Interface/IconsFontAwesome4.h>
#include <FrameWork/AFW/tools/afw_string_tools.h>

#include "Source/Hex2d/Hex_grids/hex_surface_import_export.h"

template <typename T>
class hex_grid_class; // Need to forward declare this as have a case of circular referencing 

// Hex grid brush class to display a user interface to create, store and manage hex grid
// brushes that are used to edit the main hexagonal automata grid cell data values. 
// Similar to the maim hex_grid_class except does not inherit hex_grid_base_class.
// Each hex Brush is a hex grid class object and are stored as a vector list of these 
// hex grid class objects.

template <class T>
class hex_grid_brush_class {
    //using HGBC = hex_grid_base_class<T>;
public:
    hex_grid_brush_class() {}
	~hex_grid_brush_class() {}

    std::string brush_pathname = "Brushes/Hex_Brushes";// default pathname to application sub directory
    std::filesystem::path item_path;

    typedef std::pair < std::string, hex_grid_class<T>*> hex_grid_brush_list_data_type; // basic hex grid brush datatype
    hex_grid_brush_list_data_type hex_grid_brush_selection;                             // Current selected hex grid brush datatype
    std::vector <hex_grid_brush_list_data_type > hex_grid_brush_selections = {};        // vector list of hex grid brushes to select
    
    hex_grid_brush_list_data_type hex_grid_brush_edit;                                  // Flag to edit a hex grid brush
    hex_grid_class<T> *hex_grid_brush = nullptr;                                        // pointer to current selected hex grid brush

    std::vector<hex_grid_cell_data_struct_type> hex_grid_cells_data_list = {};          // vector list of hex grid brushes diaplay data to display on screen

    // Function to load existing hex grid brush data from disc files into a list of
    //  hex grid brushes and list of display color shape and size data
    void initialise_hex_grid_brushes() {
        //folowing for testing only : comment out or delete when finished
        //hex_grid_brush_list_data_type brush0;
        //hex_grid_brush_list_data_type brush1;
        //hex_grid_brush_list_data_type brush2;
        //hex_grid_brush_list_data_type brush3;
        //hex_grid_brush_list_data_type brush4;
        //hex_grid_brush_list_data_type brush5;

        //brush0.first = "brush0"; brush0.second = nullptr;
        //brush1.first = "brush1"; brush1.second = nullptr;
        //brush2.first = "brush2"; brush2.second = nullptr;
        //brush3.first = "brush3"; brush3.second = nullptr;
        //brush4.first = "brush4"; brush4.second = nullptr;
        //brush5.first = "brush5"; brush5.second = nullptr;

        //hex_grid_brush_selections.push_back(brush0);
        //hex_grid_brush_selections.push_back(brush1);
        //hex_grid_brush_selections.push_back(brush2);
        //hex_grid_brush_selections.push_back(brush3);
        //hex_grid_brush_selections.push_back(brush4);
        //hex_grid_brush_selections.push_back(brush5);


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
                // ----------- Import hex grid brush data ------------------
 //printf("hex_grid_brush_class::initialise_hex_grid_brushes 11\n");
                hex_grid_class<T> *hgrid_brush = create_hex_grid_brush(2, 2, hex_grid_value);// Create a dummy hex grid brush to import hex grid brudh data into
//printf("hex_grid_brush_class::initialise_hex_grid_brushes 22\n");
                if (!hgrid_brush) {
                    afw_globalc::get_current_logger()->log(LogLevel::INFO, "Create hex grid brush:: Unable to define hex grid brush" + bpn +"\n");
                    continue;
                }
//printf("hex_grid_brush_class::initialise_hex_grid_brushes 33\n");
                hgrid_brush->clear_all_hex_grid_cell_data();
                hgrid_brush->delete_hex_grid();
//printf("hex_grid_brush_class::initialise_hex_grid_brushes 44\n");
		        hex_surface_import_export.import_hex_surface_automata_grid(bpn, hgrid_brush);
                hgrid_brush->define_hex_grid_coordinates();
//printf("hex_grid_brush_class::initialise_hex_grid_brushes 55\n");
                for (size_t i = 0; i < hgrid_brush->hex_colors.size(); i++) {
                    if (hgrid_brush->hex_grid[i] > 0)
                        hgrid_brush->hex_colors[i] = ImGui::GetColorU32(hex_grid_edit_color);
                }

                // -------------- Define grid brush name to displsy in the brush list ---------------------
                FW::stringtools::removeSubstring(bpn, "HGB.txt");
                size_t pos1 = bpn.find_last_of('\\');                
//                std::cout << "1 : " << bpn << " : " << pos1 << std::endl;
                bpn.erase(0, pos1 + 1);
//                std::cout << "2 : " << bpn << std::endl;
                FW::stringtools::replace(bpn, "_", " ");
//                std::cout << "3 : " << bpn << std::endl;

                hex_grid_brush_list_data_type new_brush;
                new_brush.first  = bpn;
                new_brush.second = hgrid_brush;

                add_brush_to_list(new_brush);

            }// else { // have a directory
                // not to be used but left here for reference
			//}
        }     

        // Initialise default hex brush slection
        hex_grid_brush_edit.first = "null";
        hex_grid_brush_edit.second = nullptr;

        if (hex_grid_brush_selections.size() > 0) {
            hex_grid_brush_selection = hex_grid_brush_selections[0];
        } else {
            hex_grid_brush_selection.first  = "null";
            hex_grid_brush_selection.second = nullptr;
        }
    }

    bool add_brush_to_list(hex_grid_brush_list_data_type hex_grid_brush) {
        //Check for hex grid brush of same name
        for (size_t i = 0; i < hex_grid_brush_selections.size(); i++) {
            if (hex_grid_brush.first == hex_grid_brush_selections[i].first) {
                vwDialogs::display_error_message("Add Brush ERROR:", "Could not add brush to selection list. \nHave an existing brush of the same name.");
                return false;
            }
        }

        // Check if have brush grid undefined
        if (!hex_grid_brush.second) {
            vwDialogs::display_error_message("Add Brush ERROR:", "Could not add brush to selection list. \nBrush to add is undefined.");
            return false;
        }

        hex_grid_brush_selections.push_back(hex_grid_brush);

        return true;
    }

    bool save_brush_to_list(hex_grid_brush_list_data_type &hex_grid_brush) {
        // Check to see if have brush of same name
        for(hex_grid_brush_list_data_type &hex_grid_brush_selection: hex_grid_brush_selections){
            if (hex_grid_brush.first == hex_grid_brush_selection.first) {// Have brush of the same name : overwrite it
                if (!hex_grid_brush.second) {// Check if have brush grid undefined
                    vwDialogs::display_error_message("Save Brush ERROR:", "Could not Save brush to selection list. \nBrush to save is undefined.");
                    return false;
                } else {
//printf("hex_grid_brush_class::save_brush_to_list AAAA %s %s\n", hex_grid_brush_selection.first.c_str(), hex_grid_brush.first.c_str());
//if(!hex_grid_brush_selection.second) printf("hex_grid_brush_class::save_brush_to_list ABABAB !hex_grid_brush_selection.second\n");
                    if (hex_grid_brush.second != hex_grid_brush_selection.second) {
//printf("hex_grid_brush_class::save_brush_to_list BBBB\n");
                        delete hex_grid_brush_selection.second; // delete hex grid data as it is to be replaced and no longer used
//printf("hex_grid_brush_class::save_brush_to_list CCCC\n");
                        hex_grid_brush_selection.second = hex_grid_brush.second;
                    }
//printf("hex_grid_brush_class::save_brush_to_list DDDD\n");
                    return true;
                }
            }
        }

        return add_brush_to_list(hex_grid_brush);// No brush to overwrite : add brush to list
    }


    // The main ImGui window widget panel to display widgets for user to interact with to select
    // existing hex brush to use for editing the main hex automata grid, or to create/edit/save/load
    // hex brush grid. If the editing of a hex brush is activarted, a hex brush edit window will
    // appear that operates similarly to the main hex grid automata display window widget.
    void display_hex_grid_brush_panel() {
        if (!brushes_list_initialised) { // need this or brushes will be added at infenitum every ImGui frame call
            initialise_hex_grid_brushes();
            brushes_list_initialised = true;
        }

        // For some unknown reason naming the ImGui::Begin window below other than Hex Grid Brush causes ImPlot::BeginPlot to crash application with error
        // [01120] [imgui-error] (current settings: Assert=1, Log=1, Tooltip=1)
        // [01120][imgui - error] In window 'cart Grid Brush ': Setup needs to be called after BeginPlot and before any setup locking functions(e.g.PlotX)!
        // 
        ImGui::Begin("Hex Grid Brush", nullptr, plot_window_flags);// If Have name of ImGui::Begin then have ability to minimise and dock 
        //ImGui::Begin("cart Grid Brush ", nullptr, plot_window_flags);// If Have name of ImGui::Begin then have ability to minimise and dock 

        // ###### This section of code is relevant to the main Hex brush grid display ###########
        // ###### window and if the mouse cusor is within it to perform editing tasks ###########
        // Test if the mouse cursor is within the bounds of the hex brush display window and if it is
        // test for any valid user interactions with the hex brush data and perform tasks according to what
        // keyboard or mouse interaction are initiated.
        if (hex_grid_brush && display_hex_grid_brush_edit) {
            ImGuiContext* cec = ImGui::GetCurrentContext();// Get current Imgui context of what is happening 
            bool edit_mode = true;// default mode of interaction 

            if (cec && cec->HoveredWindow) {// Have mouse cursor hovering within an ImGui window
                if (cec->HoveredWindow->ID == plot_window->ID) { // If the mouse cursor is hovering over the window that the hex brush is being displayed in
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
                    // If mouse cursor is within the bounds of the hex brush display then can perform hex brush edit functions
                    if (mouse_plot_pos.x > plot_min_x && mouse_plot_pos.x < plot_max_x && mouse_plot_pos.y > plot_min_y && mouse_plot_pos.y < plot_max_y) {
                        // Mouse cursor position is given in Cartesian coordinates of the hex brush grid ImPlot being displayed, so need to convert mouse Cartesian
                        // coordinates to hex brush grid index and hex brush grid x-y index coordinates to be able to edit the hex brush grid cell data values and retrieve them for display
                        hex_grid_index = hex_grid_brush->index_of_hex_cell_with_cartesian_coord(mouse_plot_pos.x, mouse_plot_pos.y);
                        hex_grid_brush_coord = hex_grid_brush->get_matrix_coordinate(hex_grid_index);

                        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) { //On left mouse click flip set the hex brush call value to 1 and change the hex brush grid display data
//printf("edit_grid::mouse button left : %i\n", hex_grid_index);
                            if (hex_grid_index > -1 && hex_grid_index < hex_grid_brush->hex_grid.size()) {
                                hex_grid_brush->hex_grid[hex_grid_index] = hex_grid_edit_value;
                                hex_grid_brush->hex_colors[hex_grid_index] = ImGui::GetColorU32(hex_grid_edit_color);
//printf("edit_grid::mouse button left :index %i | value %i ::color | %0.3f| %0.3f| %0.3f| %0.3f\n", hex_grid_index, hex_grid_brush->hex_grid[hex_grid_index], c[0], c[1], c[2], c[3]);
                            }

                        }

                        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {//On right mouse click flip set the hex brush call value to 0 and change the hex brush grid display data
//printf("edit_grid::mouse button left : %i\n", hex_grid_index);
                            if (hex_grid_index > -1 && hex_grid_index < hex_grid_brush->hex_grid.size()) {
                                hex_grid_brush->hex_grid[hex_grid_index] = 0.0f;
                                hex_grid_brush->hex_colors[hex_grid_index] = ImGui::GetColorU32(hex_grid_display_color);

//printf("edit_grid::mouse button left :index %i | value %i ::color | %0.3f| %0.3f| %0.3f| %0.3f\n", hex_grid_index, hex_grid_brush->hex_grid[hex_grid_index], c[0], c[1], c[2], c[3]);
                            }

                        }

                        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
//printf("edit_grid::mouse button right : %i\n", hex_grid_index);
                        }
                    }
                }
                else {// Enable plot inputs and window interaction when hex plot edit window is not hovered over in edit mode and disable edit functions
                    plot_flags = ImPlotFlags_Equal | ImPlotFlags_NoMenus | ImPlotFlags_Crosshairs | ImPlotFlags_NoLegend;
                    plot_window_flags = ImGuiWindowFlags_None;
                    edit_mode = false;
                }
            }
        }

        // ##################################################################################################

        // ###### This section of code is relevant to the display of the Hex brush grid edit widgets ###########
        // ###### to define the hex brush grid edit inputs and perform hex brush grid edit processes ###########
        // Display the current hex brush grid data and coordinate values that the mouse cursor is hovering over in the
        // hexa brush grid display window

        float x_pos = 10.0f, y_pos = 20.0f;

        text("Hex grid Brush", x_pos, y_pos);


        ImGui::Text("Select Brush To Use");
        static int item_selected_idx = 0; // Here we store our selected data as an index.

        //static bool item_highlight = false;
        int item_highlighted_idx = -1; // Here we store our highlighted data as an index.

        // Display list of available hex brush grids to select for use or edit
        ImGui::SetNextItemWidth(300);
        if (ImGui::BeginListBox("##hbslb"))
        {
            for (int n = 0; n < hex_grid_brush_selections.size(); n++)
            {
                const bool is_selected = (item_selected_idx == n);
                if (ImGui::Selectable(hex_grid_brush_selections[n].first.c_str(), is_selected)) {
                    item_selected_idx = n;
//printf("hex_grid_brush_class::listbox 0000 selection %i %s \n", item_selected_idx, items[item_selected_idx].c_str());
//printf("hex_grid_brush_class::listbox 0000 selection %i %s \n", item_selected_idx, hex_grid_brush_selections[item_selected_idx].first.c_str());
                }

                if (ImGui::IsItemHovered()) {
                    item_highlighted_idx = n;
//printf("hex_grid_brush_class::listbox 1111 hovered %i %s \n", item_highlighted_idx, items[item_highlighted_idx].c_str());
//printf("hex_grid_brush_class::listbox 1111 hovered %i %s \n", item_highlighted_idx, hex_grid_brush_selections[item_highlighted_idx].first.c_str());
                }

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected){
                    ImGui::SetItemDefaultFocus();
                    hex_grid_brush_selection = hex_grid_brush_selections[item_selected_idx];
//printf("hex_grid_brush_class::listbox 2222 is_selected %i %s \n", item_selected_idx, items[item_selected_idx].c_str());
//printf("hex_grid_brush_class::listbox 2222 is_selected %i %s \n", item_selected_idx, hex_grid_brush_selections[item_selected_idx].first.c_str());
                }
            }
            ImGui::EndListBox();
        }

        ImGui::SameLine();
        ImGui::Text("Current Selected Brush :\n %s", hex_grid_brush_selection.first.c_str());

//if (!hex_grid_brush_selection.second) printf("!hex_grid_brush_selection.second\n");

        // Display a preview of the selected hex brush in a ImPlot::ScatterPlot widget that the user cannot interact with
        if (hex_grid_brush_selection.second) {// Test that have a valid hex brush grid selected
//printf("hex_grid_brush_class::display_hex_grid_brush_panel hex_grid_brush_selection.second : %i : %i : %i\n", hex_grid_brush_selection.second->hex_centers_x.size(), hex_grid_brush_selection.second->hex_centers_y.size(),hex_grid_brush_selection.second->hex_colors.size());

        int brush_max_dim = std::max(hex_grid_brush_selection.second->grid_dimension.x, hex_grid_brush_selection.second->grid_dimension.y);
        float brush_preview_size = ((brush_preview_range_size/brush_preview_window_size) / float(brush_max_dim))*(0.5f* brush_preview_scale)* brush_preview_range_size;

        ImGui::SetCursorPosX(320);
        ImGui::SetCursorPosY(y_pos + 60);
        ImPlot::BeginPlot("Hex Grid Brush", ImVec2(brush_preview_window_size, brush_preview_window_size), brush_preview_flags);
            ImPlot::SetupAxes(nullptr, nullptr, brush_preview_axes_flags, brush_preview_axes_flags);
            ImPlot::SetupAxesLimits(-1.25f, float(brush_max_dim)*2.0f, -1.25f, float(brush_max_dim)*2.0f, ImGuiCond_None);          

            ImPlot::PlotScatter("Selected hex brush", hex_grid_brush_selection.second->hex_centers_x.data(), hex_grid_brush_selection.second->hex_centers_y.data(), hex_grid_brush_selection.second->hex_centers_x.size(), {
                ImPlotProp_Marker, hex_grid_display_shape,// use this when testing
                ImPlotProp_MarkerSize, brush_preview_size,
                ImPlotProp_MarkerFillColors, hex_grid_brush_selection.second->hex_colors.data(),
                ImPlotProp_MarkerLineColors, hex_grid_brush_selection.second->hex_colors.data(),
                ImPlotProp_FillAlpha, 0.25f,
                ImPlotProp_Flags,
                ImPlotFlags_Equal
                });
        ImPlot::EndPlot();
        }

        // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        // ###### Widget to display the hex brush edit window to edit selected or create a new hex brush #####
        ImGui::SetCursorPosY(ImGui::GetCursorPosY()+10);
        ImGui::Text("Edit/Create Brush : ");
        ImGui::SameLine();
        // interactively display edit hex grid brush
        ImGui::Checkbox("##hgebd", &display_hex_grid_brush_edit);

        // Display hex brush widgets that the user can interact with to edit a hex brush grid
        if (display_hex_grid_brush_edit) {// Display hex brush edit widgets if flag to edit hex grid brush is set to true
            ImGui::Separator();
            ImGui::Text("Edit/Create hex grid Brush");

            ImGui::Text("Hex Grid Coordinate ::   ");
            ImGui::SameLine();
            ImGui::Text("X : %i | ", hex_grid_brush_coord.x);
            ImGui::SameLine();
            ImGui::Text("Y : %i", hex_grid_brush_coord.y);

            ImGui::Text("Hex Grid Index : %i  ||", hex_grid_index);
            ImGui::SameLine();

            if (hex_grid_index > -1 && hex_grid_index < hex_grid_brush->hex_grid.size()) hex_grid_value = hex_grid_brush->hex_grid[hex_grid_index];

            ImGui::Text(" Hex Grid value : %i", hex_grid_value); // Need to accomidate all data types of hex grid here !!!!
            ImGui::Separator();

            // ------------------------------------------------------
            ImGui::Text("Grid Color : ");
            ImGui::SameLine();
            // interactively change result display color
            if (ImGui::ColorEdit4("##hbgc", (float*)&hex_grid_display_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_None)) {
                if (hex_grid_brush) {
                    hex_grid_base_class<T>* bc = hex_grid_brush;// This is to access the base class of hex_grid_class
                    bc->hex_grid_display_color = hex_grid_display_color;

                    for (size_t i = 0; i < hex_grid_brush->hex_colors.size(); i++) {
                        if (hex_grid_brush->hex_colors[i] != ImGui::GetColorU32(hex_grid_edit_color)) {
                            hex_grid_brush->hex_colors[i] = ImGui::GetColorU32(hex_grid_display_color);
                        }
                    }
                }
            }
            //ImGui::ColorEdit4("##hgc", (float*)hex_grid_brush->hex_grid_display_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_None); //This generates compilor error
            ImGui::SameLine();
            ImGui::Text("Shape : ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(60);
            const char* combo_preview_value = shapes[hex_grid_display_shape_id];
            if (ImGui::BeginCombo("##hgsh", combo_preview_value, shapes_combo_flags))
            {
                for (int n = 0; n < IM_COUNTOF(shapes); n++)
                {
                    const bool is_selected = (hex_grid_display_shape_id == n);
                    if (ImGui::Selectable(shapes[n], is_selected)) {
                        hex_grid_display_shape_id = n;
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
            ImGui::DragFloat("##hgsi", &hex_grid_display_shape_size, 0.1f, 1.0f, 50.0f, "%2.2f");
            ImGui::SameLine();
            ImGui::Text("Brush Color : ");
            ImGui::SameLine();
            // interactively change edit display color
            if (ImGui::ColorEdit4("##hgbc", (float*)&hex_grid_edit_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_None)) {
                if (hex_grid_brush) {
                    //hex_grid_base_class<T>* bc = hex_grid_brush;
                    //bc->hex_grid_display_color = hex_grid_display_color;
                    for (size_t i = 0; i < hex_grid_brush->hex_colors.size(); i++) {
                        if (hex_grid_brush->hex_colors[i] != ImGui::GetColorU32(hex_grid_display_color)) {
                            hex_grid_brush->hex_colors[i] = ImGui::GetColorU32(hex_grid_edit_color);
                        }
                    }
                }
            }
            // ----------------------------------        

            ImGui::Separator();

            x_pos += 55;
            y_pos += 300;
            if (ex_button("Edit Current selected Brush", x_pos + 100, y_pos, 200, 20)) {
 //printf("create_hex_grid_panel_class::display 1111 \n");
                if (!hex_grid_brush_selection.second) {
                    vwDialogs::display_error_message("Edit Current Brush ERROR:", "Can not edit current selected brush. \nIt is undefined.");
                } else{
                    hex_grid_brush_edit = hex_grid_brush_selection;
                    hex_grid_brush_defined = true;
                }
            }

            y_pos += 30;
            if (ex_button("Create New Hex Grid Brush", x_pos + 100, y_pos, 200, 20)) {
//printf("create_hex_grid_panel_class::display 1111 \n");
                create_hex_grid_brush_popup = true;
            }

/* !!!!!!! A Future feature for a more advanced brush editing suite implementation !!!!!!!!!!!!!!!!
            y_pos += 30;
            if (ex_button("Change Hex Brush size", x_pos + 100, y_pos, 180, 20)) {
                show_change_brush_size_widget = true;
            }

            if (show_change_brush_size_widget) {
                bool canceled_change_brush_size = false;

                change_hex_grid_brush_size(x_pos,y_pos,canceled_change_brush_size);

                if (canceled_change_brush_size) show_change_brush_size_widget = false;
            }
*/
            
            y_pos += 30;
            if (ex_button("Add Brush To List", x_pos + 100, y_pos, 200, 20)) {
                add_brush_to_list(hex_grid_brush_edit);
            }

            y_pos += 30;
            if (ex_button("Save Brush To List", x_pos + 100, y_pos, 200, 20)) {
                save_brush_to_list(hex_grid_brush_edit);
            }

            y_pos += 30;
            if (ex_button("Import Hex Grid Brush", x_pos + 10, y_pos, 180, 20)) {
                import_hex_grid_brush();
            }

            if (ex_button("Export New Hex Grid Brush", x_pos + 210, y_pos, 180, 20)) {
                export_hex_grid_brush();
            }

            if (create_hex_grid_brush_popup) {
                bool canceled_hex_brush = false;
                hex_grid_brush_list_data_type new_hex_grid_brush = display_create_hex_grid_brush_popup(canceled_hex_brush);
                if (new_hex_grid_brush.second) {
                    hex_grid_brush = new_hex_grid_brush.second;
                    hex_grid_brush_edit = new_hex_grid_brush;
                    hex_grid_brush_defined = true;
                    create_hex_grid_brush_popup = false;
                }

                if (canceled_hex_brush) { create_hex_grid_brush_popup = false; }
            }


        }

        if (hex_grid_brush_defined && display_hex_grid_brush_edit) {// Display hex brush edit window widget of hex brush grid cell data
//printf("ahex_application_class: display_ahex_main_gui_panel : 0000  \n");
            if (hex_grid_brush_edit.second) {
                // hex_grid_brush->display_hex_grid(); do not use this
                if (!hex_grid_brush_edit_display()) { // Display hex brush edit window widget of hex brush grid cell data
                    vwDialogs::display_error_message("Edit Brush ERROR:", "Can not edit current selected brush. \nIt is undefined.");
                    hex_grid_brush_defined = false;
                }
            }
        }

        ImGui::End();
    }

    // Popup window widget function to define inputs to initiate and create a new hex brush grid to edit
    hex_grid_brush_list_data_type display_create_hex_grid_brush_popup(bool &cancel = false) {
//printf("hex_grid_brush_class::display_create_hex_grid_brush_popup AAAAA \n");

		ImGui::OpenPopup("Create Hex Grid Brush");

		// Always center this window when appearing
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        hex_grid_brush_list_data_type return_value;
        return_value.first  = "Brush";
        return_value.second = nullptr;

		if (ImGui::BeginPopupModal("Create Hex Grid Brush", NULL, ImGuiWindowFlags_AlwaysAutoResize))// Must have same id name as ImGui::OpenPopup to display
		{
//printf("hex_grid_brush_class::display_create_hex_grid_brush_popup 00000 \n");
	
			float x_pos = 10.0f, y_pos = 40.0f;

            text("Define Global Hex grid", x_pos, y_pos);

            x_pos = 20; y_pos += 30;
            text("Brush Name : ", x_pos, y_pos);
            ImGui::SameLine();
            ImGui::InputText("##nhgbn", &hex_grid_brush_name, ImGuiInputTextFlags_CallbackCharFilter, Input_Filters::name);// ++++

            x_pos = 20; y_pos += 30;
            text("Brush Width :", x_pos, y_pos);
            integer_input("###hexgxd", xdim, x_pos + 100, y_pos - 3, 100.0f);

            y_pos += 30;
            text("Brush Height :", x_pos, y_pos);
            integer_input("###hexgyd", ydim, x_pos + 100, y_pos - 3, 100.0f);

            // for possible future implementation
            //y_pos += 30;
            //text("Initial Hex cell value:", x_pos, y_pos);
            //float_input("###hexghs", hex_grid_value, x_pos + 250, y_pos - 3, 100.0f);

            y_pos += 30;
            if (ex_button("Create Hex Grid Brush", x_pos + 10, y_pos, 180, 20)) {
//printf("create_hex_grid_panel_class::display 1111 \n");
                return_value.first  = hex_grid_brush_name;
                return_value.second = create_hex_grid_brush(xdim, ydim, hex_grid_value);
            }

            if (ex_button("Cancel###chgcb", x_pos + 210, y_pos, 100, 20)) {
                cancel = true;
            }

//printf("hex_grid_brush_class::display_create_hex_grid_brush_popup 22222 \n");		

			ImGui::EndPopup();

		}
//printf("hex_grid_brush_class::display_create_hex_grid_brush_popup 3333 \n");

		return return_value;
    }

    // ImGui window widget to display the hex brush grid cell data using the ImPlot::ScatterPlot function. 
    bool hex_grid_brush_edit_display(){
        hex_grid_class<T> *hex_grid_brush = hex_grid_brush_edit.second;

        if (!hex_grid_brush) {// comment out when confident this function will not be called if hex_grid_brush_edit.second is a nullptr
//printf("hex_grid_brush_class::hex_grid_brush_edit_display ERROR: Hex grid brush to edit is a nullptr\n");
            return false;
        }

        ImGui::Begin("Hex Grid Brush", nullptr, plot_window_flags);// If Have name of ImGui::Begin then have ability to minimise and dock 

        if (ImPlot::BeginPlot("Hex Grid Brush",ImVec2(-1, -1), plot_flags)) {

            ImPlotStyle& style = ImPlot::GetStyle();
            ImGuiContext *cpc = ImGui::GetCurrentContext();// Strange compilor does not complain about ImGuiContext without including imgui_internal.h
                                                           // But does complain about ImGuiContext not being defined when using cpc in the next lines.

           //float resized_width  = cpc->CurrentWindow->Size.x;
           //float resized_height = cpc->CurrentWindow->Size.y;

            // Test and functions to maintain the display aspect ratio of the ImGui-ImPlot window
            // when performing windo resizing using the window borders. This is performed 
            // so as to avoid strange disparity of the display of hex grid where the grid X-Y axis
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

//if(plot_axis_limit_changed> IMGUI_WINDOW_BORDER_NONE) printf("Axis Extents  %i\n", plot_axis_limit_changed);

//if (cpc)
//    printf("display_hex_grid : px %f : py %f: x %f : y %f \n", prev_width, prev_height,cpc->CurrentWindow->Size.x, cpc->CurrentWindow->Size.y);

            // Constrain the hex grid display to a min max Cartesian coordinate range of coordinates
            ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, plot_min_x, plot_max_x);
            //ImPlot::SetupAxisZoomConstraints(ImAxis_X1, plot_min_zoom, plot_max_zoom);
            ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, plot_min_y, plot_max_y);
            //ImPlot::SetupAxisZoomConstraints(ImAxis_Y1, plot_min_zoom, plot_max_zoom);

            // ImPlot::ScatterPlot function to display main hex grid cell data if flag is set to display hex brush grid
            if (display_ahex_grid) {
               // ImPlot::PlotScatter("hex brush", hex_grid_brush->hex_centers_x.data(), hex_grid_brush->hex_centers_y.data(), hex_grid_brush->hex_centers_x.size(), {
                ImPlot::PlotScatter("hex brush", hex_grid_brush->hex_centers_x.data(), hex_grid_brush->hex_centers_y.data(), hex_grid_brush->hex_centers_x.size(), {
			        ImPlotProp_Marker, hex_grid_display_shape,// use this when testing
			        ImPlotProp_MarkerSize, hex_grid_display_shape_size,
			        //ImPlotProp_LineColor, hex_grid_display_color,
			        //ImPlotProp_FillColor, hex_grid_display_color,
                    ImPlotProp_MarkerFillColors, hex_grid_brush->hex_colors.data(),
                    ImPlotProp_MarkerLineColors, hex_grid_brush->hex_colors.data(),
                    ImPlotProp_FillAlpha, 0.25f,
                    ImPlotProp_Flags, 
                    ImPlotFlags_Equal
                    });
            }

            mouse_plot_pos = ImPlot::GetPlotMousePos(); // This must be placed within the BeginPlot : EndPlot block or application will crash

            for (hex_grid_cell_data_struct_type hex_grid_cell : hex_grid_cells_data_list) {
//printf("hex_grid_class::display_hex_grid 3333\n");
               hex_grid_cell.display_hex_cell_data();
            }
            
            ImPlot::EndPlot();
        }

        plot_axis_limit_changed = IMGUI_WINDOW_BORDER_NONE;

        ImGui::End();

        return true;
}


    hex_grid_class<T>* create_hex_grid_brush(hex_surface_index_data_type xdim, hex_surface_index_data_type ydim, float hex_grid_value) {
//printf("create_hex_grid_panel_class::create_global_grid 00000 \n");

        if (xdim < 1 || ydim < 1) {
            //imgui_message_model_popup("Hex Grid Error Message", "Create Hex grid", "Cannot create hex grid : \n Hex grid xdimension is of zero size", dummy);
            return nullptr;
        }

        hex_grid_class<T>* new_hex_grid_brush = new hex_grid_class<T>(hex_grid_value, { xdim, ydim });

        if (!new_hex_grid_brush) {
            afw_globalc::get_current_logger()->log(LogLevel::INFO, "Create hex grid brush:: Unable to define hex grid brush.\n");
            return nullptr;
        }

        new_hex_grid_brush->define_hex_grid_coordinates();

        return new_hex_grid_brush;
    }

    /* !!!!!!! A Future feature for a more advanced brush editing suite implementation !!!!!!!!!!!!!!!!
    void change_hex_grid_brush_size(hex_grid_class<T> *hex_grid_brush,float &x_pos,float &y_pos, bool &cancel) {
        
       // ImGui::Begin("Change Hex Grid Brush size");// Must have same id name as ImGui::OpenPopup to display
        
//printf("hex_grid_brush_class::display_create_hex_grid_brush_popup 00000 \n");

        if (!hex_grid_brush) {
            vwDialogs::display_error_message("Change hex grid brush size", "ERROR ::\n No Hex grid brush defined to change brush size.\n");
            cancel = true;
            return;
        }

        if (hex_grid_brush->hex_grid.size() < 1) {
            vwDialogs::display_error_message("Change hex grid brush size", "ERROR ::\n No Hex grid brush defined to change brush size.\n");
            cancel = true;
            return;
        }

        hex_surface_index_data_type new_xdim = 5, new_ydim = 5;
        std::vector<T> new_hex_grid = {};

        ImGui::Separator();

        x_pos = 20; y_pos += 40;
        text("New Brush Width  :", x_pos+120, y_pos);
        integer_input("###hexgbcxd", nxdim, x_pos + 250, y_pos - 3, 100.0f);

        y_pos += 30;
        text("New Brush Height :", x_pos+120, y_pos);
        integer_input("###hexgbcyd", nydim, x_pos + 250, y_pos - 3, 100.0f);

        y_pos += 30;
        if (ex_button("Change Hex Grid Brush Size", x_pos + 10, y_pos, 190, 20)) {
//printf("create_hex_grid_panel_class::display 1111 \n");
            nhex_grid = create_hex_grid_brush(new_xdim, new_ydim, float hex_grid_value);
            if (!new_hex_grid) {
                vwDialogs::display_error_message("Change hex grid brush size", "ERROR ::\n Could not define new hex grid brush of specified size.\n");
            } else{
                hex_surface_index_data_type old_xdim = hex_grid_brush->grid_dimension.x, old_ydim = hex_grid_brush->grid_dimension.y;

                hex_surface_index_data_type x_shift = (new_xdim - old_xdim)/2;
                hex_surface_index_data_type y_shift = (new_ydim - old_ydim)/2;

                

                if (new_ydim < old_ydim) {

                } else {
                
                }
            }
        }

        if (ex_button("Finish Brush Size Change###chgbccb", x_pos + 210, y_pos, 190, 20)) {
            cancel = true;
        }

        ImGui::Separator();
//printf("hex_grid_brush_class::display_create_hex_grid_brush_popup 22222 \n");		

         //   ImGui::End();
    }
*/

    void import_hex_grid_brush() {
//printf("hex_grid_brush_class::import_hex_grid_brush 00\n");
		char const* patterns[] = { "*HGB.txt" };
		char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			//if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No automata rules file defined to import hex surface generation parameter data from.\n");
			return;
		}

        if (!hex_grid_brush) {
//printf("hex_grid_brush_class::import_hex_grid_brush 11\n");
            hex_grid_brush = create_hex_grid_brush(2,2,hex_grid_value);// Create a dummy hex grid brush to import hex grid brudh data into
        }

//printf("hex_grid_brush_class::import_hex_grid_brush 22\n");
        if (!hex_grid_brush) {
            afw_globalc::get_current_logger()->log(LogLevel::INFO, "Create hex grid brush:: Unable to define hex grid brush.\n");
            return;
        }

//printf("hex_grid_brush_class::import_hex_grid_brush 33\n");
        hex_grid_brush->clear_all_hex_grid_cell_data();
        hex_grid_brush->delete_hex_grid();

//printf("hex_grid_brush_class::import_hex_grid_brush 44\n");
		hex_surface_import_export.import_hex_surface_automata_grid(file_pathname, hex_grid_brush);
        hex_grid_brush->define_hex_grid_coordinates();

//printf("hex_grid_brush_class::import_hex_grid_brush 55\n");

            for (size_t i = 0; i < hex_grid_brush->hex_colors.size(); i++) {
                if (hex_grid_brush->hex_grid[i] > 0) {
                    hex_grid_brush->hex_colors[i] = ImGui::GetColorU32(hex_grid_edit_color);
                }
            }

            hex_grid_brush_defined = true;

//printf("hex_grid_brush_class::import_hex_grid_brush 66\n");

    }

    void export_hex_grid_brush() {

//printf("hex_surface_automata_widget_class :: save_automata_rules\n");// replace with clear variables
		char const* patterns[] = { "*HGB.txt" };
		char const* file_pathname = vwDialogs::save_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			//if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No automata rules file defined to save hex surface automata rules data to\n");
			vwDialogs::display_error_message("Save Hex Grid Brush", "ERROR : \n No hex grid brush data file defined to save hex grid brush data to\nSave hex grid Brush data aborted");
			return;
		}
//else
//printf("save_generation_parameters != NULL %s \n", file_pathname);
//printf("save_automata_rules != NULL  \n");

		hex_surface_import_export.export_hex_surface_automata_grid(file_pathname, hex_grid_brush);
    }

private:
    bool hex_grid_brush_defined = false;
    bool create_hex_grid_brush_popup = false;

    bool display_hex_grid_brush_edit   = false;
    bool show_change_brush_size_widget = false;
    bool brushes_list_initialised      = false;

    float brush_preview_window_size = 100.0f;
    float brush_preview_range_size = (100.0f / 120.0f) * 100;
    float brush_preview_scale = 1.0f;

    hex_surface_import_export_class<T> hex_surface_import_export;

    hex_surface_index_data_type xdim = 5, ydim = 5;

    ImGuiWindow* plot_window = nullptr;
    ImGuiWindowFlags plot_window_flags = ImGuiWindowFlags_None;
    ImPlotFlags plot_flags = ImPlotFlags_Equal | ImPlotFlags_NoMenus | ImPlotFlags_Crosshairs | ImPlotFlags_NoLegend;
    ImPlotFlags brush_preview_flags = ImPlotFlags_CanvasOnly | ImPlotFlags_Equal | ImPlotFlags_NoInputs | ImPlotFlags_NoFrame;
    ImPlotAxisFlags brush_preview_axes_flags = ImPlotAxisFlags_NoDecorations;

    ImPlotPoint mouse_plot_pos = { 0.0f,0.0f };
    hex_surface_index_data_type hex_grid_index = -1;
    std::string hex_grid_brush_name = "Brush";
    hex_surface_vec_data_type hex_grid_brush_coord = { 0,0 };
    T hex_grid_value = 0;

    bool change_extents_popup = false;

    int hex_grid_cell_data_id = -1;

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

    // Hex grid display paramters 
    bool display_ahex_grid = true;
    bool edit_ahex_grid = false;
    const char* shapes[9] = { "Circle", "Square", "Diamond", "Up", "Down", "Left", "Right", "Cross", "Asterix" };
    ImGuiComboFlags shapes_combo_flags = ImGuiComboFlags_NoArrowButton;
    signed char plot_axis_limit_changed = -1;
    ImVec4 hex_grid_display_color = { 0.5f,0.5f,1.0f,1.0f };
    int hex_grid_display_shape_id = 1;
    float hex_grid_display_shape_size = 6.0f;
    float hex_brush_preview_display_shape_size = 6.0f;
    ImPlotMarker_ hex_grid_display_shape = ImPlotMarker_::ImPlotMarker_Circle;

    float hex_grid_edit_value = 1.0f;
    ImVec4 hex_grid_edit_color = { 1.0f,1.0f,1.0f,1.0f };

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
        switch (hex_grid_display_shape_id) {
            case 0:  hex_grid_display_shape = ImPlotMarker_::ImPlotMarker_Circle; break;
            case 1:  hex_grid_display_shape = ImPlotMarker_::ImPlotMarker_Square; break;
            case 2:  hex_grid_display_shape = ImPlotMarker_::ImPlotMarker_Diamond; break;
            case 3:  hex_grid_display_shape = ImPlotMarker_::ImPlotMarker_Up; break;
            case 4:  hex_grid_display_shape = ImPlotMarker_::ImPlotMarker_Down; break;
            case 5:  hex_grid_display_shape = ImPlotMarker_::ImPlotMarker_Left; break;
            case 6:  hex_grid_display_shape = ImPlotMarker_::ImPlotMarker_Right; break;
            case 7:  hex_grid_display_shape = ImPlotMarker_::ImPlotMarker_Cross; break;
            case 8:  hex_grid_display_shape = ImPlotMarker_::ImPlotMarker_Plus; break;
            case 9:  hex_grid_display_shape = ImPlotMarker_::ImPlotMarker_Asterisk; break;
            default: hex_grid_display_shape = ImPlotMarker_::ImPlotMarker_Circle;
        }
    }

};