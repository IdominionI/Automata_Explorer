#pragma once

#include "Source/Hex_grids/hex_grid.h"
#include "Source/Editor/Main_Window/Panels/create_hex_grid_panel.h"
#include "Source/Automata/Hex_automata_widget.h"

template <class HT>
class ahex_application_class {
public:
	ahex_application_class() {}
	~ahex_application_class() {}

	void display_ahex_main_gui_panel(glfw_openGL_window_class *glfw_window) {
//printf("ahex_application_class::display_ahex_main_gui_panel 000\n");	
		// Display main window menu bar
		if (ImGui::BeginMainMenuBar()) {// UnComment out to have menu in application main window

			if (ImGui::BeginMenu("Project")) {
//printf("display_tree_branch :BeginMenu: %s : %i \n", child_tree_branch->label.c_str(), i);

				if (ImGui::MenuItem("New Automata Project")) {
//printf("Add to selection :MenuItem: \n");
					if (global_hex_grid) end_current_project();
					if (!global_hex_grid) create_global_grid_popup = true;
				}

				if (ImGui::MenuItem("End Current Automata Poject")) {
//printf("Edit selection :MenuItem:\n");
					if (global_hex_grid) end_current_project();
				}

				if (ImGui::MenuItem("Add Child Grid")) {
//printf("Remove from selection :MenuItem:\n");
					if (global_hex_grid) create_hex_sub_grid_popup = true;
				}

				if (ImGui::MenuItem("Exit")) {
//printf("Exit WSM :MenuItem:\n");
					exit_ahex_application(glfw_window);
				}

				ImGui::EndMenu();
			}

// Preferences menu just in case it will bw implemented
/*
			if (ImGui::BeginMenu("Preferences")) {
				if (ImGui::BeginMenu("Define WSM Media Apps ...")) {
					if (ImGui::MenuItem("html editor")) {
//printf("Add to selection :MenuItem: \n");
						//define_html_editor();
					}

					if (ImGui::MenuItem("image editor")) {
//printf("Add to selection :MenuItem: \n");
						//define_image_editor();
					}

					if (ImGui::MenuItem("text editor")) {
//printf("Add to selection :MenuItem: \n");
						//define_text_editor();
					}

					if (ImGui::MenuItem("media player")) {
//printf("Add to selection :MenuItem: \n");
						//define_media_player();
					}

					// Add other web site media applications to view/edit WSM files here

					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}
*/
			ImGui::EndMainMenuBar();// UnComment out to have menu in application main window
		}// UnComment out to have menu in application main window


		if (global_grid_defined) {
//printf("ahex_application_class: display_ahex_main_gui_panel : 0000  \n");

			if (global_hex_grid) {
//printf("ahex_application_class: display_ahex_main_gui_panel : AAAA  \n");
				global_hex_grid->display_hex_grid();

				// For testing only remove or comment out when not needed
				//global_hex_grid->logical_test_hex_sub_grids();
				//global_hex_grid->operation_testhex_sub_grids();
				//global_hex_grid->test_hex_grid_cell_list();
//printf("ahex_application_class: display_ahex_main_gui_panel : BBBB  \n");
				define_automata_rules(global_hex_grid);
//printf("ahex_application_class::display_ahex_main_gui_panel 111\n");
			}
//printf("ahex_application_class: display_ahex_main_gui_panel : 11111  \n");
		}
//printf("ahex_application_class::display_ahex_main_gui_panel 222\n");
		if (create_global_grid_popup) {
			create_global_grid();
		}
//printf("ahex_application_class::display_ahex_main_gui_panel 333\n");
		if (create_hex_sub_grid_popup) {
			create_hex_sub_grid();
		}
	}



protected:
	void exit_ahex_application(glfw_openGL_window_class* glfw_window) {
		end_current_project();
		glfwDestroyWindow(glfw_window->get_window_ptr());
		exit(0);
	}

	void end_current_project() {
		hex_surface_automata_widget.stop_automata_execution();

		global_hex_grid->clear_all_hex_grid_cell_data();
		global_hex_grid->hex_grid_cells_data_list.clear();

		global_hex_grid     = nullptr;
		global_grid_defined = false;

		hex_surface_automata_widget.clear_all_automata_rules();
	}

	void create_global_grid() {
		bool canceled_global_grid = false;

		// It seems that calling this ImGui popup function imediately designates the global_hex_grid pointer variable to a nullptr
		// despite no where in this code prior to calling this function for this to occur. This could be a C++ template factor but doubt it
		// Thus Cannot define a new grid and assign existing rule_results to be displayed in that new created grid. What seems to be occuring
		// is that the modal popup somehow is reinisialising global_hex_grid to be null.
		//new_global_hex_grid = create_hex_grid_panel.create_global_hex_popup(canceled_global_grid);

		global_hex_grid = create_hex_grid_panel.create_global_hex_popup(canceled_global_grid);

//if(global_hex_grid) printf("ahex_application_class :: create_global_grid 0000 global_hex_grid != NULL: %i \n", global_hex_grid->hex_grid_cells_data_list.size());
//if(!global_hex_grid) printf("ahex_application_class :: create_global_grid 0000 global_hex_grid == NULL: \n");

		if (global_hex_grid) {
			global_grid_defined = true;
			create_global_grid_popup = false;
		
			// !!!! Testing only : delete or comment out when finished !!!!
			//for (size_t i = 95; i < 105; i++) {
			//	global_hex_grid->hex_grid[i] = 1;
			//}
			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	
		}

		if (canceled_global_grid) { create_global_grid_popup = false; }
//if(!global_hex_grid)printf("ahex_application_class::create_global_grid !global_hex_grid $$$$$$ \n");
	}

	bool create_hex_sub_grid() {
		bool canceled_child_grid = false;
//printf("ahex_application_class: create_hex_sub_grid : 0000  \n");

		if (!global_hex_grid) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "ahex_application_class::create_hex_sub_grid : ERROR :  Could not create automata hex sub grid. global_hex_grid undefined \n");
			create_hex_sub_grid_popup = false;
			return false;
		}

		if (create_hex_grid_panel.add_hex_sub_grid_popup(global_hex_grid, canceled_child_grid)) {
			if (global_hex_grid->hex_sub_grids.size() - 1 >= 0) { 
				global_hex_grid->define_sub_grid_plot_data(global_hex_grid->hex_sub_grids[global_hex_grid->hex_sub_grids.size() - 1]); 
			}
			create_hex_sub_grid_popup = false;
		}
//printf("ahex_application_class: create_hex_sub_grid : 1111  \n");
		if (canceled_child_grid) { create_hex_sub_grid_popup = false; }

		return true;
	}

	void define_automata_rules(hex_grid_class<HT> *global_hex_grid) {
		hex_surface_automata_widget.display(global_hex_grid);
	}

private:
	bool global_grid_defined      = false;
	bool create_global_grid_popup = false;
	bool create_hex_sub_grid_popup  = false;

	hex_grid_class<HT> *global_hex_grid     = nullptr;

	create_hex_grid_panel_class<HT> create_hex_grid_panel;
	hex_surface_automata_widget_class<HT> hex_surface_automata_widget;
};