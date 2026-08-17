#pragma once

#include <thirdparty/ImGUI/imgui_docking/imgui.h>

#include <FrameWork/AFW/ImGUI/ImGuiEx/imgui_widgets.h>

#include <FrameWork/AFW/Interface/IconsFontAwesome4.h>

#include "Source/HCP3D/Hcp_grids/hcp_grid.h"

// Class to display ImGui popups to create hcp automata grid and subgrid and create them

template <class T>
class create_hcp_grid_panel_class {
public:
	// Popup to display parameters for user to enter values to create and create application main hcp automata  grid 
	hcp_grid_class<T>* create_global_hcp_popup(bool &cancel=false) {
//printf("create_hcp_grid_panel_class::display AAAAA \n");
		ImGui::OpenPopup("Global HCP Grid");

		// Always center this window when appearing
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		hcp_grid_class<T>* return_value = nullptr;

		if (ImGui::BeginPopupModal("Global HCP Grid", NULL, ImGuiWindowFlags_AlwaysAutoResize))// Must have same id name as ImGui::OpenPopup to display
		{
//printf("create_hcp_grid_panel_class::display 00000 \n");
			static hcp_surface_index_data_type xdim=10, ydim=10, zdim=10;
			static float hcp_cell_value = 0.0f;
	
			float x_pos = 10.0f, y_pos = 20.0f;

			text("Define Global HCP grid", x_pos, y_pos);

			x_pos = 20; y_pos += 20;
			text("Number of Columns (x Dimension) :", x_pos, y_pos);
			integer_input("###hcpgxd", xdim, x_pos +250, y_pos-3, 100.0f);

			y_pos += 30;
			text("Number of Rows (y dimanesion) :", x_pos, y_pos);
			integer_input("###hcpgyd", ydim, x_pos + 250, y_pos-3, 100.0f);

			y_pos += 30;
			text("Number of Layers (z dimanesion) :", x_pos, y_pos);
			integer_input("###hcpgzd", zdim, x_pos + 250, y_pos - 3, 100.0f);

			y_pos += 30;
			text("Initial HCP cell value:", x_pos, y_pos);
			float_input("###hcpghs", hcp_cell_value, x_pos + 250, y_pos-3 , 100.0f);

			y_pos += 30;
			if (ex_button("Create Global HCP Grid", x_pos + 10, y_pos, 180, 20)) {
//printf("create_hcp_grid_panel_class::display 1111 \n");
				return_value = create_global_hcp_grid(xdim, ydim, zdim, hcp_cell_value);
			}

			if (ex_button("Cancel###chcpgcb", x_pos + 210, y_pos, 100, 20)) {
				cancel = true;
			}

//printf("create_hcp_grid_panel_class::display 22222 \n");		

			ImGui::EndPopup();

		}
//printf("create_hcp_grid_panel_class::display 3333 \n");

		return return_value;

	}

	// Popup to display parameters for user to enter values to create and create application hcp automata sub grid 
	bool add_hcp_sub_grid_popup(hcp_grid_class<T> * hcp_grid, bool& cancel = false) {
//printf("create_hcp_grid_panel_class::display AAAAA \n");

		bool return_value = false;
		static int  error_code   = 0;
		static bool dummy = false;

		//if (error_message) {
		if (!hcp_grid) {
			imgui_message_model_popup("Child HCP Grid Error Message", "Define Child HCP grid", "Cannot create child hcp grid : \nNo global or parent hcp grid defined to have as a parent of.", cancel);
		} else {
			ImGui::OpenPopup("HCP Sub Grid");

			// Always center this window when appearing
			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

			if (ImGui::BeginPopupModal("HCP Sub Grid", NULL, ImGuiWindowFlags_AlwaysAutoResize)){
//printf("create_hcp_grid_panel_class::display 00000 \n");
				static hcp_surface_vec_data_type   min_coord = { 0,0,0 }, max_coord{ 0,0,0 };
				float hcp_cell_value = 1.0f;

				float x_pos = 10.0f, y_pos = 20.0f;

				text("Create child hcp grid", x_pos, y_pos);

				y_pos += 30;
				text("Min HCP sub grid coordinate : ", x_pos, y_pos);

				x_pos = 20; y_pos += 20;
				text("Min X :", x_pos, y_pos);
				integer_input("###hcpgxd", min_coord.x, x_pos + 50, y_pos - 3, 50.0f);

				text("Min Y :", x_pos+150, y_pos);
				integer_input("###hcpgyd", min_coord.y, x_pos + 200, y_pos - 3, 50.0f);

				y_pos += 30;
				text("Max HCP sub grid coordinate : ", x_pos, y_pos);

				y_pos += 30;
				text("Max X :", x_pos, y_pos);
				integer_input("###hgxoc", max_coord.x, x_pos + 50, y_pos - 3, 50.0f);

				text("Max Y :", x_pos+150, y_pos);
				integer_input("###hgyoc", max_coord.y, x_pos + 200, y_pos - 3, 50.0f);


				y_pos += 30;
				if (ex_button("Create child HCP Grid", x_pos + 10, y_pos, 180, 20)) {
//printf("create_hcp_grid_panel_class::display 1111 \n");
					error_code = 0;
					dummy = false;
					return_value = create_hcp_sub_grid(hcp_grid, min_coord, max_coord, error_code);
				}

				if (ex_button("Cancel###chgcb", x_pos + 210, y_pos, 100, 20)) {
					cancel = true;
				}

				if (error_code > 0 and !dummy) {
					
					switch (error_code) {
						case 1:	imgui_message_model_popup("HCP sub Grid Error Message", "Define HCP sub grid", "Cannot create hcp sub grid : \n hcp sub grid x lower range greater than x upper range.", dummy); break;
						case 2:	imgui_message_model_popup("HCP sub Grid Error Message", "Define HCP sub grid", "Cannot create hcp sub grid : \n hcp sub grid y lower range greater than y upper range.", dummy); break;
						case 3:	imgui_message_model_popup("HCP sub Grid Error Message", "Define HCP sub grid", "Cannot create hcp sub grid : \n hcp sub grid origin x coordinate is outside the x bounds of the parent hcp grid", dummy); break;
						case 4:	imgui_message_model_popup("HCP sub Grid Error Message", "Define HCP sub grid", "Cannot create hcp sub grid : \n hcp sub grid origin y coordinate is outside the y bounds of the parent hcp grid", dummy); break;
						case 5:	imgui_message_model_popup("HCP sub Grid Error Message", "Define HCP sub grid", "Cannot create hcp sub grid : \n hcp sub grid origin x extents is outside the x bounds of the parent grid", dummy); break;
						case 6:	imgui_message_model_popup("HCP sub Grid Error Message", "Define HCP sub grid", "Cannot create hcp sub grid : \n hcp sub grid origin y extents is outside the y bounds of the parent grid", dummy); break;
						//case 7:	imgui_message_model_popup("HCP sub Grid Error Message", "Define HCP sub grid", "Cannot create hcp sub grid : \n not sub allocate memory to define child grid. ", dummy); break;
					}
				}
//printf("create_hcp_grid_panel_class::display 22222 \n");		

				ImGui::EndPopup();

			}
		}
//printf("create_hcp_grid_panel_class::display 3333 \n");

		return return_value;

	}

protected:

	hcp_grid_class<T>* create_global_hcp_grid(hcp_surface_index_data_type xdim, hcp_surface_index_data_type ydim, hcp_surface_index_data_type zdim, float hcp_cell_value) {
//printf("create_hcp_grid_panel_class::create_global_hcp_grid 00000 \n");

		if (xdim < 1 || ydim < 1 || zdim <1) {
			//imgui_message_model_popup("HCP Grid Error Message", "Create HCP grid", "Cannot create hcp grid : \n HCP grid xdimension is of zero size", dummy);
//printf("create_hcp_grid_panel_class::create_global_hcp_grid xdim < 1 || ydim < 1 || zdim <1 1111 \n");
			return nullptr;
		}

		hcp_grid_class<T> *global_hcp_grid = new hcp_grid_class<T>(hcp_cell_value, { xdim, ydim, zdim });
//printf("create_hcp_grid_panel_class::create_global_hcp_grid 22222 \n");
		if (!global_hcp_grid) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "Create glabal hcp grid :: Unable to define global HCP grid.\n");
			return nullptr;
		}

//printf("create_hcp_grid_panel_class::create_global_hcp_grid 33333 \n");
		global_hcp_grid->define_hcp_grid_coordinates();

//printf("create_hcp_grid_panel_class::create_global_hcp_grid 44444 \n");
		return global_hcp_grid;
	}

	bool create_hcp_sub_grid(hcp_grid_class<T>* hcp_grid, hcp_surface_vec_data_type lower_range, hcp_surface_vec_data_type upper_range, int &error_code) {
printf("create_hcp_grid_panel_class::create_global_grid 00000 \n");

		if (!hcp_grid) {return false;}

		if (upper_range.x < lower_range.x) { error_code = 1; }
		if (upper_range.y < lower_range.y) { error_code = 2; }

		if (lower_range.x < hcp_grid->global_grid_origin.x || lower_range.x > hcp_grid->global_grid_origin.x + hcp_grid->grid_dimension.x) { error_code = 3; }
		if (lower_range.y < hcp_grid->global_grid_origin.y || lower_range.y > hcp_grid->global_grid_origin.y + hcp_grid->grid_dimension.y) { error_code = 4; }

		if (upper_range.x < hcp_grid->global_grid_origin.x || upper_range.x > hcp_grid->global_grid_origin.x + hcp_grid->grid_dimension.x) { error_code = 5; }
		if (upper_range.y < hcp_grid->global_grid_origin.y || upper_range.y > hcp_grid->global_grid_origin.y + hcp_grid->grid_dimension.y) { error_code = 6; }

		if (error_code != 0) { return false; }

		hcp_grid->create_hcp_sub_grid(lower_range, upper_range);

		return true;
	}


	void imgui_message_model_popup(std::string open_popup_id, std::string open_popup_title = "", std::string message="Message", bool& cancel = false) {
		ImGui::OpenPopup(open_popup_id.c_str());

		// Always center this window when appearing
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
//printf("create_hcp_grid_panel_class::imgui_message_model_popup 00000 \n");
            bool unused_open = true;

		if (ImGui::BeginPopupModal(open_popup_id.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize)){
//printf("create_hcp_grid_panel_class::imgui_message_model_popup 11111 \n");
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

//printf("create_hcp_grid_panel_class::imgui_message_model_popup 22222 \n");		

			ImGui::EndPopup();

		}
	}


private:

};