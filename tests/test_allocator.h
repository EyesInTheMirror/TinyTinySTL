//
// Created by 10986 on 2022/11/30.
//

#ifndef TINYTINYSTL_TEST_ALLOCATOR_H
#define TINYTINYSTL_TEST_ALLOCATOR_H

#include "../allocator.h"
#include <vector>

namespace tinystl::test::allocator_test {
    void test_allocator() {
        int ia[5] = {0, 1, 2, 3, 4};

        std::vector<int, tinystl::allocator<int>> iv(ia, ia + 5);
        for (auto i: iv) {
            std::cout << i << ' ';
        }
        std::cout << std::endl;
    }
}

#endif //TINYTINYSTL_TEST_ALLOCATOR_H
