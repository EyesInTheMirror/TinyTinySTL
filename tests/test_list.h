//
// Created by 镜瞳 on 2023/1/7.
//

#ifndef TINYTINYSTL_TEST_LIST_H
#define TINYTINYSTL_TEST_LIST_H
#include "../list.h"
#include <string>
namespace tinystl::test::list_test {
    template<typename T>
    void print_list(list<T>& l) {
        for(const auto& n : l) {
            std::cout << n << ' ';
        }
        std::cout << std::endl;
    }

    void test_list() {
        using std::cout;
        using std::endl;
        int iv[5] = {1, 2, 3, 4, 5};
        int iv2[3] = {99, 99, 99};
        list<int> l(iv, iv + 5);
        list<int> l2(iv2, iv2 + 3);
        *(l2.begin()) = 10;
        print_list(l2);
        auto itr = std::find(l2.begin(), l2.end(), 99);
        l2.splice(itr, l);
        print_list(l2);
    }
}
#endif //TINYTINYSTL_TEST_LIST_H
