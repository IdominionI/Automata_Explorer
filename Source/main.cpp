
#include <FrameWork/AFW/Interface/IconsFontAwesome4.h>

#include <Framework/AFW/Application/afw_global.h>
#include <FrameWork/AFW/Application/afw_app_base.h>

#include <FrameWork/OGLFW/Window/glfw_ogl_window.h>
#include <FrameWork/OGLFW/imGui/oglfw_imgui_glfw.h>

#include "Application/ahex_application.h"

#include <glm/glm.hpp>

// Main application class to setup, run and manage the application
// using the FrameWork application base class
class app_main_class : public afw_app_base_class {
public:
    app_main_class() {}
    ~app_main_class() {}

    glfw_openGL_window_class *create_glfw_window() {
        glfw_openGL_window_class *glfw_window = new glfw_openGL_window_class();
        glfw_window->settings.setGLVersion(4, 6);
        if (!glfw_window->create_window("Hex grid Automata", { 200.0f,200.0f }, 1600, 800)) {
            delete glfw_window;
            return nullptr;
        }
        return glfw_window;
    }

    // Define and set application logger to save debug and other logging messages to
    void  define_logger() {
        logger = new afw_Logger_class("logger.txt");
        afw_globalc::set_current_logger(logger);
    }


    void setup() override {
        define_logger();

        glfw_window = create_glfw_window();
        if (!glfw_window) {
            afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "Unable to initialize/create OpenGL GLFW! : Application Aborted");
            exit(0);
        }

        //initialise imgui
        oglfw_ImGui.ImGui_init(glfw_window->get_window_ptr());

        //log_panel = new log_panel_class();
        //if (log_panel == NULL) {
        //    afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "No Applicaton Logger Defined : Cannot perform application");
        //    return;
        //}


        // This causes render problems and need to be left commented out until debuged
        // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        // Set up the fonts file to use to display text 
        // and the awesome icon font file to display icons since ImGui is not designed
        // to have icon images imported and displayed as part of the GUI !!!!!!!
        ImFontConfig cfg;
        cfg.OversampleH = 3;
        cfg.OversampleV = 1;
        cfg.PixelSnapH = true;
        //cfg.GlyphExtraSpacing.x = 0.0f;// Depreciated
        //cfg.GlyphExtraSpacing.y = 0.0f;// Depreciated
        cfg.GlyphExtraAdvanceX = 0.0f; // New to ImGui to replace GlyphExtraSpacing
        cfg.RasterizerMultiply = 1.25f;

        ImGuiIO& io = ImGui::GetIO();

        io.Fonts->Clear();
        ImFont* mainFont = io.Fonts->AddFontFromFileTTF("Fonts/Cousine-Regular.ttf", 13.0f, &cfg);
        static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };

        ImFontConfig icons_config;
        icons_config.MergeMode = true;
        icons_config.GlyphOffset = ImVec2(0, 1);
        ImFont* iconFont = io.Fonts->AddFontFromFileTTF("Fonts/fontawesome_6_solid.otf", 13.0f + 1.0, &icons_config, icons_ranges);
        // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    }

    // application run loop
    void run_loop() override {
        while (!glfwWindowShouldClose(glfw_window->get_window_ptr()))
        {
//printf("here\n");

            // IO
            glfw_window->poll_events();

            if (glfwWindowShouldClose(glfw_window->get_window_ptr())) {
                afw_globalc::get_current_logger()->log(LogLevel::INFO, "EXITED APPLICATION WITHOUT SAVING CHANGES!!!!\n");
            }

            ///Process user input, in this case if the user presses the 'esc' key
            ///to close the application
            gflw_event.glfw_key_event    = glfw_window->glfw_key_event;
            gflw_event.glfw_mouse_event  = glfw_window->glfw_mouse_event;
            gflw_event.glfw_cursor_event = glfw_window->glfw_cursor_event;


            // Update any changes to the window framebuffer to be used in rendering the scene
            // Code here in its raw form rather than as a function in class glfw_ogl_window so as to compare
            // with any opengl or glfw code that may be referenced as an example or modification
            glfwGetFramebufferSize(glfw_window->get_window_ptr(), &glfw_window->window_w, &glfw_window->window_h);
            glViewport(0, 0, glfw_window->window_w, glfw_window->window_h);

            glClearColor(background_color.r, background_color.g, background_color.b, background_color.a );
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            
            // !!!!!!!!!!!!!!! Window callback actions must be performed before ImGui rendering !!!!!!!!!!!!!

            //if (entered_window(glfw_window)) {
            //    printf("run loop entered_window\n");
            //    glfw_window->window_entered = true;
            //    glfw_window->glfw_cursor_event->reset_cursor_enter_event();
            //} else
            //    glfw_window->window_entered = false;

            //if (glfw_window->is_window_hovered()) {
            //    glm::vec2 window_pos = get_cursor_position(glfw_window);
            //    printf("cursor position callback x : %f :: y : %f\n", window_pos.x, window_pos.y);
            //}


            //if (window_is_resized(active_window)) {
            //    active_window->perform_window_resize_action();
            //}

            //if (window_is_moved(active_window)) {
            //    active_window->perform_window_move_action();
            //}

            //if (window_focused(glfw_window)) {
            //    //printf("run loop window_focused\n");
            //    //active_window = glfw_window;
            //    glfw_window->window_active = true;
            //} else
            //    glfw_window->window_active = false;

            //if (window_maximized(glfw_window)) {
            //    printf("run loop window_maximized %i\n",glfw_window->glfw_window_event->window_maximized);
            //}
//printf("before02\n");
            // Perform Imgui pre render routines to set up UmGui to 
            // render to opengl window for a new frame of display 
            oglfw_ImGui.ImGui_pre_render();
//printf("After02\n");
            // Draw the UI for the current opengl window frame
            draw_UI();// This must be placed before glfwSwapBuffers
//printf("After03\n");
            // Perform Imgui post render routines of the current render frame 
            oglfw_ImGui.ImGui_post_render(glfw_window);
//printf("After04\n");

            // Display current glfw window frame display buffer 
            glfwSwapBuffers(glfw_window->get_window_ptr());// This must be presentc
        }
    }

    void draw_UI() {
        // Define interface style parameters each render pass so as
        // to be able to change interface style or style components
        //tron_style();
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());// Required to have the main glfw window viewport as the main docking node

        ahex_application.display_ahex_main_gui_panel(glfw_window);// Display main application GUI
 
        // Uncommented following uwhen investigating ImGui widget examples
        ImGui::Begin;
        bool show_demo_window = true;
        bool show_plot_demo_window = true;
        //ImGui::ShowDemoWindow(&show_demo_window);
        //ImPlot::ShowDemoWindow(&show_plot_demo_window);
        ImGui::End;

    
        // Following testing only : delete/comment out when no longer needed
/*      srand(0);
        static float xs1[100], ys1[100];
        for (int i = 0; i < 100; ++i) {
            xs1[i] = i * 0.01f;
            ys1[i] = xs1[i] + 0.1f * ((float)rand() / (float)RAND_MAX);
        }
        static float xs2[50], ys2[50];
        for (int i = 0; i < 50; i++) {
            xs2[i] = 0.25f + 0.2f * ((float)rand() / (float)RAND_MAX);
            ys2[i] = 0.75f + 0.2f * ((float)rand() / (float)RAND_MAX);
        }
        ImGui::Begin("Scatter Plot");// If Have name of ImGui::Begin then have ability to minimise and dock 
            if (ImPlot::BeginPlot("Scatter Plot")) {
                ImPlot::PlotScatter("Data 1", xs1, ys1, 100);
                ImPlot::PlotScatter("Data 2", xs2, ys2, 50, {
                    ImPlotProp_Marker, ImPlotMarker_Square,
                    ImPlotProp_MarkerSize, 6,
                    ImPlotProp_LineColor, ImPlot::GetColormapColor(1),
                    ImPlotProp_FillColor, ImPlot::GetColormapColor(1),
                    ImPlotProp_FillAlpha, 0.25f
                    });
                ImPlot::EndPlot();
            }
        ImGui::End();
*/

        // ----------------------------------


        ImGui::Render();// Render ImGui GUI
    }

    void close() override {
        oglfw_ImGui.ImGui_end();

        if (glfw_window) { glfw_window->close(); }
        glfwTerminate();
    }


    gflw_event_struct_type gflw_event;

    glm::vec4 background_color = { 0.0f,0.0f,0.0f,1.0f };

private:
    // FrameWork glfw openGl window class that handles and manages openGL glfw windows
    glfw_openGL_window_class *glfw_window = nullptr;

    // FrameWork glfw openGl ImGui class that handles and manages ImGui glfw openGL implementation
    oglFW_ImGui_GLFW_class oglfw_ImGui;
    
    // FrameWork Logger class that handles application logging messages
    afw_Logger_class* logger = nullptr;

    // UI components
    //log_panel_class* log_panel = nullptr;

    // Hexagaonal automata application class that handles and manages application GUI, execution and exit
    // Currently set to handle hexagaonal automata grid of type integer. 
    // Future enhancement to handle float and boolean datatypes to be done
    ahex_application_class<int> ahex_application;
};

// Application entry point
int main() {
    app_main_class* app = new app_main_class;

    app->run();

    exit(0);
}