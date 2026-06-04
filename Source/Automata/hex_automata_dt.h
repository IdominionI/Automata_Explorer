#pragma once

#include <string>
#include <vector>

#include "ThirdParty/ImGui/imgui_docking/imgui.h"
#include "ThirdParty/ImGui/implot/implot.h"

#include "../Hexagon/Objects/hex_grid_parameters.h"

#include "../Parser/parser_definitions.h"

/*
	Structures and definitions that define the parameters
	and variables to perform cellula automata rules on a
	hexagonal 2d surface data, and export/import the
	parameter data.
*/

#define NUMBER_HEX_NEIGHBOURS 6

#define GENERATED_HEX_SURFACE 0
#define DEFINED_HEX_SURFACE   1
#define RANDOM_GENERATED      2

#define HEX_NEIGHBOR_IGNOR_RULE_CODE    -1
#define HEX_NEIGHBOR_INACTIVE_RULE_CODE  0
#define HEX_NEIGHBOR_ACTIVE_RULE_CODE    1

// Start New voxel automata definitions
#define HEX_NEIGHBOR_LESS_THAN_RULE_CODE           -2020
#define HEX_NEIGHBOR_LESS_THAN_OR_EQUAL_RULE_CODE  -2010
#define HEX_NEIGHBOR_NOT_EQUAL_RULE_CODE           -2000
#define HEX_NEIGHBOR_EQUAL_RULE_CODE                2000
#define HEX_NEIGHBOR_GREATER_THAN_OR_EQUALRULE_CODE 2010
#define HEX_NEIGHBOR_GREATER_THAN_RULE_CODE         2020

#define HEX_MAX_NAME_LENGTH			     30
#define HEX_MAX_RULE_DEFINITION_LENGTH   1000
#define HEX_MAX_RULE_RESULT_RULES_LENGTH 30
#define HEX_MAX_RULE_RESULT_VALUE_LENGTH 15

typedef unsigned long int u_long_int;

enum class hex_lower_rule_condition_enum_type { ignore, LTEQ, LT };
enum class hex_upper_rule_condition_enum_type { ignore, NE, EQ, LTEQ, LT };

struct hex_automata_sub_rule_struct_type {
	int	 sub_rule_id = -1;
	std::string  sub_rule_name = "sub_rule";
	bool active_sub_rule = false;

	parser_base_node *root_parser_node = nullptr; //Forward declaration to root parser node of sub_rule

	std::string sub_rule_definition = "";
	std::string sub_rule_edit       = "";

	bool sub_rule_conditions_met = false;
	bool edit_sub_rule           = false;
	bool edit_sub_rule_text      = false;

};

struct hex_surface_automata_rule_struct_type {
	int	 rule_id          = -1;
	std::string	rule_name = "rule";
	//bool active_rule	  = false;
	//int  rule_start_step  = 0, rule_end_step = 0;

	bool display_sub_rules = false;

	std::vector<hex_automata_sub_rule_struct_type> sub_rules;

	void add_sub_rule(hex_automata_sub_rule_struct_type sub_rule) {
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

	hex_automata_sub_rule_struct_type *get_sub_rule(int sub_rule_id) {
		for (hex_automata_sub_rule_struct_type sub_rule : sub_rules) {
			if (sub_rule.sub_rule_id == sub_rule_id) {
				return &sub_rule;
				break;
			}
		}

		return nullptr;
	}

	bool modify_sub_rule(hex_automata_sub_rule_struct_type sr) {
		for (hex_automata_sub_rule_struct_type sub_rule : sub_rules) {
			if (sub_rule.sub_rule_id == sr.sub_rule_id) {
				sub_rule = sr;
				return true;
				break;
			}
		}

		return false;
	}

	bool all_rule_conditions_met() {
		for (hex_automata_sub_rule_struct_type sub_rule : sub_rules) {
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
		for (hex_automata_sub_rule_struct_type sub_rule : sub_rules) {
			sub_rule.sub_rule_conditions_met = false;
		}
	}

//private:
	int sub_rule_id = -1;
};


// This struct may need to be defined elsewhere
struct hex_surface_automata_rule_result_struct_type {
	int	 rule_result_id = -1;
	std::string  result_name = "hex result";
	bool active_result = false;
	int  rule_start_step = 0, rule_end_step = 0;

	std::string hex_result_definition = "";
	std::string hex_result_value      = "";
	ImVec4 result_display_color		  = { 1.0f,1.0f,1.0f,1.0f };
	int display_shape_id			  = 1;
	float display_shape_size          = 6.0f;
	ImPlotMarker_ result_display_shape = ImPlotMarker_::ImPlotMarker_Circle;

	bool display_result = true;
	std::vector<int> hex_surface_automata_rules_met;

	void define_imgui_shape() {
		switch (display_shape_id) {
			case 0 : result_display_shape = ImPlotMarker_::ImPlotMarker_Circle; break;
			case 1 : result_display_shape = ImPlotMarker_::ImPlotMarker_Square; break;
			case 2 : result_display_shape = ImPlotMarker_::ImPlotMarker_Diamond; break;
			case 3 : result_display_shape = ImPlotMarker_::ImPlotMarker_Up    ; break;
			case 4 : result_display_shape = ImPlotMarker_::ImPlotMarker_Down  ; break;
			case 5 : result_display_shape = ImPlotMarker_::ImPlotMarker_Left  ; break;
			case 6 : result_display_shape = ImPlotMarker_::ImPlotMarker_Right ; break;
			case 7 : result_display_shape = ImPlotMarker_::ImPlotMarker_Cross ; break;
			case 8 : result_display_shape = ImPlotMarker_::ImPlotMarker_Plus  ; break;
			case 9 : result_display_shape = ImPlotMarker_::ImPlotMarker_Asterisk; break;
			default: result_display_shape = ImPlotMarker_::ImPlotMarker_Circle;
		}
	}

	int hex_grid_cell_data_id = -1;
};

// End New voxel automata definitions

struct hex_surface_automata_generator_parameters_struct_type {
	float	 x_start = -1.0f, x_end = 1.0f;
	float	 y_start = -1.0f, y_end = 1.0f;
	float	 resolution_step = 0.01f;
};