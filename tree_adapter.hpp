#ifndef TREE_ADAPTER_H
#define TREE_ADAPTER_H

namespace ft {

template <typename Arg, typename Result>
struct unary_function
{
    typedef Arg    argument_type;
    typedef Result result_type;
};

template <typename Arg1, typename Arg2, typename Result>
struct binary_function
{
    typedef Arg1   firstArgument_type;
    typedef Arg2   secondArgument_type;
    typedef Result result_type;
};

template <typename T>
struct Identity : public unary_function<T, T>
{
    T& operator()(T& x)
    {
        return x;
    }

    const T& operator()(const T& x) const
    {
        return x;
    }
};

template <typename T>
struct Identity<const T> : Identity<T> { };

template <typename Pair>
struct Select1st : public unary_function<Pair, typename Pair::first_type>
{
    typename Pair::first_type& operator()(Pair& x)
    {
        return x.first;
    }

    const typename Pair::first_type& operator()(const Pair& x) const
    {
        return x.first;
    }
};

template <class T>
struct less : public binary_function<T, T, bool>
{
    bool operator()(const T& lhs, const T& rhs) const
    {
        return lhs < rhs;
    }
};

} // namespace ft

#endif // TREE_ADAPTER_H
