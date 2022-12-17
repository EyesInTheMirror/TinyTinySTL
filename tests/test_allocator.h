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
        std::array<int, 5> ia = {0, 1, 2, 3, 4};

        std::vector<int, tinystl::allocator<int>> iv(ia.begin(), ia.end());
        for (auto i: iv) {
            std::cout << i << ' ';
        }
        std::cout << std::endl;
        std::vector<int, tinystl::allocator<int>> iv2(10, 5);
        for (auto i: iv2) {
            std::cout << i << ' ';
        }
        std::cout << std::endl;
    }
}

#endif //TINYTINYSTL_TEST_ALLOCATOR_H
