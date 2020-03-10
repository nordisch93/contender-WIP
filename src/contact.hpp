#include<string>
#include"sqlitewrapper.hpp"

#ifndef CONTACT_HPP
#define CONTACT_HPP

class Date{
    private:
        uint8_t day_;
        uint8_t month_;
        uint16_t year_;
        
        bool checkIfValidDate(uint32_t day, uint32_t month, uint32_t year);
        
    public:
        inline Date(){
            day_ = 1;
            month_ = 1;
            year_ = 0;
        };
        
        Date(uint32_t day, uint32_t month, uint32_t year);
        
        inline uint32_t getDay(){
            return (uint32_t) day_;
        };
        inline uint32_t getMonth(){
            return (uint32_t) month_;
        };
        inline uint32_t getYear(){
            return (uint32_t) year_;
        };
        
        void setDate(uint32_t day, uint32_t month, uint32_t year);
        
        enum MONTH : int {
            MONTH_JANUARY = 1, MONTH_FEBRUARY = 2, MONTH_MARCH = 3,
            MONTH_APRIL = 4, MONTH_MAY = 5, MONTH_JUNE = 6,
            MONTH_JULY = 7, MONTH_AUGUST = 8, MONTH_SEPTEMBER = 9,
            MONTH_OCTOBER = 10, MONTH_NOVEMBER = 11, MONTH_DECEMBER = 12};
    };
    
    
class Contact : public Sqlitewrapper::DatabaseObject{
private:
    Json::Value data_;
    inline static const std::list<Sqlitewrapper::ColumnAttributes> layout_ =
        {Sqlitewrapper::ColumnAttributes(Sqlitewrapper::ColumnType::INT, "id"),
        Sqlitewrapper::ColumnAttributes(Sqlitewrapper::ColumnType::TEXT, "firstNames"),
        Sqlitewrapper::ColumnAttributes(Sqlitewrapper::ColumnType::TEXT, "lastNames") };
public:
    Contact();
    Contact(Json::Value);
    Contact(std::list<std::string> firstNames, std::list<std::string> lastNames);
    void printContact();

    std::string getInsertStatement();
    std::string getDeleteStatement();
    Json::Value getData(){return data_;}
    std::list<Sqlitewrapper::ColumnAttributes> getLayout(){return layout_;}
};

#endif