#pragma once
#include <sstream>
#include <fstream>
#include <vector>
#include "Database.h"
#include "Tokenizer.h"

#include <list>
#include <vector>

using namespace std;

class SourceProcessor {
public:
	// Declaring variables
	list<string> TokensList;

	// method for processing the source program
	void process(string program);
	void parser(list<string> tokens);

	// methods to evaluate/parse tokens for database
	void parseProcedure(string option);						// Check for procedure	(iter 1)
	void parseStatement(string additional);					// Check for statement	(iter 1)
	void parseVariable(string value);						// Process variable		(iter 1)
	void parseAssignment();									// Process Assignment	(iter 1)
	void parseAssignee(string lhs, string rhs);				// Process Assignments	(iter 1)
	void parseWhile();										// Process While loop	(iter 2)
	void parseIf(string option);							// Process If loop		(iter 2)

	// Main Supporting methods
	void processIdx(string option, string lhs, string rhs);
	string parseConstant(string option);					// process constant values
	void parseContainer(string option);						// process Container Statements for if/while/else (...)

	// General Supporting methods
	bool checkName(string token);
	bool checkNum(string token);
	void next_token();
	void expectedSymbol(string symbol);
	string intToStr(int value);
	int strToInt(string value);
	bool checkOperator(string token);
	bool checkComparison(string token);
	string expandListString(list<string> toExpand);

	// Constant Supporting methods
	string getConstantValue(string constantName);
	void updateConstantMap(string constantName, string value);

	// Procedures Supporting methods

	// Uses Supporting methods
	void useProcessor(string LineIdx, string used);

	// Modifies Supporting methods
	void modifyProcessor(string LineIdx, string assigned);
};