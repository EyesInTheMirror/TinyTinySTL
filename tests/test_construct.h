//
// Created by 镜瞳 on 2022/12/13.
//

#ifndef TINYTINYSTL_TEST_CONSTRUCT_H
#define TINYTINYSTL_TEST_CONSTRUCT_H
#include "../construct.h"
#include <iostream>
namespace tinystl::test::construct_test{
    class Foo{
        int a;
    public:
        Foo():a(0){}
        explicit Foo(int val):a(val){}
        //[[nodiscard]]指的是如果调用这个函数的时候我没有接收这个返回值编译器会给予警告
        [[nodiscard]] int getA() const{return a;}
    };

    class Foo2{
        Foo* ptr;
    public:
        Foo2():ptr(new Foo()){}
        explicit Foo2(int val):ptr(new Foo(val)){}
        ~Foo2(){delete ptr;ptr = nullptr;}
        [[nodiscard]]Foo* getPtr(){return ptr;}
    };
    void test_construct(){
        //默认析构函数的情况
        Foo* ptr = new Foo(1000);
        int val = 10;
        tinystl::construct(ptr, val);
        std::cout << ptr->getA() <<std::endl;
        tinystl::destroy(ptr);
        std::cout << ptr->getA() <<std::endl;//这里依然能读到，因为无事发生
        //非默认析构函数的情况
        Foo2* ptr2 = new Foo2(2000);
        std::cout << ptr2->getPtr()->getA() << std::endl;
        tinystl::construct(ptr2);
        std::cout << ptr2->getPtr()->getA() << std::endl;
        tinystl::destroy(ptr2);
        if(ptr2->getPtr() == nullptr)
            std::cout << "the ptr has been destroyed" << std::endl;
        else
            std::cout << ptr2->getPtr()->getA() << std::endl;
        delete ptr;
        delete ptr2;
    }
}

#endif //TINYTINYSTL_TEST_CONSTRUCT_H
