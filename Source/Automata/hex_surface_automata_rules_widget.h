#pragma once

#include <vector>

#include <ImGui/imgui_docking/misc/cpp/imgui_stdlib.h>// Need This so as to be able to input text as a std::string variable

#include <Framework/AFW/Tools/afw_string_tools.h>

#include "../Parser/logic_parser.h"
#include "automata_hex_surface_import_export.h"

/*
		Hexagonal cellula automata rules widget class

	This class widget defines an ImGui widget that is used to define
	a set of cellula automata rules that the hexagonal 2D surface
	grid model can be subjected to perform.
*/

template <class T>
class hex_surface_automata_rules_widget_class {
public:
	// for testing only Delete or comment out when not in use or needed
	//hex_grid_base_class<T>* hex_test_grid = nullptr;

	int current_selected_var_id = -1;

	std::string automata_rule_dir_path = "./Rules/Rules";
	std::string automata_sub_rule_dir_path = "./Rules/Subrules";

	std::vector<hex_surface_automata_rule_struct_type> hex_surface_automata_rules;

	void add_rule(hex_surface_automata_rule_struct_type rule) {
		if (rule.rule_id < 0) {
			rule_id += 1;
			rule.rule_id = rule_id;
		}
		hex_surface_automata_rules.push_back(rule);
	}

	void add_new_automata_rule() {
		hex_surface_automata_rule_struct_type automata_rule;
		add_rule(automata_rule);
	}

	bool delete_rule(int rule_id) {
		for (size_t i = 0; i < hex_surface_automata_rules.size(); i++) {
			if (hex_surface_automata_rules[i].rule_id == rule_id) {
				return delete_automata_rule(hex_surface_automata_rules[i].rule_id);
			}
		}
		return false;
	}

	bool delete_automata_rule(int rule_selected_index) {
		if (rule_selected_index > -1 && rule_selected_index < hex_surface_automata_rules.size()) {
			delete_all_sub_rules(hex_surface_automata_rules[rule_selected_index].sub_rules);
			hex_surface_automata_rules.erase(hex_surface_automata_rules.begin() + rule_selected_index);
			return true;
		}

		return false;
	}

	hex_surface_automata_rule_struct_type* get_rule(int rule_id) {
		for (hex_surface_automata_rule_struct_type rule : hex_surface_automata_rules) {
			if (rule.rule_id == rule_id) {
				return &rule;
				break;
			}
		}

		return nullptr;
	}

	bool modify_rule(hex_surface_automata_rule_struct_type sr) {
		for (hex_surface_automata_rule_struct_type rule : hex_surface_automata_rules) {
			if (rule.rule_id == sr.rule_id) {
				rule = sr;
				return true;
				break;
			}
		}

		return false;
	}

	void save_automata_rule(int rule_selected_index) {
		char const* patterns[] = { "*HGR.txt" };
		//char const* file_pathname = vwDialogs::save_file(nullptr, patterns, 1);
		char const* file_pathname = vwDialogs::save_file(automata_rule_dir_path.c_str(), patterns, 1);

		if (file_pathname == nullptr) {
			vwDialogs::display_error_message("Save Automata Rule", "ERROR : \n No automata rule file defined to save hex surface automata rule data to\nSave automata rule aborted");
			return;
		}

		if (rule_selected_index < 0 || rule_selected_index > hex_surface_automata_rules.size()) {
			return;
		}

		import_export_hex_surface_automata_rules.save_automata_rule(file_pathname, hex_surface_automata_rules[rule_selected_index]);
	}

	void import_automata_rule() {
		char const* patterns[] = { "*HGR.txt" };
		//char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);
		char const* file_pathname = vwDialogs::open_file(automata_rule_dir_path.c_str(), patterns, 1);

		if (file_pathname == nullptr) {
			vwDialogs::display_error_message("Import Automata Rule", "ERROR : \n No automata rule file defined to import hex surface automata rule data to\nImport automata rule aborted");
			return;
		}
		
		if (import_export_hex_surface_automata_rules.import_automata_rule(file_pathname, hex_surface_automata_rules)) {
			rule_id += 1; // define rule id number for imported rule
			hex_surface_automata_rule_struct_type& rule = hex_surface_automata_rules.back();

			rule.rule_id = rule_id;// imported file is last hex_surface_automata_rules vector element

			// Need to edit sub rule id number manually here to enable ImGui to display widgets without error and to initialise an ID idetification for each sub rule.
			for (hex_automata_sub_rule_struct_type &sub_rule : rule.sub_rules) {
//std::cout << "hex_surface_automata_widget_class::load_automata_rules iiiiiA : " << rule.sub_rules.size() << "::" << sub_rule.sub_rule_id << std::endl;
				rule.sub_rule_id += 1;
				sub_rule.sub_rule_id = rule.sub_rule_id;
//std::cout << "hex_surface_automata_widget_class::load_automata_rules iiiiiB : " << rule.sub_rules.size() << "::" << sub_rule.sub_rule_id << std::endl;
			}
		}
	}

	void add_new_automata_sub_rule(hex_surface_automata_rule_struct_type &hex_surface_automata_rule) {
		hex_automata_sub_rule_struct_type sub_rule;

		hex_surface_automata_rule.add_sub_rule(sub_rule);
	}

	void import_automata_sub_rule(hex_surface_automata_rule_struct_type& hex_surface_automata_rule) {
		char const* patterns[] = { "*HGSR.txt" };
		//char const* file_pathname = vwDialogs::open_file(nullptr, patterns, 1);
		char const* file_pathname = vwDialogs::open_file(automata_sub_rule_dir_path.c_str(), patterns, 1);

		if (file_pathname == nullptr) {
			vwDialogs::display_error_message("Import Automata Sub Rule", "ERROR : \n No automata sub rule file defined to import hex surface automata sub rule data to\nImport automata sub rule aborted");
			return;
		}

		import_export_hex_surface_automata_rules.import_automata_sub_rule(file_pathname,hex_surface_automata_rule);
	}

	void save_rule_sub_rule(hex_surface_automata_rule_struct_type rule, int sub_rule_index) {
		char const* patterns[] = { "*HGSR.txt" };
		//char const* file_pathname = vwDialogs::save_file(nullptr, patterns, 1);
		char const* file_pathname = vwDialogs::save_file(automata_sub_rule_dir_path.c_str(), patterns, 1);

		if (file_pathname == nullptr) {
			vwDialogs::display_error_message("Save Automata Sub Rule", "ERROR : \n No automata sub rule file defined to save hex surface automata sub rule data to\nSave automata sub rule aborted");
			return;
		}

		if (sub_rule_index < 0 || sub_rule_index > rule.sub_rules.size()) {
			return;
		}

		import_export_hex_surface_automata_rules.save_automata_sub_rule(file_pathname, rule.sub_rules[sub_rule_index]);
	}

	bool delete_rule_sub_rule(hex_surface_automata_rule_struct_type &rule, int sub_rule_index) {
		if (sub_rule_index < 0 || sub_rule_index > rule.sub_rules.size()) { return false; }

		logic_parser.delete_pars_tree(rule.sub_rules[sub_rule_index].root_parser_node);
		rule.sub_rules.erase(rule.sub_rules.begin() + sub_rule_index);
		return true;
	}

	void delete_all_sub_rules(std::vector<hex_automata_sub_rule_struct_type> &sub_rules) {
		for (int i = sub_rules.size()-1; i > -1 ; i--) {
			logic_parser.delete_pars_tree(sub_rules[i].root_parser_node);
			sub_rules.erase(sub_rules.begin() + i);
		}
	}

	void delete_all_rules() {
		for (int i = hex_surface_automata_rules.size() - 1; i > -1; i--) {
			delete_automata_rule(i);
		}

		hex_surface_automata_rules.clear();
		hex_surface_automata_rules.shrink_to_fit();
		rule_id = -1;// +++++!!!!!
	}

	void hex_automata_rules_editor(ImVec2 editor_display_window_size) {
		ImGui::BeginChild("Automata Rules", editor_display_window_size, true);

		if (ImGui::BeginPopupContextWindow(0, 1)) {
			ImVec2 click_pos = ImGui::GetMousePosOnOpeningCurrentPopup();

			if (ImGui::BeginMenu("Automata Rules")) {
				if (ImGui::MenuItem("Add Rule")) {
					add_new_automata_rule();
				}

				if (ImGui::MenuItem("Import Rule")) {
					import_automata_rule();
				}

				if (rule_selected_index > -1) {

					if (ImGui::MenuItem("Delete Rule")) {
//printf("hex_surface_automata_rules_widget_class :: hex_automata_rules_editor:: %i : %s\n", hex_surface_automata_rules[rule_selected_index].rule_id, hex_surface_automata_rules[rule_selected_index].rule_name.c_str());
						delete_automata_rule(rule_selected_index);
						rule_selected_index = -1;
					}

					if (ImGui::MenuItem("Save Rule")) {
//printf("hex_surface_automata_rules_widget_class :: hex_automata_rules_editor:: %i : %s\n", hex_surface_automata_rules[rule_selected_index].rule_id, hex_surface_automata_rules[rule_selected_index].rule_name.c_str());
						save_automata_rule(rule_selected_index);
						rule_selected_index = -1;
					}
				}
				ImGui::EndMenu();
			}
			ImGui::EndPopup();
		}

			if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {//selection of sub rule by hovering over input test entry and right mouse 
//printf("hex_surface_automata_rules_widget_class :: hex_automata_rules_editor:: LEFT\n");
				rule_selected_index = -1;
			}

		display_hex_automata_rules();

		ImGui::EndChild();
	}

	void display_hex_automata_rules() {
		float x_pos = 5.0f, y_pos = 5.0f;

		float t_height = ImGui::GetTextLineHeight();
//printf("hex_surface_automata_rules_widget_class :: display_hex_automata_rules:: 00000 :%i\n", hex_surface_automata_rules.size());
		for (size_t i = 0; i < hex_surface_automata_rules.size(); i++) {
			float y_pos2 = y_pos + (t_height * 2) * i;
			display_hex_automata_rule({ x_pos,y_pos2 },i, hex_surface_automata_rules[i]);


			if (hex_surface_automata_rules[i].display_sub_rules) {
//printf("hex_surface_automata_rules_widget_class :: display_hex_automata_rules:: AAAAA \n");

				display_hex_automata_sub_rules_widget(hex_surface_automata_rules[i]);
			}
		}
	}

	void display_hex_automata_rule(glm::vec2 window_loc,size_t rule_index, hex_surface_automata_rule_struct_type &hex_surface_automata_rule) {
		ImGui::SetNextItemWidth(50);
		text(std::to_string(rule_index), window_loc.x, window_loc.y + 5);

		ImGui::SetCursorPosX(window_loc.x+50);
		ImGui::SetCursorPosY(window_loc.y);

		std::string w_id = "##hrcb" + std::to_string(hex_surface_automata_rule.rule_id);
		std::string rn_id = "##hrrn" + std::to_string(hex_surface_automata_rule.rule_id);
		std::string ss_id_string = "##hrss" + std::to_string(hex_surface_automata_rule.rule_id);
		std::string es_id_string = "##hrer" + std::to_string(hex_surface_automata_rule.rule_id);
		std::string sr_id_string = "EDIT RULES##hrsr" + std::to_string(hex_surface_automata_rule.rule_id);

		ImGui::BeginGroup();
		{
			//ImGui::Checkbox(w_id.c_str(), &hex_surface_automata_rule.active_rule);
			//ImGui::SameLine();
			ImGui::SetNextItemWidth(120);
			ImGui::InputText(rn_id.c_str(), &hex_surface_automata_rule.rule_name, ImGuiInputTextFlags_CallbackCharFilter, Input_Filters::name);
			ImGui::SameLine();
			//ImGui::SetNextItemWidth(50);
			//ImGui::InputInt(ss_id_string.c_str(), &hex_surface_automata_rule.rule_start_step, 0);
			//ImGui::SameLine();
			//ImGui::SetNextItemWidth(50);
			//ImGui::InputInt(es_id_string.c_str(), &hex_surface_automata_rule.rule_end_step, 0);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(100);

			if (ImGui::Button(sr_id_string.c_str(), { 80,20 })) {
				hex_surface_automata_rule.display_sub_rules = true;
//printf("hex_surface_automata_rules_widget_class :: display_hex_automata_rule:: BBBB \n");

			}
		}
		ImGui::EndGroup();

		if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {//selection of sub rule by hovering over input test entry and right mouse 
//printf("hex_surface_automata_rules_widget_class :: display_hex_automata_rule:: %i : %s\n", hex_surface_automata_rule.rule_id, hex_surface_automata_rule.rule_name.c_str());
			rule_selected_index = rule_index;
		} 

	}

	void display_hex_automata_sub_rules_widget(hex_surface_automata_rule_struct_type &hex_surface_automata_rule) {
		std::string sr_id = "Automata Sub Rules : " + hex_surface_automata_rule.rule_name + "##asr" + std::to_string(hex_surface_automata_rule.rule_id);
		std::string sr_id2 = "##asrc" + std::to_string(hex_surface_automata_rule.rule_id);

//printf("hex_surface_automata_rules_widget_class :: display_hex_automata_sub_rules:: 0000 \n");

		if (ImGui::Begin(sr_id.c_str())) {
			std::string sra_id_string = "ACCEPT##hrsr" + std::to_string(hex_surface_automata_rule.rule_id);
			std::string srq_id_string = "QUIT##hrsr" + std::to_string(hex_surface_automata_rule.rule_id);

			float x_pos = 10.0f, y_pos = 60.0f;

			// Always center this window when appearing
			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

			ImVec2 click_pos = ImGui::GetMousePosOnOpeningCurrentPopup();

			title("Hex Surface Automata");
			text("Automata Sub Rules", x_pos, y_pos);

			ImGui::BeginChild(sr_id2.c_str(), ImVec2(370, 200.0f), true);

//printf("hex_surface_automata_rules_widget_class :: display_hex_automata_sub_rules:: 1111 \n");
			if (ImGui::BeginPopupContextWindow(0, 1)) {
				ImVec2 click_pos = ImGui::GetMousePosOnOpeningCurrentPopup();

				if (ImGui::BeginMenu("Automata Sub Rules")) {
					if (ImGui::MenuItem("Add Sub Rule")) {
						add_new_automata_sub_rule(hex_surface_automata_rule);
					}

					if (ImGui::MenuItem("Import Sub Rule")) {
						import_automata_sub_rule(hex_surface_automata_rule);
					}

					if (sub_rule_selected_index > -1) {

						if (ImGui::MenuItem("Remove Sub Rule")) {
//printf("hex_surface_automata_rules_widget_class :: display_hex_automata_sub_rules_widget:: sub rule to delete : %i : %s \n", sub_rule_selected_index,hex_surface_automata_rule.sub_rules[sub_rule_selected_index].sub_rule_name.c_str());
								delete_rule_sub_rule(hex_surface_automata_rule, sub_rule_selected_index);
								sub_rule_selected_index = -1;
						}

						if (ImGui::MenuItem("Save Sub Rule")) {
//printf("hex_surface_automata_rules_widget_class :: display_hex_automata_sub_rules_widget:: sub rule to delete : %i : %s \n", sub_rule_selected_index,hex_surface_automata_rule.sub_rules[sub_rule_selected_index].sub_rule_name.c_str());
								save_rule_sub_rule(hex_surface_automata_rule, sub_rule_selected_index);
								sub_rule_selected_index = -1;
						}
// Not sure if the following should be implemented as it may cause too many problems
// Left here just in case it will be implemented
/*
						if (ImGui::BeginMenu("Move Sub Rule")) {
							if (ImGui::MenuItem("Up##msru")) {
								//create_node<hcp_voxel_node_class>(click_pos); // uncomment when hcp_voxel implemented
								sub_rule_selected_index = -1;
							}

							if (ImGui::MenuItem("Down##sard")) {
								//create_node<hcp_voxel_node_class>(click_pos); // uncomment when hcp_voxel implemented
								sub_rule_selected_index = -1;
							}

							if (ImGui::MenuItem("To Top##sart")) {
								//create_node<hcp_voxel_node_class>(click_pos); // uncomment when hcp_voxel implemented
								sub_rule_selected_index = -1;
							}

							if (ImGui::MenuItem("To Bottom##sarb")) {
								//create_node<hcp_voxel_node_class>(click_pos); // uncomment when hcp_voxel implemented
								sub_rule_selected_index = -1;
							}

							ImGui::EndMenu();
						}
*/
					}
					ImGui::EndMenu();
				}
				ImGui::EndPopup();
			}

			if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {//selection of sub rule by hovering over input test entry and right mouse 
//printf("hex_surface_automata_rules_widget_class :: hex_automata_rules_editor:: LEFT\n");
				sub_rule_selected_index = -1;
			}

			display_hex_automata_sub_rules(hex_surface_automata_rule);

			ImGui::EndChild();

			ImGui::SetCursorPosX(x_pos+40);

			if (ImGui::Button(sra_id_string.c_str(), { 80,20 })) {
				// Copy sub rules to hex_surface_automata_rule subrules;
				hex_surface_automata_rule.display_sub_rules = false;
//printf("hex_surface_automata_rules_widget_class :: display_hex_automata_sub_rules_widget:: B accept \n");
			}

			ImGui::SameLine();
			ImGui::SetCursorPosX(x_pos + 220);
			if (ImGui::Button(srq_id_string.c_str(), { 80,20 })) {
				hex_surface_automata_rule.display_sub_rules = false;
//printf("hex_surface_automata_rules_widget_class :: display_hex_automata_sub_rules_widget:: B quit \n");
			}
		}
		ImGui::End();

		
	}

	void display_hex_automata_sub_rules(hex_surface_automata_rule_struct_type &hex_surface_automata_rule) {
		float x_pos = 5.0f, y_pos = 5.0f;
		float t_height = ImGui::GetTextLineHeight();

		for (size_t i = 0; i < hex_surface_automata_rule.sub_rules.size(); i++) {
			float y_pos2 = y_pos + (t_height * 2) * i;
			display_hex_automata_sub_rule({ x_pos,y_pos2 }, hex_surface_automata_rule.sub_rules[i],i);
		}
//printf("hex_surface_automata_rules_widget_class :: display_hex_automata_sub_rules:: %i \n", selected);
	}

	void display_hex_automata_sub_rule(glm::vec2 window_loc, hex_automata_sub_rule_struct_type &hex_surface_automata_sub_rule, int sub_rule_index) {
		ImGui::SetCursorPosX(window_loc.x);
		ImGui::SetCursorPosY(window_loc.y);

		std::string w_id = "##hsrcb" + std::to_string(hex_surface_automata_sub_rule.sub_rule_id);
		std::string rn_id = "##hsrrn" + std::to_string(hex_surface_automata_sub_rule.sub_rule_id);
		std::string sr_id_string = "EDIT SUB RULE##hsrsr" + std::to_string(hex_surface_automata_sub_rule.sub_rule_id);

		ImGui::BeginGroup();
		{
			ImGui::Checkbox(w_id.c_str(), &hex_surface_automata_sub_rule.active_sub_rule);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(120);
			ImGui::InputText(rn_id.c_str(), &hex_surface_automata_sub_rule.sub_rule_name, ImGuiInputTextFlags_CallbackCharFilter, Input_Filters::name);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(250);
			if (ImGui::Button(sr_id_string.c_str(), { 120,20 }) && hex_surface_automata_sub_rule.edit_sub_rule == false) {
				hex_surface_automata_sub_rule.edit_sub_rule = true;
//printf("hex_surface_automata_rules_widget_class :: Edit_hex_automata_sub_rule:: BBBB %i\n", hex_surface_automata_sub_rule.sub_rule_id);
			}

			if (hex_surface_automata_sub_rule.edit_sub_rule) {
				edit_hex_surface_automata_sub_rule(hex_surface_automata_sub_rule);
			}
		}
		ImGui::EndGroup();

		if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {//selection of sub rule by hovering over input test entry and right mouse 
//printf("hex_surface_automata_rules_widget_class :: Edit_hex_automata_sub_rule:: BBBB %i : %s\n", hex_surface_automata_sub_rule.sub_rule_id, hex_surface_automata_sub_rule.sub_rule_name.c_str());
			sub_rule_selected_index = sub_rule_index;
		}

	}

	void edit_hex_surface_automata_sub_rule(hex_automata_sub_rule_struct_type& hex_surface_automata_sub_rule) {
		std::string esr_id = "##ehsrw" + std::to_string(hex_surface_automata_sub_rule.sub_rule_id);
		std::string esre_id = "##ehsrte" + std::to_string(hex_surface_automata_sub_rule.sub_rule_id);
		std::string esrb0_id = "Verify Code##ehsrvb" + std::to_string(hex_surface_automata_sub_rule.sub_rule_id);
		std::string esrb1_id = "Accept##ehsrab" + std::to_string(hex_surface_automata_sub_rule.sub_rule_id);
		std::string esrb2_id = "Reject##ehsreb" + std::to_string(hex_surface_automata_sub_rule.sub_rule_id);

		std::string title = "Edit Hex Surface Automata Sub Rule " + hex_surface_automata_sub_rule.sub_rule_name;

		ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;

		static std::string sub_rule_text;
		if (!hex_surface_automata_sub_rule.edit_sub_rule_text) {
			sub_rule_text = hex_surface_automata_sub_rule.sub_rule_definition;
			hex_surface_automata_sub_rule.edit_sub_rule_text = true;
		}

		std::string popup_id = "Hex Surface Automata Sub Rule " + hex_surface_automata_sub_rule.sub_rule_name;
		ImGui::OpenPopup(popup_id.c_str());

		if (ImGui::BeginPopupModal(popup_id.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			float x_pos = 10.0f;
			ImGui::SetCursorPosX(x_pos);
			ImGui::SetNextItemWidth(300);
			ImGui::InputTextMultiline(esre_id.c_str(), &sub_rule_text ,ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), flags);
//			if (ImGui::InputTextMultiline(esre_id.c_str(), &sub_rule_text, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), flags)) {
//printf("test input multiline string text %i : |%s|\n", sub_rule_text.size(), sub_rule_text.c_str());
//			}

			ImGui::SetCursorPosX(x_pos + 100);
			if (ImGui::Button(esrb0_id.c_str(), { 100,20 })) {
printf("hex_surface_automata_rules_widget_class :: display_hex_automata_sub_rules:: B verify code"); printf(" %s\n",sub_rule_text.c_str());
				if (valid_sub_rule_definition(sub_rule_text)) {
					vwDialogs::display_error_message("Automata sub rule", "INFO : \n Automata sub rule is valid\n");
					afw_globalc::get_current_logger()->log(LogLevel::INFO, "Have valid automata sub rule definition for " + hex_surface_automata_sub_rule.sub_rule_name + "!!!!\n");
				} else {
					vwDialogs::display_error_message("Automata sub rule", "INFO : \n Automata sub rule is invalid\n");
					afw_globalc::get_current_logger()->log(LogLevel::INFO, "Have invalid automata sub rule definition for " + hex_surface_automata_sub_rule.sub_rule_name + "!!!!\n");
				}
			}

			ImGui::SetCursorPosX(x_pos);
			if (ImGui::Button(esrb1_id.c_str(), { 80,20 })) {
printf("hex_surface_automata_rules_widget_class :: display_hex_automata_sub_rules:: B accept code  0000:\n");
				logic_parser_tree = valid_sub_rule_definition(sub_rule_text);
				if (logic_parser_tree) {
					// if have valid code 
					hex_surface_automata_sub_rule.edit_sub_rule      = false;
					hex_surface_automata_sub_rule.edit_sub_rule_text = false;
					hex_surface_automata_sub_rule.root_parser_node = logic_parser_tree;
					logic_parser_tree_defined = true;
					hex_surface_automata_sub_rule.sub_rule_definition = sub_rule_text.c_str();
					vwDialogs::display_error_message("Automata sub rule", "INFO : \n Valid automata sub rule definition accepted\n");
					afw_globalc::get_current_logger()->log(LogLevel::INFO, "Have valid automata sub rule definition accepted for " + hex_surface_automata_sub_rule.sub_rule_name + "!!!!\n");
				} else {
					logic_parser_tree_defined = false;
					hex_surface_automata_sub_rule.root_parser_node = nullptr;
					vwDialogs::display_error_message("Automata sub rule", "INFO : \n Invalid automata sub rule definition not accepted\n");
					afw_globalc::get_current_logger()->log(LogLevel::INFO, "Have invalid automata sub rule definition not accepted for " + hex_surface_automata_sub_rule.sub_rule_name + "!!!!\n");
				}

printf("hex_surface_automata_rules_widget_class :: display_hex_automata_sub_rules:: B accept code  :"); printf(" %i : %s \n", hex_surface_automata_sub_rule.sub_rule_definition.size(), hex_surface_automata_sub_rule.sub_rule_definition.c_str());
			}

			ImGui::SameLine();
			ImGui::SetCursorPosX(x_pos + 220);
			if (ImGui::Button(esrb2_id.c_str(), { 80,20 })) {
				hex_surface_automata_sub_rule.edit_sub_rule = false;
				hex_surface_automata_sub_rule.edit_sub_rule_text = false;
printf("hex_surface_automata_rules_widget_class :: display_hex_automata_sub_rules:: B reject  code\n");
			}

			ImGui::EndPopup();
		}
	}

	parser_base_node  *valid_sub_rule_definition(std::string &sub_rule_text) {
printf("hex_surface_automata_rules_widget_class :: valid_sub_rule_definition:: B accept code  0000:"); printf("|%s|\n", sub_rule_text.c_str());

		//Test if have a new line character at the end of the rule definition string, and if not append at end of string
		if (sub_rule_text.find_first_of('\n') == sub_rule_text.npos || sub_rule_text.find_last_of('\n') < sub_rule_text.size()) {
			sub_rule_text.append("\n");
		} 

printf("hex_surface_automata_rules_widget_class :: valid_sub_rule_definition:: B accept code  3333:"); printf(" |%s| \n", sub_rule_text.c_str());
		return logic_parser.create_logic_parse_tree(sub_rule_text);
	}

	ImVec2 editor_sub_rules_display_window_size = { 700,400 };

	size_t max_sub_rule_char = 1000;


	logic_parser_class<T> logic_parser;
	parser_base_node  *logic_parser_tree = nullptr;
	bool logic_parser_tree_defined = false;



private:
	int rule_id = -1;

	int rule_selected_index = -1;
	int sub_rule_selected_index = -1;

	import_export_hex_surface_automata_rules_class import_export_hex_surface_automata_rules;
};