#include <iostream>
#include <exception>
#include <limits>
#include <iterator>


template <class T>
class Allocator
{
public:
    using value_type = T;
    using pointer = T*;
    using size_type = size_t;

    Allocator(){}

    pointer allocate(size_type count){
        return (pointer) malloc(sizeof(value_type) * count);
    }
    void deallocate(pointer ptr, size_type count){
        for(size_t i = 0; i < count; ++i){
            ptr[i].~value_type();
        }
        free(ptr); 
    }

    size_t max_size(){
        return std::numeric_limits<size_t>::max();
    }
};

template <class T>
class Iterator
{
    T* elem;
    bool rev;
public:

    Iterator(T* ptr, bool reverse=true){
        elem = ptr;
        rev = reverse;
    }

    T& operator* () const{
        return *elem;
    }

    Iterator& operator ++(){
        if (rev) {
            elem++;
        } else{
            elem--;
        }
        return *this;
    }

    Iterator& operator --(){
        if (rev) {
            elem--;
        } else{
            elem++;
        }
        return *this;
    }

    Iterator& operator +(size_t count){
        if (rev) {
            elem += count;
        } else{
            elem -= count;
        }
        return *this;
    }

    Iterator& operator -(size_t count){
        if (rev) {
            elem -= count;
        } else{
            elem += count;
        }
        return *this;
    }

    bool operator == (const Iterator<T>& other) const{
        return elem == other.elem;
    }

    bool operator !=(const Iterator<T>& other) const{
        return !(*this == other);
    }
};

template <class T, class Alloc = Allocator<T> >
class Vector
{
    T* v;
    size_t size_;
    size_t capacity_;
    Alloc alloc_;
public:
    using iterator = Iterator<T>;
    using value_type = T;
    using pointer = T*;

    Vector(){
        alloc_ = Alloc();
        v = alloc_.allocate(1);
        size_ = 0;
        capacity_ = 1;
    }

    ~Vector(){
        alloc_.deallocate(v, size_);
    }

    Vector(size_t count){
        alloc_ = Alloc();
        v = alloc_.allocate(count);
        size_ = 0;
        capacity_ = count;
    }

    value_type& operator[](size_t ind){
        if (ind >= size_ || ind < 0){
            throw std::out_of_range("Bad index");
        }
        return v[ind];
    }

    iterator begin(){
        return iterator(v);
    }

    iterator end(){
        return iterator(v + size_);
    }

    iterator rbegin(){
        return iterator(v + size_ - 1, false);
    }

    iterator rend(){
        return iterator(v - 1, false);
    }

    void push_back(T&& val){
        if (size_ == capacity_){
            auto new_v = alloc_.allocate(capacity_ * 2);
            std::copy(v, v + size_, new_v);
            alloc_.deallocate(v, capacity_);
            v = new_v;
            capacity_ *= 2;
        }
        v[size_++] = val;
    }

    void pop_back(){
        if (size_ == 0){
            throw std::runtime_error("You have empty vector");
        }
        v[size_ - 1].~value_type();
        size_--;
    }

    bool empty(){
        return size_ == 0;
    }

    size_t size(){
        return size_;
    }

    size_t capacity(){
        return capacity_;
    }

    void clear(){
        for(size_t i = 0; i < size_; ++i){
            v[i].~value_type();
        }
        size_ = 0;
    }

    void resize(size_t nSize){
        if (nSize < size_){
            for(size_t i = nSize; i < size_; ++i){
                v[i].~value_type();
            }
            size_ = nSize;
        } else if(nSize > size_){
            if (nSize > capacity_){
                auto new_v = alloc_.allocate(nSize);
                std::copy(v, v + size_, new_v);
                alloc_.deallocate(v, size_);
                v = new_v;
                capacity_ = nSize;
            }
            for(size_t i = size_; i < nSize; ++i){
                new (v + i) value_type();
            }
            size_ = nSize;
        }
    }

    void reserve(size_t n){
        if (n > capacity_){
            pointer new_v = alloc_.allocate(n);
            std::copy(v, v + size_, new_v);
            alloc_.deallocate(v, size_);
            v = new_v;
            capacity_ = n;
        }
    }
};