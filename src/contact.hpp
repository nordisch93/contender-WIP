#include<string>

#ifndef CONTACT_HPP
#define CONTACT_HPP

class DatabaseObject{
public:
    virtual ~DatabaseObject(){}
    virtual std::string insertStatement() = 0;
    virtual uint32_t getArgumentCount() = 0;
    virtual std::string getArgumentString() = 0;
};

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
    
    
class Contact : public DatabaseObject{
private:
    std::string lastName_;
    std::string firstName_;
    std::string phone_;
    std::string email_;
    Date birthday_;
public:
    //~Contact(){};
    Contact();
    Contact(std::string, std::string);
    std::string printContact();

    virtual std::string insertStatement();
    virtual uint32_t getArgumentCount() {return 5;}
    virtual std::string getArgumentString(){
        return (std::string("IGNORE") + "%" + firstName_ + "%" + lastName_ + "%" + email_ + "%" + phone_);
    }

    inline std::string getFirstName(){  return firstName_;}
    inline std::string getLastName(){  return lastName_;}
    inline std::string getEmail(){  return email_;}
    inline std::string getPhone(){  return phone_;}
    inline Date getBirthday(){  return birthday_;}
    inline void setBirthday(Date birthday){
        birthday_ = birthday;
    }
};

#endif
