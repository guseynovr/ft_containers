#ifndef TYPE_TRAITS
#define TYPE_TRAITS

namespace ft {

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
template<> struct is_integral<short>              : public true_type { };
template<> struct is_integral<unsigned short>     : public true_type { };
template<> struct is_integral<int>                : public true_type { };
template<> struct is_integral<unsigned int>       : public true_type { };
template<> struct is_integral<long>               : public true_type { };
template<> struct is_integral<unsigned long>      : public true_type { };
template<> struct is_integral<long long>          : public true_type { };
template<> struct is_integral<unsigned long long> : public true_type { };

}; // namespace ft

#endif // TYPE_TRAITS