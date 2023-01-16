//
// Created by 镜瞳 on 2023/1/14.
//

#ifndef TINYTINYSTL_DEQUE_H
#define TINYTINYSTL_DEQUE_H

#include "iterator.h"
#include "memory.h"

namespace tinystl {
    template<typename T>
    struct deque_buf_size { //获取单个缓冲区的大小
        static constexpr size_t value = sizeof(T) < 256 ? 4096 / sizeof(T) : 16;
    };

    template<typename T, typename Ref, typename Ptr>
    struct deque_iterator : public iterator<random_access_iterator_tag, T> {
        using iterator = deque_iterator<T, T &, T *>;
        using const_iterator = deque_iterator<T, const T &, const T *>;
        using self = deque_iterator;
        static const size_t buffer_size = deque_buf_size<T>::value;

        using value_type = T;
        using pointer = Ptr;
        using reference = Ref;
        using map_pointer = T **;
        using size_type = size_t;
        using differerce_type = ptrdiff_t;

        T *cur;             //当前迭代器指向的实际元素
        T *first;           //当前元素所处缓冲区的头部
        T *last;            //当前元素所处缓冲区的尾部
        map_pointer node;   //当前缓冲区在map的位置

        void set_node(map_pointer new_node) {//把当前迭代器挂到另一个缓冲区去
            node = new_node;
            first = *new_node;
            last = first + buffer_size;
        }

        reference operator*() { return *cur; }

        pointer operator->() { return &(operator*()); }

        differerce_type operator-(const self &x) const {
            //逻辑：两个迭代器之间相差了多少个完整的缓冲区+x离所处缓冲区末尾差多少+自己离所处缓冲区开头差多少
            return buffer_size * (node - x.node - 1) + (x.last - x.cur) + (cur - first);
        }

        self &operator++() {
            ++cur;
            if (cur == last) {
                set_node(node + 1);
            }
            return *this;
        }

        self operator++(int) {
            self tmp = *this;
            ++*this;
            return tmp;
        }

        self &operator--() {
            if (cur == first) {
                set_node(node - 1);
                cur = last;
            }
            --cur;
            return *this;
        }

        self operator--(int) {
            self tmp = *this;
            --*this;
            return tmp;
        }

        self &operator+=(differerce_type n) {
            differerce_type offset = n + cur - first; //加完n以后相对本缓冲区头部的偏移
            if (offset >= 0 && offset < static_cast<size_type>(buffer_size)) {
                cur += n;
            } else { //偏移到另一个区
                differerce_type node_offset = offset > 0 ? offset / static_cast<size_type>(buffer_size)
                        : (offset + 1) / static_cast<size_type>(buffer_size) - 1;
                set_node(node + node_offset);
                //以下获得最终的块内偏移
                cur = first + (offset - node_offset * static_cast<size_type>(buffer_size));
            }
            return *this;
        }

        self operator+(differerce_type n) {
            self tmp = *this;
            tmp += n;
            return tmp;
        }

        self& operator-=(differerce_type n) {
            return this->operator+=(-n);
        }

        self operator-(differerce_type n) {
            self tmp = *this;
            tmp -= n;
            return tmp;
        }

        reference operator[](differerce_type n) const { return *(*this + n);}

        bool operator==(const self& x) const { return cur == x.cur; }
        bool operator!=(const self& x) const { return cur != x.cur; }
        bool operator<(const self& x) const {
            //缓冲区不同比缓冲区前后，缓冲区相同比区内cur前后
            return node == x.node ? (cur < x.cur) : (node < x.node);
        }
    };


    template<typename T>
    class deque {
    public:
        using value_type = T;
        using pointer = T *;
        using reference = T &;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using iterator = deque_iterator<T, T&, T*>;

        static const size_t buffer_size = deque_buf_size<T>::value;
    protected:
        using map_pointer = pointer *;
    private:
        map_pointer map; //二级指针，数组中的每一个元素都是一个指针，指向一个缓冲区
        size_type map_size;
        iterator _start;
        iterator _finish;

    public:



        iterator begin() { return _start;}
        iterator end() { return _finish;}

        reference front() { return *_start;}
        iterator back() { return *(_finish - 1);}

        size_type size() { _finish - _start;}
        size_type max_size() { return static_cast<size_type>(-1);}

        bool empty() { return _start == _finish;}

        reference operator[](size_type n) {return *(_start + n);}
    };
}


#endif //TINYTINYSTL_DEQUE_H
