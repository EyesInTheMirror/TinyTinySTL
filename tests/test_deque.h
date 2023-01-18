//
// Created by 镜瞳 on 2023/1/14.
//

#ifndef TINYTINYSTL_TEST_DEQUE_H
#define TINYTINYSTL_TEST_DEQUE_H
#include "../deque.h"

namespace tinystl::test::deque_test {
    void test_deque() {
        deque<int> dq(10000, 5);
        deque<int> dq2(3, 4);
        dq2[0] = 99;
        for(auto n : dq2) {
            std::cout << n << ' ';
        }
        std::cout << std::endl;
    }
}
#endif //TINYTINYSTL_TEST_DEQUE_H
