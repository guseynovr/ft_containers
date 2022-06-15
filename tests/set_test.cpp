#include <gtest/gtest.h>
#include <typeinfo>
#include "test_types.h"

///defines

#define COMPARE_TEST(ft_set, std_set) \
{\
    ASSERT_EQ(ft_set.size(), std_set.size());\
    ASSERT_EQ(ft_set.empty(), std_set.empty());\
\
    typename TestFixture::std_set_type::iterator std_begin = std_set.begin();\
    typename TestFixture::std_set_type::iterator std_end   = std_set.end();\
    typename TestFixture::ft_set_type::iterator  ft_begin  = ft_set.begin();\
    typename TestFixture::ft_set_type::iterator  ft_end    = ft_set.end();\
\
    size_t key_compare_fails = 0;\
\
    for ( ; std_begin != std_end;\
        ++std_begin,\
        ++ft_begin)\
    {\
        ASSERT_NE(ft_begin, ft_end);\
        key_compare_fails += (*ft_begin!= *std_begin);\
    }\
\
    EXPECT_EQ(key_compare_fails, 0);\
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
            || *ft_first != *std_first)
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
struct SetTest : public ::testing::Test
{
    typedef typename _Defines::key_type   key_type;

    typedef std::set<key_type> std_set_type;
    typedef  ft::set<key_type>  ft_set_type;

    typedef typename _Defines::key_genereator_type   key_gen_type;

    static void init(std_set_type& std_set,
                     ft_set_type& ft_set,
                     key_gen_type& key_gen,
                     const size_t size = SetTest::size)
    {
        for (size_t i = 0; i < size; ++i)
        {
            std_set.insert(key_gen.get());
            ft_set.insert(key_gen.get());
            key_gen.next();
        }
    }

    void init(const size_t size = SetTest::size)
    {
        SetTest::init(std_set, ft_set, key_gen, size);
    }

    void SetUp()
    {
        key_gen.reset();
    }

    static const size_t size = _Defines::size;

    static key_gen_type   key_gen;

    std_set_type std_set;
     ft_set_type  ft_set;
};

template <typename _Defines>
typename _Defines::key_genereator_type   SetTest<_Defines>::key_gen;


TYPED_TEST_SUITE_P(SetTest);

/// Tests

TYPED_TEST_P(SetTest, Capacity)
{
    EXPECT_EQ(this->std_set.empty(), this->ft_set.empty());
    EXPECT_EQ(this->std_set.size(), this->ft_set.size());
    EXPECT_EQ(this->std_set.max_size(), this->ft_set.max_size());
}

TYPED_TEST_P(SetTest, TypeDefs)
{
    typedef typename TestFixture::std_set_type std_set_type;
    typedef typename TestFixture::ft_set_type  ft_set_type;

    EXPECT_EQ(typeid(typename std_set_type::key_type),
              typeid(typename ft_set_type::key_type));
    EXPECT_EQ(typeid(typename std_set_type::value_type),
              typeid(typename ft_set_type::value_type));
    EXPECT_EQ(typeid(typename std_set_type::size_type),
              typeid(typename ft_set_type::size_type));
    EXPECT_EQ(typeid(typename std_set_type::difference_type),
              typeid(typename ft_set_type::difference_type));
    EXPECT_EQ(typeid(typename std_set_type::reference),
              typeid(typename ft_set_type::reference));
    EXPECT_EQ(typeid(typename std_set_type::const_reference),
              typeid(typename ft_set_type::const_reference));
    EXPECT_EQ(typeid(typename std_set_type::pointer),
              typeid(typename ft_set_type::pointer));
    EXPECT_EQ(typeid(typename std_set_type::const_pointer),
              typeid(typename ft_set_type::const_pointer));

    EXPECT_NE(typeid(typename std_set_type::key_compare),
              typeid(typename ft_set_type::key_compare));
    EXPECT_NE(typeid(typename std_set_type::value_compare),
              typeid(typename ft_set_type::value_compare));
    EXPECT_NE(typeid(typename std_set_type::iterator),
              typeid(typename ft_set_type::iterator));
    EXPECT_NE(typeid(typename std_set_type::const_iterator),
              typeid(typename ft_set_type::const_iterator));
    EXPECT_NE(typeid(typename std_set_type::reverse_iterator),
              typeid(typename ft_set_type::reverse_iterator));
    EXPECT_NE(typeid(typename std_set_type::const_reverse_iterator),
              typeid(typename ft_set_type::const_reverse_iterator));
}

TYPED_TEST_P(SetTest, Iterator)
{
    this->init();

    typename TestFixture::std_set_type::iterator std_begin = this->std_set.begin();
    typename TestFixture::std_set_type::iterator std_end = this->std_set.end();
    typename TestFixture::ft_set_type::iterator ft_begin = this->ft_set.begin();
    typename TestFixture::ft_set_type::iterator ft_end = this->ft_set.end();

    while (std_begin != std_end)
    {
        ASSERT_NE(ft_begin, ft_end);
        ASSERT_EQ(*std_begin, *ft_begin);
        ++std_begin;
        ++ft_begin;
    }

    ASSERT_EQ(ft_begin, ft_end);

    if (TestFixture::size < 3) return;

    --std_begin;
    --ft_begin;

#ifdef __clang__

    while (std_begin != this->std_set.begin())
    {
        ASSERT_NE(ft_begin, this->ft_set.begin());
        ASSERT_EQ(*std_begin, *ft_begin);
        --std_begin;
        --ft_begin;
    }
    ASSERT_EQ(*std_begin, *ft_begin);
    ASSERT_EQ(ft_begin, this->ft_set.begin());

#else

    while (std_begin != std_end)
    {
        ASSERT_NE(ft_begin, ft_end);
        ASSERT_EQ(*std_begin, *ft_begin);
        --std_begin;
        --ft_begin;
    }

    ASSERT_EQ(ft_begin, ft_end);

#endif // __clang__
}

TYPED_TEST_P(SetTest, ReverseIterator)
{
    this->init();

    typename TestFixture::std_set_type::reverse_iterator
        std_rbegin = this->std_set.rbegin();
    typename TestFixture::std_set_type::reverse_iterator
        std_rend = this->std_set.rend();
    typename TestFixture::ft_set_type::reverse_iterator
        ft_rbegin = this->ft_set.rbegin();
    typename TestFixture::ft_set_type::reverse_iterator
        ft_rend = this->ft_set.rend();

    while (std_rbegin != std_rend)
    {
        ASSERT_NE(ft_rbegin, ft_rend);
        ASSERT_EQ(*std_rbegin, *ft_rbegin);
        ++std_rbegin;
        ++ft_rbegin;
    }

    ASSERT_EQ(ft_rbegin, ft_rend);
}

TYPED_TEST_P(SetTest, AssignOperator)
{
    typename TestFixture::std_set_type std_set;
    typename TestFixture::ft_set_type ft_set;

    this->init();
    std_set = this->std_set;
    ft_set = this->ft_set;

    COMPARE_TEST(ft_set, std_set);

    std_set.clear();
    ft_set.clear();
    TestFixture::init(std_set, ft_set, this->key_gen);
    std_set = this->std_set;
    ft_set = this->ft_set;

    COMPARE_TEST(ft_set, std_set);
}

TYPED_TEST_P(SetTest, RangeConstructor)
{
    this->init();

    typename TestFixture::std_set_type
        std_set(this->std_set.begin(), this->std_set.end());
    typename TestFixture::ft_set_type
        ft_set(this->ft_set.begin(), this->ft_set.end());

    COMPARE_TEST(ft_set, std_set);
}

TYPED_TEST_P(SetTest, CopyConstructor)
{
    this->init();

    typename TestFixture::std_set_type std_set(this->std_set);
    typename TestFixture::ft_set_type ft_set(this->ft_set);

    COMPARE_TEST(ft_set, std_set);
}

TYPED_TEST_P(SetTest, Insert)
{
    this->init();

    COMPARE_TEST(this->ft_set, this->std_set);

    typedef typename TestFixture::std_set_type::iterator std_iter_type;
    typedef typename TestFixture::ft_set_type::iterator ft_iter_type;

    std::pair<std_iter_type, bool> std =
        this->std_set.insert(this->key_gen.get());
    ft::pair<ft_iter_type, bool> ft =
        this->ft_set.insert(this->key_gen.get());
    EXPECT_EQ(std.second, ft.second);
    COMPARE_TEST(this->ft_set, this->std_set);

    this->key_gen.next();

    std = this->std_set.insert(this->key_gen.get());
    ft = this->ft_set.insert(this->key_gen.get());
    EXPECT_EQ(std.second, ft.second);
    COMPARE_TEST(this->ft_set, this->std_set);
}

TYPED_TEST_P(SetTest, InsertHint)
{
    for (size_t i = 0; i < TestFixture::size; ++i)
    {
        this->std_set.insert(this->std_set.end(), this->key_gen.get());
        this->ft_set.insert(this->ft_set.end(), this->key_gen.get());
    }

    COMPARE_TEST(this->ft_set, this->std_set);

    this->std_set.insert(this->std_set.end(), this->key_gen.get());
    this->ft_set.insert(this->ft_set.end(), this->key_gen.get());

    COMPARE_TEST(this->ft_set, this->std_set);
}

TYPED_TEST_P(SetTest, InsertRange)
{
    this->init();

    typedef typename TestFixture::std_set_type std_set_type;
    typedef typename TestFixture::ft_set_type  ft_set_type;

    std_set_type std_set_tmp;
    ft_set_type ft_set_tmp;

    this->std_set.insert(std_set_tmp.begin(), std_set_tmp.end());
    this->ft_set.insert(ft_set_tmp.begin(), ft_set_tmp.end());
    COMPARE_TEST(this->ft_set, this->std_set);

    TestFixture::init(std_set_tmp, ft_set_tmp, this->key_gen);

    this->std_set.insert(std_set_tmp.begin(), std_set_tmp.end());
    this->ft_set.insert(ft_set_tmp.begin(), ft_set_tmp.end());
    COMPARE_TEST(this->ft_set, this->std_set);
}

TYPED_TEST_P(SetTest, Clear)
{
    this->init();

    this->ft_set.clear();

    EXPECT_TRUE(this->ft_set.empty());
    EXPECT_EQ(this->ft_set.size(), 0);
    EXPECT_EQ(this->ft_set.begin(), this->ft_set.end());
}

TYPED_TEST_P(SetTest, Erase)
{
    this->init();

    if (TestFixture::size > 2)
    {
        this->std_set.erase(this->std_set.begin());
        this->std_set.erase(++this->std_set.begin());
        this->std_set.erase(--this->std_set.end());

        this->ft_set.erase(this->ft_set.begin());
        this->ft_set.erase(++this->ft_set.begin());
        this->ft_set.erase(--this->ft_set.end());

        COMPARE_TEST(this->ft_set, this->std_set);
    }

    typename TestFixture::std_set_type::iterator std_begin = this->std_set.begin();
    typename TestFixture::std_set_type::iterator std_end   = this->std_set.end();
    typename TestFixture::ft_set_type::iterator  ft_begin  = this->ft_set.begin();
    typename TestFixture::ft_set_type::iterator  ft_end    = this->ft_set.end();

    while (std_begin != std_end)
    {
        ASSERT_NE(ft_begin, ft_end);
        this->std_set.erase(std_begin++);
        this->ft_set.erase(ft_begin++);
    }

    COMPARE_TEST(this->ft_set, this->std_set);
}

TYPED_TEST_P(SetTest, EraseRange)
{
    this->init();

    this->std_set.erase(this->std_set.begin(), this->std_set.begin());
    this->ft_set.erase(this->ft_set.begin(), this->ft_set.begin());
    COMPARE_TEST(this->ft_set, this->std_set);

    this->std_set.erase(this->std_set.end(), this->std_set.end());
    this->ft_set.erase(this->ft_set.end(), this->ft_set.end());
    COMPARE_TEST(this->ft_set, this->std_set);

    if (TestFixture::size < 4) return;

#ifndef __clang__
    this->std_set.erase(++this->std_set.begin(), --this->std_set.begin());
    this->ft_set.erase(++this->ft_set.begin(), --this->ft_set.begin());
    COMPARE_TEST(this->ft_set, this->std_set);
#endif // __clang__

    this->std_set.erase(this->std_set.begin(), this->std_set.end());
    this->ft_set.erase(this->ft_set.begin(), this->ft_set.end());
    COMPARE_TEST(this->ft_set, this->std_set);
}

TYPED_TEST_P(SetTest, EraseKey)
{
    this->init();

    typename TestFixture::std_set_type::iterator std_begin = this->std_set.begin();

    while (!this->std_set.empty())
    {
        typename TestFixture::key_type key = *std_begin;
        ++std_begin;
        ASSERT_EQ(this->std_set.erase(key), this->ft_set.erase(key));
    }
    COMPARE_TEST(this->ft_set, this->std_set);
}

TYPED_TEST_P(SetTest, Swap)
{
    this->init();

    typename TestFixture::std_set_type std_set2;
    typename TestFixture::ft_set_type  ft_set2;

    TestFixture::init(std_set2, ft_set2, this->key_gen);

    this->ft_set.swap(ft_set2);

    COMPARE_TEST(this->ft_set, std_set2);
    COMPARE_TEST(ft_set2, this->std_set);

    ft::swap(this->ft_set, ft_set2);

    COMPARE_TEST(this->ft_set, this->std_set);
    COMPARE_TEST(ft_set2, std_set2);
}

TYPED_TEST_P(SetTest, Count)
{
    typedef typename TestFixture::key_type key_type;

    const key_type key = key_type();
    EXPECT_EQ(this->std_set.count(key), this->ft_set.count(key));

    for (size_t i = 0; i < (TestFixture::size >> 1); ++i)
        this->key_gen.next();

    this->init();

    typename TestFixture::key_gen_type key_gen;
    size_t count_fails = 0;
    for (size_t i = 0; i < (TestFixture::size << 1); ++i)
    {
        count_fails += (this->std_set.count(key_gen.get()) !=
                        this->ft_set.count(key_gen.get()));
        key_gen.next();
    }

    EXPECT_EQ(count_fails, 0);

    this->std_set.clear();
    this->ft_set.clear();
    EXPECT_EQ(this->std_set.count(key), this->ft_set.count(key));
}

TYPED_TEST_P(SetTest, Find)
{
    typedef typename TestFixture::key_type key_type;

    const key_type key = key_type();
    EXPECT_EQ(this->ft_set.find(key), this->ft_set.end());

    size_t edge = TestFixture::size >> 1;
    for (size_t i = 0; i < edge; ++i)
        this->key_gen.next();

    this->init();

    const typename TestFixture::std_set_type::iterator std_end =
        this->std_set.end();
    const typename TestFixture::ft_set_type::iterator ft_end =
        this->ft_set.end();

    typename TestFixture::key_gen_type key_gen;
    size_t find_fails = 0;

    for (size_t i = 0; i < (TestFixture::size << 1); ++i)
    {
        typename TestFixture::std_set_type::iterator std_iter =
            this->std_set.find(key_gen.get());
        typename TestFixture::ft_set_type::iterator ft_iter =
            this->ft_set.find(key_gen.get());

        if (std_iter == std_end)
            find_fails += (ft_iter != ft_end);
        else
            find_fails += (*std_iter != *ft_iter);

        key_gen.next();
    }

    EXPECT_EQ(find_fails, 0);

    this->ft_set.clear();
    EXPECT_EQ(this->ft_set.find(key), this->ft_set.end());
}

TYPED_TEST_P(SetTest, EqualRange)
{
    typedef typename TestFixture::std_set_type::iterator std_iter_type;
    typedef typename TestFixture::ft_set_type::iterator  ft_iter_type;

    typedef typename std::pair<std_iter_type,std_iter_type> std_iter_pair_type;
    typedef typename ft::pair<ft_iter_type,ft_iter_type>    ft_iter_pair_type;

    typedef typename TestFixture::key_type key_type;

    const key_type key = key_type();
    EXPECT_EQ(this->ft_set.equal_range(key).first,
              this->ft_set.equal_range(key).second);

    size_t edge = TestFixture::size >> 1;
    for (size_t i = 0; i < edge; ++i)
        this->key_gen.next();

    this->init();

    typename TestFixture::key_gen_type key_gen;
    size_t equal_range_fails = 0;

    for (size_t i = 0; i < (TestFixture::size << 1); ++i)
    {
        std_iter_pair_type std = this->std_set.equal_range(key_gen.get());
        ft_iter_pair_type ft = this->ft_set.equal_range(key_gen.get());
        equal_range_fails += compare_iters(std.first, std.second,
                                           ft.first, ft.second);
        key_gen.next();
    }

    EXPECT_EQ(equal_range_fails, 0);

    this->ft_set.clear();
    EXPECT_EQ(this->ft_set.equal_range(key).first,
              this->ft_set.equal_range(key).second);
}

TYPED_TEST_P(SetTest, LowerBound)
{
    typedef typename TestFixture::key_type key_type;

    const key_type key = key_type();
    EXPECT_EQ(this->ft_set.lower_bound(key), this->ft_set.end());

    size_t edge = TestFixture::size >> 1;
    for (size_t i = 0; i < edge; ++i)
        this->key_gen.next();

    this->init();

    typename TestFixture::key_gen_type key_gen;
    size_t bound_fails = 0;

    for (size_t i = 0; i < edge; ++i)
    {
        bound_fails += (
            (this->std_set.lower_bound(key_gen.get()) == this->std_set.end()) !=
            (this->ft_set.lower_bound(key_gen.get()) == this->ft_set.end()));
        key_gen.next();
    }

    for (size_t i = edge; i < TestFixture::size + edge; ++i)
    {
        typename TestFixture::std_set_type::iterator std_iter =
            this->std_set.lower_bound(key_gen.get());
        typename TestFixture::ft_set_type::iterator  ft_iter =
            this->ft_set.lower_bound(key_gen.get());

        if (std_iter == this->std_set.end())
            bound_fails += (ft_iter != this->ft_set.end());
        else
            bound_fails += (*std_iter != *ft_iter);

        key_gen.next();
    }

    for (size_t i = TestFixture::size + edge; i < (TestFixture::size << 1); ++i)
    {
        bound_fails += (
            (this->std_set.lower_bound(key_gen.get()) == this->std_set.end()) !=
            (this->ft_set.lower_bound(key_gen.get()) == this->ft_set.end()));
        key_gen.next();
    }

    EXPECT_EQ(bound_fails, 0);

    this->ft_set.clear();
    EXPECT_EQ(this->ft_set.lower_bound(key), this->ft_set.end());
}

TYPED_TEST_P(SetTest, UpperBound)
{
    typedef typename TestFixture::key_type key_type;

    const key_type key = key_type();
    EXPECT_EQ(this->ft_set.upper_bound(key), this->ft_set.end());

    size_t edge = TestFixture::size >> 1;
    for (size_t i = 0; i < edge; ++i)
        this->key_gen.next();

    this->init();

    typename TestFixture::key_gen_type key_gen;
    size_t bound_fails = 0;

    for (size_t i = 0; i < edge; ++i)
    {
        bound_fails += (
            (this->std_set.upper_bound(key_gen.get()) == this->std_set.end()) !=
            (this->ft_set.upper_bound(key_gen.get()) == this->ft_set.end()));
        key_gen.next();
    }

    for (size_t i = edge; i < TestFixture::size + edge; ++i)
    {
        typename TestFixture::std_set_type::iterator std_iter =
            this->std_set.upper_bound(key_gen.get());
        typename TestFixture::ft_set_type::iterator  ft_iter =
            this->ft_set.upper_bound(key_gen.get());

        if (std_iter == this->std_set.end())
            bound_fails += (ft_iter != this->ft_set.end());
        else
            bound_fails += (*std_iter!= *ft_iter);

        key_gen.next();
    }

    for (size_t i = TestFixture::size + edge; i < (TestFixture::size << 1); ++i)
    {
        bound_fails += (
            (this->std_set.upper_bound(key_gen.get()) == this->std_set.end()) !=
            (this->ft_set.upper_bound(key_gen.get()) == this->ft_set.end()));
        key_gen.next();
    }

    EXPECT_EQ(bound_fails, 0);

    this->ft_set.clear();
    EXPECT_EQ(this->ft_set.upper_bound(key), this->ft_set.end());
}

TYPED_TEST_P(SetTest, CompareOperators)
{
    typename TestFixture::std_set_type std_set;
    typename TestFixture::ft_set_type  ft_set;

    typename TestFixture::std_set_type std_set2;
    typename TestFixture::ft_set_type  ft_set2;

    this->init();
    TestFixture::init(std_set, ft_set, this->key_gen);
    this->SetUp();
    TestFixture::init(std_set, ft_set, this->key_gen, TestFixture::size >> 1);

    EXPECT_EQ(this->std_set == this->std_set, this->ft_set == this->ft_set);
    EXPECT_EQ(this->std_set != this->std_set, this->ft_set != this->ft_set);
    EXPECT_EQ(this->std_set <  this->std_set, this->ft_set <  this->ft_set);
    EXPECT_EQ(this->std_set <= this->std_set, this->ft_set <= this->ft_set);
    EXPECT_EQ(this->std_set >  this->std_set, this->ft_set >  this->ft_set);
    EXPECT_EQ(this->std_set >= this->std_set, this->ft_set >= this->ft_set);

    EXPECT_EQ(this->std_set == std_set, this->ft_set == ft_set);
    EXPECT_EQ(this->std_set != std_set, this->ft_set != ft_set);
    EXPECT_EQ(this->std_set <  std_set, this->ft_set <  ft_set);
    EXPECT_EQ(this->std_set <= std_set, this->ft_set <= ft_set);
    EXPECT_EQ(this->std_set >  std_set, this->ft_set >  ft_set);
    EXPECT_EQ(this->std_set >= std_set, this->ft_set >= ft_set);

    EXPECT_EQ(this->std_set == std_set2, this->ft_set == ft_set2);
    EXPECT_EQ(this->std_set != std_set2, this->ft_set != ft_set2);
    EXPECT_EQ(this->std_set <  std_set2, this->ft_set <  ft_set2);
    EXPECT_EQ(this->std_set <= std_set2, this->ft_set <= ft_set2);
    EXPECT_EQ(this->std_set >  std_set2, this->ft_set >  ft_set2);
    EXPECT_EQ(this->std_set >= std_set2, this->ft_set >= ft_set2);
}

TYPED_TEST_P(SetTest, KeyCompare)
{
    typename TestFixture::std_set_type std_set;
    typename TestFixture::ft_set_type ft_set;

    this->init();
    TestFixture::init(std_set, ft_set, this->key_gen);

    typename TestFixture::std_set_type::key_compare std_kc;
    typename TestFixture::ft_set_type::key_compare  ft_kc;

    typename TestFixture::std_set_type::iterator std_begin = this->std_set.begin();
    typename TestFixture::std_set_type::iterator std_end = this->std_set.end();
    typename TestFixture::ft_set_type::iterator ft_begin = this->ft_set.begin();
    typename TestFixture::ft_set_type::iterator ft_end = this->ft_set.end();

    typename TestFixture::std_set_type::iterator std_begin2 = this->std_set.begin();
    typename TestFixture::std_set_type::iterator std_end2 = this->std_set.end();
    typename TestFixture::ft_set_type::iterator ft_begin2 = this->ft_set.begin();
    typename TestFixture::ft_set_type::iterator ft_end2 = this->ft_set.end();

    size_t compare_fails = 0;

    while (std_begin != std_end)
    {
        compare_fails += ((std_kc(*std_begin, *std_begin) !=
                           ft_kc(*ft_begin, *ft_begin))
                           || (std_kc(*std_begin, *std_begin2) !=
                           ft_kc(*ft_begin, *ft_begin2)));
        ++std_begin;
        ++std_begin2;
        ++ft_begin;
        ++ft_begin2;
    }

    EXPECT_EQ(compare_fails, 0);
}

TYPED_TEST_P(SetTest, ValueCompare)
{
    typename TestFixture::std_set_type std_set;
    typename TestFixture::ft_set_type ft_set;

    this->init();
    TestFixture::init(std_set, ft_set, this->key_gen);

    typename TestFixture::std_set_type::value_compare std_vc =
        this->std_set.value_comp();
    typename TestFixture::ft_set_type::value_compare  ft_vc =
        this->ft_set.value_comp();

    typename TestFixture::std_set_type::iterator std_begin = this->std_set.begin();
    typename TestFixture::std_set_type::iterator std_end = this->std_set.end();
    typename TestFixture::ft_set_type::iterator ft_begin = this->ft_set.begin();
    typename TestFixture::ft_set_type::iterator ft_end = this->ft_set.end();

    typename TestFixture::std_set_type::iterator std_begin2 = this->std_set.begin();
    typename TestFixture::std_set_type::iterator std_end2 = this->std_set.end();
    typename TestFixture::ft_set_type::iterator ft_begin2 = this->ft_set.begin();
    typename TestFixture::ft_set_type::iterator ft_end2 = this->ft_set.end();

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

REGISTER_TYPED_TEST_SUITE_P(SetTest,
// TESTS LIST
    Capacity,
    TypeDefs,
    Iterator,
    ReverseIterator,
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

INSTANTIATE_TYPED_TEST_SUITE_P(SetInstance, SetTest, TestTypes::TestSetTypes);
