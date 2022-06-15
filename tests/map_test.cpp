#include <gtest/gtest.h>
#include <typeinfo>
#include "test_types.h"

///defines

#define COMPARE_TEST(ft_map, std_map) \
{\
    ASSERT_EQ(ft_map.size(), std_map.size());\
    ASSERT_EQ(ft_map.empty(), std_map.empty());\
\
    typename TestFixture::std_map_type::iterator std_begin = std_map.begin();\
    typename TestFixture::std_map_type::iterator std_end   = std_map.end();\
    typename TestFixture::ft_map_type::iterator  ft_begin  = ft_map.begin();\
    typename TestFixture::ft_map_type::iterator  ft_end    = ft_map.end();\
\
    size_t key_compare_fails = 0;\
    size_t value_compare_fails = 0;\
\
    for ( ; std_begin != std_end;\
        ++std_begin,\
        ++ft_begin)\
    {\
        ASSERT_NE(ft_begin, ft_end);\
        key_compare_fails += (ft_begin->first != std_begin->first);\
        value_compare_fails += (ft_begin->second != std_begin->second);\
    }\
\
    EXPECT_EQ(key_compare_fails, 0);\
    EXPECT_EQ(value_compare_fails, 0);\
}

/// helpers

template <typename std_iter, typename ft_iter>
int compare_iters(std_iter std_first, std_iter std_last,
                  ft_iter ft_first, ft_iter ft_last)
{
    if (std_first == std_last
        && ft_first != ft_last)
    {
        return 1;
    }

    while (std_first != std_last)
    {
        if (ft_first == ft_last
            || ft_first->second != std_first->second)
        {
            return 1;
        }
        ++std_first;
        ++ft_first;
    }
    return 0;
}

/// fixture

template <typename _Defines>
struct MapTest : public ::testing::Test
{
    typedef typename _Defines::key_type   key_type;
    typedef typename _Defines::value_type value_type;

    typedef std::map<key_type, value_type> std_map_type;
    typedef  ft::map<key_type, value_type>  ft_map_type;

    typedef typename _Defines::key_genereator_type   key_gen_type;
    typedef typename _Defines::value_genereator_type value_gen_type;

    static void init(std_map_type& std_map,
                     ft_map_type& ft_map,
                     key_gen_type& key_gen,
                     value_gen_type& value_gen,
                     const size_t size = MapTest::size)
    {
        for (size_t i = 0; i < size; ++i)
        {
            std_map.insert(std::make_pair(key_gen.get(), value_gen.get()));
            ft_map.insert(ft::make_pair(key_gen.get(), value_gen.get()));
            key_gen.next();
            value_gen.next();
        }
    }

    void init(const size_t size = MapTest::size)
    {
        MapTest::init(std_map, ft_map, key_gen, value_gen, size);
    }

    void SetUp()
    {
        key_gen.reset();
        value_gen.reset();
    }

    static const size_t size = _Defines::size;

    static key_gen_type   key_gen;
    static value_gen_type value_gen;

    std_map_type std_map;
     ft_map_type  ft_map;
};

template <typename _Defines>
typename _Defines::key_genereator_type   MapTest<_Defines>::key_gen;
template <typename _Defines>
typename _Defines::value_genereator_type MapTest<_Defines>::value_gen;


TYPED_TEST_SUITE_P(MapTest);

/// Tests

TYPED_TEST_P(MapTest, Capacity)
{
    EXPECT_EQ(this->std_map.empty(), this->ft_map.empty());
    EXPECT_EQ(this->std_map.size(), this->ft_map.size());
    EXPECT_EQ(this->std_map.max_size(), this->ft_map.max_size());
}

TYPED_TEST_P(MapTest, TypeDefs)
{
    typedef typename TestFixture::std_map_type std_map_type;
    typedef typename TestFixture::ft_map_type  ft_map_type;

    EXPECT_EQ(typeid(typename std_map_type::key_type),
              typeid(typename ft_map_type::key_type));
    EXPECT_EQ(typeid(typename std_map_type::mapped_type),
              typeid(typename ft_map_type::mapped_type));
    EXPECT_EQ(typeid(typename std_map_type::size_type),
              typeid(typename ft_map_type::size_type));
    EXPECT_EQ(typeid(typename std_map_type::difference_type),
              typeid(typename ft_map_type::difference_type));

    EXPECT_NE(typeid(typename std_map_type::key_compare),
              typeid(typename ft_map_type::key_compare));
    EXPECT_NE(typeid(typename std_map_type::value_type),
              typeid(typename ft_map_type::value_type));
    EXPECT_NE(typeid(typename std_map_type::allocator_type),
              typeid(typename ft_map_type::allocator_type));
    EXPECT_NE(typeid(typename std_map_type::reference),
              typeid(typename ft_map_type::reference));
    EXPECT_NE(typeid(typename std_map_type::const_reference),
              typeid(typename ft_map_type::const_reference));
    EXPECT_NE(typeid(typename std_map_type::pointer),
              typeid(typename ft_map_type::pointer));
    EXPECT_NE(typeid(typename std_map_type::const_pointer),
              typeid(typename ft_map_type::const_pointer));
    EXPECT_NE(typeid(typename std_map_type::iterator),
              typeid(typename ft_map_type::iterator));
    EXPECT_NE(typeid(typename std_map_type::const_iterator),
              typeid(typename ft_map_type::const_iterator));
    EXPECT_NE(typeid(typename std_map_type::reverse_iterator),
              typeid(typename ft_map_type::reverse_iterator));
    EXPECT_NE(typeid(typename std_map_type::const_reverse_iterator),
              typeid(typename ft_map_type::const_reverse_iterator));
}

TYPED_TEST_P(MapTest, Iterator)
{
    this->init();

    typename TestFixture::std_map_type::iterator std_begin = this->std_map.begin();
    typename TestFixture::std_map_type::iterator std_end = this->std_map.end();
    typename TestFixture::ft_map_type::iterator ft_begin = this->ft_map.begin();
    typename TestFixture::ft_map_type::iterator ft_end = this->ft_map.end();

    while (std_begin != std_end)
    {
        ASSERT_NE(ft_begin, ft_end);
        ASSERT_EQ(std_begin->first == ft_begin->first
                && std_begin->second == ft_begin->second, true);
        ++std_begin;
        ++ft_begin;
    }

    ASSERT_EQ(ft_begin, ft_end);

    if (TestFixture::size < 3) return;

    --std_begin;
    --ft_begin;

#ifdef __clang__

    while (std_begin != this->std_map.begin())
    {
        ASSERT_NE(ft_begin, this->ft_map.begin());
        ASSERT_EQ(std_begin->first == ft_begin->first
                && std_begin->second == ft_begin->second, true);
        --std_begin;
        --ft_begin;
    }
    ASSERT_EQ(std_begin->first == ft_begin->first
            && std_begin->second == ft_begin->second, true);
    ASSERT_EQ(ft_begin, this->ft_map.begin());

#else

    while (std_begin != std_end)
    {
        ASSERT_NE(ft_begin, ft_end);
        ASSERT_EQ(std_begin->first == ft_begin->first
                && std_begin->second == ft_begin->second, true);
        --std_begin;
        --ft_begin;
    }

    ASSERT_EQ(ft_begin, ft_end);

#endif // __clang__
}

TYPED_TEST_P(MapTest, ReverseIterator)
{
    this->init();

    typename TestFixture::std_map_type::reverse_iterator
        std_rbegin = this->std_map.rbegin();
    typename TestFixture::std_map_type::reverse_iterator
        std_rend = this->std_map.rend();
    typename TestFixture::ft_map_type::reverse_iterator
        ft_rbegin = this->ft_map.rbegin();
    typename TestFixture::ft_map_type::reverse_iterator
        ft_rend = this->ft_map.rend();

    while (std_rbegin != std_rend)
    {
        ASSERT_NE(ft_rbegin, ft_rend);
        ASSERT_EQ(std_rbegin->first == ft_rbegin->first
                && std_rbegin->second == ft_rbegin->second, true);
        ++std_rbegin;
        ++ft_rbegin;
    }

    ASSERT_EQ(ft_rbegin, ft_rend);
}

TYPED_TEST_P(MapTest, AccessOperator)
{
    size_t edge = TestFixture::size >> 1;
    for (size_t i = 0; i < edge; ++i)
        this->key_gen.next();

    this->init();

    typename TestFixture::key_gen_type   key_gen;
    typename TestFixture::value_gen_type value_gen;

    for (size_t i = 0; i < edge; ++i)
    {
        this->std_map[key_gen.get()] = value_gen.get();
        this->ft_map[key_gen.get()] = value_gen.get();
        key_gen.next();
        value_gen.next();
    }

    for (size_t i = edge; i < TestFixture::size + edge; ++i)
    {
        key_gen.next();
        value_gen.next();
    }

    for (size_t i = TestFixture::size + edge; i < (TestFixture::size << 1); ++i)
    {
        this->std_map[key_gen.get()] = value_gen.get();
        this->ft_map[key_gen.get()] = value_gen.get();
        key_gen.next();
        value_gen.next();
    }

    COMPARE_TEST(this->ft_map, this->std_map);
}

TYPED_TEST_P(MapTest, AssignOperator)
{
    typename TestFixture::std_map_type std_map;
    typename TestFixture::ft_map_type ft_map;

    this->init();
    std_map = this->std_map;
    ft_map = this->ft_map;

    COMPARE_TEST(ft_map, std_map);

    std_map.clear();
    ft_map.clear();
    TestFixture::init(std_map, ft_map, this->key_gen, this->value_gen);
    std_map = this->std_map;
    ft_map = this->ft_map;

    COMPARE_TEST(ft_map, std_map);
}

TYPED_TEST_P(MapTest, RangeConstructor)
{
    this->init();

    typename TestFixture::std_map_type
        std_map(this->std_map.begin(), this->std_map.end());
    typename TestFixture::ft_map_type
        ft_map(this->ft_map.begin(), this->ft_map.end());

    COMPARE_TEST(ft_map, std_map);
}

TYPED_TEST_P(MapTest, CopyConstructor)
{
    this->init();

    typename TestFixture::std_map_type std_map(this->std_map);
    typename TestFixture::ft_map_type ft_map(this->ft_map);

    COMPARE_TEST(ft_map, std_map);
}

TYPED_TEST_P(MapTest, Insert)
{
    this->init();

    COMPARE_TEST(this->ft_map, this->std_map);

    typedef typename TestFixture::std_map_type::iterator std_iter_type;
    typedef typename TestFixture::ft_map_type::iterator ft_iter_type;

    this->value_gen.next();

    std::pair<std_iter_type, bool> std =
        this->std_map.insert(std::make_pair(this->key_gen.get(),
        this->value_gen.get()));
    ft::pair<ft_iter_type, bool> ft =
        this->ft_map.insert(ft::make_pair(this->key_gen.get(),
        this->value_gen.get()));
    EXPECT_EQ(std.second, ft.second);
    COMPARE_TEST(this->ft_map, this->std_map);

    this->key_gen.next();

    std = this->std_map.insert(std::make_pair(this->key_gen.get(),
        this->value_gen.get()));
    ft = this->ft_map.insert(ft::make_pair(this->key_gen.get(),
        this->value_gen.get()));
    EXPECT_EQ(std.second, ft.second);
    COMPARE_TEST(this->ft_map, this->std_map);
}

TYPED_TEST_P(MapTest, InsertHint)
{
    for (size_t i = 0; i < TestFixture::size; ++i)
    {
        this->std_map.insert(this->std_map.end(),
              std::make_pair(this->key_gen.get(), this->value_gen.get()));
        this->ft_map.insert(this->ft_map.end(),
              ft::make_pair(this->key_gen.get(), this->value_gen.get()));
    }

    COMPARE_TEST(this->ft_map, this->std_map);

    this->std_map.insert(this->std_map.begin(),
          std::make_pair(this->key_gen.get(), this->value_gen.get()));
    this->ft_map.insert(this->ft_map.begin(),
          ft::make_pair(this->key_gen.get(), this->value_gen.get()));

    COMPARE_TEST(this->ft_map, this->std_map);
}

TYPED_TEST_P(MapTest, InsertRange)
{
    this->init();

    typedef typename TestFixture::std_map_type std_map_type;
    typedef typename TestFixture::ft_map_type  ft_map_type;

    std_map_type std_map_tmp;
    ft_map_type ft_map_tmp;

    this->std_map.insert(std_map_tmp.begin(), std_map_tmp.end());
    this->ft_map.insert(ft_map_tmp.begin(), ft_map_tmp.end());
    COMPARE_TEST(this->ft_map, this->std_map);

    TestFixture::init(std_map_tmp, ft_map_tmp, this->key_gen, this->value_gen);

    this->std_map.insert(std_map_tmp.begin(), std_map_tmp.end());
    this->ft_map.insert(ft_map_tmp.begin(), ft_map_tmp.end());
    COMPARE_TEST(this->ft_map, this->std_map);
}

TYPED_TEST_P(MapTest, Clear)
{
    this->init();

    this->ft_map.clear();

    EXPECT_TRUE(this->ft_map.empty());
    EXPECT_EQ(this->ft_map.size(), 0);
    EXPECT_EQ(this->ft_map.begin(), this->ft_map.end());
}

TYPED_TEST_P(MapTest, Erase)
{
    this->init();

    if (TestFixture::size > 2)
    {
        this->std_map.erase(this->std_map.begin());
        this->std_map.erase(++this->std_map.begin());
        this->std_map.erase(--this->std_map.end());

        this->ft_map.erase(this->ft_map.begin());
        this->ft_map.erase(++this->ft_map.begin());
        this->ft_map.erase(--this->ft_map.end());

        COMPARE_TEST(this->ft_map, this->std_map);
    }

    typename TestFixture::std_map_type::iterator std_begin = this->std_map.begin();
    typename TestFixture::std_map_type::iterator std_end   = this->std_map.end();
    typename TestFixture::ft_map_type::iterator  ft_begin  = this->ft_map.begin();
    typename TestFixture::ft_map_type::iterator  ft_end    = this->ft_map.end();

    while (std_begin != std_end)
    {
        ASSERT_NE(ft_begin, ft_end);
        this->std_map.erase(std_begin++);
        this->ft_map.erase(ft_begin++);
    }

    COMPARE_TEST(this->ft_map, this->std_map);
}

TYPED_TEST_P(MapTest, EraseRange)
{
    this->init();

    this->std_map.erase(this->std_map.begin(), this->std_map.begin());
    this->ft_map.erase(this->ft_map.begin(), this->ft_map.begin());
    COMPARE_TEST(this->ft_map, this->std_map);

    this->std_map.erase(this->std_map.end(), this->std_map.end());
    this->ft_map.erase(this->ft_map.end(), this->ft_map.end());
    COMPARE_TEST(this->ft_map, this->std_map);

    if (TestFixture::size < 4) return;

#ifndef __clang__
    this->std_map.erase(++this->std_map.begin(), --this->std_map.begin());
    this->ft_map.erase(++this->ft_map.begin(), --this->ft_map.begin());
    COMPARE_TEST(this->ft_map, this->std_map);
#endif // __clang__

    this->std_map.erase(this->std_map.begin(), this->std_map.end());
    this->ft_map.erase(this->ft_map.begin(), this->ft_map.end());
    COMPARE_TEST(this->ft_map, this->std_map);
}

TYPED_TEST_P(MapTest, EraseKey)
{
    this->init();

    typename TestFixture::std_map_type::iterator std_begin = this->std_map.begin();

    while (!this->std_map.empty())
    {
        typename TestFixture::key_type key = std_begin->first;
        ++std_begin;
        ASSERT_EQ(this->std_map.erase(key), this->ft_map.erase(key));
    }
    COMPARE_TEST(this->ft_map, this->std_map);
}

TYPED_TEST_P(MapTest, Swap)
{
    this->init();

    typename TestFixture::std_map_type std_map2;
    typename TestFixture::ft_map_type  ft_map2;

    TestFixture::init(std_map2, ft_map2, this->key_gen, this->value_gen);

    this->ft_map.swap(ft_map2);

    COMPARE_TEST(this->ft_map, std_map2);
    COMPARE_TEST(ft_map2, this->std_map);

    ft::swap(this->ft_map, ft_map2);

    COMPARE_TEST(this->ft_map, this->std_map);
    COMPARE_TEST(ft_map2, std_map2);
}

TYPED_TEST_P(MapTest, Count)
{
    typedef typename TestFixture::key_type key_type;

    const key_type key = key_type();
    EXPECT_EQ(this->std_map.count(key), this->ft_map.count(key));

    for (size_t i = 0; i < (TestFixture::size >> 1); ++i)
        this->key_gen.next();

    this->init();

    typename TestFixture::key_gen_type key_gen;
    size_t count_fails = 0;
    for (size_t i = 0; i < (TestFixture::size << 1); ++i)
    {
        count_fails += (this->std_map.count(key_gen.get()) !=
                        this->ft_map.count(key_gen.get()));
        key_gen.next();
    }

    EXPECT_EQ(count_fails, 0);

    this->std_map.clear();
    this->ft_map.clear();
    EXPECT_EQ(this->std_map.count(key), this->ft_map.count(key));
}

TYPED_TEST_P(MapTest, Find)
{
    typedef typename TestFixture::key_type key_type;

    const key_type key = key_type();
    EXPECT_EQ(this->ft_map.find(key), this->ft_map.end());

    size_t edge = TestFixture::size >> 1;
    for (size_t i = 0; i < edge; ++i)
        this->key_gen.next();

    this->init();

    const typename TestFixture::std_map_type::iterator std_end =
        this->std_map.end();
    const typename TestFixture::ft_map_type::iterator ft_end =
        this->ft_map.end();

    typename TestFixture::key_gen_type key_gen;
    size_t find_fails = 0;

    for (size_t i = 0; i < (TestFixture::size << 1); ++i)
    {
        typename TestFixture::std_map_type::iterator std_iter =
            this->std_map.find(key_gen.get());
        typename TestFixture::ft_map_type::iterator ft_iter =
            this->ft_map.find(key_gen.get());

        if (std_iter == std_end)
            find_fails += (ft_iter != ft_end);
        else
            find_fails += (std_iter->second != ft_iter->second);

        key_gen.next();
    }

    EXPECT_EQ(find_fails, 0);

    this->ft_map.clear();
    EXPECT_EQ(this->ft_map.find(key), this->ft_map.end());
}

TYPED_TEST_P(MapTest, EqualRange)
{
    typedef typename TestFixture::std_map_type::iterator std_iter_type;
    typedef typename TestFixture::ft_map_type::iterator  ft_iter_type;

    typedef typename std::pair<std_iter_type,std_iter_type> std_iter_pair_type;
    typedef typename ft::pair<ft_iter_type,ft_iter_type>    ft_iter_pair_type;

    typedef typename TestFixture::key_type key_type;

    const key_type key = key_type();
    EXPECT_EQ(this->ft_map.equal_range(key).first,
              this->ft_map.equal_range(key).second);

    size_t edge = TestFixture::size >> 1;
    for (size_t i = 0; i < edge; ++i)
        this->key_gen.next();

    this->init();

    typename TestFixture::key_gen_type key_gen;
    size_t equal_range_fails = 0;

    for (size_t i = 0; i < (TestFixture::size << 1); ++i)
    {
        std_iter_pair_type std = this->std_map.equal_range(key_gen.get());
        ft_iter_pair_type ft = this->ft_map.equal_range(key_gen.get());
        equal_range_fails += compare_iters(std.first, std.second,
                                           ft.first, ft.second);
        key_gen.next();
    }

    EXPECT_EQ(equal_range_fails, 0);

    this->ft_map.clear();
    EXPECT_EQ(this->ft_map.equal_range(key).first,
              this->ft_map.equal_range(key).second);
}

TYPED_TEST_P(MapTest, LowerBound)
{
    typedef typename TestFixture::key_type key_type;

    const key_type key = key_type();
    EXPECT_EQ(this->ft_map.lower_bound(key), this->ft_map.end());

    size_t edge = TestFixture::size >> 1;
    for (size_t i = 0; i < edge; ++i)
        this->key_gen.next();

    this->init();

    typename TestFixture::key_gen_type key_gen;
    size_t bound_fails = 0;

    for (size_t i = 0; i < edge; ++i)
    {
        bound_fails += (
            (this->std_map.lower_bound(key_gen.get()) == this->std_map.end()) !=
            (this->ft_map.lower_bound(key_gen.get()) == this->ft_map.end()));
        key_gen.next();
    }

    for (size_t i = edge; i < TestFixture::size + edge; ++i)
    {
        typename TestFixture::std_map_type::iterator std_iter =
            this->std_map.lower_bound(key_gen.get());
        typename TestFixture::ft_map_type::iterator  ft_iter =
            this->ft_map.lower_bound(key_gen.get());

        if (std_iter == this->std_map.end())
            bound_fails += (ft_iter != this->ft_map.end());
        else
            bound_fails += (std_iter->second != ft_iter->second);

        key_gen.next();
    }

    for (size_t i = TestFixture::size + edge; i < (TestFixture::size << 1); ++i)
    {
        bound_fails += (
            (this->std_map.lower_bound(key_gen.get()) == this->std_map.end()) !=
            (this->ft_map.lower_bound(key_gen.get()) == this->ft_map.end()));
        key_gen.next();
    }

    EXPECT_EQ(bound_fails, 0);

    this->ft_map.clear();
    EXPECT_EQ(this->ft_map.lower_bound(key), this->ft_map.end());
}

TYPED_TEST_P(MapTest, UpperBound)
{
    typedef typename TestFixture::key_type key_type;

    const key_type key = key_type();
    EXPECT_EQ(this->ft_map.upper_bound(key), this->ft_map.end());

    size_t edge = TestFixture::size >> 1;
    for (size_t i = 0; i < edge; ++i)
        this->key_gen.next();

    this->init();

    typename TestFixture::key_gen_type key_gen;
    size_t bound_fails = 0;

    for (size_t i = 0; i < edge; ++i)
    {
        bound_fails += (
            (this->std_map.upper_bound(key_gen.get()) == this->std_map.end()) !=
            (this->ft_map.upper_bound(key_gen.get()) == this->ft_map.end()));
        key_gen.next();
    }

    for (size_t i = edge; i < TestFixture::size + edge; ++i)
    {
        typename TestFixture::std_map_type::iterator std_iter =
            this->std_map.upper_bound(key_gen.get());
        typename TestFixture::ft_map_type::iterator  ft_iter =
            this->ft_map.upper_bound(key_gen.get());

        if (std_iter == this->std_map.end())
            bound_fails += (ft_iter != this->ft_map.end());
        else
            bound_fails += (std_iter->second != ft_iter->second);

        key_gen.next();
    }

    for (size_t i = TestFixture::size + edge; i < (TestFixture::size << 1); ++i)
    {
        bound_fails += (
            (this->std_map.upper_bound(key_gen.get()) == this->std_map.end()) !=
            (this->ft_map.upper_bound(key_gen.get()) == this->ft_map.end()));
        key_gen.next();
    }

    EXPECT_EQ(bound_fails, 0);

    this->ft_map.clear();
    EXPECT_EQ(this->ft_map.upper_bound(key), this->ft_map.end());
}

TYPED_TEST_P(MapTest, CompareOperators)
{
    typename TestFixture::std_map_type std_map;
    typename TestFixture::ft_map_type  ft_map;

    typename TestFixture::std_map_type std_map2;
    typename TestFixture::ft_map_type  ft_map2;

    this->init();
    TestFixture::init(std_map, ft_map, this->key_gen, this->value_gen);
    this->SetUp();
    TestFixture::init(std_map, ft_map, this->key_gen, this->value_gen,
        TestFixture::size >> 1);

    EXPECT_EQ(this->std_map == this->std_map, this->ft_map == this->ft_map);
    EXPECT_EQ(this->std_map != this->std_map, this->ft_map != this->ft_map);
    EXPECT_EQ(this->std_map <  this->std_map, this->ft_map <  this->ft_map);
    EXPECT_EQ(this->std_map <= this->std_map, this->ft_map <= this->ft_map);
    EXPECT_EQ(this->std_map >  this->std_map, this->ft_map >  this->ft_map);
    EXPECT_EQ(this->std_map >= this->std_map, this->ft_map >= this->ft_map);

    EXPECT_EQ(this->std_map == std_map, this->ft_map == ft_map);
    EXPECT_EQ(this->std_map != std_map, this->ft_map != ft_map);
    EXPECT_EQ(this->std_map <  std_map, this->ft_map <  ft_map);
    EXPECT_EQ(this->std_map <= std_map, this->ft_map <= ft_map);
    EXPECT_EQ(this->std_map >  std_map, this->ft_map >  ft_map);
    EXPECT_EQ(this->std_map >= std_map, this->ft_map >= ft_map);

    EXPECT_EQ(this->std_map == std_map2, this->ft_map == ft_map2);
    EXPECT_EQ(this->std_map != std_map2, this->ft_map != ft_map2);
    EXPECT_EQ(this->std_map <  std_map2, this->ft_map <  ft_map2);
    EXPECT_EQ(this->std_map <= std_map2, this->ft_map <= ft_map2);
    EXPECT_EQ(this->std_map >  std_map2, this->ft_map >  ft_map2);
    EXPECT_EQ(this->std_map >= std_map2, this->ft_map >= ft_map2);
}

TYPED_TEST_P(MapTest, KeyCompare)
{
    typename TestFixture::std_map_type std_map;
    typename TestFixture::ft_map_type ft_map;

    this->init();
    TestFixture::init(std_map, ft_map, this->key_gen, this->value_gen);

    typename TestFixture::std_map_type::key_compare std_kc;
    typename TestFixture::ft_map_type::key_compare  ft_kc;

    typename TestFixture::std_map_type::iterator std_begin = this->std_map.begin();
    typename TestFixture::std_map_type::iterator std_end = this->std_map.end();
    typename TestFixture::ft_map_type::iterator ft_begin = this->ft_map.begin();
    typename TestFixture::ft_map_type::iterator ft_end = this->ft_map.end();

    typename TestFixture::std_map_type::iterator std_begin2 = this->std_map.begin();
    typename TestFixture::std_map_type::iterator std_end2 = this->std_map.end();
    typename TestFixture::ft_map_type::iterator ft_begin2 = this->ft_map.begin();
    typename TestFixture::ft_map_type::iterator ft_end2 = this->ft_map.end();

    size_t compare_fails = 0;

    while (std_begin != std_end)
    {
        compare_fails += ((std_kc(std_begin->first, std_begin->first) !=
                           ft_kc(ft_begin->first, ft_begin->first))
                           || (std_kc(std_begin->first, std_begin2->first) !=
                           ft_kc(ft_begin->first, ft_begin2->first)));
        ++std_begin;
        ++std_begin2;
        ++ft_begin;
        ++ft_begin2;
    }

    EXPECT_EQ(compare_fails, 0);
}

TYPED_TEST_P(MapTest, ValueCompare)
{
    typename TestFixture::std_map_type std_map;
    typename TestFixture::ft_map_type ft_map;

    this->init();
    TestFixture::init(std_map, ft_map, this->key_gen, this->value_gen);

    typename TestFixture::std_map_type::value_compare std_vc =
        this->std_map.value_comp();
    typename TestFixture::ft_map_type::value_compare  ft_vc =
        this->ft_map.value_comp();

    typename TestFixture::std_map_type::iterator std_begin = this->std_map.begin();
    typename TestFixture::std_map_type::iterator std_end = this->std_map.end();
    typename TestFixture::ft_map_type::iterator ft_begin = this->ft_map.begin();
    typename TestFixture::ft_map_type::iterator ft_end = this->ft_map.end();

    typename TestFixture::std_map_type::iterator std_begin2 = this->std_map.begin();
    typename TestFixture::std_map_type::iterator std_end2 = this->std_map.end();
    typename TestFixture::ft_map_type::iterator ft_begin2 = this->ft_map.begin();
    typename TestFixture::ft_map_type::iterator ft_end2 = this->ft_map.end();

    size_t compare_fails = 0;

    while (std_begin != std_end)
    {
        compare_fails += ((std_vc(*std_begin, *std_begin) !=
                           ft_vc(*ft_begin, *ft_begin))
                           || (std_vc(*std_begin, *std_begin2) !=
                           ft_vc(*ft_begin, *ft_begin2)));
        ++std_begin;
        ++std_begin2;
        ++ft_begin;
        ++ft_begin2;
    }

    EXPECT_EQ(compare_fails, 0);
}

/// Registration of tests and instantiation of types

REGISTER_TYPED_TEST_SUITE_P(MapTest,
// TESTS LIST
    Capacity,
    TypeDefs,
    Iterator,
    ReverseIterator,
    AccessOperator,
    AssignOperator,
    // constructors
    RangeConstructor,
    CopyConstructor,
    // modifiers
    Insert,
    InsertHint,
    InsertRange,
    Clear,
    Erase,
    EraseRange,
    EraseKey,
    Swap,
    // lookup
    Count,
    Find,
    EqualRange,
    LowerBound,
    UpperBound,
    // non-member
    CompareOperators,
    // observers
    KeyCompare,
    ValueCompare
);

INSTANTIATE_TYPED_TEST_SUITE_P(MapInstance, MapTest, TestTypes::TestMapTypes);
