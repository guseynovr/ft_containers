#ifndef STACK_H
#define STACK_H

#include "vector.hpp"
// #include <stack>

namespace ft {

template <class T, class Container = vector<T> >
class stack
{
public:
    typedef Container                      container_type;
    typedef typename Container::value_type value_type;
    typedef typename Container::size_type  size_type;
    typedef typename Container::reference  reference;
    typedef typename Container::const_reference  const_reference;

// Constructor
    explicit stack (const container_type& ctnr = container_type()) : c(ctnr) {}

// Member functions
    bool      empty() const { return c.empty(); }
    size_type size() const  { return c.size(); }

    value_type&       top()       { return c.back(); }
    const value_type& top() const { return c.back(); }

    void      push(const value_type& val) { c.push_back(val); }
    void      pop()                       { c.pop_back(); }

    // template <class T, class Container>
    friend bool operator==(const stack<T,Container>& lhs, const stack<T,Container>& rhs)
        { return lhs.c == rhs.c; }
    // template <class T, class Container>
    friend bool operator!=(const stack<T,Container>& lhs, const stack<T,Container>& rhs)
        { return lhs.c != rhs.c; }
    // template <class T, class Container>
    friend bool operator< (const stack<T,Container>& lhs, const stack<T,Container>& rhs)
        { return lhs.c < rhs.c; }
    // template <class T, class Container>
    friend bool operator<=(const stack<T,Container>& lhs, const stack<T,Container>& rhs)
        { return lhs.c <= rhs.c; }
    // template <class T, class Container>
    friend bool operator> (const stack<T,Container>& lhs, const stack<T,Container>& rhs)
        { return lhs.c > rhs.c; }
    // template <class T, class Container>
    friend bool operator>=(const stack<T,Container>& lhs, const stack<T,Container>& rhs)
        { return lhs.c >= rhs.c; }

protected:
    Container c;
};

}; // namespace ft

#endif // STACK_H
