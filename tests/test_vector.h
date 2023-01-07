//
// Created by 镜瞳 on 2022/12/29.
//

#ifndef TINYTINYSTL_TEST_VECTOR_H
#define TINYTINYSTL_TEST_VECTOR_H
#include "../vector.h"
#include <string>
namespace tinystl::test::vector_test {

    using std::string;
    using std::cout;
    using std::endl;
    using tinystl::vector;

    class Person{
    public:
        Person() = default;
        Person(string& name, int age):m_Age(age), m_Name(name) {}
        Person(const char* name, int age):m_Age(age), m_Name(name) {}
        ~Person()= default;
        friend std::ostream& operator<<(std::ostream& os, const Person& p);
        string m_Name;
        int m_Age{};
    };

    std::ostream& operator<<(std::ostream& os, const Person& p) {
        os << p.m_Name << ' ' << p.m_Age;
        return os;
    }

    void test_vector() {
        //调用vector构造函数构造5个Person对象
        vector<Person> pv(5, Person("yzh", 22));
        cout << "size = " << pv.size() << ", " << "capacity = " << pv.capacity() << endl;

        //调用push_back方法在容器尾部插入数据
        pv.push_back(Person("wqy", 100));
        cout << "size = " << pv.size() << ", " << "capacity = " << pv.capacity() << endl;

        //通过operator[]取元素，以及front，back元素的访问
        pv[0].m_Name = "ywt";
        cout << "front = " << pv.front() << ' ' << "back = " << pv.back() << endl;

        //调用insert像迭代器指定位置插入元素
        auto itr = pv.begin() + 2;
        auto itr2 = pv.insert(itr, 1, Person("nxl", 3));
        cout << *itr2 << endl;

        //用pop和erase删除元素
        pv.pop_back();
        cout << "after pop: back = " << *(pv.end() - 1) << endl;
        cout << "before erase: ";
        for(const auto& n : pv) {
            cout << n.m_Name << ' ';
        }
        cout << endl;
        pv.erase(pv.end() - 2, pv.end() - 1);
        pv.erase(pv.end() - 3);
        cout << "after erase: ";
        for(const auto& n : pv) {
            cout << n.m_Name << ' ';
        }
        cout << endl;

        //用resize更改容器的大小
        cout << "size = " << pv.size() << ", " << "capacity = " << pv.capacity() << endl;
        if(!pv.empty()) cout << "not empty" << endl;
        else cout << "empty" << endl;
        pv.resize(20, Person("null", 1000));
        cout << "size = " << pv.size() << ", " << "capacity = " << pv.capacity() << endl;
        for(const auto& n : pv) {
            cout << n.m_Name << ' ';
        }
        cout << endl;
        pv.resize(3);
        cout << "size = " << pv.size() << ", " << "capacity = " << pv.capacity() << endl;
        pv.clear();
        cout << "size = " << pv.size() << ", " << "capacity = " << pv.capacity() << endl;
        if(!pv.empty()) cout << "not empty" << endl;
        else cout << "empty" << endl;
    }
}
#endif //TINYTINYSTL_TEST_VECTOR_H
