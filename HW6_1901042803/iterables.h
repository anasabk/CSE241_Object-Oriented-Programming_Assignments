#ifndef ITERABLES_H
#define ITERABLES_H

#include <iostream>
#include <memory>

using std::shared_ptr;

template<typename T>
class iterator : public std::iterator<std::random_access_iterator_tag, T, ptrdiff_t, T*, T&>{
    public:
    iterator() : _pointer(nullptr) {}
    explicit iterator(T * pointer) : _pointer(pointer) {}
    explicit iterator(T & object) : _pointer(&object) {}
    iterator(const iterator<T> & source) : _pointer(source._pointer) {}
    iterator(iterator<T> &&);
    ~iterator() { _pointer = nullptr; }

    inline friend bool operator==(const iterator & lvalue, const iterator & rvalue) 
        { return lvalue._pointer == rvalue._pointer; }   // Overloaded equality operator
    
    inline friend bool operator!=(const iterator & lvalue, const iterator & rvalue) 
        { return lvalue._pointer != rvalue._pointer; }   // Overloaded inequality operator

    inline friend ptrdiff_t operator-(const iterator & lvalue, const iterator & rvalue) 
        { return lvalue._pointer - rvalue._pointer; }   // Overloaded equality operator

    inline bool operator<(iterator<T> rhs) 
        { return _pointer < rhs._pointer; }   // Overloaded equality operator

    inline bool operator>(iterator<T> rhs) 
        { return _pointer > rhs._pointer; }   // Overloaded equality operator

    inline bool operator<=(iterator<T> rhs) 
        { return _pointer <= rhs._pointer; }   // Overloaded equality operator

    inline bool operator>=(iterator<T> rhs) 
        { return _pointer >= rhs._pointer; }   // Overloaded equality operator

    virtual T & operator*() { return *_pointer; }  //Overloaded dereference operator
    virtual T * operator->() { return _pointer; }   //Overloaded member access through pointer operator


    protected:
    T * _pointer;
};

template <typename T, typename iterator_t, typename const_iterator_t>
class iterable
{
    public:
    inline virtual bool empty() const = 0; //Test if list is empty
    virtual int size() const = 0; //Return size
    virtual void erase(const iterator_t &) = 0; //erase element pointed by the given iterator
    virtual void clear() = 0; //Clear the list

    virtual iterator_t begin() const = 0; //return iterator to the first element
    virtual iterator_t end() const = 0; //return iterator to the last element
    virtual const_iterator_t cbegin() const = 0; //return iterator to the first element
    virtual const_iterator_t cend() const = 0; //return iterator to the last element
};


template <typename T>
iterator<T>::iterator(iterator<T> && source)
{
    this->_pointer = source._pointer;
    source._pointer = nullptr;
}


#endif