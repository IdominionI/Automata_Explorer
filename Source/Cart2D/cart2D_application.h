#pragma once

#include "Cart2D_grids/cart2D_grid.h"
#include "Editor/create_cart2D_grid_panel.h"
#include "Automata/cart2D_automata_widget.h"

// Main Application class from where application GUI, execution and exit takes place
// The central application node from which manages the topmost application functions
// to select and create the cellular automata grid type projects and set/manage
// application gloabal preferences etc.

template <class HT>
class cart2D_application_class {
public:
	cart2D_application_class() {}
	~cart2D_application_class() {}

	void cart2D_grid_run_loop() {
		if (global_grid_defined) {
//printf("acart2D_application_class: display_acart2D_main_gui_panel : 0000  \n");

			if (global_cart2D_grid) {
//printf("acart2D_application_class: display_acart2D_main_gui_panel : AAAA  \n");
				global_cart2D_grid->display_cart2D_grid();

				// For testing only remove or comment out when not needed
				//global_cart2D_grid->logical_test_cart2D_sub_grids();
				//global_cart2D_grid->operation_testcart2D_sub_grids();
				//global_cart2D_grid->test_cart2D_grid_cell_list();
//printf("acart2D_application_class: display_acart2D_main_gui_panel : BBBB  \n");
				define_automata_rules(global_cart2D_grid);
//printf("acart2D_application_class::display_acart2D_main_gui_panel 111\n");
			}
//printf("acart2D_application_class: display_acart2D_main_gui_panel : 11111  \n");
		}
//printf("acart2D_application_class::display_acart2D_main_gui_panel 222\n");
		if (create_global_grid_popup) {
			create_global_grid();
		}
//printf("acart2D_application_class::display_acart2D_main_gui_panel 333\n");
		if (create_cart2D_sub_grid_popup) {
			create_cart2D_sub_grid();
		}
	}

	void end_current_project() {
		global_cart2D_grid->clear_all_cart2D_grid_cell_data();
		global_cart2D_grid->cart2D_grid_cells_data_list.clear();

		global_cart2D_grid = nullptr;
		global_grid_defined = false;
	}

	void create_global_grid() {
		bool canceled_global_grid = false;

		// It seems that calling this ImGui popup function imediately designates the global_cart2D_grid pointer variable to a nullptr
		// despite no where in this code prior to calling this function for this to occur. This could be a C++ template factor but doubt it
		// Thus Cannot define a new grid and assign existing rule_results to be displayed in that new created grid. What seems to be occuring
		// is that the modal popup somehow is reinisialising global_cart2D_grid to be null.
		//new_global_cart2D_grid = create_cart2D_grid_panel.create_global_cart2D_popup(canceled_global_grid);

		global_cart2D_grid = create_cart2D_grid_panel.create_global_cart2D_popup(canceled_global_grid);

//if(global_cart2D_grid) printf("acart2D_application_class :: create_global_grid 0000 global_cart2D_grid != NULL: %i \n", global_cart2D_grid->cart2D_grid_cells_data_list.size());
//if(!global_cart2D_grid) printf("acart2D_application_class :: create_global_grid 0000 global_cart2D_grid == NULL: \n");

		if (global_cart2D_grid) {
			global_grid_defined = true;
			create_global_grid_popup = false;
		}

		if (canceled_global_grid) { create_global_grid_popup = false; }
//if(!global_cart2D_grid)printf("acart2D_application_class::create_global_grid !global_cart2D_grid $$$$$$ \n");
	}

	bool create_cart2D_sub_grid() {
		bool canceled_child_grid = false;
//printf("acart2D_application_class: create_cart2D_sub_grid : 0000  \n");

		if (!global_cart2D_grid) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "acart2D_application_class::create_cart2D_sub_grid : ERROR :  Could not create automata hex sub grid. global_cart2D_grid undefined \n");
			create_cart2D_sub_grid_popup = false;
			return false;
		}

		if (create_cart2D_grid_panel.add_cart2D_sub_grid_popup(global_cart2D_grid, canceled_child_grid)) {
			if (global_cart2D_grid->cart2D_sub_grids.size() - 1 >= 0) {
				global_cart2D_grid->define_sub_grid_plot_data(global_cart2D_grid->cart2D_sub_grids[global_cart2D_grid->cart2D_sub_grids.size() - 1]);
			}
			create_cart2D_sub_grid_popup = false;
		}
//printf("acart2D_application_class: create_cart2D_sub_grid : 1111  \n");
		if (canceled_child_grid) { create_cart2D_sub_grid_popup = false; }

		return true;
	}

	void define_automata_rules(cart2D_grid_class<HT>* global_cart2D_grid) {
		cart2D_surface_automata_widget.display(global_cart2D_grid);
	}

//private:
	bool global_grid_defined		  = false; // Flag to indicate that a project grid is defined and ready to be used
	bool create_global_grid_popup     = false; // Flag to indicate that the ImGui popup to define and create a project grid is to be displyed
	bool create_cart2D_sub_grid_popup = false; // Flag to indicate that the ImGui popup to define and create a project sub grid is to be displyed

	cart2D_grid_class<HT>* global_cart2D_grid = nullptr; // The hexagoanal automata grid class that manages all aspects of the project hexagonal automata grid 

	create_cart2D_grid_panel_class<HT>       create_cart2D_grid_panel;	   // Class definition of the display panel to define and create a hexagonal automata grid
	cart2D_surface_automata_widget_class<HT> cart2D_surface_automata_widget; // Class definition of the window widget display that manages the user interaction and hexagonal automata execution
};