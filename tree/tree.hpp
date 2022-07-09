#ifndef TREE_H
#define TREE_H

namespace ft {

enum RB_tree_color { RED, BLACK };

template <typename value_type>
struct node {
    typedef node<value_type>*       ptr;
    typedef const node<value_type>* const_ptr;

    ptr           parent;
    ptr           left;
    ptr           right;
    RB_tree_color color;
    value_type    value;

    node() {}

    static ptr minimum(ptr x)
    {
        while (x->left)
            x = x->left;
        return x;
    }

    static const_ptr minimum(const_ptr x)
    {
        while (x->left)
            x = x->left;
        return x;
    }

    static ptr maximum(ptr x)
    {
        while (x->right)
            x = x->right;
        return x;
    }

    static const_ptr maximum(const_ptr x)
    {
        while (x->right)
            x = x->right;
        return x;
    }

    // Val* val_ptr() { return ft::addressof(value_field); }
    // const Val* val_ptr() const { return ft::addressof(value_field); }
};

template <typename Key, typename Val, typename KeyOfValue,
          typename Compare, typename Alloc = std::allocator<Val> >
class RB_Tree
{
public:

private:
    node root;
};

} // namespace ft

#endif 