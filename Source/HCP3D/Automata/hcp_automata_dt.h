#pragma once

#include <string>
#include <vector>

#include "ThirdParty/ImGui/imgui_docking/imgui.h"
#include "ThirdParty/ImGui/implot3D/implot3D.h"

#include "Source/HCP3D/HCP_grids/hcp_grid_parameters.h"

#include "Source/Parser/parser_definitions.h"

/*
	Structures and definitions that define the parameters
	and variables to perform cellula automata rules on a
	hexagonal 2d surface data, and export/import the
	parameter data.
*/

//typedef unsigned long int u_long_int; // Jusin case needed

// Hexagonal automata sub rule structure type
// To store data that defines a hexagonal cellula automata sub rule.
struct hcp_automata_sub_rule_struct_type {
	int	 sub_rule_id = -1;					 // Unique id number to ba able to display Imgui widgets without identifier clashes and for simple id purposes
	std::string  sub_rule_name = "sub_rule"; // Text label name of sub rule
	bool active_sub_rule = false;			 // Indicate if sub rule is to be used or not

	parser_base_node *root_parser_node = nullptr; // Forward declaration to root parser node of sub_rule

	std::string sub_rule_definition = ""; // String to store definition of sub rule as a string of text 
	//std::string sub_rule_edit       = "";

	bool sub_rule_conditions_met = false; // Indicate if sub rule condition has been met
	bool edit_sub_rule           = false; // Indicate if sub rule has been edited
	bool edit_sub_rule_text      = false; // Indicate if sub rule can be edited or not

};

// Hexagonal automata rule structure type
// To store data that defines a hexagonal cellula automata rule.
struct hcp_surface_automata_rule_struct_type {
	int	 rule_id          = -1;	   // Unique id number to ba able to display Imgui widgets without identifier clashes and for simple id purposes
	std::string	rule_name = "rule";// Text label name of rule

	bool display_sub_rules = false; // Indicate if sub rules are to be displayed as an overlay on the main hex grid or not

	std::vector<hcp_automata_sub_rule_struct_type> sub_rules;// List of sub rules that make up this rule defied as a dynamic vector

	// !!!!!!!!! Functions to manage the sub rule definitions that make up a rule !!!!!!!!!!

	void add_sub_rule(hcp_automata_sub_rule_struct_type sub_rule) {
		if (sub_rule.sub_rule_id < 0) { 
			sub_rule_id += 1;
			sub_rule.sub_rule_id = sub_rule_id;
		}
		sub_rules.push_back(sub_rule);
	}

	bool delete_sub_rule(int sub_rule_id) {
		for (size_t i = 0; i < sub_rules.size();i++) {
			if (sub_rules[i].sub_rule_id == sub_rule_id) {
				sub_rules.erase(sub_rules.begin()+i);
				return true;
				break;
			}
		}
		return false;
	}

	hcp_automata_sub_rule_struct_type *get_sub_rule(int sub_rule_id) {
		for (hcp_automata_sub_rule_struct_type sub_rule : sub_rules) {
			if (sub_rule.sub_rule_id == sub_rule_id) {
				return &sub_rule;
				break;
			}
		}

		return nullptr;
	}

	bool modify_sub_rule(hcp_automata_sub_rule_struct_type sr) {
		for (hcp_automata_sub_rule_struct_type sub_rule : sub_rules) {
			if (sub_rule.sub_rule_id == sr.sub_rule_id) {
				sub_rule = sr;
				return true;
				break;
			}
		}

		return false;
	}

	bool all_rule_conditions_met() {
		for (hcp_automata_sub_rule_struct_type sub_rule : sub_rules) {
			if (!sub_rule.sub_rule_conditions_met) {
				return false;
				break;
			}
		}

		return true;
	}

	void clear_all_sub_rules() {
		sub_rules.clear();
	}

	void clear_all_sub_rule_conditions() {
		for (hcp_automata_sub_rule_struct_type sub_rule : sub_rules) {
			sub_rule.sub_rule_conditions_met = false;
		}
	}

//private:
	int sub_rule_id = -1;
};


// This struct may need to be defined elsewhere
// Hexagonal automata rule results structure type
// To store data that defines a set of hexagonal cellula automata rules that assign
// a value to a hexagonal cellular automata grid cell
struct hcp_surface_automata_rule_result_struct_type {
	int	 rule_result_id = -1;						// Unique id number to ba able to display Imgui widgets without identifier clashes and for simple id purposes
	std::string  result_name = "hex result";		// Text label name of rule result
	bool active_result = false;						// Indicates if the rule result is active and to be used in the iteration process or not
	int  rule_start_step = 0, rule_end_step = 0;	// Start and end iteration steps that the rule is to be applied over

	std::string hcp_result_definition = "";			// Text based definition of which rule(s) are to be met
	std::string hcp_result_value      = "";			// Result to assign hex grid automata cell if all rule conditions are met

	// Graphical definitions used to display rule results as an overlay on the main cellula automata hex grid
	ImVec4 result_display_color		  = { 1.0f,1.0f,1.0f,1.0f };
	int display_shape_id			  = 1;
	float display_shape_size          = 6.0f;
	ImPlot3DMarker_ result_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Circle;

	bool display_result = true;						// Indicate if the rule result overlay is to be displayed or not
	std::vector<int> hcp_surface_automata_rules_met;// Indicate if all rule results have been met or not for the current iteration step

	// Function to define which ImPLot shape is to be used to display the rule result 
	void define_imgui_shape() {
		switch (display_shape_id) {
			case 0 : result_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Circle; break;
			case 1 : result_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Square; break;
			case 2 : result_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Diamond; break;
			case 3 : result_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Up    ; break;
			case 4 : result_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Down  ; break;
			case 5 : result_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Left  ; break;
			case 6 : result_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Right ; break;
			case 7 : result_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Cross ; break;
			case 8 : result_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Plus  ; break;
			case 9 : result_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Asterisk; break;
			default: result_display_shape = ImPlot3DMarker_::ImPlot3DMarker_Circle;
		}
	}

	int hcp_grid_cell_data_id = -1;
};

// End New voxel automata definitions

//struct hcp_surface_automata_generator_parameters_struct_type {
//	float	 x_start = -1.0f, x_end = 1.0f;
//	float	 y_start = -1.0f, y_end = 1.0f;
//	float	 resolution_step = 0.01f;
//};