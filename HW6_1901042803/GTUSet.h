#ifndef GTUSET_H
#define GTUSET_H

#include <iostream>
#include <memory>

#include "iterables.h"
#include "GTUIterators.h"

using std::shared_ptr;
using std::initializer_list;
using std::move;


namespace GTU
{
    template <typename T>
    class GTUSet : public iterable<T, GTUIterator<T>, GTUIteratorConst<T>>
    {
        public:
        //Constructors
        /*Default Constructor*/
        GTUSet() : _data(nullptr), _size(0) {}

        /*Copy Constructor*/
        GTUSet(const GTUSet &);

        /*Move Constructor*/
        GTUSet(GTUSet &&);

        /*Constructor for Initializer Lists*/
        GTUSet(initializer_list<T> list) : GTUSet() 
            { for(auto source : list) add(source); }


        //Setter functions
        /*Insert the given object to the set*/
        void add(const T &);

        /*Insert the given object to the set*/
        void add(T &&);

        /*erase the element pointed by the given iterator*/
        void erase(const GTUIterator<T> &);

        /*erase the element pointed by the given iterator*/
        void erase(GTUIterator<T> &&);

        /*revert the set as if it used the default constructor*/
        void clear();


        //Getter functions
        /*Test if list is empty*/
        inline bool empty() const {
            if(size() == 0) return true; 
            else return false;
        }

        /*Return the number of elements in the set*/
        inline int size() const { return _size; } 
        
        /*Search the set for the given value and return 
         *a read/write iterator pointing to it
         */
        inline GTUIterator<T> search(const T & target) { 
            for(auto temp = begin(); temp != end(); ++temp) 
                if(*temp == target) return temp;

            return GTUIterator<T>();
        }

        /*Search the set for the given value and return 
         *a read-only iterator pointing to it
         */
        inline GTUIteratorConst<T> search(const T & target) const { 
            for(auto temp = cbegin(); temp != cend(); ++temp) 
                if(temp = target) return temp;

            return GTUIteratorConst<T>();
        }


        //iterators
        /*return a read/write iterator to the first element in the set*/
        GTUIterator<T> begin() const 
            { auto temp = GTUIterator<T>(_data[0]); return temp; }

        /*return a read/write iterator to the last element in the set*/
        GTUIterator<T> end() const 
            { return GTUIterator<T>(_data[_size]); }

        /*return a read-only iterator to the first element in the set*/
        GTUIteratorConst<T> cbegin() const 
            { return GTUIteratorConst<T>(_data[0]); }
        
        /*return a read-only iterator to the last element in the set*/
        GTUIteratorConst<T> cend() const 
            { return GTUIteratorConst<T>(_data[_size]); }


        //Overloaded operators
        /*Copy Assignment operator*/
        GTUSet & operator=(const GTUSet & );

        /*Move Assignment operator*/
        GTUSet & operator=(GTUSet &&);

        /*Returns the union set*/
        template <typename>
        friend GTUSet operator+(const GTUSet & lhs, const GTUSet &rhs);

        /*Returns the union set*/
        template <typename>
        friend GTUSet operator+(GTUSet && lhs, const GTUSet &rhs);

        /*Returns the intersection set*/
        GTUSet intersect(const GTUSet &);

        /*Returns the intersection set*/
        GTUSet intersect(GTUSet &&);


        private:
        shared_ptr<T[]> _data;

        //Number of elements in the set
        int _size;
    };


    template <typename T>
    GTUSet<T>::GTUSet(GTUSet<T> && source) : GTUSet()
    {
        _data = move(source._data); /*Move the data from the source set 
                                     *to the current one
                                     */

        _size = source._size; /*Change the size of the current set*/
        source.clear(); /*Clear the source set*/
    }

    template <typename T>
    GTUSet<T>::GTUSet(const GTUSet<T> & source) : GTUSet()
    {
        for (auto temp : source) add(temp); /*Add each element found in the
                                             *source set to the current one
                                             */
    }

    template <typename T>
    void GTUSet<T>::add(const T & target)
    {
        int counter, targetelem; 

        for(targetelem = 0; targetelem < _size; ++targetelem) /*Search the set for target*/
        {
            if(_data[targetelem] == target) return; /*if the target was found*/

            else if (_data[targetelem] > target) break; /*break if a value bigger 
                                                         *than the target was found*/
        }

        decltype(_data) newSet (new T[_size + 1]);    /*Create a temporary set*/

        for (counter = 0; counter < targetelem; ++counter)
            newSet[counter] = _data[counter]; /*Copy the elements from the current set 
                                               *to the new set with their exact order
                                               *till reacing the target's cell
                                               */

        for (; counter < _size; ++counter)
            newSet[counter + 1] = _data[counter]; /*Copy the rest of the elements from the 
                                                   *current set to the new set but pushing 
                                                   *there them 1 cell to the end of the set
                                                   *leaving the target's cell empty
                                                   */

        newSet[targetelem] = target; /*assign the value of the target*/

        _data = move(newSet); /*make the new set as the current set's property*/
        ++_size; /*increment the size*/
    }

    template <typename T>
    void GTUSet<T>::add(T && target)
    {
        int counter, targetelem; 

        for(targetelem = 0; targetelem < _size; ++targetelem) /*Search the set for target*/
        {
            if(_data[targetelem] == target) return; /*if the target was found*/

            else if (_data[targetelem] > target) break; /*break if a value bigger 
                                                         *than the target was found*/
        }

        decltype(_data) newSet (new T[_size + 1]);    /*Create a temporary set*/

        for (counter = 0; counter < targetelem; ++counter)
            newSet[counter] = _data[counter]; /*Copy the elements from the current set 
                                               *to the new set with their exact order
                                               *till reacing the target's cell
                                               */

        for (; counter < _size; ++counter)
            newSet[counter + 1] = _data[counter]; /*Copy the rest of the elements from the 
                                                   *current set to the new set but pushing 
                                                   *there them 1 cell to the end of the set
                                                   *leaving the target's cell empty
                                                   */

        newSet[targetelem] = move(target); /*assign the value of the target*/

        _data = move(newSet); /*make the new set as the current set's property*/
        ++_size; /*increment the size*/
    }

    template <typename T>
    void GTUSet<T>::erase(const GTUIterator<T> & target)
    {
        auto iterator = begin(); /*Initialize the search iterator 
                                  *to the start position*/

        for (; iterator != end() &&  /*Search for the given iterator*/
               iterator != target; 
               ++iterator);

        if(iterator != end()) /*If the target was found*/
        {
            ++iterator; /*Skip the target's index*/

            for (; iterator != end();  ++iterator) 
                *(iterator.getPrevious()) = *iterator; /*for each element after the target, 
                                                        *copy it to the elemsnt before it
                                                        */

            --_size; /*Decrement the current set's size*/

            auto tempSet = _data; /*Copy the current Set to a temporary one with the decremented size*/ 
            _data = move(tempSet); /*Make the temporary set as the current set's property*/
        }

        else return;
    }

    template <typename T>
    void GTUSet<T>::erase(GTUIterator<T> && target)
    {
        auto iterator = begin(); /*Initialize the search iterator 
                                  *to the start position*/

        for (; iterator != end() &&  /*Search for the given iterator*/
               iterator != target; 
               ++iterator);

        if(iterator != end()) /*If the target was found*/
        {
            ++iterator; /*Skip the target's index*/

            for (; iterator != end();  ++iterator) 
                *(iterator.getPrevious()) = *iterator; /*for each element after the target, 
                                                        *copy it to the elemsnt before it
                                                        */

            --_size; /*Decrement the current set's size*/

            auto tempSet = _data; /*Copy the current Set to a temporary one with the decremented size*/ 
            _data = move(tempSet); /*Make the temporary set as the current set's property*/
        }

        else return;
    }

    template <typename T>
    void GTUSet<T>::clear()
    {
        _size = 0;  /*reset the set size*/
        _data.reset();  /*Empty the data structure*/
    }

    template <typename T>
    GTUSet<T> & GTUSet<T>::operator=(const GTUSet<T> & rhs)
    {
        for (auto temp : rhs) add(temp); /*Add each element found in the 
                                          *source set to the current one*/
        return *this;
    }

    template <typename T>
    GTUSet<T> & GTUSet<T>::operator=(GTUSet<T> && rhs)
    {
        _size = rhs.size(); /*Change the size of the current set*/

        _data = move(rhs._data); /*Move the data from the right-hand-side 
                                   set to the current one*/

        rhs.clear();    /*Clear rhs*/

        return *this;
    }

    template <typename T>
    GTUSet<T> operator+(const GTUSet<T> & lhs, const GTUSet<T> & rhs)
    {
        auto temp = lhs;    /*Take a copy of lhs*/

        for (auto temp2 : rhs) 
            temp.add(temp2); /*add ever element in rhs to temp*/

        return temp; /*return the result set*/
    }
    
    template <typename T>
    GTUSet<T> operator+(GTUSet<T> && lhs, const GTUSet<T> & rhs)
    {
        for (auto temp : rhs)
            lhs.add(temp); /*add ever element in rhs to temp*/

        return lhs;
    }

    template <typename T>
    GTUSet<T> GTUSet<T>::intersect(const GTUSet<T> & rhs)
    {
        GTUSet<T> tempSet;

        for (auto temp : rhs)  
            tempSet.add(*search(temp)); /*Add every element that 
                                         *exists in both sets
                                         */
        return tempSet; /*return the result set*/
    }

    template <typename T>
    GTUSet<T> GTUSet<T>::intersect(GTUSet<T> && rhs)
    {
        GTUSet<T> tempSet;

        for (auto temp : rhs)
            tempSet.add(*search(temp)); /*Add every element that 
                                         *exists in both sets
                                         */
        rhs.clear(); /*Clear the rhs*/
        return tempSet; /*return the result set*/
    }
}

#endif