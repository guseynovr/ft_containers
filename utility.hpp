#ifndef UTILITY_H
#define UTILITY_H

// #include <utility>

namespace ft {

template <class T1, class T2> struct pair
{
    typedef T1 first_type;
    typedef T2 second_type;

    T1 first;
    T2 second;

    pair() : first(), second() {}
    template<class U, class V>
    pair(const pair<U,V>& pr) : first(pr.first), second(pr.second) {}
    pair(const first_type& a, const second_type& b) : first(a), second(b) {}

    pair& operator=(const pair& pr) {
        if (this == &pr) return *this;
        first = pr.first; second = pr.second;
        return *this;
    }

    template<class U1, class U2>
    friend bool operator==(const pair<U1,U2>& lhs, const pair<U1,U2>& rhs);
    template<class U1, class U2>
    friend bool operator!=(const pair<U1,U2>& lhs, const pair<U1,U2>& rhs);
    template<class U1, class U2>
    friend bool operator< (const pair<U1,U2>& lhs, const pair<U1,U2>& rhs);
    template<class U1, class U2>
    friend bool operator<=(const pair<U1,U2>& lhs, const pair<U1,U2>& rhs);
    template<class U1, class U2>
    friend bool operator> (const pair<U1,U2>& lhs, const pair<U1,U2>& rhs);
    template<class U1, class U2>
    friend bool operator>=(const pair<U1,U2>& lhs, const pair<U1,U2>& rhs);
};

template <class T1,class T2>
pair<T1,T2> make_pair(T1 x, T2 y)
{
    return pair<T1, T2>(x, y);
}

template<class T1, class T2>
inline bool operator==(const pair<T1,T2>& lhs, const pair<T1,T2>& rhs)
{
    return lhs.first == rhs.first
        && lhs.second == rhs.second;
}

template<class T1, class T2>
inline bool operator!=(const pair<T1,T2>& lhs, const pair<T1,T2>& rhs)
{
    return !(lhs == rhs);
}

template<class T1, class T2>
inline bool operator<(const pair<T1,T2>& lhs, const pair<T1,T2>& rhs)
{
    if (lhs.first < rhs.first) return true;
    if (rhs.first < lhs.first) return false;
    if (lhs.second < rhs.second) return true;
    return false;
}

template<class T1, class T2>
inline bool operator<=(const pair<T1,T2>& lhs, const pair<T1,T2>& rhs)
{
    return !(rhs < lhs);
}

template<class T1, class T2>
inline bool operator>(const pair<T1,T2>& lhs, const pair<T1,T2>& rhs)
{
    return rhs < lhs;
}

template<class T1, class T2>
inline bool operator>=(const pair<T1,T2>& lhs, const pair<T1,T2>& rhs)
{
    return !(lhs < rhs);
}

//-------------TYPE TRAITS

template<bool, typename T = void>
struct enable_if { };
template<class T>
struct enable_if<true, T> { typedef T type; };

// integral_constant
template<typename T, T val>
struct integral_constant
{
    static const bool value = val;
    typedef T value_type;
    typedef integral_constant type;
    operator value_type() const { return value; }
};

// The type used as a compile-time boolean with true value.
typedef integral_constant<bool, true>     true_type;
// The type used as a compile-time boolean with false value.
typedef integral_constant<bool, false>    false_type;

template<class T> struct is_integral : public false_type { };
// template<typename> struct integral_constant : public false_type { };

// template <class T> struct is_integral : public integral_constant<T, false> {};

template<> struct is_integral<bool>               : public true_type { };
template<> struct is_integral<char>               : public true_type { };
template<> struct is_integral<signed char>        : public true_type { };
template<> struct is_integral<unsigned char>      : public true_type { };
template<> struct is_integral<wchar_t>            : public true_type { };
template<> struct is_integral<char16_t>           : public true_type { };
template<> struct is_integral<short>              : public true_type { };
template<> struct is_integral<unsigned short>     : public true_type { };
template<> struct is_integral<int>                : public true_type { };
template<> struct is_integral<unsigned int>       : public true_type { };
template<> struct is_integral<long>               : public true_type { };
template<> struct is_integral<unsigned long>      : public true_type { };
template<> struct is_integral<long long>          : public true_type { };
template<> struct is_integral<unsigned long long> : public true_type { };


//-----FUNCTIONAL
template <typename _Arg, typename _Result>
struct unary_function
{
    typedef _Arg    argument_type;
    typedef _Result result_type;
};

template <typename _Arg1, typename _Arg2, typename _Result>
struct binary_function
{
    typedef _Arg1   first_argument_type;
    typedef _Arg2   second_argument_type;
    typedef _Result result_type;
};

template <typename _Tp>
struct _Identity : public unary_function<_Tp, _Tp>
{
    _Tp&
    operator()(_Tp& x)
    {
        return x;
    }

    const _Tp&
    operator()(const _Tp& x) const
    {
        return x;
    }
};

template <typename _Tp>
struct _Identity<const _Tp> : _Identity<_Tp> { };

template <typename _Pair>
struct _Select1st : public unary_function<_Pair, typename _Pair::first_type>
{
    typename _Pair::first_type&
    operator()(_Pair& x)
    {
        return x.first;
    }

    const typename _Pair::first_type&
    operator()(const _Pair& x) const
    {
        return x.first;
    }
};

template <class _Tp>
struct less : public binary_function<_Tp, _Tp, bool>
{
    bool
    operator()(const _Tp& lhs,
               const _Tp& rhs) const
    {
        return lhs < rhs;
    }
};

//-----------ALGORITHM
template <class T>
inline void swap (T& a, T& b)
{
    T temp = a;
    a = b;
    b = temp;
}

template <class InputIterator1, class InputIterator2>
inline bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
                                    InputIterator2 first2, InputIterator2 last2)
{
    // while (first1!=last1) {
    //     if (first2==last2 || *first2<*first1) return false;
    //     else if (*first1<*first2) return true;
    //     ++first1; ++first2;
    // }
    // return (first2!=last2);
    for (; first1 != last1 && first2 != last2; ++first1, (void)++first2)
    {
        if (*first1 < *first2) return true;
        if (*first2 < *first1) return false;
    }
    return first1 == last1 && first2 != last2;
}

template <class InputIterator1, class InputIterator2, class Compare>
inline bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
                                    InputIterator2 first2, InputIterator2 last2,
                                    Compare comp)
{
    for (; first1 != last1 && first2 != last2; ++first1, (void)++first2)
    {
        if (comp(*first1, *first2)) return true;
        if (comp(*first2, *first1)) return false;
    }
    return first1 == last1 && first2 != last2;
}

template <class InputIterator1, class InputIterator2>
inline bool equal(InputIterator1 first1, InputIterator1 last1,
                  InputIterator2 first2/* , InputIterator2 last2 */)
{
    for (; first1 != last1; ++first1, ++first2) {
        if (*first1 != *first2) return false;
    }
    return true;
}

template <class InputIterator1, class InputIterator2, class BinaryPredicate>
inline bool equal(InputIterator1 first1, InputIterator1 last1,
                  InputIterator2 first2, BinaryPredicate pred)
{
    for (; first1 != last1; ++first1, ++first2) {
        if (!pred(*first1, *first2)) return false;
    }
    return true;
}

}; // namespace ft

#endif // UTILITY_H
