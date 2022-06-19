#ifndef ITERATOR_H
#define ITERATOR_H

// #include <iterator>
namespace ft {

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag       : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template <class Category, class T, class Distance = ptrdiff_t,
          class Pointer = T*, class Reference = T&>
struct iterator {
    typedef T         value_type;
    typedef Distance  difference_type;
    typedef Pointer   pointer;
    typedef Reference reference;
    typedef Category  iterator_category;
};

template <class Iter>
struct std::iterator_traits<youriterator> {        
    typedef ???? difference_type; //almost always ptrdiff_t
    typedef ???? value_type; //almost always T
    typedef ???? reference; //almost always T& or const T&
    typedef ???? pointer; //almost always T* or const T*
    typedef ???? iterator_category;  //usually std::forward_iterator_tag or similar
};

} // namespace ft
