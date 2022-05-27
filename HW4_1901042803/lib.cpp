#include <iostream>
#include <vector>
#include <new>
#include <iomanip>
#include <string>
#include <fstream>
#include <cstdlib>
#include "header.h"

using std::vector;
using std::ostream;
using std::string;
using std::endl;
using std::setfill;
using std::setw;
using std::cerr;
using std::cout;
using std::getline;

namespace DayofYearNS
{
    /*Initialize the number of live members*/
    int DayofYearSet::liveDates = 0;

    /*Initialize the list of number of days in each month*/
    int DayofYearSet::DayofYear::_daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


    /*DayofYear class functions*/

    DayofYearSet::DayofYear::DayofYear(int day, int month)
    {
        if (checkDate(day, month))  /*Check parameters first*/
        {
            _day = day; 
            _month = month;
        }

        else exit(0);
    }


    bool DayofYearSet::DayofYear::setDate(const unsigned int day, const unsigned int month) 
    { 
        if (checkDate(day, month))  /*Check parameters first*/
        {
            _day = day; 
            _month = month;
            return true;
        }

        else return false;
    }


    /*DayofYearSet class functions*/


    DayofYearSet::DayofYearSet(const DayofYearSet & newSet)
    {
        _set = 0;       /*Initialize the set pointer*/
        _setSize = 0;   /*and it's size*/

        for (int i = 0; i < newSet.size(); ++i)
            add(newSet[i]); /*Add each member iteratively to the current set*/
        
        _setSize = newSet.size();   /*Set the size of the current set*/
    }


    DayofYearSet::DayofYearSet(const vector<DayofYear> newList)
    {
        _set = 0;       /*Initialize the set pointer*/
        _setSize = 0;   /*and it's size*/

        for (auto listMember : newList)
            add(listMember);    /*Add each member iteratively to the current set*/

        _setSize = newList.size();   /*Set the size of the current set*/
    }


    DayofYearSet::~DayofYearSet()
    {
        subtractLiveDates(size());  /*Subtract the size of the current set from the total number of members*/
        delete [] _set; /*Free the set pointers*/
    }


    ostream & operator<<(ostream & outStream, const DayofYearSet & set)
    {
        outStream << "\nList of dates:\n";

        for (int i = 0; i < set.size(); ++i)
            outStream   << setfill('0') << setw(2) << set[i].getDay() << "    " 
                        << setfill('0') << setw(2) << set[i].getMonth() << endl;

        outStream << endl;

        return outStream;
    }


    ofstream & operator<<(ofstream & outStream, const DayofYearSet & set)
    {
        outStream << "\nList of dates:\n";

        for (int i = 0; i < set.size(); ++i)
            outStream   << setfill('0') << setw(2) << set[i].getDay() << "    " 
                        << setfill('0') << setw(2) << set[i].getMonth() << endl;

        outStream << endl;

        return outStream;
    }


    bool DayofYearSet::operator==(DayofYearSet const & rightSet) const
    {
        if (rightSet.size() != size())  return false;   /*If the sizes are different, then they're definitely inequal*/
        
        int setSize = size();

        for (int i = 0; i < setSize; ++i)
            if (findinSet(rightSet[i]) == -1 || rightSet.findinSet((*this)[i]) == -1)  return false; /*if any member of a set was not in the other set, return false*/

        return true;    /*All members of each set exists in the other set*/
    }


    bool DayofYearSet::operator!=(DayofYearSet const & rightSet) const
    {
        if (rightSet.size() != size())  return true;    /*If the sizes are different, then they're definitely inequal*/
        
        int setSize = size();

        for (int i = 0; i < setSize; ++i)
            if (findinSet(rightSet[i]) == -1 || rightSet.findinSet((*this)[i]) == -1)  return true; /*if any member of a set was not in the other set, return true*/

        return false ;    /*All members of each set exists in the other set*/
    }


    DayofYearSet DayofYearSet::operator+(DayofYearSet const & rightSet) const
    {
        DayofYearSet newSet;
        
        for (int i = 0; i < size(); ++i)
            newSet.add((*this)[i]); /*Add the elements of the left set*/

        for (int i = 0; i < rightSet.size(); ++i)
            newSet.add(rightSet[i]); /*Add the elements of the right set*/

        return newSet;
    }


    DayofYearSet DayofYearSet::operator-(DayofYearSet const & rightSet) const
    {
        DayofYearSet newSet = *this;    /*Copy the left set to the result set*/

        for (int i = 0; i < newSet.size(); ++i)
            if (rightSet.findinSet(newSet[i]))  newSet.remove(i);   /*If a member of the right set exists in the result, remove it from the result*/

        return newSet;  
    }


    DayofYearSet DayofYearSet::operator^(DayofYearSet const & rightSet) const
    {
        DayofYearSet newSet;

        for (int i = 0; i < rightSet.size(); ++i)
            if (findinSet(rightSet[i]) != -1)   newSet.add(rightSet[i]); /*Add every mutual member to the result*/

        return newSet;
    }


    DayofYearSet DayofYearSet::operator!() const
    {
        DayofYearSet complementSet;

        /*This loop iteratively goes through each day in a year, if the day exists in the base set, don't add it to the result*/
        for (int month = 1; DayofYear::checkDate(1, month); ++month)
            for (int day = 1; DayofYear::checkDate(day, month); ++day)
                if (findinSet(DayofYear(day, month)) == -1) complementSet.add(DayofYear(day, month));

        return complementSet;
    }


    void DayofYearSet::add(const DayofYear & newObj)
    {
        int i;
        DayofYear *tempArr;

        if (findinSet(newObj) != -1)   return;  /*If the object existed in the target set, then abort function (NO DUPLICATES ARE ALLOWED)*/

        tempArr = new DayofYear[_setSize+1];    /*Dynamically allocate the new set with one extra slot*/

        for (i = 0; i < _setSize; ++i)
            tempArr[i] = (*this)[i];    /*Copy the old set to the temporary one*/

        tempArr[i] = newObj;    /*Add the new object*/
        
        if (_set != 0)  delete [] _set; /*Delete the old object*/

        _set = tempArr; /*Assign the temporary se to the old one*/
        ++_setSize; /*Increase the size of the old set*/

        addLiveDates(1);    /*Increase the total number of live set members*/
    }


    void DayofYearSet::add(int day, int month)
    {
        if(DayofYear::checkDate(day, month))    add(DayofYear(day, month));     /*Check that the parameters are valid first*/
    }


    void DayofYearSet::remove(int targetNum)
    {
        DayofYear *tempArr;

        if (targetNum > _setSize || _setSize == 0)   return;    /*If the target index is of the set or if the set is empty, abort*/

        tempArr = new DayofYear[_setSize - 1];  /*Dynamically allocate the temporary set with one slot less*/

        for (int i = 0, j = 0; (i < _setSize); ++i, ++j)    /*i is the index of the old set, j is the index fro the new one*/
        {
            if (targetNum - 1 != i) tempArr[j] = (*this)[i];    /*Copy the member to the temporary set if it wasn't the target*/
            else --j;       /*If the target was found, skip it and decrement the temporary set index to prevent skipping cells*/
        }
        
        delete [] _set; /*Free the old set*/

        _set = tempArr; /*Assign the temporary set to the old one*/
        --_setSize; /*Decrement the size of the old set*/

        subtractLiveDates(1);
    }


    int DayofYearSet::findinSet(DayofYearSet::DayofYear target) const
    {
        for (int i = 0; i < size(); ++i)
            if ((*this)[i] == target)   return i;
        return -1;
    }


    void DayofYearSet::operator<<(const string fileName)
    {
        ifstream inFile;
        string buffer;
        int day, month;

        inFile.open (fileName, ifstream::in);   /*Open the file*/

        if (inFile.is_open() == false)  /*Check that the file is open*/
        {
            cerr << "Couldn't open " << fileName << endl;
            exit(2);
        }

        while (inFile.peek() != EOF)    /*Keep reading lines till reaching end-of-file*/
        {
            getline(inFile, buffer);

            day = (buffer[0] - '0')*10 + (buffer[1] - '0');
            month = (buffer[3] - '0')*10 + (buffer[4] - '0');

            if (DayofYear::checkDate(day, month))   add(DayofYear(day, month)); /*Check the parameters first*/
        }
    }


    DayofYearSet::DayofYear & DayofYearSet::operator[](unsigned int index) const
    {
        if (index < size()) return _set[index];
        else exit(4);
    }
}