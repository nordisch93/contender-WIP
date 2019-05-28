#include<string>
#include<iostream>
// uncomment to disable assert()
// #define NDEBUG
#include<cassert>
#include"contact.hpp"

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
            //motnsh with 31 days
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

Contact::Contact(){
    lastName_ ="";
    firstName_="";
}
Contact::Contact(std::string lastName, std::string firstName){
    lastName_ = lastName;
    firstName_ = firstName;
}

std::string Contact::printContact(){
    return firstName_.append(lastName_);
}
