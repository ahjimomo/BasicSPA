#pragma once

#include <string>
#include <vector>
#include <list>
#include <sstream>
#include <fstream>
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
    static void postProcess(vector<vector<string>>& dbResults, vector<string>& results );

    /// iter 2: Next ///
    // method to insert a next into the database
    static void insertNext(string prevLine, string nextLine);

    // method to get all the next from the database
    static void getNexts(string getLine, vector<string>& results);

    // method to get the next* from the database
    static void getNextStars(string getLine, vector<string>& results);

    /// iter 2: Parents / Parents* ///
    // method to insert a parent into the database
    static void insertParent(string type, string parentLine, string childLine);

    // method to get all the parents from the database
    static void getParents(vector<string>& results);

    // iter 2: method to get specific parents line from parents/parents* table
    static void getPLine(string table, string pType, vector<string>& results);

    // iter 2: method to get specific child lines from parents/parents* table
    static void getCLine(string table, string pType, vector<string>& results);

    // method to get all the while's parentLine from the database
    static void getWhiles(vector<string>& results, string parentType, string parentOrChild, string specificLine);

    // method to get all the if's parentLine from the database
    static void getIfs(vector<string>& results, string parentType, string parentOrChild, string specificLine);

    // method to insert a parents* into the database
    static void insertParentStar(string type, string parentLine, string childLine);

    // method to get all the parent* from the database
    static void getParentStars(vector<string>& results);
    

    /// procedure ///
    // method to insert a procedure into the database
    static void insertProcedure(string procedureName);

    // method to get all the procedures from the database
    static void getProcedures(vector<string>& results);


    /// variable ///
    // method to insert a variable into the database
    static void insertVariable(string variableName, string VariableLine);

    // method to get all variables from the database
    static void getVariables(vector<string>& results);

    // method to get all variables from the database
    static void getVariablesIdx(vector<string>& results);


    /// constant ///
    // method to insert a constant into the database
    static void insertConstant(string constantValue);

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


    /// pattern ///
    // method to insert a pattern into the database
    static void insertPattern(string patternLine, string lhs, string rhs);

    // method to get all the patterns from the database
    static void getPatterns(vector<string>& results, string lhs, string rhs);


    /// uses ///
    // method to insert a use into the database
    /* type is the column containing the code for filtering:
        - p2p: root procedure calling procedure that uses variable
        - print: print using variable
        - procedure: procedure using variable
        - assignment: assignee using variable (rhs)
        - if: if using container statement that contains variable
        - while: while using container statement that contains variable
    */
    static void insertUse(string useLine, string user, string used, string type);

    // method to get all the uses (user, used, type) from the database
    static void getUses(vector<string>& results);


    /// calls ///
    // method to insert call-star into the database
    /* 
    preceding are all the preceding procedures (ie. p1, p2, p3)
    calledP is the latest procedure called (ie. p4)
    */
    static void insertCallStar(string callLine, string preceding, string calledP);

    // method to get all the calls-star from the database
    static void getCallStars(vector<string>& results);


    /// calls ///
    // method to insert a call into the database
    /*
    precedence is the one-up preceding procedure (ie. p3)
    calledP is the latest procedure called (ie. p4)
    */
    static void insertCall(string callLine, string precedence, string calledP);

    // method to get all the call from the database
    static void getCalls(vector<string>& results);


    /// Modifies ///
    // method to insert a modifies into the database
    /* type is the column containing the code for filtering:
        - p2p: root procedure calling procedure that modifies variable
        - read: read modifies variable
        - procedure: procedure modifies variable
        - assignment: assigned modified by assignment (lhs)
        - if: if using container statement that modifies variable
        - while: while using container statement that modifies variable
    */
    static void insertModifies(string modifyLine, string modifier, string modified, string type);

    // method to get all the modifies (assigner, assigned, type) from the database
    static void getModifies(vector<string>& results);

    static void getMLine(string table, string pType, vector<string>& results);

    /// [Experimental] Main Dataset ///
    static void insertMain(string Idx, string type, string procedure, string variable, string lhs, string rhs);


    ///  Supporting Method ///
    // String to Int
    static string intToStr(int value);

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

