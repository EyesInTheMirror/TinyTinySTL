//
// Created by 镜瞳 on 2023/1/14.
//

#ifndef TINYTINYSTL_TEST_DEQUE_H
#define TINYTINYSTL_TEST_DEQUE_H
#include "../deque.h"
#include <algorithm>
#include <iostream>
namespace tinystl::test::deque_test {
    template <typename T>
    void print_deque(const deque<T>& d) {
        std::cout << "deque: ";
        if(d.size() < 100) {
            for(const auto& n : d) {
                std::cout << n << ' ';
            }
            std::cout << std::endl;
        }
        else std::cout << "too large! skip printing..." << std::endl;
        std::cout << "size = " << d.size() << std::endl;
    }
    void test_deque() {
        //构造
        deque<int> dq(10000, 5);
        deque<int> dq2(3, 4);
        print_deque(dq);
        print_deque(dq2);
        //读取
        dq2[2] = 66;
        auto tmp = dq2.end() - 1;
        std::cout << *tmp << std::endl;
        dq2[0] = 99;
        std::cout << "front = " << dq2.front() << ", back = " << dq2.back() << std::endl;
        //插入
        dq2.push_back(100);
        dq2.push_front(8);
        print_deque(dq2);
        auto itr = std::find(dq2.begin(), dq2.end(), 99);
        std::cout << *itr << std::endl;
        //删除
        dq2.pop_back();
        dq2.pop_front();
        print_deque(dq2);
        dq2.erase(dq2.begin() + 1);
        print_deque(dq2);
        dq2.clear();
        dq2.push_front(1);
        print_deque(dq2);
    }
}
#endif //TINYTINYSTL_TEST_DEQUE_H
