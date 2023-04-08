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

    // Such that support methods
    vector<string> parseSuchThat();
    vector<string> parseTable(string table);

    // Type support methods
    //void getLineIdx(string option, string queryLine, string valueName);
    vector<string> getQueriedIdx(string type, vector<string> resultSpace, string stmtLine);
    bool checkType(string token);
    void parseType();
    list<string> parseElemName();
    string getType(string elemName, string table);
    void parseMQuery();

    // General supporting methods
    void next_token();
    bool checkName(string token);
    void expectedSymbol(string symbol);
    bool checkSuch(string token);
    bool checkPatt(string token);
    bool checkWild(string token);
    bool checkNum(string token);
};