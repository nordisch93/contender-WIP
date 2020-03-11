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
        
        auto list = std::list<Contact>();

        list.push_back(Contact({std::string("peter"), std::string("alfred"), std::string("m.")}, {std::string("silie")}));
        list.push_back(Contact({std::string("Mike")}, {std::string("Hunt")}));
        list.push_back(Contact({std::string("Mike")}, {std::string("Litoris")}));
        list.push_back(Contact({std::string("Ben")}, {std::string("Dover")}));
        list.push_back(Contact({std::string("Moe")}, {std::string("Lester")}));


        std::string tableName = "contacts";

        auto attrList= std::list<Sqlitewrapper::ColumnAttributes>();
        auto constraintList = std::list<int>();
        constraintList.push_back(SQLITE_CONSTRAINT_PRIMARYKEY);
        attrList.push_back(Sqlitewrapper::ColumnAttributes(Sqlitewrapper::ColumnType::INT, std::string("contact_id"), constraintList));
        constraintList.pop_back();
        constraintList.push_back(SQLITE_CONSTRAINT_NOTNULL);
        attrList.push_back(Sqlitewrapper::ColumnAttributes(Sqlitewrapper::ColumnType::TEXT, std::string("first_name"), constraintList));
        attrList.push_back(Sqlitewrapper::ColumnAttributes(Sqlitewrapper::ColumnType::TEXT, std::string("last_name")));
        attrList.push_back(Sqlitewrapper::ColumnAttributes(Sqlitewrapper::ColumnType::TEXT, std::string("email")));
        attrList.push_back(Sqlitewrapper::ColumnAttributes(Sqlitewrapper::ColumnType::TEXT, std::string("phone")));

        Sqlitewrapper::DatabaseTable contactTable = Sqlitewrapper::DatabaseTable(std::string("contacts"), attrList, 0);

        int temp = sqlw.createTable(contactTable);
        
        if(temp == SQLITE_OK){
            for(Contact c : list){
                temp = sqlw.addDatabaseEntry(contactTable, &c);
                if(temp == SQLITE_OK){
                    //everything fine
                    std::cout << "Added " << c.getData()["first_name"] << " to the database.\n \n";
                }
                else{
                    //couldnt add contact
                }
            }
        }
        else{
            //couldnt create table
        }

        auto destination = new std::list<Json::Value>;
        sqlw.selectDatabaseObjects(destination, std::string("SELECT * FROM contacts;"));

        auto list2 = std::list<Contact>();

        for(Json::Value d : *destination){
            Contact c = Contact(contactTable, d);
            list2.push_back(c);
        }

        for(Contact c : list2){
            c.printContact();
        }
        
        //delete an entry
        Contact d = list2.front();
        sqlw.deleteDatabaseEntry(contactTable, &d);

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
