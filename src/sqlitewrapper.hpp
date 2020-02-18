#ifndef SQLITEWRAPPER_HPP
#define SQLITEWRAPPER_HPP

#include<iostream>
#include<string>
#include<cstring>
#include<list>
#include"sqlite3.h"
#include"json.h"
//#include<json/json.h>

class Sqlitewrapper{

private:

    sqlite3** database_;
    bool isSlim_;

    sqlite3_stmt* generic_insert_stmt_ = NULL;
    sqlite3_stmt* generic_delete_stmt_ = NULL;
    sqlite3_stmt* generic_select_stmt_ = NULL;

public:
    /**
     * This enum lists the four possible datatypes a column in a database can have.
     */
    enum ColumnType{INT = SQLITE_INTEGER, FLOAT = SQLITE_FLOAT,
                    TEXT = SQLITE_TEXT, BLOB = SQLITE_BLOB};
    
    /**
     * This struct holds the type and the value of a specific field belonging to a DatabaseObject.
     * The value is always stored as a string in this struct, even though the type my not be TEXT.
     */
    struct DatabaseField{
        ColumnType type;
        std::string value;
    };

    struct ColumnAttributes{
        ColumnAttributes(ColumnType t, std::string n){
            type = t;
            name = n;
        }
        ColumnType type;
        std::string name;
    };
    /**
     * This Interface defines the methods that any Class has to implement in order to store them in
     * the database via this sqlitewrapper.
     * A DatabaseObject needs predefined statements for Inserting, etc. as well as a list of
     * DatabaseFields containing the type and values (as strings) of the field.
     */
    class DatabaseObject{
    public:
        virtual ~DatabaseObject(){};
        virtual std::string getInsertStatement() = 0;
        virtual std::string getDeleteStatement() = 0;
        virtual Json::Value getData() = 0;
        //virtual std::list<std::list<std::string>> getData() = 0;
        virtual std::list<ColumnAttributes> getLayout() = 0;
        
        int databaseId_ = -1;
        virtual int getDatabaseId() { return databaseId_;};
    private:
        static std::string tableName;                                   //the name of the table containing the objects of this type 
    };

    /**
     * Creates a new instance of the sqlitewrapper class.
     *
     * var database:        the contact database wrapped by this class 
     * var isSlim:          if true, statements will only be prepared when needed
     *                      if false, statements will be prepared immediatly
     *
     */
    Sqlitewrapper(sqlite3** database, bool isSlim);

    /**
     * Tries to open a database connection at the specified path and returns
     * the sqlite3 return code.
     * var filename:  the path and filename of the database
     *
     * return:  SQLITE_OK if successful
     *          errorcode else
     */
    int openDb(const char* filename);

    /**
     * Tries to create a sqlite3 database with the given filename .
     *
     * var filename:    the filename of the new db
     *
     * return:          SQLITE_OK if successful
     *                  errorcode else
     */
    int createDb(const char* filename);

    /**
     * Attempts to close the db currently opened within the wrapper by finalizing all prepared statements then closing the Db itself.
     *
     * return:          SQLITE_OK if successful
     *                  extended errorcode else
     *
     */
    int closeDb();

    /**
     * Creates an empty table in the database.
     *
     * var name:        the name of the table to be created
     * var arguments:   the names, types, and modifiers of the table's                          columns
     *               pattern:
     *                  (name
     *                  (INTEGER|TEXT|BLOB)
     *                  (NOT NULL|PRIMARY KEY)*
     *                  )*
     *
     * return:          SQLITE_OK if successful
     *                  extended errorcode else
     */
    int createTable(std::string name, std::string arguments);


    /**
     * Adds a DatabaseObject to the database by using an INSERT INTO statement on the specified table.
     *
     * var databaseObject:  the object to be added
     * var tableName:       the name of the table where the object is to be added
     *
     * return:          SQLITE_OK if successful
     *                  errorcode else
     */
    int addDatabaseEntry(DatabaseObject* databaseObject);

    /**
     * Deletes a contact from the database by using a DELETE statement on the contact table.
     *
     * var entryId:     the ID of the contact to be deleted
     * var tableName:   the name of the table the entry is to be deelted from
     *
     * return:          SQLITE_OK if successful
     *                  errorcode else
     */
    int deleteDatabaseEntry(DatabaseObject* databaseObject);

    /**
     * Carries out a Select operation on the database and writes a list of all retreived db-entries to
     * the given pointer.
     * 
     * var destination:                     the pointer where the list is written to
     * var selectStatement:                 the statement to be used to select db-entries
     * 
     * return: SQLITE_OK            if successful
     *                              errorcode else
     */

    int selectDatabaseObjects(DatabaseObject* destination, std::string selectStatement);

    /**
     * Defragments the database by copying the db to a temporary db ignoring all free spaces and copying it back to the original db overwriting it.
     *
     * Should be called perdiodically and after drastic changes to the db structure.
     * Only successful if there are no open transactions.
     *
     * return:
     *
     */
    int vacuumDb();

};
#endif