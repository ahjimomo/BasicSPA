#pragma once
#include <sstream>
#include <fstream>
#include <vector>
#include "Database.h"
#include "Tokenizer.h"

#include <list>

using namespace std;

class SourceProcessor {
public:
	// Declaring variables
	list<string> TokensList;

	// method for processing the source program
	void process(string program);
	void parser(list<string> tokens);

	// methods to evaluate/parse tokens for database
	void parseProcedure();	// Step 1: Check for procedure
	void parseStatement();	// Step 2: Check for statement
	//void parseVariable(); // Check for variable 
	//void parseConstant(); // Check for constant
	//void parseExpression(); // Check for expression

	// Supporting methods
	bool checkName(string token);
	bool checkNum(string token);
	void next_token();
};