//
// Created by 10986 on 2022/11/30.
//

#ifndef TINYTINYSTL_TEST_ALLOCATOR_H
#define TINYTINYSTL_TEST_ALLOCATOR_H

#include "../allocator.h"
#include <vector>
#include <iostream>
#include <array>

namespace tinystl::test::allocator_test {
    void test_allocator() {
        const int arr_len = 5;
        std::array<int, arr_len> ia = {0, 1, 2, 3, 4};

        std::vector<int, tinystl::allocator<int>> iv(ia.begin(), ia.end());
        for (auto i: iv) {
            std::cout << i << ' ';
        }
        std::cout << std::endl;
        const int iv_size = 10, iv_val = 5;
        std::vector<int, tinystl::allocator<int>> iv2(iv_size, iv_val);
        for (auto i: iv2) {
            std::cout << i << ' ';
        }
        std::cout << std::endl;
    }
}

#endif //TINYTINYSTL_TEST_ALLOCATOR_H
