#ifndef MAP_H_
#define MAP_H_

#include "tree.hpp"
#include "utility.hpp"

namespace ft
{

template <class Key, class T, class Compare = ft::less<Key>,
          class Allocator = std::allocator<ft::pair<const Key, T> > >
class map
{
public:
    typedef Key                    key_type;
    typedef T                      mapped_type;
    typedef ft::pair<const Key, T> value_type;
    typedef Compare                key_compare;
    typedef Allocator              allocator_type;

private:
    typedef typename allocator_type::template
        rebind<value_type>::other _pair_alloc_type;

    typedef _Rb_tree<Key, value_type, _Select1st<value_type>,
        Compare, _pair_alloc_type> _tree_type;

public:
    typedef typename _pair_alloc_type::pointer          pointer;
    typedef typename _pair_alloc_type::const_pointer    const_pointer;
    typedef typename _pair_alloc_type::reference        reference;
    typedef typename _pair_alloc_type::const_reference  const_reference;
    typedef typename _tree_type::iterator               iterator;
    typedef typename _tree_type::const_iterator         const_iterator;
    typedef typename _tree_type::reverse_iterator       reverse_iterator;
    typedef typename _tree_type::const_reverse_iterator const_reverse_iterator;
    typedef typename _tree_type::size_type              size_type;
    typedef typename _tree_type::difference_type        difference_type;

    class value_compare : ft::binary_function<value_type, value_type, bool>
    {
    private:
        friend class map<Key, T, Compare, Allocator>;

    protected:
        Compare comp;

        value_compare(Compare c)
            : comp(c)
        {}

    public:
        bool operator()(const value_type& lhs,
                   const value_type& rhs) const
        {
            return comp(lhs.first, rhs.first);
        }
    };

public:
    /// default
    explicit
    map (const key_compare& comp = key_compare(),
         const allocator_type& alloc = allocator_type())
        : _tree(comp, alloc)
    {}

    /// range
    template <class InputIterator>
    map (InputIterator first, InputIterator last,
         const key_compare& comp = key_compare(),
         const allocator_type& alloc = allocator_type())
        : _tree(first, last, comp, alloc)
    {}

    /// copy
    map (const map& other)
        : _tree(other._tree)
    {}

    ~map() {}

public:
    map&
    operator=(const map& other)
    {
        if (this != &other)
            _tree = other._tree;
        return *this;
    }

    allocator_type
    get_allocator() const
    { return allocator_type(); }

    /// element access
    mapped_type&
    operator[] (const key_type& k)
    {
        return _tree.insert(ft::make_pair(k, mapped_type())).first->second;
    }

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
    insert (InputIterator first, InputIterator last)
    { return _tree.insert(first, last); }

    void
    erase(iterator pos)
    { _tree.erase(pos); }

    void
    erase(iterator first, iterator last)
    { _tree.erase(first, last); }

    size_type
    erase(const Key& key)
    { return _tree.erase(key); }

    void
    swap(map& other)
    { _tree.swap(other._tree); }

    /// lookup
    size_type
    count(const Key& key) const
    { return _tree.count(key); }

    iterator
    find(const Key& key)
    { return _tree.find(key); }

    const_iterator
    find(const Key& key) const
    { return _tree.find(key); }

    ft::pair<iterator,iterator>
    equal_range(const Key& key)
    { return _tree.equal_range(key); }

    ft::pair<const_iterator,const_iterator>
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

    /// observers
    key_compare
    key_comp() const
    { return _tree.key_comp(); }

    value_compare
    value_comp() const
    { return value_compare(key_comp()); }

    /// non-member functions
    friend bool
    operator==(const map& lhs, const map& rhs)
    { return lhs._tree == rhs._tree; }

    friend bool
    operator!=(const map& lhs, const map& rhs)
    { return !(lhs == rhs); }

    friend bool
    operator<(const map& lhs, const map& rhs)
    { return lhs._tree < rhs._tree; }

    friend bool
    operator<=(const map& lhs, const map& rhs)
    { return lhs._tree <= rhs._tree; }

    friend bool
    operator>(const map& lhs, const map& rhs)
    { return !(lhs <= rhs); }

    friend bool
    operator>=(const map& lhs, const map& rhs)
    { return !(lhs < rhs); }

    friend void
    swap(map& lhs, map& rhs)
    { lhs.swap(rhs); }

private:
    _tree_type _tree;
};

}

#endif // MAP_H_
