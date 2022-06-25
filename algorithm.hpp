#ifndef ALGORITHM_H
#define ALGORITHM_H

namespace ft {

template <class T>
inline void swap (T& a, T& b)
{
    T temp = a;
    a = b;
    b = temp;
}

template <class InputIterator1, class InputIterator2>
inline bool lexicographical_compare (InputIterator1 first1, InputIterator1 last1,
                                     InputIterator2 first2, InputIterator2 last2)
{
    while (first1!=last1) {
        if (first2==last2 || *first2<*first1) return false;
        else if (*first1<*first2) return true;
        ++first1; ++first2;
    }
    return (first2!=last2);
}

template <class InputIterator1, class InputIterator2, class Compare>
inline bool lexicographical_compare (InputIterator1 first1, InputIterator1 last1,
                                     InputIterator2 first2, InputIterator2 last2,
                                     Compare comp)
{

}

}; // namescape ft

#endif // ALGORITM_H
