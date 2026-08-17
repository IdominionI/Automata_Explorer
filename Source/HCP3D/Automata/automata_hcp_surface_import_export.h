#pragma once
#include <fstream>
#include <string>
#include <vector>

#include "hcp_automata_dt.h"

#define OVER_WRITE_FUNCTION_FILE    100
#define READ_FUNCTION_FILE          101

#define RULE_DATA_START "[[["
#define RULE_DATA_END   "]]]"

#define RESULT_RULE_BLOCK_START "{{|"
#define RESULT_RULE_BLOCK_END   "|}}"

#define RESULT_RULE_DATA_BLOCK_START "({{"
#define RESULT_RULE_DATA_BLOCK_END   "}})"

#define RULE_BLOCK_START "((|"
#define RULE_BLOCK_END   "|))"

#define RULE_DATA_BLOCK_START "((-"
#define RULE_DATA_BLOCK_END   "-))"

#define SUB_RULE_BLOCK_START "<<|"
#define SUB_RULE_BLOCK_END   "|>>"

#define SUB_RULE_START "(<<"
#define SUB_RULE_END   ">>)"

#define SUB_RULE_DEFINITION_START "<<<"
#define SUB_RULE_DEFINITION_END   ">>>"

#define RULE_DELIMINATOR ":"

/*
	import_export_hcp_surface_automata_rules_class

	C++ Class that handles the import and export of cellula automata rule data from and to
	a formatted text file

	Method of file format to read/write this data is to flag each cellula automata rule with
	a RESULT_RULE_BLOCK_START and RULE_BLOCK_START on a line to indicate the following lines up 
	to a RESULT_RULE_BLOCK_END and RULE_BLOCK_START flag is a cellula automata rule. 
	The cellula automata rule is also in a particular order that dictates which line betweeen 
	these flags corresponds to a particular cellula automata rule prarmeter.
*/

class import_export_hcp_surface_automata_rules_class {
public:
	std::fstream stream;
	std::string input_line;

	int line_number = 0;

	std::vector <hcp_surface_automata_rule_result_struct_type>  hcp_surface_automata_result_rules;
	std::vector <hcp_surface_automata_rule_struct_type>			hcp_surface_automata_rules;

	bool export_hcp_surface_automata_rules(std::string file_pathname);

	bool save_automata_rule(std::string file_pathname, hcp_surface_automata_rule_struct_type rule);
	bool save_automata_sub_rule(std::string file_pathname, hcp_automata_sub_rule_struct_type sub_rule);

	bool import_hcp_surface_automata_rules(std::string file_pathname);

	bool import_automata_rule(std::string file_pathname, std::vector <hcp_surface_automata_rule_struct_type> &hcp_surface_automata_rules);
	bool import_automata_sub_rule(std::string file_pathname, hcp_surface_automata_rule_struct_type &hcp_surface_automata_rule);

private:
	// Export functions
	int  open_file_stream(std::string filename, int write_mode);
	void close_file_stream();
	void write_hcp_grid_result_rules(std::fstream& stream);
	void write_result_rule_automata_rules(std::fstream& stream);

	//Import functions
	bool import_hcp_automata_rules(std::vector<std::string> lines, int& line_number);
	bool import_hcp_surface_automata_result_rules(std::vector<std::string> lines, int& line_number);
	bool import_hcp_surface_automata_rules(std::vector<std::string> lines, int& line_number);
	bool import_hcp_surface_automata_sub_rules(std::vector<std::string> lines, hcp_surface_automata_rule_struct_type& hcp_surface_automata_rule, int& line_number);

	int error_code = 0;
};
