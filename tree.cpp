#include "tree.hpp"

namespace ft {

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