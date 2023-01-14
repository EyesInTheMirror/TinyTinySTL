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
        using std::cout;
        using std::endl;
        //构造
        int iv[5] = {1, 2, 3, 4, 5};
        list<int> l(iv, iv + 5);
        list<int> l2(3, 99);
        print_list(l);
        print_list(l2);
        std::cout << "------------------" << std::endl;

        //元素存取
        cout << "l.front() = " << l.front() << endl;
        cout << "l.back() = " << l.back() << endl;
        cout << "l.size() = " << l.size() << endl;
        *(l2.begin()) = 10;
        print_list(l2);
        std::cout << "------------------" << std::endl;

        //插入
        l.push_back(100);
        l.push_front(100);
        auto itr = std::find(l.begin(), l.end(), 3);
        l.insert(itr, 100);
        print_list(l);
        std::cout << "------------------" << std::endl;

        //删除
        l.erase(l.begin());
        print_list(l);
        l.push_back(100);
        l.push_back(100);
        print_list(l);
        l.unique();
        print_list(l);
        l.remove(100);
        print_list(l);
        std::cout << "------------------" << std::endl;

        //骚操作
        auto itr2 = std::find(l2.begin(), l2.end(), 99);
        l2.splice(itr2, l);
        if(l.empty()) cout << "l is now empty!" << endl;
        print_list(l2);
        l2.reverse();
        print_list(l2);
        l2.sort();
        print_list(l2);
    }
}
#endif //TINYTINYSTL_TEST_LIST_H
