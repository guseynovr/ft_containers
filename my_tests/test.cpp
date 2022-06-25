#include "../vector.hpp"
#include <vector>
#include <algorithm>
#include <iostream>

void compare_vec(ft::vector<int> ft_vec, std::vector<int> std_vec);
void compare_vec2(std::vector<int> ft_vec, std::vector<int> std_vec);
void swap_test();

int _ratio = 10000;

int main()
{
    swap_test();
}

void swap_test() {
    std::vector<int> std_vec;
    std::vector<int> v_std;
    std_vec.assign(1100 * _ratio, 11);
    std::vector<int> tmp_std(500 * _ratio, 5), tmp2_std(1000 * _ratio, 10), tmp3_std(1500 * _ratio, 15), tmp4_std(3000 * _ratio, 30);
    v_std.push_back(std_vec[2]);
    v_std.push_back(std_vec.size());
    v_std.push_back(std_vec.capacity());
    long *adr1_std = reinterpret_cast<long *>(&std_vec);
    long *adr2_std = reinterpret_cast<long *>(&tmp_std);
    std_vec.swap(tmp_std);
    if (reinterpret_cast<long *>(&std_vec) == adr1_std && reinterpret_cast<long *>(&tmp_std) == adr2_std)
    	v_std.push_back(1);
    v_std.push_back(std_vec[2]);
    v_std.push_back(std_vec.size());
    v_std.push_back(std_vec.capacity());
    std_vec.swap(tmp3_std);
    v_std.push_back(std_vec[2]);
    v_std.push_back(std_vec.size());
    v_std.push_back(std_vec.capacity());
    printf("std_vec cap = %lu, tmp2_std cap = %lu\n", std_vec.capacity(), tmp2_std.capacity());
    std::swap(std_vec, tmp2_std);
    printf("AFTER SWAP: std_vec cap = %lu, tmp2_std cap = %lu\n", std_vec.capacity(), tmp2_std.capacity());
    v_std.push_back(std_vec[2]);
    v_std.push_back(std_vec.size());
    v_std.push_back(std_vec.capacity());
    // std::swap(std_vec, tmp4_std);
    // v_std.push_back(std_vec[2]);
    // v_std.push_back(std_vec.size());
    // v_std.push_back(std_vec.capacity());

    ft::vector<int> ft_vec;
    std::vector<int> v_ft;
    ft_vec.assign(1100 * _ratio, 11);
    ft::vector<int> tmp_ft(500 * _ratio, 5), tmp2_ft(1000 * _ratio, 10), tmp3_ft(1500 * _ratio, 15), tmp4_ft(3000 * _ratio, 30);
    v_ft.push_back(ft_vec[2]);
    v_ft.push_back(ft_vec.size());
    v_ft.push_back(ft_vec.capacity());
    long *adr1_ft = reinterpret_cast<long *>(&ft_vec);
    long *adr2_ft = reinterpret_cast<long *>(&tmp_ft);
    ft_vec.swap(tmp_ft);
    if (reinterpret_cast<long *>(&ft_vec) == adr1_ft && reinterpret_cast<long *>(&tmp_ft) == adr2_ft)
    	v_ft.push_back(1);
    v_ft.push_back(ft_vec[2]);
    v_ft.push_back(ft_vec.size());
    v_ft.push_back(ft_vec.capacity());
    ft_vec.swap(tmp3_ft);
    v_ft.push_back(ft_vec[2]);
    v_ft.push_back(ft_vec.size());
    v_ft.push_back(ft_vec.capacity());
    printf("ft_vec cap = %lu, tmp2_ft cap = %lu\n", ft_vec.capacity(), tmp2_ft.capacity());
    std::swap(ft_vec, tmp2_ft);
    printf("AFTER SWAP: ft_vec cap = %lu, tmp2_ft cap = %lu\n", ft_vec.capacity(), tmp2_ft.capacity());
    v_ft.push_back(ft_vec[2]);
    v_ft.push_back(ft_vec.size());
    v_ft.push_back(ft_vec.capacity());
    // std::swap(ft_vec, tmp4_ft);
    // v_ft.push_back(ft_vec[2]);
    // v_ft.push_back(ft_vec.size());
    // v_ft.push_back(ft_vec.capacity());

    compare_vec2(v_ft, v_std);
}

void compare_vec(ft::vector<int> ft_vec, std::vector<int> std_vec) 
{
    if (ft_vec.size() != std_vec.size()) std::cout << "size inequal\n";
    if (ft_vec.capacity() != std_vec.capacity()) std::cout << "capacity inequal\n";
    if (ft_vec.empty() != std_vec.empty()) std::cout << "empty inequal\n";

    std::vector<int>::iterator std_begin = std_vec.begin();
    std::vector<int>::iterator std_end   = std_vec.end();
    ft::vector<int>::iterator  ft_begin  = ft_vec.begin();
    ft::vector<int>::iterator  ft_end    = ft_vec.end();


    for ( ; std_begin != std_end; ++std_begin, ++ft_begin)
    {
        // if (*ft_begin != *std_begin) {
        //     std::cout << "value inequal: ft = " << *ft_begin
        //         << ", std = " << *std_begin << "\n";
        // }
        std::cout << "values: ft = " << *ft_begin
            << ", std = " << *std_begin << "\n";
    }
    std::cout << "compare finished\n";
}

void compare_vec2(std::vector<int> ft_vec, std::vector<int> std_vec) 
{
    if (ft_vec.size() != std_vec.size()) std::cout << "size inequal\n";
    if (ft_vec.capacity() != std_vec.capacity()) std::cout << "capacity inequal\n";
    if (ft_vec.empty() != std_vec.empty()) std::cout << "empty inequal\n";

    std::vector<int>::iterator std_begin = std_vec.begin();
    std::vector<int>::iterator std_end   = std_vec.end();
    std::vector<int>::iterator  ft_begin  = ft_vec.begin();
    std::vector<int>::iterator  ft_end    = ft_vec.end();


    for ( ; std_begin != std_end; ++std_begin, ++ft_begin)
    {
        if (*ft_begin != *std_begin) {
            std::cout << "value inequal: ft = " << *ft_begin
                << ", std = " << *std_begin << "\n";
        }
        // std::cout << "values: ft = " << *ft_begin
        //     << ", std = " << *std_begin << "\n";
    }
    std::cout << "compare finished\n";
}