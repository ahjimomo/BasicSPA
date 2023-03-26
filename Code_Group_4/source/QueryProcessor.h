#pragma once

#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include "Database.h"
#include "Tokenizer.h"

using namespace std;

class QueryProcessor {
public:
	// default constructor
	QueryProcessor();

	// destructor
	~QueryProcessor();

    // declaring variables
    vector<string> TokensList;

    // method for evaluating a query
	void evaluate(string query, vector<string>& results);
    void extract(vector<string> tokens);

    // method to evaluate query without pattern or such that clause
    void querySingle(string qType, string option);

    // Type support methods
    bool checkType(string token);
    void parseType();
    list<string> parseElemName();
    string getType(string elemName);

    // General supporting methods
    void next_token();
    bool checkName(string token);
    void expectedSymbol(string symbol);
    bool checkExpr(string token);
};