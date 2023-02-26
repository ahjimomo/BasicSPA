#include "SourceProcessor.h"

#include <regex>			// C++ regex library for regex expressions
#include <list>				// Use of list vs. vector (https://www.educba.com/c-plus-plus-vector-vs-list/)
#include <iostream>
#include <unordered_map>	// Use of map to store constantname and values
#include <string>
#include <iterator>

#define NUM_EXPR "[0-9]+"
#define NAME_EXPR "[a-zA-Z]+[0-9]*"
#define OPERATOR_EXPR "[\+\-\/\%\*]"
#define COMPARE_EXPR "[\>\<]"

// method for processing the source program
// This method currently only inserts the procedure name into the database
// using some highly simplified logic.
// You should modify this method to complete the logic for handling all the required syntax.
void SourceProcessor::process(string program)
{
	// initialize the database
	Database::initialize();

	// tokenize the program
	Tokenizer tk;
	vector<string> tokens;
	tk.tokenize(program, tokens);

	// store individual elements in a list
	list<string> allTokens;

	int i = 0;
	while (i < tokens.size())
	{
		string elem = tokens.at(i);
		allTokens.push_back(elem);
		i++;
	}

	// push our tokens into a parser
	parser(allTokens);
}

///// Parameters /////
int curLineIdx = 1;								// int to track line we are at

int constCounter = 0;							// int to use as alias name to avoid duplication
std::unordered_map<string, int> cval;			// map to store constant values [string-int pair]

string curProd;									// string to track current procedure we are in
string prevProd;								// string to track previous procedure that may not be the root but calling another procedure
string rootProd;								// string to track root procedure that called our current procedure (if any)
int rootDepth = 1;								// int to track depth of procedures
std::list<string> prodList;						// list to store iterated procedures except for root

int curWhileCounter;							// int to track line index of current while statement

int curIfCounter;								// int to track line index of current if statement

std::list<string> patternFullList;				// List to store all values in condition expression (Use Substr function to extract patterns https://www.sqlitetutorial.net/sqlite-functions/sqlite-substr/#:~:text=The%20SQLite%20substr%20function%20returns,position%20with%20a%20predefined%20length.)


///// Methods to process source /////
// iter 1: method to take in tokens for processing (first step to check on procedure)
void SourceProcessor::parser(list<string> Tokens)
{
	// Initialize TokensList as our temp tokens list (allTokens)
	TokensList = Tokens;

	// Commence processing tokens starting with procedure as long as tokens are not exhausted
	while (TokensList.size() > 0)
	{
		try
		{
			parseProcedure("root");
		}

		// Catch and print any errors our code runs into (https://stackoverflow.com/questions/3074646/how-to-catch-unknown-exception-and-print-it)
		catch (const std::exception& e)
		{
			std::throw_with_nested("Error in first step of parsing procedure");
		}
	}
}

///// Methods to process source /////
// iter 1: method to check if first element is a procedure, takes it's name to insert into DB (ie. procedure p1)
// iter 2: update method to accept called procedures vs. preceding procedures
void SourceProcessor::parseProcedure(string option)
{
	// Check if we are dealing with new procedure (root) or called procedure
	if (option == "root")
	{
		// Reset root depth & procedure list
		rootDepth = 1;
		prodList.clear();

		// Word "procedure" is expected
		expectedSymbol("procedure");

		if (checkName(TokensList.front()) == true)
		{
			// insert procedure into database & rest all local procedure list & index
			Database::insertProcedure(TokensList.front());
			prodList.push_back(curProd);
			curProd = TokensList.front();
			rootProd = TokensList.front();
			prevProd = TokensList.front();


			// Next element should be a "{" symbol for the statement
			next_token();
			if (TokensList.front() == "{")
			{
				next_token();
				parseStatement();
			}
			else
			{
				std::throw_with_nested(std::runtime_error("Error with parsing statement, missing '{' symbol"));
			}
		}
	}

	// If we are dealing with a called procedure
	else if (option == "called")
	{
		// First word should be "call"
		expectedSymbol("call");

		// Update local mappings (Can be overwritten if recalled)
		curProd = TokensList.front();

		// Check if there are other procedures called before else there should only be a single root procedure nested in list
		if (rootDepth > 1)
		{
			// Update calls* table
			for (string callerProd : prodList)
			{
				Database::insertCallStar(intToStr(curLineIdx), callerProd, curProd);
			}
		}

		// Update calls table
		Database::insertCall(intToStr(curLineIdx), prevProd, curProd);

		// +1 to rootDepth, update previousProd to current procedure and append current procedure to prodList list
		rootDepth++;
		prevProd = TokensList.front();
		prodList.push_back(curProd);
	}
}

///// methods to evaluate/parse tokens for database /////
// iter 1: method to process statement
void SourceProcessor::parseStatement()
{
	// Statement will end with "}", iterate till end of statement
	while (TokensList.front() != "}")
	{
		// 1. Check if print function is called
		if (TokensList.front() == "print")
		{
			// Next element should be the variable called for printing
			next_token();
			parseVariable();

			// Process the print line index & uses in DB
			processIdx("print");

			// Next element should be a ";" assignment
			next_token();
			expectedSymbol(";");
		}

		// 2. Check if read function is called
		else if (TokensList.front() == "read") 
		{
			// capture the current token
			// [Future] string prev_token = TokensList.front();

			// Next element should be the variable called for reading
			next_token();
			parseVariable();

			// Process read line index
			processIdx("read");

			// Next element shoud be a ";" for assignment
			next_token();
			expectedSymbol(";");
		}

		// 3. Iter 2: Check if while function is called
		else if (TokensList.front() == "while")
		{
			// Update the index of while function called & update DB
			parseWhile();

			// Next element should be a 'then' or '{' to finish move into statement
			next_token();

			if (TokensList.front() == "then")
			{
				expectedSymbol("then");
				curLineIdx++;
			}
			else
			{
				expectedSymbol("{");
				curLineIdx++;
			}

			// Process the while-statement
			parseStatement();
		}

		// 4. Iter 3: Call for procedure
		else if (TokensList.front() == "call")
		{
			parseProcedure("called");
			expectedSymbol(";");
		}

		// 5. 
		// else. if it's none of the above methods, it should be a direct assignment 
		else if (checkName(TokensList.front()) == true)
		{
			parseAssignment();
			expectedSymbol(";");
		}

		// Update line capturing statement in DB
		processIdx("statement");
		// Update count of line & move to next element
		curLineIdx++;
	}

}

// iter 2: method to process while clause
void SourceProcessor::parseWhile()
{
	// Insert while statement into DB
	curWhileCounter = curLineIdx;
	Database::insertWhile(intToStr(curWhileCounter));

	parseContainer("while"); // Every line should also update the while table for Modifies & Uses if necessary
}

// iter 2: method to process Condition Expression
void SourceProcessor::parseContainer(string option)
{
	std::cout << "Hello world!";
}

// iter 1: method to process variable
void SourceProcessor::parseVariable()
{
	// Check if variable is a valid name
	if (checkName(TokensList.front()) == true) 
	{
		// Insert variable into database
		Database::insertVariable(TokensList.front());
	}
	else 
	{
		std::throw_with_nested(std::runtime_error("Invalid variable name for expected variable"));
	}
}

// iter 1: method to process assignments
// iter 2: update to take in term instead of single assignment and to update Pattern table
void SourceProcessor::parseAssignment()
{
	// Assignment should commence with variable name
	if (checkName(TokensList.front()) == true)
	{
		// Get lhs & rhs
		string lhs = TokensList.front();
		string rhs;

		// Insert lhs as variable
		parseVariable();

		// Check if assignment sign is correct, expected "=" before moving to next element
		next_token();
		expectedSymbol("=");

		while (TokensList.front() != ";")
		{
			// put value into list
			patternFullList.push_back(TokensList.front());

			// Insert value into constant table if it's not an operator
			if (checkOperator(TokensList.front()) == false)
			{
				parseAssignee(lhs, TokensList.front());
			}

			rhs = TokensList.front(); // Will be overwritten if assignee is a term and not a single value
			next_token();
		}
		// Get full string of term to overwrite incorrect rhs value
		if (patternFullList.size() > 1)
		{
			rhs = expandListString(patternFullList);
		}

		// Update assignment table in DB
		string aLineIdx = intToStr(curLineIdx);
		Database::insertAssignment(aLineIdx, lhs, rhs);
		Database::insertPattern(aLineIdx, lhs, rhs);
	}

	else
	{
		std::throw_with_nested(std::runtime_error("Error with assignment name, expected variable name [a-zA-Z]+[0-9]*, please check again."));
	}
}

// iter 1: method to process assigned values
void SourceProcessor::parseAssignee(string lhs, string rhs)
{
	// Check if assignment is an integer value
	if (checkNum(TokensList.front()) == true)
	{
		string val = parseConstant("value");

		// Insert constant-value pair to constants table
		Database::insertConstant(lhs, val);

		// Update cval on key-pair value
		updateConstantMap(lhs, val);
	}
	// if asignee is a variable, get variable existing value
	else if (checkName(TokensList.front()) == true)
	{
		// Insert into use table since value is a variable
		processIdx("assignment", lhs, rhs);

		string val = parseConstant("name");

		// Insert constant-value pair to constants table if there is an existing value from the variable else skip
		if (val != "none")
		{
			Database::insertConstant(lhs, val);

			// Update cval on key-pair value(s)
			updateConstantMap(lhs, val);
		}

	}
}

///// Main Supporting Functions /////
// iter 1: method to process line index
void SourceProcessor::processIdx(string option, string lhs = "None", string rhs = "None")
{
	// Convert current line index to string using stringstream object (https://www.simplilearn.com/tutorials/cpp-tutorial/int-to-string-cpp)
	string saveLineIdx;
	stringstream lineIdx;
	lineIdx << curLineIdx;
	lineIdx >> saveLineIdx;

	// process index
	if (option == "print")
	{
		Database::insertPrint(saveLineIdx);
		Database::insertUse(saveLineIdx, saveLineIdx, TokensList.front(), "print");
		Database::insertUse(saveLineIdx, curProd, TokensList.front(), "procedure");
		useProcessor(saveLineIdx);
	}
	else if (option == "read")
	{
		Database::insertRead(saveLineIdx);
		Database::insertModifies(saveLineIdx, saveLineIdx, TokensList.front(), "read");
		Database::insertModifies(saveLineIdx, curProd, TokensList.front(), "procedure");
		modifyProcessor(saveLineIdx, lhs);

	}
	else if (option == "statement")
	{
		Database::insertStatement(saveLineIdx);
	}
	else if (option == "assignment")
	{
		Database::insertUse(intToStr(curLineIdx), intToStr(curLineIdx), rhs, "assignment");
		Database::insertModifies(intToStr(curLineIdx), intToStr(curLineIdx), lhs, "assignment");
	}
	else if (option == "containerStatement")
	{
		Database::insertModifies(saveLineIdx, saveLineIdx, TokensList.front(), "cStatement");
		Database::insertModifies(saveLineIdx, curProd, TokensList.front(), "procedure");
		useProcessor(saveLineIdx);
		Database::insertUse(saveLineIdx, saveLineIdx, TokensList.front(), "cStatement");
		Database::insertUse(saveLineIdx, curProd, TokensList.front(), "procedure");
		modifyProcessor(saveLineIdx, lhs);
	}
	else if (option == "while")
	{
		Database::insertWhile(saveLineIdx);
	}
	else if (option == "pattern")
	{

	}
}

// iter 1: method to process constant
string SourceProcessor::parseConstant(string option)
{
	// if constant provided is an integer, use line as temp alias to create value ([curLineIdx] + [constCounter])
	if (option == "value")
	{
		return TokensList.front();
	}
	// if constant provided is a variable, check if there is an existing value to use else just insert
	else if (option == "name")
	{
		// Return variable value as string, return 'none' if variable does not exist and create key in map
		string val = getConstantValue(TokensList.front());

		return val;
	}
}


///// General Supporting Functions /////
// iter 2: support method to perform additional action for modifies table
void SourceProcessor::modifyProcessor(string LineIdx, string assigned)
{
	// Check: If current procedure is not the root procedure
	if (curProd != rootProd)
	{
		// Create temp list & remove the last element since it has been inserted into uses table earlier tagged as "procedure"
		list<string> templist = prodList;
		templist.pop_back();

		for (string precedingProd : templist)
		{
			// Insert modifies of current variable for all preceding procedures
			Database::insertModifies(LineIdx, precedingProd, assigned, "p2p");
		}
	}
}

// iter 2: support method to perform additional action for uses table
void SourceProcessor::useProcessor(string LineIdx)
{
	// Check: If current procedure is not the root procedure
	if (curProd != rootProd)
	{
		// Create temp list & remove the last element since it has been inserted into uses table earlier tagged as "procedure"
		list<string> templist = prodList;
		templist.pop_back();

		for (string precedingProd : templist)
		{
			// Insert uses of current variable for all preceding procedures
			Database::insertUse(LineIdx, precedingProd, TokensList.front(), "p2p");
		}
	}
}

// iter 2: support method to check if token_string is a pattern (ie. +, -, /, %, *)
bool SourceProcessor::checkOperator(string token)
{
	// Return boolean if element is an pattern
	return regex_match(token, regex(OPERATOR_EXPR));
}

// iter 2: support method to check if token_string is a comparison operator (ie. <, >)
bool SourceProcessor::checkComparison(string token)
{
	// Return boolean if element is an pattern
	return regex_match(token, regex(COMPARE_EXPR));
}

// iter 2: support method to expand list into string
string SourceProcessor::expandListString(list<string> toExpand)
{
	// Expand list into string and return values
	string delim = " ";
	string expanded;

	// Iterate through list and concatenate words
	for (string word : toExpand)
	{
		expanded = expanded + word;
	}
	return expanded;
}

// iter 1: support method to check if symbol provided matches before moving to next element
void SourceProcessor::expectedSymbol(string symbol)
{
	// Check if expected symbol is at EOL (ie. '}', ';', ')')
	if (TokensList.front() == symbol)
	{
		next_token();
	}
	else
	{
		std::throw_with_nested(std::runtime_error("Error, expected symbol " + symbol));
	}
}

// iter 1: support method to move to the next element and pop the current element
void SourceProcessor::next_token()
{
	// Check if all elements has been visited in list(tokens)
	if (TokensList.empty() == false)
	{
		// Remove the current/front element
		TokensList.pop_front();
	}
}

// iter 1: support method to check if token_string is numerical
bool SourceProcessor::checkNum(string token)
{
	// Return boolean if element is an integer
	return regex_match(token, regex(NUM_EXPR));
}

// iter 1: support method to check if token_string is a name (ie. a1, w2, etc..)
bool SourceProcessor::checkName(string token)
{
	// Return boolean if element is a 'name'
	return regex_match(token, regex(NAME_EXPR));
}

// iter 1: support method to throw exception for debugging (https://en.cppreference.com/w/cpp/error/throw_with_nested)

// iter 1: process integer to string
string SourceProcessor::intToStr(int value) 
{
	string valueInStr;
	stringstream OriginalInt;
	OriginalInt << value;
	OriginalInt >> valueInStr;

	return valueInStr;
}

// iter 1: process string to integer
int SourceProcessor::strToInt(string value)
{
	int valueInInt = stoi(value);
	return valueInInt;
}

// iter 1: support method to check if constant is in map else create key with value = 'None'
string SourceProcessor::getConstantValue(string constantName)
{
	// Check if constant exists
	if (cval.count(constantName))
	{
		int int_result = cval[constantName];
		return intToStr(int_result);
	}
	else
	{
		//int int_result = cval[constantName]; // (Non-existent key should perform an insert with value of 0: https://en.cppreference.com/w/cpp/container/map)
		return "none";
	}
}

// iter 1: support method to update constant in local map for future usage [Modifies Value]
void SourceProcessor::updateConstantMap(string constantName, string value)
{
	// Update map value or create if it does not exist
	cval[constantName] = strToInt(value);
}