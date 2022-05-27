#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using std::vector;
using std::ostream;
using std::ifstream;
using std::ofstream;
using std::string;
using std::cout;

namespace DayofYearNS
{
    class DayofYearSet{
        public:

        /*Inerclasses*/
        class DayofYear{/*THIS CLASS HAS THE BIG THREE*/
            public:
            /*Constructors*/

            /*Default constructor*/
            DayofYear() : _day(1), _month(1){}

            /*Constructor with first parameter as day, second as month*/
            DayofYear(int, int);

            /*Copy constructor*/
            DayofYear(const DayofYear & baseDay) : _day(baseDay.getDay()), _month(baseDay.getMonth()) {}


            /*Setters*/

            /*Sets the day and month of a DayofYear object. Returns true on success, false if input is invalid*/
            bool setDate(const unsigned int, const unsigned int);


            /*Getters*/

            /*return day*/
            unsigned int getDay() const { return _day; }

            /*return month*/
            unsigned int getMonth() const { return _month; }


            /*Overloaded operators*/

            /*Overloaded equality operator*/
            inline bool operator==(DayofYear const & rightObj) const
            {
                if (_day != rightObj.getDay() || _month != rightObj.getMonth())   return false;
                else return true;
            }

            /*Overloaded inequality operator*/
            inline bool operator!=(DayofYear const & rightObj) const
            {
                if (_day != rightObj.getDay() || _month != rightObj.getMonth())   return true;
                else return false;
            }

            /*Overloaded assignment operator*/
            void operator=(DayofYear const & rightObj)
            {
                _day = rightObj.getDay();
                _month = rightObj.getMonth();
            }


            /*Other functions*/

            /*Checks if the date parameters are valid*/
            static inline bool checkDate(const unsigned int day, const unsigned int month)
            {
                if (month > 12 || day == 0 || day > _daysInMonth[month-1])   return false;
                else return true;
            }

            private:
            /*Day and month data*/
            unsigned int _day, _month;

            /*Stores the number of days in each month*/
            static int _daysInMonth[12];
        };
        

        /*Constructors*/

        /*Default constructor of DayofYearSet class*/
        DayofYearSet() : _set(0), _setSize(0) {}
        
        /*Constructor of DayofYearSet class that takes the list of days as a vector*/
        DayofYearSet(const vector<DayofYear>);

        /*Copy constructor of DayofYearSet class*/
        DayofYearSet(const DayofYearSet &);

        /*Destructor of DayofYearSet class*/
        ~DayofYearSet();


        /*Overloaded operators*/

        /*Overloaded stream insertion operator*/
        friend ostream & operator<<(ostream &, const DayofYearSet &);

        /*Overloaded file stream insertion operator*/
        friend ofstream & operator<<(ofstream &, const DayofYearSet &);

        /*Overloaded stream insertion operator for taking the data of a DayofYearSet from a file. Takes the name of the input file*/
        void operator<<(const string);

        /*Overloade equality functions*/
        bool operator==(DayofYearSet const &) const;

        /*Overloade inequality functions*/
        bool operator!=(DayofYearSet const &) const;

        /*Overloaded addition operator to return the union set*/
        DayofYearSet operator+(DayofYearSet const &) const;
        
        /*Overloaded subtraction operator to return the difference set*/
        DayofYearSet operator-(DayofYearSet const &) const;

        /*Overloaded bitwise operator to return the intersection set*/
        DayofYearSet operator^(DayofYearSet const &) const;

        /*Overloaded NOT operator to return the complement from 365 days set*/
        DayofYearSet operator!() const;

        /*Overloaded index operator to return an elemnt in the set*/
        DayofYear & operator[](unsigned int index) const;
        

        /*Other functions*/

        /*Add an element to the set*/
        void add(const DayofYear &);

        /*Add an element to the set using it's parameters*/
        void add(int, int);
    
        /*Remove the element with this index from the list*/
        void remove(int);

        /*Return number of live DayofYear objects*/
        inline static int numofLiveDates() { return liveDates; }
        
        /*Return the number of members in the set*/
        inline int size() const { return _setSize; }

        /*Find a member that matches the target and return it's index*/
        int findinSet(DayofYear) const;

        private:
        /*List of members*/
        DayofYear *_set;

        /*Number of members in the list*/
        int _setSize;

        /*Total number of live members in all living objects*/
        static int liveDates;

        /*Add num to the liveDates*/
        inline static void addLiveDates(unsigned int num) { liveDates += num; }

        /*Subtract num from the liveDates*/
        inline static void subtractLiveDates(unsigned int num) { liveDates -= num; }
    };

    /*Overloaded stream insertion operator*/
    ostream & operator<<(ostream &, const DayofYearSet &);

    /*Overloaded file stream insertion operator*/
    ofstream & operator<<(ofstream &, const DayofYearSet &);
}

#endif