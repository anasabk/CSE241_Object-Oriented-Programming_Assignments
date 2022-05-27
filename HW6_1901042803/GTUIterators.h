#ifndef GTUITERATORS_H
#define GTUITERATORS_H

#include <iostream>
#include <memory>

#include "iterables.h"


template<typename T>
class iterator;

namespace GTU
{
    template<typename T>
    class GTUIterator : public iterator<T>
    {
        public:
        //Constructors
        /*Default Constructor*/
        GTUIterator() : iterator<T>() {}

        /*Copy Constructor*/
        GTUIterator(const GTUIterator & iterator) : iterator<T>(iterator) {}
        
        /*Move Constructor*/
        GTUIterator(GTUIterator && iterator) : iterator<T>(move(iterator)) {}

        /*A constructor for pointers*/
        explicit GTUIterator(T * pointer) : iterator<T>(pointer) {}

        /*A constructor for references*/
        explicit GTUIterator(T & object) : iterator<T>(object) {}

        
        //Overloaded operators
        /*Copy Assignment operator*/
        GTUIterator & operator=(const GTUIterator & iterator) 
            { this->_pointer = iterator._pointer; return *this; }

        /*Move Assignment operator*/
        GTUIterator & operator=(GTUIterator &&);
        
        /*Pre-increment operator*/
        virtual inline GTUIterator & operator++() 
            { ++this->_pointer; return *this; }

        /*Post-increment operator*/
        virtual inline GTUIterator operator++(int) 
            { return GTUIterator(this->_pointer++); }

        /*Pre-decrement operator*/
        virtual GTUIterator & operator--() 
            { --this->_pointer; return *this; }

        /*Post-decrement operator*/
        virtual GTUIterator operator--(int) 
            { return GTUIterator(this->_pointer--); }

        /*pointer-minus-ptrdiff_t operator*/
        inline GTUIterator operator-(ptrdiff_t n) 
            { return GTUIterator(this->_pointer - n); }
        
        /*pointer-plus-ptrdiff_t operator*/
        inline GTUIterator operator+(ptrdiff_t n) 
            { return GTUIterator(this->_pointer + n); }
            
        /*pointer-plus-ptrdiff_t operator*/
        inline friend GTUIterator<T> operator+(ptrdiff_t n, const GTUIterator & lvalue) 
            { return GTUIterator<T>(lvalue._pointer + n); }
        
        inline GTUIterator & operator+=(ptrdiff_t n) 
            { this->_pointer += n; return *this; }

        inline GTUIterator & operator-=(ptrdiff_t n) 
            { this->_pointer -= n; return *this; }   // Overloaded equality operator

        //Getter functions
        /*return an iterator for the previous member*/
        inline GTUIterator getPrevious() const 
            { auto temp = *this; return --temp; }

        /*return an iterator for the next member*/
        inline GTUIterator getNext() const 
            { auto temp = *this; return ++temp; }
    };

    template<typename T>
    class GTUIteratorConst : public iterator<T>
    {
        public:
        //Constructors
        /*Default Constructor*/
        GTUIteratorConst() : iterator<T>() {}

        /*Copy Constructor*/
        GTUIteratorConst(const GTUIteratorConst & iterator) : iterator<T>(iterator) {}
        
        /*Move Constructor*/
        GTUIteratorConst(GTUIteratorConst && iterator) : iterator<T>(move(iterator)) {}

        /*A constructor for pointers*/
        explicit GTUIteratorConst(T * pointer) : iterator<T>(pointer) {}

        /*A constructor for references*/
        explicit GTUIteratorConst(T & object) : iterator<T>(object) {}

        
        //Overloaded operators
        /*Copy Assignment operator*/
        GTUIteratorConst & operator=(const GTUIteratorConst & iterator) 
            { this->_pointer = iterator._pointer; return *this; }

        /*Move Assignment operator*/
        GTUIteratorConst & operator=(GTUIteratorConst &&);
        
        /*Pre-increment operator*/
        virtual inline GTUIteratorConst & operator++() 
            { ++this->_pointer; return *this; }

        /*Post-increment operator*/
        virtual inline GTUIteratorConst operator++(int) 
            { return GTUIteratorConst(this->_pointer++); }

        /*Pre-decrement operator*/
        virtual GTUIteratorConst & operator--() 
            { --this->_pointer; return *this; }

        /*Post-decrement operator*/
        virtual GTUIteratorConst operator--(int) 
            { return GTUIteratorConst(this->_pointer--); }

        /*pointer-minus-ptrdiff_t operator*/
        inline GTUIteratorConst operator-(ptrdiff_t n) 
            { return GTUIteratorConst(this->_pointer - n); }
        
        /*pointer-plus-ptrdiff_t operator*/
        inline GTUIteratorConst operator+(ptrdiff_t n) 
            { return GTUIteratorConst(this->_pointer + n); }
            
        /*pointer-plus-ptrdiff_t operator*/
        inline friend GTUIteratorConst<T> operator+(ptrdiff_t n, const GTUIteratorConst & lvalue) 
            { return GTUIteratorConst<T>(lvalue._pointer + n); }
        
        inline GTUIteratorConst & operator+=(ptrdiff_t n) 
            { this->_pointer += n; return *this; }

        inline GTUIteratorConst & operator-=(ptrdiff_t n) 
            { this->_pointer -= n; return *this; }   // Overloaded equality operator

        //Getter functions
        /*return an iterator for the previous member*/
        inline GTUIteratorConst getPrevious() const 
            { auto temp = *this; return --temp; }

        /*return an iterator for the next member*/
        inline GTUIteratorConst getNext() const 
            { auto temp = *this; return ++temp; }
    
    };

    
    template <typename T>
    GTUIterator<T> & GTUIterator<T>::operator=(GTUIterator && rvalue)
    {
        if (*this != rvalue)
        {
            this->_pointer = rvalue._pointer;
            rvalue._pointer = nullptr;
        }

        return *this;
    }

    template <typename T>
    GTUIteratorConst<T> & GTUIteratorConst<T>::operator=(GTUIteratorConst && rvalue)
    {
        if (this != &rvalue)
        {
            this->_pointer = rvalue._pointer;
            rvalue._pointer = nullptr;
        }

        return *this;
    }
}


#endif