#include<iostream>
#include<string>
#include<cstring>
#include<stdexcept>
#include<sstream>
#include"sqlite3.h"
#include"sqlitewrapper.hpp"


/**
    * Creates a new instance of the sqlitewrapper class.
    *
    * var database:        the contact database wrapped by this class 
    * var isSlim:          if true, statements will only be prepared when needed
    *                      if false, statements will be prepared immediatly
    *
    */
Sqlitewrapper::Sqlitewrapper(sqlite3** database, bool isSlim){
    database_ = database;
    isSlim_ = isSlim;

    if(isSlim)
        return;
    else{
        //prepare unbound generic statements to INSERT, DELETE, and SELECT a contact fromt he contact table

        const char* pzTail;

        //unbound sql statements
        const char* unbound_insert_stmnt = "INSERT INTO contacts ( contact_id, first_name, last_name, email, phone) VALUES (@contact_id, @first_name, @last_name, @email, @phone);";

        const char* unbound_delete_stmnt = "DELETE FROM @table WHERE @attribute = @value";

        const char* unbound_select_stmnt = "INSERT INTO contacts ( contact_id, first_name, last_name, email, phone) VALUES (0, 'Alex', 'Ander', 'a', '0');";

        //prepare statements
        int prepareInsertSuccess = sqlite3_prepare_v2(
            *database_,
            unbound_insert_stmnt,
            -1,
            &generic_insert_stmt_,
            &pzTail
        );
        int prepareDeleteSuccess = sqlite3_prepare_v2(
            *database_,
            unbound_delete_stmnt,
            -1,
            &generic_delete_stmt_,
            &pzTail
        );
        int prepareSelectSuccess = sqlite3_prepare_v2(
            *database_,
            unbound_select_stmnt,
            -1,
            &generic_select_stmt_,
            &pzTail
        );
    }
}


/**
    * Tries to open a database connection at the specified path and returns
    * the sqlite3 return code.
    * var path:     the path and path to the database
    *
    * return:       SQLITE_OK if successful
    *               extended errorcode else
    */
int Sqlitewrapper::openDb(const char *path){
    int resultCode = sqlite3_open_v2(path, database_, SQLITE_OPEN_READWRITE, NULL);
    if(resultCode != SQLITE_OK)
        resultCode = sqlite3_extended_errcode(*database_);
    return resultCode;
}

/**
    * Tries to create a sqlite3 database with at the given path.
    *
    * var path:         the path to the new db
    *
    * return:           SQLITE_OK if successful
    *                   errorcode else
    */
int Sqlitewrapper::createDb(const char* path){
    int resultCode = sqlite3_open_v2(path, database_, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if(resultCode != SQLITE_OK)
        resultCode = sqlite3_extended_errcode(*database_);
    return resultCode;
}

/**
     * Attempts to close the db currently opened within the wrapper by finalizing all prepared statements then closing the Db itself.
     *
     * return:          SQLITE_OK if successful
     *                  extended errorcode else
     *
     * "Applications should finalize all prepared statements, close all BLOB handles, and finish all sqlite3_backup objects associated with the sqlite3 object prior to attempting to close the object. If sqlite3_close_v2() is called on a database connection that still has outstanding prepared statements, BLOB handles, and/or sqlite3_backup objects then it returns SQLITE_OK and the deallocation of resources is deferred until all prepared statements, BLOB handles, and sqlite3_backup objects are also destroyed."
     */
int Sqlitewrapper::closeDb(){
    if(generic_delete_stmt_ != NULL){
        if(sqlite3_finalize(generic_delete_stmt_) !=  SQLITE_OK){
            std::cout << "Could not finalize generic delete stmt\n";
            return sqlite3_extended_errcode(*database_);
        }
    }
    if(generic_insert_stmt_ != NULL){
        if(sqlite3_finalize(generic_insert_stmt_) !=  SQLITE_OK){
            std::cout << "Could not finalize generic insert stmt\n";
            return sqlite3_extended_errcode(*database_);
        }
    }
    if(generic_select_stmt_ != NULL){
        if(sqlite3_finalize(generic_select_stmt_) !=  SQLITE_OK){
            std::cout << "Could not finalize generic select stmt\n";
            return sqlite3_extended_errcode(*database_);
        }
    }
    return sqlite3_close_v2(*database_);
}


/**
     * Creates an empty table in the database.
     *
     * var name:        The name of the table to be created.
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
int Sqlitewrapper::createTable(std::string name, std::string arguments){
    int stepReturnValue;
    int finalizeReturnValue;
    int prepareReturnValue;

    std::string temp = "CREATE TABLE IF NOT EXISTS " + name + " (" + arguments + ");";

    const char* add_table_stmnt = temp.c_str();

    sqlite3_stmt* ppStmt;
    const char* pzTail;
    prepareReturnValue = sqlite3_prepare_v2(
        *database_,
        add_table_stmnt,
        -1,
        &ppStmt,
        &pzTail
    );
    if(prepareReturnValue == SQLITE_OK){
        std::cout << "Successfully prepared create " + name +"-table statement.\n";

        //execute & finalize statement
        stepReturnValue = sqlite3_step(ppStmt);
        if(stepReturnValue == SQLITE_DONE){
                std::cout << "Successfully processed create table statement.\n";
                finalizeReturnValue = sqlite3_finalize(ppStmt);
                if(finalizeReturnValue == SQLITE_OK){
                std::cout << "Successfully finalized create table statement.\n";
                return SQLITE_OK;
                }
                else{
                    return sqlite3_extended_errcode(*database_);
                }
        }
        else{
            return sqlite3_extended_errcode(*database_);
        }
    }
    else{
        return sqlite3_extended_errcode(*database_);
    }
}

/**
    * Adds a new Entry for a given Database Object into its given table by taking the Object's
    * insert statement, binding its values to it and processing the statement.
    * 
    * var   databaseObject: the object for which the entry in the database is added
    *
    * return:               SQLITE_OK if successful
    *                       errorcode else
*/


int Sqlitewrapper::addDatabaseEntry(Sqlitewrapper::DatabaseObject *databaseObject){

    std::string statementString = std::string(databaseObject->insertStatement());
    const char* statementPointer = statementString.c_str();

    sqlite3_stmt* insertStmt;
    if(generic_insert_stmt_ != NULL)
        insertStmt = generic_insert_stmt_;
    else{
        const char* pzTail;
        const char *insertStmntText = statementPointer;
        int prepareReturnValue = sqlite3_prepare_v2(
            *database_,
            insertStmntText,
            -1,
            &insertStmt,
            &pzTail
        );
        if(prepareReturnValue == SQLITE_OK)
            std::cout << "Successfully prepared Insert Statement\n";
        else
            std::cout << "Error preparing Insert Statement\n";            
    }

    /*
    * iterate through all dataBaseFields provided by the DatabaseObject and bind their values to the 
    * insertStatement. The first argument has index 1 (!!) and should not be touched if it is a 
    * unique id, since it will be set to max_id+1 automatically by the database if left unbound.
    */

    int bindReturnValue = SQLITE_OK;

    auto list = databaseObject->getDatabaseFields();
    uint32_t argumentCount = 2;
    for(DatabaseField field : list){
        switch(field.type){
            //TODO: implement other cases
            case ColumnType::INT:

            break;
            case ColumnType::FLOAT:

            break;
            case ColumnType::TEXT:
                bindReturnValue = sqlite3_bind_text(insertStmt, argumentCount, field.value.c_str(),-1,SQLITE_STATIC);
            break;
            case ColumnType::BLOB:

            break;
            default:
            std::cout << "Undefined ColumnType while trying to add new entry. Field value: " << field.value << "\n";
            break;
        }
        argumentCount++; 
        //std::cout << field.value << argumentCount-1 << " Status: " << bindReturnValue << "\n";
    }

    if(bindReturnValue == SQLITE_OK){
        std::cout << "Successfully bound values to insert statement.\n";
        //execute & finalize statement
        if(sqlite3_step(insertStmt) == SQLITE_DONE){
            std::cout << "Successfully processed insert statement.\n";
            //finalize statement if wrapper is in a mode that conserves memory
            if(isSlim_){
                if(sqlite3_finalize(insertStmt) == SQLITE_OK){
                    std::cout << "Successfully finalized insert statement.\n";
                    return SQLITE_OK;
                }
                else{
                    std::cout << "Statement could not be finalized.\n Error code " << sqlite3_extended_errcode(*database_) << ".\n";
                    return sqlite3_extended_errcode(*database_);
                }
            }
            //reset the statement, but don't finalize
            else{
                if(sqlite3_reset(insertStmt) == SQLITE_OK){
                    std::cout << "Successfully reset insert statement.\n";
                    return SQLITE_OK;
                }
                else{
                    std::cout << "Statement could not be reset.\n Error code " << sqlite3_extended_errcode(*database_) << ".\n";
                    return sqlite3_extended_errcode(*database_);
                }
            }
        }
        else{
            std::cout << "Statement to insert could not be processed.\n Error code " << sqlite3_extended_errcode(*database_) << ".\n";
            return sqlite3_extended_errcode(*database_);
        }
    }
    else{
            std::cout << "Error binding values to Insert statement.\n Error code " << sqlite3_extended_errcode(*database_) << ".\n";
            return sqlite3_extended_errcode(*database_);
    }
}

/**
    * Deletes a contact from the database by using a DELETE statement on the contact table. If a generic delete statement already exists it is rebound and used, otherwise a new delete statement is prepared.
    * 
    * var contact_Id:  the ID of the contact to be deleted
    * 
    * return:          SQLITE_OK if successful
    *                  errorcode else
    */
int Sqlitewrapper::deleteDatabaseEntry(std::string attributeName, DatabaseField field){
    
    const char* deleteStmntText = "DELETE FROM contacts WHERE @attributeName = @id";
    sqlite3_stmt* deleteStmt;
    
    if(generic_delete_stmt_ != NULL)
        deleteStmt = generic_delete_stmt_;
    else{
        const char* pzTail;        
        int prepareReturnValue = sqlite3_prepare_v2(
            *database_,
            deleteStmntText,
            -1,
            &deleteStmt,
            &pzTail
        );
        std::cout << prepareReturnValue << "\n";
    }

    const char* attribute = attributeName.c_str();
    int bindReturnValue = sqlite3_bind_text(deleteStmt, 1, attribute,-1,SQLITE_STATIC);
    
    switch(field.type){
        case ColumnType::INT:
            bindReturnValue = sqlite3_bind_int(deleteStmt, 2, std::stoi(field.value)); 
            break;
        default:
            bindReturnValue = sqlite3_bind_text(deleteStmt, 2, field.value.c_str(), -1, SQLITE_STATIC); 
    }
    
    if(bindReturnValue == SQLITE_OK){
        std::cout << "Successfully prepared delete statement.\n";

        //execute & finalize statement
        if(sqlite3_step(deleteStmt) == SQLITE_DONE){
            std::cout << "Successfully processed delete statement.\n";

            //finalize statement if wrapper is in a mode that conserves memory
            if(isSlim_){
                if(sqlite3_finalize(deleteStmt) == SQLITE_OK){
                    std::cout << "Successfully finalized delete statement.\n";
                    return SQLITE_OK;
                }
                else{
                    std::cout << "Statement could not be finalized.\n Error code " << sqlite3_extended_errcode(*database_) << ".\n";
                    return sqlite3_extended_errcode(*database_);
                }
            }
            //reset the statement, but don't finalize
            else{
                if(sqlite3_reset(deleteStmt) == SQLITE_OK){
                    std::cout << "Successfully reset delete statement.\n";
                    return SQLITE_OK;
                }
                else{
                    std::cout << "Statement could not be reset.\n Error code " << sqlite3_extended_errcode(*database_) << ".\n";
                    return sqlite3_extended_errcode(*database_);
                }

            }
        }
        else{
            std::cout << "Statement to delete could not be processed.\n Error code " << sqlite3_extended_errcode(*database_) << ".\n";
            return sqlite3_extended_errcode(*database_);
        }
    }
    else{
            std::cout << "Statement to delete could not be prepared.\n Error code " << sqlite3_extended_errcode(*database_) << ".\n";
            return sqlite3_extended_errcode(*database_);
    }
}

/**
    * Edits a contact from the database with updated data by calculating the difference of the entries and updating oudated data. 
    *
    * var contact:     the new contact information to be written
    * var contact_Id:  the ID of the contact to be deleted
    *
    * return:          SQLITE_OK if successful
    *                  errorcode else
    */
/* int Sqlitewrapper::editContact(Contact contact, int contactId){
    return 0;
} */


