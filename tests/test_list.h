//
// Created by 镜瞳 on 2023/1/7.
//

#ifndef TINYTINYSTL_TEST_LIST_H
#define TINYTINYSTL_TEST_LIST_H
#include "../list.h"

namespace tinystl::test::list_test {
    template<typename T>
    void print_list(list<T>& l) {
        for(const auto& n : l) {
            std::cout << n << ' ';
        }
        std::cout << std::endl;
    }

    void test_list() {

        int iv[5] = {1, 2, 3, 4, 5};
        list<int> l(iv, iv + 5);
        list<int> l2(3, 99);
        *(l2.begin()) = 10;
        print_list(l2);
        auto itr = std::find(l2.begin(), l2.end(), 99);

        l2.splice(itr, l);
        print_list(l2);
        l2.reverse();
        print_list(l2);
        std::cout << "------------------" << std::endl;
        l2.swap(l);
        print_list(l);
        print_list(l2);
        l2.swap(l);
        print_list(l);
        print_list(l2);
        std::cout << "------------------" << std::endl;
        l2.pop_front();
        l2.sort();
        print_list(l2);
    }
}
#endif //TINYTINYSTL_TEST_LIST_H
