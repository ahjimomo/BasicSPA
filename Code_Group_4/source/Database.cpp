#include "Database.h"
#include <iostream>
#include <string>

using namespace std;

sqlite3* Database::dbConnection;
vector<vector<string>> Database::dbResults;
char* Database::errorMessage;

///// Create Table /////
// method to connect to the database and initialize tables in the database
void Database::initialize() {
    // open a database connection and store the pointer into dbConnection
    sqlite3_open("database.db", &dbConnection);

    /// Procedure ///
    // drop the existing procedure table (if any)
    string dropProcedureTableSQL = "DROP TABLE IF EXISTS procedures";
    sqlite3_exec(dbConnection, dropProcedureTableSQL.c_str(), NULL, 0, &errorMessage);

    // create a procedure table
    string createProcedureTableSQL = "CREATE TABLE procedures ( procedureName VARCHAR(255) PRIMARY KEY);";
    sqlite3_exec(dbConnection, createProcedureTableSQL.c_str(), NULL, 0, &errorMessage);

    /// Variable ///
    // drop the existing variable table (if any)
    string dropVariableTableSQL = "DROP TABLE IF EXISTS variables";
    sqlite3_exec(dbConnection, dropVariableTableSQL.c_str(), NULL, 0, &errorMessage);

    // create a variable table
    string createVariableTableSQL = "CREATE TABLE variables ( variableName VARCHAR(255), variableLine VARCHAR(255));";
    sqlite3_exec(dbConnection, createVariableTableSQL.c_str(), NULL, 0, &errorMessage);

    /// Constant ///
    // drop the existing constant table (if any)
    string dropConstantTableSQL = "DROP TABLE IF EXISTS constants";
    sqlite3_exec(dbConnection, dropConstantTableSQL.c_str(), NULL, 0, &errorMessage);

    // create a constant table
    string createConstantTableSQL = "CREATE TABLE constants( constantValue VARCHAR(255) PRIMARY KEY);";
    sqlite3_exec(dbConnection, createConstantTableSQL.c_str(), NULL, 0, &errorMessage);

    /// Assignment ///
    // drop the existing assignment table (if any)
    string dropAssignmentTableSQL = "DROP TABLE IF EXISTS assignments";
    sqlite3_exec(dbConnection, dropAssignmentTableSQL.c_str(), NULL, 0, &errorMessage);

    // create an assignment table
    string createAssignmentTable = "CREATE TABLE assignments ( assignmentLines VARCHAR(255) PRIMARY KEY, lhs VARCHAR(255), rhs VARCHAR(255));";
    sqlite3_exec(dbConnection, createAssignmentTable.c_str(), NULL, 0, &errorMessage);

    /// Statement ///
    // drop the existing statement table (if any)
    string dropStatementTableSQL = "DROP TABLE IF EXISTS statements";
    sqlite3_exec(dbConnection, dropStatementTableSQL.c_str(), NULL, 0, &errorMessage);

    // create a statement table
    string createStatementTable = "CREATE TABLE statements ( stmtLine VARCHAR(255) PRIMARY KEY);";
    sqlite3_exec(dbConnection, createStatementTable.c_str(), NULL, 0, &errorMessage);

    /// Read ///
    // drop the existing read table (if any)
    string dropReadTableSQL = "DROP TABLE IF EXISTS reads";
    sqlite3_exec(dbConnection, dropReadTableSQL.c_str(), NULL, 0, &errorMessage);

    // create a read table
    string createReadTableSQL = "CREATE TABLE reads ( line VARCHAR(255) PRIMARY KEY);";
    sqlite3_exec(dbConnection, createReadTableSQL.c_str(), NULL, 0, &errorMessage);

    /// Print ///
    // drop the existing print table (if any)
    string dropPrintTableSQL = "DROP TABLE IF EXISTS prints";
    sqlite3_exec(dbConnection, dropPrintTableSQL.c_str(), NULL, 0, &errorMessage);

    // create a print table
    string createPrintTableSQL = "CREATE TABLE prints ( line VARCHAR(255) PRIMARY KEY);";
    sqlite3_exec(dbConnection, createPrintTableSQL.c_str(), NULL, 0, &errorMessage);

    /// Parents ///
    // drop the existing parents table (if any)
    string dropParentTableSQL = "DROP TABLE IF EXISTS parents";
    sqlite3_exec(dbConnection, dropParentTableSQL.c_str(), NULL, 0, &errorMessage);

    // create a parents table
    string createParentTableSQL = "CREATE TABLE parents ( type VARCHAR(255), parentLine VARCHAR(255), childLine VARCHAR(255));";
    sqlite3_exec(dbConnection, createParentTableSQL.c_str(), NULL, 0, &errorMessage);

    /// Parents* ///
    // drop the existing parentstars table (if any)
    string dropParentStarTableSQL = "DROP TABLE IF EXISTS parentstars";
    sqlite3_exec(dbConnection, dropParentStarTableSQL.c_str(), NULL, 0, &errorMessage);

    // create a parentstars table
    string createParentStarTableSQL = "CREATE TABLE parentstars ( type VARCHAR(255), parentLine VARCHAR(255), childLine VARCHAR(255));";
    sqlite3_exec(dbConnection, createParentStarTableSQL.c_str(), NULL, 0, &errorMessage);

    /// Pattern ///
    // drop the existing patterns table (if any)
    string dropPatternTableSQL = "DROP TABLE IF EXISTS patterns";
    sqlite3_exec(dbConnection, dropPatternTableSQL.c_str(), NULL, 0, &errorMessage);

    // create a patterns table
    string createPatternTable = "CREATE TABLE patterns ( patternLine VARCHAR(255) PRIMARY KEY, lhs VARCHAR(255), rhs VARCHAR(255));";
    sqlite3_exec(dbConnection, createPatternTable.c_str(), NULL, 0, &errorMessage);

    /// Uses ///
    // drop the existing uses table (if any)
    string dropUseTableSQL = "DROP TABLE IF EXISTS uses";
    sqlite3_exec(dbConnection, dropUseTableSQL.c_str(), NULL, 0, &errorMessage);

    // create a uses table
    string createUseTable = "CREATE TABLE uses ( useLine VARCHAR(255), user VARCHAR(255), used VARCHAR(255), type VARCHAR(255));";
    sqlite3_exec(dbConnection, createUseTable.c_str(), NULL, 0, &errorMessage);

    /// Calls* ///
    // drop the existing callStars table (if any)
    string dropCallStarTableSQL = "DROP TABLE IF EXISTS callstars";
    sqlite3_exec(dbConnection, dropCallStarTableSQL.c_str(), NULL, 0, &errorMessage);

    // create a callStars table
    string createCallStarsTable = "CREATE TABLE callstars ( callLine VARCHAR(255), preceding VARCHAR(255), calledP VARCHAR(255));";
    sqlite3_exec(dbConnection, createCallStarsTable.c_str(), NULL, 0, &errorMessage);

    /// Calls ///
    // drop the existing calls table (if any)
    string dropCallTableSQL = "DROP TABLE IF EXISTS calls";
    sqlite3_exec(dbConnection, dropCallTableSQL.c_str(), NULL, 0, &errorMessage);

    // create a calls table
    string createCallsTable = "CREATE TABLE calls ( callLine VARCHAR(255), precedence VARCHAR(255), calledP VARCHAR(255));";
    sqlite3_exec(dbConnection, createCallsTable.c_str(), NULL, 0, &errorMessage);

    /// Modifies ///
    // drop the existing modifies table (if any)
    string dropModifiesTableSQL = "DROP TABLE IF EXISTS modifies";
    sqlite3_exec(dbConnection, dropModifiesTableSQL.c_str(), NULL, 0, &errorMessage);

    // create a modifies table
    string createModifiesTable = "CREATE TABLE modifies ( modifyLine VARCHAR(255), modifier VARCHAR(255), modified VARCHAR(255), type VARCHAR(255));";
    sqlite3_exec(dbConnection, createModifiesTable.c_str(), NULL, 0, &errorMessage);

    /// [Experimental] Main Data Source ///
    string dropMainTableSQL = "DROP TABLE IF EXISTS mains";
    sqlite3_exec(dbConnection, dropMainTableSQL.c_str(), NULL, 0, &errorMessage);

    string createMainTable = "CREATE TABLE mains ( Idx VARCHAR(255) PRIMARY KEY, type VARCHAR(255), procedure VARCHAR(255), variable VARCHAR(255), lhs VARCHAR(255), rhs VARCHAR(255));";
    sqlite3_exec(dbConnection, createMainTable.c_str(), NULL, 0, &errorMessage);

    /// Next ///
    // drop the existing nexts table (if any)
    string dropNextsTableSQL = "DROP TABLE IF EXISTS nexts";
    sqlite3_exec(dbConnection, dropNextsTableSQL.c_str(), NULL, 0, &errorMessage);

    // create a nexts table
    string createNextsTable = "CREATE TABLE nexts ( prevLine VARCHAR(255), nextLine VARCHAR(255));";
    sqlite3_exec(dbConnection, createNextsTable.c_str(), NULL, 0, &errorMessage);

    /// Next* ///
    // drop the existing nextstars table (if any)
    string dropNextStarsTableSQL = "DROP TABLE IF EXISTS nextstars";
    sqlite3_exec(dbConnection, dropNextStarsTableSQL.c_str(), NULL, 0, &errorMessage);

    // create a nextstars table
    string createNextStarsTable = "CREATE TABLE nextstars ( prevLine VARCHAR(255), nextLine VARCHAR(255));";
    sqlite3_exec(dbConnection, createNextStarsTable.c_str(), NULL, 0, &errorMessage);

    // initialize the result vector
    dbResults = vector<vector<string>>();
}

// method to close the database connection
void Database::close() {
    sqlite3_close(dbConnection);
}

///// Insert /////
// [Experimental] method to insert a main into the database
void Database::insertMain(string Idx, string type, string procedure, string variable, string lhs, string rhs) {
    string insertMainSQL = "INSERT INTO mains ('Idx', 'type', 'procedure', 'variable', 'lhs', 'rhs') VALUES ('" + Idx + "', '" + type + "', '" + procedure + "', '" + variable + "', '" + lhs + "', '" + rhs + "');";
    sqlite3_exec(dbConnection, insertMainSQL.c_str(), NULL, 0, &errorMessage);
}

// iter 3: method to insert an next into the database
void Database::insertNext(string prevLine, string nextLine) {
    string insertNextSQL = "INSERT INTO nexts ('prevLine' , 'nextLine') VALUES ('" + prevLine + "' , '" + nextLine + "');";
    sqlite3_exec(dbConnection, insertNextSQL.c_str(), NULL, 0, &errorMessage);
}

// method to insert a procedure into the database
void Database::insertProcedure(string procedureName) {
    string insertProcedureSQL = "INSERT INTO procedures ('procedureName') VALUES ('" + procedureName + "');";
    sqlite3_exec(dbConnection, insertProcedureSQL.c_str(), NULL, 0, &errorMessage);
}

// method to insert a variable into the database
void Database::insertVariable(string variableName, string variableLine) {
    string insertVariableSQL = "INSERT INTO variables ('variableName', 'variableLine') VALUES ('" + variableName + "', '" + variableLine + "');";
    sqlite3_exec(dbConnection, insertVariableSQL.c_str(), NULL, 0, &errorMessage);
}

// method to insert a constant into the database
void Database::insertConstant(string constantValue) {
    string insertConstantSQL = "INSERT INTO constants ('constantValue') VALUES ('" + constantValue + "');";
    sqlite3_exec(dbConnection, insertConstantSQL.c_str(), NULL, 0, &errorMessage);
}

// method to insert an assignment into the database
void Database::insertAssignment(string assignmentLines, string lhs, string rhs) {
    string insertAssignmentSQL = "INSERT INTO assignments ('assignmentLines' , 'lhs' , 'rhs') VALUES ('" + assignmentLines + "' , '" + lhs + "' , '" + rhs + "');";
    sqlite3_exec(dbConnection, insertAssignmentSQL.c_str(), NULL, 0, &errorMessage);
}

// method to insert a statement into the database
void Database::insertStatement(string statementLine) {
    string insertStatementSQL = "INSERT INTO statements ('stmtLine') VALUES ('" + statementLine + "');";
    sqlite3_exec(dbConnection, insertStatementSQL.c_str(), NULL, 0, &errorMessage);
}

// method to insert a read into the database
void Database::insertRead(string readLine) {
    string insertReadSQL = "INSERT INTO reads ('line') VALUES ('" + readLine + "');";
    sqlite3_exec(dbConnection, insertReadSQL.c_str(), NULL, 0, &errorMessage);
}

// method to insert a print into the database
void Database::insertPrint(string printLine) {
    string insertPrintSQL = "INSERT INTO prints ('line') VALUES ('" + printLine + "');";
    sqlite3_exec(dbConnection, insertPrintSQL.c_str(), NULL, 0, &errorMessage);
}

// iter 2: method to insert a parent into the database
void Database::insertParentStar(string type, string parentLine, string childLine) {
    string insertParentStarSQL = "INSERT INTO parentstars ('type', 'parentLine', 'childLine') VALUES ('" + type + "', '" + parentLine + "', '" + childLine + "');";
    sqlite3_exec(dbConnection, insertParentStarSQL.c_str(), NULL, 0, &errorMessage);
}

// iter 2: method to insert a parent into the database
void Database::insertParent(string type, string parentLine, string childLine) {
    string insertParentSQL = "INSERT INTO parents ('type', 'parentLine', 'childLine') VALUES ('" + type + "', '" + parentLine + "', '" + childLine + "');";
    sqlite3_exec(dbConnection, insertParentSQL.c_str(), NULL, 0, &errorMessage);
}

// iter 2: method to insert an pattern into the database
void Database::insertPattern(string patternLine, string lhs, string rhs) {
    string insertPatternSQL = "INSERT INTO patterns ('patternLine', 'lhs', 'rhs') VALUES ('" + patternLine + "' , '" + lhs + "' , '" + rhs + "');";
    sqlite3_exec(dbConnection, insertPatternSQL.c_str(), NULL, 0, &errorMessage);
}

// iter 2: method to insert an use into the database
void Database::insertUse(string useLine, string user, string used, string type) {
    string insertUseSQL = "INSERT INTO uses ('useLine' , 'user' , 'used', 'type') VALUES ('" + useLine + "' , '" + user + "' , '" + used + "', '" + type + "');";
    sqlite3_exec(dbConnection, insertUseSQL.c_str(), NULL, 0, &errorMessage);
}

// iter 2: method to insert a call-star into the database
void Database::insertCallStar(string callLine, string preceding, string calledP) {
    string insertCallStarSQL = "INSERT INTO callstars ('callLine' , 'preceding' , 'calledP') VALUES ('" + callLine + "' , '" + preceding + "' , '" + calledP + "');";
    sqlite3_exec(dbConnection, insertCallStarSQL.c_str(), NULL, 0, &errorMessage);
}

// iter 2: method to insert a call into the database
void Database::insertCall(string callLine, string precedence, string calledP) {
    string insertCallSQL = "INSERT INTO calls ('callLine' , 'precedence' , 'calledP') VALUES ('" + callLine + "' , '" + precedence + "' , '" + calledP + "');";
    sqlite3_exec(dbConnection, insertCallSQL.c_str(), NULL, 0, &errorMessage);
}

// iter 2: method to insert a modify into the database
void Database::insertModifies(string modifyLine, string modifier, string modified, string type) {
    string insertModifiesSQL = "INSERT INTO modifies ('modifyLine' , 'modifier' , 'modified', 'type') VALUES ('" + modifyLine + "' , '" + modifier + "' , '" + modified + "', '" + type + "');";
    sqlite3_exec(dbConnection, insertModifiesSQL.c_str(), NULL, 0, &errorMessage);
}

///// Supprting: PostProcess Result /////
void Database::postProcess(vector<vector<string>>& dbResults, vector<string>& results ) {
    results.clear();
    for (vector<string> dbRow : dbResults) {
        string result;
        result = dbRow.at(0);
        results.push_back(result);
    }
}

// callback method to put one row of results from the database into the dbResults vector
// This method is called each time a row of results is returned from the database
int Database::callback(void* NotUsed, int argc, char** argv, char** azColName) {
    NotUsed = 0;
    vector<string> dbRow;

    // argc is the number of columns for this row of results
    // argv contains the values for the columns
    // Each value is pushed into a vector.
    for (int i = 0; i < argc; i++) {
        dbRow.push_back(argv[i]);
    }

    // The row is pushed to the vector for storing all rows of results
    dbResults.push_back(dbRow);

    return 0;
}


///// Get /////
// iter 2: method to get Child Line index(es) from Parent/Parent* Table 
void Database::getCLine(string table, string pType, vector<string>& results)
{
    // clear existing results
    dbResults.clear();
    string getCLineSQL;

    if (pType == "_")
    {
        getCLineSQL = "SELECT DISTINCT childLine FROM " + table + ";";
        cout << "Query: " << getCLineSQL << endl;
    }
    else
    {
        getCLineSQL = "SELECT DISTINCT childLine FROM " + table + " WHERE type = '" + pType + "';";
        cout << "Query: " << getCLineSQL << endl;
    }
    sqlite3_exec(dbConnection, getCLineSQL.c_str(), callback, 0, &errorMessage);

    postProcess(dbResults, results);
}

// iter 2: method to get Parent Line index(es) from Parent/Parent* Table
void Database::getPLine(string table, string pType, vector<string>& results)
{
    // clear existing results
    dbResults.clear();
    string getPLineSQL;

    if (pType == "_")
    {
        getPLineSQL = "SELECT DISTINCT parentLine FROM " + table + ";";
    }
    else
    {
        getPLineSQL = "SELECT DISTINCT parentLine FROM " + table + " WHERE type = '" + pType + "';";
    }

    sqlite3_exec(dbConnection, getPLineSQL.c_str(), callback, 0, &errorMessage);

    postProcess(dbResults, results);
}

// iter 2: method to get all the ifs with stipulated line number
// Arguments:
//  - string parentType: While/If/Else
//  - string parentOrChild: "parentLine" OR "childLine"
//  - int specificLine: 0 for everything and other integer values for specify index
// Returns:
//  - 2 columns [type, line index] (ie. [if, 3])
void Database::getIfs(vector<string>& results, string parentType, string parentOrChild, string specificLine) 
{
    // clear the existing results
    dbResults.clear();

    if (specificLine == "0")
    {
        string getIfsSQL = "SELECT DISTINCT CASE " + parentOrChild + " WHEN 'root' THEN 0 ELSE " + parentOrChild + " END " + parentOrChild + " FROM parents WHERE type = '" + parentType + "';";
        sqlite3_exec(dbConnection, getIfsSQL.c_str(), callback, 0, &errorMessage);
    }
    else
    {
        string getIfsSQL = "SELECT DISTINCT CASE " + parentOrChild + " WHEN 'root' THEN 0 ELSE " + parentOrChild + " END " + parentOrChild + " FROM parents WHERE type = '" + parentType + "' AND " + parentOrChild + " = '" + specificLine + "';";
        sqlite3_exec(dbConnection, getIfsSQL.c_str(), callback, 0, &errorMessage);
    }

    // postprocess the results from the database so that the output is just a vector of procedure names
    postProcess(dbResults, results);
}

// iter 2: method to get all the parent Whiles
void Database::getWhiles(vector<string>& results, string parentType, string parentOrChild, string specificLine) 
{
    // clear the existing results
    dbResults.clear();

    if (specificLine == "0")
    {
        string getWhilesSQL = "SELECT DISTINCT CASE " + parentOrChild + " WHEN 'root' THEN 0 ELSE " + parentOrChild + " END " + parentOrChild + " FROM parents WHERE type = '" + parentType + "';";
        sqlite3_exec(dbConnection, getWhilesSQL.c_str(), callback, 0, &errorMessage);
    }
    else
    {
        string getWhilesSQL = "SELECT DISTINCT CASE " + parentOrChild + " WHEN 'root' THEN 0 ELSE " + parentOrChild + " END " + parentOrChild + " FROM parents WHERE type = '" + parentType + "' AND " + parentOrChild + " = '" + specificLine + "';";
        sqlite3_exec(dbConnection, getWhilesSQL.c_str(), callback, 0, &errorMessage);
    }

    // postprocess the results from the database so that the output is just a vector of procedure names
    postProcess(dbResults, results);
}


// iter 3: method to the next* from the database
void Database::getNextStars(string getLine, vector<string>& results) {
    // clear the existing results
    dbResults.clear();

    // retrieve the (preceding line, current line) from the calls table
    // The callback method is only used when there are results to be returned.
    string getNextStarsSQL = "WITH RECURSIVE cte AS (\
        SELECT nextLine\
        FROM nexts\
        WHERE prevLine = " + getLine + "\
        UNION\
        SELECT t2.nextLine\
        FROM nexts t2\
        INNER JOIN cte ON cte.nextLine = t2." + getLine + "\
        )\
        SELECT nextLine FROM cte;";
    sqlite3_exec(dbConnection, getNextStarsSQL.c_str(), callback, 0, &errorMessage);

    // postprocess the results from the database so that the output is just a vector of nextLine indexes
    postProcess(dbResults, results);
}

// iter 3: method to the next from the database
void Database::getNexts(string getLine, vector<string>& results) {
    // clear the existing results
    dbResults.clear();

    // retrieve the (preceding line, current line) from the calls table
    // The callback method is only used when there are results to be returned.
    string getNextsSQL = "SELECT nextLine FROM nexts WHERE prevLine = " + getLine + ";";
    sqlite3_exec(dbConnection, getNextsSQL.c_str(), callback, 0, &errorMessage);

    // postprocess the results from the database so that the output is just a vector of procedure names
    postProcess(dbResults, results);
}

// iter 2: method to get all the calls (1 preceding line only) from the database
void Database::getModifies(vector<string>& results) {
    // clear the existing results
    dbResults.clear();

    // retrieve the (preceding procedure, current procedure) from the calls table
    // The callback method is only used when there are results to be returned.
    string getModifiesSQL = "SELECT * FROM modifies;";
    sqlite3_exec(dbConnection, getModifiesSQL.c_str(), callback, 0, &errorMessage);

    // postprocess the results from the database so that the output is just a vector of procedure names
    postProcess(dbResults, results);
}

void Database::getMLine(string table, string pType, vector<string>& results)
{
    // clear the existing results
    dbResults.clear();

    // retrieve the (preceding procedure, current procedure) from the calls table
    // The callback method is only used when there are results to be returned.
    string getMLineSQL = "SELECT modifyLine FROM modifies WHERE type = " + pType + "; ";
    sqlite3_exec(dbConnection, getMLineSQL.c_str(), callback, 0, &errorMessage);

    // postprocess the results from the database so that the output is just a vector of procedure names
    postProcess(dbResults, results);
}

// iter 2: method to get all the calls (1 preceding procedure only) from the database
void Database::getCalls(vector<string>& results) {
    // clear the existing results
    dbResults.clear();

    // retrieve the (preceding procedure, current procedure) from the calls table
    // The callback method is only used when there are results to be returned.
    string getCallsSQL = "SELECT precedence, calledP FROM calls;";
    sqlite3_exec(dbConnection, getCallsSQL.c_str(), callback, 0, &errorMessage);

    // postprocess the results from the database so that the output is just a vector of procedure names
    postProcess(dbResults, results);
}

// iter 2: method to get all the calls (all preceding procedures) from the database
void Database::getCallStars(vector<string>& results) {
    // clear the existing results
    dbResults.clear();

    // retrieve the procedures from the callStars table
    // The callback method is only used when there are results to be returned.
    string getCallStarsSQL = "SELECT preceding, calledP FROM callstars;";
    sqlite3_exec(dbConnection, getCallStarsSQL.c_str(), callback, 0, &errorMessage);

    // postprocess the results from the database so that the output is just a vector of procedure names
    postProcess(dbResults, results);
}
 
// iter 2: method to get all the uses (user, use, type) from the database
void Database::getUses(vector<string>& results) {
    // clear the existing results
    dbResults.clear();

    // retrieve the lines from the udrds table
    // The callback method is only used when there are results to be returned.
    string getUsesSQL = "SELECT user, used, type FROM uses;";
    sqlite3_exec(dbConnection, getUsesSQL.c_str(), callback, 0, &errorMessage);

    // postprocess the results from the database so that the output is just a vector of procedure names
    postProcess(dbResults, results);
}
 
// iter 2: method to get all the patterns (lhs, rhs) from the database
void Database::getPatterns(vector<string>& results, string lhs, string rhs) {
    // clear the existing results
    dbResults.clear();

    // retrieve the lhs, rhs from the patterns table
    // The callback method is only used when there are results to be returned.
    if (lhs.empty()) {
        string getPatternsSQL = "SELECT patternLine FROM patterns WHERE rhs LIKE '" + rhs + "'; ";
        //cout << "sql1== " << getPatternsSQL << endl;
        sqlite3_exec(dbConnection, getPatternsSQL.c_str(), callback, 0, &errorMessage);
    }

    else if (rhs.empty()) {
        string getPatternsSQL = "SELECT patternLine FROM patterns WHERE lhs LIKE '" + lhs + "'; ";
        //cout << "sql2== " << getPatternsSQL << endl;
        sqlite3_exec(dbConnection, getPatternsSQL.c_str(), callback, 0, &errorMessage);
    }

    else {
        string getPatternsSQL = "SELECT patternLine FROM patterns WHERE lhs LIKE '" + lhs + "' AND rhs LIKE '" + rhs + "'; ";
        //cout << "sql3== " << getPatternsSQL << endl;
        sqlite3_exec(dbConnection, getPatternsSQL.c_str(), callback, 0, &errorMessage);
    }

    // postprocess the results from the database so that the output is just a vector of procedure names
    //cout << "program enter here" << endl;

    postProcess(dbResults, results);

    //cout << "results size: " << results.size() << endl;

    //cout << "program enter here too" << endl;
}

// iter 2: method to get all the parents* statements from the database
void Database::getParentStars(vector<string>& results, string stmtLine) {
    // clear the existing results
    dbResults.clear();

    // retrieve the procedures from the whiles table
    // The callback method is only used when there are results to be returned.
    string getParentStarsSQL = "SELECT DISTINCT parentLine FROM parentstars WHERE childLine = '" + stmtLine +"';";
    sqlite3_exec(dbConnection, getParentStarsSQL.c_str(), callback, 0, &errorMessage);

    // postprocess the results from the database so that the output is just a vector of procedure names
    postProcess(dbResults, results);
}

// iter 2: method to get all the parents statements from the database
void Database::getParents(vector<string>& results, string stmtLine) {
    // clear the existing results
    dbResults.clear();

    // retrieve the procedures from the whiles table
    // The callback method is only used when there are results to be returned.
    string getParentsSQL = "SELECT DISTINCT parentLine FROM parents WHERE childLine = '" + stmtLine + "';";
    sqlite3_exec(dbConnection, getParentsSQL.c_str(), callback, 0, &errorMessage);

    // postprocess the results from the database so that the output is just a vector of procedure names
    postProcess(dbResults, results);
}

// method to get all the procedures from the database
void Database::getProcedures(vector<string>& results){
    // clear the existing results
    dbResults.clear();

    // retrieve the procedures from the procedure table
    // The callback method is only used when there are results to be returned.
    string getProceduresSQL = "SELECT * FROM procedures;";
    sqlite3_exec(dbConnection, getProceduresSQL.c_str(), callback, 0, &errorMessage);

    // postprocess the results from the database so that the output is just a vector of procedure names
    postProcess(dbResults, results);
}

// method to get all the variables from the database
void Database::getVariables(vector<string>& results) {
    // clear the existing results
    dbResults.clear();

    // retrieve the variables from the variable table
    // The callback method is only used when there are results to be returned.
    string getVariableSQL = "SELECT DISTINCT variableName FROM variables;";
    sqlite3_exec(dbConnection, getVariableSQL.c_str(), callback, 0, &errorMessage);

    // postprocess the results from the database so that the output is just a vector
    postProcess(dbResults, results);
}

void Database::getVariablesIdx(vector<string>& results) {
    // clear the existing results
    dbResults.clear();

    // retrieve the variables from the variable table
    // The callback method is only used when there are results to be returned.
    string getVariableIdxSQL = "SELECT DISTINCT variableLine FROM variables;";
    sqlite3_exec(dbConnection, getVariableIdxSQL.c_str(), callback, 0, &errorMessage);

    // postprocess the results from the database so that the output is just a vector
    postProcess(dbResults, results);
}

// method to get all the constants values from the database
void Database::getConstants(vector<string>& results) {
    // clear the existing results
    dbResults.clear();

    // retrieve the constants from the constants table
    // The callback method is only used when there are results to be returned.
    string getConstantSQL = "SELECT * FROM constants;";
    sqlite3_exec(dbConnection, getConstantSQL.c_str(), callback, 0, &errorMessage);

    // postprocess the results from the database so that the output is just a vector
    postProcess(dbResults, results);
}

// method to get all the assignment statements from the database
void Database::getAssignments(vector<string>& results) {
    // clear the existing results
    dbResults.clear();

    // retrieve the assignments from the assignments table
    // The callback method is only used when there are results to be returned.
    string getAssignmentSQL = "SELECT assignmentLines FROM assignments;";
    sqlite3_exec(dbConnection, getAssignmentSQL.c_str(), callback, 0, &errorMessage);

    // postprocess the results from the database so that the output is just a vector
    postProcess(dbResults, results);
}


// method to get all the statements from the database
void Database::getStatements(vector<string>& results) {
    // clear the existing results
    dbResults.clear();

    // retrieve the statements from the statement table
    // The callback method is only used when there are results to be returned.
    string getStatementSQL = "SELECT * FROM statements;";
    sqlite3_exec(dbConnection, getStatementSQL.c_str(), callback, 0, &errorMessage);

    // postprocess the results from the database so that the output is just a vector
    postProcess(dbResults, results);
}

// method to get all the read lines from the database
void Database::getReads(vector<string>& results) {
    // clear the existing results
    dbResults.clear();

    // retrieve the read lines from the reads table
    // The callback method is only used when there are results to be returned.
    string getReadSQL = "SELECT * FROM reads;";
    sqlite3_exec(dbConnection, getReadSQL.c_str(), callback, 0, &errorMessage);

    // postprocess the results from the database so that the output is just a vector
    postProcess(dbResults, results);
}

// method to get all the print statements from the database
void Database::getPrints(vector<string>& results) {
    // clear the existing results
    dbResults.clear();

    // retrieve the print lines from the print table
    // The callback method is only used when there are results to be returned.
    string getPrintSQL = "SELECT * FROM prints;";
    sqlite3_exec(dbConnection, getPrintSQL.c_str(), callback, 0, &errorMessage);

    // postprocess the results from the database so that the output is just a vector
    postProcess(dbResults, results);
}

// Support Methods
// iter 2: process integer to string
string Database::intToStr(int value)
{
    string valueInStr;
    stringstream OriginalInt;
    OriginalInt << value;
    OriginalInt >> valueInStr;

    return valueInStr;
}