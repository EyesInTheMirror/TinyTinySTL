//
// Created by 镜瞳 on 2022/12/29.
//

#ifndef TINYTINYSTL_UNINITIALIZED_H
#define TINYTINYSTL_UNINITIALIZED_H

#include "construct.h"
#include <type_traits>
//暂时用一下
#include <algorithm>

namespace tinystl {//uninitialized_fill
    template<typename ForwardIterator, typename T>
    void uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
                                           const T &val, std::true_type) {
        //fill必须要在trivially copy assignable的前提下才能用，否则只能用construct
        std::fill(first, last, val);
    }

    template<typename ForwardIterator, typename T>
    void uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
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
    void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T &val) {
        using value_type = typename tinystl::iterator_traits<ForwardIterator>::value_type;
        uninitialized_fill_aux(first, last, val,
                               std::is_pod<value_type>{});
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
        return uninitialized_fill_aux_n(first, n, val,
                               std::is_pod<typename tinystl::iterator_traits<ForwardIterator>::value_type>{});
    }

    template<typename InputIterator, typename ForwardIterator>
    ForwardIterator uninitialized_copy_aux(InputIterator first, InputIterator last,
                                           ForwardIterator dest, std::true_type) {
        return std::copy(first, last, dest);
    }

    template<typename InputIterator, typename ForwardIterator>
    ForwardIterator uninitialized_copy_aux(InputIterator first, InputIterator last,
                                           ForwardIterator dest, std::false_type) {
        ForwardIterator cur = dest;
        try {
            for (; first != last; ++first, ++cur) {
                tinystl::construct(&*cur, *first);
            }
        }
        catch (...) {
            for (; cur != dest; --cur) {
                tinystl::destroy(&*cur);
            }
        }
        return cur;
    }


    //返回的是复制过去的区间尾部
    template<typename InputIterator, typename ForwardIterator>
    ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator dest) {
        typedef typename tinystl::iterator_traits<InputIterator>::value_type value_type;
        return uninitialized_copy_aux(first, last, dest, std::is_pod<value_type>{});
    }
}


#endif //TINYTINYSTL_UNINITIALIZED_H
