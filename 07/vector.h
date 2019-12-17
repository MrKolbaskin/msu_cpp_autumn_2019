#ifndef VECTOR_CLASS
#define VECTOR_CLASS 


template <class T>
class Allocator
{
public:
    using value_type = T;
    using pointer = T*;
    using size_type = size_t;

    pointer allocate(size_type count){
        return (pointer) malloc(sizeof(value_type) * count);
    }

    void construct(pointer ptr, size_type count){
        for (size_type i = 0; i < count; ++i) {
            new (ptr + i) value_type();
        }
    }

    void construct(pointer ptr, value_type val){
        new (ptr) value_type(val);
    }

    void deallocate(pointer ptr, size_type count){
        free(ptr); 
    }

    void destructor(pointer ptr, size_type count){
        for(size_t i = 0; i < count; ++i){
            ptr[i].~value_type();
        }
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
        size_ = 0;
        capacity_ = 0;
        v = nullptr;
    }

    ~Vector(){
        alloc_.destructor(v, size_);
        alloc_.deallocate(v, capacity_);
    }

    Vector(size_t count){
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
        if (capacity_ == 0){
            v = alloc_.allocate(1);
            alloc_.construct(v, val);
            size_ = 1;
            capacity_ = 1;
        }
        else if (size_ == capacity_){
            auto new_v = alloc_.allocate(capacity_ * 2);

            for (size_t i = 0; i < size_; ++i){
                alloc_.construct(new_v + i, v[i]);
            }

            alloc_.destructor(v, size_);
            alloc_.deallocate(v, capacity_);
            v = new_v;
            alloc_.construct(v + size_, val);
            size_++;
            capacity_ *= 2;
        } else {
            alloc_.construct(v + size_, val);
            size_++;
        }
    }

    void pop_back(){
        if (size_ == 0){
            throw std::runtime_error("You have empty vector");
        }
        alloc_.destructor(v + size_ - 1, 1);
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
        alloc_.destructor(v, size_);
        size_ = 0;
    }

    void resize(size_t nSize){
        if (nSize < size_){
            alloc_.destructor(v + nSize, size_ - nSize);
            size_ = nSize;
        } else if(nSize > size_){
            if (nSize > capacity_){
                auto new_v = alloc_.allocate(nSize);

                for (size_t i = 0; i < size_; ++i){
                    alloc_.construct(new_v + i, v[i]);
                }

                alloc_.destructor(v, size_);
                alloc_.deallocate(v, capacity_);
                v = new_v;
                capacity_ = nSize;
            }
            alloc_.construct(v + size_, nSize - size_);
            size_ = nSize;
        }
    }

    void reserve(size_t n){
        if (n > capacity_){
            pointer new_v = alloc_.allocate(n);

            for (size_t i = 0; i < size_; ++i){
                alloc_.construct(new_v + i, v[i]);
            }

            alloc_.destructor(v, size_);
            alloc_.deallocate(v, size_);
            v = new_v;
            capacity_ = n;
        }
    }
};

#endif