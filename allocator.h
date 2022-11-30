//
// Created by 10986 on 2022/11/29.
// 空间配置器
//

#ifndef TINYSTL_ALLOCATOR_H
#define TINYSTL_ALLOCATOR_H

#include <new>
#include <cstddef>
#include <cstdlib>
#include <climits>
#include <iostream>

namespace tinystl{

    template <typename T>
    class allocator{
    public:
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using size_type = size_t;
        using difference_type = ptrdiff_t;

    public:

    };

    template <typename T>
    inline T* _allocate(ptrdiff_t size, T*){
        std::set_new_handler(nullptr);//放弃异常处理，分配失败时抛出bad_alloc异常
        T* tmp = ::operator new
    }
}

#endif //TINYSTL_ALLOCATOR_H
