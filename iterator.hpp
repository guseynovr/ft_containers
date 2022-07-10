#ifndef ITERATOR_H
#define ITERATOR_H

// #include <iterator>
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
    typedef iterator        iterator_type;
    typedef T               value_type;
    typedef Distance        difference_type;
    typedef Pointer         pointer;
    typedef const Pointer   const_pointer;
    typedef Reference       reference;
    typedef const Reference const_reference;
    typedef Category        iterator_category;
};

template <class Iter>
struct iterator_traits {
    typedef typename Iter::difference_type   difference_type;
    typedef typename Iter::value_type        value_type;
    typedef typename Iter::reference         reference;
    typedef typename Iter::const_reference   const_reference;
    typedef typename Iter::pointer           pointer;
    typedef typename Iter::const_pointer     const_pointer;
    typedef typename Iter::iterator_category iterator_category;
};

template <class T>
struct iterator_traits<T*>
{
    typedef std::ptrdiff_t             difference_type;
    typedef T                          value_type;
    typedef T*                         pointer;
    typedef const T*                   const_pointer;
    typedef T&                         reference;
    typedef const T&                   const_reference;
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
    typedef ra_iter                                                             iterator_type;
    typedef typename iterator<random_access_iterator_tag, T>::iterator_category iterator_category;
    typedef typename iterator<random_access_iterator_tag, T>::value_type        value_type;
    typedef typename iterator<random_access_iterator_tag, T>::difference_type   difference_type;
    typedef typename iterator<random_access_iterator_tag, T>::pointer           pointer;
    typedef typename iterator<random_access_iterator_tag, T>::const_pointer     const_pointer;
    typedef typename iterator<random_access_iterator_tag, T>::reference         reference;
    typedef typename iterator<random_access_iterator_tag, T>::const_reference   const_reference;

    ra_iter() : ptr_(NULL) {}
    ra_iter(pointer ptr) : ptr_(ptr) {}
    ra_iter(const ra_iter& rai) : ptr_(rai.ptr_) {}
    ~ra_iter() {}

    ra_iter& operator=(const ra_iter& rai) {
        if (this != &rai) {
            ptr_ = rai.ptr_;
        }
        return *this;
    }

// Dereference
    reference       operator*()                         { return *ptr_;   }
    const reference operator*() const                   { return *ptr_;   }
    pointer         operator->()                        { return ptr_;    }
    const pointer   operator->() const                  { return ptr_;    }
    reference       operator[](difference_type n)       { return ptr_[n]; }
    const reference operator[](difference_type n) const { return ptr_[n]; }

// Increment/decrement
    ra_iter& operator++()    {                         ++ptr_; return *this; }
    ra_iter& operator--()    {                         --ptr_; return *this; }
    ra_iter  operator++(int) { ra_iter<T> tmp = *this; ++ptr_; return tmp;   }
    ra_iter  operator--(int) { ra_iter<T> tmp = *this; --ptr_; return tmp;   }

// Arithmetic
    ra_iter& operator+=(difference_type n) { ptr_ += n; return *this;  }
    ra_iter& operator-=(difference_type n) { ptr_ -= n; return *this;  }
    ra_iter  operator+(difference_type n) const {
            ra_iter<T> result(*this); result.ptr_ += n; return result; }
    ra_iter  operator-(difference_type n) const {
            ra_iter<T> result(*this); result.ptr_ -= n; return result; }

// Comparison / non-member overloads
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

    template <class U>
    friend ra_iter<U> operator+(typename ra_iter<U>::difference_type n,
                                const ra_iter<U>& it);
    template <class U>
    friend typename ra_iter<U>::difference_type operator-(const ra_iter<U>& lhs,
                                                          const ra_iter<U>& rhs);

private:
    pointer ptr_;
};

template <class Iter>
class reverse_iterator
{
public:
    typedef Iter                                                iterator_type;
    typedef typename iterator_traits<Iter>::iterator_category   iterator_category;
    typedef typename iterator_traits<Iter>::value_type          value_type;
    typedef typename iterator_traits<Iter>::difference_type     difference_type;
    typedef typename iterator_traits<Iter>::pointer             pointer;
    typedef typename iterator_traits<Iter>::const_pointer       const_pointer;
    typedef typename iterator_traits<Iter>::reference           reference;
    typedef typename iterator_traits<Iter>::const_reference     const_reference;

// Constructors
    reverse_iterator()                                    : it_() {}
    reverse_iterator(iterator_type x)                     : it_(x) {}
    reverse_iterator(const reverse_iterator<Iter>& other) : it_(other.it_) {}

    reverse_iterator& operator=(const reverse_iterator<Iter>& other) {
        if (this != &other) {
            it_ = other.it_;
        }
        return *this;
    }

// Dereference
    reference         operator*() const                   { Iter temp(it_); return *--temp; }
    pointer           operator->() const                  { return &(operator*()); }
    reference         operator[](difference_type n) const { return it_[-n - 1];    }

// Increment/decrement
    reverse_iterator& operator++()    {  --it_; return *this; }
    reverse_iterator& operator--()    {  ++it_; return *this; }
    reverse_iterator  operator++(int) {
        reverse_iterator<Iter> tmp = *this; --it_; return tmp;   }
    reverse_iterator  operator--(int) {
        reverse_iterator<Iter> tmp = *this; ++it_; return tmp;   }

// Arithmetic
    reverse_iterator  operator+(difference_type n) const
                                { return reverse_iterator<Iter>(it_ - n); }
    reverse_iterator  operator-(difference_type n) const
                                { return reverse_iterator<Iter>(it_ + n); }
    reverse_iterator& operator+=(difference_type n) { it_ -= n; return *this; }
    reverse_iterator& operator-=(difference_type n) { it_ += n; return *this; }


// Comparison / non-member overloads
    template<class Iterator>
    friend bool operator==(const reverse_iterator<Iterator>& lhs,
                           const reverse_iterator<Iterator>& rhs);
    template<class Iterator>
    friend bool operator!=(const reverse_iterator<Iterator>& lhs,
                           const reverse_iterator<Iterator>& rhs);
    template<class Iterator>
    friend bool operator< (const reverse_iterator<Iterator>& lhs,
                           const reverse_iterator<Iterator>& rhs);
    template<class Iterator>
    friend bool operator<=(const reverse_iterator<Iterator>& lhs,
                           const reverse_iterator<Iterator>& rhs);
    template<class Iterator>
    friend bool operator> (const reverse_iterator<Iterator>& lhs,
                           const reverse_iterator<Iterator>& rhs);
    template<class Iterator>
    friend bool operator>=(const reverse_iterator<Iterator>& lhs,
                           const reverse_iterator<Iterator>& rhs);
    template <class Iterator>
    friend reverse_iterator<Iterator> operator+(
        typename reverse_iterator<Iterator>::difference_type n,
        const reverse_iterator<Iterator>& rev_it);

    template <class Iterator>
    friend typename reverse_iterator<Iterator>::difference_type operator-(
        const reverse_iterator<Iterator>& lhs,
        const reverse_iterator<Iterator>& rhs);

protected:
    Iter it_;
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

template <class U>
inline ra_iter<U> operator+(typename ra_iter<U>::difference_type n,
                            const ra_iter<U>& it)
{
    ra_iter<U> result(it);
    result.ptr_ += n;
    return result;
}

template <class U>
inline typename ra_iter<U>::difference_type operator-(const ra_iter<U>& lhs,
                                                      const ra_iter<U>& rhs)
{
    return lhs.ptr_ - rhs.ptr_;
}

/***** Reverse iterator *****/

template<class Iterator>
bool operator==(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
{
    return lhs.it_ == rhs.it_;
}

template<class Iterator>
bool operator!=(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
{
    return lhs.it_ != rhs.it_;
}

template<class Iterator>
bool operator<(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
{
    return lhs.it_ > rhs.it_;
}

template<class Iterator>
bool operator<=(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
{
    return lhs.it_ >= rhs.it_;
}

template<class Iterator>
bool operator>(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
{
    return lhs.it_ < rhs.it_;
}

template<class Iterator>
bool operator>=(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
{
    return lhs.it_ <= rhs.it_;
}

template <class Iterator>
reverse_iterator<Iterator> operator+(
typename reverse_iterator<Iterator>::difference_type n, const reverse_iterator<Iterator>& rev_it)
{
    return reverse_iterator<Iterator>(rev_it.it_ - n);
}

template <class Iterator>
typename reverse_iterator<Iterator>::difference_type operator-(
const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
{
    return rhs.it_ - lhs.it_;
}

}; // namespace ft

#endif // ITERATOR_H
