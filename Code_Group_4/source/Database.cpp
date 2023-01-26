#include "Database.h"

sqlite3* Database::dbConnection;
vector<vector<string>> Database::dbResults;
char* Database::errorMessage;

///// Initialize Methods /////
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

	/// Variables ///
	// drop the existing variable table (if any)
	string dropVariableTableSQL = "DROP TABLE IF EXISTS variables";
	sqlite3_exec(dbConnection, dropVariableTableSQL.c_str(), NULL, 0, &errorMessage);

	// create a procedure table
	string createVariableTableSQL = "CREATE TABLE variables ( variableName VARCHAR(255) PRIMARY KEY);";
	sqlite3_exec(dbConnection, createVariableTableSQL.c_str(), NULL, 0, &errorMessage);

	/// Read ///
	// drop the existing read table (if any)
	string dropReadTableSQL = "DROP TABLE IF EXISTS reads";
	sqlite3_exec(dbConnection, dropReadTableSQL.c_str(), NULL, 0, &errorMessage);

	// create a read table
	string createReadTableSQL = "CREATE TABLE reads ( line VARCHAR(255) PRIMARY KEY);";
	sqlite3_exec(dbConnection, createReadTableSQL.c_str(), NULL, 0, &errorMessage);

	/*
	procedure
	read
	print
	assignment (
	variable (string, integer, None)
	constant
	*/

	// initialize the result vector
	dbResults = vector<vector<string>>();
}

// method to close the database connection
void Database::close() {
	sqlite3_close(dbConnection);
}

///// Insert Methods /////
// Insert procedure into database
void Database::insertProcedure(string procedureName) {
	string insertProcedureSQL = "INSERT INTO procedures ('procedureName') VALUES ('" + procedureName + "');";
	sqlite3_exec(dbConnection, insertProcedureSQL.c_str(), NULL, 0, &errorMessage);
}

// Insert variable into database
void Database::insertVariable(string variableName) {
	string insertVariableSQL = "INSERT INTO variables ('variableName') VALUES ('" + variableName + "');";
	sqlite3_exec(dbConnection, insertVariableSQL.c_str(), NULL, 0, &errorMessage);
}

// Insert read into database
void Database::insertRead(string readLine) {
	string insertReadSQL = "INSERT INTO reads ('line') VALUES ('" + readLine + "');";
	sqlite3_exec(dbConnection, insertReadSQL.c_str(), NULL, 0, &errorMessage);
}

///// Get Methods /////
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

	// retrieve the procedures from the procedure table
	// The callback method is only used when there are results to be returned.
	string getVariableSQL = "SELECT * FROM variables;";
	sqlite3_exec(dbConnection, getVariableSQL.c_str(), callback, 0, &errorMessage);

	// postprocess the results from the database so that the output is just a vector of procedure names
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

	// retrieve the procedures from the procedure table
	// The callback method is only used when there are results to be returned.
	string getReadSQL = "SELECT * FROM reads;";
	sqlite3_exec(dbConnection, getReadSQL.c_str(), callback, 0, &errorMessage);

	// postprocess the results from the database so that the output is just a vector of procedure names
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
