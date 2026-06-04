#pragma once

//#include <string>
#include <random>

//#include <FrameWork/AFW/ImGUI/ImGuiEx/imgui_custom.h>
//#include <FrameWork/AFW/ImGUI/ImGuiEx/imgui_widgets.h>
//#include <FrameWork/AFW/Interface/IconsFontAwesome4.h>

// --------------- Random number generator variables -----------------------------

#define USE_USER_DEFINED_SEED    0
#define USE_MACHINE_DEFINED_SEED 1

#define USE_TIME_DEFINED_SEED 0
#define USE_DEVICE_DEFINED_SEED 1

enum class number_generation_engine_enum { default_engine, minstd_rand, minstd_rand0, mt19937, mt19937_64, ranlux24_base, ranlux48_base, ranlux24, ranlux48, knuth_b };
//enum class number_generation_engine_enum { default_engine, minstd_rand, minstd_rand0, mt19937, mt19937_64, ranlux24_base, ranlux48_base, ranlux24, ranlux48, knuth_b, random_device }; // Using random device creates compile errors

enum class distribution_category_enum { Uniform, Bernouli_related, Rate_based, Normal_related, Undefined };
enum class distribution_parameter_data_type_enum { Integer, Float, Boolean };
enum class distribution_parameter_input_type_enum { single, dual, none };
//enum class distribution_engine_enum { Uniform_int, Uniform_real, bernoulli, binomial, geometric, negative_binomial, poisson, exponential, gamma, weibull, extreme_value, normal, lognormal, chi_sqaured, cauchy, fisher_f, student_t, discrete, piecwise_constant, peicwise_linear, seed_sequence, generate_canonical };
enum class distribution_engine_enum { Uniform_int, Uniform_real, bernoulli, binomial, geometric, negative_binomial, poisson, exponential, gamma, weibull, extreme_value, normal, lognormal, chi_sqaured, cauchy, fisher_f, student_t };


union distribution_parameter_value {
    int   integer_p;
    float float_p;
    bool  bool_p;
};

struct distribution_parameter_data_struct_type {
    distribution_parameter_data_struct_type() {
        value.float_p = 0.0f;
    }

    std::string parameter_text = "parameter";
    distribution_parameter_data_type_enum data_type = distribution_parameter_data_type_enum::Float;
    distribution_parameter_value value;
};

struct distribution_parameter_input_struct_type {
    distribution_category_enum             distribution_category = distribution_category_enum::Undefined;
    distribution_engine_enum               distribution_engine = distribution_engine_enum::Uniform_int;
    distribution_parameter_input_type_enum input_type = distribution_parameter_input_type_enum::dual;
    distribution_parameter_data_type_enum  output_data_type = distribution_parameter_data_type_enum::Integer;

    distribution_parameter_data_struct_type parameter_1;
    distribution_parameter_data_struct_type parameter_2;
};

struct random_number_generator_struct_type {
    number_generation_engine_enum number_generation_engine = number_generation_engine_enum::default_engine;

    bool               use_default_seed = true;
    unsigned long long seed_value = 0;

    std::default_random_engine default_random_engine;
    std::minstd_rand           minstd_rand_engine;
    std::minstd_rand0          minstd_rand0_engine;
    std::mt19937               mt19937_rand_engine;
    std::mt19937_64            mt19937_64_rand_engine;
    std::ranlux24_base         ranlux24_base_rand_engine;
    std::ranlux48_base         ranlux48_base_rand_engine;
    std::ranlux24              ranlux24_rand_engine;
    std::ranlux48              ranlux48_rand_engine;
    std::knuth_b               knuth_b_rand_engine;
    //std::random_device         random_device_rand_engine; //Cannot use as this causes a function cannot be referenced - it is a deleted function error 

    void define_random_generator_engine_to_use() {
        switch (number_generation_engine) {
            case number_generation_engine_enum::default_engine  : { default_random_engine.seed(seed_value); break;}
            case number_generation_engine_enum::minstd_rand     : { minstd_rand_engine.seed(seed_value); break;}
            case number_generation_engine_enum::minstd_rand0    : { minstd_rand0_engine.seed(seed_value); break;}
            case number_generation_engine_enum::mt19937         : { mt19937_rand_engine.seed(seed_value); break;}
            case number_generation_engine_enum::mt19937_64      : { mt19937_64_rand_engine.seed(seed_value); break;}
            case number_generation_engine_enum::ranlux24_base   : { ranlux24_base_rand_engine.seed(seed_value); break;}
            case number_generation_engine_enum::ranlux48_base   : { ranlux48_base_rand_engine.seed(seed_value); break;}
            case number_generation_engine_enum::ranlux24        : { ranlux24_rand_engine.seed(seed_value); break;}
            case number_generation_engine_enum::ranlux48        : { ranlux48_rand_engine.seed(seed_value); break;}
            case number_generation_engine_enum::knuth_b         : { knuth_b_rand_engine.seed(seed_value); break;}
        }
    }

};

template <class random_number_distribution_type, typename return_value_type>
class random_number_generation_base_class {
public:
    random_number_generation_base_class() {}
    ~random_number_generation_base_class() {}

    random_number_generator_struct_type      random_number_generator;
    distribution_parameter_input_struct_type distribution_parameters;
    return_value_type                        generated_number_value;

    random_number_distribution_type         random_number_distribution;

    void define_random_number_generation(random_number_generator_struct_type rn_generator, distribution_parameter_input_struct_type random_distribution_input_parameters) {
        random_number_generator = random_number_generator;
        distribution_parameters = random_distribution_input_parameters;

        define_distribution_range();
    }

    void define_distribution_range() {
        if (distribution_parameters.input_type == distribution_parameter_input_type_enum::single) {
            define_single_distribution_range();
        } else {
            if (distribution_parameters.input_type == distribution_parameter_input_type_enum::dual) {
                define_dual_distribution_range();
            }
        }
        // do nothing where no distribution range required
    }

    return_value_type generate_random_number() {
        switch (random_number_generator.number_generation_engine) {
            case number_generation_engine_enum::default_engine  : { generated_number_value = random_number_distribution(random_number_generator.default_random_engine); break; }
            case number_generation_engine_enum::minstd_rand     : { generated_number_value = random_number_distribution(random_number_generator.minstd_rand_engine); break; }
            case number_generation_engine_enum::minstd_rand0    : { generated_number_value = random_number_distribution(random_number_generator.minstd_rand0_engine); break;}
            case number_generation_engine_enum::mt19937         : { generated_number_value = random_number_distribution(random_number_generator.mt19937_rand_engine); break;}
            case number_generation_engine_enum::mt19937_64      : { generated_number_value = random_number_distribution(random_number_generator.mt19937_64_rand_engine); break;}
            case number_generation_engine_enum::ranlux24_base   : { generated_number_value = random_number_distribution(random_number_generator.ranlux24_base_rand_engine); break;}
            case number_generation_engine_enum::ranlux48_base   : { generated_number_value = random_number_distribution(random_number_generator.ranlux48_base_rand_engine); break;}
            case number_generation_engine_enum::ranlux24        : { generated_number_value = random_number_distribution(random_number_generator.ranlux24_rand_engine); break;}
            case number_generation_engine_enum::ranlux48        : { generated_number_value = random_number_distribution(random_number_generator.ranlux48_rand_engine); break;}
            case number_generation_engine_enum::knuth_b         : { generated_number_value = random_number_distribution(random_number_generator.knuth_b_rand_engine); break;}
        }

        return generated_number_value;
    }

    virtual void define_single_distribution_range() {
        //switch (distribution_parameters.parameter_1.data_type) {
        //    case distribution_parameter_data_type_enum::Integer : { random_number_distribution = random_number_distribution_type(distribution_parameters.parameter_1.value.integer_p); break;}
        //    case distribution_parameter_data_type_enum::Float   : { random_number_distribution = random_number_distribution_type(distribution_parameters.parameter_1.value.float_p); break;}
        //    case distribution_parameter_data_type_enum::Boolean : { random_number_distribution = random_number_distribution_type(distribution_parameters.parameter_1.value.bool_p); break;}
        //}
    }

    virtual void define_dual_distribution_range() {
        //switch (distribution_parameters.parameter_1.data_type) {// Following parameter testing not really needed, but used in case have distributions parameter values using different data types in future
        //    case distribution_parameter_data_type_enum::Integer: {
        //        switch (distribution_parameters.parameter_2.data_type) {
        //            case distribution_parameter_data_type_enum::Integer : { random_number_distribution = random_number_distribution_type(distribution_parameters.parameter_1.value.integer_p, distribution_parameters.parameter_2.value.integer_p); break;}
        //            case distribution_parameter_data_type_enum::Float   : { random_number_distribution = random_number_distribution_type(distribution_parameters.parameter_1.value.integer_p, distribution_parameters.parameter_2.value.float_p); break;}
        //            case distribution_parameter_data_type_enum::Boolean : { random_number_distribution = random_number_distribution_type(distribution_parameters.parameter_1.value.integer_p, distribution_parameters.parameter_2.value.bool_p); break;}
        //        }
        //        break;}
        //    case distribution_parameter_data_type_enum::Float:   { 
        //        switch (distribution_parameters.parameter_2.data_type) {
        //            case distribution_parameter_data_type_enum::Integer : { random_number_distribution = random_number_distribution_type(distribution_parameters.parameter_1.value.float_p, distribution_parameters.parameter_2.value.integer_p); break;}
        //            case distribution_parameter_data_type_enum::Float   : { random_number_distribution = random_number_distribution_type(distribution_parameters.parameter_1.value.float_p, distribution_parameters.parameter_2.value.float_p); break;}
        //            case distribution_parameter_data_type_enum::Boolean : { random_number_distribution = random_number_distribution_type(distribution_parameters.parameter_1.value.float_p, distribution_parameters.parameter_2.value.bool_p); break;}
        //        }
        //        break; }
        //    case distribution_parameter_data_type_enum::Boolean: { 
        //        switch (distribution_parameters.parameter_2.data_type) {
        //            case distribution_parameter_data_type_enum::Integer : { random_number_distribution = random_number_distribution_type(distribution_parameters.parameter_1.value.bool_p, distribution_parameters.parameter_2.value.integer_p); break;}
        //            case distribution_parameter_data_type_enum::Float   : { random_number_distribution = random_number_distribution_type(distribution_parameters.parameter_1.value.bool_p, distribution_parameters.parameter_2.value.float_p); break;}
        //            case distribution_parameter_data_type_enum::Boolean : { random_number_distribution = random_number_distribution_type(distribution_parameters.parameter_1.value.bool_p, distribution_parameters.parameter_2.value.bool_p); break;}
        //        }
        //        break; }
        //}
    }

private:
};

// Get compiler errors for random distribution classes that only accept one input range value for the above base class which means that need to split one base class into two derived classes for each of
// one and two input distribution range parameter values despite the context of which range parameter to use is within a conditional switch statement.

template <class random_number_distribution_type, typename return_value_type>
class random_number_generation_1_range_class : public random_number_generation_base_class<random_number_distribution_type, return_value_type> {
public:
    random_number_generation_1_range_class() {}
    ~random_number_generation_1_range_class() {}
    using RNGB = random_number_generation_base_class<random_number_distribution_type, return_value_type>;
    
    void define_single_distribution_range() override {
        switch (RNGB::distribution_parameters.parameter_1.data_type) {
            case distribution_parameter_data_type_enum::Integer: { RNGB::random_number_distribution = random_number_distribution_type(RNGB::distribution_parameters.parameter_1.value.integer_p); break; }
            case distribution_parameter_data_type_enum::Float:   { RNGB::random_number_distribution = random_number_distribution_type(RNGB::distribution_parameters.parameter_1.value.float_p); break; }
            case distribution_parameter_data_type_enum::Boolean: { RNGB::random_number_distribution = random_number_distribution_type(RNGB::distribution_parameters.parameter_1.value.bool_p); break; }
        }
    }

};

template <class random_number_distribution_type, typename return_value_type>
class random_number_generation_2_range_class : public random_number_generation_base_class<random_number_distribution_type, return_value_type> {
public:
    random_number_generation_2_range_class() {}
    ~random_number_generation_2_range_class() {}
    using RNGB = random_number_generation_base_class<random_number_distribution_type, return_value_type>;

    virtual void define_dual_distribution_range() override {
        switch (RNGB::distribution_parameters.parameter_1.data_type) {// Following parameter testing not really needed, but used in case have distributions parameter values using different data types in future
            case distribution_parameter_data_type_enum::Integer: {
                switch (RNGB::distribution_parameters.parameter_2.data_type) {
                    case distribution_parameter_data_type_enum::Integer : { RNGB::random_number_distribution = random_number_distribution_type(RNGB::distribution_parameters.parameter_1.value.integer_p, RNGB::distribution_parameters.parameter_2.value.integer_p); break;}
                    case distribution_parameter_data_type_enum::Float   : { RNGB::random_number_distribution = random_number_distribution_type(RNGB::distribution_parameters.parameter_1.value.integer_p, RNGB::distribution_parameters.parameter_2.value.float_p); break;}
                    case distribution_parameter_data_type_enum::Boolean : { RNGB::random_number_distribution = random_number_distribution_type(RNGB::distribution_parameters.parameter_1.value.integer_p, RNGB::distribution_parameters.parameter_2.value.bool_p); break;}
                }
                break;}
            case distribution_parameter_data_type_enum::Float:   { 
                switch (RNGB::distribution_parameters.parameter_2.data_type) {
                    case distribution_parameter_data_type_enum::Integer : { RNGB::random_number_distribution = random_number_distribution_type(RNGB::distribution_parameters.parameter_1.value.float_p, RNGB::distribution_parameters.parameter_2.value.integer_p); break;}
                    case distribution_parameter_data_type_enum::Float   : { RNGB::random_number_distribution = random_number_distribution_type(RNGB::distribution_parameters.parameter_1.value.float_p, RNGB::distribution_parameters.parameter_2.value.float_p); break;}
                    case distribution_parameter_data_type_enum::Boolean : { RNGB::random_number_distribution = random_number_distribution_type(RNGB::distribution_parameters.parameter_1.value.float_p, RNGB::distribution_parameters.parameter_2.value.bool_p); break;}
                }
                break; }
            case distribution_parameter_data_type_enum::Boolean: { 
                switch (RNGB::distribution_parameters.parameter_2.data_type) {
                    case distribution_parameter_data_type_enum::Integer : { RNGB::random_number_distribution = random_number_distribution_type(RNGB::distribution_parameters.parameter_1.value.bool_p, RNGB::distribution_parameters.parameter_2.value.integer_p); break;}
                    case distribution_parameter_data_type_enum::Float   : { RNGB::random_number_distribution = random_number_distribution_type(RNGB::distribution_parameters.parameter_1.value.bool_p, RNGB::distribution_parameters.parameter_2.value.float_p); break;}
                    case distribution_parameter_data_type_enum::Boolean : { RNGB::random_number_distribution = random_number_distribution_type(RNGB::distribution_parameters.parameter_1.value.bool_p, RNGB::distribution_parameters.parameter_2.value.bool_p); break;}
                }
                break; }
        }
    }

};

class random_engine_class {
public:
    random_engine_class() { initialise_distribution_input_parameters(); }
    ~random_engine_class() {}

    ImGuiComboFlags random_generators_combo_flags = ImGuiComboFlags_NoArrowButton;
    ImGuiComboFlags random_distributions_combo_flags = ImGuiComboFlags_NoArrowButton;
    const char* random_generators[11] = { "default_engine", "minstd_rand", "minstd_rand0", "mt19937", "mt19937_64", "ranlux24_base", "ranlux48_base", "ranlux24", "ranlux48","knuth_b","random_device" };
    //const char* random_distributions[22] = { "Uniform_int", "Uniform_real", "bernoulli", "binomial", "geometric", "negative_binomial", "poisson", "exponential", "gamma","weibull","extreme_value","normal","lognormal","chi_sqaured","cauchy","fisher_f","student_t","discrete","piecwise_constant","piecwise_linear","seed_sequence","generate_canonical"};
    const char* random_distributions[17] = { "Uniform_int", "Uniform_real", "bernoulli", "binomial", "geometric", "negative_binomial", "poisson", "exponential", "gamma","weibull","extreme_value","normal","lognormal","chi_sqaured","cauchy","fisher_f","student_t" };

    int random_generator_index = 0;
    int random_distribution_index = 0;
    bool rng_use_seed = false;
    bool rng_add_to_existing_hex_grid = false;
    int seed_type_rb = 0;
    int machine_seed_type_rb = 0;

    int rng_seed = 0;
    float rng_min_range_float = 0.0f;
    float rng_max_range_float = 1.0f;

    int number_distribution_engines = 17;
    int number_of_generator_types = 11;

    std::vector<distribution_parameter_input_struct_type> random_distribution_input_parameters = {};

    random_number_generator_struct_type    random_number_generator;

    void display_generation_inputs() {
        ImGui::Text("--- Random Hex Grid Generation ---");

        ImGui::Checkbox("Add to Existing Grid", &rng_add_to_existing_hex_grid);

        const char* combo_random_generator = random_generators[random_generator_index];
        ImGui::SetNextItemWidth(100);
        if (ImGui::BeginCombo("Generator", combo_random_generator, random_generators_combo_flags))
        {
            for (int n = 0; n < IM_COUNTOF(random_generators); n++)
            {
                const bool is_selected = (random_generator_index == n);
                if (ImGui::Selectable(random_generators[n], is_selected)) {
                    random_generator_index = n;
                }

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected){
                    ImGui::SetItemDefaultFocus();}
            }
            ImGui::EndCombo();
        }

        ImGui::Checkbox("Use defined Seed value", &rng_use_seed);
        if (rng_use_seed) {
            ImGui::RadioButton("Use User Seed value", &seed_type_rb, USE_USER_DEFINED_SEED); ImGui::SameLine();
            ImGui::RadioButton("Use machine Seed value", &seed_type_rb, USE_MACHINE_DEFINED_SEED);

            if (seed_type_rb == USE_USER_DEFINED_SEED) {
                ImGui::SetNextItemWidth(60);
                ImGui::DragInt("Seed value", &rng_seed, 1, 0, 1000, "%5i");
            }

            if (seed_type_rb == USE_MACHINE_DEFINED_SEED) {
                ImGui::RadioButton("Time based", &machine_seed_type_rb, USE_TIME_DEFINED_SEED); ImGui::SameLine();
                ImGui::RadioButton("Random device", &machine_seed_type_rb, USE_DEVICE_DEFINED_SEED);
            }
        }

        ImGui::NewLine();

        const char* combo_random_distribution = random_distributions[random_distribution_index];
        ImGui::SetNextItemWidth(100);
        if (ImGui::BeginCombo("Distribution", combo_random_distribution, random_distributions_combo_flags))
        {
            for (int n = 0; n < IM_COUNTOF(random_distributions); n++)
            {
                const bool is_selected = (random_distribution_index == n);
                if (ImGui::Selectable(random_distributions[n], is_selected)) {
                    random_distribution_index = n;
                }

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        // !!!!! CRITICAL :: Order and index of random_distribution_input_parameters vector list must match that of the random_distributions array list
        ImGui::Text("Set Parameters for Chosen Distribution");
        if (random_distribution_index >= 0 && random_distribution_index < random_distribution_input_parameters.size()) {
            distribution_parameter_input_struct_type &dpi = random_distribution_input_parameters[random_distribution_index];

            if (dpi.input_type == distribution_parameter_input_type_enum::single) {
                if (dpi.parameter_1.data_type == distribution_parameter_data_type_enum::Integer) {
                    ImGui::SetNextItemWidth(100);
                    ImGui::DragInt(dpi.parameter_1.parameter_text.c_str(), &dpi.parameter_1.value.integer_p, 1, 0, 10000, "%4i");
                }
                else {
                    ImGui::SetNextItemWidth(100);
                    ImGui::DragFloat(dpi.parameter_1.parameter_text.c_str(), &dpi.parameter_1.value.float_p, 1.0f, -1.0f, 1000.0f, "%3.1f");
                }
            }
            else {
                if (dpi.parameter_1.data_type == distribution_parameter_data_type_enum::Integer) {
                    ImGui::SetNextItemWidth(100);
                    ImGui::DragInt(dpi.parameter_1.parameter_text.c_str(), &dpi.parameter_1.value.integer_p, 1, 0, 10000, "%4i");
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100);
                    ImGui::DragInt(dpi.parameter_2.parameter_text.c_str(), &dpi.parameter_2.value.integer_p, 1, 0, 10000, "%4i");
                }
                else {
                    ImGui::SetNextItemWidth(100);
                    ImGui::DragFloat(dpi.parameter_1.parameter_text.c_str(), &dpi.parameter_1.value.float_p, 1.0f, -1.0f, 1000.0f, "%3.1f");
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100);
                    ImGui::DragFloat(dpi.parameter_2.parameter_text.c_str(), &dpi.parameter_2.value.float_p, 1.0f, -1.0f, 1000.0f, "%3.1f");
                }
            }
        }
    }

    void generate_random_value() {
        define_random_engine();

        // This is for testing 
        //for (int i = 0; i < 20; i++) {
        //    uniform_int_distribution.generate_random_number();
        //    //printf("random_engine_class : generate_random_value gen_number : %i\n", uniform_int_distribution.generated_number_value.integer_p);
        //    printf("random_engine_class : generate_random_value gen_number : %i\n", uniform_int_distribution.generated_number_value);
        //}

    }

    void define_random_engine() {
        if (random_generator_index < 0 || random_generator_index >= IM_COUNTOF(random_generators)) {
            // error message here
        }

        define_random_number_generatior_engine_to_use();
        define_random_number_generator_engine_seed_value();

        random_number_generator.define_random_generator_engine_to_use();

        define_random_number_generator_distribution();
    }

    void define_random_number_generator_engine_seed_value() {
        random_number_generator.use_default_seed = !rng_use_seed;// Not user defined seed value

        if (rng_use_seed) {
            if (seed_type_rb == USE_USER_DEFINED_SEED) {
                random_number_generator.seed_value = rng_seed;
                return;
            }

            if (seed_type_rb == USE_MACHINE_DEFINED_SEED) {
                if (machine_seed_type_rb == USE_TIME_DEFINED_SEED) {
                    random_number_generator.seed_value = std::chrono::high_resolution_clock::now().time_since_epoch().count();
                }
                else {
                    std::random_device rd;
                    std::mt19937 gen(rd());

                    random_number_generator.seed_value = gen();
                }
                return;
            }
        }
    }

    // CRITICAL : The random_number_generator.number_generation_engine assignment must match the random_generator_index value that
    //            the user has selected in the random_generators array list used in the ImGui combo widget
    void define_random_number_generatior_engine_to_use() {
        switch (random_generator_index) {
            case 0: { random_number_generator.number_generation_engine = number_generation_engine_enum::default_engine; break; }
            case 1: { random_number_generator.number_generation_engine = number_generation_engine_enum::minstd_rand; break; }
            case 2: { random_number_generator.number_generation_engine = number_generation_engine_enum::minstd_rand0; break; }
            case 3: { random_number_generator.number_generation_engine = number_generation_engine_enum::mt19937; break; }
            case 4: { random_number_generator.number_generation_engine = number_generation_engine_enum::mt19937_64; break; }
            case 5: { random_number_generator.number_generation_engine = number_generation_engine_enum::ranlux24_base; break; }
            case 6: { random_number_generator.number_generation_engine = number_generation_engine_enum::ranlux48_base; break; }
            case 7: { random_number_generator.number_generation_engine = number_generation_engine_enum::ranlux24; break; }
            case 8: { random_number_generator.number_generation_engine = number_generation_engine_enum::ranlux48; break; }
            case 9: { random_number_generator.number_generation_engine = number_generation_engine_enum::knuth_b; break; }
            //case 10: { random_number_generator.number_generation_engine = number_generation_engine_enum::random_device; break; } // Using random device creates compile errors cannot use
        }
    }

    // CRITICAL : The random_distribution_index assignment must match the index of the random_distributions array list that
    //            the user has selected in the ImGui combo widget of random number distributions.
    void define_random_number_generator_distribution() {
        switch (random_distribution_index) {
            case 0:  { uniform_int_distribution.define_random_number_generation(random_number_generator, random_distribution_input_parameters[random_distribution_index]); break; }
            case 1:  { uniform_real_distribution.define_random_number_generation(random_number_generator, random_distribution_input_parameters[random_distribution_index]); break; }
            case 2:  { bernoulli_distribution.define_random_number_generation(random_number_generator, random_distribution_input_parameters[random_distribution_index]); break; }
            case 3:  { binomial_distribution.define_random_number_generation(random_number_generator, random_distribution_input_parameters[random_distribution_index]); break; }
            case 4:  { geometric_distribution.define_random_number_generation(random_number_generator, random_distribution_input_parameters[random_distribution_index]); break; }
            case 5:  { negative_binomial_distribution.define_random_number_generation(random_number_generator, random_distribution_input_parameters[random_distribution_index]); break; }
            case 6:  { poisson_distribution.define_random_number_generation(random_number_generator, random_distribution_input_parameters[random_distribution_index]); break; }
            case 7:  { exponential_distribution.define_random_number_generation(random_number_generator, random_distribution_input_parameters[random_distribution_index]); break; }
            case 8:  { gamma_distribution.define_random_number_generation(random_number_generator, random_distribution_input_parameters[random_distribution_index]); break; }
            case 9:  { weibull_distribution.define_random_number_generation(random_number_generator, random_distribution_input_parameters[random_distribution_index]); break; }
            case 10: { extreme_value_distribution.define_random_number_generation(random_number_generator, random_distribution_input_parameters[random_distribution_index]); break; }
            case 11: { normal_distribution.define_random_number_generation(random_number_generator, random_distribution_input_parameters[random_distribution_index]); break; }
            case 12: { lognormal_distribution.define_random_number_generation(random_number_generator, random_distribution_input_parameters[random_distribution_index]); break; }
            case 13: { chi_squared_distribution.define_random_number_generation(random_number_generator, random_distribution_input_parameters[random_distribution_index]); break; }
            case 14: { cauchy_distribution.define_random_number_generation(random_number_generator, random_distribution_input_parameters[random_distribution_index]); break; }
            case 15: { fisher_f_distribution.define_random_number_generation(random_number_generator, random_distribution_input_parameters[random_distribution_index]); break; }
            case 16: { student_t_distribution.define_random_number_generation(random_number_generator, random_distribution_input_parameters[random_distribution_index]); break; }
        }
    }
 
    // -----------------
    // CRTICAL :: The order of this initialisation must correspond to the order of the random_distributions array entries
    void initialise_distribution_input_parameters() {
        //Uniform distributions
        random_distribution_input_parameters.push_back(define_Uniform_int_distribution());
        random_distribution_input_parameters.push_back(define_Uniform_real_distribution());
        //Related to Bernoulli (yes/no) trials distributions
        random_distribution_input_parameters.push_back(define_bernoulli_distribution());
        random_distribution_input_parameters.push_back(define_binomial_distribution());
        random_distribution_input_parameters.push_back(define_geometric_distribution());
        random_distribution_input_parameters.push_back(define_negative_binomial_distribution());
        //Rate - based distributions :
        random_distribution_input_parameters.push_back(define_poisson_distribution());
        random_distribution_input_parameters.push_back(define_exponential_distribution());
        random_distribution_input_parameters.push_back(define_gamma_distribution());
        random_distribution_input_parameters.push_back(define_weibull_distribution());
        random_distribution_input_parameters.push_back(define_extreme_value_distribution());
        //Related to Normal distribution:
        random_distribution_input_parameters.push_back(define_normal_distribution());
        random_distribution_input_parameters.push_back(define_lognormal_distribution());
        random_distribution_input_parameters.push_back(define_chi_squared_distribution());
        random_distribution_input_parameters.push_back(define_cauchy_distribution());
        random_distribution_input_parameters.push_back(define_fisher_f_distribution());
        random_distribution_input_parameters.push_back(define_student_t_distribution());

        number_distribution_engines = random_distribution_input_parameters.size();
    }

    // ************ RANDOM NUMBER DISTRIBUTION DEFINITIONS *******************
    // -------------------- UNIFORM DISTRIBUTIONS ----------------------------
    random_number_generation_2_range_class<std::uniform_int_distribution<int>, int>        uniform_int_distribution;
    random_number_generation_2_range_class<std::uniform_real_distribution<double>, float>  uniform_real_distribution;
    
    // -------------------- BERNOULI RELATED DISTRIBUTIONS ----------------------------
    random_number_generation_1_range_class<std::bernoulli_distribution, bool>              bernoulli_distribution;
    random_number_generation_2_range_class<std::binomial_distribution<int>, int>           binomial_distribution;
    random_number_generation_1_range_class<std::geometric_distribution<int>, int>          geometric_distribution;
    random_number_generation_2_range_class<std::negative_binomial_distribution<int>, int>  negative_binomial_distribution;
    //
    //// -------------------- RATE BASED DISTRIBUTIONS ----------------------------
    random_number_generation_1_range_class<std::poisson_distribution<int>, int>            poisson_distribution;
    random_number_generation_1_range_class<std::exponential_distribution<double>, int>     exponential_distribution;
    random_number_generation_2_range_class<std::gamma_distribution<double>, float>         gamma_distribution;
    random_number_generation_2_range_class<std::weibull_distribution<double>, float>       weibull_distribution;
    random_number_generation_2_range_class<std::extreme_value_distribution<double>, float> extreme_value_distribution;
    //
    //// ------------------ NORMAL RELATED DISTRIBUTIONS ---------------------------
    random_number_generation_2_range_class<std::normal_distribution<double>, float>        normal_distribution;
    random_number_generation_2_range_class<std::lognormal_distribution<double>, float>     lognormal_distribution;
    random_number_generation_1_range_class<std::chi_squared_distribution<double>, float>   chi_squared_distribution;
    random_number_generation_2_range_class<std::cauchy_distribution<double>, float>        cauchy_distribution;
    random_number_generation_2_range_class<std::fisher_f_distribution<double>, float>      fisher_f_distribution;
    random_number_generation_1_range_class<std::student_t_distribution<double>, float>     student_t_distribution;
    
    // -----------------------------------------------------------------------

    // ******************** DISTRIBUTION INPUT PARAMETERS ********************

    // -------------------- UNIFORM DISTRIBUTIONS ----------------------------
    distribution_parameter_input_struct_type define_Uniform_int_distribution() {
        distribution_parameter_input_struct_type input_parameter;

        input_parameter.distribution_category = distribution_category_enum::Uniform;
        input_parameter.distribution_engine = distribution_engine_enum::Uniform_int;
        input_parameter.input_type = distribution_parameter_input_type_enum::dual;
        input_parameter.output_data_type = distribution_parameter_data_type_enum::Integer;

        input_parameter.parameter_1.parameter_text = "Minimum";
        input_parameter.parameter_1.data_type = distribution_parameter_data_type_enum::Integer;
        input_parameter.parameter_1.value.integer_p = 0;
        input_parameter.parameter_2.parameter_text = "Maximum";
        input_parameter.parameter_2.data_type = distribution_parameter_data_type_enum::Integer;
        input_parameter.parameter_2.value.integer_p = 1;

        return input_parameter;
    }

    distribution_parameter_input_struct_type define_Uniform_real_distribution() {
        distribution_parameter_input_struct_type input_parameter;

        input_parameter.distribution_category = distribution_category_enum::Uniform;
        input_parameter.distribution_engine = distribution_engine_enum::Uniform_real;
        input_parameter.input_type = distribution_parameter_input_type_enum::dual;
        input_parameter.output_data_type = distribution_parameter_data_type_enum::Float;

        input_parameter.parameter_1.parameter_text = "Minimum";
        input_parameter.parameter_1.data_type = distribution_parameter_data_type_enum::Float;
        input_parameter.parameter_1.value.float_p = 0.0f;
        input_parameter.parameter_2.parameter_text = "Maximum";
        input_parameter.parameter_2.data_type = distribution_parameter_data_type_enum::Float;
        input_parameter.parameter_2.value.float_p = 1.0f;

        return input_parameter;
    }

    // -------------------- BERNOULI RELATED DISTRIBUTIONS ----------------------------

    distribution_parameter_input_struct_type define_bernoulli_distribution() {
        distribution_parameter_input_struct_type input_parameter;

        input_parameter.distribution_category = distribution_category_enum::Bernouli_related;
        input_parameter.distribution_engine = distribution_engine_enum::bernoulli;
        input_parameter.input_type = distribution_parameter_input_type_enum::single;
        input_parameter.output_data_type = distribution_parameter_data_type_enum::Boolean;

        input_parameter.parameter_1.parameter_text = "Probability of true";
        input_parameter.parameter_1.data_type = distribution_parameter_data_type_enum::Float;
        input_parameter.parameter_1.value.float_p = 0.5f;

        return input_parameter;
    }

    distribution_parameter_input_struct_type define_binomial_distribution() {
        distribution_parameter_input_struct_type input_parameter;

        input_parameter.distribution_category = distribution_category_enum::Bernouli_related;
        input_parameter.distribution_engine = distribution_engine_enum::binomial;
        input_parameter.input_type = distribution_parameter_input_type_enum::dual;
        input_parameter.output_data_type = distribution_parameter_data_type_enum::Integer;

        input_parameter.parameter_1.parameter_text = "Upper bound";
        input_parameter.parameter_1.data_type = distribution_parameter_data_type_enum::Integer;
        input_parameter.parameter_1.value.integer_p = 0;
        input_parameter.parameter_2.parameter_text = "Probability of success";
        input_parameter.parameter_2.data_type = distribution_parameter_data_type_enum::Float;
        input_parameter.parameter_2.value.float_p = 1.0f;

        return input_parameter;
    }

    distribution_parameter_input_struct_type define_geometric_distribution() {
        distribution_parameter_input_struct_type input_parameter;

        input_parameter.distribution_category = distribution_category_enum::Bernouli_related;
        input_parameter.distribution_engine = distribution_engine_enum::geometric;
        input_parameter.input_type = distribution_parameter_input_type_enum::single;
        input_parameter.output_data_type = distribution_parameter_data_type_enum::Integer;

        input_parameter.parameter_1.parameter_text = "Probability of Success";
        input_parameter.parameter_1.data_type = distribution_parameter_data_type_enum::Float;
        input_parameter.parameter_1.value.float_p = 0.5f;

        return input_parameter;
    }

    distribution_parameter_input_struct_type define_negative_binomial_distribution() {
        distribution_parameter_input_struct_type input_parameter;

        input_parameter.distribution_category = distribution_category_enum::Bernouli_related;
        input_parameter.distribution_engine = distribution_engine_enum::negative_binomial;
        input_parameter.input_type = distribution_parameter_input_type_enum::dual;
        input_parameter.output_data_type = distribution_parameter_data_type_enum::Integer;

        input_parameter.parameter_1.parameter_text = "Paramater k";
        input_parameter.parameter_1.data_type = distribution_parameter_data_type_enum::Integer;
        input_parameter.parameter_1.value.integer_p = 0;
        input_parameter.parameter_2.parameter_text = "Parameter p";
        input_parameter.parameter_2.data_type = distribution_parameter_data_type_enum::Float;
        input_parameter.parameter_2.value.float_p = 1.0f;

        return input_parameter;
    }

    // -------------------- RATE BASED DISTRIBUTIONS ----------------------------
    distribution_parameter_input_struct_type define_poisson_distribution() {
        distribution_parameter_input_struct_type input_parameter;

        input_parameter.distribution_category = distribution_category_enum::Rate_based;
        input_parameter.distribution_engine = distribution_engine_enum::poisson;
        input_parameter.input_type = distribution_parameter_input_type_enum::single;
        input_parameter.output_data_type = distribution_parameter_data_type_enum::Integer;

        input_parameter.parameter_1.parameter_text = "Mean";
        input_parameter.parameter_1.data_type = distribution_parameter_data_type_enum::Float;
        input_parameter.parameter_1.value.float_p = 0.5f;

        return input_parameter;
    }

    distribution_parameter_input_struct_type define_exponential_distribution() {
        distribution_parameter_input_struct_type input_parameter;

        input_parameter.distribution_category = distribution_category_enum::Rate_based;
        input_parameter.distribution_engine = distribution_engine_enum::exponential;
        input_parameter.input_type = distribution_parameter_input_type_enum::single;
        input_parameter.output_data_type = distribution_parameter_data_type_enum::Float;

        input_parameter.parameter_1.parameter_text = "Lambda";
        input_parameter.parameter_1.data_type = distribution_parameter_data_type_enum::Float;
        input_parameter.parameter_1.value.float_p = 0.5f;

        return input_parameter;
    }

    distribution_parameter_input_struct_type define_gamma_distribution() {
        distribution_parameter_input_struct_type input_parameter;

        input_parameter.distribution_category = distribution_category_enum::Rate_based;
        input_parameter.distribution_engine = distribution_engine_enum::gamma;
        input_parameter.input_type = distribution_parameter_input_type_enum::dual;
        input_parameter.output_data_type = distribution_parameter_data_type_enum::Float;

        input_parameter.parameter_1.parameter_text = "Alpha";
        input_parameter.parameter_1.data_type = distribution_parameter_data_type_enum::Float;
        input_parameter.parameter_1.value.float_p = 0.0f;
        input_parameter.parameter_2.parameter_text = "Beta";
        input_parameter.parameter_2.data_type = distribution_parameter_data_type_enum::Float;
        input_parameter.parameter_2.value.float_p = 1.0f;

        return input_parameter;
    }

    distribution_parameter_input_struct_type define_weibull_distribution() {
        distribution_parameter_input_struct_type input_parameter;

        input_parameter.distribution_category = distribution_category_enum::Rate_based;
        input_parameter.distribution_engine = distribution_engine_enum::weibull;
        input_parameter.input_type = distribution_parameter_input_type_enum::dual;
        input_parameter.output_data_type = distribution_parameter_data_type_enum::Float;

        input_parameter.parameter_1.parameter_text = "Paramater a";
        input_parameter.parameter_1.data_type = distribution_parameter_data_type_enum::Float;
        input_parameter.parameter_1.value.float_p = 0.0f;
        input_parameter.parameter_2.parameter_text = "Paramater b";
        input_parameter.parameter_2.data_type = distribution_parameter_data_type_enum::Float;
        input_parameter.parameter_2.value.float_p = 1.0f;

        return input_parameter;
    }

    distribution_parameter_input_struct_type define_extreme_value_distribution() {
        distribution_parameter_input_struct_type input_parameter;

        input_parameter.distribution_category = distribution_category_enum::Rate_based;
        input_parameter.distribution_engine = distribution_engine_enum::extreme_value;
        input_parameter.input_type = distribution_parameter_input_type_enum::dual;
        input_parameter.output_data_type = distribution_parameter_data_type_enum::Float;

        input_parameter.parameter_1.parameter_text = "Paramater a";
        input_parameter.parameter_1.data_type = distribution_parameter_data_type_enum::Float;
        input_parameter.parameter_1.value.float_p = 0.0f;
        input_parameter.parameter_2.parameter_text = "Paramater b";
        input_parameter.parameter_2.data_type = distribution_parameter_data_type_enum::Float;
        input_parameter.parameter_2.value.float_p = 1.0f;

        return input_parameter;
    }

    // ------------------ NORMAL RELATED DISTRIBUTIONS ---------------------------

    distribution_parameter_input_struct_type define_normal_distribution() {
        distribution_parameter_input_struct_type input_parameter;

        input_parameter.distribution_category = distribution_category_enum::Normal_related;
        input_parameter.distribution_engine = distribution_engine_enum::normal;
        input_parameter.input_type = distribution_parameter_input_type_enum::dual;
        input_parameter.output_data_type = distribution_parameter_data_type_enum::Float;

        input_parameter.parameter_1.parameter_text = "Mean";
        input_parameter.parameter_1.data_type = distribution_parameter_data_type_enum::Float;
        input_parameter.parameter_1.value.float_p = 0.0f;
        input_parameter.parameter_2.parameter_text = "Standard deviation";
        input_parameter.parameter_2.data_type = distribution_parameter_data_type_enum::Float;
        input_parameter.parameter_2.value.float_p = 1.0f;

        return input_parameter;
    }

    distribution_parameter_input_struct_type define_lognormal_distribution() {
        distribution_parameter_input_struct_type input_parameter;

        input_parameter.distribution_category = distribution_category_enum::Normal_related;
        input_parameter.distribution_engine = distribution_engine_enum::lognormal;
        input_parameter.input_type = distribution_parameter_input_type_enum::dual;
        input_parameter.output_data_type = distribution_parameter_data_type_enum::Float;

        input_parameter.parameter_1.parameter_text = "Mean";
        input_parameter.parameter_1.data_type = distribution_parameter_data_type_enum::Float;
        input_parameter.parameter_1.value.float_p = 0.0f;
        input_parameter.parameter_2.parameter_text = "Standard deviation";
        input_parameter.parameter_2.data_type = distribution_parameter_data_type_enum::Float;
        input_parameter.parameter_2.value.float_p = 1.0f;

        return input_parameter;
    }

    distribution_parameter_input_struct_type define_chi_squared_distribution() {
        distribution_parameter_input_struct_type input_parameter;

        input_parameter.distribution_category = distribution_category_enum::Normal_related;
        input_parameter.distribution_engine = distribution_engine_enum::chi_sqaured;
        input_parameter.input_type = distribution_parameter_input_type_enum::single;
        input_parameter.output_data_type = distribution_parameter_data_type_enum::Float;

        input_parameter.parameter_1.parameter_text = "Degrees of freedom";
        input_parameter.parameter_1.data_type = distribution_parameter_data_type_enum::Float;
        input_parameter.parameter_1.value.float_p = 0.5f;

        return input_parameter;
    }

    distribution_parameter_input_struct_type define_cauchy_distribution() {
        distribution_parameter_input_struct_type input_parameter;

        input_parameter.distribution_category = distribution_category_enum::Normal_related;
        input_parameter.distribution_engine = distribution_engine_enum::cauchy;
        input_parameter.input_type = distribution_parameter_input_type_enum::dual;
        input_parameter.output_data_type = distribution_parameter_data_type_enum::Float;

        input_parameter.parameter_1.parameter_text = "Parameter a";
        input_parameter.parameter_1.data_type = distribution_parameter_data_type_enum::Float;
        input_parameter.parameter_1.value.float_p = 0.0f;
        input_parameter.parameter_2.parameter_text = "Parameter b";
        input_parameter.parameter_2.data_type = distribution_parameter_data_type_enum::Float;
        input_parameter.parameter_2.value.float_p = 1.0f;

        return input_parameter;
    }

    distribution_parameter_input_struct_type define_fisher_f_distribution() {
        distribution_parameter_input_struct_type input_parameter;

        input_parameter.distribution_category = distribution_category_enum::Normal_related;
        input_parameter.distribution_engine = distribution_engine_enum::fisher_f;
        input_parameter.input_type = distribution_parameter_input_type_enum::dual;
        input_parameter.output_data_type = distribution_parameter_data_type_enum::Float;

        input_parameter.parameter_1.parameter_text = "Degrees of freedom m";
        input_parameter.parameter_1.data_type = distribution_parameter_data_type_enum::Float;
        input_parameter.parameter_1.value.float_p = 0.0f;
        input_parameter.parameter_2.parameter_text = "Degrees of freedom n";
        input_parameter.parameter_2.data_type = distribution_parameter_data_type_enum::Float;
        input_parameter.parameter_2.value.float_p = 1.0f;

        return input_parameter;
    }

    distribution_parameter_input_struct_type define_student_t_distribution() {
        distribution_parameter_input_struct_type input_parameter;

        input_parameter.distribution_category = distribution_category_enum::Normal_related;
        input_parameter.distribution_engine = distribution_engine_enum::student_t;
        input_parameter.input_type = distribution_parameter_input_type_enum::single;
        input_parameter.output_data_type = distribution_parameter_data_type_enum::Float;

        input_parameter.parameter_1.parameter_text = "Degrees of freedom";
        input_parameter.parameter_1.data_type = distribution_parameter_data_type_enum::Float;
        input_parameter.parameter_1.value.float_p = 0.5f;

        return input_parameter;
    }


protected:

private:

};

