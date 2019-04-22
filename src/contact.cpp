#include<string>
#include<iostream>
// uncomment to disable assert()
// #define NDEBUG
#include<cassert>


/**
 * This class stores a date of the gregorian calender in 4 Bytes
 */
class Date{
private:
    uint8_t day_;
    uint8_t month_;
    uint16_t year_;
    
    bool checkIfValidDate(uint32_t day, uint32_t month, uint32_t year){
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
    
public:
    Date(){
        day_ = 1;
        month_ = 1;
        year_ = 0;
    };
    Date(uint32_t day, uint32_t month, uint32_t year){
        if(!checkIfValidDate(day, month, year))
            throw(new std::range_error("Called Date::Date(uint32_t, uint32_t, uint32_t) with invalid parameters"));
        else
        {
            day_ = day;
            month_ = month;
            year_ = year;
        }
    };
    
    uint32_t getDay(){
        return (uint32_t) day_;
    };
    uint32_t getMonth(){
        return (uint32_t) month_;
    };
    uint32_t getYear(){
        return (uint32_t) year_;
    };
    
    void setDate(uint32_t day, uint32_t month, uint32_t year){
        if(!checkIfValidDate(day, month, year))
            throw(new std::range_error("Called Date::setDate(uint32_t, uint32_t, uint32_t) with invalid parameters"));
        else
        {
            day_ = day;
            month_ = month;
            year_ = year;
        }
    };
    
    enum MONTH : int {
        MONTH_JANUARY = 1, MONTH_FEBRUARY = 2, MONTH_MARCH = 3,
        MONTH_APRIL = 4, MONTH_MAY = 5, MONTH_JUNE = 6,
        MONTH_JULY = 7, MONTH_AUGUST = 8, MONTH_SEPTEMBER = 9,
        MONTH_OCTOBER = 10, MONTH_NOVEMBER = 11, MONTH_DECEMBER = 12};
};


class Contact{
    
private:
    std::string lastName_;
    std::string firstName_;
    Date birthday_;
public:
    Contact(){
        lastName_ ="";
        firstName_="";
    };
    Contact(std::string lastName, std::string firstName){
        lastName_ = lastName;
        firstName_ = firstName;
    };
    
    std::string printContact(){
        return firstName_.append(lastName_);
    };
    
    Date getBirthday(){
        return birthday_;
    }
    void setBirthday(Date birthday){
        birthday_ = birthday;
    }
};
