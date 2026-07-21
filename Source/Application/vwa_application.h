#pragma once

//enum class grid_value_data_type_enum { Boolean, Float, Integer, Double, nan };
//#define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V)) // move this to another location ????

#include "Source/Application/vwa_application_parameters.h"

#include "Source/Hex2D/ahex_application.h"
#include "Source/Cart2D/cart2D_application.h"

// Move these enum class to a definition .h file
enum class automata_grid_type_enum {none, hex2D, cart2D, trd3D, cart3D };
enum class automata_data_type_enum {None, Integer, Float, Boolean};

class vwa_application_class {
public:
	vwa_application_class() {}
	~vwa_application_class() {}

		void display_ahex_main_gui_panel(glfw_openGL_window_class *glfw_window) {
//printf("ahex_application_class::display_ahex_main_gui_panel 000\n");	
		// Display main window menu bar
			if (ImGui::BeginMainMenuBar()) {// UnComment out to have menu in application main window

				if (ImGui::BeginMenu("Project")) {
//printf("display_tree_branch :BeginMenu: %s : %i \n", child_tree_branch->label.c_str(), i);
					if (ImGui::BeginMenu("New Automata Project")) {
						if (ImGui::BeginMenu("Hexagonal 2D ...")) {
							if (ImGui::MenuItem("Hexagonal 2D Integer")) {
//printf("ahex_application_class:display_ahex_main_gui_panel:Hexagonal 2D\n");
								automata_grid_type = automata_grid_type_enum::hex2D;
								automata_data_type = automata_data_type_enum::Integer;

								if (ahex_application_int.global_hex_grid) ahex_application_int.end_current_project();
								if (!ahex_application_int.global_hex_grid) ahex_application_int.create_global_grid_popup = true;
							}

							if (ImGui::MenuItem("Hexagonal 2D Float")) {
								automata_grid_type = automata_grid_type_enum::hex2D;
								automata_data_type = automata_data_type_enum::Float;
//printf("ahex_application_class:display_ahex_main_gui_panel:Hexagonal 2D\n");
								//if (ahex_application.global_hex_grid) ahex_application.end_current_project();
								//if (!ahex_application.global_hex_grid) ahex_application.create_global_grid_popup = true;
							}

							if (ImGui::MenuItem("Hexagonal 2D Boolean")) {
								automata_grid_type = automata_grid_type_enum::hex2D;
								automata_data_type = automata_data_type_enum::Boolean;
//printf("ahex_application_class:display_ahex_main_gui_panel:Hexagonal 2D\n");
								//if (ahex_application.global_hex_grid) ahex_application.end_current_project();
								//if (!ahex_application.global_hex_grid) ahex_application.create_global_grid_popup = true;
							}

							ImGui::EndMenu();
						}

						if (ImGui::BeginMenu("Cartesian 2D")) {
							if (ImGui::MenuItem("Cartesian 2D Integer")) {
//printf("ahex_application_class:display_ahex_main_gui_panel:Hexagonal 2D\n");
								automata_grid_type = automata_grid_type_enum::cart2D;
								automata_data_type = automata_data_type_enum::Integer;

								if (cart2D_application_int.global_cart2D_grid) cart2D_application_int.end_current_project();
								if (!cart2D_application_int.global_cart2D_grid) cart2D_application_int.create_global_grid_popup = true;
							}

							if (ImGui::MenuItem("Cartesian 2D Float")) {
								automata_grid_type = automata_grid_type_enum::cart2D;
								automata_data_type = automata_data_type_enum::Float;
//printf("ahex_application_class:display_ahex_main_gui_panel:Hexagonal 2D\n");
								//if (ahex_application.global_hex_grid) ahex_application.end_current_project();
								//if (!ahex_application.global_hex_grid) ahex_application.create_global_grid_popup = true;
							}

							if (ImGui::MenuItem("Cartesian 2D Boolean")) {
								automata_grid_type = automata_grid_type_enum::cart2D;
								automata_data_type = automata_data_type_enum::Boolean;
//printf("ahex_application_class:display_ahex_main_gui_panel:Hexagonal 2D\n");
								//if (ahex_application.global_hex_grid) ahex_application.end_current_project();
								//if (!ahex_application.global_hex_grid) ahex_application.create_global_grid_popup = true;
							}

							ImGui::EndMenu();
						}

						if (ImGui::MenuItem("Trapezo-rhombic dodecahedral 3D")) {
							//printf("ahex_application_class:display_ahex_main_gui_panel:Cartesian 2D\n");
														//if (global_hex_grid) end_current_project();
														//if (!global_hex_grid) create_global_grid_popup = true;
						}

						if (ImGui::MenuItem("Cartesian 3D")) {
							//printf("ahex_application_class:display_ahex_main_gui_panel:Cartesian 2D\n");
														//if (global_hex_grid) end_current_project();
														//if (!global_hex_grid) create_global_grid_popup = true;
						}

						ImGui::EndMenu();
					}


					if (ImGui::MenuItem("End Current Automata Poject")) {
//printf("Edit selection :MenuItem:\n");
						//if (ahex_application_int.global_hex_grid) { ahex_application_int.end_current_project(); }
						end_current_project();
					}

					if (ImGui::MenuItem("Add Child Grid")) {
//printf("Remove from selection :MenuItem:\n");
						//if (ahex_application_int.global_hex_grid) { ahex_application_int.create_hex_sub_grid_popup = true; }
					}

					if (ImGui::MenuItem("Exit")) {
//printf("Exit WSM :MenuItem:\n");
						//ahex_application_int.exit_ahex_application(glfw_window);
						exit_application(glfw_window);
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

		switch(automata_grid_type){
			case automata_grid_type_enum::hex2D  : perform_hex2D_grid_run_loop(); break;
			case automata_grid_type_enum::cart2D : perform_cart2D_grid_run_loop(); break;
			case automata_grid_type_enum::trd3D  : perform_trd3D_grid_run_loop(); break;
			case automata_grid_type_enum::cart3D : perform_Cart3D_grid_run_loop(); break;

		}

		//ahex_application_int.hex_grid_run_loop();
	}

	void perform_hex2D_grid_run_loop() {
		switch (automata_data_type) {
			case automata_data_type_enum::Integer : {ahex_application_int.hex_grid_run_loop();   break; }
			//case automata_data_type_enum::Float   : {ahex_application_float.hex_grid_run_loop(); break; }
			//case automata_data_type_enum::Boolean : {ahex_application_bool.hex_grid_run_loop();  break; }
		}
	}

	void perform_cart2D_grid_run_loop() {
		switch (automata_data_type) {
			case automata_data_type_enum::Integer: { cart2D_application_int.cart2D_grid_run_loop();   break; }
			//case automata_data_type_enum::Float   : {ahex_application_float.hex_grid_run_loop(); break; }
			//case automata_data_type_enum::Boolean : {ahex_application_bool.hex_grid_run_loop();  break; }
		}
	}

	void perform_trd3D_grid_run_loop() {
		switch (automata_data_type) {
			//case automata_data_type_enum::Integer: { ahex_application_int.hex_grid_run_loop();   break; }
			//case automata_data_type_enum::Float   : {ahex_application_float.hex_grid_run_loop(); break; }
			//case automata_data_type_enum::Boolean : {ahex_application_bool.hex_grid_run_loop();  break; }
		}
	}

	void perform_Cart3D_grid_run_loop() {
		switch (automata_data_type) {
			//case automata_data_type_enum::Integer: { ahex_application_int.hex_grid_run_loop();   break; }
			//case automata_data_type_enum::Float   : {ahex_application_float.hex_grid_run_loop(); break; }
			//case automata_data_type_enum::Boolean : {ahex_application_bool.hex_grid_run_loop();  break; }
		}
	}

	void end_current_project() {
		switch (automata_grid_type) {
			case automata_grid_type_enum::hex2D:  end_hex2d_project(); break;
			case automata_grid_type_enum::cart2D: end_cart2d_project(); break;
			case automata_grid_type_enum::trd3D:  end_trd3d_project(); break;
			case automata_grid_type_enum::cart3D: end_cart3d_project(); break;
		}
	}

	void end_hex2d_project() {
		switch (automata_data_type) {
			case automata_data_type_enum::Integer : { ahex_application_int.end_current_project();   break; }
			//case automata_data_type_enum::Float   : { ahex_application_float.end_current_project(); break; }
			//case automata_data_type_enum::Boolean : { ahex_application_bool.end_current_project();  break; }
		}
	}

	void end_cart2d_project() {
		switch (automata_data_type) {
			case automata_data_type_enum::Integer: { cart2D_application_int.end_current_project();   break; }
		//	case automata_data_type_enum::Float:   { cart2D_application_float.end_current_project(); break; }
		//	case automata_data_type_enum::Boolean: { cart2D_application_bool.end_current_project();  break; }
		}
	}

	void end_trd3d_project() {
		//switch (automata_data_type) {
		//	case automata_data_type_enum::Integer: { ahex_application_int.end_current_project();   break; }
		//	case automata_data_type_enum::Float:   { ahex_application_float.end_current_project(); break; }
		//	case automata_data_type_enum::Boolean: { ahex_application_bool.end_current_project();  break; }
		//}
	}

	void end_cart3d_project() {
		//switch (automata_data_type) {
		//	case automata_data_type_enum::Integer: { ahex_application_int.end_current_project();   break; }
		//	case automata_data_type_enum::Float:   { ahex_application_float.end_current_project(); break; }
		//	case automata_data_type_enum::Boolean: { ahex_application_bool.end_current_project();  break; }
		//}
	}

	void exit_application(glfw_openGL_window_class* glfw_window) {
		end_current_project();
		glfwDestroyWindow(glfw_window->get_window_ptr());
		exit(0);
	}


private:
	automata_grid_type_enum automata_grid_type = automata_grid_type_enum::hex2D;
	automata_data_type_enum automata_data_type = automata_data_type_enum::Integer;

	ahex_application_class<int>   ahex_application_int;
	//ahex_application_class<float> ahex_application_float;
	//ahex_application_class<bool>  ahex_application_bool;

	cart2D_application_class<int>   cart2D_application_int;
	//ahex_application_class<float> ahex_application_float;
	//ahex_application_class<bool>  ahex_application_bool;


};