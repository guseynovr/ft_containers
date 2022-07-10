#ifndef SET_H_
#define SET_H_

#include "tree.hpp"
#include "utility.hpp"

namespace ft
{

template <class Key, class Compare = ft::less<Key>,
          class Allocator = std::allocator<Key> >
class set
{
public:
    typedef Key       key_type;
    typedef Key       value_type;
    typedef Compare   key_compare;
    typedef Compare   value_compare;
    typedef Allocator allocator_type;

private:
    typedef typename allocator_type::template
        rebind<Key>::other _key_alloc_type;

    typedef _Rb_tree<Key, Key, _Identity<Key>,
        Compare, _key_alloc_type> _tree_type;

public:
    typedef typename _key_alloc_type::pointer           pointer;
    typedef typename _key_alloc_type::const_pointer     const_pointer;
    typedef typename _key_alloc_type::reference         reference;
    typedef typename _key_alloc_type::const_reference   const_reference;
    typedef typename _tree_type::iterator               iterator;
    typedef typename _tree_type::const_iterator         const_iterator;
    typedef typename _tree_type::reverse_iterator       reverse_iterator;
    typedef typename _tree_type::const_reverse_iterator const_reverse_iterator;
    typedef typename _tree_type::size_type              size_type;
    typedef typename _tree_type::difference_type        difference_type;

public:
    /// default
    explicit
    set (const key_compare& comp = key_compare(),
         const allocator_type& alloc = allocator_type())
        : _tree(comp, alloc)
    {}

    /// range
    template <class InputIterator>
    set (InputIterator first, InputIterator last,
         const key_compare& comp = key_compare(),
         const allocator_type& alloc = allocator_type())
        : _tree(first, last, comp, alloc)
    {}

    /// copy
    set (const set& x)
        : _tree(x._tree)
    {}

    ~set() {}

public:
    set&
    operator=(const set& other)
    {
        if (this != &other)
            _tree = other._tree;
        return *this;
    }

    allocator_type
    get_allocator() const
    { return allocator_type(); }

    /// iterators
    iterator
    begin()
    { return _tree.begin(); }

    const_iterator
    begin() const
    { return _tree.begin(); }

    iterator
    end()
    { return _tree.end(); }

    const_iterator
    end() const
    { return _tree.end(); }

    reverse_iterator
    rbegin()
    { return _tree.rbegin(); }

    const_reverse_iterator
    rbegin() const
    { return _tree.rbegin(); }

    reverse_iterator
    rend()
    { return _tree.rend(); }

    const_reverse_iterator
    rend() const
    { return _tree.rend(); }

    /// capacity
    bool
    empty() const
    { return _tree.empty(); }

    size_type
    size() const
    { return _tree.size(); }

    size_type
    max_size() const
    { return _tree.max_size(); }

    /// modifiers
    void
    clear()
    { _tree.clear(); }

    ft::pair<iterator, bool>
    insert(const value_type& value)
    { return _tree.insert(value); }

    iterator
    insert(iterator hint, const value_type& value)
    { return _tree.insert(hint, value); }

    template <class InputIterator>
    void
    insert(InputIterator first, InputIterator last)
    { _tree.insert(first, last); }

    void
    erase(iterator pos)
    { _tree.erase(pos); }

    void
    erase(iterator first, iterator last)
    { _tree.erase(first, last); }

    size_type
    erase(const key_type& key)
    { return _tree.erase(key); }

    void
    swap(set& other)
    { _tree.swap(other._tree); }

    /// lookup
    size_type
    count(const Key& key)
    { return _tree.count(key); }

    iterator
    find(const Key& key)
    { return _tree.find(key); }

    const_iterator
    find(const Key& key) const
    { return _tree.find(key); }

    ft::pair<iterator, iterator>
    equal_range(const Key& key)
    { return _tree.equal_range(key); }

    ft::pair<const_iterator, const_iterator>
    equal_range(const Key& key) const
    { return _tree.equal_range(key); }

    iterator
    lower_bound(const Key& key)
    { return _tree.lower_bound(key); }

    const_iterator
    lower_bound(const Key& key) const
    { return _tree.lower_bound(key); }

    iterator
    upper_bound(const Key& key)
    { return _tree.upper_bound(key); }

    const_iterator
    upper_bound(const Key& key) const
    { return _tree.upper_bound(key); }

    key_compare
    key_comp() const
    { return _tree.key_comp(); }

    value_compare
    value_comp() const
    { return _tree.key_comp(); }

    /// non-member functions
    friend bool
    operator==(const set& lhs, const set& rhs)
    { return lhs._tree == rhs._tree; }

    friend bool
    operator!=(const set& lhs, const set& rhs)
    { return !(lhs == rhs); }

    friend bool
    operator<(const set& lhs, const set& rhs)
    { return lhs._tree < rhs._tree; }

    friend bool
    operator<=(const set& lhs, const set& rhs)
    { return lhs._tree <= rhs._tree; }

    friend bool
    operator>(const set& lhs, const set& rhs)
    { return !(lhs <= rhs); }

    friend bool
    operator>=(const set& lhs, const set& rhs)
    { return !(lhs < rhs); }

    friend void
    swap(set& lhs, set& rhs)
    { lhs.swap(rhs); }

private:
    _tree_type _tree;
};

} // namespace ft

#endif // SET_H_
