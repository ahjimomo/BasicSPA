#include "QueryProcessor.h"
#include "Tokenizer.h"
#include "Database.h"

#include <regex>			// C++ regex library for regex expressions
#include <string>
#include <iostream>
#include <list>
#include <vector>	        // Use of vector to temporarily store synonymType and name
#include <unordered_map>	// Use of map to store constantname and values

#define NUM_EXPR "[0-9]+"
#define NAME_EXPR "[a-zA-Z]+[0-9]*"
#define OPERATOR_EXPR "[\+\-\/\%\*]"
#define COMPARE_EXPR "[\>\<\==\!=]"
#define TYPE_EXPR "(stmt)|(read)|(print)|(assign)|(while)|(if)|(call)|(variable)|(procedure)|(constant)"
#define CLAUSE_EXPR "(pattern)|(such)|(that)"

using namespace std;

// constructor
QueryProcessor::QueryProcessor() {}

// destructor
QueryProcessor::~QueryProcessor() {}

///// Parameters /////
vector<string> databaseResults;                 // initialize a vector for storing the results from database
vector<string> output;                          // vector to store final output
vector<string> tempResults;                     // vector storing temp results from database

unordered_map<string, string> elemMap;          // Map storing select and query elem name

list<string> qFullComponents;                   // List to track the full query
list<string> qComponent;                        // List to track the contained query
list<string> sComponent;                        // List to track select component

///// Methods to evaluate query /////
// iter1: method to evaluate a query
// This method currently only handles queries for getting all the procedure names,
// using some highly simplified logic.
// You should modify this method to complete the logic for handling all required queries.
void QueryProcessor::evaluate(string query, vector<string>& output) 
{
    // clear the output vector
    output.clear();

    // tokenize the query
    Tokenizer tk;
    vector<string> tokens;
    tk.tokenize(query, tokens);

    // Sample printing
    cout << "Query: Did tokens get tokenized?: " << tokens.size() << endl;

    // store individual elements in a list
    vector<string> allTokens;

    unsigned int i = 0;
    while (i < tokens.size()) {
        string element = tokens.at(i);
        allTokens.push_back(element);
        i++;

    }

    // push elements into extract function
    extract(allTokens);

    // Reset class-level variable
    qFullComponents.clear();
    qComponent.clear();
    sComponent.clear();

    // store results in output
    for (string dbResult : databaseResults)
    {
        output.push_back(dbResult);
    }

    /*//print results
    for (unsigned int j = 0; j < output.size(); j++)
    {
        cout << "output" << output[j] << endl;
    }
    */

}

///// Extractor /////
// call the method in database to retrieve the results
void QueryProcessor::extract(vector<string> tokens) {

    TokensList = tokens;                // Initialize TokensList as our temp tokens list (allTokens)

    vector<string> prod;                // Vector to track procedure results
    vector<string> var;                 // Vector to track variable results
    vector<string> cond;                // Vector to track string results

    string currQ;                       // List to track action
    string prev;                        // List to track prev string we were in

    // Commence processing tokens starting with procedure as long as tokens are not exhausted
    while (TokensList.size() > 0) {

        // 1. Start with mapping of queries elements in local mapping
        if (checkType(TokensList.front()) == true)
        {
            parseType();
        }
        
        // 2. Extract final output required
        if (TokensList.front() == "Select" | TokensList.front() == "select")
        {
            // Update local tracking lists
            qFullComponents.push_back(TokensList.front());
            sComponent.push_back(TokensList.front());
            next_token();
            
            // Case: Single query with no pattern or clause (ie. Left [a] in TokensList)
            if (TokensList.size() == 1)
            {
                // Get data type of queried element
                string qType = getType(TokensList.front());

                querySingle(qType, "single");
            }
            // Case: Multiple select queries (ie. <w1, w2, w3>)
            if (TokensList.front() == "<")
            {
                // Update local tracking lists
                qFullComponents.push_back(TokensList.front());
                sComponent.push_back(TokensList.front());

                // Parse into 
                //parseNestedSelect();

            }
            // Case: If not of the above, it should be a single variable with additional clause (ie. w1 such that ()..)
            else
            {
                // Get data type of queried element
                string qType = getType(TokensList.front());

                // Process clause
                //next_token();
                //if (TokensList.front() == )


            }
            
        }
        // Move to next token
        next_token();
    }
}


///// Support Methods /////
// iter 2: Process single variable
void QueryProcessor::querySingle(string qType, string option)
{
    // 1. constant 
    if (qType == "constant") 
    {
        Database::getConstants(tempResults);
    }
    // 2. assignment
    else if (qType == "assign")
    {
        Database::getAssignments(tempResults);
    }
    // 3. statements
    else if (qType == "stmt")
    {
        Database::getStatements(tempResults);

    }
    // 4. read
    else if (qType == "read")
    {
        Database::getReads(tempResults);
    }
    // 5. print
    else if (qType == "print")
    {
        Database::getPrints(tempResults);
    }
    // 6. while
    else if (qType == "while")
    {
        Database::getWhiles(tempResults);
    }
    // 7. if
    else if (qType == "if")
    {
        Database::getIfs(tempResults, "if", "parentLine", 0);
    }
    // 8. Procedure
    else if (qType == "procedure")
    {
        Database::getProcedures(tempResults);
    }
    // 9. variable
    else if (qType == "variable")
    {
        Database::getVariables(tempResults);
    }

    // If single query
    if (option == "single")
    {
        databaseResults = tempResults;
    }
}

// iter 2: Get query type from local map
string QueryProcessor::getType(string elemName)
{
    auto items = elemMap.find(elemName);
    string typeFound = items->second;
    return typeFound;
}

// iter 2: support method to check if token is expression (ie. pattern / such that)
bool QueryProcessor::checkExpr(string token)
{
    // Check if token is "such"
    if (token == "such")
    {
        next_token();
    }

    // Return boolean if element is an clause
    return regex_match(token, regex(CLAUSE_EXPR));
}

// iter 2: support method to check token's type
bool QueryProcessor::checkType(string token)
{
    // Return boolean if element is an integer
    return regex_match(token, regex(TYPE_EXPR));
}

// iter 2: Support method to parse element(s) queried belonging to any of the type of Data type
void QueryProcessor::parseType()
{
    string elem = TokensList.front();
    list<string> elemList;

    while (TokensList.front() != ";")
    {
        // 1. Variable
        if (TokensList.front() == "variable")
        {
            // Get the variableName and map it
            next_token();
            elemList = parseElemName();

            for (string item : elemList)
            {
                elemMap[item] = elem;
            }
        }
        // 2. Procedure
        if (TokensList.front() == "procedure")
        {
            // Get the procedureName and map it
            next_token();
            elemList = parseElemName();

            for (string item : elemList)
            {
                elemMap[item] = elem;
            }
        }
        // 3. Constant
        if (TokensList.front() == "constant")
        {
            next_token();
            elemList = parseElemName();

            for (string item : elemList)
            {
                elemMap[item] = elem;
            }
        }
        // 4. Statement
        if (TokensList.front() == "stmt")
        {
            // Get the Name and map it
            next_token();
            elemList = parseElemName();

            for (string item : elemList)
            {
                elemMap[item] = elem;
            }
        }
        // 5. read
        if (TokensList.front() == "read")
        {
            // Get the Name and map it
            next_token();
            elemList = parseElemName();

            for (string item : elemList)
            {
                elemMap[item] = elem;
            }
        }
        // 6. print
        if (TokensList.front() == "print")
        {
            // Get the Name and map it
            next_token();
            elemList = parseElemName();

            for (string item : elemList)
            {
                elemMap[item] = elem;
            }
        }
        // 7. assignment
        if (TokensList.front() == "assign")
        {
            // Get the Name and map it
            next_token();
            elemList = parseElemName();

            for (string item : elemList)
            {
                elemMap[item] = elem;
            }
        }
        // 8. while
        if (TokensList.front() == "while")
        {
            // Get the Name and map it
            next_token();
            elemList = parseElemName();

            for (string item : elemList)
            {
                elemMap[item] = elem;
            }
        }
        // 9. if
        if (TokensList.front() == "if")
        {
            // Get the Name and map it
            next_token();
            elemList = parseElemName();

            for (string item : elemList)
            {
                elemMap[item] = elem;
            }
        }
        // 10. call
        if (TokensList.front() == "call")
        {
            // Get the Name and map it
            next_token();
            elemList = parseElemName();

            for (string item : elemList)
            {
                elemMap[item] = elem;
            }
        }
    }
}

// iter 2: Support method to append elemName
list<string> QueryProcessor::parseElemName()
{
    list<string> elemList;
    while (TokensList.front() != ";")
    {
        elemList.push_back(TokensList.front());
        next_token();
    }
    return elemList;
}


// iter 2: support method to move to the next element and pop the current element
void QueryProcessor::next_token(){

    // Check if all elements has been visited in list(tokens)
    if (TokensList.empty() == false){
        // Remove the current/front element
        //TokensList.pop_front();
        TokensList.erase(TokensList.begin());
    }
}

// iter 2: support method to check if token_string is a name (ie. a1, w2, etc..)
bool QueryProcessor::checkName(string token){
    // Return boolean if element is a 'name'
    return regex_match(token, regex(NAME_EXPR));
}

// iter 2: support method to check if symbol provided matches before moving to next element
void QueryProcessor::expectedSymbol(string symbol)
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
