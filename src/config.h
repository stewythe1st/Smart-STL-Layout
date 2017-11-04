#ifndef config_H
#define config_H

#include <string>
#include <map>

#define COMMENT_CHAR		('#')
#define DEFAULT_FILE		("default.cfg")
#define CFG_PATH			("./cfg/")

enum seedType {
	SEED_STATIC,
	SEED_TIME_BASED
};

enum dataType {
	INT,
	STRING
};

struct cfg_value {
	void* address;
	dataType type;
};

struct config {

	std::string	srcfile = DEFAULT_FILE;
	int			seed;
	int			seedType;
	int			runs;
	int			evals;
	int			mu;
	int			lambda;

	std::map<std::string, cfg_value> defs = {
		{ "seed", cfg_value{ &seed, INT } },
		{ "seedType", cfg_value{ &seedType, INT } },
		{ "runs", cfg_value{ &runs, INT } },
		{ "evals", cfg_value{ &evals, INT } },
		{ "mu", cfg_value{ &mu, INT } },
		{ "lambda", cfg_value{ &lambda, INT } }
	};

	// Constructors & Destructors
	inline config() { read(DEFAULT_FILE); };
	inline config(std::string filename) { read(filename); };

	// Member Functions
	bool read(std::string filename);

};
#endif