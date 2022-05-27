#ifndef GTUVECTOR_H
#define GTUVECTOR_H

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
    class GTUVector : public iterable<T, GTUIterator<T>, GTUIteratorConst<T>>
    {
        public:
        //Constructors
        /*Default Constructor*/
        GTUVector() : _data(nullptr), _size(0), _capacity(0) {}

        /*A Constructor that take an integer*/
        GTUVector(int size)  : _size(size), _capacity(5 * (size/5 + 1)) 
            { reallocate(); }

        /*Copy Constructor*/
        GTUVector(const GTUVector &);

        /*Move Constructor*/
        GTUVector(GTUVector &&);

        /*Constructor for Initializer Lists*/
        GTUVector(const initializer_list<T> list) : GTUVector() 
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

        /*Increment the capacity by 5*/
        void incrCapacity() { _capacity += 5; }

        /*Decrement the capacity by 5*/
        void decrCapacity() { _capacity -= 5; }

        /*Reallocate the vector*/
        void reallocate();


        //Getter functions
        /*Test if list is empty*/
        inline bool empty() const {
            if(size() == 0) return true; 
            else return false;
        }

        /*Return the number of elements in the set*/
        inline int size() const { return _size; } 

        /*Return the maximum number of elements possible before reallocation*/
        inline int capacity() const { return _capacity; }


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
        GTUVector<T> & operator=(const GTUVector<T> & vector);

        /*Move Assignment operator*/
        GTUVector<T> & operator=(GTUVector<T> && vector);

        /*Index operator*/
        T & operator[](int index) 
            { return _data[index]; }

        /*Const-Index operator*/
        const T & operator[](int index) const 
            { return _data[index]; }


        private:
        shared_ptr<T[]> _data;

        int _capacity, /*The number of elements this vector 
                        *can use without reallocating*/

            _size;  /*The number of active cells*/
    };
    
    template <typename T>
    GTUVector<T>::GTUVector(GTUVector<T> && rhs)
    {
        this->_data = move(rhs._data);   /*Make rhs's data as the 
                                          *current vector's property*/
        _size = rhs._size;  /*Update the size of the current vector*/
        _capacity = rhs._capacity; /*Update the capacity of the current vector*/
        rhs.clear(); /*Clear rhs*/
    }

    template <typename T>
    GTUVector<T>::GTUVector(const GTUVector<T> & rhs)
    {
        _size = rhs.size(); /*Update the size of the current vector*/
        _capacity = rhs.capacity(); /*Update the capacity of the current vector*/
        reallocate(); /*Reallocate the vector according 
                       *to the new size and capacity info*/

        for(int i = 0; i < _size; ++i)
            (*this)[i] = rhs[i]; /*Copy each member to the current vector*/
    }

    template <typename T>
    GTUVector<T> & GTUVector<T>::operator=(GTUVector<T> && rhs)
    {
        if (this != &rhs)
        {
            this->clear(); /*Reset the current vector*/

            /*Copy rhs's info to the current vector*/
            this->_data = move(rhs._data);
            _size = rhs._size;
            _capacity = rhs._capacity;

            /*Clear rhs*/
            rhs.clear();
        }

        return *this;
    }

    template <typename T>
    GTUVector<T> & GTUVector<T>::operator=(const GTUVector<T> & vector)
    {
        _size = vector.size(); /*Update the size of the current vector*/
        _capacity = vector.capacity(); /*Update the capacity of the current vector*/
        reallocate(); /*Reallocate the vector according 
                       *to the new size and capacity info*/

        for(int i = 0; i < _size; ++i)
            (*this)[i] = vector[i]; /*Copy each member to the current vector*/

        return *this;
    }

    template <typename T>
    void GTUVector<T>::reallocate()
    {
        /*Create a temporary list*/
        shared_ptr<T[]> newArray (new T[_capacity]);

        /*Copy the data to the temporary list*/
        if(_data != nullptr)
            for (int counter = 0; counter < _size; ++counter)
                newArray[counter] = _data[counter];

        /*Make the temporary list the current vector's property*/
        _data = move(newArray);
    }

    template <typename T>
    void GTUVector<T>::erase(const GTUIterator<T> & target)
    {
        auto iterator = begin(); /*Initialize the search iterator 
                                  *to the start position*/

        /*Search for thr given iterator*/
        for ( ; (iterator != end()) && (iterator != target); ++iterator);

        if(iterator != end())
        {
            ++iterator;

            /*Copy each member after the target to the member before it*/
            for (; iterator != end();  ++iterator) 
                *(iterator.getPrevious()) = *iterator;

            *(end().getPrevious()) = T();  /*Default the target*/
            --_size;

            if(_capacity - _size >= 5) /*If 5 or more cells were empty, 
                                        *decrease the capacity of the 
                                        *vector and reallocate it*/
            {
                decrCapacity();
                reallocate();
            }

            else return;
        }

        else return;
    }

    template <typename T>
    void GTUVector<T>::erase(GTUIterator<T> && target)
    {
        auto iterator = begin(); /*Initialize the search iterator 
                                  *to the start position*/

        /*Search for thr given iterator*/
        for ( ; (iterator != end()) && (iterator != target); ++iterator);

        if(iterator != end())
        {
            ++iterator;

            /*Copy each member after the target to the member before it*/
            for (; iterator != end();  ++iterator) 
                *(iterator.getPrevious()) = *iterator;

            *(end().getPrevious()) = T();  /*Default the target*/
            --_size;

            if(_capacity - _size >= 5) /*If 5 or more cells were empty, 
                                        *decrease the capacity of the 
                                        *vector and reallocate it*/
            {
                decrCapacity();
                reallocate();
            }

            else return;
        }

        else return;
    }

    template <typename T>
    void GTUVector<T>::add(const T & target)
    {
        if (_size == _capacity) /*If there is no more space 
                                 *for a new object, expand the vector
                                 */
        {
            incrCapacity();
            reallocate();
        }

        _data[_size] = target; /*Add the new object*/
        ++_size; /*increment the size*/
    }

    template <typename T>
    void GTUVector<T>::add(T && target)
    {
        if (_size == _capacity) /*If there is no more space 
                                 *for a new object, expand the vector
                                 */
        {
            incrCapacity();
            reallocate();
        }

        _data[_size] = move(target); /*Move the new object*/
        ++_size; /*increment the size*/
    }

    template <typename T>
    void GTUVector<T>::clear()
    {
        _size = 0;  /*reset the size*/
        _capacity = 0; /*reset the capacity*/
        _data.reset(); /*reset the _data list to it's empty state*/
    }
}

#endif