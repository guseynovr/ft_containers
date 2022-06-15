#include <gtest/gtest.h>
#include <typeinfo>
#include "test_types.h"

#include <iostream>

/// defines

#define COMPARE_TEST(ft_vec, std_vec) \
{\
    ASSERT_EQ(ft_vec.size(), std_vec.size());\
    ASSERT_EQ(ft_vec.capacity(), std_vec.capacity());\
    ASSERT_EQ(ft_vec.empty(), std_vec.empty());\
\
    typename TestFixture::std_vec_type::iterator std_begin = std_vec.begin();\
    typename TestFixture::std_vec_type::iterator std_end   = std_vec.end();\
    typename TestFixture::ft_vec_type::iterator  ft_begin  = ft_vec.begin();\
    typename TestFixture::ft_vec_type::iterator  ft_end    = ft_vec.end();\
\
    size_t compare_fails = 0;\
\
    for ( ; std_begin != std_end;\
        ++std_begin,\
        ++ft_begin)\
    {\
        ASSERT_NE(ft_begin, ft_end);\
        compare_fails += (*ft_begin != *std_begin);\
    }\
\
    EXPECT_EQ(compare_fails, 0);\
}

/// fixture

template <typename _Defines>
struct VectorTest : public ::testing::Test
{
    typedef typename _Defines::value_type value_type;

    typedef std::vector<value_type> std_vec_type;
    typedef ft::vector<value_type>  ft_vec_type;

    typedef typename _Defines::generator_type gen_type;

    static void init(std_vec_type& std_vec,
                     ft_vec_type& ft_vec,
                     gen_type& gen,
                     const size_t size = VectorTest::size)
    {
        //std_vec.reserve(size);
        //ft_vec.reserve(size);

        for (size_t i = 0; i < size; ++i)
        {
            std_vec.push_back(gen.get());
            ft_vec.push_back(gen.get());
            gen.next();
        }
    }

    void init(const size_t size = VectorTest::size)
    {
        VectorTest::init(std_vec, ft_vec, gen, size);
    }

    void SetUp()
    {
        gen.reset();
    }

    static const size_t size = _Defines::size;

    static gen_type gen;

    std_vec_type std_vec;
    ft_vec_type  ft_vec;
};

template <typename _Defines>
typename _Defines::generator_type VectorTest<_Defines>::gen;

TYPED_TEST_SUITE_P(VectorTest);

/// Tests

TYPED_TEST_P(VectorTest, Capacity)
{
    EXPECT_EQ(this->std_vec.empty(), this->ft_vec.empty());
    EXPECT_EQ(this->std_vec.size(), this->ft_vec.size());
    EXPECT_EQ(this->std_vec.max_size(), this->ft_vec.max_size());
    EXPECT_EQ(this->std_vec.capacity(), this->ft_vec.capacity());
}

TYPED_TEST_P(VectorTest, TypeDefs)
{
    typedef typename TestFixture::std_vec_type std_vec_type;
    typedef typename TestFixture::ft_vec_type  ft_vec_type;

    EXPECT_EQ(typeid(typename std_vec_type::value_type),
              typeid(typename ft_vec_type::value_type));
    EXPECT_EQ(typeid(typename std_vec_type::allocator_type),
              typeid(typename ft_vec_type::allocator_type));
    EXPECT_EQ(typeid(typename std_vec_type::size_type),
              typeid(typename ft_vec_type::size_type));
    EXPECT_EQ(typeid(typename std_vec_type::difference_type),
              typeid(typename ft_vec_type::difference_type));
    EXPECT_EQ(typeid(typename std_vec_type::reference),
              typeid(typename ft_vec_type::reference));
    EXPECT_EQ(typeid(typename std_vec_type::const_reference),
              typeid(typename ft_vec_type::const_reference));
    EXPECT_EQ(typeid(typename std_vec_type::pointer),
              typeid(typename ft_vec_type::pointer));
    EXPECT_EQ(typeid(typename std_vec_type::const_pointer),
              typeid(typename ft_vec_type::const_pointer));

    EXPECT_NE(typeid(typename std_vec_type::iterator),
              typeid(typename ft_vec_type::iterator));
    EXPECT_NE(typeid(typename std_vec_type::const_iterator),
              typeid(typename ft_vec_type::const_iterator));
    EXPECT_NE(typeid(typename std_vec_type::reverse_iterator),
              typeid(typename ft_vec_type::reverse_iterator));
    EXPECT_NE(typeid(typename std_vec_type::const_reverse_iterator),
              typeid(typename ft_vec_type::const_reverse_iterator));
}

TYPED_TEST_P(VectorTest, Iterator)
{
    this->init();

    typename TestFixture::std_vec_type::iterator std_begin = this->std_vec.begin();
    typename TestFixture::ft_vec_type::iterator ft_begin = this->ft_vec.begin();
    typename TestFixture::std_vec_type::iterator std_end = this->std_vec.end();
    typename TestFixture::ft_vec_type::iterator ft_end = this->ft_vec.end();

    EXPECT_EQ(ft_begin + TestFixture::size, ft_end);

    while (std_begin != std_end)
    {
        ASSERT_NE(ft_begin, ft_end);
        ASSERT_EQ(*std_begin, *ft_begin);
        ++std_begin;
        ++ft_begin;
    }

    ASSERT_EQ(ft_begin, ft_end);

    std_end -= TestFixture::size + 1;
    ft_end -= TestFixture::size + 1;
    ASSERT_EQ(ft_end, this->ft_vec.begin() - 1);
    --std_begin;
    --ft_begin;

    while (std_begin != std_end)
    {
        ASSERT_NE(ft_begin, ft_end);
        ASSERT_EQ(*std_begin, *ft_begin);
        --std_begin;
        --ft_begin;
    }

    ASSERT_EQ(ft_begin, ft_end);
    if (TestFixture::size > 0)
    {
        ASSERT_LT(ft_begin, this->ft_vec.begin());
        ASSERT_EQ(ft_begin[TestFixture::size], this->ft_vec.end()[-1]);
    }
}

TYPED_TEST_P(VectorTest, ReverseIterator)
{
    this->init();

    typename TestFixture::std_vec_type::reverse_iterator std_rbegin =
        this->std_vec.rbegin();
    typename TestFixture::ft_vec_type::reverse_iterator ft_rbegin =
        this->ft_vec.rbegin();
    typename TestFixture::std_vec_type::reverse_iterator std_rend =
        this->std_vec.rend();
    typename TestFixture::ft_vec_type::reverse_iterator ft_rend =
        this->ft_vec.rend();

    EXPECT_EQ(ft_rbegin + TestFixture::size, ft_rend);

    while (std_rbegin != std_rend)
    {
        ASSERT_NE(ft_rbegin, ft_rend);
        ASSERT_EQ(*std_rbegin, *ft_rbegin);
        ++std_rbegin;
        ++ft_rbegin;
    }

    ASSERT_EQ(ft_rbegin, ft_rend);

    std_rend -= TestFixture::size + 1;
    ft_rend -= TestFixture::size + 1;
    ASSERT_EQ(ft_rend, this->ft_vec.rbegin() - 1);
    --std_rbegin;
    --ft_rbegin;

    while (std_rbegin != std_rend)
    {
        ASSERT_NE(ft_rbegin, ft_rend);
        ASSERT_EQ(*std_rbegin, *ft_rbegin);
        --std_rbegin;
        --ft_rbegin;
    }

    ASSERT_EQ(ft_rbegin, ft_rend);
    if (TestFixture::size > 0)
    {
        ASSERT_LT(ft_rbegin, this->ft_vec.rbegin());
        ASSERT_EQ(ft_rbegin[TestFixture::size], this->ft_vec.rend()[-1]);
    }
}

TYPED_TEST_P(VectorTest, AssignOperator)
{
    typename TestFixture::std_vec_type std_vec;
    typename TestFixture::ft_vec_type ft_vec;

    this->init();
    std_vec = this->std_vec;
    ft_vec = this->ft_vec;

    COMPARE_TEST(ft_vec, std_vec);

    std_vec.clear();
    ft_vec.clear();
    TestFixture::init(std_vec, ft_vec, this->gen);
    std_vec = this->std_vec;
    ft_vec = this->ft_vec;

    COMPARE_TEST(ft_vec, std_vec);
}

TYPED_TEST_P(VectorTest, Assign)
{
    typename TestFixture::std_vec_type std_vec;
    typename TestFixture::ft_vec_type ft_vec;

    this->init();

    this->std_vec.assign(TestFixture::size << 1, this->gen.get());
    this->ft_vec.assign(TestFixture::size << 1, this->gen.get());
    COMPARE_TEST(ft_vec, std_vec);

    this->std_vec.assign(0, this->gen.get());
    this->ft_vec.assign(0, this->gen.get());
    COMPARE_TEST(ft_vec, std_vec);
}

TYPED_TEST_P(VectorTest, AssignRange)
{
    typename TestFixture::std_vec_type std_vec;
    typename TestFixture::ft_vec_type ft_vec;

    this->init();
    std_vec.assign(this->std_vec.begin(), this->std_vec.end());
    ft_vec.assign(this->ft_vec.begin(), this->ft_vec.end());

    COMPARE_TEST(ft_vec, std_vec);

    std_vec.assign(std_vec.begin(), std_vec.end());
    ft_vec.assign(ft_vec.begin(), ft_vec.end());

    COMPARE_TEST(ft_vec, std_vec);
}

TYPED_TEST_P(VectorTest, FillConstructor)
{
    typename TestFixture::std_vec_type std_vec(TestFixture::size);
    typename TestFixture::ft_vec_type ft_vec(TestFixture::size);

    COMPARE_TEST(ft_vec, std_vec);
}

TYPED_TEST_P(VectorTest, RangeConstructor)
{
    this->init();

    typename TestFixture::std_vec_type
        std_vec(this->std_vec.begin(), this->std_vec.end());
    typename TestFixture::ft_vec_type
        ft_vec(this->ft_vec.begin(), this->ft_vec.end());

    COMPARE_TEST(ft_vec, std_vec);
}

TYPED_TEST_P(VectorTest, CopyConstructor)
{
    this->init();

    typename TestFixture::std_vec_type std_vec(this->std_vec);
    typename TestFixture::ft_vec_type ft_vec(this->ft_vec);

    COMPARE_TEST(ft_vec, std_vec);
}

TYPED_TEST_P(VectorTest, AccessOperator)
{
    this->init();

    for (size_t i = 0; i < TestFixture::size; ++i)
    {
        this->std_vec[i] = this->gen.get();
        this->ft_vec[i] = this->gen.get();
        this->gen.next();
    }

    COMPARE_TEST(this->ft_vec, this->std_vec);
}

TYPED_TEST_P(VectorTest, Front)
{
    if (TestFixture::size == 0) return;

    this->init();

    this->std_vec.front() = this->gen.get();
    this->ft_vec.front() = this->gen.get();
    EXPECT_EQ(this->ft_vec.front(), this->std_vec.front());

    while(this->std_vec.size())
    {
        ASSERT_EQ(this->ft_vec.front(), this->std_vec.front());
        this->std_vec.erase(this->std_vec.begin());
        this->ft_vec.erase(this->ft_vec.begin());
    }
}

TYPED_TEST_P(VectorTest, Back)
{
    if (TestFixture::size == 0) return;

    this->init();

    this->std_vec.back() = this->gen.get();
    this->ft_vec.back() = this->gen.get();
    EXPECT_EQ(this->ft_vec.back(), this->std_vec.back());

    while(this->std_vec.size())
    {
        ASSERT_EQ(this->ft_vec.back(), this->std_vec.back());
        this->std_vec.erase(this->std_vec.end() - 1);
        this->ft_vec.erase(this->ft_vec.end() - 1);
    }
}

TYPED_TEST_P(VectorTest, Insert)
{
    this->init();
    COMPARE_TEST(this->ft_vec, this->std_vec);
}

TYPED_TEST_P(VectorTest, InsertFill)
{
    this->init();

    this->std_vec.insert(this->std_vec.end(), 0, this->gen.get());
    this->ft_vec.insert(this->ft_vec.end(), 0, this->gen.get());
    this->std_vec.insert(this->std_vec.begin(), 0, this->gen.get());
    this->ft_vec.insert(this->ft_vec.begin(), 0, this->gen.get());

    COMPARE_TEST(this->ft_vec, this->std_vec);

    this->std_vec.insert(this->std_vec.begin(),
        TestFixture::size, this->gen.get());
    this->ft_vec.insert(this->ft_vec.begin(),
        TestFixture::size, this->gen.get());

    COMPARE_TEST(this->ft_vec, this->std_vec);
}

TYPED_TEST_P(VectorTest, InsertRange)
{
    this->init();

    typename TestFixture::std_vec_type std_vec;
    typename TestFixture::ft_vec_type ft_vec;

    this->std_vec.insert(this->std_vec.begin(), std_vec.begin(), std_vec.end());
    this->ft_vec.insert(this->ft_vec.begin(), ft_vec.begin(), ft_vec.end());

    COMPARE_TEST(this->ft_vec, this->std_vec);

    TestFixture::init(std_vec, ft_vec, this->gen);

    this->std_vec.insert(this->std_vec.begin(), std_vec.begin(), std_vec.end());
    this->ft_vec.insert(this->ft_vec.begin(), ft_vec.begin(), ft_vec.end());

    COMPARE_TEST(this->ft_vec, this->std_vec);
}

TYPED_TEST_P(VectorTest, Clear)
{
    this->init();

    this->ft_vec.clear();

    EXPECT_TRUE(this->ft_vec.empty());
    EXPECT_EQ(this->ft_vec.size(), 0);
    EXPECT_EQ(this->ft_vec.capacity(), this->std_vec.capacity());
    EXPECT_EQ(this->ft_vec.begin(), this->ft_vec.end());
}

TYPED_TEST_P(VectorTest, Erase)
{
    this->init();

    if (TestFixture::size > 2)
    {
        this->std_vec.erase(this->std_vec.begin());
        this->std_vec.erase(this->std_vec.begin() + 1);
        this->std_vec.erase(this->std_vec.end() - 1);

        this->ft_vec.erase(this->ft_vec.begin());
        this->ft_vec.erase(this->ft_vec.begin() + 1);
        this->ft_vec.erase(this->ft_vec.end() - 1);

        COMPARE_TEST(this->ft_vec, this->std_vec);
    }

    while (this->std_vec.size())
    {
        ASSERT_NE(this->ft_vec.begin(), this->ft_vec.end());
        this->std_vec.erase(this->std_vec.begin());
        this->ft_vec.erase(this->ft_vec.begin());
    }

    COMPARE_TEST(this->ft_vec, this->std_vec);
}

TYPED_TEST_P(VectorTest, EraseRange)
{
    this->init();

    this->std_vec.erase(this->std_vec.begin(), this->std_vec.begin());
    this->ft_vec.erase(this->ft_vec.begin(), this->ft_vec.begin());
    COMPARE_TEST(this->ft_vec, this->std_vec);

    this->std_vec.erase(this->std_vec.end(), this->std_vec.end());
    this->ft_vec.erase(this->ft_vec.end(), this->ft_vec.end());
    COMPARE_TEST(this->ft_vec, this->std_vec);

    this->std_vec.erase(this->std_vec.begin(), this->std_vec.end());
    this->ft_vec.erase(this->ft_vec.begin(), this->ft_vec.end());
    COMPARE_TEST(this->ft_vec, this->std_vec);
}

TYPED_TEST_P(VectorTest, PushBack)
{
    for (size_t i = 0; i < TestFixture::size; ++i)
    {
        this->std_vec.push_back(this->gen.get());
        this->ft_vec.push_back(this->gen.get());
        this->gen.next();
        EXPECT_EQ(this->std_vec.size(), this->ft_vec.size());
        EXPECT_EQ(this->std_vec.capacity(), this->ft_vec.capacity());
    }

    COMPARE_TEST(this->ft_vec, this->std_vec);
}

TYPED_TEST_P(VectorTest, PopBack)
{
    this->init();

    while (this->std_vec.size())
    {
        ASSERT_NE(this->ft_vec.begin(), this->ft_vec.end());
        this->std_vec.pop_back();
        this->ft_vec.pop_back();
    }

    COMPARE_TEST(this->ft_vec, this->std_vec);
}

TYPED_TEST_P(VectorTest, Resize)
{
    this->init();

    this->std_vec.resize(TestFixture::size >> 1);
    this->ft_vec.resize(TestFixture::size >> 1);
    COMPARE_TEST(this->ft_vec, this->std_vec);

    this->std_vec.resize(TestFixture::size << 1, this->gen.get());
    this->ft_vec.resize(TestFixture::size << 1, this->gen.get());
    COMPARE_TEST(this->ft_vec, this->std_vec);
}

TYPED_TEST_P(VectorTest, Swap)
{
    this->init();

    typename TestFixture::std_vec_type std_vec;
    typename TestFixture::ft_vec_type ft_vec;

    for (size_t i = 0; i < TestFixture::size; ++i)
    {
        std_vec.push_back(this->gen.get());
        ft_vec.push_back(this->gen.get());
        this->gen.next();
    }

    this->ft_vec.swap(ft_vec);
    COMPARE_TEST(this->ft_vec, std_vec);
    COMPARE_TEST(ft_vec, this->std_vec);

    this->ft_vec.swap(ft_vec);
    COMPARE_TEST(this->ft_vec, this->std_vec);
    COMPARE_TEST(ft_vec, std_vec);
}

TYPED_TEST_P(VectorTest, CompareOperators)
{
    typename TestFixture::std_vec_type std_vec;
    typename TestFixture::ft_vec_type  ft_vec;

    typename TestFixture::std_vec_type std_vec2;
    typename TestFixture::ft_vec_type  ft_vec2;

    this->init();
    TestFixture::init(std_vec, ft_vec, this->gen);
    this->SetUp();
    TestFixture::init(std_vec, ft_vec, this->gen, TestFixture::size >> 1);

    EXPECT_EQ(this->std_vec == this->std_vec, this->ft_vec == this->ft_vec);
    EXPECT_EQ(this->std_vec != this->std_vec, this->ft_vec != this->ft_vec);
    EXPECT_EQ(this->std_vec <  this->std_vec, this->ft_vec <  this->ft_vec);
    EXPECT_EQ(this->std_vec <= this->std_vec, this->ft_vec <= this->ft_vec);
    EXPECT_EQ(this->std_vec >  this->std_vec, this->ft_vec >  this->ft_vec);
    EXPECT_EQ(this->std_vec >= this->std_vec, this->ft_vec >= this->ft_vec);

    EXPECT_EQ(this->std_vec == std_vec, this->ft_vec == ft_vec);
    EXPECT_EQ(this->std_vec != std_vec, this->ft_vec != ft_vec);
    EXPECT_EQ(this->std_vec <  std_vec, this->ft_vec <  ft_vec);
    EXPECT_EQ(this->std_vec <= std_vec, this->ft_vec <= ft_vec);
    EXPECT_EQ(this->std_vec >  std_vec, this->ft_vec >  ft_vec);
    EXPECT_EQ(this->std_vec >= std_vec, this->ft_vec >= ft_vec);

    EXPECT_EQ(this->std_vec == std_vec2, this->ft_vec == ft_vec2);
    EXPECT_EQ(this->std_vec != std_vec2, this->ft_vec != ft_vec2);
    EXPECT_EQ(this->std_vec <  std_vec2, this->ft_vec <  ft_vec2);
    EXPECT_EQ(this->std_vec <= std_vec2, this->ft_vec <= ft_vec2);
    EXPECT_EQ(this->std_vec >  std_vec2, this->ft_vec >  ft_vec2);
    EXPECT_EQ(this->std_vec >= std_vec2, this->ft_vec >= ft_vec2);
}

/// Registration of tests and instantiation of types

REGISTER_TYPED_TEST_SUITE_P(VectorTest,
/// TESTS LIST
    Capacity,
    TypeDefs,
    Iterator,
    ReverseIterator,
    AssignOperator,
    Assign,
    AssignRange,
    // constructors
    FillConstructor,
    RangeConstructor,
    CopyConstructor,
    // element access
    AccessOperator,
    Front,
    Back,
    // modifiers
    Insert,
    InsertFill,
    InsertRange,
    Clear,
    Erase,
    EraseRange,
    PushBack,
    PopBack,
    Resize,
    Swap,
    // non-member
    CompareOperators
);

INSTANTIATE_TYPED_TEST_SUITE_P(VectorInstance, VectorTest, TestTypes::TestVectorTypes);
