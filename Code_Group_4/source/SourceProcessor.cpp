#include "SourceProcessor.h"

#include <regex>			// C++ regex library for regex expressions
#include <list>				// Use of list vs. vector (https://www.educba.com/c-plus-plus-vector-vs-list/)
#include <iostream>
#include <unordered_map>	// Use of map to store constantname and values
#include <string>
#include <iterator>

#define NUM_EXPR "[0-9]+"
#define NAME_EXPR "[a-zA-Z]+[0-9]*"

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
int curLineIdx = 1;						// int to track line we are at
int constCounter = 0;					// int to use as alias name to avoid duplication
std::unordered_map<string, int> cval;	// map to store constant values [string-int pair]

///// Methods to process source /////
// iter 1: method to take in tokens for processing (first step to check on procedure)
void SourceProcessor::parser(list<string> Tokens)
{
	// Initialize TokensList as our temp tokens list (allTokens)
	TokensList = Tokens;

	// Commence processing tokens starting with procedure
	try
	{
		parseProcedure();
	}

	// Catch and print any errors our code runs into (https://stackoverflow.com/questions/3074646/how-to-catch-unknown-exception-and-print-it)
	catch (const std::exception& e)
	{
		std::throw_with_nested("Error in first step of parsing procedure");
	}
}

///// Methods to process source /////
// iter 1: method to check if first element is a procedure, takes it's name to insert into DB (ie. procedure p1)
void SourceProcessor::parseProcedure()
{
	// Check if first element is "procedure"
	if (TokensList.front() == "procedure")
	{
		// Next element should be the name for the procedure
		next_token();
		if (checkName(TokensList.front()) == true)
		{
			// insert procedure into database
			Database::insertProcedure(TokensList.front());

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

			// Process the print line index
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
void SourceProcessor::parseAssignment()
{
	// Assignment should commence with variable name
	if (checkName(TokensList.front()) == true)
	{
		while (TokensList.front() != ";")
		{
			// Get lhs & rhs
			string lhs = TokensList.front();
			string rhs;

			// Insert lhs as variable
			parseVariable();

			// [Future] Checked if target exists, return value if yes, else creates constant as 0 or None

			// Check if assignment sign is correct, expected "=" before moving to next element
			next_token();
			expectedSymbol("=");

			// [Future] To check if it's an expression instead of single value/variable
			if (TokensList.front() == "(")
			{
				std::cout << "Nested!";
			}
			// Single value / Variable
			else
			{
				string rhs = TokensList.front();

				// Parse in assignee to DB
				parseAssignee(lhs, rhs);
				// [Future]
				//parseModifies(); // update modifies(rhs, lhs)

				// Update assignment table in DB
				string aLineIdx = intToStr(curLineIdx);
				Database::insertAssignment(aLineIdx, lhs, rhs);
			}

		}
	}

	else
	{
		std::throw_with_nested(std::runtime_error("Error with assignment name, expected variable name [a-zA-Z]+[0-9]*, please check again."));
	}
}

// iter 1: method to process assigned values
void SourceProcessor::parseAssignee(string lhs, string rhs)
{
	while (TokensList.front() != ";")
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
			string val = parseConstant("name");

			// Insert constant-value pair to constants table if there is an existing value from the variable else skip
			if (val != "none")
			{
				Database::insertConstant(lhs, val);

				// Update cval on key-pair value(s)
				updateConstantMap(lhs, val);
			}


		}
		// [Future] check if assignment is a factor/term
		
		// [Not required / check with prof] check if assignment is a string

		// Move to next element
		next_token();
	}
}

///// Main Supporting Functions /////
// iter 1: method to process line index
void SourceProcessor::processIdx(string option)
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
		//Database::insertUses(saveLineIdx, tokensList.front());
	}
	else if (option == "read")
	{
		Database::insertRead(saveLineIdx);
		//Database::insertModifies(saveLineIdx, tokensList.front());
	}
	else if (option == "statement")
	{
		Database::insertStatement(saveLineIdx);
		//Database::insertModifies(saveLineIdx, tokensList.front());
		//Database::insertUses(saveLineIdx, tokensList.front());
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

// iter 2: Tabulate results

///// Side Supporting Functions /////
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

/*// iter 1: support method to throw exception for debugging (https://en.cppreference.com/w/cpp/error/throw_with_nested)
//void SourceProcessor::print_exception(const std::exception& e, int level = 0)
//{
//	std::cerr << std::string(level, ' ') << "exception: " << e.what() << "\n";
//	try 
//	{
//		std::rethrow_if_nested(e);
//	}
//	catch (const std::exception& nestedException) 
//	{
//		print_exception(nestedException, level + 1);
//	}
//	catch (...) {}
//}*/

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
