#pragma once

#include <vector>
#include <algorithm>
#include <chrono>

#include <FrameWork/AFW/Tools/afw_dialogs.h> // PLace this here and not needed to be defined in following included files

#include "Source/Cart2D/Cart2D_grids/cart2D_grid.h"

#include "automata_cart2D_surface_import_export.h"
#include "Source/Cart2D/Cart2D_grids/cart2D_surface_import_export.h"

#include "cart2D_surface_automata_rules_widget.h"
#include "cart2D_surface_automata_results_widget.h"

//#define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V)) // move this to another location ????

/*
		Cartesian 2D cellula automata rules widget class

	This class widget defines a ImGui widget and child widgets
	that are used to define one or more sets of cellula automata
	rules that the Cartesian 2D surface grid model can be subjected
	to perform.

	Also this widget class has controls to perform the cellula
	automata rules one incremental step at a time according to the
	defined cellula automata rules that are defined here.
*/

template <class T>
class cart2D_surface_automata_widget_class {
public:

	std::string automata_rules_dir_path = ".//Rules/Automata/Cart2D_A";// Default path name to the directory where the cellular automata rules data files are stored
	std::string automata_grids_dir_path = ".//Grids/Cart2D_G";		  // Default path name to the directory where the cellular automata grid data files are stored

	// The main ImGui window widget function to display widgets and to manage the user interaction with them to
	// create, edit, save/load the hexagaonal cellular automata rules and manage the display and functioning
	// of these rules that act on the main Cartesian 2D automata grid and its display .  

	void display(cart2D_grid_class<T> *cart2D_surface_object_to_execute) {
		if (ImGui::Begin("Hex Surface Automata Rules")) {
			float x_pos = 10.0f, y_pos = 35.0f;

			title("Hex Surface Automata");

			y_pos += 15;

			//##### Section to display and manage the list of hexagaonal rules ######
			//      Hexagonal rules define the individual rules that make up 
			//      the cellular automata model.

			text("Automata Rules", x_pos + 235, y_pos);
			y_pos += 15;
			text("Rule           Name", x_pos, y_pos);

			// Widget class that displays an ImGui editor widget to display and manage the Cartesian 2D automata rules 
			cart2D_surface_automata_results_widget.cart2D_surface_automata_rules_widget.cart2D_automata_rules_editor(editor_display_window_size);

			y_pos += 225;

			if (ex_button("Clear All Automata Rules###caacrb", x_pos + 230, y_pos, 175, 20)) {
				clear_automata_rules();
			}

			// #####################################################################

			//##### Section to display and manage the list of hexagaonal rule results ######
			//      The Cartesian 2D rule results defines the assignment of values to the
			//		main hexagaonal automata grid based upon a selection of automata rules

			y_pos += 25;
			text("Automata Rule Results", x_pos + 245, y_pos);
			y_pos += 25;
			text("Active   Name          Begin    End      Rules     Result Color  Shape    Size  Display", x_pos, y_pos);

if(!cart2D_surface_object_to_execute) printf("cart2D_surface_automata_widget_class::display : 0000 : !cart2D_surface_object_to_execute \n");

			// Widget class that displays an ImGui editor widget to display and manage the Cartesian 2D automata rules results
			cart2D_surface_automata_results_widget.cart2D_automata_rule_results_editor(cart2D_surface_object_to_execute,editor_display_window_size);

			y_pos += 225;
			x_pos += 105;

			if (ex_button("Clear All Results###caucrrb", x_pos + 130, y_pos, 155, 20)) {
				clear_automata_rule_results();
			}

			// ######################################################################
			// ##### ImGui buttons to manage the import/export and clearing of the main Cartesian 2D automata grid #####

			y_pos += 25;
			if (ex_button("Import Grid###iagb", x_pos - 30, y_pos, 155, 20)) {
				import_automata_grid(cart2D_surface_object_to_execute);
			}

			if (ex_button("Clear Grid###gaucrrb", x_pos + 130, y_pos, 155, 20)) {
				clear_automata_grid(cart2D_surface_object_to_execute);
			}

			if (ex_button("Export Grid###eagb", x_pos + 290, y_pos, 155, 20)) {
				export_automata_grid(cart2D_surface_object_to_execute);
			}

			// ######################################################################
			// ##### ImGui widget to validate the cellular automata rules #####
			y_pos += 25;

			if (ex_button("Validate Rule Results###vaucrrb", x_pos + 130, y_pos, 155, 20)) {
				validate_automata_rules_results(cart2D_surface_object_to_execute);
			}

			// ######################################################################
			// ##### ImGui widgets to manage the execution of the cellular automata rules #####
			y_pos += 25;

			ImGui::SetCursorPosX(x_pos + 90);
			ImGui::Text("No Individual Step Contraints : ");
			ImGui::SameLine();
			ImGui::Checkbox("##hgsc", &ignore_individual_step_intervals);
			y_pos += 25;

			float x_pos2 = x_pos + 105;
			x_pos2 += 18;
			if (ex_button(ICON_FA_STOP "###hausb", x_pos2, y_pos, 40, 23)) {
				stop_automata_execution();
			}

			if (ex_button(ICON_FA_PAUSE "###haupab", x_pos2 + 43, y_pos, 40, 23)) {
				pause_automata_execution();
			}

			// No recording of automata execution to be implemented 
			//if (ex_button(ICON_FA_CIRCLE "###haurb", x_pos2 + 86, y_pos, 40, 23))//ICON_FA_CIRCLE - ICON_FA_RECORD_VINAL
			//	record_automata_execution();

			x_pos2 -= 43;
			if (ex_button(ICON_FA_PLAY "###hauplb", x_pos2 + 129, y_pos, 40, 23)) {
				play_automata_execution();
			}

			if (ex_button(ICON_FA_STEP_FORWARD "###hausfb", x_pos2 + 171, y_pos, 40, 23)) {
				next_step_automata_execution(cart2D_surface_object_to_execute);
			}

			ImGui::SetCursorPosX(240);
			ImGui::SetNextItemWidth(165);
			//if (ImGui::SliderFloat("Step execution delay (seconds)##hgpsf", &play_step_execution_slider_delay_time_seconds,0.1f,10.0f,"%2.3f", ImGuiSliderFlags_None)) {
			if (ImGui::DragFloat("Step execution delay (seconds)##hgpsf", &play_step_execution_slider_delay_time_seconds, 0.1f, 0.1f,10.0f,"%2.1f", ImGuiSliderFlags_None)) {
				play_step_execution_delay_time = int(play_step_execution_slider_delay_time_seconds * 1000.0f);
			}

			y_pos += 50;

			// Typically we would use ImVec2(-1.0f,0.0f) or ImVec2(-FLT_MIN,0.0f) to use all available width,
			// or ImVec2(width,0.0f) for a specified width. ImVec2(0.0f,0.0f) uses ItemWidth.

			// Display a progress bar to indicate progross of cellular automata execution
			float progress_saturated = IM_CLAMP(progress, 0.0f, 1.0f);
			std::string sbuf = std::to_string((int)(progress_saturated * number_automata_steps)) + "/" + std::to_string(number_automata_steps);

			ImGui::SetCursorPosX(x_pos + 130);
			ImGui::SetCursorPosY(y_pos);
			ImGui::ProgressBar(progress, ImVec2(160.f, 0.f),sbuf.c_str());


			// ######################################################################
			// ##### ImGui widgets to save/load and clear all of the cellular automata rules #####
			y_pos += 30;

			if (ex_button(ICON_FA_UPLOAD " Save Rules###hausrb", x_pos + 20, y_pos, 120, 20)) {
				save_automata_rules();
			}

			if (ex_button("Clear All Rules###vaucrb", x_pos + 150, y_pos, 125, 20)) {
				clear_all_automata_rules();
			}

			if (ex_button(ICON_FA_DOWNLOAD " Load Rules###hauirb", x_pos + 280, y_pos, 120, 20)) {
				load_automata_rules(cart2D_surface_object_to_execute);
			}

			// If the play button was selected in the GUI control widgets of the execution of the cellular automata rules
			// an automata_play flag is set. If in play mode, the execution of the cellular automata process has a time
			// delay between processes so the user can contol and view the progress of the cellular automata iteration
			// That process of delay is performed here and without it, the process in many cases would be so fast that
			// the user would not be able to view or control the intermediate iteration steps between the start and end 
			// iteration step. Done by getting a time stamp when the iteration process began, and what the current time
			// stamp is. If the difference is less than a user defined interation time step, the next iteration step is
			// not performed, otherwise it is.

			if (automata_play) {
				// This here to easily be able to pause stop automata process perform this statement
				// Seems that placing one or both of the following time statements in a location where it is being constantly
				// updated like outside of this if(automata_play) block will cause the application to unpredictably crash.!!!!!
				end = end_clock.now();// current time stamp of iteration step
				auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start); // current time stamp of iteration step to that of begining difference
//printf("cart2D_surface_automata_widget_class : display 0000 : automata_play : %i\n", duration.count());
				if (duration.count() > play_step_execution_delay_time) { // If current time stamp of iteration step > that of begining difference execute next iteration step
//printf("cart2D_surface_automata_widget_class : display 11111 : automata_play\n");
					if (!use_multithreading) {
//printf("cart2D_surface_automata_widget_class : display 2222 : !use_multithreading : %i : %i \n", current_automata_step, number_automata_steps);
						if (current_automata_step < number_automata_steps) {
							if (!automata_paused) {
//printf("cart2D_surface_automata_widget_class : display 3333 : !automata_paused\n");

								//if (!next_step_automata_execution()) { // ++++
								if (!next_step_automata_execution(cart2D_surface_object_to_execute)) { // ++++
//printf("cart2D_surface_automata_widget_class : display 4444 : !perform_automata_step(current_automata_step)\n");
																	//if (log_panel != NULL) log_panel->application_log.AddLog("Unable to continue automata generation");
									stop_automata_execution();
								}
								start = start_clock.now();// begin time stamp of iteration step
							}
						}
					}
					else { // Use multithreading

					}
				}
			}
		}

//printf("cart2D_surface_automata_widget_class : display 5555 : perform_automata_step(current_automata_step)\n");
		ImGui::End();
	}

	// Set flags to values to stop current automata interation process in play mode
	// or reset flags to start values to perform a new sequence of iteration steps
	void stop_automata_execution() {
		automata_reset = true;
		automata_stopped = true;
		current_automata_step = 0;
		progress = 0.0f;
		automata_paused = false;

		automata_play = false;
	}


	void clear_all_automata_rules() {
		clear_automata_rules();
		clear_automata_rule_results();
	}

protected:

private:
	float progress = 0.0f;
	int   number_automata_steps = 0, current_automata_step = 0;
	bool  automata_paused		= false;
	bool  automata_play			= false;
	bool  automata_rules_valid	= false;
	bool  automata_reset		= true;
	bool  use_multithreading	= false;
	bool  animate_automata		= false;
	bool  automata_stopped		= true;
	int   min_step_value = 0, max_step_value = 0;

	bool ignore_individual_step_intervals = false;

	import_export_cart2D_surface_automata_rules_class import_export_cart2D_surface_automata_rules;
	cart2D_surface_automata_rules_widget_class<T> cart2D_surface_automata_rules_widget;
	cart2D_surface_automata_results_widget_class<T> cart2D_surface_automata_results_widget;

	ImVec2 editor_display_window_size = { 610, 200.0f };

	cart2D_surface_import_export_class<T> cart2D_surface_import_export;

	// Varables to be used in varying the delay of executing the automata step function
	std::chrono::steady_clock start_clock; // Timer<milliseconds, steady_clock>
	std::chrono::steady_clock end_clock; // Timer<milliseconds, steady_clock>
	std::chrono::steady_clock::time_point start, end;
	int play_step_execution_delay_time = 1000; // milliseonds
	float play_step_execution_slider_delay_time_seconds = 1.0f;

	// !!!!!!!!!!!!!!!!!!!!! Functions !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	void validate_automata_rules_results(cart2D_grid_base_class<T>* cart2D_surface_object_to_execute) {
		cart2D_surface_automata_results_widget.automata_rules_verified = cart2D_surface_automata_results_widget.verify_result_rules(cart2D_surface_object_to_execute);

		if (cart2D_surface_automata_results_widget.automata_rules_verified)
			vwDialogs::display_error_message("cart2D_surface_automata_widget_class", "INFO : \n Hex Automata rules validated and defined\n");
		else
			vwDialogs::display_error_message("cart2D_surface_automata_widget_class", "INFO : \n Invalid Hex Automata rules \n");

		printf("cart2D_surface_automata_widget_class : validate_automata_rules_results 1111\n");
		//printf("cart2D_surface_automata_widget_class : validate_automata_rules_results"); printf("% i\n", cart2D_surface_automata_results_widget.automata_rules_verified);// Could this cause application to crash
	}

	void clear_automata_rules() {
		cart2D_surface_automata_results_widget.delete_all_automata_rules();
	}

	void clear_automata_rule_results() {
		cart2D_surface_automata_results_widget.delete_all_rule_results();
	}

	void clear_automata_grid(cart2D_grid_class<T>* cart2D_surface_object_to_execute) {
		if (cart2D_surface_object_to_execute) {
			T initial_cart2D_grid_value = cart2D_surface_object_to_execute->get_initial_cart2D_grid_value();

			for (T& cart2D_grid_cell : cart2D_surface_object_to_execute->cart2D_grid) {
				cart2D_grid_cell = initial_cart2D_grid_value;
			}
			cart2D_surface_object_to_execute->update_cart2D_grid_colors();
			cart2D_surface_object_to_execute->delete_all_cart2D_grid_cell_data();
			cart2D_surface_automata_results_widget.reset_rule_results_cart2D_grid_cell_data_id();// +++++
		}
	}



	// Set flags to pause current automata interation process in play mode
	void pause_automata_execution() {
		//printf("paused button pressed\n");
		automata_paused = !automata_paused;

		if (automata_paused == false)
		{
			printf("automata_paused == false\n");
		}
		else
		{
			printf("automata_paused == true\n");
		}
	}

	//void record_automata_execution() {
		// not yet implemented
	//}

	// Set flags to set current automata interation process to play mode
	void play_automata_execution() {
		//printf("cart2D_surface_automata_widget_class::play_automata_execution 00\n");
		if (!automata_play || automata_paused) {
			//printf("cart2D_surface_automata_widget_class::play_automata_execution 11\n");
			setup_play();
			start = start_clock.now();
		}
	}

	// Set flags to values to perform cellular iteration in a play or automatic mode
	void setup_play() {
		//printf("setup_play 00");
		automata_stopped = false;
		automata_paused = false;
		automata_play = true;

		if (automata_reset) {
			setup_automata_execution();
		}
	}

	// Set up flags to start a cellula automata iteration step process
	// Must be done before any cellula automata step process is performed
	void setup_automata_execution() {
		//printf("cart2D_surface_automata_widget_class :: setup_automata_execution 111111 :  %i \n" ,  cart2D_surface_object_to_execute->cart2D_grid.size());

				// following is to update the progress bar
		min_step_value = cart2D_surface_automata_results_widget.get_min_rule_start();
		max_step_value = cart2D_surface_automata_results_widget.get_max_rule_end();

		number_automata_steps = max_step_value;
		current_automata_step = 0;
		automata_reset = false;
	}

	// Perform the next cellular automata iteration step
	// Possinility of performing such an iteraration step using multiple theads in future is facilitated 
	bool next_step_automata_execution(cart2D_grid_class<T>* cart2D_surface_object_to_execute) {
		//if (use_multithreading)
		//	thread_next_step_voxel_automata(); // Yet to be implemented, if at all
		//else
			//return next_step_voxel_automata_no_thread();
		return next_step_voxel_automata_no_thread(cart2D_surface_object_to_execute);
	}

	// Make a copy of the current iteration state which is to be overwritten 
	bool make_copy_of_current_cart2D_surface(std::vector<T>& cart2D_grid, std::vector<T>& cart2D_grid_copy) {
		if (cart2D_grid.empty()) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "cart2D_surface_automata_widget_class<T>::make_copy_of_current_cart2D_surface() : Hex grid to execute automata is empty.\n");
			return false;
		}

		// Need tests for available ram memeory to make a hex grid copy ?????

//printf("cart2D_surface_automata_widget_class :: make_copy_of_current_cart2D_surface 00000 :  %i \n" ,cart2D_grid.size());// ++++
		cart2D_grid_copy.clear();
		cart2D_grid_copy = cart2D_grid;

		return true;
	}

	// Perform the next iteration step using no CPU threading
	bool next_step_voxel_automata_no_thread(cart2D_grid_class<T>* cart2D_surface_object_to_execute) {
		//printf("cart2D_surface_automata_widget_class :: next_step_voxel_automata_no_thread 000\n");
		if (automata_reset) {
			setup_automata_execution();
		}

		automata_stopped = false;

		if (current_automata_step <= max_step_value) {
			if (perform_automata_step(cart2D_surface_object_to_execute, current_automata_step)) {
				current_automata_step++;
				progress = ((float)current_automata_step / (float)max_step_value);
				//printf("cart2D_surface_automata_widget_class :: next_step_voxel_automata_no_thread 111\n");
				return true;
			}
		}
		return false;
	}

	// Perform the cellula automata iteration step
	// cart2D_surface_object_to_execute : hex grid class with current hexgagonal automata grid data of the current current completes iteration step
	// step : iteration step to perform and updat
	bool perform_automata_step(cart2D_grid_class<T>* cart2D_surface_object_to_execute, int step) {
		printf("cart2D_surface_automata_widget_class :: perform_automata_step 00 :");  printf("%i\n", step);

		std::vector<T> cart2D_grid_copy = {};// Copy of hex grid data of current automata step

		if (!cart2D_surface_object_to_execute) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "cart2D_surface_automata_widget_class<T>::perform_automata_step()  No voxel hcp object specified to execute\n");
			return false;
		}

		make_copy_of_current_cart2D_surface(cart2D_surface_object_to_execute->cart2D_grid, cart2D_grid_copy);

		if (cart2D_grid_copy.size() != cart2D_surface_object_to_execute->cart2D_grid.size()) {
			printf("cart2D_surface_automata_widget_class :: perform_automata_step 11AA :"); printf("%i :: %i \n", cart2D_grid_copy.size(), cart2D_surface_object_to_execute->cart2D_grid.size());
		}

		//for (int i = 0; i < cart2D_surface_object_to_execute->cart2D_grid.size(); i++) {
		//	printf("cart2D_surface_automata_widget_class :: perform_automata_step @@@@@@HG : %i ::",i);
		//	switch (cart2D_surface_object_to_execute->cart2D_grid_value_data_type) {
		//		//case cart2D_grid_value_data_type_enum::Boolean: dn->value.second.bvalue = cart2D_grid->cart2D_grid[cart2D_neighbour_index].bvalue; break;
		//		//case cart2D_grid_value_data_type_enum::Float:   dn->value.second.fvalue = cart2D_grid->cart2D_grid[cart2D_neighbour_index].fvalue; break;
		//		//case cart2D_grid_value_data_type_enum::Integer: dn->value.second.ivalue = cart2D_grid->cart2D_grid[cart2D_neighbour_index].ivalue; break;
		//		//case cart2D_grid_value_data_type_enum::Double:  dn->value.second.dvalue = cart2D_grid->cart2D_grid[cart2D_neighbour_index].dvalue; break;
		//
		//	case cart2D_grid_value_data_type_enum::Boolean:  printf("Boolean:%i\n", cart2D_surface_object_to_execute->cart2D_grid[i]); break;
		//	case cart2D_grid_value_data_type_enum::Float:    printf("Float:  %f\n", cart2D_surface_object_to_execute->cart2D_grid[i]); break;
		//	case cart2D_grid_value_data_type_enum::Integer:  printf("Integer:%i\n", cart2D_surface_object_to_execute->cart2D_grid[i]); break;
		//	case cart2D_grid_value_data_type_enum::Double:   printf("Double: %d\n", cart2D_surface_object_to_execute->cart2D_grid[i]); break;
		//	}
		//}

		//for (int i = 0; i < cart2D_grid_copy.size(); i++) {
		//	printf("cart2D_surface_automata_widget_class :: perform_automata_step @@@@@@HGC : %i ::", i);
		//	switch (cart2D_surface_object_to_execute->cart2D_grid_value_data_type) {
		//		//case cart2D_grid_value_data_type_enum::Boolean: dn->value.second.bvalue = cart2D_grid->cart2D_grid[cart2D_neighbour_index].bvalue; break;
		//		//case cart2D_grid_value_data_type_enum::Float:   dn->value.second.fvalue = cart2D_grid->cart2D_grid[cart2D_neighbour_index].fvalue; break;
		//		//case cart2D_grid_value_data_type_enum::Integer: dn->value.second.ivalue = cart2D_grid->cart2D_grid[cart2D_neighbour_index].ivalue; break;
		//		//case cart2D_grid_value_data_type_enum::Double:  dn->value.second.dvalue = cart2D_grid->cart2D_grid[cart2D_neighbour_index].dvalue; break;
		//
		//	case cart2D_grid_value_data_type_enum::Boolean:  printf("Boolean:%i\n", cart2D_grid_copy[i]); break;
		//	case cart2D_grid_value_data_type_enum::Float:    printf("Float:  %f\n", cart2D_grid_copy[i]); break;
		//	case cart2D_grid_value_data_type_enum::Integer:  printf("Integer:%i\n", cart2D_grid_copy[i]); break;
		//	case cart2D_grid_value_data_type_enum::Double:   printf("Double: %d\n", cart2D_grid_copy[i]); break;
		//	}
		//}

		printf("cart2D_surface_automata_widget_class :: perform_automata_step 11"); printf(": %i :: %i \n", step, cart2D_surface_object_to_execute->cart2D_grid.size());
		// Perform next cellula automata iteration step using the current cellular automat data stored within the cart2D_surface_object_to_execute class and
		// store results into the cart2D_grid_copy dynamic vector array. If unsuccesful return a false value and do not destroy current cellular automata data
		if (!cart2D_surface_automata_results_widget.perform_cart2D_surface_automata_generation(step, ignore_individual_step_intervals, cart2D_surface_object_to_execute, cart2D_grid_copy)) return false;

		//for (int i = 0; i < cart2D_grid_copy.size(); i++) {
		//	printf("cart2D_surface_automata_widget_class :: perform_automata_step @@@@@@HGC : %i ::", i);
		//	switch (cart2D_surface_object_to_execute->cart2D_grid_value_data_type) {
		//		//case cart2D_grid_value_data_type_enum::Boolean: dn->value.second.bvalue = cart2D_grid->cart2D_grid[cart2D_neighbour_index].bvalue; break;
		//		//case cart2D_grid_value_data_type_enum::Float:   dn->value.second.fvalue = cart2D_grid->cart2D_grid[cart2D_neighbour_index].fvalue; break;
		//		//case cart2D_grid_value_data_type_enum::Integer: dn->value.second.ivalue = cart2D_grid->cart2D_grid[cart2D_neighbour_index].ivalue; break;
		//		//case cart2D_grid_value_data_type_enum::Double:  dn->value.second.dvalue = cart2D_grid->cart2D_grid[cart2D_neighbour_index].dvalue; break;
		//
		//	case cart2D_grid_value_data_type_enum::Boolean:  printf("Boolean:%i\n", cart2D_grid_copy[i]); break;
		//	case cart2D_grid_value_data_type_enum::Float:    printf("Float:  %f\n", cart2D_grid_copy[i]); break;
		//	case cart2D_grid_value_data_type_enum::Integer:  printf("Integer:%i\n", cart2D_grid_copy[i]); break;
		//	case cart2D_grid_value_data_type_enum::Double:   printf("Double: %d\n", cart2D_grid_copy[i]); break;
		//	}
		//}

		printf("cart2D_surface_automata_widget_class :: perform_automata_step 22"); printf(": %i : %i \n", step, cart2D_surface_object_to_execute->cart2D_grid_cells_data_list.size());

		// Successful step iteration process performed : substitute the next step cellular iteration process and state
		// performed and stored in the cart2D_grid_copy dynamic vector array to be the current cellular iteration state
		// and update the hex grid display colors to correlate to the current defied color gradient 
		cart2D_surface_object_to_execute->cart2D_grid = cart2D_grid_copy;
		cart2D_surface_object_to_execute->update_cart2D_grid_colors();

		//for(int i=0;i< cart2D_surface_object_to_execute->cart2D_grid.size();i++){
		//	printf("cart2D_surface_automata_widget_class :: perform_automata_step 22AA : %i: %i\n",i, cart2D_surface_object_to_execute->cart2D_grid[i]);
		//}

				//for (int i = 0; i < cart2D_surface_object_to_execute->cart2D_grid_cells_data_list.size(); i++) {
					//cart2D_surface_object_to_execute->cart2D_grid_cells_data_list[i].cart2D_grid_cells_display_color = cart2D_surface_automata_results_widget.cart2D_surface_automata_rule_results[i].result_display_color;
					//cart2D_surface_object_to_execute->cart2D_grid_cells_data_list[i].cart2D_grid_cells_display_shape = cart2D_surface_automata_results_widget.cart2D_surface_automata_rule_results[i].result_display_shape;
					//cart2D_surface_object_to_execute->cart2D_grid_cells_data_list[i].display_data = cart2D_surface_automata_results_widget.cart2D_surface_automata_rule_results[i].display_result;
		//printf("cart2D_surface_automata_widget_class :: perform_automata_step 33A : %i : %i \n", step, cart2D_surface_object_to_execute->cart2D_grid_cells_data_list[0].cart2D_grid_cells_index.size());
				//}

		printf("cart2D_surface_automata_widget_class :: perform_automata_step 44"); printf(": %i\n", step);
		return true;
	}

	void thread_next_step_voxel_automata() {// Not implemented
		printf("thread_next_step_voxel_automata 00");
		/*
				if (current_selected_enity_id < 0) {
					if (log_panel != NULL) log_panel->application_log.AddLog("ERROR", "next_step_voxel_automata_thread():: No voxel hcp object specified to execute");
					return;
				}

		printf("thread_next_step_voxel_automata 01 : ");

				automata_stopped = false;

				// Saving Voxel automata data not needed as the automata data is directly saved and stord by this editor
				//Save Automata rules to execute for selected voxel object
				//save_current_automata_data()

				// set automata object to execute automata function data on
				voxel_automata_hcp_functions.cart2D_surface_object_to_execute = cart2D_surface_object_to_execute;
				voxel_automata_hcp_functions.cloud                       = cart2D_surface_object_to_execute;
				voxel_automata_hcp_functions.voxel_hcp_automata_rules    = cart2D_surface_object_to_execute->cart2D_surface_object_data.cart2D_surface_automata_rules;
		//printf("thread_next_step_voxel_automata 02 : ");

				// following is to update the progress bar
				int max_step_value = voxel_automata_hcp_functions.get_max_voxel_automata_step_value();

		//printf("thread_next_step_voxel_automata 03 : %i ",max_step_value);

				number_automata_steps = max_step_value;

				if (current_automata_step <= max_step_value)
					if (thread_perform_automata_step(current_automata_step)) {
						current_automata_step++;
						progress =  ((float)current_automata_step/ (float) max_step_value);
					}
		*/
		automata_stopped = true;

		printf("thread_next_step_voxel_automata 04 : ");
	}

	bool thread_perform_automata_step(int step) { // Not implemented
		printf("INFO : thread_perform_automata_step 00 : %i\n", step);
		/*
				if (!voxel_automata_hcp_thread_functions.perform_voxel_automata_generation(step)) return false;
		//printf("INFO","perform_automata_step 01 : %i",step);

				//####### GET RENDER OBJECT THAT HAS GEOMETRY DATA AND UPDATE #######
				scene_node_class <render_object_class> *scene_voxel_object = scene_manager->get_render_object(current_selected_enity_id);
		//printf("voxel_hcp_autmoata_widget_class :: execute_voxel_function 11111\n");

				if (scene_voxel_object == NULL) {
					if (log_panel != NULL) log_panel->application_log.AddLog("ERROR : Voxel Automata : Could not find voxel in the scene to update geometry data.\n");
		//printf("voxel_hcp_autmoata_widget_class 22222 scene_voxel_object == NULL.\n");
					return false;
				}
				else {
		//printf("voxel_hcp_autmoata_widget_class 33333 scene_voxel_object != NULL.\n");

					if (!voxel_hcp_render.update_geometry_vertex_cloud_data(&cart2D_surface_object_to_execute->point_cloud, scene_voxel_object, log_panel)) {
						if (log_panel != NULL) log_panel->application_log.AddLog("ERROR :Voxel Automata : Scene voxel object geometry could not be updated.\n");
		//printf("voxel_hcp_autmoata_widget_class 44444 not updated\n");
						return false;
					}

		//printf("voxel_hcp_autmoata_widget_class :: execute_voxel_function 55555\n");
				}
		*/
		return true;
	}



	void save_automata_rules() {// Have crash here !!!!!
		//printf("cart2D_surface_automata_widget_class :: save_automata_rules 000\n");
		char const* patterns[] = { "*C2AR.txt" };
		//printf("cart2D_surface_automata_widget_class :: save_automata_rules 000AAAA\n");
		char const* file_pathname = vwDialogs::save_file(automata_rules_dir_path.c_str(), patterns, 1);// get file pathname to save automata rules data to 
		//printf("cart2D_surface_automata_widget_class :: save_automata_rules 000BBBB\n");
		if (file_pathname == nullptr) {
			//printf("cart2D_surface_automata_widget_class :: save_automata_rules 000CCC\n");
						//vwDialogs::display_error_message("Save Automata rules", "ERROR : \n No automata rules file defined to save hex surface automata rules data to\nSave automata rules aborted");
			return;
		}
		//printf("cart2D_surface_automata_widget_class :: save_automata_rules 111\n");;

				// Link current automata rules class data to export class
		import_export_cart2D_surface_automata_rules.cart2D_surface_automata_result_rules = cart2D_surface_automata_results_widget.cart2D_surface_automata_rule_results;
		import_export_cart2D_surface_automata_rules.cart2D_surface_automata_rules = cart2D_surface_automata_results_widget.cart2D_surface_automata_rules_widget.cart2D_surface_automata_rules;
		import_export_cart2D_surface_automata_rules.export_cart2D_surface_automata_rules(file_pathname);

		//printf("cart2D_surface_automata_widget_class :: save_automata_rules 222\n");
	}

	void load_automata_rules(cart2D_grid_class<T>* cart2D_surface_object_to_execute) {
		//printf("cart2D_surface_automata_widget_class::load_automata_rules 000\n");
		char const* patterns[] = { "*C2AR.txt" };
		//printf("cart2D_surface_automata_widget_class::load_automata_rules 111");printf("%s \n", patterns[0]);
		char const* file_pathname = vwDialogs::open_file(automata_rules_dir_path.c_str(), patterns, 1);// get file pathname to import automata rules data from

		if (file_pathname == nullptr) {
			//vwDialogs::display_error_message("Save Automata rules", "ERROR : \n No automata rules file defined to import hex grid automata rules data from.\n");
			return;
		}
		//printf("cart2D_surface_automata_widget_class::load_automata_rules 222\n");
		stop_automata_execution();
		cart2D_surface_object_to_execute->delete_all_cart2D_grid_cell_data();// clear Cartesian 2D automata grid data

		//printf("cart2D_surface_automata_widget_class::load_automata_rules 333\n");
		cart2D_surface_automata_results_widget.define_and_clear_cart2D_surface_automata_rules_and_results();// clear Cartesian 2D automata rules data

		import_export_cart2D_surface_automata_rules.import_cart2D_surface_automata_rules(file_pathname);// import Cartesian 2D autoamata rules into temp import_export_cart2D_surface_automata_rules class list of rules

		//td::cout << "cart2D_surface_automata_widget_class::load_automata_rules 444 : "; std::cout << import_export_cart2D_surface_automata_rules.cart2D_surface_automata_rules.size() << std::endl;
				// Create new list of Cartesian 2D automata result rules from imported list of Cartesian 2D automata result rules
		for (cart2D_surface_automata_rule_result_struct_type& result_rule : import_export_cart2D_surface_automata_rules.cart2D_surface_automata_result_rules) {
			result_rule.define_imgui_shape();
			cart2D_surface_automata_results_widget.add_rule_result(result_rule);
		}

		// Create new list of Cartesian 2D automata rules from imported list of Cartesian 2D automata rules
		for (cart2D_surface_automata_rule_struct_type rule : import_export_cart2D_surface_automata_rules.cart2D_surface_automata_rules) {
			// Need to edit sub rule id number manually here to enable ImGui to display widgets without error and to initialise an ID idetification for each sub rule.
			for (cart2D_automata_sub_rule_struct_type& sub_rule : rule.sub_rules) {
				//std::cout << "cart2D_surface_automata_widget_class::load_automata_rules iiiiiA : "; std::cout << rule.sub_rules.size() << "::" << sub_rule.sub_rule_id << std::endl;
				rule.sub_rule_id += 1;
				sub_rule.sub_rule_id = rule.sub_rule_id;
				//std::cout << "cart2D_surface_automata_widget_class::load_automata_rules iiiiiB : "; std::cout << rule.sub_rules.size() << "::" << sub_rule.sub_rule_id << std::endl;
			}
			cart2D_surface_automata_results_widget.cart2D_surface_automata_rules_widget.add_rule(rule);
		}

		setup_automata_execution();
		//std::cout << "cart2D_surface_automata_widget_class::load_automata_rules 1111 : "; std::cout << cart2D_surface_automata_results_widget.cart2D_surface_automata_rules_widget.cart2D_surface_automata_rules.size() << std::endl;
	}

	void export_automata_grid(cart2D_grid_class<T> *cart2D_surface_object_to_execute) {
printf("cart2D_surface_automata_widget_class :: export_automata_grid000\n");
		char const* patterns[] = { "*C2AG.txt" };
		char const* file_pathname = vwDialogs::save_file(automata_grids_dir_path.c_str(), patterns, 1);

		if (file_pathname == nullptr) {// No file selected
			//vwDialogs::display_error_message("Save Automata Grid Data", "ERROR : \n No hex grid data file defined to save hex surface grid data to\nSave hex grid data aborted");
			return;
		}
//else
//printf("save_generation_parameters != NULL %s \n", file_pathname);
//printf("save_automata_rules != NULL  \n");
printf("cart2D_surface_automata_widget_class :: export_automata_grid111\n");
		cart2D_surface_import_export.export_cart2D_surface_automata_grid(file_pathname, cart2D_surface_object_to_execute);

//if (log_panel != NULL) log_panel->application_log.AddLog("INFO : hex surface automata rules data saved to file\n %s\n", file_pathname);
	}

	void import_automata_grid(cart2D_grid_class<T> *cart2D_surface_object_to_execute) {
printf("cart2D_surface_automata_widget_class :: import_automata_grid000\n");
		stop_automata_execution();

		char const* patterns[] = { "*C2AG.txt" };
printf("cart2D_surface_automata_widget_class :: import_automata_grid111\n");
		char const* file_pathname = vwDialogs::open_file(automata_grids_dir_path.c_str(), patterns, 1);

		if (file_pathname == nullptr) {
			return;
		}

		if (!cart2D_surface_object_to_execute) {
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "cart2D_surface_automata_widget_class<T>::import_automata_grid() : ERROR :  Could not import automata hex grid. cart2D_surface_object_to_execute undefined \n");
			return;
		}
printf("cart2D_surface_automata_widget_class :: import_automata_grid222\n");
		cart2D_surface_object_to_execute->clear_all_cart2D_grid_cell_data(); // It is assumed that when inporting a grid, the automata rules are not changed so only need to clear existing hex grid cell data
		//cart2D_surface_object_to_execute->delete_all_cart2D_grid_cell_data();
		cart2D_surface_object_to_execute->delete_cart2D_grid();
		cart2D_surface_automata_results_widget.reset_rule_results_cart2D_grid_cell_data_id();
//else
//	//printf("save_generation_parameters != NULL %s \n", file_pathname);
//	printf("load_automata_rules != NULL  \n");

		cart2D_surface_import_export.import_cart2D_surface_automata_grid(file_pathname, cart2D_surface_object_to_execute);

		cart2D_surface_object_to_execute->define_cart2D_grid_coordinates();
		cart2D_surface_object_to_execute->update_cart2D_grid_colors();

std::cout << "cart2D_surface_automata_widget_class<T>::import_automata_grid() 00000 : " << cart2D_surface_object_to_execute->cart2D_grid.size() << std::endl;// ++++
	}

};
