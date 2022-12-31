//
// Created by 镜瞳 on 2022/12/28.
//

#ifndef TINYTINYSTL_VECTOR_H
#define TINYTINYSTL_VECTOR_H
#include "memory.h"
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

    //插入的辅助函数，空间不足时搬家扩容
    void insert_aux(iterator position, const T &x);

    void deallocate() {
        if (_start) {
            data_allocator::deallocate(_start, _end_of_storage - _start);
        }
    }

    //用于构造函数
    void fill_initialize(size_type n, const T& value) {
        _start = allocate_and_fill(n, value);
        _finish = _start + n;
        _end_of_storage = _finish;
    }

    iterator allocate_and_fill(size_type n, const T& val) {
        //申请一片内存空间，并返回起始地址
        iterator start = data_allocator::allocate(n);
        tinystl::uninitialized_fill_n(start, n, val);
        return start;
    }

public:
    //提取容器的信息
    iterator begin() { return _start; }

    iterator end() { return _finish; }

    [[nodiscard]] size_type size() const { return static_cast<size_type>(_finish - _start); }

    [[nodiscard]] size_type capacity() const { return static_cast<size_type>(_end_of_storage - _start); }

    bool empty() { return begin() == end(); }

    reference front() { return *_start; }

    reference back() { return *(_finish - 1); }

    reference operator[](size_type n) { return *(begin() + n); }

    //一系列构造、析构函数
    vector() : _start(nullptr), _finish(nullptr), _end_of_storage(nullptr) {}

    //一个参数的构造函数声明为explicit阻止参数的隐式转换
    //调用对应类型的默认构造函数
    explicit vector(size_type n) { fill_initialize(n, T()); }

    vector(size_type n, const T &value) { fill_initialize(n, value); }

    //为什么要特地把int和long拎出来
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

    //先放一下
    iterator erase(iterator first, iterator last) {
        std::copy(last + 1, _finish, first);
        _finish -= last - first + 1;
        data_allocator::destroy(_finish, _finish + last - first);
        return first;
    }

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

    template<typename T, typename Alloc>
    void vector<T, Alloc>::insert_aux(vector::iterator position, const T &x) {
        if(_finish != _end_of_storage) {//这函数不只给push_back用，这里还得判断
            //在最后一个元素的后一个位置构造一个最后元素
            tinystl::construct(_finish, *(_finish - 1));
            ++_finish;
            //太他妈细节了，必须要确保存储下来的是元素的一个副本,不然以后修改容器里面的值影响容器外面岂不是乱套
            T x_copy = x;
            //position后面整体右移
            std::copy_backward(position, _finish - 2, _finish - 1);
            *position = x_copy;
        }
        else {
            const size_type old_size = size();
            //如果原空间为0，新空间为1，否则为两倍的原空间
            //注意这里的新空间指的是分配的内存，跟size()没关系
            const size_type len = old_size != 0 ? 2 * old_size : 1;
            iterator new_start = data_allocator::allocate(len);
            iterator new_finish = new_start;
            try {
                //别忘了uninitialized系列主打的是构造与内存分配的分离～
                //首先把插入位置之前的对象拷贝到新位置去
                new_finish = std::uninitialized_copy(_start, position, new_start);
                //构造插入对象
                tinystl::construct(new_finish, x);
                ++new_finish;
                //继续把插入位置后面的拷贝到新位置去，接在插入对象后面
                new_finish = std::uninitialized_copy(position, _finish, new_finish);
            }
            catch (...) {
                //要么做完，要么一个别做！
                tinystl::destroy(new_start, new_finish);
                data_allocator::deallocate(new_start, len);
                throw;
            }
            //至此，新空间准备好了，老空间彻底失去了价值
            destroy(begin(), end());
            deallocate();
            //新的头尾指针干过去
            _start = new_start;
            _finish = new_finish;
            _end_of_storage = new_start + len;
        }
    }

}

#endif //TINYTINYSTL_VECTOR_H
