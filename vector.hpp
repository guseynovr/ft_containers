#ifndef VECTOR_H
#define VECTOR_H

#include <memory>

#include "iterator.hpp"
#include "algorithm.hpp"
#include "type_traits.hpp"
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
    typedef ft::ra_iter<value_type>                  iterator;
    typedef ft::ra_iter<const value_type>            const_iterator;
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
                    const allocator_type& alloc = allocator_type(),
                    typename ft::enable_if<!ft::is_integral<InputIterator>::value, bool>::type = 0);
             vector(const vector& x);

    ~vector();

    vector& operator=(const vector& x);

// Iterators
    iterator               begin()        { return iterator(begin_);                }
    const_iterator         begin() const  { return const_iterator(begin_);          }
    iterator               end()          { return iterator(begin_ + size_);        }
    const_iterator         end() const    { return const_iterator(begin_ + size_);  }
    reverse_iterator       rbegin()       { return reverse_iterator(end());         }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end());   }
    reverse_iterator       rend()         { return reverse_iterator(begin());       }
    const_reverse_iterator rend() const   { return const_reverse_iterator(begin()); }

// Capacity
    size_type size() const     { return size_; }
    size_type max_size()       { return alloc_.max_size(); }
    void      resize(size_type n, value_type val = value_type());
    size_type capacity() const { return capacity_; }
    bool      empty() const    { return !size_; }
    void      reserve(size_type n);

// Element access
    reference       operator[](size_type n)       { return begin_[n]; }
    const_reference operator[](size_type n) const { return begin_[n]; }
    reference       at(size_type n);
    const_reference at(size_type n) const;
    reference       front()       { return *begin_; }
    const_reference front() const { return *begin_; }
    reference       back()        { return begin_[size_-1]; }
    const_reference back() const  { return begin_[size_-1]; }

// Modifiers
    template <class InputIterator>
    void     assign(InputIterator first, InputIterator last,
                    typename ft::enable_if<
                    !ft::is_integral<InputIterator>::value, bool>::type = 0);
    void     assign(size_type n, const value_type& val) { resize(n, val); }
    void     push_back(const value_type& val);
    void     pop_back();
    iterator insert(iterator position, const value_type& val);
    void     insert(iterator position, size_type n, const value_type& val);
    template <class InputIterator>
    void     insert(iterator position, InputIterator first, InputIterator last,
                    typename ft::enable_if<
                    !ft::is_integral<InputIterator>::value, bool>::type = 0);
    iterator erase (iterator position);
    iterator erase (iterator first, iterator last);
    void     swap  (vector& x);
    void     clear();

// Allocator
    allocator_type get_allocator() const { return alloc_; }

// Non-member function overloads
    template <class U, class Alloc>
    friend bool operator==(const vector<U,Alloc>& lhs, const vector<U,Alloc>& rhs);
    template <class U, class Alloc>
    friend bool operator!=(const vector<U,Alloc>& lhs, const vector<U,Alloc>& rhs);
    template <class U, class Alloc>
    friend bool operator< (const vector<U,Alloc>& lhs, const vector<U,Alloc>& rhs);
    template <class U, class Alloc>
    friend bool operator<=(const vector<U,Alloc>& lhs, const vector<U,Alloc>& rhs);
    template <class U, class Alloc>
    friend bool operator> (const vector<U,Alloc>& lhs, const vector<U,Alloc>& rhs);
    template <class U, class Alloc>
    friend bool operator>=(const vector<U,Alloc>& lhs, const vector<U,Alloc>& rhs);
    template <class U, class Alloc>
    friend void swap(vector<U,Alloc>& x, vector<U,Alloc>& y);

private:
    allocator_type alloc_;
    pointer        begin_;
    size_type      size_;
    size_type      capacity_;

    void    move_(pointer dst, pointer src, difference_type len);
    void    destroy_();
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
    , begin_(alloc_.allocate(n))
    , size_(0)
    , capacity_(n)
{
    while (size_ < n) {
        alloc_.construct(begin_ + size_++, val);
    }
}

template <class T, class Allocator>
    template <class InputIterator>
vector<T, Allocator>::vector(InputIterator first, InputIterator last, const allocator_type& alloc,
typename ft::enable_if<!ft::is_integral<InputIterator>::value, bool>::type)
    : alloc_(alloc)
    , begin_(alloc_.allocate(last - first))
    , size_(0)
    , capacity_(last - first)
{
    for (; first != last; ++first, ++size_) {
        try {
            alloc_.construct(begin_ + size_, *first);
        }
        catch (...) {
            destroy_();
            throw;
        }
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
    destroy_();
}

template <class T, class Allocator>
vector<T, Allocator>& vector<T, Allocator>::operator=(const vector& other)
{
    if (this == &other)
        return *this;
    clear();
    alloc_ = other.alloc_;
    reserve(other.size_);
    while (size_ < other.size_) {
        alloc_.construct(begin_ + size_, other[size_]);
        ++size_;
    }
    return *this;
}

/***** Capacity *****/

template <class T, class Allocator>
void vector<T, Allocator>::resize(size_type n, value_type val)
{
    // std::cout << "before n = " << n << ", size_ = " << size_ << std::endl;
    while (n < size_) {
        alloc_.destroy(begin_ + --size_);
    }
    reserve(n);
    while (size_ < n) {
        alloc_.construct(begin_ + size_++, val);
    }
    // std::cout << "after size_ = " << size_ << std::endl;
}

template <class T, class Allocator>
void vector<T, Allocator>::reserve(size_type n)
{
    if (n <= capacity_) {
        return;
    }
    size_type old_cap = capacity_;
    if (capacity_ && n < capacity_ << 1) capacity_ <<= 1;
    else capacity_ = n;
    pointer old = begin_;
    try {
        begin_ = alloc_.allocate(capacity_);
    }
    catch (std::bad_alloc& e) {
        destroy_();
        throw;
    }
    move_(begin_, old, size_);
    alloc_.deallocate(old, old_cap);
}

/***** Element access *****/

template <class T, class Allocator>
typename vector<T, Allocator>::reference vector<T, Allocator>::at(size_type n)
{
    if (n < 0 || n >= size_)
        throw std::out_of_range("vector");
    return begin_[n];
}

template <class T, class Allocator>
typename vector<T, Allocator>::const_reference vector<T, Allocator>::at(size_type n) const
{
    if (n < 0 || n >= size_)
        throw std::out_of_range("vector");
    return begin_[n];
}

/***** Modifiers *****/

template <class T, class Allocator>
    template <class InputIterator>
void vector<T, Allocator>::assign(InputIterator first, InputIterator last,
typename ft::enable_if<!ft::is_integral<InputIterator>::value, bool>::type)
{
    // std::cout << "before n = " << last - first << ", size_ = " << size_ << std::endl;
    resize(last - first);
    size_ = 0;
    while (first != last) {
        begin_[size_++] = *first;
        ++first;
    }
    // std::cout << "after size_ = " << size_ << std::endl;
}

template <class T, class Allocator>
void vector<T, Allocator>::push_back(const value_type& val)
{
    // std::cout << "push_back";
    // std::cout.flush();
    reserve(size_ + 1);
    // std::cout << "\tstill ok\n";
    // std::cout.flush();
    alloc_.construct(begin_ + size_++, val);
}

template <class T, class Allocator>
void vector<T, Allocator>::pop_back()
{
    if (size_) {
        alloc_.destroy(begin_ + --size_);
    }
}

template <class T, class Allocator>
typename vector<T, Allocator>::iterator
vector<T, Allocator>::insert(iterator position, const value_type& val)
{
    difference_type shift = &*position - begin_;

    reserve(size_ + 1);
    pointer pos = begin_ + shift;
    move_(pos + 1, pos, size_ - shift);
    ++size_;
    alloc_.construct(pos, val);
    return iterator(pos);
}

template <class T, class Allocator>
void vector<T, Allocator>::insert(iterator position, size_type n, const value_type& val)
{
    difference_type shift = &*position - begin_;

    // std::cout << "insert\t";
    // std::cout.flush();
    reserve(size_ + n);
    // std::cout << "ok\n";
    // std::cout.flush();
    pointer pos = begin_ + shift;
    // pointer pos =  &*position;
    move_(pos + n, pos, size_ - shift);
    size_ += n;
    for (; n != 0; pos++, --n) {
        alloc_.construct(pos, val);
    }
}

template <class T, class Allocator>
    template <class InputIterator>
void vector<T, Allocator>::insert(iterator position, InputIterator first, InputIterator last,
typename ft::enable_if<!ft::is_integral<InputIterator>::value, bool>::type)
{
    difference_type shift = &*position - begin_;
    difference_type n = last - first;

    reserve(size_ + n);
    pointer pos = begin_ + shift;
    move_(pos + n, pos, size_ - shift);
    size_ += n;
    for (; first != last; ++first, ++pos) {
        try {
            alloc_.construct(pos,  *first);
        }
        catch (...) {
            destroy_();
            throw;
        }
    }
}

template <class T, class Allocator>
typename vector<T, Allocator>::iterator
vector<T, Allocator>::erase(iterator position)
{
    pointer pos = &*position;

    alloc_.destroy(pos);
    move_(pos, pos + 1, begin_ + --size_ - pos);
    return position;
}

template <class T, class Allocator>
typename vector<T, Allocator>::iterator
vector<T, Allocator>::erase(iterator first, iterator last)
{
    difference_type n = last - first;

    for (iterator it = first; it != last; ++it) {
        alloc_.destroy(&*first);
    }
    move_(&*first, &*first + n, end() - last - 1);
    size_ -= n;
    return first;
}

template <class T, class Allocator>
void vector<T, Allocator>::swap(vector& x)
{
    ft::swap(begin_, x.begin_);
    ft::swap(size_, x.size_);
    ft::swap(capacity_, x.capacity_);
}

template <class T, class Allocator>
void vector<T, Allocator>::clear()
{
    while (size_ > 0) {
        alloc_.destroy(begin_ + --size_);
    }
}

/***** Non-member function overloads *****/

template <class U, class Alloc>
inline bool operator==(const vector<U,Alloc>& lhs, const vector<U,Alloc>& rhs)
{
    if (lhs.size_ != rhs.size_)
        return false;
    for (typename vector<U,Alloc>::size_type i = 0; i < lhs.size_; ++i) {
        if (lhs[i] != rhs[i])
            return false;
    }
    return true;
}

template <class U, class Alloc>
inline bool operator!=(const vector<U,Alloc>& lhs, const vector<U,Alloc>& rhs)
{
    return !(lhs == rhs);
}

template <class U, class Alloc>
inline bool operator< (const vector<U,Alloc>& lhs, const vector<U,Alloc>& rhs)
{
    // int i = 0;
    // for (; i < lhs.size_ && i < rhs.size_; ++i) {
    //     if (lhs[i] < rhs[i]) return true;
    //     if (rhs[i] < lhs[i]) return false;
    // }
    // return (lhs.size_ < rhs.size_);
    return ft::lexicographical_compare(lhs.begin(), lhs.end(),
                                       rhs.begin(), rhs.end());
}

template <class U, class Alloc>
inline bool operator<=(const vector<U,Alloc>& lhs, const vector<U,Alloc>& rhs)
{
    return lhs == rhs || lhs < rhs;
}

template <class U, class Alloc>
inline bool operator> (const vector<U,Alloc>& lhs, const vector<U,Alloc>& rhs)
{
    return !(lhs <= rhs);
}

template <class U, class Alloc>
inline bool operator>=(const vector<U,Alloc>& lhs, const vector<U,Alloc>& rhs)
{
    return !(lhs < rhs);
}

template <class U, class Alloc>
inline void swap(vector<U,Alloc>& x, vector<U,Alloc>& y)
{
    x.swap(y);
}

/***** private *****/

template <class T, class Allocator>
inline void vector<T, Allocator>::move_(pointer dst, pointer src, difference_type len)
{
    if (dst < src) {
        for (difference_type i = 0; i < len; ++i) {
            // dst[i] = src[i];
            alloc_.construct(dst + i, src[i]);
            alloc_.destroy(src + i);
        }
    } else {
        for (difference_type i = len - 1; i >= 0; --i) {
            // dst[i] = src[i];
            alloc_.construct(dst + i, src[i]);
            alloc_.destroy(src + i);
        }
    }
}

template <class T, class Allocator>
inline void vector<T, Allocator>::destroy_()
{
    clear();
    alloc_.deallocate(begin_, capacity_);
    begin_ = NULL;
    capacity_ = 0;
}

}; // namespace ft

namespace std {
	template <class T, class A>
	void swap(ft::vector<T, A> &v1, ft::vector<T, A> &v2 ) {
		v1.swap(v2);
	}

	// template <class Key, class T, class Compare, class A>
	// void swap(ft::Map<Key, T, Compare, A> &m1, ft::Map<Key, T, Compare, A> &m2 ) {
	// 	m1.swap(m2);
	// }
}; //namespace std

#endif // VECTOR_H
