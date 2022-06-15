#ifndef TEST_TYPES_H_
#define TEST_TYPES_H_

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wwrite-strings"

#include <iostream>

#include <string>
#include <cstddef>
#include <cstdlib>
#include <ctime>

#include <vector>
#include "../vector.hpp"

#include <map>
#include "../map.hpp"

#include <set>
#include "../set.hpp"

#include <stack>
#include "../stack.hpp"

namespace TestTypes
{

template <typename _Tp>
struct DefaultGenerator
{
    void init()
    {}

    void reset()
    {}

    void next()
    {}

    _Tp get() const
    { return _Tp(); }
};

/// sequence generators

struct IntGenerator
{
    IntGenerator()
        : _val(0)
    {}

    void init()
    {}

    void reset()
    { _val = 0; }

    void next()
    { ++_val; }

    int get() const
    { return _val; }

private:
    int _val;
};

struct FloatGenerator
{
    FloatGenerator()
        : _val(0.0)
    {}

    void init()
    {}

    void reset()
    { _val = 0.0; }

    void next()
    { _val += 1.0; }

    float get() const
    { return _val; }

private:
    float _val;
};

struct DoubleGenerator
{
    DoubleGenerator()
        : _val(0.0)
    {}

    void init()
    {}

    void reset()
    { _val = 0.0; }

    void next()
    { _val += 1.0; }

    double get() const
    { return _val; }

private:
    double _val;
};

struct StringGenerator
{
    StringGenerator()
        : _val("0")
    {}

    void init()
    {}

    void reset()
    { _val = "0"; }

    void next()
    {
        size_t size = _val.size();
        size_t i = size;

        while (i-- && _val[i] == '9')
            _val[i] = '0';

        if (size > 1 && i < size - 1 &&_val[i + 1] == '0')
            size = i + 1;

        if (i == -1 && _val[0] == '0')
        {
            _val.push_back('0');
            ++size;
        }
        else
            ++_val[size - 1];
    }

    const std::string&
    get() const
    { return _val; }

private:
    std::string _val;
};

/// random set generators

template <typename T, typename Gen>
void get_random_set(std::vector<T>& v, const size_t size, Gen gen = Gen())
{
    v.clear();
    v.reserve(size);

    std::srand(std::clock());
    v.push_back(gen.get());
    gen.next();
    v.push_back(gen.get());

    for (size_t i = 2; i < size; ++i)
    {
        gen.next();
        size_t index = std::rand() % i;
        T tmp = std::move(v[index]);
        v[index] = gen.get();
        v.emplace_back(tmp);
    }
}

struct IntSetRandomGenerator
{
    IntSetRandomGenerator()
        : _index(0)
    { init(); }

    void init()
    {
        get_random_set<int, IntGenerator>(_v, max_size);
    }

    void reset()
    { _index = 0; }

    void next()
    {
        _index = (_index + 1) % max_size;
    }

    int get() const
    { return _v[_index]; }

private:
    static const size_t max_size = 10000;

    std::vector<int> _v;
    size_t _index;
};

struct IntRandomGenerator
{
    IntRandomGenerator()
        : _val(0)
    { init(); }

    void init()
    { std::srand(std::clock()); }

    void reset()
    { }

    void next()
    { _val = std::rand(); }

    int get() const
    { return _val; }

private:
    int _val;
};

struct StringRandomGenerator
{
    StringRandomGenerator()
    {
        _val.reserve(max_len);
        init();
    }

    void init()
    {
        std::srand(std::clock());
        next();
    }

    void reset()
    { }

    void next()
    {
        size_t size = std::rand() % (max_len - 1) + 1;

        _val.clear();
        while (size--)
        {
            _val.push_back(base64_set[std::rand() % base64_size]);
        }
    }

    const std::string& get() const
    { return _val; }

    static constexpr char* base64_set =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    static constexpr size_t base64_size = 64;
    static constexpr size_t max_len = 64;

private:
    std::string _val;
};

template <typename _Vec, typename _Gen>
struct VectorRandomGenerator
{
    VectorRandomGenerator()
    { init(); }

    void init()
    {
        std::srand(std::clock());
        _val.reserve(max_size);
        _gen.init();
    }

    void reset()
    {
        _gen.reset();
    }

    void next()
    {
        size_t size = std::rand() % max_size;

        _val.clear();
        for (size_t i = 0; i < size; ++i)
        {
            _val.push_back(_gen.get());
            _gen.next();
        }
    }

    const _Vec& get() const
    {
        return _val;
    }

    static constexpr size_t max_size = 10;

private:
    _Vec _val;
    _Gen _gen;
};

template <typename _Map, typename _KeyGen, typename _ValueGen>
struct MapRandomGenerator
{
    MapRandomGenerator()
    { init(); }

    void init()
    {
        std::srand(std::clock());
        _key_gen.init();
        _value_gen.init();
    }

    void reset()
    {
        _key_gen.reset();
        _value_gen.reset();
    }

    void next()
    {
        size_t size = std::rand() % max_size;

        _val.clear();
        for (size_t i = 0; i < size; ++i)
        {
            _val[_key_gen.get()] = _value_gen.get();
            _key_gen.next();
            _value_gen.next();
        }
    }

    const _Map& get() const
    {
        return _val;
    }

    static constexpr size_t max_size = 10;

private:
    _Map      _val;
    _KeyGen   _key_gen;
    _ValueGen _value_gen;
};

template <typename _Set, typename _KeyGen>
struct SetRandomGenerator
{
    SetRandomGenerator()
    { init(); }

    void init()
    {
        std::srand(std::clock());
        _key_gen.init();
    }

    void reset()
    {
        _key_gen.reset();
    }

    void next()
    {
        size_t size = std::rand() % max_size;

        _val.clear();
        for (size_t i = 0; i < size; ++i)
        {
            _val.insert(_key_gen.get());
            _key_gen.next();
        }
    }

    const _Set& get() const
    {
        return _val;
    }

    static constexpr size_t max_size = 10;

private:
    _Set      _val;
    _KeyGen   _key_gen;
};

/// vector defines

template <typename _Val, typename _Generator = DefaultGenerator<_Val>,
          size_t _Size = 1000>
struct VectorDefines
{
    typedef _Val       value_type;
    typedef _Generator generator_type;

    static const size_t size = _Size;
};

typedef std::vector<int> std_vec_int_type;
typedef ft::vector<int>  ft_vec_int_type;

typedef VectorRandomGenerator<std_vec_int_type, IntRandomGenerator> svr_gen_type;
typedef VectorRandomGenerator<ft_vec_int_type, IntRandomGenerator>  fvr_gen_type;

/// map defines

template <typename _Key, typename _Val,
          typename _KeyGenerator = DefaultGenerator<_Key>,
          typename _ValueGenerator = DefaultGenerator<_Val>,
          size_t _Size = 1000>
struct MapDefines
{
    typedef _Key            key_type;
    typedef _Val            value_type;
    typedef _KeyGenerator   key_genereator_type;
    typedef _ValueGenerator value_genereator_type;

    static const size_t size = _Size;
};

typedef std::map<int,int> std_map_int_type;
typedef ft::map<int,int> ft_map_int_type;

typedef MapRandomGenerator<std_map_int_type,
            IntRandomGenerator,
            IntRandomGenerator
> smr_gen_type;

typedef MapRandomGenerator<ft_map_int_type,
            IntRandomGenerator,
            IntRandomGenerator
> fmr_gen_type;

/// set defines

template <typename _Key,
          typename _KeyGenerator = DefaultGenerator<_Key>,
          size_t _Size = 1000>
struct SetDefines
{
    typedef _Key            key_type;
    typedef _KeyGenerator   key_genereator_type;

    static const size_t size = _Size;
};

typedef std::set<int> std_set_int_type;
typedef ft::set<int> ft_set_int_type;

typedef SetRandomGenerator<std_set_int_type,
            IntRandomGenerator
> ssr_gen_type;

typedef SetRandomGenerator<ft_set_int_type,
            IntRandomGenerator
> fsr_gen_type;

// types lists

typedef ::testing::Types<
//            |value            |gen                    |size
VectorDefines< int,              IntGenerator,           0L >,
VectorDefines< int,              IntGenerator,           1L >,
VectorDefines< int,              IntGenerator,           2L >,
VectorDefines< int,              IntGenerator,           3L >,
VectorDefines< int,              IntGenerator               >,
VectorDefines< std::string,      StringGenerator            >,
VectorDefines< int,              IntSetRandomGenerator      >,
VectorDefines< int,              IntRandomGenerator         >,
VectorDefines< std::string,      StringRandomGenerator      >,
VectorDefines< std_vec_int_type, svr_gen_type               >,
VectorDefines< ft_vec_int_type,  fvr_gen_type               >
>
TestVectorTypes;

typedef ::testing::Types<
//         |key              |value |key_gen               |value_gen       |size
MapDefines< int,              int,   IntGenerator,          IntGenerator,    0L >,
MapDefines< int,              int,   IntGenerator,          IntGenerator,    1L >,
MapDefines< int,              int,   IntGenerator,          IntGenerator,    2L >,
MapDefines< int,              int,   IntGenerator,          IntGenerator,    3L >,
MapDefines< int,              int,   IntGenerator,          IntGenerator        >, //1000 by default
MapDefines< int,              float, IntGenerator,          FloatGenerator      >,
MapDefines< std::string,      float, StringGenerator,       FloatGenerator      >,
MapDefines< int,              int,   IntSetRandomGenerator, IntGenerator        >,
MapDefines< int,              int,   IntRandomGenerator,    IntGenerator        >,
MapDefines< std::string,      int,   StringRandomGenerator, IntGenerator        >,
MapDefines< std_map_int_type, int,   smr_gen_type,          IntGenerator        >,
MapDefines< ft_map_int_type,  int,   fmr_gen_type,          IntGenerator        >
>
TestMapTypes;

typedef ::testing::Types<
//         |value            |gen                    |size
SetDefines< int,              IntGenerator,           0L >,
SetDefines< int,              IntGenerator,           1L >,
SetDefines< int,              IntGenerator,           2L >,
SetDefines< int,              IntGenerator,           3L >,
SetDefines< int,              IntGenerator               >,
SetDefines< std::string,      StringGenerator            >,
SetDefines< int,              IntSetRandomGenerator      >,
SetDefines< int,              IntRandomGenerator         >,
SetDefines< std::string,      StringRandomGenerator      >,
SetDefines< std_vec_int_type, svr_gen_type               >,
SetDefines< ft_vec_int_type,  fvr_gen_type               >
>
TestSetTypes;


} // namespace TestTypes

#endif // TEST_TYPES_H_
