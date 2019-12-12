#include<iostream>
#include<string>
#include<cstring>
#include<stdexcept>
#include<sstream>
#include"sqlite3.h"

#include"contact.hpp"
#include"sqlitewrapper.hpp"

int main(int argc, char* argv[]){
    
    const char* pathToDb;
    if(argc < 2)
        pathToDb = "MyDatabase.sql";
    else
        pathToDb = argv[1];
    
    bool dbIsOpen = false;
    sqlite3* database;
    
    Sqlitewrapper sqlw = Sqlitewrapper(&database, false);
    
    
    if(sqlw.openDb(pathToDb) == SQLITE_OK)
    {
        dbIsOpen = true;
        int stepReturnValue;
        int finalizeReturnValue;
        int prepareReturnValue;

        std::cout << "Successfully opened database.\n";
        
        Contact c = Contact("peter", "silie");
        
        std::string tableName = "contacts";
        std::string arguments = "contact_id INTEGER PRIMARY KEY, first_name TEXT NOT NULL, last_name TEXT, email text, phone text";
        
        int temp = sqlw.createTable(tableName, arguments);
        
        if(temp == SQLITE_OK){
        
            temp = sqlw.addDatabaseEntry(&c);
            if(temp == SQLITE_OK){
                //evereything fine
            }
            else{
                //couldnt add contact
            }
        }
        else{
            //couldnt create table
        }
        
        //close database
        if(dbIsOpen){
            if(sqlw.closeDb() == SQLITE_OK)
                std::cout << "Successfully closed database.\n";
            else
                std::cout << "Error closing database.\n";
            }
        std::cout << "Exiting. \n";
        return 0;
    }
    else{
        std::cout << "Error opening database.\n Error Code: " << sqlite3_extended_errcode(database) << ".\nTrying to create new database..\n";
        if(sqlw.createDb(pathToDb) == SQLITE_OK)
            std::cout << "Successfully created new db.\n";
        else
            std::cout << "Error creating database.\n Exiting.\n";
        return 0;        
    }
}
