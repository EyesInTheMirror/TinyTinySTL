//
// Created by 镜瞳 on 2023/1/14.
//

#ifndef TINYTINYSTL_TEST_DEQUE_H
#define TINYTINYSTL_TEST_DEQUE_H
#include "../deque.h"
#include <algorithm>
#include <iostream>
namespace tinystl::test::deque_test {
    void test_deque() {
        deque<int> dq(10000, 5);
        deque<int> dq2(3, 4);
        auto tmp = dq2.end() - 1;
        std::cout << *tmp << std::endl;
        dq2[0] = 99;
        dq2.push_back(100);
        dq2.push_front(200);
        dq2.push_front(999);
        dq2.clear();
        dq2.push_front(1);
        for(auto n : dq2) {
            std::cout << n << ' ';
        }
        std::cout << "size = " << dq2.size() << std::endl;
        auto itr = std::find(dq2.begin(), dq2.end(), 1);
        std::cout << *itr << std::endl;
    }
}
#endif //TINYTINYSTL_TEST_DEQUE_H
