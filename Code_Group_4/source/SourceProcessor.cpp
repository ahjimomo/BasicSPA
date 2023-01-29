#include "SourceProcessor.h"

#include <regex> // C++ regex library for regex expressions
#include <list>  // Use of list vs. vector (https://www.educba.com/c-plus-plus-vector-vs-list/)
#include <iostream>

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
	catch (const std::exception& exc)
	{
		std::cerr << "Caught exception \"" << exc.what() << "\"\n";
	}
}


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
				parseStatement();
			}
			else
			{
				//std::Error with parsing statement, missing "{"
			}
		}
	}
}

// iter 1: method to check and process statement
void SourceProcessor::parseStatement()
{
	// to be continue...
}


///// Supporting Functions /////
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

// iter 1: support method to check if token is numerical
bool SourceProcessor::checkNum(string token)
{
	// Return boolean if element is an integer
	return regex_match(token, regex(NUM_EXPR));
}

// iter 1: support method to check if token is a name (ie. a1, w2, etc..)
bool SourceProcessor::checkName(string token)
{
	// Return boolean if element is a 'name'
	return regex_match(token, regex(NAME_EXPR));
}

