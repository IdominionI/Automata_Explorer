#pragma once

#include <fstream>
#include <string>
#include <vector>

#include <Framework/AFW/Tools/afw_string_tools.h>
#include <Framework/AFW/Tools/afw_file_tools.h>
#include <Framework/AFW/Tools/afw_dialogs.h>// Seems cannot have or use this in a .h file. Get already has a body compile errors

#include "cart2D_grid.h"

#define OVER_WRITE_CART2D_GRID_FILE    100
#define READ_CART2D_GRID_FILE          101

#define BEGIN_HEADER_BLOCK  "["
#define END_HEADER_BLOCK	"]"
#define BEGIN_CART2D_GRID_DATA_BLOCK  "{"
#define END_CART2D_GRID_DATA_BLOCK    "}"

//data type flags
#define DATA_TYPE_BOOLEAN   0
#define DATA_TYPE_FLOAT     1
#define DATA_TYPE_INTEGER   2
#define DATA_TYPE_DOUBLE    3

template <typename T>
class cart2D_grid_class;

template <class T>
class cart2D_surface_import_export_class {
public:
	std::fstream stream;
	std::string input_line;

	int line_number = 0;

	bool export_cart2D_surface_automata_grid(std::string file_pathname, cart2D_grid_class<T> *cart2D_grid) {
		if (!cart2D_grid) {
			vwDialogs::display_error_message("Export cart2D grid", "ERROR ::\n Hex automata grid to export is undefined\nExport Hex surface grid aborted.\n");
			return false;
		}

		if (cart2D_grid->cart2D_grid_value_data_type == cart2D_grid_value_data_type_enum::nan) {
			vwDialogs::display_error_message("Export cart2D grid", "ERROR ::\n Hex automata grid to export data type is undefined\nExport Hex surface grid aborted.\n");
			return false;
		}

		if (file_pathname.size() == 0) {
			vwDialogs::display_error_message("Export cart2D grid", "ERROR ::\n No file name defined to export data to\nExport Hex surface grid aborted.\n");
			return false;
		}

		if (!open_file_stream(file_pathname, OVER_WRITE_CART2D_GRID_FILE)) {
			std::string error_message = "ERROR :: Unable to open file\n" + file_pathname + "\n to export cart2D surface grid data to.\n";
			vwDialogs::display_error_message("Export cart2D surface grid",error_message);
			return false;
		}

		write_header_block(cart2D_grid);
		write_cart2D_grid_data(cart2D_grid);

		close_file_stream();

		return true;
	}

	bool import_cart2D_surface_automata_grid(std::string file_pathname, cart2D_grid_class<T>* cart2D_grid) {
		if (!cart2D_grid) {
			vwDialogs::display_error_message("Import cart2D grid", "ERROR ::\n Hex automata grid to import into is undefined\nExport Hex surface grid aborted.\n");
			return false;
		}

		if (file_pathname.size() == 0) {
			vwDialogs::display_error_message("Import cart2D grid", "ERROR ::\nNo file name defined to import data from \n Import cart2D surface grid aborted\n");
			return false;
		}

		std::fstream working_model_file(file_pathname, std::ios::in);

		if (!working_model_file) {
			std::string error_message = "ERROR : \n Could not find/open cart2D file \n" + file_pathname + " to import data from.\n";
			vwDialogs::display_error_message("Import cart2D automata grid", error_message);
			return false;
		}

		std::string working_model_string = FW::filetools::read_all(working_model_file);// Read cart2D grid data text file into a string buffer

		std::vector<std::string> lines = FW::stringtools::split(working_model_string, '\n');// Split string buffer into a vector array represeting lines of text
		int line_number = 0; // variable representing file line number and lines vector array index number

		if (!import_cart2D_grid_data(lines, line_number, cart2D_grid)) {// Importation of cart2D grid data failed
			stream.close();

			std::string error_message = "ERROR :: Import cart2D grid data failed.\n Corrupted or out of sequence data at line\n";
			error_message += std::to_string(line_number) + "\n with entry  \n" + lines[line_number] + "\n";
			vwDialogs::display_error_message("Import cart2D grid data", error_message);

			return false;
		}

		stream.close();
		return true;
	}

private:
	bool  open_file_stream(std::string filename, int write_mode) {
		if (write_mode == OVER_WRITE_CART2D_GRID_FILE) {
			stream.open(filename, std::ios::out | std::ios::out);

			if (!stream) return false;// Must have QIODevice::Text or std::endl newline not written !!
		} else{
			if (!stream.is_open()) { return false; }
		}

		return true;
	}

	void close_file_stream() {
		stream.close();
	}

	void write_header_block(cart2D_grid_class<T>* cart2D_grid) {
		stream << BEGIN_HEADER_BLOCK << std::endl;

		stream << cart2D_grid->grid_dimension.x << std::endl;
		stream << cart2D_grid->grid_dimension.y << std::endl;
		switch (cart2D_grid->cart2D_grid_value_data_type) {
			case cart2D_grid_value_data_type_enum::Boolean : {stream << DATA_TYPE_BOOLEAN << std::endl; break; }
			case cart2D_grid_value_data_type_enum::Float   : {stream << DATA_TYPE_FLOAT   << std::endl; break; }
			case cart2D_grid_value_data_type_enum::Integer : {stream << DATA_TYPE_INTEGER << std::endl; break; }
			case cart2D_grid_value_data_type_enum::Double  : {stream << DATA_TYPE_DOUBLE  << std::endl; break; }
		}

		stream << END_HEADER_BLOCK << std::endl;
	}

	void write_cart2D_grid_data(cart2D_grid_class<T>* cart2D_grid) {
		stream << BEGIN_CART2D_GRID_DATA_BLOCK << std::endl;

		for (size_t i = 0; i < cart2D_grid->cart2D_grid.size(); i++) {
			stream << cart2D_grid->cart2D_grid[i] << std::endl;
		}

		stream << END_CART2D_GRID_DATA_BLOCK << std::endl;
	}

	bool import_cart2D_grid_data(std::vector<std::string> lines, int &line_number, cart2D_grid_class<T>* cart2D_grid) {
		std::string line;

		// First line read must be a START_HEADER_FLAG
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "import_export_byte_automata_rules_class::read_automata_byte_rules_into_byte_rules_parameters 1111 : " << line_number << ":" << lines[line_number] << ":" << std::endl;
		if (!FW::stringtools::contains(lines[line_number], BEGIN_HEADER_BLOCK)) { // There must ba a BEGIN_HEADER_BLOCK flag at the start to begin the cart2D grid data importation
//std::cout <<"Import Automata byte rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\nMissing START_HEADER_FLAG", //QMessageBox::Ok);
			vwDialogs::display_error_message("Import cart2D grid data", "ERROR : \n Could not find grid data header start block flag\n");
			return false;
		}

		//next 3 lines must have cart2D grid x dimension , y dimension and data type in this order
		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "import_export_cart2D_surface_automata_rules_class::read_automata_byte_rule BBBBB : " << line_number << ":" << lines[line_number] << std::endl;
		cart2D_grid->grid_dimension.x = stoi(line);

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "import_export_cart2D_surface_automata_rules_class::read_automata_byte_rule BBBBB : " << line_number << ":" << lines[line_number] << std::endl;
		cart2D_grid->grid_dimension.y = stoi(line);

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "import_export_cart2D_surface_automata_rules_class::read_automata_byte_rule BBBBB : " << line_number << ":" << lines[line_number] << std::endl;
		int data_type_id = stoi(line);

		switch (data_type_id) {
			case DATA_TYPE_BOOLEAN : {cart2D_grid->cart2D_grid_value_data_type = cart2D_grid_value_data_type_enum::Boolean; break;}
			case DATA_TYPE_FLOAT   : {cart2D_grid->cart2D_grid_value_data_type = cart2D_grid_value_data_type_enum::Float  ; break;}
			case DATA_TYPE_INTEGER : {cart2D_grid->cart2D_grid_value_data_type = cart2D_grid_value_data_type_enum::Integer; break;}
			case DATA_TYPE_DOUBLE  : {cart2D_grid->cart2D_grid_value_data_type = cart2D_grid_value_data_type_enum::Double ; break;}
		}

		line_number++;
		// next line read must be a END_HEADER_FLAG
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "import_export_byte_automata_rules_class::read_automata_byte_rules_into_byte_rules_parameters 1111 : " << line_number << ":" << lines[line_number] << ":" << std::endl;
		if (!FW::stringtools::contains(lines[line_number], END_HEADER_BLOCK)) { // There must ba a END_HEADER_FLAG flag at the start to begin the cart2D grid data importation
//std::cout <<"Import Automata byte rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\nMissing START_HEADER_FLAG", //QMessageBox::Ok);
			vwDialogs::display_error_message("Import cart2D grid data", "ERROR : \n Could not find grid data header end block flag\n");
			return false;
		}

		line_number++;
		// next line read must be a BEGIN_CART2D_GRID_DATA_BLOCK
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "import_export_byte_automata_rules_class::read_automata_byte_rules_into_byte_rules_parameters 1111 : " << line_number << ":" << lines[line_number] << ":" << std::endl;
		if (!FW::stringtools::contains(lines[line_number], BEGIN_CART2D_GRID_DATA_BLOCK)) { // There must ba a BEGIN_HEADER_BLOCK flag at the start to begin the cart2D grid data importation
//std::cout <<"Import Automata byte rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\nMissing START_HEADER_FLAG", //QMessageBox::Ok);
			vwDialogs::display_error_message("Import cart2D grid data", "ERROR : \n Could not find grid data block start flag\n");
			return false;
		}

		line_number++;
		while (!FW::stringtools::contains(lines[line_number], END_CART2D_GRID_DATA_BLOCK)) {
			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
			switch (data_type_id) {
				case DATA_TYPE_BOOLEAN : { int value    = stoi(line); cart2D_grid->cart2D_grid.push_back(value); break;}
				case DATA_TYPE_FLOAT   : { float value  = stof(line); cart2D_grid->cart2D_grid.push_back(value); break;}
				case DATA_TYPE_INTEGER : { int value    = stoi(line); cart2D_grid->cart2D_grid.push_back(value); break;}
				case DATA_TYPE_DOUBLE  : { double value = stod(line); cart2D_grid->cart2D_grid.push_back(value); break;}
			}
			line_number++;
		}

		//last line must contain END_CART2D_GRID_DATA_BLOCK
		if (!FW::stringtools::contains(lines[line_number], END_CART2D_GRID_DATA_BLOCK)) { // There must ba a BEGIN_HEADER_BLOCK flag at the start to begin the cart2D grid data importation
//std::cout <<"Import Automata byte rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\nMissing START_HEADER_FLAG", //QMessageBox::Ok);
			vwDialogs::display_error_message("Import cart2D grid data", "ERROR : \n Could not find grid data block start flag\n");
			return false;
		}
 
		return true;
	}

};