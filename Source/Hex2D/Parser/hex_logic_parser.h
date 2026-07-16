#pragma once

#include "hex_functions.h"

/*
    Parser class to create and manage a parser tree from a
    user defined string of text that is a conditional statement
    used to construct a cellular automata model. The conditional
    statement can have as part of it, assign values, perform
    basic mathematical operations and logical comparisons
    that evaluate the conditional statement to be true or false.

    This parser class is not designed for complex mathematical
    calculations or to perform programable routines.

    Not sure what type of parser this is as it was created with
    no knowledge of how to write a paser and only used a basic
    example that was heavily modified and expanded upon without
    any research.
    Thus may be replaced in future if find a parser library
    suitable for the purpose it serves.
*/

template <class T>
class hex_logic_parser_class {
public:
    // Initialise or define the parser categories of tokens
    hex_logic_parser_class() {
        define_white_space();
        define_braces();
        define_comparison_operator_characters();
        define_unary_operator_tokens();
        define_binary_operator_tokens();
        define_set_operator_tokens();
        define_hex_grid_neighbour_tokens();
    }

    ~hex_logic_parser_class() {}

    // for testing only Delete or comment out when not in use or needed
    //hex_grid_base_class<T> *hex_test_grid = nullptr;

    //hex_grid_value_data_type_enum hex_grid_data_type = hex_grid_value_data_type_enum::Integer;

    // Create Parse tree by creating a vector list of tokens that
    // represents the conditional statement of a string of text,
    // and then submit that list of tokens to a parser function
    // that creates a binary tree of parser nodes that when read
    // and evaluated in post order traversal will evaluate the
    // value of the conditional statement as being true or false.
    // Tests are performed to check if the submitted text creates
    // a valid parser tree or not and returns where any context
    // errors were found.

    // text : string of text that defines a C/C++ like conditional statement
    parser_base_node* create_logic_parse_tree(std::string& text) {
        //printf("logic_parser_class :: Parse::  000 %s\n",text.c_str());
        if (text.empty()) {
            // error message that logic function text is empty
            return nullptr;
        }

        // Create automata sub rule token list
        std::vector<std::string> tokens = {};// container to store the list of tokens
        std::stringstream sb;                // string buffer to store string of characters to create parser tokens

        sb.str("");// initialise sting buffer 
        sb.clear();
        //printf("logic_parser_class :: Parse::  111 \n");
                // Read string of text one character at a time to add tokens to token list as tokens are found
        for (int i = 0; i < text.size(); i++) {
            char c = text[i];
            //printf("logic_parser_class :: Parse::  AAA %s\n",&c);
                        //ignore what is defines as a white space
            if (have_white_space_character(c)) {
                //printf("logic_parser_class :: Parse::  BBB %s\n",&c);
                if (sb.str().empty()) {
                    continue; // get next character and continue loop from the top
                }
                //sb << c;
                continue;// get next character and continue loop from the top
            }
            printf("logic_parser_class :: Parse::  CCC :"); printf("%c:\n", c);

            // If have = character
            if (have_equal_character(c)) {
                printf("logic_parser_class :: Parse::  CCC111 :"); printf("%c:\n", c);
                if (!sb.str().empty()) {
                    char prev = sb.str().back(); //go back to previous character
                    printf("logic_parser_class :: Parse::  CCC222 :"); printf("%c:\n", prev);
                    if (have_comparison_operator_characters(prev)) {// if previous character is a comparison operator character
                        printf("logic_parser_class :: Parse::  CCC233:"); printf("%c:\n", prev);
                        sb.str(sb.str().substr(0, sb.str().size() - 1)); // read all left of prev character into sb as a string 
                        printf("logic_parser_class :: Parse::  CCC244:"); printf("%s:\n", sb.str());
                        tokens.push_back(sb.str()); // add sb string of characters left of  prev to list of parser tokens
                        sb.str(""); // reinitialise sb string to be empty
                        sb.clear();

                        std::string token_string = std::string(1, prev) + c;// create complete parser = comparison token as a string

                        tokens.push_back(token_string);// add = comparison token to list of tokens
                        continue;// get next character and continue loop from the top
                    }
                    // else cannot have lone = character assignment statement : Need to rectify this ???
                }
                sb << c; // add = character to sb
                continue;// get next character and continue loop from the top
            }

            // If have one of (){}[] characters : NOTE Should only use ()
            if (have_open_close_brace(c)) {
                printf("logic_parser_class :: Parse::  DDD0000"); printf(" %c\n", c);
                if (!sb.str().empty()) { //
                    printf("logic_parser_class :: Parse::  DDD1111"); printf(" %s\n", sb.str());
                    tokens.push_back(sb.str()); // add string token before brace to token list
                    sb.str("");// reinitialise sb string to be empty
                    sb.clear();
                }
                printf("logic_parser_class :: Parse::  DDD222"); printf(" %c\n", c);
                tokens.push_back(std::string(1, c));// add brace token to token list
                continue;// get next character and continue loop from the top
            }

            std::string op = sb.str() + std::string(1, c);
            std::string sb1 = "";
            printf("logic_parser_class :: Parse::  EEE (op, sb1)"); printf(" %s\n", op.c_str());
            if (have_binary_operator(op, sb1)) {
                printf("logic_parser_class :: Parse::  FFF have_binary_operator(op, sb1)"); printf(" %s:#%s:\n", sb1.c_str(), op.c_str());
                if (i < text.size() - 1) {
                    char next = text[i + 1];
                    if (!have_equal_character(next))// Do not have comparison binary operator eg += which is not permited
                    {
                        tokens.push_back(sb1);// add string before binary operator to list of tokens
                        tokens.push_back(op);// add binary operator character to list of tokens
                        sb.str("");// reinitialise sb string to be empty
                        sb.clear();
                        continue;// get next character and continue loop from the top
                    }
                }
            }
            //printf("logic_parser_class :: Parse::  GGG %s\n",&c);
            sb << c;// add character to sb
        }

        // Add final token string to tokens list
        if (!sb.str().empty()) {
            tokens.push_back(sb.str());
        }

        // Need to create a parse filter function
        printf("logic_parser_class :: Parse before filter spaces:"); printf(" %i \n", tokens.size());
        // Filter out spaces in tokens and remove any epmty tokens from the list
        for (int i = tokens.size() - 1; i > -1; i--) {
            std::string token = tokens[i];
            printf("logic_parser_class :: Parse %s:\n", token.c_str());
            if (have_spaces_in_token(token)) {
                printf("logic_parser_class :: Parse have_space_token ::"); printf("%s:\n", token.c_str());
                remove_token_spaces(tokens[i]);
            }

            if (have_empty_token(token)) {
                printf("logic_parser_class :: Parse have_empty_token ::"); printf("%s:\n", token.c_str());
                tokens.erase(tokens.begin() + i);
            }
        }

        //For testing : Delete or comment out when not in use or needed
        printf("logic_parser_class :: Parse after filter spaces:"); printf(" %i\n", tokens.size());
        for (std::string token : tokens) {
            printf("logic_parser_class :: Parse "); printf("%s|\n", token.c_str());
        }

        // create parser tree with t_node being the top root node of that parser tree
        parser_base_node* t_node = Parse(tokens);

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
    void display_parse_tree(parser_base_node* tt_node) {
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
        case function_type_enum::unary: {
            unary_node* un = static_cast<unary_node*>(tt_node);
            display_parse_tree(un->_expression);
            break;
        }
        case function_type_enum::value: {
            literal_node* ln = static_cast<literal_node*>(tt_node);
            break;
        }
        }

        printf("logic_parser_class :: display_parse_tree"); printf(" %s\n", tt_node->function_token_definition.second.c_str());
    }

    void delete_pars_tree(parser_base_node* tt_node) {
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

    bool evaluate_parse_tree(parser_base_node* tt_node, hex_grid_base_class<T>* hex_grid, hex_surface_index_data_type hex_index) {
        if (!tt_node) {
            printf("logic_parser_class :: evaluate_parse_tree tt_node == null\n");
            return false;
        }

        switch (tt_node->function_token_definition.first) {
        case function_type_enum::binary: {
            binary_node* bn = static_cast<binary_node*>(tt_node);
            if (!evaluate_parse_tree(bn->_leftExpression, hex_grid, hex_index)) {
                //                   printf("evaluate_parse_tree ERROR : Left expression value cannot be evaluated %s\n", bn->_leftExpression->function_token_definition.second.c_str());
                return false;
            }
            if (!evaluate_parse_tree(bn->_rightExpression, hex_grid, hex_index)) {
                //                   printf("evaluate_parse_tree ERROR : Right expression value cannot be evaluated %s\n", bn->_rightExpression->function_token_definition.second.c_str());
                return false;
            }
            //printf("logic_parser_class :: perform_binary_operator_function 0000 %s\n", tt_node->function_token_definition.second.c_str());
            //if(bn->_leftExpression->value.first == parse_value_data_type_enum::nan) printf("logic_parser_class :: evaluate_parse_tree : DATA :: bn->_leftExpression.value.first == parse_value_data_type_enum::nan %s :: %i\n", bn->_leftExpression->function_token_definition.second.c_str(), hex_index);// testing only : delete comment out when not needed
            //if(bn->_rightExpression->value.first == parse_value_data_type_enum::nan) printf("logic_parser_class :: evaluate_parse_tree : DATA :: bn->_rightExpression.value.first == parse_value_data_type_enum::nan %s :: %i\n", bn->_rightExpression->function_token_definition.second.c_str(), hex_index);// testing only : delete comment out when not needed

            if (!functions.perform_binary_operator_function(bn)) {
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
            literal_node* ln = static_cast<literal_node*>(tt_node);
            if (!functions.get_hex_grid_value(ln, hex_grid, hex_index)) {
                //printf("logic_parser_class :: ParseSubExp Could not define hex node neigbour value for hex %s\n", ln->function_token_definition.second.c_str());
                return false;
            }
            //if(dn->value.first == parse_value_data_type_enum::nan) printf("logic_parser_class :: evaluate_parse_tree : DATA :: dn->value.first == parse_value_data_type_enum::nan %s :: %i\n", dn->function_token_definition.second.c_str(), hex_index);// testing only : delete comment out when not needed
            //printf("logic_parser_class :: evaluate_parse_tree : DATA :: Parse Tree hex grid value of %s :: \n", dn->function_token_definition.second.c_str());// testing only : delete comment out when not needed
            //display_evaluation_value(dn);// testing only : delete comment out when not needed

            //if(tt_node->value.first == parse_value_data_type_enum::nan) printf("logic_parser_class :: evaluate_parse_tree : DATA :: tt_node->value.first == parse_value_data_type_enum::nan %s :: %i\n", tt_node->function_token_definition.second.c_str(), hex_index);// testing only : delete comment out when not needed

            break;
        }

        case function_type_enum::value: {
            literal_node* ln = static_cast<literal_node*>(tt_node);
            //printf("logic_parser_class :: assign_node_value 1111 %s\n", tt_node->function_token_definition.second.c_str());
            if (!functions.assign_node_value(ln, parse_value_data_type_enum::Integer)) {
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

    void display_evaluation_value(parser_base_node* t_node) {
        switch (t_node->value.first) {
        case parse_value_data_type_enum::Boolean: printf("Boolean %i :: \n", t_node->value.second.bvalue); break;
        case parse_value_data_type_enum::Integer: printf("Integer %i :: \n", t_node->value.second.ivalue); break;
        case parse_value_data_type_enum::Float:   printf("Float   %f :: \n", t_node->value.second.fvalue); break;
        case parse_value_data_type_enum::Double:  printf("Double  %d :: \n", t_node->value.second.dvalue); break;
        }
    }

    // Create pasrer tree representing a C/C++ like conditional statement
    // expression and return pointer to top root node of the parser tree
    parser_base_node* Parse(const std::vector<std::string>& tokens) {
        size_t index = 0;
        return ParseExp(tokens, index);
    }

private:
    int number_left_braces = 0;// Number of left braces not balanced with a right brace
    int number_right_braces = 0;// Number of right braces not balanced with a left brace

    // Create pasrer tree representing a C/C++ like conditional statement
    // expression and return pointer to top root node of the parser tree
    // Read each node token one at a time and based upon the type of token
    // encountered, build a binary tree that when traveresed post order will
    // perform an evaluation of a C/C++ like conditional statement.
    // This is a recursive function that is very sensitive to the order tokens
    // in the token list being read left to right.

    // Tokens and index must be referenced or value cahages will be lost and creation of
    // parser tree will fail.

    // Braces do not form part of the parser tree but are used in the token list to define
    // parser tree sub branches and the top or root nodes of the sub branches that form
    // the child node of the parser tree node that is their parent. 
    parser_base_node* ParseExp(const std::vector<std::string>& tokens, size_t& index) {
        // create a left sub expresion parser node tree and have the top node as the left
        // expression node of this binary parser node. If only a left expression paser tree
        // is created, then the top parser node of that left expression tree is the top
        // root node of the exression. 
        auto leftExp = ParseSubExp(tokens, index);

        if (index >= tokens.size()) {
            return leftExp;// return the leftExp node to where the ParseExp was called 
        }

        const std::string& token = tokens[index];

        if (token == std::string(1, BRACE_RIGHT)) {// Encountered a right brace token
            number_right_braces++;
            if (number_left_braces < number_right_braces) { // have invalid brace encapsulation
                // error message
//printf("logic_parser_class :: ParseExp ERROR :Expected ')'\n", tokens[index].c_str());
                return nullptr;// return a nullptr to where the ParseExp was called 
            }

            // Update left right brace balance
            number_left_braces--;
            number_right_braces--;
            return leftExp;// return the leftExp node of this closed brace to where the ParseExp was called 
        }

        // Binary operators have a left and right expression that are child parser nodes
        if (have_binary_operator(token)) {
            //printf("logic_parser_class :: ParseExp have_binary_operator 000:%s\n", token.c_str());
            index++;
            auto rightExp = ParseExp(tokens, index);// create a right expresion parser node tree and have the top node as the right expression node of this binary parser node

            if (!rightExp) {
                afw_globalc::get_current_logger()->log(LogLevel::ERROR, "logic_parser_class :: ParseExp ERROR : No right expression defined for binary operator " + token + "\n");
                delete_pars_tree(leftExp);
                //printf("logic_parser_class :: ParseExp binary operator right exp 1111 \n");
                return nullptr;
            }
            //printf("logic_parser_class :: ParseExp binary operator right exp 2222 :%s\n", rightExp->function_token_definition.second.c_str());
            binary_node* b_node = new binary_node(std::move(leftExp), std::move(rightExp));// Create a binary parser node with the curent left and right root parser nodes as its child parser nodes
            b_node->function_token_definition.first = function_type_enum::binary;
            b_node->function_token_definition.second = token;
            //printf("logic_parser_class :: ParseExp have_binary_operator 3333 :%s\n", b_node->function_token_definition.second.c_str());
            return b_node; // return the binary node to where the ParseExp was called 
        }
        else {
            //printf("logic_parser_class :: ParseExp Expected '&&' or '||' or '^' or EOF :: %s\n", token.c_str());
            return nullptr;// return a nullptr to where the ParseExp was called 
        }
    }

    // Create a pasrer tree left branch of a main parser tree representing 
    // a C/C++ like conditional statement expression and return pointer to
    // the top root node of this left branch parser tree.
    // Read each node token one at a time and based upon the type of token
    // encountered, build a binary tree that when traveresed post order will
    // perform an evaluation of a C/C++ like conditional statement.
    // 
    // This is a recursive function that is very sensitive to the order tokens
    // in the token list being read left to right.

    // tokens and index must be referenced or value cahages will be lost and creation of
    // parser tree will fail.

    // Braces do not form part of the parser tree but are used in the token list to define
    // parser tree sub branches and the top or root nodes of the sub branches that form
    // the child node of the parser tree node that is their parent. 
    parser_base_node* ParseSubExp(const std::vector<std::string>& tokens, size_t& index) {
        const std::string& token = tokens[index];

        if (token.empty()) { return nullptr; }

        if (token == std::string(1, BRACE_LEFT)) {// Encountered a left brace token
            index++;
            number_left_braces++;// Add to the number of unbalanced token braces

            //printf("logic_parser_class :: ParseSubExp 000 %s\n", token.c_str());
            parser_base_node* node = ParseExp(tokens, index);// recursivly create a parser sub tree branch with the top node returned

            //if(node) printf("logic_parser_class :: ParseSubExp 111 %s : %s\n", node->function_token_definition.second.c_str(), tokens[index].c_str());

            if (index >= tokens.size() || tokens[index] != std::string(1, BRACE_RIGHT)) {
                //printf("logic_parser_class :: ParseSubExp ERROR :Expected ')'\n", tokens[index].c_str());
                return nullptr;
            }

            index++; // next parser token index
            return node;
        }

        if (token == std::string(1, BRACE_RIGHT)) {// This does not seem to work ????
            index++;
            number_right_braces++;

            if (number_left_braces < number_right_braces) {// Could have incomplete left brace ????
                return nullptr;
            }
        }

        if (have_unary_operator(token)) {// unary operator parser tree branch nodes created in this function only
            index++;// next parser token index
            parser_base_node* node = ParseExp(tokens, index);// recursivly create a parser sub tree branch with the top node returned
            unary_node* u_node = new unary_node(node);// create a parser tree unary node 
            u_node->function_token_definition.first = function_type_enum::unary;
            u_node->function_token_definition.second = token;
            //printf("logic_parser_class :: ParseSubExp unary_node 2222 %s\n", u_node->function_token_definition.second.c_str());
            return u_node; // return pointer to uniary node 
        }

        if (have_hex_grid_neighbour(token)) {// get hex grid cell data value and assign to the value of a paser literal node 
            index++;// next parser token index
            literal_node* l_node = new literal_node(token);// create a parser tree unary node 
            l_node->function_token_definition.first = function_type_enum::data;
            l_node->function_token_definition.second = token;
            //printf("logic_parser_class :: ParseSubExp 3333 literal_node %s\n", l_node->function_token_definition.second.c_str());
            return l_node;
        }

        // Only token type left to construct a tree node is a literal node that stores a constant value
        index++;// next parser token index
        literal_node* l_node = new literal_node(token);// create a parser tree literal node that stores a numerical value
        l_node->function_token_definition.first = function_type_enum::value;
        l_node->function_token_definition.second = token;
        //printf("logic_parser_class :: ParseSubExp 4444 literal_node #%s#|\n", l_node->function_token_definition.second.c_str());
        return l_node;
    }



protected:

private:
    //functions_class functions;
    hex_functions_class functions;

    std::vector<char> white_space_characters;
    std::vector<char> brace_charecters;
    std::vector<char> comparison_operator_characters;

    std::vector<std::string> unary_operators;
    std::vector<std::string> binary_operators;
    std::vector<std::string> set_operators;

    std::vector<std::string> hex_grid_neighbours;

    // Functions to initiate and define the categories of tokens
    // to be used to query what type of token is encountered while
    // creating the parser node expression tree

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
        //set_operators.push_back(MATH_SET_OPERATOR_SUM);
        //set_operators.push_back(MATH_SET_OPERATOR_AVERAGE);
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

    // Functions to test is a character or string of characters belongs
    // to a certain token category.

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
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool have_open_close_brace(char c) {
        for (char brace_charecter : brace_charecters) {
            if (brace_charecter == c) { return true; }
        }
        return false;
    }

    bool have_equal_character(char c) {
        if (c == COMPARISON_OPERATOR_EQUAL)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool have_comparison_operator_characters(char c) {
        for (char comparison_operator_character : comparison_operator_characters) {
            if (comparison_operator_character == c) { return true; }
        }
        return false;
    }

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

    bool have_binary_operator(std::string& token, std::string& prefix) {
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
};