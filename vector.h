//
// Created by 镜瞳 on 2022/12/28.
//

#ifndef TINYTINYSTL_VECTOR_H
#define TINYTINYSTL_VECTOR_H
#include "allocator.h"
#include "iterator.h"
#include <cstddef>
//暂时用std::copy()
#include <algorithm>

namespace tinystl {
template<typename T, typename Alloc = tinystl::allocator<T> >
class vector {
public:
    using value_type = T;
    using pointer = T *;
    using iterator = T *;
    using reference = T &;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
protected:
    using data_allocator = typename tinystl::allocator<value_type>;

    iterator _start;
    iterator _finish;
    iterator _end_of_storage;

    //还没学到这个函数，但是盲猜是要做一个搬家扩容
    void insert_aux(iterator position, const T &x);

    void deallocate() {
        if (_start) {
            data_allocator::deallocate(_start, _end_of_storage - _start);
        }
    }

    void fill_initialize(size_type n, const T& value) {
        _start = allocate_and_fill(n, value);
        _finish = _start + n;
        _end_of_storage = _finish;
    }

    iterator allocate_and_fill(size_type n, const T& val) {
        //申请一片内存空间，并返回起始地址
        iterator start = data_allocator::allocate(n);
        //暂时用一下
        std::uninitialized_fill_n(start, n, val);
        return start;
    }

public:
    //提取容器的信息
    iterator begin() { return _start; }

    iterator end() { return _finish; }

    size_type size() const { return static_cast<size_type>(_finish - _start); }

    size_type capacity() const { return static_cast<size_type>(_end_of_storage - _start); }

    bool empty() { return begin() == end(); }

    reference front() { return *_start; }

    reference back() { return *_finish - 1; }

    reference operator[](size_type n) { return *(begin() + n); }

    //一系列构造、析构函数
    vector() : _start(nullptr), _finish(nullptr), _end_of_storage(nullptr) {}

    //一个参数的构造函数声明为explicit阻止参数的隐式转换
    //调用对应类型的默认构造函数
    explicit vector(size_type n) { fill_initialize(n, T()); }

    vector(size_type n, const T &value) { fill_initialize(n, value); }

    //why?
    vector(int n, const T &value) { fill_initialize(n, value); }

    vector(long n, const T &value) { fill_initialize(n, value); }

    ~vector() {
        //第一步，析构所有已有的对象
        tinystl::destroy(_start, _finish);
        //第二步，回收所有内存
        deallocate();
    }

    //修改容器的数据的一系列成员函数
    void push_back(const T &val) {
        if (_finish != _end_of_storage) {
            tinystl::construct(_finish, val);
            ++_finish;
        } else {
            //如果当前已经申请到的空间不足以放下新的元素了，执行这个函数
            insert_aux(end(), val);
        }
    }

    void pop_back() {
        --_finish;
        tinystl::destroy(_finish);
    }

    iterator erase(iterator pos) {
        if (pos + 1 != end()) {
            //把pos后面的都复制到pos的位置来
            std::copy(pos + 1, _finish, pos);
        }
        --_finish;
        data_allocator::destroy(_finish);
        //原来的迭代器会失效，不要erase(iter++)，大概率不是咱想要的效果
        return pos;
    }

    //记得定义
    iterator erase(iterator first, iterator last);

    void clear() { erase(begin(), end());}
    void resize(size_type new_size, const T& val) {
        if(new_size < size())
            erase(begin() + new_size, end());
        else
            //从end开始，插这么多个val
            insert(end(), new_size - size(), val);
    }

    void resize(size_type new_size){
        resize(new_size, T());
    }
};

}

#endif //TINYTINYSTL_VECTOR_H
