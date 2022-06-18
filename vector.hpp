
namespace ft {

template < class T, class Alloc = allocator<T> >
class vector {

    template< class T, class Alloc >
        bool operator==( const std::vector<T,Alloc>& lhs,
                        const std::vector<T,Alloc>& rhs );

    template< class T, class Alloc >
    constexpr bool operator==(const std::vector<T,Alloc>& lhs,
                              const std::vector<T,Alloc>& rhs );

    template< class T, class Alloc >
    bool operator!=(const std::vector<T,Alloc>& lhs,
                    const std::vector<T,Alloc>& rhs );

    template< class T, class Alloc >
    bool operator<(const std::vector<T,Alloc>& lhs,
                   const std::vector<T,Alloc>& rhs );

    template< class T, class Alloc >
    bool operator<=(const std::vector<T,Alloc>& lhs,
                    const std::vector<T,Alloc>& rhs );

    template< class T, class Alloc >
    bool operator>(const std::vector<T,Alloc>& lhs,
                   const std::vector<T,Alloc>& rhs );

    template< class T, class Alloc >
    bool operator>=(const std::vector<T,Alloc>& lhs,
                    const std::vector<T,Alloc>& rhs );
};

} //namespace ft
