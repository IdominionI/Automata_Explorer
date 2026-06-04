#pragma once

//#include "parser_definitions.h"

#include "functions.h"


template <class T>
class logic_parser_class {
public:
    logic_parser_class() {
        define_white_space();
        define_braces();
        define_comparison_operator_characters();
        define_unary_operator_tokens();
        define_binary_operator_tokens();
        define_set_operator_tokens();
        define_hex_grid_neighbour_tokens();
    }

    ~logic_parser_class() {}

    // for testing only Delete or comment out when not in use or needed
    hex_grid_base_class<T> *hex_test_grid = nullptr;

    hex_grid_value_data_type_enum hex_grid_data_type = hex_grid_value_data_type_enum::Integer;

    parser_base_node *create_logic_parse_tree(std::string& text) {
//printf("logic_parser_class :: Parse::  000 %s\n",text.c_str());
        if (text.empty()) {
            // error message that logic function text is empty
            return nullptr;
        }

        // Create automata sub rule token list
        std::vector<std::string> tokens = {};
        std::stringstream sb;

        sb.str("");
        sb.clear();
//printf("logic_parser_class :: Parse::  111 \n");
        for(int i=0;i<text.size();i++){
            char c = text[i];
//printf("logic_parser_class :: Parse::  AAA %s\n",&c);
            if (have_white_space_character(c)) {
//printf("logic_parser_class :: Parse::  BBB %s\n",&c);
                if (sb.str().empty()) {
                    continue;
                }
                //sb << c;
                continue;
            }
printf("logic_parser_class :: Parse::  CCC :"); printf("%c:\n",c);

            if (have_equal_character(c)) {
printf("logic_parser_class :: Parse::  CCC111 :"); printf("%c:\n",c);
                if (!sb.str().empty()) {
                    char prev = sb.str().back();
printf("logic_parser_class :: Parse::  CCC222 :"); printf("%c:\n",prev);
                    if (have_comparison_operator_characters(prev)) {
printf("logic_parser_class :: Parse::  CCC233:"); printf("%c:\n",prev);
                        sb.str(sb.str().substr(0, sb.str().size() - 1));
printf("logic_parser_class :: Parse::  CCC244:"); printf("%s:\n",sb.str());
                        tokens.push_back(sb.str());
                        sb.str("");
                        sb.clear();

                        std::string token_string = std::string(1, prev) + c;

                        tokens.push_back(token_string);
                        continue;
                    }
                    // else error message
                }
                sb << c;
                continue;
            }

            if (have_open_close_brace(c)) {
printf("logic_parser_class :: Parse::  DDD"); printf(" %s\n",&c);
                if (!sb.str().empty()) {
                    tokens.push_back(sb.str());
                    sb.str("");
                    sb.clear();
                }
                tokens.push_back(std::string(1, c));
                continue;
            }

            std::string op = sb.str() + std::string(1, c);
            std::string sb1 = "";
printf("logic_parser_class :: Parse::  EEE (op, sb1)"); printf(" %s\n", op.c_str());
            if (have_binary_operator(op, sb1)) {
printf("logic_parser_class :: Parse::  FFF have_binary_operator(op, sb1)"); printf(" %s:#%s:\n", sb1.c_str(), op.c_str());
                if(i<text.size()-1){
                    char next = text[i + 1];
                    if (!have_equal_character(next))
                    {
                        tokens.push_back(sb1);
                        tokens.push_back(op);
                        sb.str("");
                        sb.clear();
                        continue;
                    }
                }
            }
//printf("logic_parser_class :: Parse::  GGG %s\n",&c);
            sb << c;
        }

        if (!sb.str().empty()) {
            tokens.push_back(sb.str());
        }

// Need to create a parse filter function
        //#################
printf("logic_parser_class :: Parse before filter spaces:"); printf(" %i \n", tokens.size());
       for (int i = tokens.size()-1; i > -1 ; i--) {
            std::string token = tokens[i];
printf("logic_parser_class :: Parse %s:\n", token.c_str());
            if (have_spaces_in_token(token)) {
printf("logic_parser_class :: Parse have_space_token ::"); printf("%s:\n", token.c_str());
                remove_token_spaces(tokens[i]);
            }

            if(have_empty_token(token)){
printf("logic_parser_class :: Parse have_empty_token ::"); printf("%s:\n", token.c_str());
                tokens.erase(tokens.begin() + i);
            }
        }
       //#################

//For testing : Delete or comment out when not in use or needed
printf("logic_parser_class :: Parse after filter spaces:"); printf(" %i\n", tokens.size());
       for (std::string token : tokens) {
printf("logic_parser_class :: Parse "); printf("%s|\n", token.c_str());      
       }

        parser_base_node *t_node = Parse(tokens);

//------For testing : Delete or comment out when not in use or needed -----
if (t_node == nullptr) {
    printf("logic_parser_class :: Parse !!!! t_node == nullptr !!!!!\n");
    return nullptr;
}

display_parse_tree(t_node);

/*
if (!hex_test_grid) {
    printf("logic_parser_class::Parse  hex_test_grid is nullptr : cannot test get hex grid neighbours \n");
    return nullptr;
}


size_t H0_index = 75;
hex_test_grid->hex_grid[H0_index] = 0;
printf("logic_parser_class::Parse  test grid neighbour 0 grid index %i : value |%i|\n", H0_index, hex_test_grid->hex_grid[H0_index]);

hex_surface_index_data_type h_i = get_hex_neighbour_index<T>(hex_test_grid, H0_index, 1);
if (h_i > -1) { hex_test_grid->hex_grid[h_i] = 1; }
printf("logic_parser_class::Parse  test grid neighbour 1 grid index %i : value |%i|\n", h_i, hex_test_grid->hex_grid[h_i]);
h_i = get_hex_neighbour_index(hex_test_grid, H0_index, 2);
if (h_i > -1) { hex_test_grid->hex_grid[h_i] = 2; }
printf("logic_parser_class::Parse  test grid neighbour 2 grid index %i : value |%i|\n", h_i, hex_test_grid->hex_grid[h_i]);
h_i = get_hex_neighbour_index(hex_test_grid, H0_index, 3);
if (h_i > -1) { hex_test_grid->hex_grid[h_i] = 3; }
printf("logic_parser_class::Parse  test grid neighbour 3 grid index %i : value |%i|\n", h_i, hex_test_grid->hex_grid[h_i]);
h_i = get_hex_neighbour_index(hex_test_grid, H0_index, 4);
if (h_i > -1) { hex_test_grid->hex_grid[h_i] = 4; }
printf("logic_parser_class::Parse  test grid neighbour 4 grid index %i : value |%i|\n", h_i, hex_test_grid->hex_grid[h_i]);
h_i = get_hex_neighbour_index(hex_test_grid, H0_index, 5);
if (h_i > -1) { hex_test_grid->hex_grid[h_i] = 5; }
printf("logic_parser_class::Parse  test grid neighbour 5 grid index %i : value |%i|\n", h_i, hex_test_grid->hex_grid[h_i]);
h_i = get_hex_neighbour_index(hex_test_grid, H0_index, 6);
if (h_i > -1) { hex_test_grid->hex_grid[h_i] = 6; }
printf("logic_parser_class::Parse  test grid neighbour 6 grid index %i : value |%i|\n", h_i, hex_test_grid->hex_grid[h_i]);


//evaluate_parse_tree(t_node, hex_test_grid, H0_index);

printf("logic_parser_class :: Parse Tree Result :: %i \n", t_node->value.second.bvalue);
*/

//return nullptr;
// ---------------------------------------------------------------------
        
        // uncomment when ready to implement
         return t_node;
    }

    // post-order tree traversal
    void display_parse_tree(parser_base_node *tt_node) {
printf("logic_parser_class :: display_parse_tree 00000\n");
        if (!tt_node) {
            printf("logic_parser_class :: display_parse_tree tt_node == null\n");
            return;
        }

        switch (tt_node->function_token_definition.first) {
            case function_type_enum::binary: {
                binary_node* bn = static_cast<binary_node*>(tt_node);
                display_parse_tree(bn->_leftExpression);
                display_parse_tree(bn->_rightExpression);
                break;
            }
            case function_type_enum::unary : {
                unary_node* un = static_cast<unary_node*>(tt_node);
                display_parse_tree(un->_expression);
                break;
            }
            case function_type_enum::value : {
                literal_node* ln = static_cast<literal_node*>(tt_node);
                break;
            }
        }

printf("logic_parser_class :: display_parse_tree"); printf(" %s\n", tt_node->function_token_definition.second.c_str());
    }

    void delete_pars_tree(parser_base_node *tt_node) {
        if (!tt_node) {
            printf("logic_parser_class :: delete_pars_tree tt_node == null\n");
            return;
        }

        switch (tt_node->function_token_definition.first) {
            case function_type_enum::binary: {
                binary_node* bn = static_cast<binary_node*>(tt_node);
                delete_pars_tree(bn->_leftExpression);
                delete_pars_tree(bn->_rightExpression);
                delete bn;
                break;
            }
            case function_type_enum::unary: {
                unary_node* un = static_cast<unary_node*>(tt_node);
                delete_pars_tree(un->_expression);
                delete un;
                break;
            }
            case function_type_enum::value: {
                literal_node* ln = static_cast<literal_node*>(tt_node);
                delete ln;
                break;
            }
        }

//printf("logic_parser_class :: delete_pars_tree completed\n");
    }

    bool evaluate_parse_tree(parser_base_node *tt_node, hex_grid_base_class<T> *hex_grid, hex_surface_index_data_type hex_index) {
        if (!tt_node) {
printf("logic_parser_class :: evaluate_parse_tree tt_node == null\n");
            return false;
        }

        switch (tt_node->function_token_definition.first) {
            case function_type_enum::binary: {
                binary_node* bn = static_cast<binary_node*>(tt_node);
                if (!evaluate_parse_tree(bn->_leftExpression,hex_grid, hex_index)) {
 //                   printf("evaluate_parse_tree ERROR : Left expression value cannot be evaluated %s\n", bn->_leftExpression->function_token_definition.second.c_str());
                    return false;
                }
                if (!evaluate_parse_tree(bn->_rightExpression,hex_grid, hex_index)) {
 //                   printf("evaluate_parse_tree ERROR : Right expression value cannot be evaluated %s\n", bn->_rightExpression->function_token_definition.second.c_str());
                    return false;
                }
//printf("logic_parser_class :: perform_binary_operator_function 0000 %s\n", tt_node->function_token_definition.second.c_str());
//if(bn->_leftExpression->value.first == parse_value_data_type_enum::nan) printf("logic_parser_class :: evaluate_parse_tree : DATA :: bn->_leftExpression.value.first == parse_value_data_type_enum::nan %s :: %i\n", bn->_leftExpression->function_token_definition.second.c_str(), hex_index);// testing only : delete comment out when not needed
//if(bn->_rightExpression->value.first == parse_value_data_type_enum::nan) printf("logic_parser_class :: evaluate_parse_tree : DATA :: bn->_rightExpression.value.first == parse_value_data_type_enum::nan %s :: %i\n", bn->_rightExpression->function_token_definition.second.c_str(), hex_index);// testing only : delete comment out when not needed

                if(!functions.perform_binary_operator_function(bn)){
//printf("evaluate_parse_tree ERROR : Cannot perform binary function %s\n", tt_node->function_token_definition.second.c_str());
                    return false;
                }
//printf("logic_parser_class :: evaluate_parse_tree : BINARY :: Parse Tree Result of function %s :: ",bn->function_token_definition.second.c_str());// testing only : delete comment out when not needed
//display_evaluation_value(bn);// testing only : delete comment out when not needed

                tt_node->value = bn->value;
                break;
            }
            //case function_type_enum::unary : {
            //    unary_node* un = static_cast<unary_node*>(tt_node);
            //    evaluate_parse_tree(un->_expression);
            //    perform_unary_operator_function(un);
            //    break;
            //}
            case  function_type_enum::data: {
                literal_node *ln = static_cast<literal_node*>(tt_node);
                if(!functions.get_hex_grid_value(ln,hex_grid, hex_index)){
 //printf("logic_parser_class :: ParseSubExp Could not define hex node neigbour value for hex %s\n", ln->function_token_definition.second.c_str());
                    return false;
                }
//if(dn->value.first == parse_value_data_type_enum::nan) printf("logic_parser_class :: evaluate_parse_tree : DATA :: dn->value.first == parse_value_data_type_enum::nan %s :: %i\n", dn->function_token_definition.second.c_str(), hex_index);// testing only : delete comment out when not needed
//printf("logic_parser_class :: evaluate_parse_tree : DATA :: Parse Tree hex grid value of %s :: \n", dn->function_token_definition.second.c_str());// testing only : delete comment out when not needed
//display_evaluation_value(dn);// testing only : delete comment out when not needed

//if(tt_node->value.first == parse_value_data_type_enum::nan) printf("logic_parser_class :: evaluate_parse_tree : DATA :: tt_node->value.first == parse_value_data_type_enum::nan %s :: %i\n", tt_node->function_token_definition.second.c_str(), hex_index);// testing only : delete comment out when not needed

                break;
            }

            case function_type_enum::value : {
                literal_node* ln = static_cast<literal_node*>(tt_node);
//printf("logic_parser_class :: assign_node_value 1111 %s\n", tt_node->function_token_definition.second.c_str());
                if(!functions.assign_node_value(ln, parse_value_data_type_enum::Integer)){
 //printf("evaluate_parse_tree ERROR : Cannot assign literal data value of %s\n", tt_node->function_token_definition.second.c_str());
                    return false;
                }
//printf("logic_parser_class :: evaluate_parse_tree : VALUE :: Parse Tree literal value of %s :: ", ln->function_token_definition.second.c_str());// testing only : delete comment out when not needed
//display_evaluation_value(ln);// testing only : delete comment out when not needed

                break;
            }
        }

//printf("logic_parser_class :: evaluate_parse_tree : RETURN TRUE :: for Parse Tree Result :: \n");// testing only : delete comment out when not needed
        return true;
    }

    void display_evaluation_value(parser_base_node *t_node) {
        switch (t_node->value.first) {
            case parse_value_data_type_enum::Boolean: printf("Boolean %i :: \n", t_node->value.second.bvalue); break;
            case parse_value_data_type_enum::Integer: printf("Integer %i :: \n", t_node->value.second.ivalue); break;
            case parse_value_data_type_enum::Float:   printf("Float   %f :: \n", t_node->value.second.fvalue); break;
            case parse_value_data_type_enum::Double:  printf("Double  %d :: \n", t_node->value.second.dvalue); break;
        }
    }

   parser_base_node* Parse(const std::vector<std::string>& tokens) {
        size_t index = 0;
        return ParseExp(tokens, index);
    }

private:
    int number_left_braces = 0;
    int number_right_braces = 0;
    
    parser_base_node *ParseExp(const std::vector<std::string>& tokens, size_t& index) {
        auto leftExp = ParseSubExp(tokens, index);

        if (index >= tokens.size()) {
            return leftExp;
        }

        const std::string& token = tokens[index];

        if (token == std::string(1, BRACE_RIGHT)) {
            number_right_braces++;
            if (number_left_braces < number_right_braces) {
                // error message
//printf("logic_parser_class :: ParseExp ERROR :Expected ')'\n", tokens[index].c_str());
                return nullptr;
            }

            number_left_braces--;
            number_right_braces--;
            return leftExp;
        }

        if (have_binary_operator(token)) {
//printf("logic_parser_class :: ParseExp have_binary_operator 000:%s\n", token.c_str());
            index++;
            auto rightExp = ParseExp(tokens, index);

            if (!rightExp) {
                afw_globalc::get_current_logger()->log(LogLevel::ERROR, "logic_parser_class :: ParseExp ERROR : No right expression defined for binary operator " + token + "\n");
                delete_pars_tree(leftExp);
//printf("logic_parser_class :: ParseExp binary operator right exp 1111 \n");
                return nullptr;
            }
//printf("logic_parser_class :: ParseExp binary operator right exp 2222 :%s\n", rightExp->function_token_definition.second.c_str());
            binary_node *b_node = new binary_node(std::move(leftExp), std::move(rightExp));
            b_node->function_token_definition.first = function_type_enum::binary;
            b_node->function_token_definition.second = token;
//printf("logic_parser_class :: ParseExp have_binary_operator 3333 :%s\n", b_node->function_token_definition.second.c_str());
            return b_node;
        }
        else {
//printf("logic_parser_class :: ParseExp Expected '&&' or '||' or '^' or EOF :: %s\n", token.c_str());
            return nullptr;
        }
    }

    parser_base_node *ParseSubExp(const std::vector<std::string>& tokens, size_t& index) {
        const std::string& token = tokens[index];

        if (token.empty()) { return nullptr; }

        if (token == std::string(1, BRACE_LEFT)) {
            index++;
            number_left_braces++;

//printf("logic_parser_class :: ParseSubExp 000 %s\n", token.c_str());
            parser_base_node *node = ParseExp(tokens, index);

//if(node) printf("logic_parser_class :: ParseSubExp 111 %s : %s\n", node->function_token_definition.second.c_str(), tokens[index].c_str());

            if (index >= tokens.size() || tokens[index] != std::string(1, BRACE_RIGHT)) {
//printf("logic_parser_class :: ParseSubExp ERROR :Expected ')'\n", tokens[index].c_str());
                return nullptr;
            }

            index++; // Skip ')'
            return node;
        }

        if (token == std::string(1, BRACE_RIGHT)) {// This does not seem to work
            index++;
            number_right_braces++;

            if (number_left_braces < number_right_braces) {// Could have incomplete left brace
                return nullptr;
            }
        }

        if (have_unary_operator(token)) {
            index++;
            parser_base_node* node = ParseExp(tokens, index);
            unary_node* u_node = new unary_node(node);
            u_node->function_token_definition.first = function_type_enum::unary;
            u_node->function_token_definition.second = token;
//printf("logic_parser_class :: ParseSubExp unary_node 2222 %s\n", u_node->function_token_definition.second.c_str());
            return u_node;
        }

        if (have_hex_grid_neighbour(token)) {
            index++;

            literal_node* l_node = new literal_node(token);
            l_node->function_token_definition.first = function_type_enum::data;
            l_node->function_token_definition.second = token;
//printf("logic_parser_class :: ParseSubExp 3333 literal_node %s\n", l_node->function_token_definition.second.c_str());
            return l_node;
        }

        index++;

        literal_node *l_node = new literal_node(token);
        l_node->function_token_definition.first = function_type_enum::value;
        l_node->function_token_definition.second = token;
//printf("logic_parser_class :: ParseSubExp 4444 literal_node #%s#|\n", l_node->function_token_definition.second.c_str());
        return l_node;
    }



protected:

private:
    functions_class functions;// ++++++++++

    std::vector<char> white_space_characters;
    std::vector<char> brace_charecters;
    std::vector<char> comparison_operator_characters;

    std::vector<std::string> unary_operators;
    std::vector<std::string> binary_operators;
    std::vector<std::string> set_operators;
    
    std::vector<std::string> hex_grid_neighbours;

    void define_white_space() {
        white_space_characters.push_back(' ');
        white_space_characters.push_back('\t');
        white_space_characters.push_back('\r');
        white_space_characters.push_back('\n');
        white_space_characters.push_back('\0');
    }

    void define_braces() {
        brace_charecters.push_back(BRACE_LEFT);
        brace_charecters.push_back(BRACE_RIGHT);
        brace_charecters.push_back(SET_BRACE_LEFT);
        brace_charecters.push_back(SET_BRACE_RIGHT);
        brace_charecters.push_back(PARAMETERS_BRACE_LEFT);
        brace_charecters.push_back(PARAMETERS_BRACE_RIGHT);
    }

    void define_comparison_operator_characters() {
        comparison_operator_characters.push_back(COMPARISON_OPERATOR_EQUAL);
        comparison_operator_characters.push_back(COMPARISON_OPERATOR_LESS_THAN);
        comparison_operator_characters.push_back(COMPARISON_OPERATOR_GREATER_THAN);
        comparison_operator_characters.push_back(COMPARISON_OPERATOR_NOT);
    }

    void define_unary_operator_tokens() {
        unary_operators.push_back(LOGIC_OPERATOR_NOT);
        unary_operators.push_back(MATH_UNIARY_OPERATOR_SIN);
    }

    void define_binary_operator_tokens() {
        binary_operators.push_back(LOGIC_OPERATOR_AND);
        binary_operators.push_back(LOGIC_OPERATOR_OR);
        binary_operators.push_back(LOGIC_OPERATOR_EQUAL);

        binary_operators.push_back(std::string(1, COMPARISON_OPERATOR_EQUAL));
        binary_operators.push_back(std::string(1, COMPARISON_OPERATOR_LESS_THAN));
        binary_operators.push_back(std::string(1, COMPARISON_OPERATOR_GREATER_THAN));
        binary_operators.push_back(COMPARISON_OPERATOR_NOT_EQUAL);
        binary_operators.push_back(COMPARISON_OPERATOR_LESS_THAN_OR_EQUAL);
        binary_operators.push_back(COMPARISON_OPERATOR_GREATER_THAN_OR_EQUAL);

        binary_operators.push_back(MATH_BINARY_OPERATOR_ADD);
        binary_operators.push_back(MATH_BINARY_OPERATOR_MINUS);
        binary_operators.push_back(MATH_BINARY_OPERATOR_MULTIPLY);
        binary_operators.push_back(MATH_BINARY_OPERATOR_DIVIDE);
        binary_operators.push_back(MATH_BINARY_OPERATOR_MODULUS);
    }

    void define_set_operator_tokens() {
        set_operators.push_back(MATH_SET_OPERATOR_SUM);
        set_operators.push_back(MATH_SET_OPERATOR_AVERAGE);
    }

    void define_hex_grid_neighbour_tokens() {
        hex_grid_neighbours.push_back(HEX_GRID_NEIGHBOUR_H0);
        hex_grid_neighbours.push_back(HEX_GRID_NEIGHBOUR_H1);
        hex_grid_neighbours.push_back(HEX_GRID_NEIGHBOUR_H2);
        hex_grid_neighbours.push_back(HEX_GRID_NEIGHBOUR_H3);
        hex_grid_neighbours.push_back(HEX_GRID_NEIGHBOUR_H4);
        hex_grid_neighbours.push_back(HEX_GRID_NEIGHBOUR_H5);
        hex_grid_neighbours.push_back(HEX_GRID_NEIGHBOUR_H6);
    }

    bool have_white_space_character(char c) {
        for (char white_space_character : white_space_characters) {
            if (white_space_character == c) { return true; }
        }
        return false;
    }

    bool have_spaces_in_token(std::string token) {
        for (char c : token) {
            if (have_white_space_character(c)) {
                return true;
            }
        }
        return false;
    }

    void remove_token_spaces(std::string& token) {
        for (int i = token.size() - 1; i > -1; i--) {
            char c = token[i];
            if (have_white_space_character(c)) {
                token.erase(token.begin() + i);
            }
        }
    }

    bool have_empty_token(std::string token) {
        if (token.size() == 0) { return true; }
        return false;
    }

    bool have_logic_and_or_character(char c) {
        if (c == '&' || c == '|' || c == '!') {
            return true;
        }

        return false;
    }

    bool have_logic_not_character(char c) {
        if (c == COMPARISON_OPERATOR_NOT)
            {return true;}
        else
            {return false;}
    }

    bool have_open_close_brace(char c) {
        for (char brace_charecter : brace_charecters) {
            if (brace_charecter == c) { return true; }
        }
        return false;
    }

    bool have_equal_character(char c) {
        if (c == COMPARISON_OPERATOR_EQUAL)
            {return true;}
        else
            {return false;}
    }

    bool have_comparison_operator_characters(char c) {
        for (char comparison_operator_character : comparison_operator_characters) {
            if (comparison_operator_character == c) { return true; }
        }
        return false;
    }

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    bool have_unary_operator(std::string token) {
        for (std::string unary_operator : unary_operators) {
            if (token == unary_operator) { return true; }
        }
        return false;
    }

    bool have_binary_operator(std::string token) {
        for (std::string binary_operator : binary_operators) {
            if (token == binary_operator) { return true; }
        }
        return false;
    }

    bool have_binary_operator(std::string &token, std::string &prefix) {
        for (std::string binary_operator : binary_operators) {
            int pos = token.rfind(binary_operator);

            if (pos != token.npos) {
//printf("logic_parser_class :: Parse::  have_binary_operator(op, sb1) %s:%s: %i \n", token.c_str(), binary_operator.c_str(), pos);
                if (token == binary_operator) { return true; }

                prefix = token.substr(0, pos);
                token = token.substr(pos, binary_operator.size());

                return true;
            }
        }
        return false;
    }

    bool have_set_operator(std::string token) {
        for (std::string set_operator : set_operators) {
            if (token == set_operator) { return true; }
        }
        return false;
    }

    bool have_hex_grid_neighbour(std::string token) {
        for (std::string hex_grid_neighbour : hex_grid_neighbours) {
            if (token == hex_grid_neighbour) { return true; }
        }
        return false;
    }

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

};