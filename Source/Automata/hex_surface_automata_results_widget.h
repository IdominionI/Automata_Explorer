#pragma once

#include <ImGui/imgui_docking/misc/cpp/imgui_stdlib.h>// Need This so as to be able to input text as a std::string variable

#include "hex_surface_automata_rules_widget.h"

template <class T>
class hex_surface_automata_results_widget_class {
public:
	hex_surface_automata_results_widget_class() {}
	~hex_surface_automata_results_widget_class() {}

	int current_selected_rule_result_id = -1;
	bool automata_rules_verified		= false;

	std::vector<hex_surface_automata_rule_result_struct_type> hex_surface_automata_rule_results;

	hex_surface_automata_rules_widget_class<T> hex_surface_automata_rules_widget;

	void reset_rule_results_hex_grid_cell_data_id() {// This is required when clearing or importing a hex surface grid
		for (hex_surface_automata_rule_result_struct_type &rule_result : hex_surface_automata_rule_results) {
			rule_result.hex_grid_cell_data_id = -1;
		}
	}

	void add_rule_result(hex_surface_automata_rule_result_struct_type rule_result) {
		if (rule_result.rule_result_id < 0) {
			rule_result_id += 1;
			rule_result.rule_result_id = rule_result_id;
		}
		hex_surface_automata_rule_results.push_back(rule_result);
	}

	void add_new_rule_result() {
		hex_surface_automata_rule_result_struct_type rule_result;
		add_rule_result(rule_result);
	}
	// +++++++++++++
	void define_and_clear_hex_surface_automata_rules_and_results() {
		rule_result_id = -1;
		delete_all_automata_rules();
		clear_hex_surface_automata_rule_results();
		//clear_hex_surface_automata_rules();
	}

	void clear_hex_surface_automata_rule_results() {
		hex_surface_automata_rule_results.clear();
		hex_surface_automata_rule_results.shrink_to_fit();
	}

	void clear_hex_surface_automata_rules() {
		hex_surface_automata_rules_widget.hex_surface_automata_rules.clear();
		hex_surface_automata_rules_widget.hex_surface_automata_rules.shrink_to_fit();
	}
	// +++++++++++++
	bool delete_rule_result(int rule_result_index) {
		if (rule_result_index < 0 || rule_result_index > hex_surface_automata_rule_results.size() - 1) { return false; }

		hex_surface_automata_rule_results.erase(hex_surface_automata_rule_results.begin() + rule_result_index);
		return true;
	}

	void delete_all_rule_results() {
		hex_surface_automata_rule_results.clear();
		hex_surface_automata_rule_results.shrink_to_fit();
	}

	void delete_all_automata_rules() {
		hex_surface_automata_rules_widget.delete_all_rules();
	}

	hex_surface_automata_rule_result_struct_type* get_rule_result(int rule_result_id) {
		for (hex_surface_automata_rule_result_struct_type rule_result : hex_surface_automata_rule_results) {
			if (rule_result.rule_result_id == rule_result_id) {
				return &rule_result;
				break;
			}
		}

		return nullptr;
	}

	bool modify_rule_result(hex_surface_automata_rule_result_struct_type rr) {
		for (hex_surface_automata_rule_result_struct_type rule_result : hex_surface_automata_rule_results) {
			if (rule_result.rule_result_id == rr.rule_result_id) {
				rule_result = rr;
				return true;
				break;
			}
		}

		return false;
	}

	void hex_automata_rule_results_editor(hex_grid_base_class<T>* hex_surface_object_to_execute,ImVec2 editor_display_window_size) {
		ImGui::BeginChild("Automata Rule Results", editor_display_window_size, true);

		if (ImGui::BeginPopupContextWindow(0, 1)) {
			ImVec2 click_pos = ImGui::GetMousePosOnOpeningCurrentPopup();

			if (ImGui::BeginMenu("Automata Rule Result")) {
				if (ImGui::MenuItem("Add Result Definition")) {
					add_new_rule_result();
				}

				if (selected_rule_result_index > -1) {

					if (ImGui::MenuItem("Delete Rule Result")) {
						delete_rule_result(selected_rule_result_index);
						selected_rule_result_index = -1;
					}

					// Following not yet implemented as not decided whether to do so
					//if (ImGui::BeginMenu("Move Rule Result")) {
					//	if (ImGui::MenuItem("Up##mrru")) {
					//		selected_rule_result_index = -1;
					//	}

					//	if (ImGui::MenuItem("Down##srrd")) {
					//		selected_rule_result_index = -1;
					//	}

					//	if (ImGui::MenuItem("To Top##srrt")) {
					//		selected_rule_result_index = -1;
					//	}

					//	if (ImGui::MenuItem("To Bottom##srrb")) {
					//		selected_rule_result_index = -1;
					//	}

					//	ImGui::EndMenu();
					//}
				}
				ImGui::EndMenu();
			}
			ImGui::EndPopup();
		}

		if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
//printf("hex_surface_automata_rules_widget_class :: hex_automata_rules_editor:: LEFT\n");
			selected_rule_result_index = -1;
		}

		display_hex_automata_rule_results(hex_surface_object_to_execute);

		ImGui::EndChild();
	}

	void display_hex_automata_rule_results(hex_grid_base_class<T> *hex_surface_object_to_execute) {
		float x_pos = 5.0f, y_pos = 5.0f;

		float t_height = ImGui::GetTextLineHeight();

		for (size_t i = 0; i < hex_surface_automata_rule_results.size(); i++) {
			float y_pos2 = y_pos + (t_height * 2) * i;
//printf("hex_surface_automata_results_widget_class:display_hex_automata_rule_results :: %i  \n", hex_surface_automata_rule_results.size());
			display_hex_automata_rule_result(hex_surface_object_to_execute,{ x_pos,y_pos2 }, hex_surface_automata_rule_results[i],i);
		}
	}

	void display_hex_automata_rule_result(hex_grid_base_class<T> *hex_surface_object_to_execute,glm::vec2 window_loc, hex_surface_automata_rule_result_struct_type& hex_surface_automata_rule_result,int result_index) {
		ImGui::SetCursorPosX(window_loc.x);
		ImGui::SetCursorPosY(window_loc.y);

		std::string w_id = "##hrrcb" + std::to_string(hex_surface_automata_rule_result.rule_result_id);
		std::string rn_id = "##hrrrn" + std::to_string(hex_surface_automata_rule_result.rule_result_id);
		std::string ss_id_string = "##hrrss" + std::to_string(hex_surface_automata_rule_result.rule_result_id);
		std::string es_id_string = "##hrrer" + std::to_string(hex_surface_automata_rule_result.rule_result_id);
		std::string sr_id_string = "##hrrsr" + std::to_string(hex_surface_automata_rule_result.rule_result_id);
		std::string rv_id_string = "##hrrrv" + std::to_string(hex_surface_automata_rule_result.rule_result_id);
		std::string rc_id_string = "##hrrrc" + std::to_string(hex_surface_automata_rule_result.rule_result_id);
		std::string rs_id_string = "##hrrrs" + std::to_string(hex_surface_automata_rule_result.rule_result_id);
		std::string rss_id_string = "##hrrshs" + std::to_string(hex_surface_automata_rule_result.rule_result_id);
		std::string rd_id_string = "##hrrrd" + std::to_string(hex_surface_automata_rule_result.rule_result_id);

		ImGui::BeginGroup();
		{
			ImGui::Checkbox(w_id.c_str(), &hex_surface_automata_rule_result.active_result);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(120);
			ImGui::InputText(rn_id.c_str(), &hex_surface_automata_rule_result.result_name, ImGuiInputTextFlags_CallbackCharFilter, Input_Filters::name);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(50);
			ImGui::InputInt(ss_id_string.c_str(), &hex_surface_automata_rule_result.rule_start_step, 0);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(50);
			ImGui::InputInt(es_id_string.c_str(), &hex_surface_automata_rule_result.rule_end_step, 0);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(75);
			ImGui::InputText(sr_id_string.c_str(), &hex_surface_automata_rule_result.hex_result_definition, ImGuiInputTextFlags_CallbackCharFilter, rules_input);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(50);
			ImGui::InputText(rv_id_string.c_str(), &hex_surface_automata_rule_result.hex_result_value, ImGuiInputTextFlags_CallbackCharFilter, Input_Filters::number);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(50);
			// interactively change result display color
			if (ImGui::ColorEdit4(rc_id_string.c_str(), (float*)&hex_surface_automata_rule_result.result_display_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_None)) {

if(!hex_surface_object_to_execute) printf("hex_surface_automata_results_widget_class::hex_automata_rule_results_editor : 0000 : !hex_surface_object_to_execute \n");
				hex_grid_class<T>* hex_grid = static_cast<hex_grid_class<T>*> (hex_surface_object_to_execute);
				if(result_index < hex_grid->hex_grid_cells_data_list.size() )
					hex_grid->hex_grid_cells_data_list[result_index].hex_grid_cells_display_color = hex_surface_automata_rule_result.result_display_color;
			}
			ImGui::SameLine();
			ImGui::SetNextItemWidth(60);
			const char* combo_preview_value = shapes[hex_surface_automata_rule_result.display_shape_id];
			if (ImGui::BeginCombo(rs_id_string.c_str(), combo_preview_value, shapes_combo_flags))
			{
				for (int n = 0; n < IM_COUNTOF(shapes); n++)
				{
					const bool is_selected = (hex_surface_automata_rule_result.display_shape_id == n);
					if (ImGui::Selectable(shapes[n], is_selected)) {
						hex_surface_automata_rule_result.display_shape_id = n;
						hex_surface_automata_rule_result.define_imgui_shape();
					}

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected) { 
						ImGui::SetItemDefaultFocus(); }
				}
				ImGui::EndCombo();

if(!hex_surface_object_to_execute) printf("hex_surface_automata_results_widget_class::hex_automata_rule_results_editor : 1111 : !hex_surface_object_to_execute \n");
				// interactively change result display shape
				hex_grid_class<T>* hex_grid = static_cast<hex_grid_class<T>*> (hex_surface_object_to_execute);
				if (result_index < hex_grid->hex_grid_cells_data_list.size()) {
					hex_grid->hex_grid_cells_data_list[result_index].hex_grid_cells_display_shape = hex_surface_automata_rule_result.result_display_shape;
				}
			}
			ImGui::SameLine();
			ImGui::SetNextItemWidth(60);
			// interactively change result display shape size
			if (ImGui::DragFloat(rss_id_string.c_str(), &hex_surface_automata_rule_result.display_shape_size,0.1f, 1.0f, 50.0f, "%2.2f")) {
if(!hex_surface_object_to_execute) printf("hex_surface_automata_results_widget_class::hex_automata_rule_results_editor : 0000 : !hex_surface_object_to_execute \n");
				hex_grid_class<T>* hex_grid = static_cast<hex_grid_class<T>*>(hex_surface_object_to_execute);
				if (result_index < hex_grid->hex_grid_cells_data_list.size()) {
					hex_grid->hex_grid_cells_data_list[result_index].display_shape_size = hex_surface_automata_rule_result.display_shape_size;
				}
			}
			ImGui::SameLine();
			// interactively display result data
			if (ImGui::Checkbox(rd_id_string.c_str(), &hex_surface_automata_rule_result.display_result)) {
if(!hex_surface_object_to_execute) printf("hex_surface_automata_results_widget_class::hex_automata_rule_results_editor : 0000 : !hex_surface_object_to_execute \n");
				hex_grid_class<T>* hex_grid = static_cast<hex_grid_class<T>*> (hex_surface_object_to_execute);
				if (result_index < hex_grid->hex_grid_cells_data_list.size()) {
					hex_grid->hex_grid_cells_data_list[result_index].display_data = hex_surface_automata_rule_result.display_result;
				}
			}
		}
		ImGui::EndGroup();

		if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {//selection of sub rule by hovering over input test entry and right mouse 
//printf("hex_surface_automata_results_widget_class :: display_hex_automata_rule_result:: %i : %s\n", hex_surface_automata_rule_result.rule_result_id, hex_surface_automata_rule_result.result_name.c_str());
			selected_rule_result_index = result_index;
		}

	}


	int get_min_rule_start() {
		int min_rule_start = 0;

		if (!hex_surface_automata_rule_results.empty()) {
			min_rule_start = hex_surface_automata_rule_results[0].rule_start_step;
		}

		for (hex_surface_automata_rule_result_struct_type rule_result : hex_surface_automata_rule_results) {
			if (rule_result.rule_start_step < min_rule_start) min_rule_start = rule_result.rule_start_step;
		}

		return min_rule_start;
	}

	int get_max_rule_end() {
		int max_rule_end = 0;

		if (!hex_surface_automata_rule_results.empty()) {
			max_rule_end = hex_surface_automata_rule_results[0].rule_end_step;
		}

		for (hex_surface_automata_rule_result_struct_type rule_result : hex_surface_automata_rule_results) {
			if (rule_result.rule_end_step > max_rule_end) { max_rule_end = rule_result.rule_end_step; }
		}

		return max_rule_end;
	}

	// !!!!!!!!!!! Begin Verify rule results !!!!!!!!!!!!!!!
	bool verify_result_rules(hex_grid_base_class<T> *hex_surface_object_to_execute) {
printf("hex_surface_automata_results_widget_class :: verify_result_rules 000");
		for (hex_surface_automata_rule_result_struct_type rule_result : hex_surface_automata_rule_results) {
			if (!verify_result_rule(hex_surface_object_to_execute,rule_result)) { return false; }
		}
	
		for (hex_surface_automata_rule_struct_type &rule : hex_surface_automata_rules_widget.hex_surface_automata_rules) {
printf("hex_surface_automata_results_widget_class :: verify_result_rules 111");
printf(": rule :: %s\n",rule.rule_name.c_str());
			//if (!verify_rule(rule)){ return false;}

			for (hex_automata_sub_rule_struct_type &sub_rule : rule.sub_rules) {
printf("hex_surface_automata_results_widget_class :: verify_result_rules 222");
printf(": rule "); printf("%s sub rule %s !! \n", rule.rule_name.c_str(), sub_rule.sub_rule_name.c_str());
				parser_base_node *logic_parser_tree = hex_surface_automata_rules_widget.valid_sub_rule_definition(sub_rule.sub_rule_definition); // For testing

				if (!logic_parser_tree) {// Sub rule code is invalid and a parser tree could not be constructed : define sub rule parameters for a null parser tree
//logic_parser.display_parse_tree(logic_parser_tree); // For testing only
					afw_globalc::get_current_logger()->log(LogLevel::ERROR, "Have invalid sub rule " + sub_rule.sub_rule_name + " of rule " + rule.rule_name);
					hex_surface_automata_rules_widget.logic_parser_tree_defined = false;
					sub_rule.root_parser_node = nullptr;
					return false;
				}

				//have valid parser_tree : define sub rule parameters for valid parser tree 
				sub_rule.edit_sub_rule      = false;
				sub_rule.edit_sub_rule_text = false;
				sub_rule.root_parser_node   = logic_parser_tree;
				hex_surface_automata_rules_widget.logic_parser_tree_defined   = true;
				printf("hex_surface_automata_results_widget_class::verify_result_rules: Have valid automata sub rule definition accepted for  !!!!\n");
				afw_globalc::get_current_logger()->log(LogLevel::INFO, "hex_surface_automata_results_widget_class::verify_result_rules: Have valid automata sub rule definition accepted for " + sub_rule.sub_rule_name + "!!!!\n");
			}
		}

		return true;
	}

	//bool verify_rule(hex_surface_automata_rule_struct_type rule) {
	//	if (!valid_start_step(rule.rule_start_step, rule.rule_end_step)) {
	//		//vwDialogs::display_error_message("Hex Rule Result", "ERROR : \nHave invalid start step value for rule " + ahex_tools::get_propper_string(rule.rule_name) + " that is < 0 or above the end step value\n");
	//		vwDialogs::display_error_message("Hex Rule Result", "ERROR : \nHave invalid start step value for rule " + rule.rule_name + " that is < 0 or above the end step value\n");
	//		return false;
	//	}

	//	if (!valid_end_step(rule.rule_start_step, rule.rule_end_step)) {
	//		//vwDialogs::display_error_message("Hex Rule Result", "ERROR : \nHave invalid end step value for rule " + ahex_tools::get_propper_string(rule.rule_name) + " that is < 0 below the end step value\n");
	//		vwDialogs::display_error_message("Hex Rule Result", "ERROR : \nHave invalid end step value for rule " + rule.rule_name + " that is < 0 below the end step value\n");
	//		return false;
	//	}

	//	return true;
	//}

	
	bool verify_result_rule(hex_grid_base_class<T> *hex_surface_object_to_execute,hex_surface_automata_rule_result_struct_type rule_result) {
		if (!valid_start_step(rule_result.rule_start_step, rule_result.rule_end_step)) {
			vwDialogs::display_error_message("Hex Rule Result", "ERROR : \nHave invalid start step value that is < 0 or above the end step value for rule result : " + rule_result.result_name + "\n");
			return false;
		}

		if (!valid_end_step(rule_result.rule_start_step, rule_result.rule_end_step)) {
			vwDialogs::display_error_message("Hex Rule Result", "ERROR : \nHave invalid end step value that is < 0 or above the end step value for rule result : " + rule_result.result_name + "\n");
			return false;
		}

		if (!valid_rules(rule_result)) {
			return false;
		}

		if (!valid_result(hex_surface_object_to_execute,rule_result.hex_result_value)) {
			vwDialogs::display_error_message("Hex Rule Result", "ERROR : \nHave invalid rule result value that is incompatibe for hex grid data type for rule result : " + rule_result.result_name + "\n");
			return false;
		}

		return true;
	}

	bool valid_start_step(int start_step, int end_step) {
		if (start_step < 0 || start_step > end_step) {
			return false;
		} else {
			return true;
		}
	}

	bool valid_end_step(int start_step, int end_step) {
		if (end_step < 0 || end_step < start_step){
			return false;
		} else {
			return true;}
	}

	bool valid_rules(hex_surface_automata_rule_result_struct_type rule_result) {
		std::string rule_definitions = rule_result.hex_result_definition;

//printf("hex_surface_automata_results_widget_class : valid_rules 0000 |%s| %i\n", rule_definitions.c_str(), rule_definitions.size());
		if (rule_definitions == "\0" || rule_definitions == "\n" || rule_definitions == "") { return true; }// Want to use all rules to define result value
//printf("hex_surface_automata_results_widget_class : valid_rules 1111\n");

		std::vector<std::string> rule_ids = FW::stringtools::split(rule_definitions, ":");
//printf("hex_surface_automata_results_widget_class : valid_rules 2222 |%s|\n",rule_definitions.c_str());
//printf("hex_surface_automata_results_widget_class : valid_rules %i\n", rule_ids.size());

		// Will have at least one rule id defined by split function of rule_definitions
		// If no rule id specified then all rules are to be applied so return true
		//if (rule_ids.size() == 1  && (rule_ids[0] == "\n" || rule_ids[0] == "\0" || rule_ids[0].size() == 0)) return true; // This and variations does not work !!!!! Have no idea
//printf("hex_surface_automata_results_widget_class : valid_rules 3333 %i\n", rule_ids.size());

		for (std::string rule_id : rule_ids) {
//printf("hex_surface_automata_results_widget_class : result rule |%s|\n", rule_id.c_str());

			int rule_idi;
			try { rule_idi = stoi(rule_id); }
			catch (std::invalid_argument const& ex)
			{
				// Will have at least one rule id defined by split function of rule_definitions
				// If no rule id specified then all rules are to be applied so return true
				// Need this because cannot return true for a null entry of rule_definitions to specify to use all automata rules
				if (rule_ids.size() == 1) { return true; }
				//vwDialogs::display_error_message("Hex Rule", "ERROR : \nHave invalid argument for rule id " + ahex_tools::get_propper_string(rule_id) + " for rule result " + ahex_tools::get_propper_string(rule_result.result_name) + " that is not defined in the list of rules\n");
				vwDialogs::display_error_message("Hex Rule", "ERROR : \nHave invalid argument for rule id " + rule_id + " for rule result " + rule_result.result_name + " that is not defined in the list of rules\n");
				return false;
			}
			catch (std::out_of_range const& ex)
			{
				// Will have at least one rule id defined by split function of rule_definitions
				// If no rule id specified then all rules are to be applied so return true
				// Need this because cannot return true for a null entry of rule_definitions to specify to use all automata rules
				if (rule_ids.size() == 1) { return true; }
				//afw_globalc::get_current_logger()->log(LogLevel::ERROR, "Have rule id out of range for rule id" + ahex_tools::get_propper_string(rule_id) + " for rule result " + ahex_tools::get_propper_string(rule_result.result_name) + " that is not defined in the list of rules !!!!\n");
				//vwDialogs::display_error_message("Hex Rule", "ERROR : \nHave rule id out of range for rule id" + ahex_tools::get_propper_string(rule_id) + " for rule result " + ahex_tools::get_propper_string(rule_result.result_name) + " that is not defined in the list of rules\n");
				afw_globalc::get_current_logger()->log(LogLevel::ERROR, "Have rule id out of range for rule id" + rule_id + " for rule result " +rule_result.result_name + " that is not defined in the list of rules !!!!\n");
				vwDialogs::display_error_message("Hex Rule", "ERROR : \nHave rule id out of range for rule id" + rule_id + " for rule result " + rule_result.result_name + " that is not defined in the list of rules\n");
				return false;
			}

			if (!valid_rule(rule_idi)) {
				//vwDialogs::display_error_message("Hex Rule", "ERROR : \nHave rule of id : " + ahex_tools::get_propper_string(rule_id) + " for rule result : " + ahex_tools::get_propper_string(rule_result.result_name) + " that is not defined in the list of rules\n");
				vwDialogs::display_error_message("Hex Rule", "ERROR : \nHave rule of id : " + rule_id + " for rule result : " + rule_result.result_name + " that is not defined in the list of rules\n");
				return false;
			}
		}

		return true;
	}

	bool valid_rule(int rule_id) {
		if (rule_id >= 0 and rule_id < hex_surface_automata_rules_widget.hex_surface_automata_rules.size()) {
			return true;
		} else{
			return false;
		}
	}

	bool valid_result(hex_grid_base_class<T> *hex_surface_object_to_execute,std::string hex_result_value) {

		switch (hex_surface_object_to_execute->hex_grid_value_data_type) {
			case hex_grid_value_data_type_enum::Boolean:
			case hex_grid_value_data_type_enum::Integer: {
				if (FW::stringtools::contains(hex_result_value,"e") || FW::stringtools::contains(hex_result_value,".") || FW::stringtools::contains(hex_result_value,"^")) {
					//afw_globalc::get_current_logger()->log(LogLevel::ERROR, "Have invalid rule result value that is incompatibe for hex grid integer data type !!!!\n");
					vwDialogs::display_error_message("Hex Rule", "ERROR : \nHave invalid rule result value that is incompatibe for hex grid integer data type\n");
					return false;
				}

				try { stoi(hex_result_value);}
				catch (std::invalid_argument const& ex)	{
					return false;
				}
				catch (std::out_of_range const& ex) {
					return false;
				}
				break;
			}
			case hex_grid_value_data_type_enum::Float: {
				try { stof(hex_result_value); }
				catch (std::invalid_argument const& ex) {
					return false;
				}
				catch (std::out_of_range const& ex) {
					return false;
				}
				break;
			}
			case hex_grid_value_data_type_enum::Double: {
				try { stod(hex_result_value); }
				catch (std::invalid_argument const& ex) {
					return false;
				}
				catch (std::out_of_range const& ex) {
					return false;
				}
				break;
			}
		}

		return true;
	}
	// !!!!!!!!!!! End Verify rule results !!!!!!!!!!!!!!!

	// !!!!!!!!!!! Begin process rule results !!!!!!!!!!!!!!!
	bool perform_hex_surface_automata_generation(int step,bool ignore_individual_step_intervals, hex_grid_class<T> *hex_surface_object_to_execute, std::vector<T> &hex_grid_copy) {
//printf("hex_surface_automata_results_widget_class :: perform_hex_surface_automata_generation 0000: %i\n" , step);
		if (!hex_surface_object_to_execute) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "hex_surface_automata_results_widget_class::perform_hex_surface_automata_generation() Have hex grid data not defined\n");
			return false;
		}

		if (!automata_rules_verified) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "hex_surface_automata_results_widget_class::perform_hex_surface_automata_generation()  automata rules have not been verified!!!!\n Verify Automamata rules before proceeding.\n");
			return false;
		}

		if (hex_surface_object_to_execute->hex_grid.size() == 0) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "hex_surface_automata_results_widget_class::perform_hex_surface_automata_generation() Have Hex grid data not initialised\n");
			return false;
		}

//printf("hex_surface_automata_results_widget_class :: perform_hex_surface_automata_generation 1111:"); printf(" %i : %i : %i\n" , step, hex_surface_object_to_execute->hex_grid.size(), hex_grid_copy.size());
		// Clear or create hex cell data list of all index and hex coordinate data that is to be displayed on screen for active result 
		for (int i = 0; i < hex_surface_automata_rule_results.size();i++) {
			hex_surface_automata_rule_result_struct_type &rule_result = hex_surface_automata_rule_results[i];
//printf("hex_surface_automata_results_widget_class :: perform_hex_surface_automata_generation 2222:");printf("%i : %s\n" , step, rule_result.result_name.c_str());
				if (rule_result.hex_grid_cell_data_id < 0) {// Result Hex grid call data list not defined
//printf("hex_surface_automata_results_widget_class :: perform_hex_surface_automata_generation 3333:");printf("%i : %i\n" , step, rule_result.hex_grid_cell_data_id);
					rule_result.hex_grid_cell_data_id = hex_surface_object_to_execute->define_new_hex_grid_cell_data();
//printf("hex_surface_automata_results_widget_class :: perform_hex_surface_automata_generation 4444:");printf("%i : %i\n" , step, rule_result.hex_grid_cell_data_id);
					if (rule_result.hex_grid_cell_data_id < 0) {
						afw_globalc::get_current_logger()->log(LogLevel::INFO, "hex_surface_automata_results_widget_class::perform_hex_surface_automata_generation() Could not define a hex grid cells results data list\n");
						return false;
					}

if(i>= hex_surface_object_to_execute->hex_grid_cells_data_list.size()) {
printf("hex_surface_automata_results_widget_class::perform_hex_surface_automata_generation() 4444AAAA: Error : i>=hex_grid_cells_data_list.size()");printf(": %i : %i\n",i, hex_surface_object_to_execute->hex_grid_cells_data_list.size());//+++++
}

//printf("hex_surface_automata_results_widget_class :: perform_hex_surface_automata_generation 4444BBBB:"); printf("% i : %i : %i\n", step, rule_result.hex_grid_cell_data_id, rule_result.display_shape_id);

					hex_surface_object_to_execute->hex_grid_cells_data_list[i].hex_grid_cells_display_color = rule_result.result_display_color;
					hex_surface_object_to_execute->hex_grid_cells_data_list[i].hex_grid_cells_display_shape = rule_result.result_display_shape;
					hex_surface_object_to_execute->hex_grid_cells_data_list[i].display_data = rule_result.display_result;
				}
				else {
//printf("hex_surface_automata_results_widget_class :: perform_hex_surface_automata_generation 5555:");printf("% i : % i\n" , step, rule_result.hex_grid_cell_data_id);
					hex_surface_object_to_execute->clear_hex_grid_cell_data(rule_result.hex_grid_cell_data_id);
				}
//printf("hex_surface_automata_results_widget_class :: perform_hex_surface_automata_generation 7777:"); printf(" %i : %i\n" , step, rule_result.hex_grid_cell_data_id);
		}

//printf("hex_surface_automata_results_widget_class :: perform_hex_surface_automata_generation 8888:"); printf(" %i : %i : %i\n" , step, hex_surface_object_to_execute->hex_grid.size(), hex_grid_copy.size());
		for (hex_surface_index_data_type hex_index=0; hex_index < hex_surface_object_to_execute->hex_grid.size(); hex_index++) {
			// !!!!!!!!!!!!!! NOTE : Need to take into account the hex indices that are on the edge or boundaries of the hex grid !!!!!!!!!!!!!!!!!!!!!!!

//printf("hex_surface_automata_results_widget_class :: perform_hex_surface_automata_generation 9999:"); printf(" %i\n" , step);
			for (size_t rule_result_index = 0; rule_result_index < hex_surface_automata_rule_results.size(); rule_result_index ++) {
				hex_surface_automata_rule_result_struct_type rule_result =  hex_surface_automata_rule_results[rule_result_index];
//printf("hex_surface_automata_results_widget_class :: perform_hex_surface_automata_generation AAAA:"); printf(" %i : %i :%s \n" , step, rule_result.active_result, rule_result.result_name.c_str());

				if (rule_result.active_result) {
					if ((step >= rule_result.rule_start_step  && step <= rule_result.rule_end_step) || ignore_individual_step_intervals) { // perform automata step if within specified step range
						//printf("hex_surface_automata_results_widget_class :: perform_hex_surface_automata_generation BBBB:"); printf(" %i : %i :%s : %s\n" , step, rule_result.active_result, rule_result.result_name.c_str(), rule_result.hex_result_definition.c_str());
						std::string rule_definitions = FW::stringtools::trim(rule_result.hex_result_definition);
						//printf("hex_surface_automata_results_widget_class :: perform_hex_surface_automata_generation CCCC111AAAA:"); printf(" %i : %i :%s : |%s| :|%s| : %i\n" , hex_index, rule_result.active_result, rule_result.result_name.c_str(), rule_result.hex_result_definition.c_str(), rule_definitions.c_str(), rule_definitions.size());
						if (rule_definitions.empty() || rule_definitions[0] == '\0' || rule_definitions[0] == '\n') {// Want to use all rules to define result value
							//printf("hex_surface_automata_results_widget_class :: perform_hex_surface_automata_generation CCCC111BBB:\n");
							if (all_rule_conditions_met(rule_result, hex_surface_object_to_execute, hex_index)) {
								//printf("hex_surface_automata_results_widget_class :: perform_hex_surface_automata_generation CCCC111CCC:\n");
								assign_rule_result(hex_surface_object_to_execute, rule_result, hex_grid_copy, hex_index);
								//printf("hex_surface_automata_results_widget_class :: perform_hex_surface_automata_generation CCCC111DDD:\n");
								hex_surface_object_to_execute->add_hex_grid_cell_index(rule_result_index, hex_index);
								//printf("hex_surface_automata_results_widget_class :: perform_hex_surface_automata_generation CCCC111EEE:\n");
								rule_result_index = hex_surface_automata_rule_results.size();// Once a rule result condition is met, any remaining rule result conditions are skipped and ignored.
							}
						}
						else { // Use specified automata rules
							//printf("hex_surface_automata_results_widget_class :: perform_hex_surface_automata_generation CCCC222AAA:\n");
							if (all_specified_rule_conditions_met(rule_result, hex_surface_object_to_execute, hex_index)) {
								//printf("hex_surface_automata_results_widget_class :: perform_hex_surface_automata_generation CCCC222BBB:\n");
								assign_rule_result(hex_surface_object_to_execute, rule_result, hex_grid_copy, hex_index);
								//printf("hex_surface_automata_results_widget_class :: perform_hex_surface_automata_generation CCCC222CCC:\n");
								hex_surface_object_to_execute->add_hex_grid_cell_index(rule_result_index, hex_index);
								//printf("hex_surface_automata_results_widget_class :: perform_hex_surface_automata_generation CCCC222DDD\n");
								rule_result_index = hex_surface_automata_rule_results.size();// Once a rule result condition is met, any remaining rule result conditions are skipped and ignored.
							}
						}
					}
				}
			}
		}

		return true;
	}
	

	bool all_rule_conditions_met(hex_surface_automata_rule_result_struct_type &rule_result, hex_grid_base_class<T>* hex_surface_object_to_execute, hex_surface_index_data_type hex_index) {
		if (!hex_surface_object_to_execute) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "hex_surface_automata_results_widget_class::all_rule_conditions_met : ERROR :  Could not test all rule conditions met. hex_surface_object_to_execute undefined \n");
			return false;
		}

		if (hex_surface_automata_rules_widget.hex_surface_automata_rules.empty()) {
			vwDialogs::display_error_message("Hex Rule", "ERROR : \nno rules defined to evaluate for result rule\n Cannot perform automata.\n");
			return false;
		}
		
		bool have_active_and_valid_rule_met = false;// Flag to indicate if have active and valid rule met

//printf("hex_surface_automata_results_widget_class :: all_rule_conditions_met 000000: %i : :%i\n" , hex_index, hex_surface_automata_rules_widget.hex_surface_automata_rules.size());
		for (hex_surface_automata_rule_struct_type rule : hex_surface_automata_rules_widget.hex_surface_automata_rules) {
//printf("hex_surface_automata_results_widget_class :: all_rule_conditions_met 11111: %i : %i :%s : %s\n" , hex_index, rule.active_rule, rule_result.result_name.c_str(), rule.rule_name.c_str());
			//if (rule.active_rule) {
				if (!all_sub_rule_conditions_met(rule, hex_surface_object_to_execute, hex_index)) {

// testing only : delete comment out when not needed
//printf("hex_surface_automata_results_widget_class :: all_rule_conditions_met :: NOT all_rule_conditions_met 222222: %i : %s : ", hex_index, rule.sub_rules[0].sub_rule_name.c_str());
//switch (rule.sub_rules[0].root_parser_node->value.first) {
//    case parse_value_data_type_enum::Boolean: printf("Boolean %i :: \n", rule.sub_rules[0].root_parser_node->value.second.bvalue); break;
//    case parse_value_data_type_enum::Integer: printf("Integer %i :: \n", rule.sub_rules[0].root_parser_node->value.second.ivalue); break;
//    case parse_value_data_type_enum::Float:   printf("Float   %f :: \n", rule.sub_rules[0].root_parser_node->value.second.fvalue); break;
//    case parse_value_data_type_enum::Double:  printf("Double  %d :: \n", rule.sub_rules[0].root_parser_node->value.second.dvalue); break;
//}

					return false;
				}
				have_active_and_valid_rule_met = true;
// testing only : delete comment out when not needed
//printf("hex_surface_automata_results_widget_class :: all_rule_conditions_met 33333: %s : " , rule.sub_rules[0].sub_rule_name.c_str());
//switch (rule.sub_rules[0].root_parser_node->value.first) {
//    case parse_value_data_type_enum::Boolean: printf("Boolean %i :: \n", rule.sub_rules[0].root_parser_node->value.second.bvalue); break;
//    case parse_value_data_type_enum::Integer: printf("Integer %i :: \n", rule.sub_rules[0].root_parser_node->value.second.ivalue); break;
//    case parse_value_data_type_enum::Float:   printf("Float   %f :: \n", rule.sub_rules[0].root_parser_node->value.second.fvalue); break;
//    case parse_value_data_type_enum::Double:  printf("Double  %d :: \n", rule.sub_rules[0].root_parser_node->value.second.dvalue); break;
//}
			//}
		}

		return have_active_and_valid_rule_met;
	}

	bool all_specified_rule_conditions_met(hex_surface_automata_rule_result_struct_type &rule_result, hex_grid_base_class<T>* hex_surface_object_to_execute, hex_surface_index_data_type hex_index) {
		if (!hex_surface_object_to_execute) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "hex_surface_automata_results_widget_class::all_specified_rule_conditions_met : ERROR :  Could not test all rule conditions met. hex_surface_object_to_execute undefined \n");
			return false;
		}

//printf("hex_surface_automata_results_widget_class : all_specified_rule_conditions_met 1111\n");
		std::vector<std::string> rule_ids = FW::stringtools::split(rule_result.hex_result_definition, ":");
//printf("hex_surface_automata_results_widget_class : all_specified_rule_conditions_met 2222 |%s|\n", rule_result.hex_result_definition.c_str());
//printf("hex_surface_automata_results_widget_class : all_specified_rule_conditions_met %i\n", rule_ids.size());

		// Will have at least one rule id defined by split function of rule_definitions
		// If no rule id specified then all rules are to be applied so return true
		//if (rule_ids.size() == 1  && (rule_ids[0] == "\n" || rule_ids[0] == "\0" || rule_ids[0].size() == 0)) return true; // This and variations does not work !!!!! Have no idea why
//printf("hex_surface_automata_results_widget_class : all_specified_rule_conditions_met 3333 %i\n", rule_ids.size());

		bool have_active_and_valid_rule_met = false;// Flag to indicate if have active and valid rule met

		// Test that all rule id numbers are valid
		for (std::string rule_id : rule_ids) {
//printf("hex_surface_automata_results_widget_class : all_specified_rule_conditions_met 4444 |%s|\n", rule_id.c_str());

			int rule_idi = 0 ;
			try { rule_idi = stoi(rule_id); }
			catch (std::invalid_argument const& ex)
			{
				// Will have at least one rule id defined by split function of rule_definitions
				// If no rule id specified then all rules are to be applied so return true
				// Need this because cannot return true for a null entry of rule_definitions to specify to use all automata rules
				if (rule_ids.size() == 1) { return true; }
				//vwDialogs::display_error_message("Hex Rule", "ERROR : \nHave invalid argument for rule id " + ahex_tools::get_propper_string(rule_id) + " for rule result " + ahex_tools::get_propper_string(rule_result.result_name) + " that is not defined in the list of rules.\n");
				vwDialogs::display_error_message("Hex Rule", "ERROR : \nHave invalid argument for rule id " + rule_id + " for rule result " + rule_result.result_name + " that is not defined in the list of rules.\n");
				return false;
			}
			catch (std::out_of_range const& ex)
			{
				// Will have at least one rule id defined by split function of rule_definitions
				// If no rule id specified then all rules are to be applied so return true
				// Need this because cannot return true for a null entry of rule_definitions to specify to use all automata rules
				if (rule_ids.size() == 1) { return true; }
				//vwDialogs::display_error_message("Hex Rule", "ERROR : \nHave rule id out of range for rule id " + ahex_tools::get_propper_string(rule_id) + " for rule result " + ahex_tools::get_propper_string(rule_result.result_name) + " that is not defined in the list of rules.\n");
				vwDialogs::display_error_message("Hex Rule", "ERROR : \nHave rule id out of range for rule id " + rule_id + " for rule result " + rule_result.result_name + " that is not defined in the list of rules.\n");

				return false;
			}

			if (!valid_rule(rule_idi)) {
				//vwDialogs::display_error_message("Hex Rule", "ERROR : \nHave rule id " + ahex_tools::get_propper_string(rule_id) + " for rule result " + ahex_tools::get_propper_string(rule_result.result_name) + " that is not defined in the list of rules.\n");
				vwDialogs::display_error_message("Hex Rule", "ERROR : \nHave rule id " +rule_id + " for rule result " + rule_result.result_name + " that is not defined in the list of rules.\n");
				return false;
			}

			hex_surface_automata_rule_struct_type rule = hex_surface_automata_rules_widget.hex_surface_automata_rules[rule_idi];
			//if (rule.active_rule) {
				if (!all_sub_rule_conditions_met(rule, hex_surface_object_to_execute, hex_index)) {
					return false;
				}
				have_active_and_valid_rule_met = true;
			//}
		}

		return have_active_and_valid_rule_met;
	}

	bool all_sub_rule_conditions_met(hex_surface_automata_rule_struct_type &rule, hex_grid_base_class<T>* hex_surface_object_to_execute, hex_surface_index_data_type hex_index) {
		logic_parser_class<T> logic_parser;

		if (rule.sub_rules.empty()) {
			//afw_globalc::get_current_logger()->log(LogLevel::ERROR, "hex_surface_automata_results_widget_class::all_sub_rule_conditions_met : rule " + rule.rule_name + " has no sub rules !!!!Cannot perform automata. !!!\n");
			//vwDialogs::display_error_message("Hex Rule", "ERROR : \nRule " + ahex_tools::get_propper_string(rule.rule_name) + " has no sub rules. \nCannot perform automata.\n");
			vwDialogs::display_error_message("Hex Rule", "ERROR : \nRule " + rule.rule_name + " has no sub rules. \nCannot perform automata.\n");
			return false;
		}

		bool have_active_sub_rule_met = false;// Flag to indicate if have active and valid sub rule met

		for (hex_automata_sub_rule_struct_type &sub_rule : rule.sub_rules) {
//printf("hex_surface_automata_results_widget_class :: all_sub_rule_conditions_met 000000: %i : :%s : %s\n" , hex_index, rule.rule_name.c_str(), sub_rule.sub_rule_name.c_str());
			if (sub_rule.active_sub_rule) {
//printf("hex_surface_automata_results_widget_class :: all_sub_rule_conditions_met 11111: \n");
				if (sub_rule.root_parser_node == nullptr) {
					return false;
				}

//printf("hex_surface_automata_results_widget_class :: all_sub_rule_conditions_met 22222: \n");
				bool valid_evaluation = logic_parser.evaluate_parse_tree(sub_rule.root_parser_node, hex_surface_object_to_execute, hex_index);

				if (!valid_evaluation) {
					return false;
				}

//printf("hex_surface_automata_results_widget_class::all_sub_rule_conditions_met ####### ::");
//switch (sub_rule.root_parser_node->value.first) {
//case hex_grid_value_data_type_enum::Boolean:  printf("Boolean:%i\n", sub_rule.root_parser_node->value.second.bvalue); break;
//case hex_grid_value_data_type_enum::Float:    printf("Float:  %f\n", sub_rule.root_parser_node->value.second.fvalue); break;
//case hex_grid_value_data_type_enum::Integer:  printf("Integer:%i\n", sub_rule.root_parser_node->value.second.ivalue); break;
//case hex_grid_value_data_type_enum::Double:   printf("Double: %d\n", sub_rule.root_parser_node->value.second.dvalue); break;
//}

				switch (sub_rule.root_parser_node->value.first) {
					case(parse_value_data_type_enum::Boolean): if (!sub_rule.root_parser_node->value.second.bvalue) { sub_rule.sub_rule_conditions_met = false; return false; } break;
					case(parse_value_data_type_enum::Float)  : if (sub_rule.root_parser_node->value.second.fvalue > 0.00000000001f) { sub_rule.sub_rule_conditions_met = false; return false; } break;// This is as a precaution that a zero value in a float may not be stored as a zero value
					case(parse_value_data_type_enum::Integer): if (sub_rule.root_parser_node->value.second.ivalue > 0) { sub_rule.sub_rule_conditions_met = false; return false; } break;
					case(parse_value_data_type_enum::Double) : if (sub_rule.root_parser_node->value.second.dvalue > 0.00000000001f) { sub_rule.sub_rule_conditions_met = false; return false; } break;// This is as a precaution that a zero value in a double may not be stored as a zero value
					//case(parse_value_data_type_enum::nan)    :  {sub_rule.root_parser_node->value.second.ivalue = 0; sub_rule.sub_rule_conditions_met = false; return false; } break;//++++++++
				}

				have_active_sub_rule_met = true;
			}
		}

		return have_active_sub_rule_met;
	}

	void assign_rule_result(hex_grid_class<T> *hex_surface_object_to_execute,hex_surface_automata_rule_result_struct_type rule_result, std::vector<T>& hex_grid_copy, hex_surface_index_data_type hex_index) {
//printf("hex_surface_automata_results_widget_class :: assign_rule_result 000\n");

		if (!hex_surface_object_to_execute) {// Here to test for debug 
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "hex_surface_automata_results_widget_class::assign_rule_result : ERROR :  Could not assign rule result. hex_surface_object_to_execute undefined \n");
			return ;
		}
//printf("hex_surface_automata_results_widget_class :: assign_rule_result 111\n");
		if (hex_index >= 0 && hex_index < hex_grid_copy.size()) {
			switch (hex_surface_object_to_execute->hex_grid_value_data_type) {
				case hex_grid_value_data_type_enum::Boolean:
				case hex_grid_value_data_type_enum::Integer: {
					hex_grid_copy[hex_index] = stoi(rule_result.hex_result_value);
					break;
				}
				case hex_grid_value_data_type_enum::Float: {
					hex_grid_copy[hex_index] = stof(rule_result.hex_result_value);
					break;
				}
				case hex_grid_value_data_type_enum::Double: {
					hex_grid_copy[hex_index] = stod(rule_result.hex_result_value);
				}
			}
		}
//printf("hex_surface_automata_results_widget_class :: assign_rule_result 222\n");
	}
	
	// !!!!!!!!!!! End process rule results !!!!!!!!!!!!!!!

protected:

private:
	int rule_result_id = -1;
	int selected_rule_result_index = -1;

	const char* shapes[9] = { "Circle", "Square", "Diamond", "Up", "Down", "Left", "Right", "Cross", "Asterix" };
	ImGuiComboFlags shapes_combo_flags = ImGuiComboFlags_NoArrowButton;

	static int rules_input(ImGuiInputTextCallbackData* data)
	{
		if (data->EventChar < 256 && strchr("0123456789:", (char)data->EventChar)){
			return 0;
		}

		return 1;
	}


};
