#pragma once

#include <thirdparty/ImGUI/imgui_docking/imgui.h>

#include <FrameWork/AFW/ImGUI/ImGuiEx/imgui_widgets.h>

#include <FrameWork/AFW/Interface/IconsFontAwesome4.h>

#include "Source/Hex2d/Hex_grids/hex_grid.h"

// Class to display ImGui popups to create hex automata grid and subgrid and create them

template <class T>
class create_hex_grid_panel_class {
public:
	// Popup to display parameters for user to enter values to create and create application main hex automata  grid 
	hex_grid_class<T>* create_global_hex_popup(bool &cancel=false) {
//printf("create_hex_grid_panel_class::display AAAAA \n");
		ImGui::OpenPopup("Global Hex Grid");

		// Always center this window when appearing
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		hex_grid_class<T>* return_value = nullptr;

		if (ImGui::BeginPopupModal("Global Hex Grid", NULL, ImGuiWindowFlags_AlwaysAutoResize))// Must have same id name as ImGui::OpenPopup to display
		{
//printf("create_hex_grid_panel_class::display 00000 \n");
			static hex_surface_index_data_type xdim=20, ydim=20;
			static float hex_cell_value = 0.0f;
	
			float x_pos = 10.0f, y_pos = 20.0f;

			text("Define Global Hex grid", x_pos, y_pos);

			x_pos = 20; y_pos += 20;
			text("Number of Columns (x Dimension) :", x_pos, y_pos);
			integer_input("###hexgxd", xdim, x_pos +250, y_pos-3, 100.0f);

			y_pos += 30;
			text("Number of Rows (y dimanesion) :", x_pos, y_pos);
			integer_input("###hexgyd", ydim, x_pos + 250, y_pos-3, 100.0f);

			y_pos += 30;
			text("Initial Hex cell value:", x_pos, y_pos);
			float_input("###hexghs", hex_cell_value, x_pos + 250, y_pos-3 , 100.0f);

			y_pos += 30;
			if (ex_button("Create Global Hex Grid", x_pos + 10, y_pos, 180, 20)) {
//printf("create_hex_grid_panel_class::display 1111 \n");
				return_value = create_global_hex_grid(xdim, ydim, hex_cell_value);
			}

			if (ex_button("Cancel###chgcb", x_pos + 210, y_pos, 100, 20)) {
				cancel = true;
			}

//printf("create_hex_grid_panel_class::display 22222 \n");		

			ImGui::EndPopup();

		}
//printf("create_hex_grid_panel_class::display 3333 \n");

		return return_value;

	}

	// Popup to display parameters for user to enter values to create and create application hex automata sub grid 
	bool add_hex_sub_grid_popup(hex_grid_class<T> * hex_grid, bool& cancel = false) {
//printf("create_hex_grid_panel_class::display AAAAA \n");

		bool return_value = false;
		static int  error_code   = 0;
		static bool dummy = false;

		//if (error_message) {
		if (!hex_grid) {
			imgui_message_model_popup("Child Hex Grid Error Message", "Define Child Hex grid", "Cannot create child hex grid : \nNo global or parent hex grid defined to have as a parent of.", cancel);
		} else {
			ImGui::OpenPopup("Hex Sub Grid");

			// Always center this window when appearing
			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

			if (ImGui::BeginPopupModal("Hex Sub Grid", NULL, ImGuiWindowFlags_AlwaysAutoResize)){
//printf("create_hex_grid_panel_class::display 00000 \n");
				static hex_surface_vec_data_type   min_coord = { 0,0 }, max_coord{ 0,0 };
				float hex_cell_value = 1.0f;

				float x_pos = 10.0f, y_pos = 20.0f;

				text("Create child hex grid", x_pos, y_pos);

				y_pos += 30;
				text("Min Hex sub grid coordinate : ", x_pos, y_pos);

				x_pos = 20; y_pos += 20;
				text("Min X :", x_pos, y_pos);
				integer_input("###hexgxd", min_coord.x, x_pos + 50, y_pos - 3, 50.0f);

				text("Min Y :", x_pos+150, y_pos);
				integer_input("###hexgyd", min_coord.y, x_pos + 200, y_pos - 3, 50.0f);

				y_pos += 30;
				text("Max Hex sub grid coordinate : ", x_pos, y_pos);

				y_pos += 30;
				text("Max X :", x_pos, y_pos);
				integer_input("###hgxoc", max_coord.x, x_pos + 50, y_pos - 3, 50.0f);

				text("Max Y :", x_pos+150, y_pos);
				integer_input("###hgyoc", max_coord.y, x_pos + 200, y_pos - 3, 50.0f);


				y_pos += 30;
				if (ex_button("Create child Hex Grid", x_pos + 10, y_pos, 180, 20)) {
//printf("create_hex_grid_panel_class::display 1111 \n");
					error_code = 0;
					dummy = false;
					return_value = create_hex_sub_grid(hex_grid, min_coord, max_coord, error_code);
				}

				if (ex_button("Cancel###chgcb", x_pos + 210, y_pos, 100, 20)) {
					cancel = true;
				}

				if (error_code > 0 and !dummy) {
					
					switch (error_code) {
						case 1:	imgui_message_model_popup("Hex sub Grid Error Message", "Define Hex sub grid", "Cannot create hex sub grid : \n hex sub grid x lower range greater than x upper range.", dummy); break;
						case 2:	imgui_message_model_popup("Hex sub Grid Error Message", "Define Hex sub grid", "Cannot create hex sub grid : \n hex sub grid y lower range greater than y upper range.", dummy); break;
						case 3:	imgui_message_model_popup("Hex sub Grid Error Message", "Define Hex sub grid", "Cannot create hex sub grid : \n hex sub grid origin x coordinate is outside the x bounds of the parent hex grid", dummy); break;
						case 4:	imgui_message_model_popup("Hex sub Grid Error Message", "Define Hex sub grid", "Cannot create hex sub grid : \n hex sub grid origin y coordinate is outside the y bounds of the parent hex grid", dummy); break;
						case 5:	imgui_message_model_popup("Hex sub Grid Error Message", "Define Hex sub grid", "Cannot create hex sub grid : \n hex sub grid origin x extents is outside the x bounds of the parent grid", dummy); break;
						case 6:	imgui_message_model_popup("Hex sub Grid Error Message", "Define Hex sub grid", "Cannot create hex sub grid : \n hex sub grid origin y extents is outside the y bounds of the parent grid", dummy); break;
						//case 7:	imgui_message_model_popup("Hex sub Grid Error Message", "Define Hex sub grid", "Cannot create hex sub grid : \n not sub allocate memory to define child grid. ", dummy); break;
					}
				}
//printf("create_hex_grid_panel_class::display 22222 \n");		

				ImGui::EndPopup();

			}
		}
//printf("create_hex_grid_panel_class::display 3333 \n");

		return return_value;

	}

protected:

	hex_grid_class<T>* create_global_hex_grid(hex_surface_index_data_type xdim, hex_surface_index_data_type ydim, float hex_cell_value) {
//printf("create_hex_grid_panel_class::create_global_grid 00000 \n");

		if (xdim < 1 || ydim < 1) {
			//imgui_message_model_popup("Hex Grid Error Message", "Create Hex grid", "Cannot create hex grid : \n Hex grid xdimension is of zero size", dummy);
			return nullptr;
		}

		hex_grid_class<T> *global_hex_grid = new hex_grid_class<T>(hex_cell_value, { xdim, ydim });

		if (!global_hex_grid) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "Create glabal hex grid :: Unable to define global Hex grid.\n");
			return nullptr;
		}

		global_hex_grid->define_hex_grid_coordinates();

		return global_hex_grid;
	}

	bool create_hex_sub_grid(hex_grid_class<T>* hex_grid, hex_surface_vec_data_type lower_range, hex_surface_vec_data_type upper_range, int &error_code) {
//printf("create_hex_grid_panel_class::create_global_grid 00000 \n");
		//bool cancel = false;

		if (!hex_grid) {return false;}

		if (upper_range.x < lower_range.x) { error_code = 1; }
		if (upper_range.y < lower_range.y) { error_code = 2; }

		if (lower_range.x < hex_grid->global_grid_origin.x || lower_range.x > hex_grid->global_grid_origin.x + hex_grid->grid_dimension.x) { error_code = 3; }
		if (lower_range.y < hex_grid->global_grid_origin.y || lower_range.y > hex_grid->global_grid_origin.y + hex_grid->grid_dimension.y) { error_code = 4; }

		if (upper_range.x < hex_grid->global_grid_origin.x || upper_range.x > hex_grid->global_grid_origin.x + hex_grid->grid_dimension.x) { error_code = 5; }
		if (upper_range.y < hex_grid->global_grid_origin.y || upper_range.y > hex_grid->global_grid_origin.y + hex_grid->grid_dimension.y) { error_code = 6; }

		if (error_code != 0) { return false; }

		hex_grid->create_hex_sub_grid(lower_range, upper_range);

		return true;
	}


	void imgui_message_model_popup(std::string open_popup_id, std::string open_popup_title = "", std::string message="Message", bool& cancel = false) {
		ImGui::OpenPopup(open_popup_id.c_str());

		// Always center this window when appearing
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
//printf("create_hex_grid_panel_class::imgui_message_model_popup 00000 \n");
            bool unused_open = true;

		if (ImGui::BeginPopupModal(open_popup_id.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize)){
//printf("create_hex_grid_panel_class::imgui_message_model_popup 11111 \n");
			float x_pos = 10.0f, y_pos = 20.0f;

			text(open_popup_title.c_str(), x_pos, y_pos);

			x_pos = 20; y_pos += 20;
			text(message.c_str(), x_pos, y_pos);

			y_pos += 30;

			std::string dd = "OK" + open_popup_id;

			if (ex_button("OK###mpok", x_pos + 210, y_pos, 100, 20)) {
				ImGui::CloseCurrentPopup();
				cancel = true;
			}

//printf("create_hex_grid_panel_class::imgui_message_model_popup 22222 \n");		

			ImGui::EndPopup();

		}
	}


private:

};