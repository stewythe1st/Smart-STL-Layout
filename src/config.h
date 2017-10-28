#ifndef config_H
#define config_H

#include <string>
#include <map>

#define COMMENT_CHAR		('#')
#define DEFAULT_FILE		("default.cfg")
#define CFG_PATH			("./cfg/")

class config {

private:
	std::string	srcfile = DEFAULT_FILE;
	int	mu = 10;
	int	lambda = 20;
	std::map<std::string, int*>	defs = { {"mu",&mu},{"lambda",&lambda} };
public:

	// Constructors & Destructors
	inline config() { read(DEFAULT_FILE); };
	inline config(std::string filename) { read(filename); };

	// Member Functions
	bool read(std::string filename);

};
#endif