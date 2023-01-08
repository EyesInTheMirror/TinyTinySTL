//
// Created by 镜瞳 on 2023/1/7.
//

#ifndef TINYTINYSTL_TEST_LIST_H
#define TINYTINYSTL_TEST_LIST_H
#include "../list.h"
#include <string>
namespace tinystl::test::list_test {
    class Person{
    public:
        Person(): m_Name("null"), m_Age(0) {};
        Person(std::string& name, int age):m_Age(age), m_Name(name) {}
        Person(const char* name, int age):m_Age(age), m_Name(name) {}
        ~Person()= default;
        friend std::ostream& operator<<(std::ostream& os, const Person& p);
        std::string m_Name;
        int m_Age{};
    };

    std::ostream& operator<<(std::ostream& os, const Person& p) {
        os << p.m_Name << ' ' << p.m_Age;
        return os;
    }

    void test_list() {
        using std::cout;
        using std::endl;
        //list<int> l(5, 100);
        //*(l.begin()) = 99;
        list<Person> l2(5, Person("yzh", 22));
        list<Person> l(5);
        (++l.begin())->m_Name = "nxl";
        auto itr = l.begin();
        advance(itr, 1);
        cout << *itr << endl;
        for(const auto& n : l) {
            cout << n << ' ';
        }
    }
}
#endif //TINYTINYSTL_TEST_LIST_H
