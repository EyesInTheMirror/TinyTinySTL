//
// Created by 镜瞳 on 2022/12/13.
//

#ifndef TINYTINYSTL_CONSTRUCT_H
#define TINYTINYSTL_CONSTRUCT_H
//construct.h做两件事，construct和destroy，也就是对象的构造和析构
#include <new>
#include <type_traits>
#include <utility>

namespace tinystl{
    //construct()所做的是在ptr所指的空间上构造一个T类型的对象
    template <typename T>
    void construct(T* ptr){
        new((void*)ptr) T();
    }

    template <typename T1, typename T2>
    void construct(T1* ptr, T2& val){
        new((void*)ptr) T1(val);
    }

    template <typename T1, typename... T2>
    void construct(T1* ptr, T2&&... args){
        new((void*)ptr) T1(std::forward<T2>(args)...);
    }

    //destroy()所做的是将指定位置的对象析构
    //当这个类型存在trivial析构函数时，不需要做任何事，提高效率
    template <typename T>
    void destroy_one(T* ptr, std::true_type){}

    template <typename T>
    void destroy_one(T* ptr, std::false_type){
        if(ptr != nullptr){
            ptr->~T();
        }
    }

    template <typename ForwardIter>
    void destroy_more(ForwardIter first, ForwardIter last, std::true_type){}

    template <typename ForwardIter>
    void destroy_more(ForwardIter first, ForwardIter last, std::false_type){
        for(;first != last; ++first){
            destroy_one(&*first, false);
        }
    }

    template <typename T>
    void destroy(T* ptr){
        destroy_one(ptr, std::is_trivially_destructible<T>{});
    }

    //还没写到迭代器部分不知道怎么萃取迭代器所指的类型，所以先用T占位
    template <typename ForwardIter, typename T>
    void destroy(ForwardIter first, ForwardIter last){
        destroy_more(first, last, std::is_trivially_destructible<T>{});
    }

}
#endif //TINYTINYSTL_CONSTRUCT_H
