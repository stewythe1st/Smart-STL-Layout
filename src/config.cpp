
#include "config.h"
#include <fstream>


bool config::read(std::string filename) {

	// Variables
	std::ifstream	in;
	std::string		line;
	std::size_t		split;
	std::string		lhs;
	std::string		rhs;
	std::map<std::string, cfg_value>::iterator
					value;

	// Open file
	srcfile = filename;
	in.open(CFG_PATH + filename);
	if (!in.is_open()) {
		return false;
	}

	// Read in line-by-line
	while (getline(in, line)) {

		// Skip comment lines
		if (line[0] == COMMENT_CHAR)
			continue;

		// Handling for Windows-style line endings
		if (line.length() > 1 && line[line.length() - 1] == '\r')
			line.pop_back();

		// Split line into token(lhs) and value(rhs)
		split = line.find("=");
		if (split != std::string::npos) {
			lhs = line.substr(0, split);
			rhs = line.substr(split + 1);
		}
		
		// Parse string to value
		value = defs.find(lhs);
		if (value != defs.end()) {
			if ((*value).second.type == INT)
				*(int*)(*value).second.address = atoi(rhs.c_str());
			else if ((*value).second.type == STRING)
				*(std::string*)(*value).second.address = rhs;
		}		
	}

	// Clean up
	in.close();
	return true;
}
