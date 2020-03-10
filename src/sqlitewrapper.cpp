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
    * TODO: the following functionality is not available yet
    * var isSlim:          if true, statements will only be prepared when needed
    *                      if false, statements will be prepared immediatly
    *
    */
Sqlitewrapper::Sqlitewrapper(sqlite3** database, bool isSlim){
    database_ = database;
    return;
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
     * var name:        the name of the table to be created
     * var columns:     the columns of the table to be created (name, type, and optionally modifiers)
     *
     * return:          SQLITE_OK if successful
     *                  extended errorcode else
     */

int Sqlitewrapper::createTable(std::string name, std::list<ColumnAttributes> columns){
    int stepReturnValue;
    int finalizeReturnValue;
    int prepareReturnValue;

    //Assemble the argumentsstring from the columns
    std::string arguments;
    for(ColumnAttributes c : columns){
        arguments.append(c.name);
        switch(c.type){
            case ColumnType::BLOB:
            arguments.append(" BLOB");
            break;
            case ColumnType::DOUBLE:
            arguments.append(" REAL");
            break;
            case ColumnType::INT:
            arguments.append(" INTEGER");
            break;
            case ColumnType::TEXT:
            arguments.append(" TEXT");
            break;
        }
        for(int con : c.constraints){
            switch(con){
                case SQLITE_CONSTRAINT_NOTNULL:
                arguments.append(" NOT NULL");
                break;
                case SQLITE_CONSTRAINT_PRIMARYKEY:
                arguments.append(" PRIMARY KEY");
                break;
                default:
                break;
            }
        }
        arguments.append(", ");
    }
    arguments.erase(arguments.length()-1);
    arguments.erase(arguments.length()-1);
    std::cout << arguments;


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

    std::string statementString = std::string(databaseObject->getInsertStatement());
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
    * getInsertStatement. The first argument has index 1 (!!) and should not be touched if it is a 
    * unique id, since it will be set to max_id+1 automatically by the database if left unbound.
    */

    int bindReturnValue = SQLITE_OK;

    auto layout = databaseObject->getLayout();
    auto data = databaseObject->getData();
    auto it = layout.begin();
    /* argumentCount keeps track of the position of the variable to be bound within the statement.
    Has to start at 1 since the leftmost field in prepare statement (usually id) has index 1
    */
    uint32_t argumentCount = 1;
    for(; it != layout.end(); it++, argumentCount++){
        switch(it->type){
            case ColumnType::INT:
                if(data[it->name.c_str()] != NULL){
                    bindReturnValue += sqlite3_bind_int(insertStmt, argumentCount, data[it->name.c_str()].asInt());
                }
            break;
            case ColumnType::DOUBLE:
                if(data[it->name.c_str()] != NULL){
                    bindReturnValue += sqlite3_bind_double(insertStmt, argumentCount, data[it->name.c_str()].asDouble());
                }
            break;
            case ColumnType::TEXT:
                bindReturnValue += sqlite3_bind_text(insertStmt, argumentCount, data[it->name.c_str()].asCString(),-1,SQLITE_STATIC);
            break;
            case ColumnType::BLOB:
            //TODO: implement BLOB cases
            std::cout << "we somehow slipped into blob case when binding variables to prepared statement.";
            break;
            default:
            std::cout << "Undefined ColumnType while trying to add new entry. Field value: " << data[it->name.c_str()].asCString() << "\n Field Type: " << it->name.c_str() << "\n";
            break;
        }
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
    * Deletes a contact from the database by using a DELETE statement on the contact table. If a generic delete statement already exists it is rebound and used,
    * otherwise a new delete statement is prepared.
    * 
    * var databaseObject:  the databaseObject to be deleted
    * 
    * return:              SQLITE_OK if successful
    *                      errorcode else
    */
int Sqlitewrapper::deleteDatabaseEntry(Sqlitewrapper::DatabaseObject *databaseObject){
    
    if(databaseObject->getData()["id"] == NULL){
        std::cout << "trying to delete an object not present in the database\n";
        return -1;
    }

    std::string deleteStatementString = std::string(databaseObject->getDeleteStatement());
    const char* deleteStatementPointer = deleteStatementString.c_str();
    sqlite3_stmt* deleteStmt;
    
    if(generic_delete_stmt_ != NULL)
        deleteStmt = generic_delete_stmt_;
    else{
        const char* pzTail;        
        int prepareReturnValue = sqlite3_prepare_v2(
            *database_,
            deleteStatementPointer,
            -1,
            &deleteStmt,
            &pzTail
        );
        //std::cout << prepareReturnValue << "\n";
    }

    int databaseId = databaseObject->getDatabaseId();
    int bindReturnValue = bindReturnValue = sqlite3_bind_int(deleteStmt, 1, databaseId);

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
 * Carries out a Select operation on the database and writes a list of all retreived db-entries to
 * the given pointer.
 * 
 * var destination:                     the pointer where the list is written to
 * var selectStatement:                 the statement to be used to select db-entries
 * 
 * return: SQLITE_OK            if successful
 *         errorcode            else
 */

int Sqlitewrapper::selectDatabaseObjects(std::list<Json::Value>* destination, std::string selectStatement){
    std::cout << "working on it\n";

    int stepReturnValue;
    int finalizeReturnValue;
    int prepareReturnValue;

    sqlite3_stmt* selectStmt;
    const char* pzTail;
    const char* selectStmntText = selectStatement.c_str();
    prepareReturnValue = sqlite3_prepare_v2(
        *database_,
        selectStmntText,
        -1,
        &selectStmt,
        &pzTail
    );
    if(prepareReturnValue == SQLITE_OK){
        std::cout << "Successfully prepared Select Statement\n";
        //Retrieve data from all rows that satisfy the query's requirements and create a Json-Object for each of them.
        int stepReturnValue = sqlite3_step(selectStmt);
        while(stepReturnValue == SQLITE_ROW){
            //std::cout << "step successful \n";
            
            //Build a Json from the retrieved row
            Json::Value currentRow = Json::Value();
            currentRow["fromDatabase"] = true;
            for(int i = 0; i < sqlite3_column_count(selectStmt); i++){
                //std::cout << i;
                switch(sqlite3_column_type(selectStmt, i)){
                    case SQLITE3_TEXT:
                    //sqlite3_column_text returns a const UNSIGNED char* which will be interpreted as bool by jsoncpp unless cast to const char*
                    currentRow[std::to_string(i)] = reinterpret_cast<const char*>(sqlite3_column_text(selectStmt, i));
                    
                    //std::cout << "read " << sqlite3_column_text(selectStmt, i) << "from the database and placed at"<< std::to_string(i) << "\n";
                    //std::cout << "value type of " << std::to_string(i) << " is " << currentRow[std::to_string(i)].type() << "\n";
                    //std::cout << "wrote " << currentRow[std::to_string(i)].asString() << " to JSON\n";
                    break;
                    case SQLITE_FLOAT:
                    currentRow[std::to_string(i)] = sqlite3_column_double(selectStmt, i);
                    break;
                    case SQLITE_INTEGER:
                    currentRow[std::to_string(i)] = sqlite3_column_int(selectStmt, i);
                    break;
                    case SQLITE_BLOB:
                    currentRow[std::to_string(i)] = sqlite3_column_blob(selectStmt, i);
                    break;
                    case SQLITE_NULL:
                    currentRow[std::to_string(i)] = NULL;
                    break;
                    default:
                    std::cout << "unknown column type returned by step.\n";
                    break;
                }
            }
            destination->push_back(currentRow);
            stepReturnValue = sqlite3_step(selectStmt);
        }
        if(stepReturnValue == SQLITE_DONE){
            std::cout << "step finished \n";
        }
        else{
            std::cout << "sqlite3_step() return with errorcode " << sqlite3_extended_errcode(*database_) << "\n";
            return 1;
        }
    }        
    else{
            std::cout << "Error preparing Select Statement " << sqlite3_extended_errcode(*database_) << "\n";
            return 1;
    }        
}