#ifndef config_H
#define config_H

#include <string>
#include <map>
#include <vector>

#define COMMENT_CHAR		('#')
#define DEFAULT_FILE		("default.cfg")
#define CFG_PATH			("./cfg/")

enum seedType {
	SEED_STATIC,
	SEED_TIME_BASED
};

enum termTestType {
	TERMTEST_EVALS,
	TERMTEST_GENS,
	TERMTEST_BESTUNCHANGED
};

enum dataType {
	INT,
	FLOAT,
	FLOAT_VECTOR,
	STRING,
	STRING_VECTOR
};

struct cfg_value {
	void* address;
	dataType type;
};

struct config {

	std::string					srcfile = DEFAULT_FILE;
	std::vector<std::string>	stls;
	std::vector<float>			stl_scales;
	std::string					name;
	int							width;
	int							height;
	int							seed;
	int							seedType;
	int							runs;
	int							evals;
	int							mu;
	int							lambda;
	int							parentSelTournSize;
	int							survivorSelTournSize;
	int							termTest;
	int							termTarget;
	int							crossovers;
	float						mutationRate;
	int							creepDist;
	int							rotationalRestriction;

	std::map<std::string, cfg_value> defs = {
		{ "name", cfg_value{ &name, STRING } },
		{ "stl", cfg_value{ &stls, STRING_VECTOR } },
		{ "scale", cfg_value{ &stl_scales, FLOAT_VECTOR } },
		{ "width", cfg_value{ &width, INT } },
		{ "height", cfg_value{ &height, INT } },
		{ "seed", cfg_value{ &seed, INT } },
		{ "seedType", cfg_value{ &seedType, INT } },
		{ "runs", cfg_value{ &runs, INT } },
		{ "evals", cfg_value{ &evals, INT } },
		{ "mu", cfg_value{ &mu, INT } },
		{ "lambda", cfg_value{ &lambda, INT } },
		{ "parentSelTournSize", cfg_value{ &parentSelTournSize, INT } },
		{ "survivorSelTournSize", cfg_value{ &survivorSelTournSize, INT } },
		{ "termTest", cfg_value{ &termTest, INT } },
		{ "termTarget", cfg_value{ &termTarget, INT } },
		{ "crossovers", cfg_value{ &crossovers, INT } },
		{ "mutationRate", cfg_value{ &mutationRate, FLOAT } },
		{ "creepDist", cfg_value{ &creepDist, INT } },
		{ "rotationalRestriction", cfg_value{ &rotationalRestriction, INT } }
	};

	// Constructors & Destructors
	inline config() { read(DEFAULT_FILE); };
	inline config(std::string filename) { read(filename); };

	// Member Functions
	bool read(std::string filename);

};
#endif