#include "automata_hex_surface_import_export.h"

#include <Framework/AFW/Tools/afw_string_tools.h>
#include <Framework/AFW/Tools/afw_file_tools.h>
#include <Framework/AFW/Tools/afw_dialogs.h>// Seems cannot have or use this in a .h file. Get already has a body compile errors

	int import_export_hex_surface_automata_rules_class::open_file_stream(std::string filename, int write_mode) {

		if (write_mode == OVER_WRITE_FUNCTION_FILE) {
			stream.open(filename, std::ios::out | std::ios::out);

			if (!stream) { return false; }// Must have QIODevice::Text or std::endl newline not written !!
		}
		else
			if (!stream.is_open()) { return false; }

		return true;
	}

	void import_export_hex_surface_automata_rules_class::close_file_stream() {
		stream.close();
	}

	bool import_export_hex_surface_automata_rules_class::export_hex_surface_automata_rules(std::string file_pathname) {
		if (file_pathname.size() == 0) {
			vwDialogs::display_error_message("Export hex automata rules", "ERROR ::\n No file name defined to save data to\nSave voxel automata byte rules aborted.\n");
			return false;
		}

		if (!open_file_stream(file_pathname, OVER_WRITE_FUNCTION_FILE)) {
			std::string error_message = "ERROR :: Unable to open file\n" + file_pathname + "\n to save automata data to.\n";
			vwDialogs::display_error_message("Export hex automata rules",error_message);
			return false;
		}

		stream << RULE_DATA_START << std::endl;

		stream << RESULT_RULE_BLOCK_START << std::endl;
		write_hex_grid_result_rules(stream);
		stream << RESULT_RULE_BLOCK_END << std::endl;

		stream << RULE_BLOCK_START << std::endl;
		write_result_rule_automata_rules(stream);
		stream << RULE_BLOCK_END << std::endl;

		stream << RULE_DATA_END << std::endl;

		close_file_stream();

		return true;
	}

	void import_export_hex_surface_automata_rules_class::write_hex_grid_result_rules(std::fstream& stream) {

		for(hex_surface_automata_rule_result_struct_type result_rule : hex_surface_automata_result_rules){
			stream << RESULT_RULE_DATA_BLOCK_START << std::endl;

			stream << result_rule.result_name			<< std::endl;
			stream << result_rule.active_result			<< std::endl;
			stream << result_rule.rule_start_step		<< std::endl;
			stream << result_rule.rule_end_step			<< std::endl;
			stream << result_rule.hex_result_definition << std::endl;
			stream << result_rule.hex_result_value		<< std::endl;
			stream << result_rule.result_display_color.x << RULE_DELIMINATOR << result_rule.result_display_color.y << RULE_DELIMINATOR << result_rule.result_display_color.z << RULE_DELIMINATOR << result_rule.result_display_color.w << std::endl;
			stream << result_rule.display_shape_id      << std::endl;

			stream << RESULT_RULE_DATA_BLOCK_END << std::endl;
		}
	}

	void import_export_hex_surface_automata_rules_class::write_result_rule_automata_rules(std::fstream& stream){
		
		for (hex_surface_automata_rule_struct_type rule : hex_surface_automata_rules) {
			stream << RULE_DATA_BLOCK_START << std::endl;

			stream << rule.rule_name		<< std::endl;
			//stream << rule.active_rule		<< std::endl;
			//stream << rule.rule_start_step	<< std::endl;
			//stream << rule.rule_end_step	<< std::endl;

			stream << SUB_RULE_BLOCK_START << std::endl;
			for (hex_automata_sub_rule_struct_type sub_rule : rule.sub_rules) {
				stream << SUB_RULE_START			   << std::endl;

				stream << sub_rule.sub_rule_name       << std::endl;
				stream << sub_rule.active_sub_rule     << std::endl;

				stream << SUB_RULE_DEFINITION_START    << std::endl;
				stream << sub_rule.sub_rule_definition << std::endl;
				stream << SUB_RULE_DEFINITION_END      << std::endl;

				stream << SUB_RULE_END				   << std::endl;
			}
			stream << SUB_RULE_BLOCK_END << std::endl;

			stream << RULE_DATA_BLOCK_END << std::endl;
		}

	}

	bool import_export_hex_surface_automata_rules_class::save_automata_rule(std::string file_pathname, hex_surface_automata_rule_struct_type rule) {
		if (file_pathname.size() == 0) {
			vwDialogs::display_error_message("Export hex automata sub rule", "ERROR ::\n No file name defined to save rule data to\nSave automata sub rules aborted.\n");
			return false;
		}

		if (!open_file_stream(file_pathname, OVER_WRITE_FUNCTION_FILE)) {
			std::string error_message = "ERROR :: Unable to open file\n" + file_pathname + "\n to save automata data to.\n";
			vwDialogs::display_error_message("Export hex automata rules", error_message);
			return false;
		}

		stream << RULE_DATA_BLOCK_START << std::endl;

		stream << rule.rule_name		<< std::endl;
		//stream << rule.active_rule		<< std::endl;
		//stream << rule.rule_start_step	<< std::endl;
		//stream << rule.rule_end_step	<< std::endl;

		stream << SUB_RULE_BLOCK_START << std::endl;
		for (hex_automata_sub_rule_struct_type sub_rule : rule.sub_rules) {
			stream << SUB_RULE_START << std::endl;

			stream << sub_rule.sub_rule_name		<< std::endl;
			stream << sub_rule.active_sub_rule		<< std::endl;

			stream << SUB_RULE_DEFINITION_START		<< std::endl;
			stream << sub_rule.sub_rule_definition	<< std::endl;
			stream << SUB_RULE_DEFINITION_END		<< std::endl;

			stream << SUB_RULE_END << std::endl;
		}
		stream << SUB_RULE_BLOCK_END << std::endl;

		stream << RULE_DATA_BLOCK_END << std::endl;

		close_file_stream();

		return true;
	}

	bool import_export_hex_surface_automata_rules_class::save_automata_sub_rule(std::string file_pathname, hex_automata_sub_rule_struct_type sub_rule) {
		if (file_pathname.size() == 0) {
			vwDialogs::display_error_message("Export hex automata sub rule", "ERROR ::\n No file name defined to save sub rule data to\nSave automata sub rules aborted.\n");
			return false;
		}

		if (!open_file_stream(file_pathname, OVER_WRITE_FUNCTION_FILE)) {
			std::string error_message = "ERROR :: Unable to open file\n" + file_pathname + "\n to save automata data to.\n";
			vwDialogs::display_error_message("Export hex automata rules", error_message);
			return false;
		}


		stream << SUB_RULE_START << std::endl;

		stream << sub_rule.sub_rule_name		<< std::endl;
		stream << sub_rule.active_sub_rule		<< std::endl;

		stream << SUB_RULE_DEFINITION_START		<< std::endl;
		stream << sub_rule.sub_rule_definition	<< std::endl;
		stream << SUB_RULE_DEFINITION_END		<< std::endl;

		stream << SUB_RULE_END << std::endl;

		close_file_stream();

		return true;
	}

	// -----------------------------------------------------------------------------------------------------------

	bool import_export_hex_surface_automata_rules_class::import_hex_surface_automata_rules(std::string file_pathname) {
		if (file_pathname.size() == 0) {
			vwDialogs::display_error_message("Import hex automata rules", "ERROR ::\nNo file name defined to import data from \n Import automata rules aborted\n");
			return false;
		}

		std::fstream working_model_file(file_pathname, std::ios::in);

		if (!working_model_file) {
//std::cout << "Import voxel generation model", "Import voxel generation model ERROR : \n Could not find read Import voxel generation model defined in file \n" +
//																 file_pathname, //QMessageBox::Ok);

			std::string error_message = "ERROR : \n Could not find read Import voxel generation model defined in file \n" + file_pathname + "\n";
			vwDialogs::display_error_message("Import hex automata rules",error_message);
			return false;
		}

		hex_surface_automata_result_rules.clear();
		hex_surface_automata_result_rules.shrink_to_fit();

		hex_surface_automata_rules.clear();
		hex_surface_automata_rules.shrink_to_fit();

		std::string working_model_string = FW::filetools::read_all(working_model_file);// Read hex grid data text file into a string buffer

		std::vector<std::string> lines = FW::stringtools::split(working_model_string, '\n');// Split string buffer into a vector array represeting lines of text
		int line_number = 0;// variable representing file line number and lines vector array index number

		if (!import_hex_automata_rules(lines, line_number)) {// Importation of hex automata data failed
			stream.close();
			
			std::string error_message = "ERROR :: Import hex automata data failed.\n Corrupted or out of sequence data at line\n";
			error_message += std::to_string(line_number) + "\n with entry  \n" + lines[line_number] + "\n";
			vwDialogs::display_error_message("Import hex automata rules",error_message);

			return false;
		}

		stream.close();
		return true;
	}

	bool import_export_hex_surface_automata_rules_class::import_hex_automata_rules(std::vector<std::string> lines, int& line_number) {
		std::string line;

		// First line read must be a START_HEADER_FLAG
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "import_export_byte_automata_rules_class::read_automata_byte_rules_into_byte_rules_parameters 1111 : " << line_number << ":" << lines[line_number] << ":" << std::endl;
		if (!FW::stringtools::contains(lines[line_number], RULE_DATA_START)) { // There must ba a RULE_DATA_START flag at the start to begin the automata rules importation
		//if (!FW::stringtools::contains(line, RULE_DATA_START)) { // There msut allways be at least two nodes. The input and output link nodes
//std::cout <<"Import Automata byte rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\nMissing START_HEADER_FLAG", //QMessageBox::Ok);
			vwDialogs::display_error_message("Import hex automata rules", "ERROR : \n Could not find automata rule data start block flag\n");
			return false;
		}

//std::cout << "import_export_byte_automata_rules_class::read_automata_byte_rules_into_byte_rules_parameters 2222 : " << line_number << ":" << lines[line_number] << std::endl;

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		if (!FW::stringtools::contains(lines[line_number], RESULT_RULE_BLOCK_START)) { // This line must be the start of the rule results data block
//std::cout <<"Import Automata byte rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\nMissing START_HEADER_FLAG", //QMessageBox::Ok);
			vwDialogs::display_error_message("Import hex automata rules", "ERROR : \n Could not find automata rule result data start block flag\n");
			return false;
		}

		while (!FW::stringtools::contains(lines[line_number], RESULT_RULE_BLOCK_END)) {
			if (!import_hex_surface_automata_result_rules(lines, line_number)) {
				vwDialogs::display_error_message("Import hex automata rules", "ERROR : \n Could not import hex automata result rule(s)\n");
				return false;
			}
		}

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "import_export_byte_automata_rules_class::read_automata_byte_rules_into_byte_rules_parameters 3333 : " << line_number << ":" << lines[line_number] << std::endl;
		
		if (!FW::stringtools::contains(lines[line_number], RULE_BLOCK_START)) { // The next line must be a flag to signal the start of importing the atomata block of rules 
//std::cout <<"Import Automata byte rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\nMissing START_HEADER_FLAG", //QMessageBox::Ok);
			vwDialogs::display_error_message("Import hex automata rules", "ERROR : \n Could not find automata rule data start block flag\n");
			return false;
		}	

		while (!FW::stringtools::contains(lines[line_number], RULE_BLOCK_END)) {
			if (!import_hex_surface_automata_rules(lines, line_number)) {
				vwDialogs::display_error_message("Import hex automata rules", "ERROR : \n Could not import hex automata result rule(s)\n");
				return false;
			}
		}

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		if (!FW::stringtools::contains(lines[line_number], RULE_DATA_END)) { // The next line must be a flag to signal the start of importing the atomata block of rules 
//std::cout <<"Import Automata byte rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\nMissing START_HEADER_FLAG", //QMessageBox::Ok);
			vwDialogs::display_error_message("Import hex automata rules", "INFO : \n Could not find automata rule data end block flag\n");
		}

		return true;
	}

	bool import_export_hex_surface_automata_rules_class::import_hex_surface_automata_result_rules(std::vector<std::string> lines, int& line_number) {
		hex_surface_automata_rule_result_struct_type hex_surface_automata_rule_result;
		std::string line;
		line_number++;

		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		if (!FW::stringtools::contains(lines[line_number], RESULT_RULE_DATA_BLOCK_START)) { // This line must be the start of the rule results data block
//std::cout <<"Import Automata byte rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\nMissing START_HEADER_FLAG", //QMessageBox::Ok);
			vwDialogs::display_error_message("Import hex automata rules", "ERROR : \n Could not find automata rule result data start block flag\n");
			return false;
		}

		//next 7 lines must have rule result name,active rule result ,start_step,end_step , rule result definition,valid rule value, result rule display color in this order
		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "import_export_hex_surface_automata_rules_class::read_automata_byte_rule BBBBB : " << line_number << ":" << lines[line_number] << std::endl;
		hex_surface_automata_rule_result.result_name = line;

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "import_export_hex_surface_automata_rules_class::read_automata_byte_rule BBBBB : " << line_number << ":" << lines[line_number] << std::endl;
		bool hrule;
		if (!FW::stringtools::string_to_bool(line, &hrule, error_code)) { return false; }
		if (hrule == 0){ hex_surface_automata_rule_result.active_result = false;} else {hex_surface_automata_rule_result.active_result = true;}

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		if (!FW::stringtools::string_to_int(line, &hex_surface_automata_rule_result.rule_start_step, error_code)) { return false; }

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		if (!FW::stringtools::string_to_int(line, &hex_surface_automata_rule_result.rule_end_step, error_code)) { return false; }

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		hex_surface_automata_rule_result.hex_result_definition = line;

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		hex_surface_automata_rule_result.hex_result_value = line;

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		std::vector<std::string> color = FW::stringtools::split(line, RULE_DELIMINATOR);
		if (color.size() != 4) { return false; }

		for (std::string s_color : color) {
			float f_color;
			try { f_color = stof(s_color); }
			//catch (std::invalid_argument const& ex)
			catch (std::invalid_argument)
			{
				vwDialogs::display_error_message("Import hex automata rules", "ERROR : \n Have invalid color for result rules at line " + std::to_string(line_number) + "!\n");
				return false;
			}
			//catch (std::out_of_range const& ex)
			catch (std::out_of_range)
			{
				vwDialogs::display_error_message("Import hex automata rules", "ERROR : \n Have color out of range for result rules at line " + std::to_string(line_number) + "!\n");
				return false;
			}
		}

		hex_surface_automata_rule_result.result_display_color = { stof(color[0]),stof(color[1]),stof(color[2]),stof(color[3]) };

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		if (!FW::stringtools::string_to_int(line, &hex_surface_automata_rule_result.display_shape_id, error_code)) { return false; }

		// Next line read must be a RESULT_RULE_DATA_BLOCK_END flag
		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		if (!FW::stringtools::contains(lines[line_number], RESULT_RULE_DATA_BLOCK_END)) { // This line must be the start of the rule results data block
			vwDialogs::display_error_message("Import hex automata rules", "ERROR : \n Could not find automata rule result data end block flag at line "+std::to_string(line_number)+"\n");
			return false;
		}

		hex_surface_automata_result_rules.push_back(hex_surface_automata_rule_result);

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		if (FW::stringtools::contains(lines[line_number], RESULT_RULE_DATA_BLOCK_START)) { line_number--; }// Have another rules result data block to read and add

		return true;
	}

	bool import_export_hex_surface_automata_rules_class::import_hex_surface_automata_rules(std::vector<std::string> lines, int& line_number) {
		hex_surface_automata_rule_struct_type hex_surface_automata_rule;
		std::string line;
		line_number++;

		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		if (!FW::stringtools::contains(lines[line_number], RULE_DATA_BLOCK_START)) { // This line must be the start of the rule results data block
//std::cout <<"Import Automata byte rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\nMissing START_HEADER_FLAG", //QMessageBox::Ok);
			vwDialogs::display_error_message("Import hex automata rules", "ERROR : \n Could not find automata rule data block start block flag\n");
			return false;
		}

		//next 4 lines must have rule name,active rule , start_step, end_step  in this order
		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "import_export_hex_surface_automata_rules_class::read_automata_byte_rule BBBBB : " << line_number << ":" << lines[line_number] << std::endl;
		hex_surface_automata_rule.rule_name = line;

//		line_number++;
//		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
////std::cout << "import_export_hex_surface_automata_rules_class::read_automata_byte_rule BBBBB : " << line_number << ":" << lines[line_number] << std::endl;
//		bool hrule;
//		if (!FW::stringtools::string_to_bool(line, &hrule, error_code)) { return false; }
//		if (hrule == 0) {hex_surface_automata_rule.active_rule = false; }else {hex_surface_automata_rule.active_rule = true;}
//
//		line_number++;
//		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//		if (!FW::stringtools::string_to_int(line, &hex_surface_automata_rule.rule_start_step, error_code)) { return false; }
//
//		line_number++;
//		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//		if (!FW::stringtools::string_to_int(line, &hex_surface_automata_rule.rule_end_step, error_code)) { return false; }

		// Next line read must be a SUB_RULE_BLOCK_START flag to indicate to load rule sub rules
		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		if (!FW::stringtools::contains(lines[line_number], SUB_RULE_BLOCK_START)) { // This line must be the start of the rule results data block
			vwDialogs::display_error_message("Import hex automata rules", "ERROR : \n Could not find automata sub rule data block start flag at line " + std::to_string(line_number) + "\n");
			return false;
		}

		while (!FW::stringtools::contains(lines[line_number], SUB_RULE_BLOCK_END)) {
			if (!import_hex_surface_automata_sub_rules(lines, hex_surface_automata_rule,line_number)) {
				vwDialogs::display_error_message("Import hex automata rules", "ERROR : \n Could not import hex automata result rule(s)\n");
				return false;
			}
		}

		hex_surface_automata_rules.push_back(hex_surface_automata_rule);

//std::cout << "import_export_hex_surface_automata_rules_class::import_hex_surface_automata_rules AAAAAAA : " << line_number << ":" << lines[line_number] << ":" << hex_surface_automata_rules.size() << std::endl;

		// Next line read must be a RULE_DATA_BLOCK_END flag
		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		if (!FW::stringtools::contains(lines[line_number], RULE_DATA_BLOCK_END)) { // This line must be the start of the rule results data block
			vwDialogs::display_error_message("Import hex automata rules", "ERROR : \n Could not find automata rule data end block flag at line " + std::to_string(line_number) + "\n");
			return false;
		}

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		if (FW::stringtools::contains(lines[line_number], RULE_DATA_BLOCK_START)) { line_number--; } // Have another rules result data block to read and add

		return true;
	}

	bool import_export_hex_surface_automata_rules_class::import_hex_surface_automata_sub_rules(std::vector<std::string> lines, hex_surface_automata_rule_struct_type &hex_surface_automata_rule, int &line_number) {
		hex_automata_sub_rule_struct_type hex_automata_sub_rule;

		std::string line;
		line_number++;

		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "import_export_hex_surface_automata_rules_class::import_hex_surface_automata_sub_rules AAAAAAA : " << line_number << ":" << lines[line_number] << std::endl;
		// next line read must be a SUB_RULE_START flag
		if (!FW::stringtools::contains(lines[line_number], SUB_RULE_START)) {
			vwDialogs::display_error_message("Read hex automata rules", "ERROR : \nMissing sub rule block start flag\n");
			return false;
		}

		//next 2 lines must have sub rule name and sub rule active_rule in that order
		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "import_export_hex_surface_automata_rules_class::read_automata_byte_rule BBBBB : " << line_number << ":" << lines[line_number] << std::endl;
		hex_automata_sub_rule.sub_rule_name = line;

		line_number++;
		line = lines[line_number];
//std::cout << "import_export_hex_surface_automata_rules_class::read_automata_byte_rule CCCCC : " << line_number << ":" << lines[line_number] << std::endl;
		bool hrule;
		if (!FW::stringtools::string_to_bool(line, &hrule, error_code)) { return false; }
		if(hrule == 0) {hex_automata_sub_rule.active_sub_rule = false;} else {hex_automata_sub_rule.active_sub_rule = true;}

		//next line(s) between SUB_RULE_DEFINITION_START and SUB_RULE_DEFINITION_END flags define the sub rule code

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "import_export_hex_surface_automata_rules_class::import_hex_surface_automata_rules AAAAAAA : " << line_number << ":" << lines[line_number] << std::endl;
		// next line read must be a SUB_RULE_START flag
		if (!FW::stringtools::contains(lines[line_number], SUB_RULE_DEFINITION_START)) {
			vwDialogs::display_error_message("Read hex automata rules", "ERROR : \nMissing sub rule code block start flag\n");
			return false;
		}

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		while (!FW::stringtools::contains(lines[line_number], SUB_RULE_DEFINITION_END)) {
			hex_automata_sub_rule.sub_rule_definition += line;
			line_number++;
			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		}

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		if (!FW::stringtools::contains(lines[line_number], SUB_RULE_END)) {
			vwDialogs::display_error_message("Read hex automata rules", "ERROR : \nMissing sub rule data end flag\n");
			return false;
		}

		hex_surface_automata_rule.sub_rules.push_back(hex_automata_sub_rule);

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		if (FW::stringtools::contains(lines[line_number], SUB_RULE_START)) { line_number--; } // Have another sub rule data block to read and add

		return true;
	}

	bool import_export_hex_surface_automata_rules_class::import_automata_rule(std::string file_pathname, std::vector <hex_surface_automata_rule_struct_type> &hex_surface_automata_rules) {
		if (file_pathname.size() == 0) {
			vwDialogs::display_error_message("Import hex automata rule", "ERROR ::\nNo file name defined to import rule data from \n Import automata rule aborted\n");
			return false;
		}

		std::fstream working_model_file(file_pathname, std::ios::in);

		if (!working_model_file) {
			std::string error_message = "ERROR : \n Could not find read Import automata rule defined in file \n" + file_pathname + "\n";
			vwDialogs::display_error_message("Import hex automata rules", error_message);
			return false;
		}

		std::string working_model_string = FW::filetools::read_all(working_model_file);// Split string buffer into a vector array represeting lines of text

		std::vector<std::string> lines = FW::stringtools::split(working_model_string, '\n');// Split string buffer into a vector array represeting lines of text
		int line_number = 0;// variable representing file line number and lines vector array index number
	
		hex_surface_automata_rule_struct_type hex_surface_automata_rule;
		std::string line;

		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		if (!FW::stringtools::contains(lines[line_number], RULE_DATA_BLOCK_START)) { // This line must be the start of the rule results data block
//std::cout <<"Import Automata byte rules", "File read Error : Read error at line " + std::string::number(line_number) + " of file \n" + file_pathname + "\nMissing START_HEADER_FLAG", //QMessageBox::Ok);
			vwDialogs::display_error_message("Import hex automata rules", "ERROR : \n Could not find automata rule data block start block flag\n");
			return false;
		}

		//next 4 lines must have rule name,active rule , start_step, end_step  in this order
		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "import_export_hex_surface_automata_rules_class::read_automata_byte_rule BBBBB : " << line_number << ":" << lines[line_number] << std::endl;
		hex_surface_automata_rule.rule_name = line;

//		line_number++;
//		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
////std::cout << "import_export_hex_surface_automata_rules_class::read_automata_byte_rule BBBBB : " << line_number << ":" << lines[line_number] << std::endl;
//		bool hrule;
//		if (!FW::stringtools::string_to_bool(line, &hrule, error_code)) { return false; }
//		if (hrule == 0) {hex_surface_automata_rule.active_rule = false;} else {hex_surface_automata_rule.active_rule = true;}
//
//		line_number++;
//		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//		if (!FW::stringtools::string_to_int(line, &hex_surface_automata_rule.rule_start_step, error_code)) { return false; }
//
//		line_number++;
//		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//		if (!FW::stringtools::string_to_int(line, &hex_surface_automata_rule.rule_end_step, error_code)) { return false; }

		// Next line read must be a SUB_RULE_BLOCK_START flag to indicate to load rule sub rules
		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		if (!FW::stringtools::contains(lines[line_number], SUB_RULE_BLOCK_START)) { // This line must be the start of the rule results data block
			vwDialogs::display_error_message("Import hex automata rules", "ERROR : \n Could not find automata sub rule data block start flag at line " + std::to_string(line_number) + "\n");
			return false;
		}

		while (!FW::stringtools::contains(lines[line_number], SUB_RULE_BLOCK_END)) {
			if (!import_hex_surface_automata_sub_rules(lines, hex_surface_automata_rule,line_number)) {
				vwDialogs::display_error_message("Import hex automata rules", "ERROR : \n Could not import hex automata result rule(s)\n");
				return false;
			}
		}

		hex_surface_automata_rules.push_back(hex_surface_automata_rule);
//std::cout << "import_export_hex_surface_automata_rules_class::import_hex_surface_automata_rules AAAAAAA : " << line_number << ":" << lines[line_number] << ":" << hex_surface_automata_rules.size() << std::endl;
		// Next line read must be a RULE_DATA_BLOCK_END flag
		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		if (!FW::stringtools::contains(lines[line_number], RULE_DATA_BLOCK_END)) { // This line must be the start of the rule results data block
			vwDialogs::display_error_message("Import hex automata rules", "ERROR : \n Could not find automata rule data end block flag at line " + std::to_string(line_number) + "\n");
			return false;
		}

		stream.close();
		return true;
	}

	bool import_export_hex_surface_automata_rules_class::import_automata_sub_rule(std::string file_pathname, hex_surface_automata_rule_struct_type& hex_surface_automata_rule) {
		if (file_pathname.size() == 0) {
			vwDialogs::display_error_message("Import hex automata sub rule", "ERROR ::\nNo file name defined to import data from \n Import automata sub rule aborted\n");
			return false;
		}

		std::fstream working_model_file(file_pathname, std::ios::in);

		if (!working_model_file) {
//std::cout << "Import voxel generation model", "Import voxel generation model ERROR : \n Could not find read Import voxel generation model defined in file \n" +
//																 file_pathname, //QMessageBox::Ok);

			std::string error_message = "ERROR : \n Could not find read Import voxel generation model defined in file \n" + file_pathname + "\n";
			vwDialogs::display_error_message("Import hex automata rules", error_message);
			return false;
		}

		std::string working_model_string = FW::filetools::read_all(working_model_file);// Split string buffer into a vector array represeting lines of text

		std::vector<std::string> lines = FW::stringtools::split(working_model_string, '\n');// Split string buffer into a vector array represeting lines of text
		int line_number = 0;// variable representing file line number and lines vector array index number

		hex_automata_sub_rule_struct_type hex_automata_sub_rule;

		std::string line;

		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "import_export_hex_surface_automata_rules_class::import_hex_surface_automata_sub_rules AAAAAAA : " << line_number << ":" << lines[line_number] << std::endl;
		// next line read must be a SUB_RULE_START flag
		if (!FW::stringtools::contains(lines[line_number], SUB_RULE_START)) {
			vwDialogs::display_error_message("Read hex automata rules", "ERROR : \nMissing sub rule block start flag\n");
			return false;
		}

		//next 2 lines must have sub rule name and sub rule active_rule in that order
		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "import_export_hex_surface_automata_rules_class::read_automata_byte_rule BBBBB : " << line_number << ":" << lines[line_number] << std::endl;
		hex_automata_sub_rule.sub_rule_name = line;

		line_number++;
		line = lines[line_number];
//std::cout << "import_export_hex_surface_automata_rules_class::read_automata_byte_rule CCCCC : " << line_number << ":" << lines[line_number] << std::endl;

		bool hrule;
		if (!FW::stringtools::string_to_bool(line, &hrule, error_code)) { return false; }
		if (hrule == 0){ hex_automata_sub_rule.active_sub_rule = false;} else {hex_automata_sub_rule.active_sub_rule = true;}
		//next line(s) between SUB_RULE_DEFINITION_START and SUB_RULE_DEFINITION_END flags define the sub rule code
		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
//std::cout << "import_export_hex_surface_automata_rules_class::import_hex_surface_automata_rules AAAAAAA : " << line_number << ":" << lines[line_number] << std::endl;
		// next line read must be a SUB_RULE_START flag
		if (!FW::stringtools::contains(lines[line_number], SUB_RULE_DEFINITION_START)) {
			vwDialogs::display_error_message("Read hex automata rules", "ERROR : \nMissing sub rule code block start flag\n");
			return false;
		}

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		while (!FW::stringtools::contains(lines[line_number], SUB_RULE_DEFINITION_END)) {
			hex_automata_sub_rule.sub_rule_definition += line;
			line_number++;
			line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		}

		line_number++;
		line = lines[line_number]; line = FW::stringtools::truncate(line, line.size());
		if (!FW::stringtools::contains(lines[line_number], SUB_RULE_END)) {
			vwDialogs::display_error_message("Read hex automata rules", "ERROR : \nMissing sub rule data end flag\n");
			return false;
		}

		hex_surface_automata_rule.sub_rules.push_back(hex_automata_sub_rule);

		stream.close();
		return true;
	}