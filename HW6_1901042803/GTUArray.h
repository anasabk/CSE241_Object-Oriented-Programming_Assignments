#ifndef GTUARRAY_H
#define GTUARRAY_H

#include <iostream>
#include <memory>

#include "iterables.h"
#include "GTUIterators.h"

using std::shared_ptr;
using std::initializer_list;
using std::move;


namespace GTU
{
    template <typename T, int SIZE>
    class GTUArray : public iterable<T, GTUIterator<T>, GTUIteratorConst<T>>
    {
        public:
        //Constructors
        /*Default Constructor*/
        GTUArray() : _data(shared_ptr<T[]>(new T[SIZE])) 
            { fill(T()); }

        /*Copy Constructor*/
        GTUArray(const GTUArray &);
        
        /*Move Constructor*/
        GTUArray(GTUArray && source) : _data(move(source._data)) 
            { source = GTUArray(); }
        
        /*Constructor for Initializer Lists*/
        GTUArray(const initializer_list<T>);


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

        /*Fill the array with the given value*/
        void fill(T val) { for (auto temp = begin(); 
                                temp != end(); 
                                ++temp) 
                                *temp = val; }


        //Getter functions
        /*Test if list is empty*/
        inline bool empty() const {
            if(size() == 0) return true; 
            else return false;
        }

        /*Return the number of elements in the set*/
        inline int size() const { return SIZE; } 


        //iterators
        /*return a read/write iterator to the first element in the set*/
        GTUIterator<T> begin() const 
            { auto temp = GTUIterator<T>(_data[0]); return temp; }

        /*return a read/write iterator to the last element in the set*/
        GTUIterator<T> end() const 
            { return GTUIterator<T>(_data[SIZE]); }

        /*return a read-only iterator to the first element in the set*/
        GTUIteratorConst<T> cbegin() const 
            { return GTUIteratorConst<T>(_data[0]); }
        
        /*return a read-only iterator to the last element in the set*/
        GTUIteratorConst<T> cend() const 
            { return GTUIteratorConst<T>(_data[SIZE]); }
        

        //Overloaded operators
        /*Copy Assignment operator*/
        GTUArray & operator=(const GTUArray & array);

        /*Move Assignment operator*/
        GTUArray & operator=(GTUArray && array);

        /*Index operator*/
        T & operator[](int index) 
            { return _data[index]; }

        /*Const-Index operator*/
        const T & operator[](int index) const 
            { return _data[index]; } 


        private:
        shared_ptr<T[]> _data;
    };


    template <typename T, int SIZE>
    GTUArray<T, SIZE>::GTUArray(const GTUArray<T, SIZE> & source) 
        : _data(shared_ptr<T[]>(new T[SIZE])) /*Initialize the array with 
                                               *the default constructor*/
    {
        for(int i = 0; i < SIZE; ++i)
            (*this)[i] = source[i]; /*Copy the elements to the 
                                     *current array one by one
                                     */
    }

    template <typename T, int SIZE>
    GTUArray<T, SIZE>::GTUArray(initializer_list<T> list) 
        : _data(shared_ptr<T[]>(new T[SIZE]))
    {
        for(auto source = list.begin(), base = begin(); 
            (source != list.end()) && (base != end()); 
            ++source, ++base)
            *base = *source;    /*Copy every element in the initializer 
                                 *list to the current array*/
    }

    template <typename T, int SIZE>
    GTUArray<T, SIZE> & GTUArray<T, SIZE>::operator=(GTUArray<T, SIZE> && rhs)
    {
        if (this != &rhs)
            this->_data = move(rhs._data); /*Make rhs's data the property 
                                            *of the current array*/
        return *this;
    }

    template <typename T, int SIZE>
    GTUArray<T, SIZE> & GTUArray<T, SIZE>::operator=(const GTUArray<T, SIZE> & rhs)
    {
        for(int i = 0; i < SIZE; ++i)
            (*this)[i] = rhs[i];  /*Copy rhs's elements to 
                                     *the current array*/
        return *this;
    }

    template <typename T, int SIZE>
    void GTUArray<T, SIZE>::erase(const GTUIterator<T> & target)
    {
        auto iterator = begin();    /*Initialize the search 
                                     *iterator to the beginning*/

        /*Search for thr given iterator*/
        for (; iterator != end() && iterator != target; ++iterator);

        if(iterator != end())   *iterator = T();  /*Default the target*/

        else return;
    }

    template <typename T, int SIZE>
    void GTUArray<T, SIZE>::erase(GTUIterator<T> && target)
    {
        auto iterator = begin();    /*Initialize the search 
                                     *iterator to the beginning*/

        for (; iterator != end() && iterator != target;
             ++iterator);   /*Search for thr given iterator*/

        if(iterator != end())   *iterator = T();  /*Default the target*/

        else return;
    }

    template <typename T, int SIZE>
    void GTUArray<T, SIZE>::clear()
    {
        fill(T());  /*Fill the array with defaulted values*/
    }
}

#endif