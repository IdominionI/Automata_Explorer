#pragma once

#include <ImGui/imgui_docking/misc/cpp/imgui_stdlib.h>// Need This so as to be able to input text as a std::string variable

#include "cart2D_surface_automata_rules_widget.h"

/*
			Cartesian 2D cellula automata rule results widget class

	This class widget defines an ImGui widget that is used to define
	a set of cellula automata rule results that the Cartesian 2D surface
	grid model cells are to be assigned to based upon one or more 
	cellula automata rules. 

	This class also handles the management of the list and display of the
	cellula automata rule results that are to be performed for every
	iteration process.

*/

template <class T>
class cart2D_surface_automata_results_widget_class {
public:
	cart2D_surface_automata_results_widget_class() {}
	~cart2D_surface_automata_results_widget_class() {}

	int current_selected_rule_result_id = -1;
	bool automata_rules_verified		= false;// Idicate if all cellular automata rules have been verified or not

	std::vector<cart2D_surface_automata_rule_result_struct_type> cart2D_surface_automata_rule_results; // List of Cartesian 2D cellula automata rule results

	// Class that defines the display, management of available Cartesian 2D cellula automata rules 
	// that can be used to defeine a Cartesian 2D cellula automata rule result
	cart2D_surface_automata_rules_widget_class<T> cart2D_surface_automata_rules_widget;

	// !!!!!!!!!!!!! Functions to perfom the management of Cartesian 2D cellula automata rule results !!!!!!!!!!!!!!!!!!!!

	void reset_rule_results_cart2D_grid_cell_data_id() {// This is required when clearing or importing a hex surface grid
		for (cart2D_surface_automata_rule_result_struct_type& rule_result : cart2D_surface_automata_rule_results) {
			rule_result.cart2D_grid_cell_data_id = -1;
		}
	}

	void add_rule_result(cart2D_surface_automata_rule_result_struct_type rule_result) {
		if (rule_result.rule_result_id < 0) {
			rule_result_id += 1;
			rule_result.rule_result_id = rule_result_id;
		}
		cart2D_surface_automata_rule_results.push_back(rule_result);
	}

	void add_new_rule_result() {
		cart2D_surface_automata_rule_result_struct_type rule_result;
		add_rule_result(rule_result);
	}

	void define_and_clear_cart2D_surface_automata_rules_and_results() {
		rule_result_id = -1;
		delete_all_automata_rules();
		clear_cart2D_surface_automata_rule_results();
	}

	void clear_cart2D_surface_automata_rule_results() {
		cart2D_surface_automata_rule_results.clear();
		cart2D_surface_automata_rule_results.shrink_to_fit();
	}

	void clear_cart2D_surface_automata_rules() {
		cart2D_surface_automata_rules_widget.cart2D_surface_automata_rules.clear();
		cart2D_surface_automata_rules_widget.cart2D_surface_automata_rules.shrink_to_fit();
	}

	bool delete_rule_result(int rule_result_index) {
		if (rule_result_index < 0 || rule_result_index > cart2D_surface_automata_rule_results.size() - 1) { return false; }

		cart2D_surface_automata_rule_results.erase(cart2D_surface_automata_rule_results.begin() + rule_result_index);
		return true;
	}

	void delete_all_rule_results() {
		cart2D_surface_automata_rule_results.clear();
		cart2D_surface_automata_rule_results.shrink_to_fit();
	}

	void delete_all_automata_rules() {
		cart2D_surface_automata_rules_widget.delete_all_rules();
	}

	cart2D_surface_automata_rule_result_struct_type* get_rule_result(int rule_result_id) {
		for (cart2D_surface_automata_rule_result_struct_type rule_result : cart2D_surface_automata_rule_results) {
			if (rule_result.rule_result_id == rule_result_id) {
				return &rule_result;
				break;
			}
		}

		return nullptr;
	}

	bool modify_rule_result(cart2D_surface_automata_rule_result_struct_type rr) {
		for (cart2D_surface_automata_rule_result_struct_type rule_result : cart2D_surface_automata_rule_results) {
			if (rule_result.rule_result_id == rr.rule_result_id) {
				rule_result = rr;
				return true;
				break;
			}
		}

		return false;
	}


	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	// Display the main Cartesian 2D automata rule results editor and manager section
	void cart2D_automata_rule_results_editor(cart2D_grid_base_class<T>* cart2D_surface_object_to_execute,ImVec2 editor_display_window_size) {
		ImGui::BeginChild("Automata Rule Results", editor_display_window_size, true);

		// ImGui popup menu to manage individual callula automata rule results as a floating menu
		if (ImGui::BeginPopupContextWindow(0, 1)) {
			ImVec2 click_pos = ImGui::GetMousePosOnOpeningCurrentPopup();

			if (ImGui::BeginMenu("Automata Rule Result")) {
				if (ImGui::MenuItem("Add Result Definition")) {
					add_new_rule_result();
				}

				if (selected_rule_result_index > -1) {// Index of selected rule results to perform management tasks upon is not none

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

		if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {// Selected rule results to perform management tasks upon to none
//printf("cart2D_surface_automata_rules_widget_class :: cart2D_automata_rules_editor:: LEFT\n");
			selected_rule_result_index = -1;
		}

		display_cart2D_automata_rule_results(cart2D_surface_object_to_execute);

		ImGui::EndChild();
	}

	// Function to display hexagaonal cellular automata rule results as a sequence of rows of ImGui widgets
	void display_cart2D_automata_rule_results(cart2D_grid_base_class<T> *cart2D_surface_object_to_execute) {
		float x_pos = 5.0f, y_pos = 5.0f;

		float t_height = ImGui::GetTextLineHeight();

		for (size_t i = 0; i < cart2D_surface_automata_rule_results.size(); i++) {
			float y_pos2 = y_pos + (t_height * 2) * i;
//printf("cart2D_surface_automata_results_widget_class:display_cart2D_automata_rule_results :: %i  \n", cart2D_surface_automata_rule_results.size());
			display_cart2D_automata_rule_result(cart2D_surface_object_to_execute,{ x_pos,y_pos2 }, cart2D_surface_automata_rule_results[i],i);
		}
	}

	// Function to display each individual rule result as a row of ImGui widgets which the user can interact with to define the rule result and display as an 
	// overlay in the main Cartesian 2D ImPlot automata grid display window widget. cart2D_surface_automata_rule_result needs to be referenced or changes will be lost.
	void display_cart2D_automata_rule_result(cart2D_grid_base_class<T> *cart2D_surface_object_to_execute,glm::vec2 window_loc, cart2D_surface_automata_rule_result_struct_type& cart2D_surface_automata_rule_result,int result_index) {
		ImGui::SetCursorPosX(window_loc.x);
		ImGui::SetCursorPosY(window_loc.y);

		// Define individual ImGui identifiers for each Imgui widget so as to avoid
		// identifier clashes and be able to display multiple like widgets 
		std::string w_id = "##hrrcb" + std::to_string(cart2D_surface_automata_rule_result.rule_result_id);
		std::string rn_id = "##hrrrn" + std::to_string(cart2D_surface_automata_rule_result.rule_result_id);
		std::string ss_id_string = "##hrrss" + std::to_string(cart2D_surface_automata_rule_result.rule_result_id);
		std::string es_id_string = "##hrrer" + std::to_string(cart2D_surface_automata_rule_result.rule_result_id);
		std::string sr_id_string = "##hrrsr" + std::to_string(cart2D_surface_automata_rule_result.rule_result_id);
		std::string rv_id_string = "##hrrrv" + std::to_string(cart2D_surface_automata_rule_result.rule_result_id);
		std::string rc_id_string = "##hrrrc" + std::to_string(cart2D_surface_automata_rule_result.rule_result_id);
		std::string rs_id_string = "##hrrrs" + std::to_string(cart2D_surface_automata_rule_result.rule_result_id);
		std::string rss_id_string = "##hrrshs" + std::to_string(cart2D_surface_automata_rule_result.rule_result_id);
		std::string rd_id_string = "##hrrrd" + std::to_string(cart2D_surface_automata_rule_result.rule_result_id);

		ImGui::BeginGroup();
		{
			ImGui::Checkbox(w_id.c_str(), &cart2D_surface_automata_rule_result.active_result);// Set rule result to be used in cellula automata iteration process
			ImGui::SameLine();
			ImGui::SetNextItemWidth(120);
			ImGui::InputText(rn_id.c_str(), &cart2D_surface_automata_rule_result.result_name, ImGuiInputTextFlags_CallbackCharFilter, Input_Filters::name); // Rule result name label
			ImGui::SameLine();
			ImGui::SetNextItemWidth(50);
			ImGui::InputInt(ss_id_string.c_str(), &cart2D_surface_automata_rule_result.rule_start_step, 0); // Iteration start step that rule result will start from
			ImGui::SameLine();
			ImGui::SetNextItemWidth(50);
			ImGui::InputInt(es_id_string.c_str(), &cart2D_surface_automata_rule_result.rule_end_step, 0); // Iteration end step that rule result will end
			ImGui::SameLine();
			ImGui::SetNextItemWidth(75);
			ImGui::InputText(sr_id_string.c_str(), &cart2D_surface_automata_rule_result.cart2D_result_definition, ImGuiInputTextFlags_CallbackCharFilter, rules_input);// defined cellular automata rules that need to be met
			ImGui::SameLine();
			ImGui::SetNextItemWidth(50);
			ImGui::InputText(rv_id_string.c_str(), &cart2D_surface_automata_rule_result.cart2D_result_value, ImGuiInputTextFlags_CallbackCharFilter, Input_Filters::number);// Result value that is assigned to hex grid cell if all automata rules are met
			ImGui::SameLine();
			ImGui::SetNextItemWidth(50);
			// interactively change result display color
			if (ImGui::ColorEdit4(rc_id_string.c_str(), (float*)&cart2D_surface_automata_rule_result.result_display_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_None)) {

if(!cart2D_surface_object_to_execute) printf("cart2D_surface_automata_results_widget_class::cart2D_automata_rule_results_editor : 0000 : !cart2D_surface_object_to_execute \n");
				cart2D_grid_class<T>* cart2D_grid = static_cast<cart2D_grid_class<T>*> (cart2D_surface_object_to_execute);
				if(result_index < cart2D_grid->cart2D_grid_cells_data_list.size() )
					cart2D_grid->cart2D_grid_cells_data_list[result_index].cart2D_grid_cells_display_color = cart2D_surface_automata_rule_result.result_display_color;
			}
			ImGui::SameLine();
			ImGui::SetNextItemWidth(60);
			const char* combo_preview_value = shapes[cart2D_surface_automata_rule_result.display_shape_id];
			if (ImGui::BeginCombo(rs_id_string.c_str(), combo_preview_value, shapes_combo_flags))
			{
				for (int n = 0; n < IM_COUNTOF(shapes); n++)
				{
					const bool is_selected = (cart2D_surface_automata_rule_result.display_shape_id == n);
					if (ImGui::Selectable(shapes[n], is_selected)) {
						cart2D_surface_automata_rule_result.display_shape_id = n;
						cart2D_surface_automata_rule_result.define_imgui_shape();
					}

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected) { 
						ImGui::SetItemDefaultFocus(); }
				}
				ImGui::EndCombo();

if(!cart2D_surface_object_to_execute) printf("cart2D_surface_automata_results_widget_class::cart2D_automata_rule_results_editor : 1111 : !cart2D_surface_object_to_execute \n");
				// interactively change result display shape
				cart2D_grid_class<T>* cart2D_grid = static_cast<cart2D_grid_class<T>*> (cart2D_surface_object_to_execute);
				if (result_index < cart2D_grid->cart2D_grid_cells_data_list.size()) {
					cart2D_grid->cart2D_grid_cells_data_list[result_index].cart2D_grid_cells_display_shape = cart2D_surface_automata_rule_result.result_display_shape;
				}
			}
			ImGui::SameLine();
			ImGui::SetNextItemWidth(60);
			// interactively change result display shape size
			if (ImGui::DragFloat(rss_id_string.c_str(), &cart2D_surface_automata_rule_result.display_shape_size,0.1f, 1.0f, 50.0f, "%2.2f")) {
if(!cart2D_surface_object_to_execute) printf("cart2D_surface_automata_results_widget_class::cart2D_automata_rule_results_editor : 0000 : !cart2D_surface_object_to_execute \n");
				cart2D_grid_class<T>* cart2D_grid = static_cast<cart2D_grid_class<T>*>(cart2D_surface_object_to_execute);
				if (result_index < cart2D_grid->cart2D_grid_cells_data_list.size()) {
					cart2D_grid->cart2D_grid_cells_data_list[result_index].display_shape_size = cart2D_surface_automata_rule_result.display_shape_size;
				}
			}
			ImGui::SameLine();
			// interactively display result data
			if (ImGui::Checkbox(rd_id_string.c_str(), &cart2D_surface_automata_rule_result.display_result)) {
if(!cart2D_surface_object_to_execute) printf("cart2D_surface_automata_results_widget_class::cart2D_automata_rule_results_editor : 0000 : !cart2D_surface_object_to_execute \n");
				cart2D_grid_class<T>* cart2D_grid = static_cast<cart2D_grid_class<T>*> (cart2D_surface_object_to_execute);
				if (result_index < cart2D_grid->cart2D_grid_cells_data_list.size()) {
					cart2D_grid->cart2D_grid_cells_data_list[result_index].display_data = cart2D_surface_automata_rule_result.display_result;
				}
			}
		}
		ImGui::EndGroup();

		if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {//selection of sub rule by hovering over input test entry and right mouse 
//printf("cart2D_surface_automata_results_widget_class :: display_cart2D_automata_rule_result:: %i : %s\n", cart2D_surface_automata_rule_result.rule_result_id, cart2D_surface_automata_rule_result.result_name.c_str());
			selected_rule_result_index = result_index;
		}

	}

	// !!!!!!!!!!! Begin Verify rule results !!!!!!!!!!!!!!!
// Function to verify that all rule sub rules are valid and assigned a parser tree to perform
// cellular automata iteration steps
	bool verify_result_rules(cart2D_grid_base_class<T>* cart2D_surface_object_to_execute) {
		printf("cart2D_surface_automata_results_widget_class :: verify_result_rules 000");
		for (cart2D_surface_automata_rule_result_struct_type rule_result : cart2D_surface_automata_rule_results) {
			if (!verify_result_rule(cart2D_surface_object_to_execute, rule_result)) { return false; }
		}

		for (cart2D_surface_automata_rule_struct_type& rule : cart2D_surface_automata_rules_widget.cart2D_surface_automata_rules) {
			printf("cart2D_surface_automata_results_widget_class :: verify_result_rules 111");
			printf(": rule :: %s\n", rule.rule_name.c_str());

			for (cart2D_automata_sub_rule_struct_type& sub_rule : rule.sub_rules) {
				printf("cart2D_surface_automata_results_widget_class :: verify_result_rules 222");
				printf(": rule "); printf("%s sub rule %s !! \n", rule.rule_name.c_str(), sub_rule.sub_rule_name.c_str());
				parser_base_node* logic_parser_tree = cart2D_surface_automata_rules_widget.valid_sub_rule_definition(sub_rule.sub_rule_definition); // For testing

				if (!logic_parser_tree) {// Sub rule code is invalid and a parser tree could not be constructed : define sub rule parameters for a null parser tree
//logic_parser.display_parse_tree(logic_parser_tree); // For testing only
					afw_globalc::get_current_logger()->log(LogLevel::ERROR, "Have invalid sub rule " + sub_rule.sub_rule_name + " of rule " + rule.rule_name);
					cart2D_surface_automata_rules_widget.logic_parser_tree_defined = false;
					sub_rule.root_parser_node = nullptr;
					return false;
				}

				//have valid parser_tree : define sub rule parameters for valid parser tree 
				sub_rule.edit_sub_rule = false;
				sub_rule.edit_sub_rule_text = false;
				sub_rule.root_parser_node = logic_parser_tree;
				cart2D_surface_automata_rules_widget.logic_parser_tree_defined = true;
				printf("cart2D_surface_automata_results_widget_class::verify_result_rules: Have valid automata sub rule definition accepted for  !!!!\n");
				afw_globalc::get_current_logger()->log(LogLevel::INFO, "cart2D_surface_automata_results_widget_class::verify_result_rules: Have valid automata sub rule definition accepted for " + sub_rule.sub_rule_name + "!!!!\n");
			}
		}

		return true;
	}

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	int get_min_rule_start() {
		int min_rule_start = 0;

		if (!cart2D_surface_automata_rule_results.empty()) {
			min_rule_start = cart2D_surface_automata_rule_results[0].rule_start_step;
		}

		for (cart2D_surface_automata_rule_result_struct_type rule_result : cart2D_surface_automata_rule_results) {
			if (rule_result.rule_start_step < min_rule_start) min_rule_start = rule_result.rule_start_step;
		}

		return min_rule_start;
	}

	int get_max_rule_end() {
		int max_rule_end = 0;

		if (!cart2D_surface_automata_rule_results.empty()) {
			max_rule_end = cart2D_surface_automata_rule_results[0].rule_end_step;
		}

		for (cart2D_surface_automata_rule_result_struct_type rule_result : cart2D_surface_automata_rule_results) {
			if (rule_result.rule_end_step > max_rule_end) { max_rule_end = rule_result.rule_end_step; }
		}

		return max_rule_end;
	}

	// !!!!!!!!!!! PROCESS RULE RESULTS FUNCTIONS !!!!!!!!!!!!!!!

	// Perform a Cartesian 2D cellula automata generation step
	// cart2D_grid_copy must be a reference to retain changes
	bool perform_cart2D_surface_automata_generation(int step,bool ignore_individual_step_intervals, cart2D_grid_class<T> *cart2D_surface_object_to_execute, std::vector<T> &cart2D_grid_copy) {
//printf("cart2D_surface_automata_results_widget_class :: perform_cart2D_surface_automata_generation 0000: %i\n" , step);
		if (!cart2D_surface_object_to_execute) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "cart2D_surface_automata_results_widget_class::perform_cart2D_surface_automata_generation() Have hex grid data not defined\n");
			return false;
		}

		if (!automata_rules_verified) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "cart2D_surface_automata_results_widget_class::perform_cart2D_surface_automata_generation()  automata rules have not been verified!!!!\n Verify Automamata rules before proceeding.\n");
			return false;
		}

		if (cart2D_surface_object_to_execute->cart2D_grid.size() == 0) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "cart2D_surface_automata_results_widget_class::perform_cart2D_surface_automata_generation() Have Hex grid data not initialised\n");
			return false;
		}

//printf("cart2D_surface_automata_results_widget_class :: perform_cart2D_surface_automata_generation 1111:"); printf(" %i : %i : %i\n" , step, cart2D_surface_object_to_execute->cart2D_grid.size(), cart2D_grid_copy.size());
		// Clear or create rule result hex cell data list of all index and hex coordinate data that is to be displayed on screen for active result 
		for (int i = 0; i < cart2D_surface_automata_rule_results.size();i++) {
			cart2D_surface_automata_rule_result_struct_type &rule_result = cart2D_surface_automata_rule_results[i];
//printf("cart2D_surface_automata_results_widget_class :: perform_cart2D_surface_automata_generation 2222:");printf("%i : %s\n" , step, rule_result.result_name.c_str());
				if (rule_result.cart2D_grid_cell_data_id < 0) {// Rule result Hex grid cell data list not defined
//printf("cart2D_surface_automata_results_widget_class :: perform_cart2D_surface_automata_generation 3333:");printf("%i : %i\n" , step, rule_result.cart2D_grid_cell_data_id);
					rule_result.cart2D_grid_cell_data_id = cart2D_surface_object_to_execute->define_new_cart2D_grid_cell_data();
//printf("cart2D_surface_automata_results_widget_class :: perform_cart2D_surface_automata_generation 4444:");printf("%i : %i\n" , step, rule_result.cart2D_grid_cell_data_id);
					if (rule_result.cart2D_grid_cell_data_id < 0) {
						afw_globalc::get_current_logger()->log(LogLevel::INFO, "cart2D_surface_automata_results_widget_class::perform_cart2D_surface_automata_generation() Could not define a hex grid cells results data list\n");
						return false;
					}

if(i>= cart2D_surface_object_to_execute->cart2D_grid_cells_data_list.size()) {
printf("cart2D_surface_automata_results_widget_class::perform_cart2D_surface_automata_generation() 4444AAAA: Error : i>=cart2D_grid_cells_data_list.size()");printf(": %i : %i\n",i, cart2D_surface_object_to_execute->cart2D_grid_cells_data_list.size());//+++++
}

//printf("cart2D_surface_automata_results_widget_class :: perform_cart2D_surface_automata_generation 4444BBBB:"); printf("% i : %i : %i\n", step, rule_result.cart2D_grid_cell_data_id, rule_result.display_shape_id);

					// Define hex grid cell display data
					cart2D_surface_object_to_execute->cart2D_grid_cells_data_list[i].cart2D_grid_cells_display_color = rule_result.result_display_color;
					cart2D_surface_object_to_execute->cart2D_grid_cells_data_list[i].cart2D_grid_cells_display_shape = rule_result.result_display_shape;
					cart2D_surface_object_to_execute->cart2D_grid_cells_data_list[i].display_data = rule_result.display_result;
				}
				else {
//printf("cart2D_surface_automata_results_widget_class :: perform_cart2D_surface_automata_generation 5555:");printf("% i : % i\n" , step, rule_result.cart2D_grid_cell_data_id);
					// Have hex automata rule result hex grid data defined : clear of existing data to be reused.
					cart2D_surface_object_to_execute->clear_cart2D_grid_cell_data(rule_result.cart2D_grid_cell_data_id);
				}
//printf("cart2D_surface_automata_results_widget_class :: perform_cart2D_surface_automata_generation 7777:"); printf(" %i : %i\n" , step, rule_result.cart2D_grid_cell_data_id);
		}

//printf("cart2D_surface_automata_results_widget_class :: perform_cart2D_surface_automata_generation 8888:"); printf(" %i : %i : %i\n" , step, cart2D_surface_object_to_execute->cart2D_grid.size(), cart2D_grid_copy.size());

		// Iterate through each main Cartesian 2D automata grid cell and process through the list of rule results to find a valid rule result that
		// the Cartesian 2D automata grid cell is to be assigned to, and if found assign the hex grid cell value to that of the rule result value
		for (cart2D_surface_index_data_type cart2D_index=0; cart2D_index < cart2D_surface_object_to_execute->cart2D_grid.size(); cart2D_index++) { // Iterate through each hex grid cell
			// !!!!!!!!!!!!!! NOTE : Need to take into account the hex indices that are on the edge or boundaries of the hex grid !!!!!!!!!!!!!!!!!!!!!!!

//printf("cart2D_surface_automata_results_widget_class :: perform_cart2D_surface_automata_generation 9999:"); printf(" %i\n" , step);
			for (size_t rule_result_index = 0; rule_result_index < cart2D_surface_automata_rule_results.size(); rule_result_index ++) { // Iterate through each rule result 
				cart2D_surface_automata_rule_result_struct_type rule_result =  cart2D_surface_automata_rule_results[rule_result_index]; // Assign a rule result data to a temp variable
//printf("cart2D_surface_automata_results_widget_class :: perform_cart2D_surface_automata_generation AAAA:"); printf(" %i : %i :%s \n" , step, rule_result.active_result, rule_result.result_name.c_str());

				if (rule_result.active_result) {// If rule result is active 
					if ((step >= rule_result.rule_start_step  && step <= rule_result.rule_end_step) || ignore_individual_step_intervals) { // perform automata step if within specified step range
//printf("cart2D_surface_automata_results_widget_class :: perform_cart2D_surface_automata_generation BBBB:"); printf(" %i : %i :%s : %s\n" , step, rule_result.active_result, rule_result.result_name.c_str(), rule_result.cart2D_result_definition.c_str());
						std::string rule_definitions = FW::stringtools::trim(rule_result.cart2D_result_definition); // make sure have no spaces in string of rule definition
//printf("cart2D_surface_automata_results_widget_class :: perform_cart2D_surface_automata_generation CCCC111AAAA:"); printf(" %i : %i :%s : |%s| :|%s| : %i\n" , cart2D_index, rule_result.active_result, rule_result.result_name.c_str(), rule_result.cart2D_result_definition.c_str(), rule_definitions.c_str(), rule_definitions.size());
						if (rule_definitions.empty() || rule_definitions[0] == '\0' || rule_definitions[0] == '\n') {// Want to use all rules to define result value
//printf("cart2D_surface_automata_results_widget_class :: perform_cart2D_surface_automata_generation CCCC111BBB:\n");
							if (all_rule_conditions_met(rule_result, cart2D_surface_object_to_execute, cart2D_index)) {
//printf("cart2D_surface_automata_results_widget_class :: perform_cart2D_surface_automata_generation CCCC111CCC:\n");
								assign_rule_result(cart2D_surface_object_to_execute, rule_result, cart2D_grid_copy, cart2D_index);
//printf("cart2D_surface_automata_results_widget_class :: perform_cart2D_surface_automata_generation CCCC111DDD:\n");
								// Add hex grid rule result grid cell to the rules result overlay hex grid for the 
								// result rule of index rule_result_index and hex grid cell of index cart2D_index
								cart2D_surface_object_to_execute->add_cart2D_grid_cell_index(rule_result_index, cart2D_index);
//printf("cart2D_surface_automata_results_widget_class :: perform_cart2D_surface_automata_generation CCCC111EEE:\n");
								rule_result_index = cart2D_surface_automata_rule_results.size();// Once a rule result condition is met, any remaining rule result conditions are skipped and ignored.
							}
						}
						else { // Use specified automata rules
							//printf("cart2D_surface_automata_results_widget_class :: perform_cart2D_surface_automata_generation CCCC222AAA:\n");
							if (all_specified_rule_conditions_met(rule_result, cart2D_surface_object_to_execute, cart2D_index)) {
//printf("cart2D_surface_automata_results_widget_class :: perform_cart2D_surface_automata_generation CCCC222BBB:\n");
								assign_rule_result(cart2D_surface_object_to_execute, rule_result, cart2D_grid_copy, cart2D_index);
//printf("cart2D_surface_automata_results_widget_class :: perform_cart2D_surface_automata_generation CCCC222CCC:\n");
								// Add hex grid rule result grid cell to the rules result overlay hex grid for the 
								// result rule of index rule_result_index and hex grid cell of index cart2D_index
								cart2D_surface_object_to_execute->add_cart2D_grid_cell_index(rule_result_index, cart2D_index);
//printf("cart2D_surface_automata_results_widget_class :: perform_cart2D_surface_automata_generation CCCC222DDD\n");
								rule_result_index = cart2D_surface_automata_rule_results.size();// Once a rule result condition is met, any remaining rule result conditions are skipped and ignored.
							}
						}
					}
				}
			}
		}

		return true;
	}
	
	// Function to verify that all rule result rules conditions have been met or not.
	// rule_result must be a reference to retain changes
	bool all_rule_conditions_met(cart2D_surface_automata_rule_result_struct_type &rule_result, cart2D_grid_base_class<T>* cart2D_surface_object_to_execute, cart2D_surface_index_data_type cart2D_index) {
		if (!cart2D_surface_object_to_execute) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "cart2D_surface_automata_results_widget_class::all_rule_conditions_met : ERROR :  Could not test all rule conditions met. cart2D_surface_object_to_execute undefined \n");
			return false;
		}

		if (cart2D_surface_automata_rules_widget.cart2D_surface_automata_rules.empty()) {
			vwDialogs::display_error_message("Hex Rule", "ERROR : \nno rules defined to evaluate for result rule\n Cannot perform automata.\n");
			return false;
		}
		
		bool have_active_and_valid_rule_met = false;// Flag to indicate if have active and valid rule met

//printf("cart2D_surface_automata_results_widget_class :: all_rule_conditions_met 000000: %i : :%i\n" , cart2D_index, cart2D_surface_automata_rules_widget.cart2D_surface_automata_rules.size());
		// Iterate through each cellula automata rule and test if all cellular automata sub rules for each rule are met or not
		for (cart2D_surface_automata_rule_struct_type rule : cart2D_surface_automata_rules_widget.cart2D_surface_automata_rules) {
//printf("cart2D_surface_automata_results_widget_class :: all_rule_conditions_met 11111: %i : %i :%s : %s\n" , cart2D_index, rule.active_rule, rule_result.result_name.c_str(), rule.rule_name.c_str());
			//if (rule.active_rule) {
				if (!all_sub_rule_conditions_met(rule, cart2D_surface_object_to_execute, cart2D_index)) {

// testing only : delete comment out when not needed
//printf("cart2D_surface_automata_results_widget_class :: all_rule_conditions_met :: NOT all_rule_conditions_met 222222: %i : %s : ", cart2D_index, rule.sub_rules[0].sub_rule_name.c_str());
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
//printf("cart2D_surface_automata_results_widget_class :: all_rule_conditions_met 33333: %s : " , rule.sub_rules[0].sub_rule_name.c_str());
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

	// Function to verify that a specified rule result rules conditions have been met or not.
	// rule_result must be a reference to retain changes
	bool all_specified_rule_conditions_met(cart2D_surface_automata_rule_result_struct_type &rule_result, cart2D_grid_base_class<T>* cart2D_surface_object_to_execute, cart2D_surface_index_data_type cart2D_index) {
		if (!cart2D_surface_object_to_execute) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "cart2D_surface_automata_results_widget_class::all_specified_rule_conditions_met : ERROR :  Could not test all rule conditions met. cart2D_surface_object_to_execute undefined \n");
			return false;
		}

//printf("cart2D_surface_automata_results_widget_class : all_specified_rule_conditions_met 1111\n");
		std::vector<std::string> rule_ids = FW::stringtools::split(rule_result.cart2D_result_definition, ":"); // Assign each rule index id to a string vector array
//printf("cart2D_surface_automata_results_widget_class : all_specified_rule_conditions_met 2222 |%s|\n", rule_result.cart2D_result_definition.c_str());
//printf("cart2D_surface_automata_results_widget_class : all_specified_rule_conditions_met %i\n", rule_ids.size());

		// Will have at least one rule id defined by split function of rule_definitions
		// If no rule id specified then all rules are to be applied so return true
		//if (rule_ids.size() == 1  && (rule_ids[0] == "\n" || rule_ids[0] == "\0" || rule_ids[0].size() == 0)) return true; // This and variations does not work !!!!! Have no idea why
//printf("cart2D_surface_automata_results_widget_class : all_specified_rule_conditions_met 3333 %i\n", rule_ids.size());

		bool have_active_and_valid_rule_met = false;// Flag to indicate if have active and valid rule met

		// Test that all rule id numbers are valid
		for (std::string rule_id : rule_ids) {
//printf("cart2D_surface_automata_results_widget_class : all_specified_rule_conditions_met 4444 |%s|\n", rule_id.c_str());

			int rule_idi = 0 ;
			try { rule_idi = stoi(rule_id); }
			catch (std::invalid_argument const& ex)
			{
				// Will have at least one rule id defined by split function of rule_definitions
				// If no rule id specified then all rules are to be applied so return true
				// Need this because cannot return true for a null entry of rule_definitions to specify to use all automata rules
				if (rule_ids.size() == 1) { return true; }
				//vwDialogs::display_error_message("Hex Rule", "ERROR : \nHave invalid argument for rule id " + acart2D_tools::get_propper_string(rule_id) + " for rule result " + acart2D_tools::get_propper_string(rule_result.result_name) + " that is not defined in the list of rules.\n");
				vwDialogs::display_error_message("Hex Rule", "ERROR : \nHave invalid argument for rule id " + rule_id + " for rule result " + rule_result.result_name + " that is not defined in the list of rules.\n");
				return false;
			}
			catch (std::out_of_range const& ex)
			{
				// Will have at least one rule id defined by split function of rule_definitions
				// If no rule id specified then all rules are to be applied so return true
				// Need this because cannot return true for a null entry of rule_definitions to specify to use all automata rules
				if (rule_ids.size() == 1) { return true; }
				//vwDialogs::display_error_message("Hex Rule", "ERROR : \nHave rule id out of range for rule id " + acart2D_tools::get_propper_string(rule_id) + " for rule result " + acart2D_tools::get_propper_string(rule_result.result_name) + " that is not defined in the list of rules.\n");
				vwDialogs::display_error_message("Hex Rule", "ERROR : \nHave rule id out of range for rule id " + rule_id + " for rule result " + rule_result.result_name + " that is not defined in the list of rules.\n");

				return false;
			}

			if (!valid_rule(rule_idi)) {
				//vwDialogs::display_error_message("Hex Rule", "ERROR : \nHave rule id " + acart2D_tools::get_propper_string(rule_id) + " for rule result " + acart2D_tools::get_propper_string(rule_result.result_name) + " that is not defined in the list of rules.\n");
				vwDialogs::display_error_message("Hex Rule", "ERROR : \nHave rule id " +rule_id + " for rule result " + rule_result.result_name + " that is not defined in the list of rules.\n");
				return false;
			}

			cart2D_surface_automata_rule_struct_type rule = cart2D_surface_automata_rules_widget.cart2D_surface_automata_rules[rule_idi];
			//if (rule.active_rule) {
				if (!all_sub_rule_conditions_met(rule, cart2D_surface_object_to_execute, cart2D_index)) {
					return false;
				}
				have_active_and_valid_rule_met = true;
			//}
		}

		return have_active_and_valid_rule_met;
	}

	// Function to verify that all sub rules conditions have been met or not.
	// rule must be a reference to retain changes
	bool all_sub_rule_conditions_met(cart2D_surface_automata_rule_struct_type &rule, cart2D_grid_base_class<T>* cart2D_surface_object_to_execute, cart2D_surface_index_data_type cart2D_index) {
		//logic_parser_class<T> logic_parser;
		cart2D_logic_parser_class<T> logic_parser;//++++

		if (rule.sub_rules.empty()) {
			//afw_globalc::get_current_logger()->log(LogLevel::ERROR, "cart2D_surface_automata_results_widget_class::all_sub_rule_conditions_met : rule " + rule.rule_name + " has no sub rules !!!!Cannot perform automata. !!!\n");
			//vwDialogs::display_error_message("Hex Rule", "ERROR : \nRule " + acart2D_tools::get_propper_string(rule.rule_name) + " has no sub rules. \nCannot perform automata.\n");
			vwDialogs::display_error_message("Hex Rule", "ERROR : \nRule " + rule.rule_name + " has no sub rules. \nCannot perform automata.\n");
			return false;
		}

		bool have_active_sub_rule_met = false;// Flag to indicate if have active and valid sub rule met. Defaults to false for case of no sub rules present

		// Iterate through each cellula automata sub rule and test if all cellular automata sub rules is met or not
		for (cart2D_automata_sub_rule_struct_type &sub_rule : rule.sub_rules) {
//printf("cart2D_surface_automata_results_widget_class :: all_sub_rule_conditions_met 000000: %i : :%s : %s\n" , cart2D_index, rule.rule_name.c_str(), sub_rule.sub_rule_name.c_str());
			if (sub_rule.active_sub_rule) {
				// Sub rule is active to be used and tested for conditions to be met
//printf("cart2D_surface_automata_results_widget_class :: all_sub_rule_conditions_met 11111: \n");
				if (sub_rule.root_parser_node == nullptr) {
					return false; // Do not have a defined parser tree for sub rule
				}

//printf("cart2D_surface_automata_results_widget_class :: all_sub_rule_conditions_met 22222: \n");
				bool valid_evaluation = logic_parser.evaluate_parse_tree(sub_rule.root_parser_node, cart2D_surface_object_to_execute, cart2D_index);

				if (!valid_evaluation) {
					return false;// Have sub rule that has conditions not met: return false
				}

//printf("cart2D_surface_automata_results_widget_class::all_sub_rule_conditions_met ####### ::");
//switch (sub_rule.root_parser_node->value.first) {
//case cart2D_grid_value_data_type_enum::Boolean:  printf("Boolean:%i\n", sub_rule.root_parser_node->value.second.bvalue); break;
//case cart2D_grid_value_data_type_enum::Float:    printf("Float:  %f\n", sub_rule.root_parser_node->value.second.fvalue); break;
//case cart2D_grid_value_data_type_enum::Integer:  printf("Integer:%i\n", sub_rule.root_parser_node->value.second.ivalue); break;
//case cart2D_grid_value_data_type_enum::Double:   printf("Double: %d\n", sub_rule.root_parser_node->value.second.dvalue); break;
//}
				// Assign sub rule condiitons met to false for root_parser_node value for next iteration of sub rules
				switch (sub_rule.root_parser_node->value.first) {
					case(parse_value_data_type_enum::Boolean): if (!sub_rule.root_parser_node->value.second.bvalue) { sub_rule.sub_rule_conditions_met = false; return false; } break;
					case(parse_value_data_type_enum::Float)  : if (sub_rule.root_parser_node->value.second.fvalue > 0.00000000001f) { sub_rule.sub_rule_conditions_met = false; return false; } break;// This is as a precaution that a zero value in a float may not be stored as a zero value
					case(parse_value_data_type_enum::Integer): if (sub_rule.root_parser_node->value.second.ivalue > 0) { sub_rule.sub_rule_conditions_met = false; return false; } break;
					case(parse_value_data_type_enum::Double) : if (sub_rule.root_parser_node->value.second.dvalue > 0.00000000001f) { sub_rule.sub_rule_conditions_met = false; return false; } break;// This is as a precaution that a zero value in a double may not be stored as a zero value
					//case(parse_value_data_type_enum::nan)    :  {sub_rule.root_parser_node->value.second.ivalue = 0; sub_rule.sub_rule_conditions_met = false; return false; } break;//++++++++
				}

				have_active_sub_rule_met = true;// assign return value to true as have sub rule conditions met for current active sub rule
			}
		}

		return have_active_sub_rule_met;
	}

	// Function to assign a rule result value to the copy of the main Cartesian 2D automata grid (cart2D_grid_copy) cell of given vector array index cart2D_index
	// cart2D_grid_copy must be a reference to retain changes
	void assign_rule_result(cart2D_grid_class<T> *cart2D_surface_object_to_execute,cart2D_surface_automata_rule_result_struct_type rule_result, std::vector<T>& cart2D_grid_copy, cart2D_surface_index_data_type cart2D_index) {
//printf("cart2D_surface_automata_results_widget_class :: assign_rule_result 000\n");

		if (!cart2D_surface_object_to_execute) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "cart2D_surface_automata_results_widget_class::assign_rule_result : ERROR :  Could not assign rule result. cart2D_surface_object_to_execute undefined \n");
			return ;
		}
//printf("cart2D_surface_automata_results_widget_class :: assign_rule_result 111\n");
		//Assign rule result value to Cartesian 2D automata grid cell of vector array index cart2D_index
		// Need a try system to test here ?????
		if (cart2D_index >= 0 && cart2D_index < cart2D_grid_copy.size()) {
			switch (cart2D_surface_object_to_execute->cart2D_grid_value_data_type) {
				case cart2D_grid_value_data_type_enum::Boolean:
				case cart2D_grid_value_data_type_enum::Integer: {
					cart2D_grid_copy[cart2D_index] = stoi(rule_result.cart2D_result_value);
					break;
				}
				case cart2D_grid_value_data_type_enum::Float: {
					cart2D_grid_copy[cart2D_index] = stof(rule_result.cart2D_result_value);
					break;
				}
				case cart2D_grid_value_data_type_enum::Double: {
					cart2D_grid_copy[cart2D_index] = stod(rule_result.cart2D_result_value);
				}
			}
		}
//printf("cart2D_surface_automata_results_widget_class :: assign_rule_result 222\n");
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



	// !!!!!!!!!!!!!!!!!! FUNCTIONS TO DEFINE THAT CELLULA AUTOMATA RULES AND RULE RESULTS ARE VALID OR NOT !!!!!!!!!!!!!!!!!!!!!
	
	// Verify that the rule result is defined within the permited minimum and maximum iteration step interval.
	// If it is return a true value, otherwise a false value.
	bool verify_result_rule(cart2D_grid_base_class<T> *cart2D_surface_object_to_execute,cart2D_surface_automata_rule_result_struct_type rule_result) {
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

		if (!valid_result(cart2D_surface_object_to_execute,rule_result.cart2D_result_value)) {
			vwDialogs::display_error_message("Hex Rule Result", "ERROR : \nHave invalid rule result value that is incompatibe for hex grid data type for rule result : " + rule_result.result_name + "\n");
			return false;
		}

		return true;
	}

	// test that have a valid start iteration step
	bool valid_start_step(int start_step, int end_step) {
		if (start_step < 0 || start_step > end_step) {
			return false;
		} else {
			return true;
		}
	}

	// test that have a valid end iteration step
	bool valid_end_step(int start_step, int end_step) {
		if (end_step < 0 || end_step < start_step){
			return false;
		} else {
			return true;}
	}

	// validate that the nominated rules of the rule result that have to be met exist in the list of available automata rules
	bool valid_rules(cart2D_surface_automata_rule_result_struct_type rule_result) {
		std::string rule_definitions = rule_result.cart2D_result_definition;// variable to store automata rule results for processing

//printf("cart2D_surface_automata_results_widget_class : valid_rules 0000 |%s| %i\n", rule_definitions.c_str(), rule_definitions.size());
		if (rule_definitions == "\0" || rule_definitions == "\n" || rule_definitions == "") { return true; }// Want to use all rules to define result value
//printf("cart2D_surface_automata_results_widget_class : valid_rules 1111\n");

		std::vector<std::string> rule_ids = FW::stringtools::split(rule_definitions, ":"); // define individual automata rule index ids that rule result uses
//printf("cart2D_surface_automata_results_widget_class : valid_rules 2222 |%s|\n",rule_definitions.c_str());
//printf("cart2D_surface_automata_results_widget_class : valid_rules %i\n", rule_ids.size());

		// Will have at least one rule id defined by split function of rule_definitions
		// If no rule id specified then all rules are to be applied so return true
		//if (rule_ids.size() == 1  && (rule_ids[0] == "\n" || rule_ids[0] == "\0" || rule_ids[0].size() == 0)) return true; // This and variations does not work !!!!! Have no idea
//printf("cart2D_surface_automata_results_widget_class : valid_rules 3333 %i\n", rule_ids.size());

		for (std::string rule_id : rule_ids) { // go through list of defined cellula automata rule index ids saved as a string 
//printf("cart2D_surface_automata_results_widget_class : result rule |%s|\n", rule_id.c_str());

			// Make sure that there are no errors in conmverting string to integer number
			int rule_idi;
			try { rule_idi = stoi(rule_id); }
			catch (std::invalid_argument const& ex)
			{
				// Will have at least one rule id defined by split function of rule_definitions
				// If no rule id specified then all rules are to be applied so return true
				// Need this because cannot return true for a null entry of rule_definitions to specify to use all automata rules
				if (rule_ids.size() == 1) { return true; }
				//vwDialogs::display_error_message("Hex Rule", "ERROR : \nHave invalid argument for rule id " + acart2D_tools::get_propper_string(rule_id) + " for rule result " + acart2D_tools::get_propper_string(rule_result.result_name) + " that is not defined in the list of rules\n");
				vwDialogs::display_error_message("Hex Rule", "ERROR : \nHave invalid argument for rule id " + rule_id + " for rule result " + rule_result.result_name + " that is not defined in the list of rules\n");
				return false;
			}
			catch (std::out_of_range const& ex)
			{
				// Will have at least one rule id defined by split function of rule_definitions
				// If no rule id specified then all rules are to be applied so return true
				// Need this because cannot return true for a null entry of rule_definitions to specify to use all automata rules
				if (rule_ids.size() == 1) { return true; }
				afw_globalc::get_current_logger()->log(LogLevel::ERROR, "Have rule id out of range for rule id" + rule_id + " for rule result " +rule_result.result_name + " that is not defined in the list of rules !!!!\n");
				vwDialogs::display_error_message("Hex Rule", "ERROR : \nHave rule id out of range for rule id" + rule_id + " for rule result " + rule_result.result_name + " that is not defined in the list of rules\n");
				return false;
			}

			if (!valid_rule(rule_idi)) {// Test that rule index id is a valid number that is in the index range of the list of defined rules vector array
				vwDialogs::display_error_message("Hex Rule", "ERROR : \nHave rule of id : " + rule_id + " for rule result : " + rule_result.result_name + " that is not defined in the list of rules\n");
				return false;
			}
		}

		return true;
	}

	// Test that a rule index id number exists within the range of the defined rules vector array
	bool valid_rule(int rule_id) {
		if (rule_id >= 0 and rule_id < cart2D_surface_automata_rules_widget.cart2D_surface_automata_rules.size()) {
			return true;
		} else{
			return false;
		}
	}

	// Test that a rule result value defined as a string is a valid number that can be assigned to a hex automata grid cell
	bool valid_result(cart2D_grid_base_class<T> *cart2D_surface_object_to_execute,std::string cart2D_result_value) {
		switch (cart2D_surface_object_to_execute->cart2D_grid_value_data_type) {
			case cart2D_grid_value_data_type_enum::Boolean:
			case cart2D_grid_value_data_type_enum::Integer: {
				if (FW::stringtools::contains(cart2D_result_value,"e") || FW::stringtools::contains(cart2D_result_value,".") || FW::stringtools::contains(cart2D_result_value,"^")) {
					vwDialogs::display_error_message("Hex Rule", "ERROR : \nHave invalid rule result value that is incompatibe for hex grid integer data type\n");
					return false;
				}

				try { stoi(cart2D_result_value);}
				catch (std::invalid_argument const& ex)	{
					return false;
				}
				catch (std::out_of_range const& ex) {
					return false;
				}
				break;
			}
			case cart2D_grid_value_data_type_enum::Float: {
				try { stof(cart2D_result_value); }
				catch (std::invalid_argument const& ex) {
					return false;
				}
				catch (std::out_of_range const& ex) {
					return false;
				}
				break;
			}
			case cart2D_grid_value_data_type_enum::Double: {
				try { stod(cart2D_result_value); }
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
};
