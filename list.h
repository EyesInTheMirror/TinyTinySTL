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
    };

    //list数据结构




}

#endif //TINYTINYSTL_LIST_H
