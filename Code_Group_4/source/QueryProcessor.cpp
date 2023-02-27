#include "QueryProcessor.h"
#include "Tokenizer.h"
#include <string>
#include <iostream>
#include <list>

using namespace std;

// constructor
QueryProcessor::QueryProcessor() {}

// destructor
QueryProcessor::~QueryProcessor() {}


///// Methods to evaluate query /////
// iter1: method to evaluate a query
// This method currently only handles queries for getting all the procedure names,
// using some highly simplified logic.
// You should modify this method to complete the logic for handling all required queries.
void QueryProcessor::evaluate(string query, vector<string>& output) {
	// clear the output vector
	output.clear();

	// tokenize the query
	Tokenizer tk;
	vector<string> tokens;
	tk.tokenize(query, tokens);

	// check what type of synonym is being declared
	//string synonymType = tokens.at(0);

	// create a vector for storing the results from database
	vector<string> databaseResults;

	// call the method in database to retrieve the results
	// This logic is highly simplified based on iteration 1 requirements and 
	// the assumption that the queries are valid.
    int i = 0;
    while (i < tokens.size()) {
        string synonymType = tokens.at(i);

        if (synonymType == "procedure")
		    Database::getProcedures(databaseResults);

        if (synonymType == "variable" )
            Database::getVariables(databaseResults);

        if (synonymType == "constant")
            Database::getConstants(databaseResults);

        if (synonymType == "assign")
            Database::getAssignments(databaseResults);

        if (synonymType == "stmt")
            Database::getStatements(databaseResults);

        if (synonymType == "read")
            Database::getReads(databaseResults);

        if (synonymType == "print")
            Database::getPrints(databaseResults);

        if (synonymType == "while")
            Database::getParents(databaseResults);

        i++;
    }

	// post process the results to fill in the output vector
	for (string databaseResult : databaseResults) {
        if (databaseResults.empty()){
            output.push_back("None");
        } else {
            output.push_back(databaseResult);
        }
	}
}