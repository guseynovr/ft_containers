#ifndef VECTOR_H
#define VECTOR_H

#include <memory>
#include <vector>

namespace ft {
template <class T, class Allocator = std::allocator<T> >
class vector
{
public:
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

    bool operator==(const std::vector<T,Alloc>& lhs, const std::vector<T,Alloc>& rhs);
    bool operator!=(const std::vector<T,Alloc>& lhs, const std::vector<T,Alloc>& rhs);
    bool operator< (const std::vector<T,Alloc>& lhs, const std::vector<T,Alloc>& rhs);
    bool operator<=(const std::vector<T,Alloc>& lhs, const std::vector<T,Alloc>& rhs);
    bool operator> (const std::vector<T,Alloc>& lhs, const std::vector<T,Alloc>& rhs);
    bool operator>=(const std::vector<T,Alloc>& lhs, const std::vector<T,Alloc>& rhs);
};

} // namespace ft

#endif // VECTOR_H
