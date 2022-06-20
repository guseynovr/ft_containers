#ifndef ITERATOR_H
#define ITERATOR_H

#include <iterator>
namespace ft {

using std::input_iterator_tag;
using std::output_iterator_tag;
using std::forward_iterator_tag;
using std::bidirectional_iterator_tag;
using std::random_access_iterator_tag;
// struct input_iterator_tag {};
// struct output_iterator_tag {};
// struct forward_iterator_tag       : public input_iterator_tag {};
// struct bidirectional_iterator_tag : public forward_iterator_tag {};
// struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template <
    class Category,
    class T,
    class Distance  = ptrdiff_t,
    class Pointer   = T*,
    class Reference = T&>
struct iterator {
    typedef T         value_type;
    typedef Distance  difference_type;
    typedef Pointer   pointer;
    typedef Reference reference;
    typedef Category  iterator_category;
};

template <class Iter>
struct iterator_traits {
    typedef typename Iter::difference_type   difference_type;
    typedef typename Iter::value_type        value_type;
    typedef typename Iter::reference         reference;
    typedef typename Iter::pointer           pointer;
    typedef typename Iter::iterator_category iterator_category;
};

template <class T>
struct iterator_traits<T*>
{
    typedef std::ptrdiff_t             difference_type;
    typedef T                          value_type;
    typedef T*                         pointer;
    typedef T&                         reference;
    typedef random_access_iterator_tag iterator_category;
};

template <class T>
struct iterator_traits<const T*>
{
    typedef std::ptrdiff_t             difference_type;
    typedef T                          value_type;
    typedef const T*                   pointer;
    typedef const T&                   reference;
    typedef random_access_iterator_tag iterator_category;
};

template <class T>
class ra_iter : public iterator<random_access_iterator_tag, T>
{
public:
    typedef typename iterator<random_access_iterator_tag, T>::iterator_category iterator_category;
    typedef typename iterator<random_access_iterator_tag, T>::value_type        value_type;
    typedef typename iterator<random_access_iterator_tag, T>::difference_type   difference_type;
    typedef typename iterator<random_access_iterator_tag, T>::pointer           pointer;
    typedef typename iterator<random_access_iterator_tag, T>::reference         reference;

    ra_iter(pointer ptr) : ptr_(ptr) {}
    ra_iter(const ra_iter& rai) : ptr_(rai.ptr_) {}
    ~ra_iter();

    ra_iter&        operator=(const ra_iter& rai) {
        if (this != &rai) {
            ptr_ = rai.ptr_;
        }
        return *this;
    }

// Dereference
    reference       operator*()                          { return *ptr_;   }
    const reference operator*() const                    { return *ptr_;   }
    pointer         operator->()                         { return ptr_;    }
    const pointer   operator->() const                   { return ptr_;    }
    reference       operator[](difference_type n);       { return ptr_[n]; }
    const reference operator[](difference_type n) const; { return ptr_[n]; }

// Increment/decrement
    ra_iter&        operator++()    {                         ++ptr_; return *this; }
    ra_iter         operator++(int) { ra_iter<T> tmp = *this; ++ptr_; return tmp;   }
    ra_iter&        operator--();   {                         --ptr_; return *this; }
    ra_iter         operator--(int) { ra_iter<T> tmp = *this; --ptr_; return tmp;   }

// Arithmetic
    ra_iter&        operator+=(difference_type n) { ptr_ += n; return *this; }
    ra_iter&        operator-=(difference_type n) { ptr_ -= n; return *this; }
    ra_iter         operator+(difference_type n) const {
                   ra_iter<T> result(*this); result.ptr_ += n; return result; }
    ra_iter         operator-(difference_type n) const {
                   ra_iter<T> result(*this); result.ptr_ -= n; return result; }

// Comparison
    template <class U>
    friend bool operator==(const ra_iter<U>& a, const ra_iter<U>& b);
    template <class U>
    friend bool operator!=(const ra_iter<U>& a, const ra_iter<U>& b);
    template <class U>
    friend bool operator< (const ra_iter<U>& a, const ra_iter<U>& b);
    template <class U>
    friend bool operator> (const ra_iter<U>& a, const ra_iter<U>& b);
    template <class U>
    friend bool operator<=(const ra_iter<U>& a, const ra_iter<U>& b);
    template <class U>
    friend bool operator>=(const ra_iter<U>& a, const ra_iter<U>& b);

private:
    pointer ptr_;
};

template <class T>
inline bool operator==(const ra_iter<T>& a, const ra_iter<T>& b)
{
    return a.ptr_ == b.ptr_;
}

template <class T>
inline bool operator!=(const ra_iter<T>& a, const ra_iter<T>& b)
{
    return a.ptr_ != b.ptr_;
}

template <class T>
inline bool operator<(const ra_iter<T>& a, const ra_iter<T>& b)
{
    return a.ptr_ < b.ptr_;
}

template <class T>
inline bool operator>(const ra_iter<T>& a, const ra_iter<T>& b)
{
    return a.ptr_ > b.ptr_;
}

template <class T>
inline bool operator<=(const ra_iter<T>& a, const ra_iter<T>& b)
{
    return a.ptr_ <= b.ptr_;
}

template <class T>
inline bool operator>=(const ra_iter<T>& a, const ra_iter<T>& b)
{
    return a.ptr_ >= b.ptr_;
}

} // namespace ft
