#pragma once

#include <string>
#include <vector>
#include "sqlite3.h"

using namespace std;

// The Database has to be a static class due to various constraints.
// It is advisable to just add the insert / get functions based on the given examples.
class Database {
public:
    // method to connect to the database and initialize tables in the database
    static void initialize();

    // method to close the database connection
    static void close();

    /// postprocess result ///
    static void postProcess(vector<vector<string>> & dbResults, vector<string>& results );

    /// procedure ///
    // method to insert a procedure into the database
    static void insertProcedure(string procedureName);

    // method to get all the procedures from the database
    static void getProcedures(vector<string>& results);


    /// variable ///
    // method to insert a variable into the database
    static void insertVariable(string variableName);

    // method to get all variables from the database
    static void getVariables(vector<string>& results);


    /// constant ///
    // method to insert a constant into the database
    // Error with creating constant table with [name, value] pair column as of 30 Jan 2023
    static void insertConstant(string constantName, string constantValue);

    // method to get all the constant values from the database
    static void getConstants(vector<string>& results);


    /// assignment ///
    // method to insert an assignment into the database
    static void insertAssignment(string assignmentLines, string lhs, string rhs);

    // method to get all the assignments from the database
    static void getAssignments(vector<string>& results);


    /// statement ///
    // method to insert a statement into the database
    static void insertStatement(string statementLine);

    // method to get all the statements from the database
    static void getStatements(vector<string>& results);


    /// read ///
    // method to insert a print into the database
    static void insertRead(string ReadLine);

    // method to get all the read lines from the database
    static void getReads(vector<string>& results);


    /// print ///
    // method to insert a print into the database
    static void insertPrint(string printLine);

    // method to get all the print lines from the database
    static void getPrints(vector<string>& results);


private:
    // the connection pointer to the database
    static sqlite3* dbConnection;
    // a vector containing the results from the database
    static vector<vector<string>> dbResults;
    // the error message from the database
    static char* errorMessage;
    // callback method to put one row of results from the database into the dbResults vector
    // This method is called each time a row of results is returned from the database
    static int callback(void* NotUsed, int argc, char** argv, char** azColName);


};

