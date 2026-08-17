#pragma once

#include <filesystem>
#include <sys/stat.h>

#include <ThirdParty/ImGui/imgui_docking/imgui_internal.h>
#include <ImGui/imgui_docking/misc/cpp/imgui_stdlib.h>// Need This so as to be able to input text as a std::string variable

#include <FrameWork/AFW/ImGUI/ImGuiEx/imgui_custom.h>
#include <FrameWork/AFW/ImGUI/ImGuiEx/imgui_widgets.h>
#include <FrameWork/AFW/Interface/IconsFontAwesome4.h>
#include <FrameWork/AFW/tools/afw_string_tools.h>

#include "Source/HCP3D/hcp_grids/hcp_surface_import_export.h"

template <typename T>
class hcp_grid_class; // Need to forward declare this as have a case of circular referencing 

// HCP grid brush class to display a user interface to create, store and manage HCP grid
// brushes that are used to edit the main HCPagonal automata grid cell data values. 
// Similar to the maim hcp_grid_class except does not inherit hcp_grid_base_class.
// Each HCP Brush is a HCP grid class object and are stored as a vector list of these 
// HCP grid class objects.

template <class T>
class hcp_grid_brush_class {
    //using HGBC = hcp_grid_base_class<T>;
public:
    hcp_grid_brush_class() {}
	~hcp_grid_brush_class() {}

    std::string brush_pathname = "Brushes/HCP_Brushes";// default pathname to application sub directory
    std::filesystem::path item_path;

    bool edit_ahcp_grid = false;

    typedef std::pair < std::string, hcp_grid_class<T>*> hcp_grid_brush_list_data_type; // basic HCP grid brush datatype
    hcp_grid_brush_list_data_type hcp_grid_brush_selection;                             // Current selected HCP grid brush datatype
    std::vector <hcp_grid_brush_list_data_type > hcp_grid_brush_selections = {};        // vector list of HCP grid brushes to select
    
    hcp_grid_brush_list_data_type hcp_grid_brush_edit;                                  // Flag to edit a HCP grid brush
    hcp_grid_class<T> *hcp_grid_brush = nullptr;                                        // pointer to current selected HCP grid brush

    std::vector<hcp_grid_cell_data_struct_type> hcp_grid_cells_data_list = {};          // vector list of HCP grid brushes diaplay data to display on screen

    // Function to load existing HCP grid brush data from disc files into a list of
    //  HCP grid brushes and list of display color shape and size data
    void initialise_hcp_grid_brushes() {
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
                // ----------- Import HCP grid brush data ------------------
//printf("hcp_grid_brush_class::initialise_hcp_grid_brushes 11\n");
                hcp_grid_class<T> *hgrid_brush = create_hcp_grid_brush(2, 2, 2, hcp_grid_value);// Create a dummy HCP grid brush to import HCP grid brudh data into
//printf("hcp_grid_brush_class::initialise_hcp_grid_brushes 22\n");
                if (!hgrid_brush) {
                    afw_globalc::get_current_logger()->log(LogLevel::INFO, "Create HCP grid brush:: Unable to define HCP grid brush" + bpn +"\n");
                    continue;
                }
//printf("hcp_grid_brush_class::initialise_hcp_grid_brushes 33\n");
                hgrid_brush->clear_all_hcp_grid_cell_data();
                hgrid_brush->delete_hcp_grid();
//printf("hcp_grid_brush_class::initialise_hcp_grid_brushes 44\n");
		        hcp_surface_import_export.import_hcp_surface_automata_grid(bpn, hgrid_brush);
                hgrid_brush->define_hcp_grid_coordinates();
//printf("hcp_grid_brush_class::initialise_hcp_grid_brushes 55\n");
                for (size_t i = 0; i < hgrid_brush->hcp_colors.size(); i++) {
                    if (hgrid_brush->hcp_grid[i] > 0)
                        hgrid_brush->hcp_colors[i] = ImGui::GetColorU32(hcp_grid_edit_color);
                }

                // -------------- Define grid brush name to displsy in the brush list ---------------------
                FW::stringtools::removeSubstring(bpn, "HCPGB.txt");
                size_t pos1 = bpn.find_last_of('\\');                
//                std::cout << "1 : " << bpn << " : " << pos1 << std::endl;
                bpn.erase(0, pos1 + 1);
//                std::cout << "2 : " << bpn << std::endl;
                FW::stringtools::replace(bpn, "_", " ");
//                std::cout << "3 : " << bpn << std::endl;

                hcp_grid_brush_list_data_type new_brush;
                new_brush.first  = bpn;
                new_brush.second = hgrid_brush;

                add_brush_to_list(new_brush);

            }// else { // have a directory
                // not to be used but left here for reference
			//}
        }     

        // Initialise default HCP brush slection
        hcp_grid_brush_edit.first = "null";
        hcp_grid_brush_edit.second = nullptr;

        if (hcp_grid_brush_selections.size() > 0) {
            hcp_grid_brush_selection = hcp_grid_brush_selections[0];
        } else {
            hcp_grid_brush_selection.first  = "null";
            hcp_grid_brush_selection.second = nullptr;
        }
    }

    bool add_brush_to_list(hcp_grid_brush_list_data_type hcp_grid_brush) {
        //Check for HCP grid brush of same name
        for (size_t i = 0; i < hcp_grid_brush_selections.size(); i++) {
            if (hcp_grid_brush.first == hcp_grid_brush_selections[i].first) {
                vwDialogs::display_error_message("Add Brush ERROR:", "Could not add brush to selection list. \nHave an existing brush of the same name.");
                return false;
            }
        }

        // Check if have brush grid undefined
        if (!hcp_grid_brush.second) {
            vwDialogs::display_error_message("Add Brush ERROR:", "Could not add brush to selection list. \nBrush to add is undefined.");
            return false;
        }

        hcp_grid_brush_selections.push_back(hcp_grid_brush);

        return true;
    }

    bool save_brush_to_list(hcp_grid_brush_list_data_type &hcp_grid_brush) {
        // Check to see if have brush of same name
        for(hcp_grid_brush_list_data_type &hcp_grid_brush_selection: hcp_grid_brush_selections){
            if (hcp_grid_brush.first == hcp_grid_brush_selection.first) {// Have brush of the same name : overwrite it
                if (!hcp_grid_brush.second) {// Check if have brush grid undefined
                    vwDialogs::display_error_message("Save Brush ERROR:", "Could not Save brush to selection list. \nBrush to save is undefined.");
                    return false;
                } else {
//printf("hcp_grid_brush_class::save_brush_to_list AAAA %s %s\n", hcp_grid_brush_selection.first.c_str(), hcp_grid_brush.first.c_str());
//if(!hcp_grid_brush_selection.second) printf("hcp_grid_brush_class::save_brush_to_list ABABAB !hcp_grid_brush_selection.second\n");
                    if (hcp_grid_brush.second != hcp_grid_brush_selection.second) {
//printf("hcp_grid_brush_class::save_brush_to_list BBBB\n");
                        delete hcp_grid_brush_selection.second; // delete HCP grid data as it is to be replaced and no longer used
//printf("hcp_grid_brush_class::save_brush_to_list CCCC\n");
                        hcp_grid_brush_selection.second = hcp_grid_brush.second;
                    }
//printf("hcp_grid_brush_class::save_brush_to_list DDDD\n");
                    return true;
                }
            }
        }

        return add_brush_to_list(hcp_grid_brush);// No brush to overwrite : add brush to list
    }


    // The main ImGui window widget panel to display widgets for user to interact with to select
    // existing HCP brush to use for editing the main HCP automata grid, or to create/edit/save/load
    // HCP brush grid. If the editing of a HCP brush is activarted, a HCP brush edit window will
    // appear that operates similarly to the main HCP grid automata display window widget.
    void display_hcp_grid_brush_panel() {
        if (!brushes_list_initialised) { // need this or brushes will be added at infenitum every ImGui frame call
            initialise_hcp_grid_brushes();
            brushes_list_initialised = true;
        }

        // For some unknown reason naming the ImGui::Begin window below other than HCP Grid Brush causes ImPlot::BeginPlot to crash application with error
        // [01120] [imgui-error] (current settings: Assert=1, Log=1, Tooltip=1)
        // [01120][imgui - error] In window 'cart Grid Brush ': Setup needs to be called after BeginPlot and before any setup locking functions(e.g.PlotX)!
        //
//printf("hcp_grid_brush : display_hcp_grid_brush_panel 000\n");
        ImGui::Begin("hcp Grid Brush", nullptr, plot_window_flags);// If Have name of ImGui::Begin then have ability to minimise and dock 
        //ImGui::Begin("cart Grid Brush ", nullptr, plot_window_flags);// If Have name of ImGui::Begin then have ability to minimise and dock 

        // ###### This section of code is relevant to the main HCP brush grid display ###########
        // ###### window and if the mouse cusor is within it to perform editing tasks ###########
        // Test if the mouse cursor is within the bounds of the HCP brush display window and if it is
        // test for any valid user interactions with the HCP brush data and perform tasks according to what
        // keyboard or mouse interaction are initiated.
        if (hcp_grid_brush && display_hcp_grid_brush_edit) {
            ImGuiContext* cec = ImGui::GetCurrentContext();// Get current Imgui context of what is happening 
            bool edit_mode = true;// default mode of interaction

            if (cec && cec->HoveredWindow) {// Have mouse cursor hovering within an ImGui window
                if (cec->HoveredWindow->ID == plot_window->ID) { // If the mouse cursor is hovering over the window that the HCP brush is being displayed in
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
                    // If mouse cursor is within the bounds of the HCP brush display then can perform HCP brush edit functions
                    if (got_nearest_hcp_grid_cell) {
                        // Mouse cursor position is given in Cartesian coordinates of the HCP brush grid ImPlot being displayed, so need to convert mouse Cartesian
                        // coordinates to HCP brush grid index and HCP brush grid x-y index coordinates to be able to edit the HCP brush grid cell data values and retrieve them for display
                       hcp_grid_index = hcp_grid_brush->get_hcp_grid_index();// Need a mouse z plot location value here !!!!!

                        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) { //On left mouse click flip set the HCP brush call value to 1 and change the HCP brush grid display data
//printf("edit_grid::mouse button left : %i\n", hcp_grid_index);
                            if (hcp_grid_index > -1 && hcp_grid_index < hcp_grid_brush->hcp_grid.size()) {
                                hcp_grid_brush->hcp_grid[hcp_grid_index] = hcp_grid_edit_value;
                                hcp_grid_brush->hcp_colors[hcp_grid_index] = ImGui::GetColorU32(hcp_grid_edit_color);
//printf("edit_grid::mouse button left :index %i | value %i ::color | %0.3f | %0.3f | %0.3f | %0.3f\n", hcp_grid_index, hcp_grid_brush->hcp_grid[hcp_grid_index], hcp_grid_edit_color.x, hcp_grid_edit_color.y, hcp_grid_edit_color.z, hcp_grid_edit_color.w);
                            }

                        }

                        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {//On right mouse click flip set the HCP brush call value to 0 and change the HCP brush grid display data
//printf("edit_grid::mouse button right : %i\n", hcp_grid_index);
                            if (hcp_grid_index > -1 && hcp_grid_index < hcp_grid_brush->hcp_grid.size()) {
                                hcp_grid_brush->hcp_grid[hcp_grid_index] = 0.0f;
                                hcp_grid_brush->hcp_colors[hcp_grid_index] = ImGui::GetColorU32(hcp_grid_display_color);

//printf("edit_grid::mouse button right :index %i | value %i ::color | %0.3f| %0.3f| %0.3f| %0.3f\n", hcp_grid_index, hcp_grid_brush->hcp_grid[hcp_grid_index], hcp_grid_edit_color.x, hcp_grid_edit_color.y, hcp_grid_edit_color.z, hcp_grid_edit_color.w);
                            }

                        }

                       // if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
//printf("edit_grid::mouse button right : %i\n", hcp_grid_index);
                       // }
                    }

                }
                else {// Enable plot inputs and window interaction when HCP plot edit window is not hovered over in edit mode and disable edit functions
                    plot_flags = ImPlotFlags_Equal | ImPlotFlags_NoMenus | ImPlotFlags_Crosshairs | ImPlotFlags_NoLegend;
                    plot_window_flags = ImGuiWindowFlags_None;
                    edit_mode = false;
                }
            }
        }

        // ##################################################################################################

        // ###### This section of code is relevant to the display of the HCP brush grid edit widgets ###########
        // ###### to define the HCP brush grid edit inputs and perform HCP brush grid edit processes ###########
        // Display the current HCP brush grid data and coordinate values that the mouse cursor is hovering over in the
        // HCPa brush grid display window

        float x_pos = 10.0f, y_pos = 20.0f;

        text("HCP grid Brush", x_pos, y_pos);


        ImGui::Text("Select Brush To Use");
        static int item_selected_idx = 0; // Here we store our selected data as an index.

        //static bool item_highlight = false;
        int item_highlighted_idx = -1; // Here we store our highlighted data as an index.

        // Display list of available HCP brush grids to select for use or edit
        ImGui::SetNextItemWidth(300);
        if (ImGui::BeginListBox("##hbslb"))
        {
            for (int n = 0; n < hcp_grid_brush_selections.size(); n++)
            {
                const bool is_selected = (item_selected_idx == n);
                if (ImGui::Selectable(hcp_grid_brush_selections[n].first.c_str(), is_selected)) {
                    item_selected_idx = n;
//printf("hcp_grid_brush_class::listbox 0000 selection %i %s \n", item_selected_idx, items[item_selected_idx].c_str());
//printf("hcp_grid_brush_class::listbox 0000 selection %i %s \n", item_selected_idx, hcp_grid_brush_selections[item_selected_idx].first.c_str());
                }

                if (ImGui::IsItemHovered()) {
                    item_highlighted_idx = n;
//printf("hcp_grid_brush_class::listbox 1111 hovered %i %s \n", item_highlighted_idx, items[item_highlighted_idx].c_str());
//printf("hcp_grid_brush_class::listbox 1111 hovered %i %s \n", item_highlighted_idx, hcp_grid_brush_selections[item_highlighted_idx].first.c_str());
                }

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected){
                    ImGui::SetItemDefaultFocus();
                    hcp_grid_brush_selection = hcp_grid_brush_selections[item_selected_idx];
//printf("hcp_grid_brush_class::listbox 2222 is_selected %i %s \n", item_selected_idx, items[item_selected_idx].c_str());
//printf("hcp_grid_brush_class::listbox 2222 is_selected %i %s \n", item_selected_idx, hcp_grid_brush_selections[item_selected_idx].first.c_str());
                }
            }
            ImGui::EndListBox();
        }

        ImGui::SameLine();
        ImGui::Text("Current Selected Brush :\n %s", hcp_grid_brush_selection.first.c_str());

//if (!hcp_grid_brush_selection.second) printf("!hcp_grid_brush_selection.second\n");

        // Display a preview of the selected HCP brush in a ImPlot::ScatterPlot widget that the user cannot interact with
        if (hcp_grid_brush_selection.second) {// Test that have a valid HCP brush grid selected
//printf("hcp_grid_brush_class::display_hcp_grid_brush_panel hcp_grid_brush_selection.second : %i : %i : %i\n", hcp_grid_brush_selection.second->hcp_centers_x.size(), hcp_grid_brush_selection.second->hcp_centers_y.size(),hcp_grid_brush_selection.second->hcp_colors.size());

            int brush_max_dim = std::max((std::max(hcp_grid_brush_selection.second->grid_dimension.x, hcp_grid_brush_selection.second->grid_dimension.y)), hcp_grid_brush_selection.second->grid_dimension.z);
            float brush_preview_size = ((brush_preview_range_size/brush_preview_window_size) / float(brush_max_dim))*(0.5f* brush_preview_scale)* brush_preview_range_size;

            ImGui::SetCursorPosX(320);
            ImGui::SetCursorPosY(y_pos + 60);
            ImPlot3D::BeginPlot("HCP Grid Brush", ImVec2(brush_preview_window_size, brush_preview_window_size), brush_preview_flags);
                ImPlot3D::SetupAxes(nullptr, nullptr, nullptr, brush_preview_axes_flags, brush_preview_axes_flags);
                ImPlot3D::SetupAxesLimits(-1.25f, float(brush_max_dim)*2.0f, -1.25f, float(brush_max_dim)*2.0f, -1.25f, float(brush_max_dim) * 2.0f, ImGuiCond_None);

                ImPlot3DSpec spec;
                //ImPlotProp_Marker, ImPlotMarker_Circle, 
                //ImPlotProp_Marker, ImPlotMarker_Square,// use this when testing
                spec.Marker = hcp_grid_display_shape;
                spec.MarkerSize = brush_preview_size;
                spec.MarkerFillColors = hcp_grid_brush_selection.second->hcp_colors.data();
                spec.MarkerLineColors = hcp_grid_brush_selection.second->hcp_colors.data();
                spec.FillAlpha = 0.25f;
                //spec.Flags = ImPlotProp_Flags;
                //spec.ImPlotFlags_Equal

                ImPlot3D::PlotScatter("Selected HCP brush", hcp_grid_brush_selection.second->hcp_centers_x.data(), hcp_grid_brush_selection.second->hcp_centers_y.data(), hcp_grid_brush_selection.second->hcp_centers_z.data(), hcp_grid_brush_selection.second->hcp_centers_x.size(), spec);
            ImPlot3D::EndPlot();
        }

        // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        // ###### Widget to display the HCP brush edit window to edit selected or create a new HCP brush #####
        ImGui::SetCursorPosY(ImGui::GetCursorPosY()+10);
        ImGui::Text("Edit/Create Brush : ");
        ImGui::SameLine();
        // interactively display edit HCP grid brush
        ImGui::Checkbox("##hgebd", &display_hcp_grid_brush_edit);

        // Display HCP brush widgets that the user can interact with to edit a HCP brush grid
        if (display_hcp_grid_brush_edit) {// Display HCP brush edit widgets if flag to edit HCP grid brush is set to true
            ImGui::Separator();
            ImGui::Text("Edit/Create HCP grid Brush");

            ImGui::Text("HCP Grid Coordinate ::   ");
            ImGui::SameLine();
            ImGui::Text("X : %i | ", hcp_grid_brush_coord.x);
            ImGui::SameLine();
            ImGui::Text("Y : %i", hcp_grid_brush_coord.y);
            ImGui::SameLine();
            ImGui::Text("Z : %i", hcp_grid_brush_coord.z);

            ImGui::Text("HCP Grid Index : %i  ||", hcp_grid_index);
            ImGui::SameLine();

            if (hcp_grid_index > -1 && hcp_grid_index < hcp_grid_brush->hcp_grid.size()) hcp_grid_value = hcp_grid_brush->hcp_grid[hcp_grid_index];

            ImGui::Text(" HCP Grid value : %i", hcp_grid_value); // Need to accomidate all data types of HCP grid here !!!!
            ImGui::Separator();

            // ------------------------------------------------------
            ImGui::Text("Grid Color : ");
            ImGui::SameLine();
            // interactively change result display color
            if (ImGui::ColorEdit4("##hbgc", (float*)&hcp_grid_display_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_None)) {
                if (hcp_grid_brush) {
                    hcp_grid_base_class<T>* bc = hcp_grid_brush;// This is to access the base class of hcp_grid_class
                    bc->hcp_grid_display_color = hcp_grid_display_color;

                    for (size_t i = 0; i < hcp_grid_brush->hcp_colors.size(); i++) {
                        if (hcp_grid_brush->hcp_colors[i] != ImGui::GetColorU32(hcp_grid_edit_color)) {
                            hcp_grid_brush->hcp_colors[i] = ImGui::GetColorU32(hcp_grid_display_color);
                        }
                    }
                }
            }
            //ImGui::ColorEdit4("##hgc", (float*)hcp_grid_brush->hcp_grid_display_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_None); //This generates compilor error
            ImGui::SameLine();
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
            ImGui::DragFloat("##hgsi", &hcp_grid_display_shape_size, 0.1f, 1.0f, 50.0f, "%2.2f");
            ImGui::SameLine();
            ImGui::Text("Brush Color : ");
            ImGui::SameLine();
            // interactively change edit display color
            if (ImGui::ColorEdit4("##hgbc", (float*)&hcp_grid_edit_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_None)) {
                if (hcp_grid_brush) {
                    //hcp_grid_base_class<T>* bc = hcp_grid_brush;
                    //bc->hcp_grid_display_color = hcp_grid_display_color;
                    for (size_t i = 0; i < hcp_grid_brush->hcp_colors.size(); i++) {
                        if (hcp_grid_brush->hcp_colors[i] != ImGui::GetColorU32(hcp_grid_display_color)) {
                            hcp_grid_brush->hcp_colors[i] = ImGui::GetColorU32(hcp_grid_edit_color);
                        }
                    }
                }
            }
            // ----------------------------------        

            ImGui::Separator();

            x_pos += 55;
            y_pos += 300;
            if (ex_button("Edit Current selected Brush", x_pos + 100, y_pos, 200, 20)) {
 //printf("create_hcp_grid_panel_class::display 1111 \n");
                if (!hcp_grid_brush_selection.second) {
                    vwDialogs::display_error_message("Edit Current Brush ERROR:", "Can not edit current selected brush. \nIt is undefined.");
                } else{
                    hcp_grid_brush_edit = hcp_grid_brush_selection;
                    hcp_grid_brush = hcp_grid_brush_selection.second;
                    hcp_grid_brush_defined = true;
                    edit_ahcp_grid = true;
                }
            }

            y_pos += 30;
            if (ex_button("Create New HCP Grid Brush", x_pos + 100, y_pos, 200, 20)) {
//printf("create_hcp_grid_panel_class::display 1111 \n");
                create_hcp_grid_brush_popup = true;
            }

/* !!!!!!! A Future feature for a more advanced brush editing suite implementation !!!!!!!!!!!!!!!!
            y_pos += 30;
            if (ex_button("Change HCP Brush size", x_pos + 100, y_pos, 180, 20)) {
                show_change_brush_size_widget = true;
            }

            if (show_change_brush_size_widget) {
                bool canceled_change_brush_size = false;

                change_hcp_grid_brush_size(x_pos,y_pos,canceled_change_brush_size);

                if (canceled_change_brush_size) show_change_brush_size_widget = false;
            }
*/
            
            y_pos += 30;
            if (ex_button("Add Brush To List", x_pos + 100, y_pos, 200, 20)) {
                add_brush_to_list(hcp_grid_brush_edit);
                edit_ahcp_grid = false;
            }

            y_pos += 30;
            if (ex_button("Save Brush To List", x_pos + 100, y_pos, 200, 20)) {
                save_brush_to_list(hcp_grid_brush_edit);
                edit_ahcp_grid = false;
            }

            y_pos += 30;
            if (ex_button("Import HCP Grid Brush", x_pos + 10, y_pos, 180, 20)) {
                import_hcp_grid_brush();
                edit_ahcp_grid = false;
            }

            if (ex_button("Export New HCP Grid Brush", x_pos + 210, y_pos, 180, 20)) {
                export_hcp_grid_brush();
                edit_ahcp_grid = false;
            }

            if (create_hcp_grid_brush_popup) {
                bool canceled_hcp_brush = false;
                hcp_grid_brush_list_data_type new_hcp_grid_brush = display_create_hcp_grid_brush_popup(canceled_hcp_brush);
                if (new_hcp_grid_brush.second) {
                    hcp_grid_brush = new_hcp_grid_brush.second;
                    hcp_grid_brush_edit = new_hcp_grid_brush;
                    hcp_grid_brush_defined = true;
                    create_hcp_grid_brush_popup = false;
                    edit_ahcp_grid = true;
                } else{
                    edit_ahcp_grid = false;
                }

                if (canceled_hcp_brush) { 
                    create_hcp_grid_brush_popup = false;
                    edit_ahcp_grid = false;
                }
            }


        }

        if (hcp_grid_brush_defined && display_hcp_grid_brush_edit) {// Display HCP brush edit window widget of HCP brush grid cell data
//printf("ahcp_application_class: display_ahcp_main_gui_panel : 0000  \n");
            if (hcp_grid_brush_edit.second) {
                // hcp_grid_brush->display_hcp_grid(); do not use this
                if (!hcp_grid_brush_edit_display()) { // Display HCP brush edit window widget of HCP brush grid cell data
                    vwDialogs::display_error_message("Edit Brush ERROR:", "Can not edit current selected brush. \nIt is undefined.");
                    hcp_grid_brush_defined = false;
                }
            }
        }

        ImGui::End();
    }

    // Popup window widget function to define inputs to initiate and create a new HCP brush grid to edit
    hcp_grid_brush_list_data_type display_create_hcp_grid_brush_popup(bool &cancel = false) {
//printf("hcp_grid_brush_class::display_create_hcp_grid_brush_popup AAAAA \n");

		ImGui::OpenPopup("Create HCP Grid Brush");

		// Always center this window when appearing
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        hcp_grid_brush_list_data_type return_value;
        return_value.first  = "Brush";
        return_value.second = nullptr;

		if (ImGui::BeginPopupModal("Create HCP Grid Brush", NULL, ImGuiWindowFlags_AlwaysAutoResize))// Must have same id name as ImGui::OpenPopup to display
		{
//printf("hcp_grid_brush_class::display_create_hcp_grid_brush_popup 00000 \n");
	
			float x_pos = 10.0f, y_pos = 40.0f;

            text("Define Global HCP grid", x_pos, y_pos);

            x_pos = 20; y_pos += 30;
            text("Brush Name : ", x_pos, y_pos);
            ImGui::SameLine();
            ImGui::InputText("##nhgbn", &hcp_grid_brush_name, ImGuiInputTextFlags_CallbackCharFilter, Input_Filters::name);// ++++

            x_pos = 20; y_pos += 30;
            text("Brush X :", x_pos, y_pos);
            integer_input("###HCPgxd", xdim, x_pos + 100, y_pos - 3, 100.0f);

            y_pos += 30;
            text("Brush Y :", x_pos, y_pos);
            integer_input("###HCPgyd", ydim, x_pos + 100, y_pos - 3, 100.0f);

            y_pos += 30;
            text("Brush Z :", x_pos, y_pos);
            integer_input("###HCPgzd", zdim, x_pos + 100, y_pos - 3, 100.0f);

            // for possible future implementation
            //y_pos += 30;
            //text("Initial HCP cell value:", x_pos, y_pos);
            //float_input("###HCPghs", hcp_grid_value, x_pos + 250, y_pos - 3, 100.0f);

            y_pos += 30;
            if (ex_button("Create HCP Grid Brush", x_pos + 10, y_pos, 180, 20)) {
//printf("create_hcp_grid_panel_class::display 1111 \n");
                return_value.first  = hcp_grid_brush_name;
                return_value.second = create_hcp_grid_brush(xdim, ydim,zdim, hcp_grid_value);
            }

            if (ex_button("Cancel###chgcb", x_pos + 210, y_pos, 100, 20)) {
                cancel = true;
            }

//printf("hcp_grid_brush_class::display_create_hcp_grid_brush_popup 22222 \n");		

			ImGui::EndPopup();

		}
//printf("hcp_grid_brush_class::display_create_hcp_grid_brush_popup 3333 \n");

		return return_value;
    }

    // ImGui window widget to display the HCP brush grid cell data using the ImPlot::ScatterPlot function. 
    bool hcp_grid_brush_edit_display(){
        hcp_grid_class<T> *hcp_grid_brush = hcp_grid_brush_edit.second;

        if (!hcp_grid_brush) {// comment out when confident this function will not be called if hcp_grid_brush_edit.second is a nullptr
//printf("hcp_grid_brush_class::hcp_grid_brush_edit_display ERROR: HCP grid brush to edit is a nullptr\n");
            return false;
        }

        ImGui::Begin("HCP Grid Brush", nullptr, plot_window_flags);// If Have name of ImGui::Begin then have ability to minimise and dock 

        if (ImPlot3D::BeginPlot("HCP Grid Brush",ImVec2(-1, -1), plot_flags)) {

            ImPlotStyle& style = ImPlot::GetStyle();
            ImGuiContext *cpc = ImGui::GetCurrentContext();// Strange compilor does not complain about ImGuiContext without including imgui_internal.h

            // Test and functions to maintain the display aspect ratio of the ImGui-ImPlot window
            // when performing windo resizing using the window borders. This is performed 
            // so as to avoid strange disparity of the display of HCP grid where the grid X-Y axis
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
//    printf("display_hcp_grid : px %f : py %f: x %f : y %f \n", prev_width, prev_height,cpc->CurrentWindow->Size.x, cpc->CurrentWindow->Size.y);

            // Constrain the HCP grid display to a min max Cartesian coordinate range of coordinates
            ImPlot3D::SetupAxisLimitsConstraints(ImAxis3D_X, plot_min_x, plot_max_x);
            //ImPlot::SetupAxisZoomConstraints(ImAxis_X1, plot_min_zoom, plot_max_zoom);
            ImPlot3D::SetupAxisLimitsConstraints(ImAxis3D_Y, plot_min_y, plot_max_y);
            //ImPlot::SetupAxisZoomConstraints(ImAxis_Y1, plot_min_zoom, plot_max_zoom);
            ImPlot3D::SetupAxisLimitsConstraints(ImAxis3D_Z, plot_min_z, plot_max_z);

            // ImPlot::ScatterPlot function to display main HCP grid cell data if flag is set to display HCP brush grid
            if (display_ahcp_grid) {
                ImPlot3DSpec spec;
                //ImPlotProp_Marker, ImPlotMarker_Circle, 
                //ImPlotProp_Marker, ImPlotMarker_Square,// use this when testing
                spec.Marker           = hcp_grid_display_shape;
                spec.MarkerSize       = hcp_grid_display_shape_size;
                spec.MarkerFillColors = hcp_grid_brush->hcp_colors.data();
                spec.MarkerLineColors = hcp_grid_brush->hcp_colors.data();
                spec.FillAlpha = 0.25f;
                //spec.Flags = ImPlotProp_Flags;
                //spec.ImPlotFlags_Equal

                ImPlot3D::PlotScatter("hcp brush", hcp_grid_brush->hcp_centers_x.data(), hcp_grid_brush->hcp_centers_y.data(), hcp_grid_brush->hcp_centers_z.data(), hcp_grid_brush->hcp_centers_x.size(), spec);
            }

            ImDrawList* draw_list = ImPlot3D::GetPlotDrawList();
            ImVec2 mouse_pos = ImGui::GetMousePos();
            ImVec2 plot_pos = ImPlot3D::GetPlotRectPos();
            ImVec2 plot_size = ImPlot3D::GetPlotRectSize();

            if (edit_ahcp_grid) { 
                got_nearest_hcp_grid_cell = hcp_grid_brush->get_display_nearest_hcp_matrix_grid_cell_to_cursor(draw_list, mouse_pos, plot_pos, plot_size);
            }

            hcp_grid_brush->display_orientation_gizmo(draw_list, mouse_pos, plot_pos, plot_size);


//            mouse_plot_pos = ImPlot::GetPlotMousePos(); // This must be placed within the BeginPlot : EndPlot block or application will crash
//
//            for (hcp_grid_cell_data_struct_type hcp_grid_cell : hcp_grid_cells_data_list) {
////printf("hcp_grid_class::display_hcp_grid 3333\n");
//               hcp_grid_cell.display_hcp_cell_data();
//            }
            
            ImPlot3D::EndPlot();
        }

        plot_axis_limit_changed = IMGUI_WINDOW_BORDER_NONE;

        ImGui::End();

        return true;
}

    hcp_grid_class<T>* create_hcp_grid_brush(hcp_surface_index_data_type xdim, hcp_surface_index_data_type ydim, hcp_surface_index_data_type zdim, float hcp_grid_value) {
//printf("create_hcp_grid_panel_class::create_global_grid 00000 \n");

        if (xdim < 1 || ydim < 1 || zdim < 1) {
            //imgui_message_model_popup("HCP Grid Error Message", "Create HCP grid", "Cannot create HCP grid : \n HCP grid xdimension is of zero size", dummy);
            return nullptr;
        }

        hcp_grid_class<T>* new_hcp_grid_brush = new hcp_grid_class<T>(hcp_grid_value, { xdim, ydim, zdim });

        if (!new_hcp_grid_brush) {
            afw_globalc::get_current_logger()->log(LogLevel::INFO, "Create HCP grid brush:: Unable to define HCP grid brush.\n");
            return nullptr;
        }

        new_hcp_grid_brush->define_hcp_grid_coordinates();

        return new_hcp_grid_brush;
    }

    /* !!!!!!! A Future feature for a more advanced brush editing suite implementation !!!!!!!!!!!!!!!!
    void change_hcp_grid_brush_size(hcp_grid_class<T> *hcp_grid_brush,float &x_pos,float &y_pos, bool &cancel) {
        
       // ImGui::Begin("Change HCP Grid Brush size");// Must have same id name as ImGui::OpenPopup to display
        
//printf("hcp_grid_brush_class::display_create_hcp_grid_brush_popup 00000 \n");

        if (!hcp_grid_brush) {
            vwDialogs::display_error_message("Change HCP grid brush size", "ERROR ::\n No HCP grid brush defined to change brush size.\n");
            cancel = true;
            return;
        }

        if (hcp_grid_brush->hcp_grid.size() < 1) {
            vwDialogs::display_error_message("Change HCP grid brush size", "ERROR ::\n No HCP grid brush defined to change brush size.\n");
            cancel = true;
            return;
        }

        hcp_surface_index_data_type new_xdim = 5, new_ydim = 5;
        std::vector<T> new_hcp_grid = {};

        ImGui::Separator();

        x_pos = 20; y_pos += 40;
        text("New Brush Width  :", x_pos+120, y_pos);
        integer_input("###HCPgbcxd", nxdim, x_pos + 250, y_pos - 3, 100.0f);

        y_pos += 30;
        text("New Brush Height :", x_pos+120, y_pos);
        integer_input("###HCPgbcyd", nydim, x_pos + 250, y_pos - 3, 100.0f);

        y_pos += 30;
        if (ex_button("Change HCP Grid Brush Size", x_pos + 10, y_pos, 190, 20)) {
//printf("create_hcp_grid_panel_class::display 1111 \n");
            nhcp_grid = create_hcp_grid_brush(new_xdim, new_ydim, float hcp_grid_value);
            if (!new_hcp_grid) {
                vwDialogs::display_error_message("Change HCP grid brush size", "ERROR ::\n Could not define new HCP grid brush of specified size.\n");
            } else{
                hcp_surface_index_data_type old_xdim = hcp_grid_brush->grid_dimension.x, old_ydim = hcp_grid_brush->grid_dimension.y;

                hcp_surface_index_data_type x_shift = (new_xdim - old_xdim)/2;
                hcp_surface_index_data_type y_shift = (new_ydim - old_ydim)/2;

                

                if (new_ydim < old_ydim) {

                } else {
                
                }
            }
        }

        if (ex_button("Finish Brush Size Change###chgbccb", x_pos + 210, y_pos, 190, 20)) {
            cancel = true;
        }

        ImGui::Separator();
//printf("hcp_grid_brush_class::display_create_hcp_grid_brush_popup 22222 \n");		

         //   ImGui::End();
    }
*/

    void import_hcp_grid_brush() {
//printf("hcp_grid_brush_class::import_hcp_grid_brush 00\n");
		char const* patterns[] = { "*HCPGB.txt" };
		char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			//if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No automata rules file defined to import HCP surface generation parameter data from.\n");
			return;
		}

        if (!hcp_grid_brush) {
//printf("hcp_grid_brush_class::import_hcp_grid_brush 11\n");
            hcp_grid_brush = create_hcp_grid_brush(2,2,2,hcp_grid_value);// Create a dummy HCP grid brush to import HCP grid brudh data into
        }

//printf("hcp_grid_brush_class::import_hcp_grid_brush 22\n");
        if (!hcp_grid_brush) {
            afw_globalc::get_current_logger()->log(LogLevel::INFO, "Create HCP grid brush:: Unable to define HCP grid brush.\n");
            return;
        }

//printf("hcp_grid_brush_class::import_hcp_grid_brush 33\n");
        hcp_grid_brush->clear_all_hcp_grid_cell_data();
        hcp_grid_brush->delete_hcp_grid();

//printf("hcp_grid_brush_class::import_hcp_grid_brush 44\n");
		hcp_surface_import_export.import_hcp_surface_automata_grid(file_pathname, hcp_grid_brush);
        hcp_grid_brush->define_hcp_grid_coordinates();

//printf("hcp_grid_brush_class::import_hcp_grid_brush 55\n");

            for (size_t i = 0; i < hcp_grid_brush->hcp_colors.size(); i++) {
                if (hcp_grid_brush->hcp_grid[i] > 0) {
                    hcp_grid_brush->hcp_colors[i] = ImGui::GetColorU32(hcp_grid_edit_color);
                }
            }

            hcp_grid_brush_defined = true;

//printf("hcp_grid_brush_class::import_hcp_grid_brush 66\n");

    }

    void export_hcp_grid_brush() {

//printf("hcp_surface_automata_widget_class :: save_automata_rules\n");// replace with clear variables
		char const* patterns[] = { "*HCPGB.txt" };
		char const* file_pathname = vwDialogs::save_file(nullptr, patterns, 1);

		if (file_pathname == nullptr) {
			//if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : No automata rules file defined to save HCP surface automata rules data to\n");
			vwDialogs::display_error_message("Save HCP Grid Brush", "ERROR : \n No HCP grid brush data file defined to save HCP grid brush data to\nSave HCP grid Brush data aborted");
			return;
		}
//else
//printf("save_generation_parameters != NULL %s \n", file_pathname);
//printf("save_automata_rules != NULL  \n");

		hcp_surface_import_export.export_hcp_surface_automata_grid(file_pathname, hcp_grid_brush);
    }

private:
    bool hcp_grid_brush_defined = false;
    bool create_hcp_grid_brush_popup = false;

    bool display_hcp_grid_brush_edit   = false;
    bool show_change_brush_size_widget = false;
    bool brushes_list_initialised      = false;

    float brush_preview_window_size = 100.0f;
    float brush_preview_range_size = (100.0f / 120.0f) * 100;
    float brush_preview_scale = .25f;

    hcp_surface_import_export_class<T> hcp_surface_import_export;

    hcp_surface_index_data_type xdim = 5, ydim = 5, zdim = 5;

    ImGuiWindow* plot_window = nullptr;
    ImGuiWindowFlags plot_window_flags = ImGuiWindowFlags_None;
    ImPlotFlags plot_flags = ImPlotFlags_Equal | ImPlotFlags_NoMenus | ImPlotFlags_Crosshairs | ImPlotFlags_NoLegend;
    ImPlotFlags brush_preview_flags = ImPlotFlags_CanvasOnly | ImPlotFlags_Equal | ImPlotFlags_NoInputs | ImPlotFlags_NoFrame;
    ImPlotAxisFlags brush_preview_axes_flags = ImPlotAxisFlags_NoDecorations;

    ImPlotPoint mouse_plot_pos = { 0.0f,0.0f };
    hcp_surface_index_data_type hcp_grid_index = -1;
    std::string hcp_grid_brush_name = "Brush";
    hcp_surface_vec_data_type hcp_grid_brush_coord = { 0,0,0 };
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
    float plot_max_z = 10.0f;

    float prev_plot_min_x = -1.0f;
    float prev_plot_max_x = 50.0f;
    float prev_plot_min_y = -1.0f;
    float prev_plot_max_y = 50.0f;
    float prev_plot_min_z = -1.0f;
    float prev_plot_max_z = 50.0f;

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

    // HCP grid display paramters 
    bool display_ahcp_grid = true;
    //bool edit_ahcp_grid = false;
    const char* shapes[9] = { "Circle", "Square", "Diamond", "Up", "Down", "Left", "Right", "Cross", "Asterix" };
    ImGuiComboFlags shapes_combo_flags = ImGuiComboFlags_NoArrowButton;
    signed char plot_axis_limit_changed = -1;
    ImVec4 hcp_grid_display_color = { 0.5f,0.5f,1.0f,1.0f };
    int hcp_grid_display_shape_id = 1;
    float hcp_grid_display_shape_size = 6.0f;
    float hcp_brush_preview_display_shape_size = 6.0f;
    ImPlotMarker_ hcp_grid_display_shape = ImPlotMarker_::ImPlotMarker_Circle;

    float hcp_grid_edit_value = 1.0f;
    ImVec4 hcp_grid_edit_color = { 1.0f,1.0f,1.0f,1.0f };

    int min_grad_value_i = 0;
    int max_grad_value_i = 100;

    bool got_nearest_hcp_grid_cell = false;

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
        switch (hcp_grid_display_shape_id) {
            case 0:  hcp_grid_display_shape = ImPlotMarker_::ImPlotMarker_Circle; break;
            case 1:  hcp_grid_display_shape = ImPlotMarker_::ImPlotMarker_Square; break;
            case 2:  hcp_grid_display_shape = ImPlotMarker_::ImPlotMarker_Diamond; break;
            case 3:  hcp_grid_display_shape = ImPlotMarker_::ImPlotMarker_Up; break;
            case 4:  hcp_grid_display_shape = ImPlotMarker_::ImPlotMarker_Down; break;
            case 5:  hcp_grid_display_shape = ImPlotMarker_::ImPlotMarker_Left; break;
            case 6:  hcp_grid_display_shape = ImPlotMarker_::ImPlotMarker_Right; break;
            case 7:  hcp_grid_display_shape = ImPlotMarker_::ImPlotMarker_Cross; break;
            case 8:  hcp_grid_display_shape = ImPlotMarker_::ImPlotMarker_Plus; break;
            case 9:  hcp_grid_display_shape = ImPlotMarker_::ImPlotMarker_Asterisk; break;
            default: hcp_grid_display_shape = ImPlotMarker_::ImPlotMarker_Circle;
        }
    }

};