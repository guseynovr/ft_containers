#ifndef TREE_H
#define TREE_H

#include <cstddef>
#include <memory>
#include <iterator>

#include <cassert>

#include "utility.hpp"
#include "iterator.hpp"
// #include "../algorithm.hpp"

namespace ft
{

enum _Rb_tree_color
{
    RED,
    BLACK
};

struct _Rb_tree_node_base
{
    typedef _Rb_tree_node_base*       base_ptr;
    typedef const _Rb_tree_node_base* const_base_ptr;

// clang implementation use tail padding
#ifdef __clang__
    _Rb_tree_node_base() {}
#endif // __clang__

    base_ptr       parent;
    base_ptr       left;
    base_ptr       right;
    _Rb_tree_color color;

    static base_ptr minimum(base_ptr x)
    {
        while (x->left)
            x = x->left;
        return x;
    }

    static const_base_ptr minimum(const_base_ptr x)
    {
        while (x->left)
            x = x->left;
        return x;
    }

    static base_ptr maximum(base_ptr x)
    {
        while (x->right)
            x = x->right;
        return x;
    }

    static const_base_ptr maximum(const_base_ptr x)
    {
        while (x->right)
            x = x->right;
        return x;
    }

};

struct _Rb_tree_header
{
    _Rb_tree_node_base header;
    size_t             count;

    _Rb_tree_header()
    {
        header.color = RED;
        reset();
    }

    void
    move_data(_Rb_tree_header& from)
    {
        header.color = from.header.color;
        header.parent = from.header.parent;
        header.left = from.header.left;
        header.right = from.header.right;
        if (header.parent)
            header.parent->parent = &header;
        count = from.count;

        from.reset();
    }

    void
    reset()
    {
        header.parent = NULL;
        header.left = &header;
        header.right = &header;
        count = 0;
    }
};

template <typename _Tp>
_Tp* addressof(_Tp& arg)
{
    return reinterpret_cast<_Tp*>(
                &const_cast<char&>(
                reinterpret_cast<const volatile char&>(arg)));
}

template <typename Val>
struct _Rb_tree_node : public _Rb_tree_node_base
{
    typedef _Rb_tree_node<Val>*       node_ptr;
    typedef const _Rb_tree_node<Val>* const_node_ptr;

    Val value_field;

    Val*
    val_ptr()
    {
        return ft::addressof(value_field);
    }

    const Val*
    val_ptr() const
    {
        return ft::addressof(value_field);
    }
};

_Rb_tree_node_base*
_Rb_tree_increment(_Rb_tree_node_base* x) throw();

const _Rb_tree_node_base*
_Rb_tree_increment(const _Rb_tree_node_base* x) throw();

_Rb_tree_node_base*
_Rb_tree_decrement(_Rb_tree_node_base* x) throw();

const _Rb_tree_node_base*
_Rb_tree_decrement(const _Rb_tree_node_base* x) throw();

template <typename T>
struct _Rb_tree_iterator
{
    typedef T        value_type;
    typedef T&       reference;
    typedef const T& const_reference;
    typedef T*       pointer;
    typedef const T* const_pointer;

    typedef std::bidirectional_iterator_tag iterator_category;
    typedef ptrdiff_t                       difference_type;

    typedef _Rb_tree_iterator<T>         self;
    typedef _Rb_tree_node_base::base_ptr base_ptr;
    typedef _Rb_tree_node<T>*            node_ptr;

    _Rb_tree_iterator()
    {}

    explicit
    _Rb_tree_iterator(base_ptr x)
    : _node(x) {}

    _Rb_tree_iterator(const _Rb_tree_iterator& _it)
    : _node(_it._node) {}

    _Rb_tree_iterator&
    operator=(const _Rb_tree_iterator& _it)
    {
        if (this != &_it)
        {
            _node = _it._node;
        }
        return *this;
    }

    reference
    operator*() const
    {
        return *static_cast<node_ptr>(_node)->val_ptr();
    }

    pointer
    operator->() const
    {
        return static_cast<node_ptr>(_node)->val_ptr();
    }

    self&
    operator++()
    {
        _node = _Rb_tree_increment(_node);
        return *this;
    }

    self
    operator++(int)
    {
        self tmp = *this;
        _node = _Rb_tree_increment(_node);
        return tmp;
    }

    self&
    operator--()
    {
        _node = _Rb_tree_decrement(_node);
        return *this;
    }

    self
    operator--(int)
    {
        self tmp = *this;
        _node = _Rb_tree_decrement(_node);
        return tmp;
    }

    friend bool
    operator==(const self& lhs, const self& rhs)
    {
        return lhs._node == rhs._node;
    }

    friend bool
    operator!=(const self& lhs, const self& rhs)
    {
        return lhs._node != rhs._node;
    }

    base_ptr _node;
};

template <typename T>
struct _Rb_tree_const_iterator
{
    typedef T        value_type;
    typedef const T& reference;
    typedef const T* pointer;

    typedef _Rb_tree_iterator<T> iterator;

    typedef std::bidirectional_iterator_tag iterator_category;
    typedef ptrdiff_t                       difference_type;

    typedef _Rb_tree_const_iterator<T>         self;
    typedef _Rb_tree_node_base::const_base_ptr base_ptr;
    typedef const _Rb_tree_node<T>*            node_ptr;

    _Rb_tree_const_iterator()
    {}

    explicit
    _Rb_tree_const_iterator(base_ptr x)
    : _node(x) {}

    _Rb_tree_const_iterator(const iterator& _it)
    : _node(_it._node) {}

    reference
    operator*() const
    {
        return *static_cast<node_ptr>(_node)->val_ptr();
    }

    pointer
    operator->() const
    {
        return static_cast<node_ptr>(_node)->val_ptr();
    }

    self&
    operator++()
    {
        _node = _Rb_tree_increment(_node);
        return *this;
    }

    self
    operator++(int)
    {
        self tmp = *this;
        _node = _Rb_tree_increment(_node);
        return tmp;
    }

    self&
    operator--()
    {
        _node = _Rb_tree_decrement(_node);
        return *this;
    }

    self
    operator--(int)
    {
        self tmp = *this;
        _node = _Rb_tree_decrement(_node);
        return tmp;
    }

    friend bool
    operator==(const self& lhs, const self& rhs)
    {
        return lhs._node == rhs._node;
    }

    friend bool
    operator!=(const self& lhs, const self& rhs)
    {
        return lhs._node != rhs._node;
    }

    base_ptr _node;
};

void
_Rb_tree_insert_and_rebalance(const bool insert_left,
                              _Rb_tree_node_base* x,
                              _Rb_tree_node_base* p,
                              _Rb_tree_node_base& header) throw();

_Rb_tree_node_base*
_Rb_tree_rebalance_for_erase(_Rb_tree_node_base* const z,
                             _Rb_tree_node_base& header) throw();

template <typename Key, typename Val, typename KeyOfValue,
          typename Compare, typename Alloc = std::allocator<Val> >
class _Rb_tree
{
public:
    typedef Key                      key_type;
    typedef Val                      value_type;
    typedef size_t                   size_type;
    typedef ptrdiff_t                difference_type;
    typedef Compare                  key_compare;
    typedef Alloc                    allocator_type;
    typedef value_type&              reference;
    typedef const value_type&        const_reference;
    typedef value_type*              pointer;
    typedef const value_type*        const_pointer;

    typedef _Rb_tree_iterator<value_type>        iterator;
    typedef _Rb_tree_const_iterator<value_type>  const_iterator;
    typedef ft::reverse_iterator<iterator>       reverse_iterator;
    typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

protected:
    typedef typename _Rb_tree_node_base::base_ptr              base_ptr;
    typedef typename _Rb_tree_node_base::const_base_ptr        const_base_ptr;
    typedef typename _Rb_tree_node<value_type>::node_ptr       node_ptr;
    typedef typename _Rb_tree_node<value_type>::const_node_ptr const_node_ptr;

private:
    typedef typename Alloc::template
        rebind<_Rb_tree_node<value_type> >::other node_allocator;

public:
    /// default
    explicit
    _Rb_tree(const key_compare& comp = key_compare(),
             const allocator_type& alloc = allocator_type())
        : _header(),
        _node_pool(alloc),
        _comp(comp)
    {}

    /// range constructor
    template <class InputIterator>
    _Rb_tree(InputIterator first, InputIterator last,
             const key_compare& comp = key_compare(),
             const allocator_type& alloc = allocator_type())
        : _header(),
        _node_pool(alloc),
        _comp(comp)
    {
        insert(first, last);
    }

    /// copy constructor
    _Rb_tree(const _Rb_tree& t)
        : _header(),
        _node_pool(t._node_pool.get_allocator()),
        _comp(t._comp)
    {
        *this = t;
    }

    ~_Rb_tree()
    {
        _node_pool._destroy(_header.header.parent);
    }

public:
    _Rb_tree&
    operator=(const _Rb_tree& t)
    {
        if (this != &t)
        {
            _node_pool._free(_header.header.parent);
            if (t.empty())
                _header.reset();
            else
            {
                _header.header.parent =
                    _copy(t._header.header.parent, &_header.header);
                _header.header.left =
                    _Rb_tree_node_base::minimum(_header.header.parent);
                _header.header.right =
                    _Rb_tree_node_base::maximum(_header.header.parent);
                _header.count = t._header.count;
            }
        }
        return *this;
    }

    allocator_type get_allocator() const
    {
        return allocator_type();
    }

    /// iterators
    iterator
    begin()
    {
        return iterator(_header.header.left);
    }

    const_iterator
    begin() const
    {
        return const_iterator(_header.header.left);
    }

    iterator 
    end()
    {
        return iterator(&_header.header);
    }

    const_iterator
    end() const
    {
        return const_iterator(&_header.header);
    }

    reverse_iterator
    rbegin()
    {
        return reverse_iterator(iterator(&_header.header));
    }

    const_reverse_iterator
    rbegin() const
    {
        return reverse_iterator(iterator(&_header.header));
    }

    reverse_iterator
    rend()
    {
        return reverse_iterator(iterator(_header.header.left));
    }

    const_reverse_iterator
    rend() const
    {
        return reverse_iterator(iterator(_header.header.left));
    }

    /// capacity
    bool
    empty() const
    {
        return !_header.count;
    }

    size_type
    size() const
    {
        return _header.count;
    }

    size_type
    max_size() const
    {
        return _node_pool.get_allocator().max_size();
    }

    // modifiers
    void
    clear()
    {
        _node_pool._free(_header.header.parent);
        _header.reset();
    }

    pair<iterator, bool>
    insert(const value_type& value)
    {
        bool was_inserted;
        base_ptr x = _insert(value, was_inserted);
        return ft::make_pair(iterator(x), was_inserted);
    }

    iterator
    insert(iterator hint, const value_type& value)
    {
        return iterator(_insert_hint(hint._node, value));
    }

    template <class InputIterator>
    void
    insert(InputIterator first, InputIterator last)
    {
        bool b;
        for ( ; first != last; ++first)
            _insert(*first, b);
    }

    void
    erase(iterator pos)
    {
        _Rb_tree_node_base* n = _Rb_tree_rebalance_for_erase(
            pos._node, _header.header);
        _node_pool._free_node(n);
        _header.count--;
    }

    void
    erase(iterator first, iterator last)
    {
        while (first != last)
            erase(first++);
    }

    size_type
    erase(const Key& key)
    {
        ft::pair<iterator, iterator> range = equal_range(key);
        size_type count = 0;
        for (iterator i = range.first; i != range.second; ++count)
            erase(i++);
        return count;
    }


    void
    swap(_Rb_tree& t)
    {
        _Rb_tree_header dest = _header;
        _Rb_tree_header src = t._header;
        _header.move_data(src);
        t._header.move_data(dest);
    }

    /// lookup
    size_type
    count(const Key& key) const
    {
        size_type count = 0;
        for (const_iterator it = find(key);
                it != end();
                ++it, ++count)
            if (_comp(key, KeyOfValue()(*it)))
                break;
        return count;
    }

    iterator
    find(const Key& key)
    {
        return iterator(const_cast<base_ptr>(_find(key)));
    }

    const_iterator
    find(const Key& key) const
    {
        return const_iterator(_find(key));
    }

    ft::pair<iterator, iterator>
    equal_range(const Key& key)
    {
        return ft::make_pair(lower_bound(key), upper_bound(key));
    }

    ft::pair<const_iterator, const_iterator>
    equal_range(const Key& key) const
    {
        return ft::make_pair(lower_bound(key), upper_bound(key));
    }

    iterator
    lower_bound(const Key& key)
    {
        return iterator(const_cast<base_ptr>(_lower_bound(key)));
    }

    const_iterator
    lower_bound(const Key& key) const
    {
        return const_iterator(_lower_bound(key));
    }

    iterator
    upper_bound(const Key& key)
    {
        return iterator(const_cast<base_ptr>(_upper_bound(key)));
    }

    const_iterator
    upper_bound(const Key& key) const
    {
        return const_iterator(_upper_bound(key));
    }

    key_compare
    key_comp() const
    {
        return _comp;
    }

    /// non-member functions
    friend bool
    operator==(const _Rb_tree& lhs, const _Rb_tree& rhs)
    {
        return lhs.size() == rhs.size()
            && ft::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    friend bool
    operator!=(const _Rb_tree& lhs, const _Rb_tree& rhs)
    {
        return !(lhs == rhs);
    }

    friend bool
    operator<(const _Rb_tree& lhs, const _Rb_tree& rhs)
    {
        return ft::lexicographical_compare(lhs.begin(), lhs.end(),
                                           rhs.begin(), rhs.end());
    }

    friend bool
    operator<=(const _Rb_tree& lhs, const _Rb_tree& rhs)
    {
        return lhs == rhs || ft::lexicographical_compare(lhs.begin(), lhs.end(),
                                                         rhs.begin(), rhs.end());
    }

    friend bool
    operator>(const _Rb_tree& lhs, const _Rb_tree& rhs)
    {
        return !(lhs <= rhs);
    }

    friend bool
    operator>=(const _Rb_tree& lhs, const _Rb_tree& rhs)
    {
        return !(lhs < rhs);
    }

private:
    base_ptr
    _insert_unique(_Rb_tree_node_base* const p, const_reference val)
    {
        const bool insert_left = (p == &_header.header
            || _comp(KeyOfValue()(val),
            KeyOfValue()(*static_cast<node_ptr>(p)->val_ptr())));

        base_ptr x = _node_pool._make_node(val);
        _Rb_tree_insert_and_rebalance(insert_left, x, p, _header.header);
        _header.count++;
        return x;
    }

    base_ptr
    _insert(const_reference val, bool& was_inserted)
    {
        base_ptr x = _header.header.parent;
        base_ptr y = &_header.header;
        while (x)
        {
            y = x;
            if (_comp(KeyOfValue()(val),
                KeyOfValue()(*static_cast<node_ptr>(x)->val_ptr())))
            {
                x = x->left;
            }
            else if (_comp(KeyOfValue()(*static_cast<node_ptr>(x)->val_ptr()),
                KeyOfValue()(val)))
            {
                x = x->right;
            }
            else
            {
                was_inserted = false;
                return x;
            }
        }
        was_inserted = true;
        return _insert_unique(y, val);
    }

    base_ptr
    _insert_hint(_Rb_tree_node_base* const hint, const_reference val)
    {
        bool b;
        if (hint == &_header.header)
            return _insert(val, b);
        if (!_comp(KeyOfValue()(*static_cast<node_ptr>(hint)->val_ptr()),
            KeyOfValue()(val))
            && !_comp(KeyOfValue()(val),
            KeyOfValue()(*static_cast<node_ptr>(hint)->val_ptr())))
        {
            return hint;
        }
        if (_comp(KeyOfValue()(*static_cast<node_ptr>(
            _Rb_tree_decrement(hint))->val_ptr()), KeyOfValue()(val))
            && _comp(KeyOfValue()(val),
            KeyOfValue()(*static_cast<node_ptr>(
            _Rb_tree_increment(hint))->val_ptr())))
        {
            return _insert_unique(hint, val);
        }
        return _insert(val, b);
    }

    const_base_ptr
    _lower_bound(const Key& key) const
    {
        const_base_ptr x = _header.header.parent;
        const_base_ptr y = &_header.header;
        while (x)
        {
            if (_comp(key, KeyOfValue()(
                *static_cast<const_node_ptr>(x)->val_ptr())))
            {
                y = x;
                x = x->left;
            }
            else if (_comp(KeyOfValue()(
                *static_cast<const_node_ptr>(x)->val_ptr()), key))
            {
                x = x->right;
            }
            else
                return x;
        }
        return y;
    }

    const_base_ptr
    _upper_bound(const Key& key) const
    {
        const_base_ptr x = _header.header.parent;
        const_base_ptr y = &_header.header;
        while (x)
        {
            if (_comp(key,
                KeyOfValue()(*static_cast<const_node_ptr>(x)->val_ptr())))
            {
                y = x;
                x = x->left;
            }
            else
                x = x->right;
        }
        return y;
    }

    const_base_ptr
    _find(const Key& key) const
    {
        const_base_ptr x = _lower_bound(key);
        if (x == &_header.header
            || _comp(key,
            KeyOfValue()(*static_cast<const_node_ptr>(x)->val_ptr())))
        {
            return &_header.header;
        }
        return x;
    }

    base_ptr
    _copy(const_base_ptr x, base_ptr p)
    {
        if (!x) return NULL;
        node_ptr n = _node_pool._make_node(
            *static_cast<const_node_ptr>(x)->val_ptr());
        n->parent = p;
        n->left = _copy(x->left, n);
        n->right = _copy(x->right, n);
        return n;
    }

private:
    class _Node_pool
    {
    public:
        _Node_pool(const node_allocator& alloc)
            : _stack_ptr(NULL),
            _alloc(alloc)
        {}

        ~_Node_pool()
        {
            while (_stack_ptr)
            {
                base_ptr tmp = _stack_ptr;
                _stack_ptr = _stack_ptr->parent;
                _alloc.deallocate(static_cast<node_ptr>(tmp), 1);
            }
        }

        node_allocator get_allocator() const
        {
            return _alloc;
        }

        node_ptr
        _make_node(const_reference val)
        {
            node_ptr n;
            if (_stack_ptr)
            {
                n = static_cast<node_ptr>(_stack_ptr);
                _stack_ptr = _stack_ptr->parent;
            }
            else
                n = _alloc.allocate(1);
            allocator_type().construct(n->val_ptr(), val);
            return n;
        }

        void
        _free(base_ptr x)
        {
            if (x)
            {
                _free(x->left);
                _free(x->right);
                _free_node(x);
            }
        }

        void
        _free_node(base_ptr x)
        {
            _alloc.destroy(static_cast<node_ptr>(x));
            x->left = NULL;
            x->right = NULL;
            x->parent = _stack_ptr;
            _stack_ptr = x;
        }

        void
        _destroy(base_ptr x)
        {
            if (x)
            {
                _destroy(x->left);
                _destroy(x->right);
                _destroy_node(x);
            }
        }

        void
        _destroy_node(base_ptr x)
        {
            const node_ptr& n = static_cast<node_ptr>(x);
            _alloc.destroy(n);
            _alloc.deallocate(n, 1);
        }

    private:
        base_ptr       _stack_ptr;
        node_allocator _alloc;
    };

    _Rb_tree_header _header;
    _Node_pool       _node_pool;
    key_compare     _comp;
};


_Rb_tree_node_base*
_Rb_tree_increment(_Rb_tree_node_base* x) throw()
{
    if (x->right)
    {
        x = x->right;
        while (x->left)
            x = x->left;
    }
    else
    {
        _Rb_tree_node_base* y = x->parent;
        while (x == y->right)
        {
            x = y;
            y = y->parent;
        }
        if (x->right != y)
            x = y;
    }
    return x;
}

const _Rb_tree_node_base*
_Rb_tree_increment(const _Rb_tree_node_base* x) throw()
{
    return _Rb_tree_increment(const_cast<_Rb_tree_node_base*>(x));
}

_Rb_tree_node_base*
_Rb_tree_decrement(_Rb_tree_node_base* x) throw()
{
    if (x->color == RED && x->parent->parent == x)
        x = x->right;
    else if (x->left)
    {
        x = x->left;
        while (x->right)
            x = x->right;
    }
    else
    {
        _Rb_tree_node_base* y = x->parent;
        while (x == y->left)
        {
            x = y;
            y = y->parent;
        }
        x = y;
    }
    return x;
}

const _Rb_tree_node_base*
_Rb_tree_decrement(const _Rb_tree_node_base* x) throw()
{
    return _Rb_tree_decrement(const_cast<_Rb_tree_node_base*>(x));
}

void _Rb_tree_rotate_left(_Rb_tree_node_base* const x,
                          _Rb_tree_node_base*& root) throw()
{
    _Rb_tree_node_base* const y = x->right;

    x->right = y->left;
    if (y->left)
        y->left->parent = x;
    y->parent = x->parent;

    if (x == root)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

void _Rb_tree_rotate_right(_Rb_tree_node_base* const x,
                          _Rb_tree_node_base*& root) throw()
{
    _Rb_tree_node_base* const y = x->left;

    x->left = y->right;
    if (y->right)
        y->right->parent = x;
    y->parent = x->parent;

    if (x == root)
        root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;

    y->right = x;
    x->parent = y;
}

void _Rb_tree_insert_and_rebalance(const bool insert_left,
                                   _Rb_tree_node_base* x,
                                   _Rb_tree_node_base* p,
                                   _Rb_tree_node_base& header) throw()
{
    _Rb_tree_node_base*& root = header.parent;

    x->left = NULL;
    x->right = NULL;
    x->parent = p;
    x->color = RED;

    if (insert_left)
    {
        p->left = x;
        if (p == &header)
        {
            header.right = x;
            header.parent = x;
        }
        else if (p == header.left)
            header.left = x;
    }
    else
    {
        p->right = x;
        if (p == header.right)
            header.right = x;
    }

    while (x != root && x->parent->color == RED)
    {
        _Rb_tree_node_base* const xpp = x->parent->parent;
        if (x->parent == xpp->left)
        {
            _Rb_tree_node_base* const w = xpp->right;
            if (w && w->color == RED)
            {
                w->color = BLACK;
                x->parent->color = BLACK;
                xpp->color = RED;
                x = xpp;
            }
            else
            {
                if (x == x->parent->right)
                {
                    x = x->parent;
                    _Rb_tree_rotate_left(x, root);
                }
                x->parent->color = BLACK;
                xpp->color = RED;
                _Rb_tree_rotate_right(xpp, root);
            }
        }
        else
        {
            _Rb_tree_node_base* const w = xpp->left;
            if (w && w->color == RED)
            {
                w->color = BLACK;
                x->parent->color = BLACK;
                xpp->color = RED;
                x = xpp;
            }
            else
            {
                if (x == x->parent->left)
                {
                    x = x->parent;
                    _Rb_tree_rotate_right(x, root);
                }
                x->parent->color = BLACK;
                xpp->color = RED;
                _Rb_tree_rotate_left(xpp, root);
            }
        }
    }
    root->color = BLACK;
}

_Rb_tree_node_base*
_Rb_tree_rebalance_for_erase(_Rb_tree_node_base* const z,
                             _Rb_tree_node_base& header) throw()
{
    _Rb_tree_node_base*& root = header.parent;
    _Rb_tree_node_base*& leftmost = header.left;
    _Rb_tree_node_base*& rightmost = header.right;
    _Rb_tree_node_base* y = z;
    _Rb_tree_node_base* x = NULL;
    _Rb_tree_node_base* x_parent = NULL;

    if (y->left == NULL)
        x = y->right;
    else if (y->right == NULL)
        x = y->left;
    else
    {
        y = y->right;
        while (y->left)
            y = y->left;
        x = y->right;
    }

    if (y != z)
    {
        z->left->parent = y;
        y->left = z->left;
        if (y != z->right)
        {
            x_parent = y->parent;
            if (x)
                x->parent = y->parent;
            y->parent->left = x;
            y->right = z->right;
            z->right->parent = y;
        }
        else
            x_parent = y;
        if (root == z)
            root = y;
        else if (z == z->parent->left)
            z->parent->left = y;
        else
            z->parent->right = y;
        y->parent = z->parent;
        ft::swap(y->color, z->color);
        y = z;
    }
    else
    {
        x_parent = y->parent;
        if (x)
            x->parent = y->parent;
        if (root == z)
            root = x;
        else if (z == z->parent->left)
            z->parent->left = x;
        else
            z->parent->right = x;
        if (leftmost == z)
        {
            if (z->right)
                leftmost = _Rb_tree_node_base::minimum(x);
            else
                leftmost = z->parent;
        }
        if (rightmost == z)
        {
            if (z->left)
                rightmost = _Rb_tree_node_base::maximum(x);
            else
                rightmost = z->parent;
        }
    }

    if (y->color == BLACK)
    {
        while (x != root && (x == NULL || x->color == BLACK))
        {
            if (x == x_parent->left)
            {
                _Rb_tree_node_base* w = x_parent->right;
                if (w->color == RED)
                {
                    w->color = BLACK;
                    x_parent->color = RED;
                    _Rb_tree_rotate_left(x_parent, root);
                    w = x_parent->right;
                }
                if ((w->left == NULL || w->left->color == BLACK) &&
                    (w->right == NULL || w->right->color == BLACK))
                {
                    w->color = RED;
                    x = x_parent;
                    x_parent = x_parent->parent;
                }
                else
                {
                    if (w->right == NULL || w->right->color == BLACK)
                    {
                        w->color = RED;
                        w->left->color = BLACK;
                        _Rb_tree_rotate_right(w, root);
                        w = x_parent->right;
                    }
                    w->color = x_parent->color;
                    x_parent->color = BLACK;
                    if (w->right)
                        w->right->color = BLACK;
                    _Rb_tree_rotate_left(x_parent, root);
                    break;
                }
            }
            else
            {
                _Rb_tree_node_base* w = x_parent->left;
                if (w->color == RED)
                {
                    w->color = BLACK;
                    x_parent->color = RED;
                    _Rb_tree_rotate_right(x_parent, root);
                    w = x_parent->left;
                }
                if ((w->left == NULL || w->left->color == BLACK) &&
                    (w->right == NULL || w->right->color == BLACK))
                {
                    w->color = RED;
                    x = x_parent;
                    x_parent = x_parent->parent;
                }
                else
                {
                    if (w->left == NULL || w->left->color == BLACK)
                    {
                        w->color = RED;
                        w->right->color = BLACK;
                        _Rb_tree_rotate_left(w, root);
                        w = x_parent->left;
                    }
                    w->color = x_parent->color;
                    x_parent->color = BLACK;
                    if (w->left)
                        w->left->color = BLACK;
                    _Rb_tree_rotate_right(x_parent, root);
                    break;
                }
            }
        }
        if (x)
            x->color = BLACK;
    }
    return y;
}

} // namespace ft

#endif // TREE_H
