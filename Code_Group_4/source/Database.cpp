#include "Database.h"

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
    string createVariableTableSQL = "CREATE TABLE variables ( variableName VARCHAR(255) PRIMARY KEY);";
    sqlite3_exec(dbConnection, createVariableTableSQL.c_str(), NULL, 0, &errorMessage);

	/// Constant ///
    // drop the existing constant table (if any)
    string dropConstantTableSQL = "DROP TABLE IF EXISTS constant";
    sqlite3_exec(dbConnection, dropConstantTableSQL.c_str(), NULL, 0, &errorMessage);

    // create a constant table
    string createConstantTableSQL = "CREATE TABLE constant( constantValue VARCHAR(255));";
    sqlite3_exec(dbConnection, createConstantTableSQL.c_str(), NULL, 0, &errorMessage);

	/// Assignment ///
    // drop the existing assignment table (if any)
    string dropAssignmentTableSQL = "DROP TABLE IF EXISTS assignments";
    sqlite3_exec(dbConnection, dropAssignmentTableSQL.c_str(), NULL, 0, &errorMessage);

    // create an assignment table
    string createAssignmentTable = "CREATE TABLE assignments ( assignmentLines VARCHAR(255), lhs VARCHAR(255), rhs VARCHAR(255));";
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

	// initialize the result vector
	dbResults = vector<vector<string>>();
}

// method to close the database connection
void Database::close() {
	sqlite3_close(dbConnection);
}

///// Insert /////
// method to insert a procedure into the database
void Database::insertProcedure(string procedureName) {
	string insertProcedureSQL = "INSERT INTO procedures ('procedureName') VALUES ('" + procedureName + "');";
	sqlite3_exec(dbConnection, insertProcedureSQL.c_str(), NULL, 0, &errorMessage);
}

// method to insert a variable into the database
void Database::insertVariable(string variableName) {
	string insertVariableSQL = "INSERT INTO variables ('variableName') VALUES ('" + variableName + "');";
	sqlite3_exec(dbConnection, insertVariableSQL.c_str(), NULL, 0, &errorMessage);
}

// method to insert a constant into the database
void Database::insertConstant(string constantValue) {
	string insertConstantSQL = "INSERT INTO constants ('constantValue') VALUES ('" + constantValue + "'); ";
	sqlite3_exec(dbConnection, insertConstantSQL.c_str(), NULL, 0, &errorMessage);
}

// method to insert an assignment into the database
void Database::insertAssignment(string assignmentLines, string lhs, string rhs) {
	string insertAssignmentSQL = "INSERT INTO assignments ('assignmentLines' , 'lhs' , 'rhs') VALUES ('" + assignmentLines + "' , '" + lhs + "' , '" + rhs + "'); ";
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

///// Get /////
// method to get all the procedures from the database
void Database::getProcedures(vector<string>& results){
	// clear the existing results
	dbResults.clear();

	// retrieve the procedures from the procedure table
	// The callback method is only used when there are results to be returned.
	string getProceduresSQL = "SELECT * FROM procedures;";
	sqlite3_exec(dbConnection, getProceduresSQL.c_str(), callback, 0, &errorMessage);

	// postprocess the results from the database so that the output is just a vector of procedure names
	for (vector<string> dbRow : dbResults) {
		string result;
		result = dbRow.at(0);
		results.push_back(result);
	}
}

// method to get all the variables from the database
void Database::getVariables(vector<string>& results) {
	// clear the existing results
	dbResults.clear();

	// retrieve the variables from the variable table
	// The callback method is only used when there are results to be returned.
	string getVariableSQL = "SELECT * FROM variables;";
	sqlite3_exec(dbConnection, getVariableSQL.c_str(), callback, 0, &errorMessage);

	// postprocess the results from the database so that the output is just a vector
	for (vector<string> dbRow : dbResults) {
		string result;
		result = dbRow.at(0);
		results.push_back(result);
	}
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
	for (vector<string> dbRow : dbResults) {
		string result;
		result = dbRow.at(0);
		results.push_back(result);
	}
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
	for (vector<string> dbRow : dbResults) {
		string result;
		result = dbRow.at(0);
		results.push_back(result);
	}
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
	for (vector<string> dbRow : dbResults) {
		string result;
		result = dbRow.at(0);
		results.push_back(result);
	}
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
	for (vector<string> dbRow : dbResults) {
		string result;
		result = dbRow.at(0);
		results.push_back(result);
	}
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
	for (vector<string> dbRow : dbResults) {
		string result;
		result = dbRow.at(0);
		results.push_back(result);
	}
}

///// Special Method /////
//void Database::insertExistingConstant(string constantName) 
//{
//	// clear the existing results
//	dbResults.clear();
//
//	// Insert constant values or create new value with 'NULL' value
//	string insertSingleConstantSQL = "INSERT INTO constants (constantName, constantValue) VALUES  ('" + constantName + "', (SELECT constantValue FROM constants WHERE constantname = '" + constantName + "' ORDER BY RowID DESC LIMIT 1));";
//	sqlite3_exec(dbConnection, insertSingleConstantSQL.c_str(), callback, 0, &errorMessage);
//}


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
