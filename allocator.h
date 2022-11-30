//
// Created by 10986 on 2022/11/29.
// 空间配置器
//

#ifndef TINYTINYSTL_ALLOCATOR_H
#define TINYTINYSTL_ALLOCATOR_H

#include <new>
#include <cstddef>
#include <cstdlib>
#include <climits>
#include <iostream>

namespace tinystl {
    template<typename T>
    inline T *_allocate(ptrdiff_t size, T *) {
        std::set_new_handler(nullptr);//放弃异常处理，分配失败时抛出bad_alloc异常
        //分配size个T类型的空间，返回给T类型指针tmp
        T *tmp = (T *) ::operator new((size_t) (size * sizeof(T)));
        if (tmp == nullptr) {
            std::cerr << "out of memory" << std::endl;
            exit(1);
        }
        return tmp;
    }

    template<typename T>
    inline void _deallocate(T *buffer) {
        ::operator delete(buffer);
    }

    template<typename T1, typename T2>
    inline void _construct(T1 *p, const T2 &value) {
        new(p) T1(value); //在指针p指向的内存空间创建一个T1类型的对象
    }

    template<typename T>
    inline void _destroy(T *ptr) {
        ptr->~T();
    }

    template<typename T>
    class allocator {
    public:
        using value_type = T;
        using pointer = T *;
        using const_pointer = const T *;
        using reference = T &;
        using const_reference = const T &;
        using size_type = size_t;
        using difference_type = ptrdiff_t;

    public:
        template<typename U>
        struct rebind {
            using other = allocator<U>; //这是在干啥。。
        };

        pointer allocate(size_type n, const void *hint = nullptr) {
            return _allocate((difference_type) n, (pointer) 0);
        }

        void deallocate(pointer p, size_type n) { _deallocate(p); }

        void construct(pointer p, const_reference value) {
            _construct(p, value);
        }

        void destroy(pointer p) { _destroy(p); }

        pointer address(reference x) { return (pointer) &x; }

        const_pointer const_address(const_reference x) { return (const_pointer) &x; }

        //[[nodiscard]] C++17 引入，返回值“不应舍弃”
        [[nodiscard]] size_type max_size() const {
            return size_type(UINT_MAX / sizeof(T)); //据说是限制分配空间时nums的大小
        }
    };

}

#endif //TINYTINYSTL_ALLOCATOR_H
