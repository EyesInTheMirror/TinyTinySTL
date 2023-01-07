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

    //以下是一些工具函数
    //插入的辅助函数，空间不足时搬家扩容
    void insert_aux(iterator position, const T &x);
    //回收分配的所有内存
    void deallocate() {
        if (_start) {
            data_allocator::deallocate(_start, _end_of_storage - _start);
        }
    }
    //用于构造函数，初始化元素头尾和空间尾指针
    void fill_initialize(size_type n, const T& value) {
        _start = allocate_and_fill(n, value);
        _finish = _start + n;
        _end_of_storage = _finish;
    }
    //fill_initialize用，申请空间，并构造元素
    iterator allocate_and_fill(size_type n, const T& val) {
        //申请一片内存空间，并返回起始地址
        iterator start = data_allocator::allocate(n);
        tinystl::uninitialized_fill_n(start, n, val);
        return start;
    }

public:

    //提取头尾迭代器
    iterator begin() { return _start; }
    iterator end() { return _finish; }

    //提取大小与容量
    [[nodiscard]] size_type size() const { return static_cast<size_type>(_finish - _start); }
    [[nodiscard]] size_type capacity() const { return static_cast<size_type>(_end_of_storage - _start); }
    bool empty() { return begin() == end(); }

    //提取特定的元素
    reference front() { return *_start; }
    reference back() { return *(_finish - 1); }
    reference operator[](size_type n) { return *(begin() + n); }

    //构造函数，容器的初始化
    vector() : _start(nullptr), _finish(nullptr), _end_of_storage(nullptr) {}
    //一个参数的构造函数声明为explicit阻止参数的隐式转换
    explicit vector(size_type n) { fill_initialize(n, T()); }
    vector(size_type n, const T &value) { fill_initialize(n, value); }

    //析构函数，析构所有对象并回收内存
    ~vector() {
        tinystl::destroy(_start, _finish);
        deallocate();
    }

    //向容器插入元素
    void push_back(const T &val);
    iterator insert(iterator pos, size_type n, const T& val);

    //删除容器内的元素
    void pop_back() {
        --_finish;
        tinystl::destroy(_finish);
    }
    iterator erase(iterator pos);
    iterator erase(iterator first, iterator last);
    void clear() { erase(begin(), end());}

    //改变容器的大小
    void resize(size_type new_size, const T& val) {
        if(new_size < size())
            erase(begin() + new_size, end());
        else
            insert(end(), new_size - size(), val);
    }
    void resize(size_type new_size){
        resize(new_size, T());
    }
};

    template<typename T, typename Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(vector::iterator first, vector::iterator last) {
        //把删除元素区间后面的有效元素移到前面来，覆盖掉删除元素，
        // 返回复制元素末尾的后一个位置
        iterator it = std::copy(last, _finish, first);
        //细节，虽然不知道it在前还是last在前，但我能确定的是从it开始到finish的一定都不要
        tinystl::destroy(it, _finish);
        //删掉多少finish前移多少
        _finish = _finish - (last - first);
        //返回删掉元素的后一个位置
        return first;
    }

    template<typename T, typename Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(vector::iterator pos) {
        if (pos + 1 != end()) {
            //把pos后面的都复制到pos的位置来
            std::copy(pos + 1, _finish, pos);
        }
        --_finish;
        data_allocator::destroy(_finish);
        //原来的迭代器会失效，不要erase(iter++)，大概率不是咱想要的效果
        return pos;
    }

    template<typename T, typename Alloc>
    void vector<T, Alloc>::push_back(const T &val) {
        if (_finish != _end_of_storage) {
            tinystl::construct(_finish, val);
            ++_finish;
        } else {
            //如果当前已经申请到的空间不足以放下新的元素了，执行这个函数
            insert_aux(end(), val);
        }
    }

    template<typename T, typename Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(vector::iterator pos, vector::size_type n, const T &val) {
        if(n == 0) return pos;
        //用来定位插入元素的第一位
        const difference_type offset = pos - begin();
        if(static_cast<size_type>(_end_of_storage - _finish) >= n) {
            //情况1：备用的空间大于待插入元素个数
            T val_copy = val;
            //插入位置后面有多少元素
            const size_type elems_after = _finish - pos;
            iterator old_finish = _finish;
            //如果后面的元素比要插入的多，意味着需要构造的只有原来的元素
            if(elems_after > n) {
                //空出n个位置，那么后n个需要构造
                tinystl::uninitialized_copy(_finish - n, _finish, _finish);
                _finish += n;
                //把剩下的后移n位，注意第三个参数的含义！！
                //first, last	-	the range of the elements to copy from
                //d_last	-	the end of the destination range
                std::copy_backward(pos, old_finish - n, old_finish);
                //空出来的填元素就可以了
                std::fill(pos, pos + n, val_copy);
            }
            //如果插入位置后面的元素比插入的少，意味着后面的元素要全部前往未构造区域，同时会有插入元素需要构造
            else {
                //想象一下后面元素搬迁后的场景，中间会有一些没构造的区域，先给他干上去一部分插入元素
                tinystl::uninitialized_fill_n(_finish, n - elems_after, val_copy);
                _finish += n - elems_after;
                //随后开始大迁徙
                tinystl::uninitialized_copy(pos, old_finish, _finish);
                _finish += elems_after;
                //最后鸠占鹊巢
                std::fill(pos, old_finish, val_copy);
            }
        }
        else {
            //情况2：备用的空间不够插了
            const size_type old_size = size();
            //如果需要的新空间比老空间还多，那就需要更大的空间正好装下，否则还是两倍的原空间
            //注意这里的新空间指的是分配的内存，跟size()没关系
            const size_type len = old_size + std::max(old_size, n);
            iterator new_start = data_allocator::allocate(len);
            iterator new_finish = new_start;
            try {
                //别忘了uninitialized系列主打的是构造与内存分配的分离～
                //首先把插入位置之前的对象拷贝到新位置去
                new_finish = tinystl::uninitialized_copy(_start, pos, new_start);
                //构造插入对象
                new_finish = uninitialized_fill_n(new_finish, n, val);
                //继续把插入位置后面的拷贝到新位置去，接在插入对象后面
                new_finish = tinystl::uninitialized_copy(pos, _finish, new_finish);
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
        return begin() + offset;
    }

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
            new_finish = tinystl::uninitialized_copy(_start, position, new_start);
            //构造插入对象
            tinystl::construct(new_finish, x);
            ++new_finish;
            //继续把插入位置后面的拷贝到新位置去，接在插入对象后面
            new_finish = tinystl::uninitialized_copy(position, _finish, new_finish);
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
