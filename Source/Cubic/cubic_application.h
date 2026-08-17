#pragma once

#include "Cubic_grids/cubic_grid.h"
#include "Editor/create_cubic_grid_panel.h"
#include "Automata/cubic_automata_widget.h"

// Main Application class from where application GUI, execution and exit takes place
// The central application node from which manages the topmost application functions
// to select and create the cellular automata grid type projects and set/manage
// application gloabal preferences etc.

template <class HT>
class cubic_application_class {
public:
	cubic_application_class() {}
	~cubic_application_class() {}

	void cubic_grid_run_loop() {
		if (global_grid_defined) {
//printf("acubic_application_class: display_acubic_main_gui_panel : 0000  \n");

			if (global_cubic_grid) {
//printf("acubic_application_class: cubic_grid_run_loop : AAAA  \n");
				global_cubic_grid->display_cubic_grid();

				// For testing only remove or comment out when not needed
				//global_cubic_grid->logical_test_cubic_sub_grids();
				//global_cubic_grid->operation_testcubic_sub_grids();
				//global_cubic_grid->test_cubic_grid_cell_list();
//printf("acubic_application_class: cubic_grid_run_loop : BBBB  \n");
				define_automata_rules(global_cubic_grid);
//printf("acubic_application_class::cubic_grid_run_loop 111\n");
			}
//printf("acubic_application_class: cubic_grid_run_loop : 11111  \n");
		}
//printf("acubic_application_class::cubic_grid_run_loop 222\n");
		if (create_global_grid_popup) {
			create_global_grid();
		}
//printf("acubic_application_class::cubic_grid_run_loop 333\n");
		if (create_cubic_sub_grid_popup) {
			create_cubic_sub_grid();
		}
	}


//protected:
	//void exit_acubic_application(glfw_openGL_window_class* glfw_window) {
	//	end_current_project();
	//	glfwDestroyWindow(glfw_window->get_window_ptr());
	//	exit(0);
	//}

	void end_current_project() {
		cubic_surface_automata_widget.stop_automata_execution();

		global_cubic_grid->clear_all_cubic_grid_cell_data();
		global_cubic_grid->cubic_grid_cells_data_list.clear();

		global_cubic_grid     = nullptr;
		global_grid_defined = false;

		cubic_surface_automata_widget.clear_all_automata_rules();
	}

	void create_global_grid() {
		bool canceled_global_grid = false;

		// It seems that calling this ImGui popup function imediately designates the global_cubic_grid pointer variable to a nullptr
		// despite no where in this code prior to calling this function for this to occur. This could be a C++ template factor but doubt it
		// Thus Cannot define a new grid and assign existing rule_results to be displayed in that new created grid. What seems to be occuring
		// is that the modal popup somehow is reinisialising global_cubic_grid to be null.
		//new_global_cubic_grid = create_cubic_grid_panel.create_global_cubic_popup(canceled_global_grid);

		global_cubic_grid = create_cubic_grid_panel.create_global_cubic_popup(canceled_global_grid);

//if(global_cubic_grid) printf("acubic_application_class :: create_global_grid 0000 global_cubic_grid != NULL: %i \n", global_cubic_grid->cubic_grid_cells_data_list.size());
//if(!global_cubic_grid) printf("acubic_application_class :: create_global_grid 0000 global_cubic_grid == NULL: \n");

		if (global_cubic_grid) {
			global_grid_defined = true;
			create_global_grid_popup = false;
		
			// !!!! Testing only : delete or comment out when finished !!!!
			//for (size_t i = 95; i < 105; i++) {
			//	global_cubic_grid->cubic_grid[i] = 1;
			//}
			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	
		}

		if (canceled_global_grid) { create_global_grid_popup = false; }
//if(!global_cubic_grid)printf("acubic_application_class::create_global_grid !global_cubic_grid $$$$$$ \n");
	}

	bool create_cubic_sub_grid() {
		bool canceled_child_grid = false;
//printf("acubic_application_class: create_cubic_sub_grid : 0000  \n");

		if (!global_cubic_grid) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "acubic_application_class::create_cubic_sub_grid : ERROR :  Could not create automata hex sub grid. global_cubic_grid undefined \n");
			create_cubic_sub_grid_popup = false;
			return false;
		}

		if (create_cubic_grid_panel.add_cubic_sub_grid_popup(global_cubic_grid, canceled_child_grid)) {
			if (global_cubic_grid->cubic_sub_grids.size() - 1 >= 0) { 
				global_cubic_grid->define_sub_grid_plot_data(global_cubic_grid->cubic_sub_grids[global_cubic_grid->cubic_sub_grids.size() - 1]); 
			}
			create_cubic_sub_grid_popup = false;
		}
//printf("acubic_application_class: create_cubic_sub_grid : 1111  \n");
		if (canceled_child_grid) { create_cubic_sub_grid_popup = false; }

		return true;
	}

	void define_automata_rules(cubic_grid_class<HT> *global_cubic_grid) {
		cubic_surface_automata_widget.display(global_cubic_grid);
	}

//private:
	bool global_grid_defined        = false; // Flag to indicate that a project grid is defined and ready to be used
	bool create_global_grid_popup   = false; // Flag to indicate that the ImGui popup to define and create a project grid is to be displyed
	bool create_cubic_sub_grid_popup  = false; // Flag to indicate that the ImGui popup to define and create a project sub grid is to be displyed

	cubic_grid_class<HT> *global_cubic_grid = nullptr; // The hexagoanal automata grid class that manages all aspects of the project hexagonal automata grid 

	create_cubic_grid_panel_class<HT>       create_cubic_grid_panel;	   // Class definition of the display panel to define and create a hexagonal automata grid
	cubic_surface_automata_widget_class<HT> cubic_surface_automata_widget; // Class definition of the window widget display that manages the user interaction and hexagonal automata execution
};