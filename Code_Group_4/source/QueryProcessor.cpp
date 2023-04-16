#include "QueryProcessor.h"
#include "Tokenizer.h"
#include "Database.h"

#include <regex>			// C++ regex library for regex expressions
#include <string>
#include <iostream>
#include <list>
#include <vector>	        // Use of vector to temporarily store synonymType and name
#include <unordered_map>	// Use of map to store constantname and values
#include <algorithm>
#include <iterator>

#define NUM_EXPR "[0-9]+"
#define NAME_EXPR "[a-zA-Z]+[0-9]*"
#define OPERATOR_EXPR "[\+\-\/\%\*]"
#define COMPARE_EXPR "[\>\<\==\!=]"
#define TYPE_EXPR "(stmt)|(read)|(print)|(assign)|(while)|(if)|(call)|(variable)|(procedure)|(constant)"
#define SUCH_EXPR "(such)|(that)|(Such)|(That)"
#define PATTERN_EXPR "(pattern)|(Pattern)"
#define WILD_EXPR "[_]"

using namespace std;

// constructor
QueryProcessor::QueryProcessor() {}

// destructor
QueryProcessor::~QueryProcessor() {}

///// Parameters /////
vector<string> databaseResults;                 // initialize a vector for storing the results from database
vector<string> output;                          // vector to store final output
vector<string> tempResults;                     // vector storing temp results from database
vector<string> answerLines;                     // vector to store all indexes from clauses
vector<string> tempResultLines;                 // vector to track existing indexes from past clauses

unordered_map<string, string> elemMap;          // Map storing select and query elem name

list<string> qFullComponent;                    // List to track the full query
list<string> qComponent;                        // List to track the contained query (Store the query data type)
list<string> sComponent;                        // List to track select component (Store the query data type)

int wild_flag = 0;                              // int to track if there is a wild card tracking
int num_flag = 0;                               // int to track if there is a line index value in clause

unsigned int nested_depth = 0;                  // int to track the level of clause we are in
vector<string> curClauseResults;                // Vector to track existing clause results

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
    //cout << "Query: Did tokens get tokenized?: " << tokens.size() << endl;

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
    qFullComponent.clear();
    qComponent.clear();
    sComponent.clear();
    answerLines.clear();
    tempResultLines.clear();

    // store results in output
    for (string dbResult : databaseResults)
    {
        output.push_back(dbResult);
    }
}

///// Extractor /////
// call the method in database to retrieve the results
void QueryProcessor::extract(vector<string> tokens) 
{
    TokensList = tokens;                // Initialize TokensList as our temp tokens list (allTokens)

    vector<string> prod;                // Vector to track procedure results
    vector<string> var;                 // Vector to track variable results
    vector<string> cond;                // Vector to track string results
    vector<string> curReturnLines;

    string currQ;                       // List to track action
    string prev;                        // List to track prev string we were in
    string qType;                       // Query type of value

    // Commence processing tokens starting with procedure as long as tokens are not exhausted
    while (TokensList.size() > 0)
    {
        cout << "Current element : " << TokensList.front() << endl;

        // 1. Start with mapping of queries elements in local mapping
        if (checkType(TokensList.front()) == true)
        {
            //var.push_back(TokensList.front());
            parseType();
        }
        // 2. Extract final output required
        else if (TokensList.front() == "Select" | TokensList.front() == "select")
        {
            // Update local tracking lists
            qFullComponent.push_back(TokensList.front());
            next_token();

            // 2A. Single query with no pattern or clause (ie. Left [a] in TokensList)
            if (TokensList.size() == 1)
            {
                // Update local tracking for selected values
                sComponent.push_back(TokensList.front());

                // Get data type of queried element
                qType = getType(TokensList.front(), "none");

                querySingle(qType, "single");
            }
            // 2B. Multi select queries (ie. <w1, w2, w3>)
            else if (TokensList.front() == "<")
            {
                // Update local tracking list
                next_token();
                qFullComponent.push_back(TokensList.front());

                // Parse to MQuery to capture all elements into sComponent
                parseMQuery();

                // It should end with the token ">"
                //expectedSymbol(">");
            }
            // 2C. Single select for clauses
            else
            {
                sComponent.push_back(TokensList.front());
            }
        }
        // After the main selects are completed, it should either be such_that or pattern additional clause (ie. w1 such that ()..)
        // 3. Pattern clause
        else if (checkPatt(TokensList.front()) == true){
                parsePattern();
//            p1. Update depth level
//            nested_depth++;
//
//            qFullComponent.push_back(TokensList.front());
//            qComponent.push_back(TokensList.front());
//            string templhs;
//            string tablelhs;
//            string temprhs;
//            /*  cout << "!!!Tokens: " << TokensList.front() << endl;*/
//
//            while (TokensList.front() != "(") {
//                next_token();
//            }
//            next_token();
//
//            /*cout << "!!!Tokens1: " << TokensList.front() << endl;*/
//            //check lhs
//            while (TokensList.front() != ",") {
//                /*cout << "!!!Tokens2: " << TokensList.front() << endl;*/
//             /*   if (checkName(TokensList.front()) == true) {
//                    tablelhs = getType(TokensList.front(), "none");
//                    cout << "temp : " << tablelhs << endl;
//                    cout << "lhs1: " << templhs << endl;
//                    next_token();
//                }*/
//
//                if (TokensList.front() == "_") {
//                    templhs.push_back('%');
//                    next_token();
//                    cout << "lhs2: " << templhs << endl;
//                }
//
//                else if (TokensList.front() == "\"") {
//                    cout << "lhs3: " << templhs << endl;
//                    next_token();
//                    cout << "!!!Tokens4: " << TokensList.front() << endl;
//                }
//
//                else {
//                    if (templhs.empty() || templhs == "%") {
//                        templhs += TokensList.front();
//                        cout << "lhs4: " << templhs << endl;
//                    }
//                    else {
//                        templhs = templhs + " " + TokensList.front();
//                        cout << "lhs5: " << templhs << endl;
//                    }
//                    next_token();
//                }
//            }
//            next_token();
//            cout << "!!!Tokens6: " << TokensList.front() << endl;
//
//            if (templhs == "%")
//                templhs = "";
//
//            if (templhs == "v") {
//               tablelhs = getType(templhs, "none");
//               templhs = "";
//               cout << "temp2 : " << tablelhs << endl;
//            }
//            cout << "lhs6: " << templhs << endl;
//
//            //check rhs
//            while (TokensList.front() != ")") {
//
//                if (TokensList.front() == "_") {
//                    temprhs.push_back('%');
//                    next_token();
//                }
//
//                else if (TokensList.front() == "\"") {
//                    next_token();
//                }
//
//                else
//                {
//                    if (temprhs.empty() || temprhs == "%") {
//                        temprhs += TokensList.front();
//                    }
//                    else {
//                        temprhs = temprhs + " " + TokensList.front();
//                    }
//                    next_token();
//                }
//
//            }
//
//            if (temprhs == "%")
//                temprhs = "";
//
//
//            cout << "pattern lhs: " << templhs << " rhs:" << temprhs << endl;
//            Database::getPatterns(databaseResults, templhs, temprhs, tablelhs);
        }
        // 4. such that clause
        else if (checkSuch(TokensList.front()) == true)
        {
            // s1. Update depth level
            nested_depth++;

            // s2A. Check if there is a prior clause
            if (nested_depth > 1)
            {
                // Extract indexes of current such that clause
                next_token();
                curReturnLines = parseSuchThat();

                // Process difference with current prior clause results and update answerLines vector
                sort(tempResultLines.begin(), tempResultLines.end());
                sort(curReturnLines.begin(), curReturnLines.end());
                set_intersection(tempResultLines.begin(), tempResultLines.end(), curReturnLines.begin(), curReturnLines.end(), back_inserter(answerLines));
            }
            // s2B. There are no prior clause
            else
            {
                // Extract indexes of current such that clause
                next_token();
                cout << "Commencing non-nested such that extraction, next element: " << TokensList.front() << endl;
                curReturnLines = parseSuchThat();

                for (string item : curReturnLines)
                {
                    // Update the tempResultLines for case there other clauses following
                    tempResultLines.push_back(item);
                    // Update the answerLines for case where there are no other clauses following
                    answerLines.push_back(item);
                }
            }
        }

        // Move to next token
        next_token();
    }

    // 3B. Throw error if no answer is populated
    if (answerLines.empty())
    {
        cout << "such that clause is returning nothing." << endl;
        //throw std::runtime_error("Issue with such that clause");
        answerLines.push_back("");
    }

    // End of tokens iteration: process answer Lines of all tokens
    if (answerLines.size() > 0)
    {
        // Process databaseResults
        databaseResults = answerLines;
    }
}


///// Support Methods /////
void QueryProcessor::parsePattern() {
    // p1. Update depth level
    nested_depth++;

    qFullComponent.push_back(TokensList.front());
    qComponent.push_back(TokensList.front());
    string templhs;
    string tablelhs;
    string temprhs;
    /*  cout << "!!!Tokens: " << TokensList.front() << endl;*/

    while (TokensList.front() != "(") {
        next_token();
    }
    next_token();
    /*cout << "!!!Tokens1: " << TokensList.front() << endl;*/
    //check lhs
    while (TokensList.front() != ",") {
        /*cout << "!!!Tokens2: " << TokensList.front() << endl;*/
        /*   if (checkName(TokensList.front()) == true) {
           tablelhs = getType(TokensList.front(), "none");
           cout << "temp : " << tablelhs << endl;
           cout << "lhs1: " << templhs << endl;
           next_token();
       }*/
        if (TokensList.front() == "_") {
            templhs.push_back('%');
            next_token();
            cout << "lhs2: " << templhs << endl;
        } else if (TokensList.front() == "\"") {
            cout << "lhs3: " << templhs << endl;
            next_token();
            cout << "!!!Tokens4: " << TokensList.front() << endl;
        } else {
            if (templhs.empty() || templhs == "%") {
                templhs += TokensList.front();
                cout << "lhs4: " << templhs << endl;
            } else {
                templhs = templhs + " " + TokensList.front();
                cout << "lhs5: " << templhs << endl;
            }
            next_token();
        }
    }
    next_token();
    cout << "!!!Tokens6: " << TokensList.front() << endl;

    if (templhs == "%")
        templhs = "";

    if (templhs == "v") {
        tablelhs = getType(templhs, "none");
        templhs = "";
        cout << "temp2 : " << tablelhs << endl;
    }
    cout << "lhs6: " << templhs << endl;

    //check rhs
    while (TokensList.front() != ")") {

        if (TokensList.front() == "_") {
            temprhs.push_back('%');
            next_token();
        } else if (TokensList.front() == "\"") {
            next_token();
        } else {
            if (temprhs.empty() || temprhs == "%") {
                temprhs += TokensList.front();
            } else {
                temprhs = temprhs + " " + TokensList.front();
            }
            next_token();
        }

    }

    if (temprhs == "%")
        temprhs = "";

    cout << "pattern lhs: " << templhs << " rhs:" << temprhs << endl;
    Database::getPatterns(databaseResults, templhs, temprhs, tablelhs);
}



// iter 2: Support method to parse element(s) queried belonging to any of the type of Data type
vector<string> QueryProcessor::parseSuchThat()
{
    vector<string> parseResults;

    // Case: Parent / Parent*
    if (TokensList.front() == "Parent" | TokensList.front() == "parent")
    {
        parseResults = parseTable("parents");
        return parseResults;
    }
    else if (TokensList.front() == "Parent*" | TokensList.front() == "parent*")
    {
        parseResults = parseTable("parentstars");
        return parseResults;
    }
    // Case: Next / Next*
    else if (TokensList.front() == "Next" | TokensList.front() == "next")
    {
        //parseNext();
    }
    else if (TokensList.front() == "Next*" | TokensList.front() == "next*")
    {
        //parseNextStar();
    }
    // Case: Calls
    else if (TokensList.front() == "Calls" | TokensList.front() == "calls")
    {
        //parseCalls();
    }
    // Case: Uses
    else if (TokensList.front() == "Uses" | TokensList.front() == "uses")
    {
        //parseUses();
    }
    else if (TokensList.front() == "Modifies" | TokensList.front() == "modifies")
    {
        return parseTable("modifies");
    }
    else
    {
        throw std::runtime_error("Unrecognised keyword: " + TokensList.front());
    }

    // If we reach the end without valid cases
    std::cout << "No parse values captured" << endl;
    throw std::runtime_error("Invalid such that clause");
}

// iter 2: Support method to parse 
vector<string> QueryProcessor::parseTable(string table)
{
    // p1. check if first token is "(" 
    next_token();
    expectedSymbol("(");

    // p1b. Variables to store values
    vector<string> rootIdx;
    vector<string> childIdx;
    vector<string> queryIdx;
    vector<string> resultIdx;
    string pType;
    string cType;

    // p1a. Check if value is a LineIdx instead of wild card of variable
    if (checkNum(TokensList.front()) == true)
    {
        num_flag = 1;
        pType = table;
        cout << "Parent's a number!" << TokensList.front() << endl;
    }
    // p1b. Check for wild card or other type of variables
    else
    {
        // p1a. Check for wild card or type of variable
        pType = getType(TokensList.front(), table);
        cout << "Parent type extracted is: " << pType << endl;
        cout << "Table check is: " << table << endl;
    }

    // p3. Get all the ParentLine indexes
    if (table == "parents" | table == "parentstars")
    {
        Database::getPLine(table, pType, childIdx, num_flag, TokensList.front());
    }
    else if (table == "modifies")
    {
        Database::getMLine(table, pType, childIdx, num_flag, TokensList.front());
    }

    // Reset number flag
    num_flag = 0;
    
    // Test printing
    cout << "Returned index length : " << childIdx.size() << endl;
    for (string child : childIdx)
    {
        cout << child << endl;
    }

    // p4. Get the child query type of the nested value (ie. Parent(w, a)..)
    next_token();
    expectedSymbol(",");

    // p4a. Check if 2nd part is line index value
    if (checkNum(TokensList.front()) == true)
    {
        num_flag = 1;
        cType = table;
        cout << "Child is a number!" << TokensList.front() << endl;

        // p5. Get the line indexes of the specified queried values
        if (table == "parents" | table == "parentstars")
        {
            Database::getCLine(table, cType, queryIdx, num_flag, TokensList.front(), TokensList.front());
        }
        else if (table == "modifies")
        {
            Database::getMLine(table, cType, queryIdx, num_flag, TokensList.front());
        }

        // p6. Store the child index(es) found as the current temp results
        // sorting the vectors
        sort(childIdx.begin(), childIdx.end());
        sort(queryIdx.begin(), queryIdx.end());

        cout << "Returned query index length : " << queryIdx.size() << endl;

        for (string item : queryIdx)
        {
            cout << "Item in returned query: " << item << endl;
        }

        // Get the difference as existing result
        set_intersection(childIdx.begin(), childIdx.end(), queryIdx.begin(), queryIdx.end(), back_inserter(resultIdx));

        cout << "Size of result: " << resultIdx.size() << " - what's inside the result" << endl;
        for (string item : resultIdx)
        {
            cout << "In result: " << item << endl;
        }

        return resultIdx;
    }
    // p4b. If 2nd part is also a wildcard, then the Parent index is to be returned since it will be the same
    else if (checkWild(TokensList.front()) == true)
    {
        return childIdx;
    }
    // p4c. Else, extract child index and return the differences
    else
    {
        cType = getType(TokensList.front(), table);

        cout << "Child type extracted is: " << cType << endl;

        // p5. Get the line indexes of the specified queried values
        if (table == "parents" | table == "parentstars")
        {
            Database::getCLine(table, cType, queryIdx, num_flag, TokensList.front(), "0");
        }
        else if (table == "modifies")
        {
            Database::getMLine(table, cType, queryIdx, num_flag, TokensList.front());
        }

        // p6. Store the child index(es) found as the current temp results
        // sorting the vectors
        sort(childIdx.begin(), childIdx.end());
        sort(queryIdx.begin(), queryIdx.end());

        cout << "Returned query index length : " << queryIdx.size() << endl;

        for (string item : queryIdx)
        {
            cout << "Item in returned query: " << item << endl;
        }

        // Get the difference as existing result
        set_intersection(childIdx.begin(), childIdx.end(), queryIdx.begin(), queryIdx.end(), back_inserter(resultIdx));

        cout << "Size of result: " << resultIdx.size() << " - what's inside the result" << endl;
        for (string item : resultIdx)
        {
            cout << "In result: " << item << endl;
        }

        // p7. Return result
        //expectedSymbol(")");
        return resultIdx;
    }

}

// iter 2: Support method to extract index of values queried (Similar to `querySingle` but returning Line Idx instead..)
vector<string> QueryProcessor::getQueriedIdx(string qType, vector<string> resultSpace, string Line)
{
    // 1. assignment
    if (qType == "assign")
    {
        Database::getAssignments(resultSpace);
    }
    // 3. statements
    else if (qType == "stmt")
    {
        Database::getStatements(resultSpace);

    }
    // 4. read
    else if (qType == "read")
    {
        Database::getReads(resultSpace);
    }
    // 5. print
    else if (qType == "print")
    {
        Database::getPrints(resultSpace);
    }
    // 6. while
    else if (qType == "while")
    {
        Database::getWhiles(resultSpace, "while", "parentLine", Line);
    }
    // 7. if
    else if (qType == "if")
    {
        Database::getIfs(resultSpace, "if", "parentLine", Line);
    }
    // 8. variable
    else if (qType == "variable")
    {
        Database::getVariablesIdx(resultSpace);
    }
    // 9. Parents 
    else if (qType == "parents")
    {
        // get child index
        Database::getParents(resultSpace, Line);
    }
    // 10. Parents*
    else if (qType == "parents*")
    {
        // get all child* index
        Database::getParentStars(resultSpace, Line);
    }
    else if (qType == "modify")
    {
        Database::getModifies(resultSpace);
    }

    return resultSpace;
}

// iter 2: Process multiple select queries
void QueryProcessor::parseMQuery()
{
    // Break case: ">"
    while (TokensList.front() != ">")
    {
        // Store queried element in sComponent list if it's not ","
        if (TokensList.front() != ",")
        {
            sComponent.push_back(TokensList.front());
        }
        next_token();
    }
}

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
        Database::getWhiles(tempResults, "while", "parentLine", "0");
    }
    // 7. if
    else if (qType == "if")
    {
        Database::getIfs(tempResults, "if", "parentLine", "0");
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
/// elemName: The element such as [ie. (a, w) / (_, w1), etc.]
string QueryProcessor::getType(string elemName, string table)
{

    // A. Process wildcard
    if (checkWild(elemName) == true)
    {
        auto items = elemMap.find(sComponent.front());
        string typeFound = items->second;
        cout << "typeFound is: " << typeFound << endl;;
        return typeFound;
    }
    // B. non-wild type
    else
    {
        auto items = elemMap.find(elemName);
        string typeFound = items->second;
        cout << "typeFound is: " << typeFound << endl;

        // Check that returned type is valid
        if (checkType(typeFound) == true)
        {
            return typeFound;
        }
        // Else it could be a parent/use/modify/etc
        else
        {
            return table;
        }
    }
}

// iter 2: support method to check if token is SUCH THAT expression
bool QueryProcessor::checkSuch(string token)
{
    // Check if token is "such"
    if (token == "such" | token == "Such")
    {
        next_token();
    }

    // Return boolean if element is an "that" clause
    return regex_match(token, regex(SUCH_EXPR));
}

// iter 2: support method to check if token is PATTERN expression
bool QueryProcessor::checkPatt(string token)
{
    // Return boolean if element is an pattern clause
    return regex_match(token, regex(PATTERN_EXPR));
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

// iter 3: support method to check if token is a wildcard ("_")
bool QueryProcessor::checkWild(string token) {
    // Return boolean if element is a '_'
    return regex_match(token, regex(WILD_EXPR));
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

// iter 3: support method to check for number
bool QueryProcessor::checkNum(string token)
{
    // Return boolean if element is an integer
    return regex_match(token, regex(NUM_EXPR));
}

// iter 3: process integer to string
string QueryProcessor::intToStr(int value)
{
    string valueInStr;
    stringstream OriginalInt;
    OriginalInt << value;
    OriginalInt >> valueInStr;

    return valueInStr;
}