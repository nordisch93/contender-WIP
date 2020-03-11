#include<string>
#include<iostream>
// uncomment to disable assert()
// #define NDEBUG
#include<cassert>
#include"contact.hpp"
//#include"json.h"

//using namespace Contact;

/**
 * This class stores a date of the gregorian calender in 4 Bytes
 */
bool Date::checkIfValidDate(uint32_t day, uint32_t month, uint32_t year){
    if(year < 0 || year > __UINT16_MAX__)
        return false;
    if(month < 1 || month > 12)
        return false;
    switch(month){
        //months with 31 days
        [[fallthrough]]case MONTH_JANUARY:
        [[fallthrough]]case MONTH_MARCH:
        [[fallthrough]]case MONTH_MAY:
        [[fallthrough]]case MONTH_JULY:
        [[fallthrough]]case MONTH_AUGUST:
        [[fallthrough]]case MONTH_OCTOBER:
                        case MONTH_DECEMBER:
            if(day < 1 || day > 31)
                return false;
            break;
        //months with 30 days
        [[fallthrough]]case MONTH_APRIL:
        [[fallthrough]]case MONTH_JUNE:
        [[fallthrough]]case MONTH_SEPTEMBER:
                        case MONTH_NOVEMBER:
            if(day < 1 || day > 30)
                return false;
            break;
                        case MONTH_FEBRUARY:
            if(year % 400 == 0 || (year % 4 == 0 || year % 100 != 0)){
                //Leapyear
                if(day < 1 || day > 29)
                return false;
            }
            else
            {
                //not a Leapyear
                if(day < 1 || day > 28)
                return false;
            }
    }
    return true;
}

Date::Date(uint32_t day, uint32_t month, uint32_t year){
    if(!checkIfValidDate(day, month, year))
        throw(new std::range_error("Called Date::Date(uint32_t, uint32_t, uint32_t) with invalid parameters"));
    else
    {
        day_ = day;
        month_ = month;
        year_ = year;
    }
};

void Date::setDate(uint32_t day, uint32_t month, uint32_t year){
    if(!checkIfValidDate(day, month, year))
        throw(new std::range_error("Called Date::setDate(uint32_t, uint32_t, uint32_t) with invalid parameters"));
    else
    {
        day_ = day;
        month_ = month;
        year_ = year;
    }
};

Contact::Contact(){ data_ = Json::Value();};
Contact::Contact(Sqlitewrapper::DatabaseTable table, Json::Value data){
    //contact data coming from the database
    if(data["fromDatabase"] == true){
        uint32_t count = 0;
        for(auto i = table.layout_.begin(); i != table.layout_.end(); i++, count++){
            if(data[std::to_string(count)].isDouble()){
                data_[i->name] = data[std::to_string(count)].asDouble();
                //std::cout << i->name << " was changed to " << data[std::to_string(count)].asDouble() << "\n";
            }
            else if(data[std::to_string(count)].isInt()){
                data_[i->name] = data[std::to_string(count)].asInt();
                //std::cout << i->name << " was changed to " << data[std::to_string(count)].asInt() << "\n";
            }
            else if(data[std::to_string(count)].isString()){
                data_[i->name] = data[std::to_string(count)].asCString();
                //std::cout << i->name << " was changed to " << data[std::to_string(count)].asCString() << "\n";
            }
            else if(data[std::to_string(count)].isBool()){
                if(data[std::to_string(count)].isConvertibleTo(Json::ValueType::stringValue)){
                    data_[i->name] = data[std::to_string(count)].asString();
                    //std::cout << i->name << " was changed to " << data[std::to_string(count)].asString() << "\n";
                }
                data_[i->name] = data[std::to_string(count)].asBool();
                //std::cout << i->name << " was changed to " << data[std::to_string(count)].asBool() << "\n";
            }
            else{
                std::cerr << "Runtime error in Contact::Contact(Json::Value data): data contains unknown datatype!";
                throw std::runtime_error("Runtime error in Contact::Contact(Json::Value data): data contains unknown datatype!");
            }
        }
    }
    //in this case the contact data is coming from the ui
    else{
        data_ = data;
        data_["fromDatabase"] = false;
    }
};
Contact::Contact(std::list<std::string> firstNames, std::list<std::string> lastNames){
        data_["fromDatabase"] = false;
        auto flattenList = [](std::list<std::string> listOfAllNames){
            std::string stringOfAllNames = "[";
            for(auto i = listOfAllNames.begin(); i != listOfAllNames.end(); ++i){
                if(i != listOfAllNames.begin())
                    stringOfAllNames.append(", ");
                stringOfAllNames.append(*i);                
            }
            return stringOfAllNames.append("]");
        };
        data_["first_name"] = flattenList(firstNames);
        data_["last_name"] = flattenList(lastNames);
        data_["email"] = "";
        data_["phone"] = "";
};

void Contact::printContact(){
    std::cout << "Contact Id: " << (data_["contact_id"].empty() ? -1 : data_["contact_id"].asInt()) << "\n";
    std::cout << "First Name: " << (data_["first_name"].empty() ? "empty" : std::string(data_["first_name"].asCString())) << "\n";
    std::cout << "Last Name: " << (data_["last_name"].empty() ? "empty" : std::string(data_["last_name"].asCString())) << "\n";
    std::cout << "Phone: " << (data_["phone"].empty() ? "empty" : std::string(data_["phone"].asCString())) << "\n";
    std::cout << "Email: " << (data_["email"].empty() ? "empty" : std::string(data_["email"].asCString())) << "\n";
}