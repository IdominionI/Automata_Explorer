#pragma once

//#include <utility>
//#include <string>
//#include <vector>
//
//#include <math.h>

#include "parser_definitions.h"

#include "../Hexagon/Objects/hex_grid_base.h"

//bool Not(bool value) {
//    return !value;
//}
//
//bool And(bool a, bool b) {
//    return a && b;
//}
//
//bool Or(bool a, bool b) {
//    return a || b;
//}

// ###### HEXAGONAL GRID CELL PARSER TOKEN DEFINITIONS ######

#define HEX_GRID_NEIGHBOUR_H0  "H0"
#define HEX_GRID_NEIGHBOUR_H1  "H1"
#define HEX_GRID_NEIGHBOUR_H2  "H2"
#define HEX_GRID_NEIGHBOUR_H3  "H3"
#define HEX_GRID_NEIGHBOUR_H4  "H4"
#define HEX_GRID_NEIGHBOUR_H5  "H5"
#define HEX_GRID_NEIGHBOUR_H6  "H6"

// Function class to perform all parser function operations and assign
// a value of the result to the parser value storage container.

// General method of function is to test which operator token is present
// for the type of operator that is being evaluated and perform the operation
// on the relevant data values for that operator.

class functions_class {
public:

    // Perform binary operatoe evaluation of two values of a paser tree node
    // and assign the result to the parser tree node value storage container.

    // General method of function is to get the left and right values of the binary
    // operator and test if either is not a number (nan) or not. If both are nan
    // assign the result as nan. If one is nan then perform a nan operation for that
    // particular operator. Otherwise perform the binary operation on the left and right
    // data values and assign the result  to the parser value storage container.

    bool perform_binary_operator_function(binary_node* bn) {
        std::string binary_function_token = bn->function_token_definition.second;

//if (bn->_leftExpression->value.first == parse_value_data_type_enum::nan) {
//    printf("functions :: perform_binary_operator_function 00: :: bn->_leftExpression.value.first == parse_value_data_type_enum::nan %s :: \n", bn->_leftExpression->function_token_definition.second.c_str());// testing only : delete comment out when not needed
//}
        //++++++++++++
        bool left_expression_nan = false;
        bool right_expression_nan = false;

        if (bn->_leftExpression->value.first == parse_value_data_type_enum::nan) { left_expression_nan = true; }
        if (bn->_rightExpression->value.first == parse_value_data_type_enum::nan) { right_expression_nan = true; }

        if (bn->_leftExpression->value.first == parse_value_data_type_enum::nan && bn->_rightExpression->value.first == parse_value_data_type_enum::nan) {
//printf("perform_binary_operator_function ::binary_function_token both value are NAN !!!! \n");
            bn->value.first = parse_value_data_type_enum::nan;
           // return false; // cannot perform binary operation
            return true; // cannot perform binary operation +++++++
        }
        //++++++++++++

        // ---------------
        //if (bn->_leftExpression->value.first == parse_value_data_type_enum::nan || bn->_rightExpression->value.first == parse_value_data_type_enum::nan) {
//printf("perform_binary_operator_function ::binary_function_token == LOGIC_OPERATOR_AND :: value is NAN !!!! \n");
        //    return false; // cannot perform binary operation
        //}
        // ---------------

        // ############## Logical binary functions ###################

        if (binary_function_token == LOGIC_OPERATOR_AND) {
//printf("perform_binary_operator_function ::binary_function_token == LOGIC_OPERATOR_AND 00000 #### !!!! \n");

            bn->value.first = parse_value_data_type_enum::Boolean;

            //++++++++++++
            if (left_expression_nan ) {
                bn->value.second.bvalue = bn->_rightExpression->value.second.bvalue;
                return true;
            }

            if (right_expression_nan) {
                bn->value.second.bvalue = bn->_leftExpression->value.second.bvalue;
                return true;
            }
            //++++++++++++

            bn->value.second.bvalue = bn->_leftExpression->value.second.bvalue && bn->_rightExpression->value.second.bvalue;
//if(bn->value.second.bvalue)
//printf("perform_binary_operator_function ::binary_function_token == LOGIC_OPERATOR_AND 11111 #### !!!! bn->value.second.bvalue == true\n");
//else
//printf("perform_binary_operator_function ::binary_function_token == LOGIC_OPERATOR_AND 11111 #### !!!! bn->value.second.bvalue == false\n");
            return true;
        }

        if (binary_function_token == LOGIC_OPERATOR_OR) {
            bn->value.first = parse_value_data_type_enum::Boolean;

            //++++++++++++
            if (left_expression_nan) {
                bn->value.second.bvalue = bn->_rightExpression->value.second.bvalue;
                return true;
            }

            if (right_expression_nan) {
                bn->value.second.bvalue = bn->_leftExpression->value.second.bvalue;
                return true;
            }
            //++++++++++++

            bn->value.second.bvalue = bn->_leftExpression->value.second.bvalue || bn->_rightExpression->value.second.bvalue;
            return true;
        }

        // ############## Comparison binary functions ###################

        if (binary_function_token == LOGIC_OPERATOR_EQUAL) {
            bn->value.first = parse_value_data_type_enum::Boolean;

            // ++++++++++++
            if (left_expression_nan || right_expression_nan) {
 //printf("perform_binary_operator_function ::binary_function_token LOGIC_OPERATOR_EQUAL have NAN !!!! \n");
                bn->value.second.bvalue = false;
                return true; // cannot perform binary operation
            }
            // ++++++++++++


            switch (bn->value.first) {
                case parse_value_data_type_enum::Boolean: {
                    bn->value.second.bvalue = (bn->_leftExpression->value.second.bvalue == bn->_rightExpression->value.second.bvalue);
                    return true;
                    break;
                }
                case parse_value_data_type_enum::Float: {
                    bn->value.second.bvalue = (bn->_leftExpression->value.second.fvalue == bn->_rightExpression->value.second.fvalue);
                    return true;
                    break;
                }
                case parse_value_data_type_enum::Integer: {
                    bn->value.second.bvalue = (bn->_leftExpression->value.second.ivalue == bn->_rightExpression->value.second.ivalue);
                    return true;
                    break;
                }
                case parse_value_data_type_enum::Double: {
                    bn->value.second.bvalue = (bn->_leftExpression->value.second.dvalue == bn->_rightExpression->value.second.dvalue);
                    return true;
                    break;
                }
            }
        }

        if (binary_function_token == "!=") {
            bn->value.first = parse_value_data_type_enum::Boolean;

            // ++++++++++++
            if (left_expression_nan || right_expression_nan) {
 //printf("perform_binary_operator_function ::binary_function_token != have NAN !!!! \n");
                bn->value.second.bvalue = true;
                return true; // cannot perform binary operation
            }
            // ++++++++++++

            switch (bn->value.first) {
                case parse_value_data_type_enum::Boolean: {
                    bn->value.second.bvalue = (bn->_leftExpression->value.second.bvalue != bn->_rightExpression->value.second.bvalue);
                    return true;
                    break;
                }
                case parse_value_data_type_enum::Float: {
                    bn->value.second.bvalue = (bn->_leftExpression->value.second.fvalue != bn->_rightExpression->value.second.fvalue);
                    return true;
                    break;
                }
                case parse_value_data_type_enum::Integer: {
                    bn->value.second.bvalue = (bn->_leftExpression->value.second.ivalue != bn->_rightExpression->value.second.ivalue);
                    return true;
                    break;
                }
                case parse_value_data_type_enum::Double: {
                    bn->value.second.bvalue = (bn->_leftExpression->value.second.dvalue != bn->_rightExpression->value.second.dvalue);
                    return true;
                    break;
                }
            }
        }

        if (binary_function_token == std::string(1, COMPARISON_OPERATOR_LESS_THAN)) {
            bn->value.first = parse_value_data_type_enum::Boolean;

            // ++++++++++++
            if (left_expression_nan || right_expression_nan) {
 //printf("perform_binary_operator_function ::binary_function_token COMPARISON_OPERATOR_LESS_THAN have NAN !!!! \n");
                bn->value.second.bvalue = false;
                return true; // cannot perform binary operation
            }
            // ++++++++++++

            switch (bn->value.first) {
                case parse_value_data_type_enum::Boolean: {
                    bn->value.second.bvalue = (bn->_leftExpression->value.second.bvalue < bn->_rightExpression->value.second.bvalue);
                    return true;
                    break;
                }
                case parse_value_data_type_enum::Float: {
                    bn->value.second.bvalue = (bn->_leftExpression->value.second.fvalue < bn->_rightExpression->value.second.fvalue);
                    return true;
                    break;
                }
                case parse_value_data_type_enum::Integer: {
                    bn->value.second.bvalue = (bn->_leftExpression->value.second.ivalue < bn->_rightExpression->value.second.ivalue);
                    return true;
                    break;
                }
                case parse_value_data_type_enum::Double: {
                    bn->value.second.bvalue = (bn->_leftExpression->value.second.dvalue < bn->_rightExpression->value.second.dvalue);
                    return true;
                    break;
                }
            }
        }

        if (binary_function_token == std::string(1, COMPARISON_OPERATOR_GREATER_THAN)) {
            bn->value.first = parse_value_data_type_enum::Boolean;

            // ++++++++++++
            if (left_expression_nan || right_expression_nan) {
 //printf("perform_binary_operator_function ::binary_function_token COMPARISON_OPERATOR_GREATER_THAN have NAN !!!! \n");
                bn->value.second.bvalue = false;
                return true; // cannot perform binary operation
            }
            // ++++++++++++

            switch (bn->value.first) {
                case parse_value_data_type_enum::Boolean: {
                    bn->value.second.bvalue = (bn->_leftExpression->value.second.bvalue > bn->_rightExpression->value.second.bvalue);
                    return true;
                    break;
                }
                case parse_value_data_type_enum::Float: {
                    bn->value.second.bvalue = (bn->_leftExpression->value.second.fvalue > bn->_rightExpression->value.second.fvalue);
                    return true;
                    break;
                }
                case parse_value_data_type_enum::Integer: {
                    bn->value.second.bvalue = (bn->_leftExpression->value.second.ivalue > bn->_rightExpression->value.second.ivalue);
                    return true;
                    break;
                }
                case parse_value_data_type_enum::Double: {
                    bn->value.second.bvalue = (bn->_leftExpression->value.second.dvalue > bn->_rightExpression->value.second.dvalue);
                    return true;
                    break;
                }
            }
        }

        if (binary_function_token == "<=") {
            bn->value.first = parse_value_data_type_enum::Boolean;

            // ++++++++++++
            if (left_expression_nan || right_expression_nan) {
// printf("perform_binary_operator_function ::binary_function_token <= have NAN !!!! \n");
                bn->value.second.bvalue = false;
                return true; // cannot perform binary operation
            }
            // ++++++++++++

            switch (bn->value.first) {
                case parse_value_data_type_enum::Boolean: {
                    bn->value.second.bvalue = (bn->_leftExpression->value.second.bvalue <= bn->_rightExpression->value.second.bvalue);
                    return true;
                    break;
                }
                case parse_value_data_type_enum::Float: {
                    bn->value.second.bvalue = (bn->_leftExpression->value.second.fvalue <= bn->_rightExpression->value.second.fvalue);
                    return true;
                    break;
                }
                case parse_value_data_type_enum::Integer: {
                    bn->value.second.bvalue = (bn->_leftExpression->value.second.ivalue <= bn->_rightExpression->value.second.ivalue);
                    return true;
                    break;
                }
                case parse_value_data_type_enum::Double: {
                    bn->value.second.bvalue = (bn->_leftExpression->value.second.dvalue <= bn->_rightExpression->value.second.dvalue);
                    return true;
                    break;
                }
            }
        }

        if (binary_function_token == ">=") {
            bn->value.first = parse_value_data_type_enum::Boolean;

            // ++++++++++++
            if (left_expression_nan || right_expression_nan) {
 //printf("perform_binary_operator_function ::binary_function_token >= have NAN !!!! \n");
                bn->value.second.bvalue = false;
                return true; // cannot perform binary operation
            }
            // ++++++++++++

            switch (bn->value.first) {
                case parse_value_data_type_enum::Boolean: {
                    bn->value.second.bvalue = (bn->_leftExpression->value.second.bvalue >= bn->_rightExpression->value.second.bvalue);
                    return true;
                    break;
                }
                case parse_value_data_type_enum::Float: {
                    bn->value.second.bvalue = (bn->_leftExpression->value.second.fvalue >= bn->_rightExpression->value.second.fvalue);
                    return true;
                    break;
                }
                case parse_value_data_type_enum::Integer: {
                    bn->value.second.bvalue = (bn->_leftExpression->value.second.ivalue >= bn->_rightExpression->value.second.ivalue);
                    return true;
                    break;
                }
                case parse_value_data_type_enum::Double: {
                    bn->value.second.bvalue = (bn->_leftExpression->value.second.dvalue >= bn->_rightExpression->value.second.dvalue);
                    return true;
                    break;
                }
            }
        }

        // ############## Mathematical binary functions ###################
    //printf("perform_binary_operator_function :: 0000 !!!!!!  |%s|\n", binary_function_token);
        if (binary_function_token == MATH_BINARY_OPERATOR_ADD) {
           // ++++++++++++
            if (left_expression_nan) {
// printf("perform_binary_operator_function ::binary_function_token MATH_BINARY_OPERATOR_ADD have NAN !!!! \n");
                bn->value.first = bn->_rightExpression->value.first;
                switch (bn->_rightExpression->value.first) {
                    case parse_value_data_type_enum::Float: {
                        bn->value.second.fvalue = (bn->_rightExpression->value.second.fvalue);
                        return true;
                        break;
                    }
                    case parse_value_data_type_enum::Integer: {
                        bn->value.second.ivalue = (bn->_rightExpression->value.second.ivalue);
                        return true;
                        break;
                    }
                    case parse_value_data_type_enum::Double: {
                        bn->value.second.dvalue = (bn->_rightExpression->value.second.dvalue);
                        return true;
                        break;
                    }
                }
            }

            if (right_expression_nan) {
                bn->value.first = bn->_leftExpression->value.first;
// printf("perform_binary_operator_function ::binary_function_token MATH_BINARY_OPERATOR_ADD have NAN !!!! \n");
                switch (bn->_leftExpression->value.first) {
                    case parse_value_data_type_enum::Float: {
                        bn->value.second.fvalue = (bn->_leftExpression->value.second.fvalue);
                        return true;
                        break;
                    }
                    case parse_value_data_type_enum::Integer: {
                        bn->value.second.ivalue = (bn->_leftExpression->value.second.ivalue);
                        return true;
                        break;
                    }
                    case parse_value_data_type_enum::Double: {
                        bn->value.second.dvalue = (bn->_leftExpression->value.second.dvalue);
                        return true;
                        break;
                    }
                }
            }
            // ++++++++++++
            bn->value.first = bn->_leftExpression->value.first;
            switch (bn->_leftExpression->value.first) {
                case parse_value_data_type_enum::Float: {
                    bn->value.second.fvalue = (bn->_leftExpression->value.second.fvalue + bn->_rightExpression->value.second.fvalue);
                    return true;
                    break;
                }
                case parse_value_data_type_enum::Integer: {
                    bn->value.second.ivalue = (bn->_leftExpression->value.second.ivalue + bn->_rightExpression->value.second.ivalue);
                    return true;
                    break;
                }
                case parse_value_data_type_enum::Double: {
                    bn->value.second.dvalue = (bn->_leftExpression->value.second.dvalue + bn->_rightExpression->value.second.dvalue);
                    return true;
                    break;
                }
            }
        }

        if (binary_function_token == MATH_BINARY_OPERATOR_MINUS) {
            // ++++++++++++
            if (left_expression_nan) {
                bn->value.first = bn->_rightExpression->value.first;
 //printf("perform_binary_operator_function ::binary_function_token MATH_BINARY_OPERATOR_MINUS have NAN !!!! \n");
                switch (bn->_rightExpression->value.first) {
                    case parse_value_data_type_enum::Float: {
                        bn->value.second.fvalue = (bn->_rightExpression->value.second.fvalue);
                        return true;
                        break;
                    }
                    case parse_value_data_type_enum::Integer: {
                        bn->value.second.ivalue = (bn->_rightExpression->value.second.ivalue);
                        return true;
                        break;
                    }
                    case parse_value_data_type_enum::Double: {
                        bn->value.second.dvalue = (bn->_rightExpression->value.second.dvalue);
                        return true;
                        break;
                    }
                }
            }

            if (right_expression_nan) {
                bn->value.first = bn->_leftExpression->value.first;
 //printf("perform_binary_operator_function ::binary_function_token MATH_BINARY_OPERATOR_MINUS have NAN !!!! \n");
                switch (bn->_rightExpression->value.first) {
                    case parse_value_data_type_enum::Float: {
                        bn->value.second.fvalue = (bn->_leftExpression->value.second.fvalue);
                        return true;
                        break;
                    }
                    case parse_value_data_type_enum::Integer: {
                        bn->value.second.ivalue = (bn->_leftExpression->value.second.ivalue);
                        return true;
                        break;
                    }
                    case parse_value_data_type_enum::Double: {
                        bn->value.second.dvalue = (bn->_leftExpression->value.second.dvalue);
                        return true;
                        break;
                    }
                }
            }
            // ++++++++++++
            bn->value.first = bn->_leftExpression->value.first;
            switch (bn->value.first) {
                case parse_value_data_type_enum::Float: {
                    bn->value.second.fvalue = (bn->_leftExpression->value.second.fvalue - bn->_rightExpression->value.second.fvalue);
                    return true;
                    break;
                }
                case parse_value_data_type_enum::Integer: {
                    bn->value.second.ivalue = (bn->_leftExpression->value.second.ivalue - bn->_rightExpression->value.second.ivalue);
                    return true;
                    break;
                }
                case parse_value_data_type_enum::Double: {
                    bn->value.second.dvalue = (bn->_leftExpression->value.second.dvalue - bn->_rightExpression->value.second.dvalue);
                    return true;
                    break;
                }
            }
        }

        if (binary_function_token == MATH_BINARY_OPERATOR_MULTIPLY) {
              // ++++++++++++
            if (left_expression_nan || right_expression_nan) {
 //printf("perform_binary_operator_function ::binary_function_token MATH_BINARY_OPERATOR_MULTIPLY have NAN !!!! \n");
                bn->value.first = parse_value_data_type_enum::nan;

                switch (bn->_rightExpression->value.first) {
                    case parse_value_data_type_enum::Float: {
                        bn->value.second.fvalue = 0.0f;
                        return true;
                        break;
                    }
                    case parse_value_data_type_enum::Integer: {
                        bn->value.second.ivalue = 0;
                        return true;
                        break;
                    }
                    case parse_value_data_type_enum::Double: {
                        bn->value.second.dvalue = 0.0f;
                        return true;
                        break;
                    }
                }
            }
            // ++++++++++++
            bn->value.first = bn->_leftExpression->value.first;
            switch (bn->value.first) {
                case parse_value_data_type_enum::Float: {
                    bn->value.second.fvalue = (bn->_leftExpression->value.second.fvalue * bn->_rightExpression->value.second.fvalue);
                    return true;
                    break;
                }
                case parse_value_data_type_enum::Integer: {
                    bn->value.second.ivalue = (bn->_leftExpression->value.second.ivalue * bn->_rightExpression->value.second.ivalue);
                    return true;
                    break;
                }
                case parse_value_data_type_enum::Double: {
                    bn->value.second.dvalue = (bn->_leftExpression->value.second.dvalue * bn->_rightExpression->value.second.dvalue);
                    return true;
                    break;
                }
            }
        }

        if (binary_function_token == MATH_BINARY_OPERATOR_DIVIDE) {
            // ++++++++++++
            if (left_expression_nan || right_expression_nan) {
 //printf("perform_binary_operator_function ::binary_function_token MATH_BINARY_OPERATOR_DIVIDE have NAN !!!! \n");
                bn->value.first = parse_value_data_type_enum::nan;

                switch (bn->_rightExpression->value.first) {
                    case parse_value_data_type_enum::Float: {
                        bn->value.second.fvalue = 0.0f;
                        return true;
                        break;
                    }
                    case parse_value_data_type_enum::Integer: {
                        bn->value.second.ivalue = 0;
                        return true;
                        break;
                    }
                    case parse_value_data_type_enum::Double: {
                        bn->value.second.dvalue = 0.0f;
                        return true;
                        break;
                    }
                }
            }
            // ++++++++++++
            bn->value.first = bn->_leftExpression->value.first;
            switch (bn->value.first) {
                case parse_value_data_type_enum::Float: {
                    bn->value.second.fvalue = (bn->_leftExpression->value.second.fvalue / bn->_rightExpression->value.second.fvalue);
                    return true;
                    break;
                }
                case parse_value_data_type_enum::Integer: {
                    bn->value.second.ivalue = (bn->_leftExpression->value.second.ivalue / bn->_rightExpression->value.second.ivalue);
                    return true;
                    break;
                }
                case parse_value_data_type_enum::Double: {
                    bn->value.second.dvalue = (bn->_leftExpression->value.second.dvalue / bn->_rightExpression->value.second.dvalue);
                    return true;
                    break;
                }
            }
        }

        if (binary_function_token == MATH_BINARY_OPERATOR_MODULUS) {
            bn->value.first = bn->_leftExpression->value.first;

            // ++++++++++++
            if (left_expression_nan || right_expression_nan) {
 //printf("perform_binary_operator_function ::binary_function_token MATH_BINARY_OPERATOR_MODULUS have NAN !!!! \n");
                bn->value.first = parse_value_data_type_enum::nan;

                switch (bn->_rightExpression->value.first) {
                    case parse_value_data_type_enum::Float: {
                        bn->value.second.fvalue = 0.0f;
                        return true;
                        break;
                    }
                    case parse_value_data_type_enum::Integer: {
                        bn->value.second.ivalue = 0;
                        return true;
                        break;
                    }
                    case parse_value_data_type_enum::Double: {
                        bn->value.second.dvalue = 0.0f;
                        return true;
                        break;
                    }
                }
            }
            // ++++++++++++

            switch (bn->value.first) {
                case parse_value_data_type_enum::Float: {
                    bn->value.second.fvalue = fmod(bn->_leftExpression->value.second.fvalue, bn->_rightExpression->value.second.fvalue);
                    return true;
                    break;
                }
                case parse_value_data_type_enum::Integer: {
                    bn->value.second.ivalue = (bn->_leftExpression->value.second.ivalue % bn->_rightExpression->value.second.ivalue);
                    return true;
                    break;
                }
                case parse_value_data_type_enum::Double: {
                    bn->value.second.dvalue = fmod(bn->_leftExpression->value.second.dvalue, bn->_rightExpression->value.second.dvalue);
                    return true;
                    break;
                }
            }
        }

        return false;
    }

    // Function to assign a literal value defined as a string of text as a number of a 
    // defined numeric datatype to a paser tree literal node storage container.

    bool assign_node_value(literal_node* ln, parse_value_data_type_enum literal_data_type) {
        std::string literal_token = ln->function_token_definition.second;

        //printf("functions :: assign_node_value AAAA %s:\n", literal_token.c_str());
        double* dvalue_ptr = new double;
        if (!dvalue_ptr) { return false; }

        int error_code = 0;
        //printf("functions :: assign_node_value BBBBB %s:\n", literal_token.c_str());
        bool valid_number = FW::stringtools::string_to_double(literal_token, dvalue_ptr, error_code);
        //printf("functions :: assign_node_value CCCCC %s:\n", literal_token.c_str());
        if (!valid_number) {
            printf("functions :: assign_node_value ERROR :: Invalid literal value to assign : %s:\n", literal_token);
            return false;
        }

        ln->value.first = literal_data_type;

        int int_value = stoi(literal_token);

        switch (literal_data_type) {
        case parse_value_data_type_enum::Boolean: if (stoi(literal_token) == 0) { ln->value.second.bvalue = true; }
                                                else { ln->value.second.bvalue = false; break; }
        case parse_value_data_type_enum::Integer: ln->value.second.ivalue = stoi(literal_token); break;
        case parse_value_data_type_enum::Float: ln->value.second.fvalue = stof(literal_token); break;
        case parse_value_data_type_enum::Double: ln->value.second.dvalue = stod(literal_token); break;
        }

        return true;
    }

    // ################ FUNCTIONS TO RETRIEVE HEXAGONAL GRID CELL VALUES ###############

    template <class T>
    bool get_hex_grid_value(literal_node* dn, hex_grid_base_class<T>* hex_grid, hex_surface_index_data_type hex_index) {
        if (hex_index < 0 || hex_index >= hex_grid->hex_grid.size()) return false; //hex_index is invalid

        std::string hex_data_token = dn->function_token_definition.second;

        hex_surface_index_data_type hex_neighbour_index = -1;
        int hex_neighbour_id = -1;

        if (hex_data_token == HEX_GRID_NEIGHBOUR_H0) {hex_neighbour_id = 0;}
        if (hex_data_token == HEX_GRID_NEIGHBOUR_H1) {hex_neighbour_id = 1;}
        if (hex_data_token == HEX_GRID_NEIGHBOUR_H2) {hex_neighbour_id = 2;}
        if (hex_data_token == HEX_GRID_NEIGHBOUR_H3) {hex_neighbour_id = 3;}
        if (hex_data_token == HEX_GRID_NEIGHBOUR_H4) {hex_neighbour_id = 4;}
        if (hex_data_token == HEX_GRID_NEIGHBOUR_H5) {hex_neighbour_id = 5;}
        if (hex_data_token == HEX_GRID_NEIGHBOUR_H6) {hex_neighbour_id = 6;}

        if (hex_neighbour_id < 0) { return false; } //have invalid hex neighbour token

        hex_neighbour_index = get_hex_neighbour_index(hex_grid, hex_index, hex_neighbour_id);

        if (hex_neighbour_index < 0 || hex_neighbour_index > hex_grid->hex_grid.size() - 1) { // Neighbour is outside of hex grid bounds and does not exist 
            dn->value.first = hex_grid_value_data_type_enum::nan;// This is to indicate that the neighbour is to be ignored as it does not exist
            return true;
        }

        dn->value.first = hex_grid->hex_grid_value_data_type;

        //printf("functions :: assign_hex_grid_value $$$$$$$$$$$$$ : %i : %i\n", hex_index, hex_neighbour_index);
        switch (hex_grid->hex_grid_value_data_type) {
            case hex_grid_value_data_type_enum::Boolean: dn->value.second.bvalue = hex_grid->hex_grid[hex_neighbour_index]; break;
            case hex_grid_value_data_type_enum::Float:   dn->value.second.fvalue = hex_grid->hex_grid[hex_neighbour_index]; break;
            case hex_grid_value_data_type_enum::Integer: dn->value.second.ivalue = hex_grid->hex_grid[hex_neighbour_index]; break;
            case hex_grid_value_data_type_enum::Double:  dn->value.second.dvalue = hex_grid->hex_grid[hex_neighbour_index]; break;

            // for testing :: Toggle this with above case statements when not testing
            //case hex_grid_value_data_type_enum::Boolean: dn->value.second.bvalue = hex_grid->hex_grid[hex_neighbour_index];printf("Boolean:%i\n", hex_grid->hex_grid[hex_neighbour_index]); break;
            //case hex_grid_value_data_type_enum::Float:   dn->value.second.fvalue = hex_grid->hex_grid[hex_neighbour_index]; printf("Float:  %f\n", hex_grid->hex_grid[hex_neighbour_index]); break;
            //case hex_grid_value_data_type_enum::Integer: dn->value.second.ivalue = hex_grid->hex_grid[hex_neighbour_index]; printf("Integer:%i\n", hex_grid->hex_grid[hex_neighbour_index]); break;
            //case hex_grid_value_data_type_enum::Double:  dn->value.second.dvalue = hex_grid->hex_grid[hex_neighbour_index]; printf("Double: %d\n", hex_grid->hex_grid[hex_neighbour_index]); break;
        }

        return true;
    }

    hex_surface_vec_data_type get_hex_surface_neighbour_index_coordinates(size_t neighbour, int x_index, int y_index) {
        hex_surface_vec_data_type neighbor_index_coordinate;
        if (y_index % 2 == 0) { // Even Y 
            switch (neighbour) {
                case 1: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index + 1;  break;
                case 2: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index;      break;
                case 3: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index - 1;  break;
                case 4: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index - 1;  break;
                case 5: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index;      break;
                case 6: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index + 1;  break;
            }
        }
        else {// Odd Y 
            switch (neighbour) {
                case 1: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index + 1; break;
                case 2: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index;     break;
                case 3: neighbor_index_coordinate.x = x_index + 1; neighbor_index_coordinate.y = y_index - 1; break;
                case 4: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index - 1; break;
                case 5: neighbor_index_coordinate.x = x_index - 1; neighbor_index_coordinate.y = y_index;     break;
                case 6: neighbor_index_coordinate.x = x_index;     neighbor_index_coordinate.y = y_index + 1;  break;
            }
        }
        return neighbor_index_coordinate;
    }

    template <class T>
    hex_surface_index_data_type get_hex_neighbour_index(hex_grid_base_class<T>* hex_grid, hex_surface_index_data_type hex_index, int neighbour_id) {
        if (neighbour_id == 0) { return hex_index; }// hex grid cell self value;

        hex_surface_vec_data_type hex_surface_coordinate = hex_grid->get_matrix_coordinate(hex_index);
        //printf("functions :: get_hex_neighbour_index ######### 00000: %i : %i : %i\n", hex_surface_coordinate.x, hex_surface_coordinate.y, hex_index);
        hex_surface_vec_data_type hex_neighbour_coordinate = get_hex_surface_neighbour_index_coordinates(neighbour_id, hex_surface_coordinate.x, hex_surface_coordinate.y);

        //printf("functions :: get_hex_neighbour_index ######### 11111: %i : %i : %i\n", hex_neighbour_coordinate.x, hex_neighbour_coordinate.y, hex_index);

        if (hex_neighbour_coordinate.y < 0 || hex_neighbour_coordinate.y >= hex_grid->grid_dimension.y) { return -1; }

        //if (hex_surface_coordinate.y % 2 == 0) { // Even Y
        if (hex_neighbour_coordinate.y % 2 == 0) { // Even Y
            if (hex_neighbour_coordinate.x < 0 || hex_neighbour_coordinate.x >= hex_grid->grid_dimension.x) { return -1; }
        }
        else {// Odd Y
            if (hex_neighbour_coordinate.x < 0 || hex_neighbour_coordinate.x >= hex_grid->grid_dimension.x - 1) { return -1; }
        }

        hex_surface_index_data_type hex_neighbour_index = hex_grid->get_index_value(hex_neighbour_coordinate.x, hex_neighbour_coordinate.y, 0);

        // printf("functions :: get_hex_neighbour_index ######### : %i : %i : %i\n", hex_neighbour_coordinate.x, hex_neighbour_coordinate.y, hex_neighbour_index);

        return hex_neighbour_index;
    }
//private:

};