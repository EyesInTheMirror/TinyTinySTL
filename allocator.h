//
// Created by 10986 on 2022/11/29.
// 空间配置器
//

#ifndef TINYTINYSTL_ALLOCATOR_H
#define TINYTINYSTL_ALLOCATOR_H

#include <new>
#include "construct.h"


namespace tinystl {
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
        //与类的实例无关的成员函数，声明为静态成员函数
        static pointer allocate();
        static pointer allocate(size_type n);

        static void deallocate(pointer ptr);
        static void deallocate(pointer ptr, size_type n);

        static void construct(pointer ptr);
        static void construct(pointer ptr, const_reference val);
        static void construct(pointer ptr, T&& value);

        template<typename ... T2>
        static void construct(pointer ptr, T2&&... args);

        static void destroy(pointer ptr);
        static void destroy(pointer first, pointer last);
    };

    template<typename T>
    T* allocator<T>::allocate() {
        return static_cast<T*>(::operator new(sizeof(T)));
    }

    template<typename T>
    T* allocator<T>::allocate(size_type n) {
        if(n == 0) return nullptr;
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    template<typename T>
    void allocator<T>::deallocate(pointer ptr) {
        if(ptr == nullptr) return;
        ::operator delete(ptr);
    }

    template<typename T>
    void allocator<T>::deallocate(pointer ptr, size_type n) {
        if(ptr == nullptr) return;
        ::operator delete(ptr);
    }

    template<typename T>
    void allocator<T>::construct(pointer ptr) {
        tinystl::construct(ptr);
    }

    template<typename T>
    void allocator<T>::construct(pointer ptr, const_reference val) {
        tinystl::construct(ptr, val);
    }

    template<typename T>
    void allocator<T>::construct(pointer ptr, T &&value) {
        tinystl::construct(ptr, std::move(value));
    }

    template<typename T>
    template<typename... T2>
    void allocator<T>::construct(pointer ptr, T2 &&... args) {
        tinystl::construct(ptr, std::forward<T2>(args)...);
    }

    template<typename T>
    void allocator<T>::destroy(pointer ptr) {
        tinystl::destroy(ptr);
    }

    template<typename T>
    void allocator<T>::destroy(pointer first, pointer last) {
        tinystl::destroy(first, last);
    }
}

#endif //TINYTINYSTL_ALLOCATOR_H
