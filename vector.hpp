#ifndef VECTOR_H
#define VECTOR_H

#include <memory>
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
    typedef implementation-defined                   iterator;
    typedef implementation-defined                   const_iterator;
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

    ~vector()

    vector& operator=(const vector& x);

// Iterators
    iterator               begin();
    const_iterator         begin() const;
    iterator               end();
    const_iterator         end() const;
    reverse_iterator       rbegin();
    const_reverse_iterator rbegin() const;
    reverse_iterator       rend();
    const_reverse_iterator rend() const;

// Capacity
    size_type size() const;
    size_type max_size();
    void      resize(size_type n, value_type val = value_type());
    size_type capacity() const;
    bool      empty() const;
    void      reserve(size_type n);

// Element access
    reference       operator[](size_type n);
    const_reference operator[](size_type n) const;
    reference       at(size_type n);
    const_reference at(size_type n) const;
    reference       front();
    const_reference front() const;
    reference       back();
    const_reference back() const;

// Modifiers
    template <class InputIterator>
    void     assign(InputIterator first, InputIterator last);
    void     assign(size_type n, const value_type& val);
    void     push_back(const value_type& val);
    void     pop_back();
    iterator insert(iterator position, const value_type& val);
    void     insert(iterator position, size_type n, const value_type& val);
    template <class InputIterator>
    void     insert(iterator position, InputIterator first, InputIterator last);
    iterator erase (iterator position);
    iterator erase (iterator first, iterator last);
    void     swap(vector& x);
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

/* Constructors */
template <class T, class Allocator>
vector<T, Allocator>::vector(const allocator_type& alloc)
    : alloc_(alloc)
    , begin_(NULL)
    , size_(0)
    , capacity(0)
{
}

template <class T, class Allocator>
vector<T, Allocator>::vector(size_type n, const value_type& val, const allocator_type& alloc)
    : alloc_(alloc)
    , begin_(alloc.allocate(n))
    , size_(0)
    , capacity(n)
{
    while (size_ < n)
        alloc.construct(begin_ + size_++, val)
}
// <class InputIterator>
// vector(InputIterator first, InputIterator last,
// const allocator_type& alloc = allocator_type());
// vector(const vector& x);

} // namespace ft

#endif // VECTOR_H
