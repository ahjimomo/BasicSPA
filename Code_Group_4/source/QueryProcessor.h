#pragma once

#include <string>
#include <vector>
#include "Database.h"

using namespace std;

class QueryProcessor {
public:
	// default constructor
	QueryProcessor();

	// destructor
	~QueryProcessor();

	///// Main Methods /////
	void evaluate(string query, vector<string>& results);

	///// Support Methods /////
	//void postProcessQuery(vector<vector<string>>& dbResults, vector<string>& results);
};