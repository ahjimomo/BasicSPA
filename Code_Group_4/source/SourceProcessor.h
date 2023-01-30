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
	void parseProcedure();		// 1: Check for procedure
	void parseStatement();		// 2: Check for statement
	void parseVariable();		// 2A: Process variable
	void parseAssignment();		// 3: Process Assignment
	void parseAssignee();		// 3A: Process Assignments
	//void parseExpression(); // Check for expression

	// Main Supporting methods
	void processIdx(string option);
	void parseConstant(string option);

	// Side Supporting methods
	bool checkName(string token);
	bool checkNum(string token);
	void next_token();
	void expectedSymbol(string symbol);
	//void print_exception(const std::exception&, int level);
	string intToStr(int value);
	int strToInt(string value);
};