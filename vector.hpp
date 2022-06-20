#ifndef VECTOR_H
#define VECTOR_H

#include <memory>

#include "iterator.hpp"
// #include <vector>

namespace ft {

template <class T, class Allocator = std::allocator<T> >
class vector
{
public:

// Member types
    typedef T                                        value_type;
    typedef Allocator                                allocator_type;
    typedef typename allocator_type::reference       reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::pointer         pointer;
    typedef typename allocator_type::const_pointer   const_pointer;
    typedef ft::ra_iter                              iterator;
    typedef ft::ra_iter                              const_iterator;
    typedef ft::reverse_iterator<iterator>           reverse_iterator;
    typedef ft::reverse_iterator<const_iterator>     const_reverse_iterator;
    typedef typename allocator_type::difference_type difference_type;
    typedef std::size_t                              size_type;

// Constructors
    explicit vector(const allocator_type& alloc = allocator_type());
    explicit vector(size_type n, const value_type& val = value_type(),
                    const allocator_type& alloc = allocator_type());
    template <class InputIterator>
             vector(InputIterator first, InputIterator last,
                    const allocator_type& alloc = allocator_type());
             vector(const vector& x);

    ~vector();

    vector& operator=(const vector& x);

// Iterators
    iterator               begin()        { return iterator(begin_); }
    const_iterator         begin() const; { return iterator(begin_); }
    iterator               end();         { return iterator(begin_ + size_); }
    const_iterator         end() const;   { return iterator(begin_ + size_); }
    reverse_iterator       rbegin();      { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const;{ return reverse_iterator(end()); }
    reverse_iterator       rend();        { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const;  { return reverse_iterator(begin()); }

// Capacity
    size_type size() const      { return size_; }
    size_type max_size();       { return alloc_.max_size(); }
    void      resize(size_type n, value_type val = value_type());
    size_type capacity() const; { return capacity_; }
    bool      empty() const;    { return !size_; }
    void      reserve(size_type n);

// Element access
    reference       operator[](size_type n)        { return begin_[n]; }
    const_reference operator[](size_type n) const; { return begin_[n]; }
    reference       at(size_type n)                { if (n < 0 || n >= size_)
                                                        throw std::out_of_range("vector");
                                                     return begin_[n]; }
    const_reference at(size_type n) const          { if (n < 0 || n >= size_)
                                                        throw std::out_of_range("vector");
                                                     return begin_[n]; }
    reference       front()        { return *begin_; }
    const_reference front() const; { return *begin_; }
    reference       back();        { return *begin_[size_-1]; }
    const_reference back() const;  { return *begin_[size_-1]; }

// Modifiers
    template <class InputIterator>
    void     assign(InputIterator first, InputIterator last);
    void     assign(size_type n, const value_type& val) { resize(n, val); }
    void     push_back(const value_type& val) { reserve(size_ + 1);
                                                alloc_.construct(begin_ + size_++); }
    void     pop_back()                       { if (size_)
                                                  alloc_.destroy(begin_ + --size_); }
    iterator insert(iterator position, const value_type& val);
    void     insert(iterator position, size_type n, const value_type& val);
    template <class InputIterator>
    void     insert(iterator position, InputIterator first, InputIterator last);
    iterator erase (iterator position);
    iterator erase (iterator first, iterator last);
    void     swap  (vector& x);
    void     clear();

// Allocator
    allocator_type get_allocator() const;

// Non-member function overloads
    template <class U, class Alloc>
    friend bool operator==(const std::vector<U,Alloc>& lhs, const std::vector<U,Alloc>& rhs);
    template <class U, class Alloc>
    friend bool operator!=(const std::vector<U,Alloc>& lhs, const std::vector<U,Alloc>& rhs);
    template <class U, class Alloc>
    friend bool operator< (const std::vector<U,Alloc>& lhs, const std::vector<U,Alloc>& rhs);
    template <class U, class Alloc>
    friend bool operator<=(const std::vector<U,Alloc>& lhs, const std::vector<U,Alloc>& rhs);
    template <class U, class Alloc>
    friend bool operator> (const std::vector<U,Alloc>& lhs, const std::vector<U,Alloc>& rhs);
    template <class U, class Alloc>
    friend bool operator>=(const std::vector<U,Alloc>& lhs, const std::vector<U,Alloc>& rhs);
    template <class U, class Alloc>
    friend void swap(vector<U,Alloc>& x, vector<U,Alloc>& y);

private:
    allocator_type alloc_;
    pointer        begin_;
    size_type      size_;
    size_type      capacity_;
};


/***** Constructors *****/

template <class T, class Allocator>
vector<T, Allocator>::vector(const allocator_type& alloc)
    : alloc_(alloc)
    , begin_(NULL)
    , size_(0)
    , capacity_(0)
{
}

template <class T, class Allocator>
vector<T, Allocator>::vector(size_type n, const value_type& val, const allocator_type& alloc)
    : alloc_(alloc)
    , begin_(alloc.allocate(n))
    , size_(0)
    , capacity_(n)
{
    while (size_ < n)
        alloc_.construct(begin_ + size_++, val);
}

template <class T, class Allocator>
    template <class InputIterator>
vector<T, Allocator>::vector(InputIterator first, InputIterator last, const allocator_type& alloc)
    : alloc_(alloc)
    , begin_(alloc.allocate(last - first))
    , size_(0)
    , capacity_(last - first)
{
    for ; first != last; ++first, ++size_ {
        alloc_.construct(begin_ + size_, *first);
    }
}

template <class T, class Allocator>
vector<T, Allocator>::vector(const vector& x)
    : alloc_(x.alloc_)
    , begin_(NULL)
    , size_(0)
    , capacity_(0)
{
    *this = x;
}

template <class T, class Allocator>
vector<T, Allocator>::~vector()
{
    clear();
    while (size_-- > 0) {
        alloc_.destroy(begin_ + size_);
    }
    alloc_.deallocate(begin_, capacity_);
}

template <class T, class Allocator>
vector<T, Allocator>& vector<T, Allocator>::operator=(const vector& other)
{
    if (this == &other)
        return *this;
    clear();
    alloc_ = other.alloc_;
    if (capacity_ < other.capacity_) {
       begin_ = alloc_.allocate(other.capacity_);
    }
    while (size_ < other.size_) {
        alloc_.construct(begin_ + size_, other[size_])
        ++size_;
    }
}

/***** Capacity *****/

template <class T, class Allocator>
void vector<T, Allocator>::resize(size_type n, value_type val)
{
    while (n < size_) {
        alloc_.destroy(begin_ + --size_);
    }
    reserve(n);
    while (size_ < n) {
        alloc_.construct(begin_ + size_++, val);
    }
}

template <class T, class Allocator>
void vector<T, Allocator>::reserve(size_type n)
{
    if (n <= capacity_) {
        return;
    }
    size_type old_cap = capacity_;
    if (capacity_ == 0) capacity_ = 1;
    while (n > capacity_) capacity_ << 1;
    pointer old = begin_;
    begin_ = alloc_.allocate(capacity_);
    for (size_type i = 0; i < size_; ++i) {
        begin_[i] = old[i];
        alloc_.destroy(old[i]);
    }
    alloc_.deallocate(old, old_cap);
}

/***** Modifiers *****/

template <class T, class Allocator>
    template <class InputIterator>
void vector<T, Allocator>::assign(InputIterator first, InputIterator last)
{
    resize(last - first);
    size_ = 0;
    while (first != last) {
        begin_[size_++] = *first++;
    }
}

template <class T, class Allocator>
iterator vector<T, Allocator>::insert(iterator position, const value_type& val) {
    // reserve(size_ + 1);

}


} // namespace ft

#endif // VECTOR_H
