//
// Created by 镜瞳 on 2022/12/29.
//

#ifndef TINYTINYSTL_UNINITIALIZED_H
#define TINYTINYSTL_UNINITIALIZED_H
#include "construct.h"
#include <type_traits>
//暂时用一下
#include <algorithm>
namespace tinystl
{//uninitialized_fill
template<typename ForwardIterator, typename T>
ForwardIterator uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
                                       const T &val, std::true_type) {
    //fill必须要在trivially copy assignable的前提下才能用，否则只能用construct
    std::fill(first, last, val);
}

template<typename ForwardIterator, typename T>
ForwardIterator uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
                                       const T &val, std::false_type) {
    auto cur = first;
    try {
        for (; cur != last; ++cur) {
            //&*cur确保拿到的是实际对象位置的地址
            tinystl::construct(&*cur, val);
        }
    }
    catch (...) {
        //一旦发生异常，就rollback
        for (; first != cur; ++first) {
            tinystl::destroy(&*first);
        }
    }
}

template<typename ForwardIterator, typename T>
ForwardIterator uninitialized_fill(ForwardIterator first, ForwardIterator last, const T &val) {
    uninitialized_fill_aux(first, last, val,
                           std::is_pod<typename tinystl::iterator_traits<ForwardIterator>::value_type>::value);
}

//uninitialized_fill_n，要返回填充结束的位置
template<typename ForwardIterator, typename Size, typename T>
ForwardIterator uninitialized_fill_aux_n(ForwardIterator first, Size n,
                                         const T &val, std::true_type) {
    //fill必须要在trivially copy assignable的前提下才能用，否则只能用construct
    return std::fill_n(first, n, val);
}

template<typename ForwardIterator, typename Size, typename T>
ForwardIterator uninitialized_fill_aux_n(ForwardIterator first, Size n,
                                         const T &val, std::false_type) {
    auto cur = first;
    try {
        for (; n > 0; --n, ++cur) {
            //&*cur确保拿到的是实际对象位置的地址
            tinystl::construct(&*cur, val);
        }
    }
    catch (...) {
        //一旦发生异常，就rollback
        for (; first != cur; ++first) {
            tinystl::destroy(&*first);
        }
    }
    return cur;
}

template<typename ForwardIterator, typename Size, typename T>
ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T &val) {
    uninitialized_fill_aux(first, n, val,
                           std::is_pod<typename tinystl::iterator_traits<ForwardIterator>::value_type>::value);
}

}
#endif //TINYTINYSTL_UNINITIALIZED_H
