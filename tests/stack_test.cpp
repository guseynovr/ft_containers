#include <gtest/gtest.h>
#include <typeinfo>
#include "test_types.h"

/// defines

#define COMPARE_TEST(ft_stack, std_stack) \
{\
    __typeof__(std_stack) std(std_stack);\
    __typeof__(ft_stack) ft(ft_stack);\
\
    ASSERT_EQ(ft.size(), std.size());\
    ASSERT_EQ(ft.empty(), std.empty());\
\
    size_t compare_fails = 0;\
\
    while (std.size())\
    {\
        ASSERT_FALSE(ft.empty());\
        compare_fails += (ft.top() != std.top());\
        std.pop();\
        ft.pop();\
    }\
\
    EXPECT_EQ(compare_fails, 0);\
}

/// fixture

template <typename _Defines>
struct StackTest : public ::testing::Test
{
    typedef typename _Defines::value_type value_type;

    typedef std::stack<value_type, ft::vector<value_type> > std_stack_type;
    typedef ft::stack<value_type>                           ft_stack_type;

    typedef typename _Defines::generator_type gen_type;

    static void init(std_stack_type& std_stack,
                     ft_stack_type& ft_stack,
                     gen_type& gen,
                     const size_t size = StackTest::size)
    {
        for (size_t i = 0; i < size; ++i)
        {
            std_stack.push(gen.get());
            ft_stack.push(gen.get());
            gen.next();
        }
    }

    void init(const size_t size = StackTest::size)
    {
        StackTest::init(std_stack, ft_stack, gen, size);
    }

    void SetUp()
    {
        gen.reset();
    }

    static const size_t size = _Defines::size;

    static gen_type gen;

    std_stack_type std_stack;
    ft_stack_type  ft_stack;
};

template <typename _Defines>
typename _Defines::generator_type StackTest<_Defines>::gen;

TYPED_TEST_SUITE_P(StackTest);

/// Tests

TYPED_TEST_P(StackTest, Capacity)
{
    EXPECT_EQ(this->std_stack.empty(), this->ft_stack.empty());
    EXPECT_EQ(this->std_stack.size(), this->ft_stack.size());
}

TYPED_TEST_P(StackTest, TypeDefs)
{
    typedef typename TestFixture::std_stack_type std_stack_type;
    typedef typename TestFixture::ft_stack_type  ft_stack_type;

    EXPECT_EQ(typeid(typename std_stack_type::value_type),
              typeid(typename ft_stack_type::value_type));
    EXPECT_EQ(typeid(typename std_stack_type::container_type),
              typeid(typename ft_stack_type::container_type));
    EXPECT_EQ(typeid(typename std_stack_type::size_type),
              typeid(typename ft_stack_type::size_type));
    EXPECT_EQ(typeid(typename std_stack_type::reference),
              typeid(typename ft_stack_type::reference));
    EXPECT_EQ(typeid(typename std_stack_type::const_reference),
              typeid(typename ft_stack_type::const_reference));
}

TYPED_TEST_P(StackTest, AssignOperator)
{
    typename TestFixture::std_stack_type std_stack;
    typename TestFixture::ft_stack_type ft_stack;

    this->init();
    std_stack = this->std_stack;
    ft_stack = this->ft_stack;

    COMPARE_TEST(ft_stack, std_stack);

    typename TestFixture::std_stack_type std_stack2;
    typename TestFixture::ft_stack_type ft_stack2;
    TestFixture::init(std_stack, ft_stack, this->gen);
    std_stack2 = this->std_stack;
    ft_stack2 = this->ft_stack;

    COMPARE_TEST(ft_stack2, std_stack2);
}

TYPED_TEST_P(StackTest, DefaultConstructor)
{
    ft::vector<typename TestFixture::value_type> v;
    v.reserve(TestFixture::size);

    for (size_t i = 0; i < TestFixture::size; ++i)
    {
        v.push_back(this->gen.get());
        this->gen.next();
    }

    typename TestFixture::std_stack_type std_stack(v);
    typename TestFixture::ft_stack_type ft_stack(v);

    COMPARE_TEST(ft_stack, std_stack);
}

TYPED_TEST_P(StackTest, CopyConstructor)
{
    this->init();

    typename TestFixture::std_stack_type std_stack(this->std_stack);
    typename TestFixture::ft_stack_type ft_stack(this->ft_stack);

    COMPARE_TEST(ft_stack, std_stack);
}

TYPED_TEST_P(StackTest, Top)
{
    if (TestFixture::size == 0) return;

    this->init();

    this->std_stack.top() = this->gen.get();
    this->ft_stack.top() = this->gen.get();
    EXPECT_EQ(this->ft_stack.top(), this->std_stack.top());

    while(this->std_stack.size())
    {
        ASSERT_EQ(this->ft_stack.top(), this->std_stack.top());
        this->std_stack.pop();
        this->ft_stack.pop();
    }
}

TYPED_TEST_P(StackTest, Push)
{
    for (size_t i = 0; i < TestFixture::size; ++i)
    {
        this->std_stack.push(this->gen.get());
        this->ft_stack.push(this->gen.get());
        this->gen.next();
        EXPECT_EQ(this->std_stack.size(), this->ft_stack.size());
    }

    COMPARE_TEST(this->ft_stack, this->std_stack);
}

TYPED_TEST_P(StackTest, Pop)
{
    this->init();

    while(this->std_stack.size())
    {
        ASSERT_EQ(this->ft_stack.size(), this->std_stack.size());
        this->std_stack.pop();
        this->ft_stack.pop();
    }

    EXPECT_TRUE(this->ft_stack.empty());
    EXPECT_EQ(this->ft_stack.size(), 0);
}

TYPED_TEST_P(StackTest, CompareOperators)
{
    typename TestFixture::std_stack_type std_stack;
    typename TestFixture::ft_stack_type  ft_stack;

    typename TestFixture::std_stack_type std_stack2;
    typename TestFixture::ft_stack_type  ft_stack2;

    this->init();
    TestFixture::init(std_stack, ft_stack, this->gen);
    this->SetUp();
    TestFixture::init(std_stack, ft_stack, this->gen, TestFixture::size >> 1);

    EXPECT_EQ(this->std_stack == this->std_stack, this->ft_stack == this->ft_stack);
    EXPECT_EQ(this->std_stack != this->std_stack, this->ft_stack != this->ft_stack);
    EXPECT_EQ(this->std_stack <  this->std_stack, this->ft_stack <  this->ft_stack);
    EXPECT_EQ(this->std_stack <= this->std_stack, this->ft_stack <= this->ft_stack);
    EXPECT_EQ(this->std_stack >  this->std_stack, this->ft_stack >  this->ft_stack);
    EXPECT_EQ(this->std_stack >= this->std_stack, this->ft_stack >= this->ft_stack);

    EXPECT_EQ(this->std_stack == std_stack, this->ft_stack == ft_stack);
    EXPECT_EQ(this->std_stack != std_stack, this->ft_stack != ft_stack);
    EXPECT_EQ(this->std_stack <  std_stack, this->ft_stack <  ft_stack);
    EXPECT_EQ(this->std_stack <= std_stack, this->ft_stack <= ft_stack);
    EXPECT_EQ(this->std_stack >  std_stack, this->ft_stack >  ft_stack);
    EXPECT_EQ(this->std_stack >= std_stack, this->ft_stack >= ft_stack);

    EXPECT_EQ(this->std_stack == std_stack2, this->ft_stack == ft_stack2);
    EXPECT_EQ(this->std_stack != std_stack2, this->ft_stack != ft_stack2);
    EXPECT_EQ(this->std_stack <  std_stack2, this->ft_stack <  ft_stack2);
    EXPECT_EQ(this->std_stack <= std_stack2, this->ft_stack <= ft_stack2);
    EXPECT_EQ(this->std_stack >  std_stack2, this->ft_stack >  ft_stack2);
    EXPECT_EQ(this->std_stack >= std_stack2, this->ft_stack >= ft_stack2);
}

/// Registration of tests and instantiation of types

REGISTER_TYPED_TEST_SUITE_P(StackTest,
/// TESTS LIST
    Capacity,
    TypeDefs,
    AssignOperator,
    DefaultConstructor,
    CopyConstructor,
    Top,
    Push,
    Pop,
    CompareOperators
);

INSTANTIATE_TYPED_TEST_SUITE_P(StackInstance, StackTest, TestTypes::TestVectorTypes);
