//
// Created by 镜瞳 on 2023/1/7.
//

#ifndef TINYTINYSTL_LIST_H
#define TINYTINYSTL_LIST_H

#include "iterator.h"

namespace tinystl {
    //双向链表节点的数据结构
    template<typename T>
    struct list_node {
        list_node<T> *prev;
        list_node<T> *next;
        T data;
    };

//list迭代器设计，继承迭代器设计模板
    template<typename T>
    struct list_iterator : public tinystl::iterator<tinystl::bidirection_iterator_tag, T> {
        using self = list_iterator<T>;
        using iterator = list_iterator<T>;

        using value_type = T;
        using pointer = T *;
        using reference = T &;
        using link_type = list_node<T> *; //这个就是节点指针的类型

        link_type _node; //指向节点的指针

        explicit list_iterator(link_type x) : _node(x) {}

        list_iterator() = default;

        list_iterator(const iterator &x) : _node(x._node) {}

        //迭代器的各种操作
        reference operator*() { return _node->data; }

        pointer operator->() { return &(operator*()); }
        //重载后itr->val其实是itr.operator->()->val
        //而operator->()做的事是取回结点内实际元素的指针

        //前加加
        self &operator++() {
            _node = _node->next;
            return *this;
        }

        //后加加
        self operator++(int) {
            self tmp = *this;
            ++*this;
            return tmp;
        }

        self &operator--() {
            _node = _node->prev;
            return *this;
        }

        self operator--(int) {
            self tmp = *this;
            --*this;
            return tmp;
        }

        bool operator!=(const iterator& rhs) { return this->_node != rhs._node; }
    };

    //list数据结构（环状双向链表）
    template<typename T>
    class list {
    protected:
        using list_node = list_node<T>;
        using list_node_allocator = tinystl::allocator<list_node>;
    public:
        using link_type = list_node *;
        using iterator = list_iterator<T>;
        using size_type = size_t;
        using value_type = T;
        using pointer = T *;
        using reference = T &;
    protected:
        link_type _node; //环形链表只需一个指针即可，这个指针指向末端结点（最后一个元素的下一个位置）
        size_type _size{}; //不想O(n)返回大小，试试优化一波

        //申请一个结点的空间
        link_type new_node() { return list_node_allocator::allocate(); }

        //释放结点空间
        void delete_node(link_type p) { list_node_allocator::deallocate(p); }

        //申请并构造一个结点
        link_type create_node(const T &val) {
            link_type blank_node = new_node();
            tinystl::construct(&blank_node->data, val);
            return blank_node;
        }

        //析构并释放一个结点
        void destroy_node(link_type p) {
            tinystl::destroy(&p->data);
            delete_node(p);
        }

        //用于构造函数
        void fill_initialize(size_type n, const T &val) {
            _node = new_node();
            _size = n;
            _node->next = _node;
            _node->prev = _node;
            for (; n > 0; --n) {
                link_type node = create_node(val);
                node->next = _node->next;
                _node->next->prev = node;
                _node->next = node;
                node->prev = _node;
            }
        }

    public:
        //默认构造函数，创建一个空表
        list() { fill_initialize(0, value_type()); }

        explicit list(size_type n) { fill_initialize(n, value_type()); }

        list(size_type n, const T &val) { fill_initialize(n, val); }

        ~list() {
            clear();
            list_node_allocator::deallocate(_node);
            _node = nullptr;
            _size = 0;
        }
        //各种操作
        iterator begin() { return static_cast<iterator>(_node->next); }

        iterator end() { return static_cast<iterator>(_node); }

        bool empty() { return begin() == end(); }

        [[nodiscard]] size_type size() const { return _size; }

        reference front() { return *begin(); }

        reference back() { return *(--end()); }
    };


}

#endif //TINYTINYSTL_LIST_H
