#pragma once

#include "parser_definitions.h"

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

// Function class to perform all parser function operations and assign
// a value of the result to the parser value storage container.

// General method of function is to test which operator token is present
// for the type of operator that is being evaluated and perform the operation
// on the relevant data values for that operator.

class functions_base_class {
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

            if (left_expression_nan ) {
                bn->value.second.bvalue = bn->_rightExpression->value.second.bvalue;
                return true;
            }

            if (right_expression_nan) {
                bn->value.second.bvalue = bn->_leftExpression->value.second.bvalue;
                return true;
            }

            bn->value.second.bvalue = bn->_leftExpression->value.second.bvalue && bn->_rightExpression->value.second.bvalue;
//if(bn->value.second.bvalue)
//printf("perform_binary_operator_function ::binary_function_token == LOGIC_OPERATOR_AND 11111 #### !!!! bn->value.second.bvalue == true\n");
//else
//printf("perform_binary_operator_function ::binary_function_token == LOGIC_OPERATOR_AND 11111 #### !!!! bn->value.second.bvalue == false\n");
            return true;
        }

        if (binary_function_token == LOGIC_OPERATOR_OR) {
            bn->value.first = parse_value_data_type_enum::Boolean;

            if (left_expression_nan) {
                bn->value.second.bvalue = bn->_rightExpression->value.second.bvalue;
                return true;
            }

            if (right_expression_nan) {
                bn->value.second.bvalue = bn->_leftExpression->value.second.bvalue;
                return true;
            }

            bn->value.second.bvalue = bn->_leftExpression->value.second.bvalue || bn->_rightExpression->value.second.bvalue;
            return true;
        }

        // ############## Comparison binary functions ###################

        if (binary_function_token == LOGIC_OPERATOR_EQUAL) {
            bn->value.first = parse_value_data_type_enum::Boolean;

            if (left_expression_nan || right_expression_nan) {
 //printf("perform_binary_operator_function ::binary_function_token LOGIC_OPERATOR_EQUAL have NAN !!!! \n");
                bn->value.second.bvalue = false;
                return true; // cannot perform binary operation
            }

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

            if (left_expression_nan || right_expression_nan) {
 //printf("perform_binary_operator_function ::binary_function_token != have NAN !!!! \n");
                bn->value.second.bvalue = true;
                return true; // cannot perform binary operation
            }

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

            if (left_expression_nan || right_expression_nan) {
 //printf("perform_binary_operator_function ::binary_function_token COMPARISON_OPERATOR_LESS_THAN have NAN !!!! \n");
                bn->value.second.bvalue = false;
                return true; // cannot perform binary operation
            }

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

            if (left_expression_nan || right_expression_nan) {
 //printf("perform_binary_operator_function ::binary_function_token COMPARISON_OPERATOR_GREATER_THAN have NAN !!!! \n");
                bn->value.second.bvalue = false;
                return true; // cannot perform binary operation
            }

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

            if (left_expression_nan || right_expression_nan) {
// printf("perform_binary_operator_function ::binary_function_token <= have NAN !!!! \n");
                bn->value.second.bvalue = false;
                return true; // cannot perform binary operation
            }

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

            if (left_expression_nan || right_expression_nan) {
 //printf("perform_binary_operator_function ::binary_function_token >= have NAN !!!! \n");
                bn->value.second.bvalue = false;
                return true; // cannot perform binary operation
            }


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
            printf("functions :: assign_node_value ERROR :: Invalid literal value to assign : %s:\n", literal_token.c_str());
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

//private:

};