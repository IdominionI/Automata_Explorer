#pragma once

#include "Hex_grids/hex_grid.h"
#include "Editor/create_hex_grid_panel.h"
#include "Automata/Hex_automata_widget.h"

// Main Application class from where application GUI, execution and exit takes place
// The central application node from which manages the topmost application functions
// to select and create the cellular automata grid type projects and set/manage
// application gloabal preferences etc.

template <class HT>
class ahex_application_class {
public:
	ahex_application_class() {}
	~ahex_application_class() {}

	void hex_grid_run_loop() {
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


//protected:
	//void exit_ahex_application(glfw_openGL_window_class* glfw_window) {
	//	end_current_project();
	//	glfwDestroyWindow(glfw_window->get_window_ptr());
	//	exit(0);
	//}

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

//private:
	bool global_grid_defined        = false; // Flag to indicate that a project grid is defined and ready to be used
	bool create_global_grid_popup   = false; // Flag to indicate that the ImGui popup to define and create a project grid is to be displyed
	bool create_hex_sub_grid_popup  = false; // Flag to indicate that the ImGui popup to define and create a project sub grid is to be displyed

	hex_grid_class<HT> *global_hex_grid = nullptr; // The hexagoanal automata grid class that manages all aspects of the project hexagonal automata grid 

	create_hex_grid_panel_class<HT>       create_hex_grid_panel;	   // Class definition of the display panel to define and create a hexagonal automata grid
	hex_surface_automata_widget_class<HT> hex_surface_automata_widget; // Class definition of the window widget display that manages the user interaction and hexagonal automata execution
};