#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <memory>
#include <cctype>

#include "../Hexagon/Objects/hex_grid_parameters.h"

#define BRACE_LEFT             '('
#define BRACE_RIGHT            ')'
#define SET_BRACE_LEFT         '{'
#define SET_BRACE_RIGHT        '}'
#define PARAMETERS_BRACE_LEFT  '['
#define PARAMETERS_BRACE_RIGHT ']'
// Other braces defined here

// ########### LOGICAL OPERATORS ################

#define LOGIC_OPERATOR_AND   "&&"
#define LOGIC_OPERATOR_OR    "||"
#define LOGIC_OPERATOR_NOT   "!!"
#define LOGIC_OPERATOR_EQUAL "=="

#define COMPARISON_OPERATOR_NOT          '!'
#define COMPARISON_OPERATOR_EQUAL        '='
#define COMPARISON_OPERATOR_LESS_THAN    '<'
#define COMPARISON_OPERATOR_GREATER_THAN '>'
#define COMPARISON_OPERATOR_NOT_EQUAL    "!="
#define COMPARISON_OPERATOR_LESS_THAN_OR_EQUAL    "<="
#define COMPARISON_OPERATOR_GREATER_THAN_OR_EQUAL ">="
// other logical operatiors defined here

// ########### MATH OPERATORS ################

#define MATH_UNIARY_OPERATOR_SIN    "SIN"

//#define MATH_BINARY_OPERATOR_EQUAL     "=" // This is not permitted as assigning values to a variable is not performed
#define MATH_BINARY_OPERATOR_ADD       ":+"
#define MATH_BINARY_OPERATOR_MINUS     ":-"
#define MATH_BINARY_OPERATOR_MULTIPLY  "*"
#define MATH_BINARY_OPERATOR_DIVIDE    "/"
#define MATH_BINARY_OPERATOR_MODULUS   "%"

#define MATH_SET_OPERATOR_SUM       "SUM"
#define MATH_SET_OPERATOR_AVERAGE   "AVERAGE"

// add other math operatiors as required here
// ###########################################

#define EXPRESSION_OPPERATOR_FLAG        "_"

// other expression operatiors defined here

typedef hex_grid_value_data_type_enum parse_value_data_type_enum;
enum class function_type_enum         { unary, binary, set, value, data, nan }; // add as required

typedef std::pair<function_type_enum, std::string > function_token_definition_type;// Type of function : string literal used to represent a function of set type

//template <class T>
union node_data_value {
    bool   bvalue;
    int    ivalue;
    float  fvalue;
    double dvalue;
    //<T>     tvalue;// probably will never use but here if one is to use it
};

typedef std::pair<parse_value_data_type_enum, node_data_value > node_data_value_type;// Type of function : string literal used to represent a function of set type

struct parser_base_node {

    function_token_definition_type function_token_definition;

    node_data_value_type value;

    bool not_a_number(node_data_value_type data_value) {
        if (data_value.first == parse_value_data_type_enum::nan)
            return true;
        else
            return false;
    }

    void define_value(parse_value_data_type_enum data_type, node_data_value data_value) {

        switch (data_type) {
            case(parse_value_data_type_enum::Boolean): value.second.bvalue = data_value.bvalue; break;
            case(parse_value_data_type_enum::Float):   value.second.fvalue = data_value.fvalue; break;
            case(parse_value_data_type_enum::Integer): value.second.ivalue = data_value.ivalue; break;
            case(parse_value_data_type_enum::Double):  value.second.dvalue = data_value.dvalue; break;
            case(parse_value_data_type_enum::nan):     value.second.ivalue = 0; break;
        }

        value.first = data_type;
    }

    virtual bool Evaluate() { return false; }
    virtual void PrettyPrint(std::ostream& writer) {};
    virtual ~parser_base_node() = default;
};

class literal_node : public parser_base_node {// raw text representing a number
private:
    std::string _literal;

public:
    explicit literal_node(std::string literal) : _literal(std::move(literal)) {}
    std::string RoleName() const { return _literal; }

    void PrettyPrint(std::ostream& writer) override {
        writer << "(" << _literal << ")";
    }
};

struct unary_node : public parser_base_node {
    parser_base_node* _expression;

    unary_node(parser_base_node* expression) : _expression(std::move(expression)) {}

    parser_base_node* Expression() const { return _expression; }
};

struct binary_node : parser_base_node {
    parser_base_node* _leftExpression;
    parser_base_node* _rightExpression;

    binary_node(parser_base_node* leftExpression, parser_base_node* rightExpression)
        : _leftExpression(std::move(leftExpression)), _rightExpression(std::move(rightExpression)) {
    }

    parser_base_node* LeftExpression() const { return _leftExpression; }
    parser_base_node* RightExpression() const { return _rightExpression; }
};

template <class T>
struct set_node : parser_base_node {
    std::vector<T> set_values;

    set_node(std::vector<T> _set_values) : set_values(_set_values) {}

};
