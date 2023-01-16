//
// Created by 镜瞳 on 2023/1/7.
//

#ifndef TINYTINYSTL_LIST_H
#define TINYTINYSTL_LIST_H

#include "iterator.h"
#include "memory.h"

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

        bool operator==(const iterator &rhs) const { return this->_node == rhs._node; }
        bool operator!=(const iterator &rhs) const { return this->_node != rhs._node; }
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
    private:
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
        void fill_initialize(size_type n, const T &val);

        template<typename Iter>
        void copy_initialize(Iter first, Iter last);

        //内部函数，用于把一个范围内的结点接到pos前面一位（前闭后开）
        void transfer(iterator pos, iterator first, iterator last);

    public:
        //默认构造函数，创建一个空表
        list() { fill_initialize(0, value_type()); }

        explicit list(size_type n) { fill_initialize(n, value_type()); }

        list(size_type n, const T &val) { fill_initialize(n, val); }

        template<typename Iter, typename std::enable_if<
                tinystl::is_input_iterator<Iter>::value, int>::type = 0>
        list(Iter first, Iter last) { copy_initialize(first, last); }

        ~list() {
            clear();
            list_node_allocator::deallocate(_node);
            _node = nullptr;
            _size = 0;
        }

        //各种操作
        iterator begin() { return iterator(_node->next); }

        iterator end() { return iterator(_node); }

        bool empty() { return begin() == end(); }

        [[nodiscard]] size_type size() const { return _size; }

        reference front() { return *begin(); }

        reference back() { return *(--end()); }

        iterator insert(iterator pos, const T &val);

        void push_front(const T &val);

        void push_back(const T &val);

        iterator erase(iterator pos);

        void pop_front();

        void pop_back();

        void clear();

        void remove(const T &val);

        //删除连续的相同元素
        void unique();

        //list的各种连接操作
        void splice(iterator pos, list& x);
        void splice(iterator pos, list& x, iterator itr);
        void splice(iterator pos, list& x, iterator first, iterator last);

        void merge(list& x);

        void reverse();

        void swap(list& x);

        void sort();
    };

    template<typename T>
    void list<T>::swap(list &x) {
        std::swap(_node, x._node);
        std::swap(_size, x._size);
    }

    template<typename T>
    void list<T>::sort() {
        if(_size == 0 || _size == 1) return;

        list<T> carry;
        list<T> counter[64];
        int fill = 0;
        while(!empty()) {
            carry.splice(carry.begin(), *this, begin());
            int i = 0;
            while(i < fill && !counter[i].empty()) {
                counter[i].merge(carry);
                carry.swap(counter[i++]);
            }
            carry.swap(counter[i]);
            if(i == fill) ++fill;
        }
        for(int i = 1; i < fill; ++i) {
            counter[i].merge(counter[i - 1]);
        }
        swap(counter[fill - 1]);
    }

    template<typename T>
    void list<T>::reverse() {
        if(_size == 0 || _size == 1) return;
        iterator cur = begin();
        ++cur;
        while(cur != end()) {
            iterator pre = cur;
            ++cur;
            transfer(begin(), pre, cur);
        }
    }

    template<typename T>
    void list<T>::merge(list &x) {
        //前排提醒：两个链表都必须有序
        iterator first1 = begin();
        iterator last1 = end();
        iterator first2 = x.begin();
        iterator last2 = x.end();
        size_type n = x._size;

        while(first1 != last1 && first2 != last2) {
            if(*first2 < *first1) {
                iterator next = first2;
                transfer(first1, first2, ++next);
                first2 = next;
            }
            else ++first1;
        }
        //如果到了最后2还没接到1上面去，那么把2剩下的全部接到1尾部
        if(first2 != last2) {
            transfer(last1, first2, last2);
        }
        _size += n;
        x._size = 0;
    }

    template<typename T>
    void list<T>::transfer(list::iterator pos, list::iterator first, list::iterator last) {
        //前排提醒：前闭后开
        if (pos != first) {
            //区间尾结点勾搭上pos
            last._node->prev->next = pos._node;
            //区间首结点的前置位宣布单方面终止和first外交，并找上尾结点后置位抱团取暖
            first._node->prev->next = last._node;
            link_type tmp = pos._node->prev;
            //pos回应了区间尾结点
            pos._node->prev = last._node->prev;
            //pos的前置位勾搭上区间首结点
            tmp->next = first._node;
            //可怜的区间外结点完成联络
            last._node->prev = first._node->prev;
            //区间首结点回应pos前置位，至此区间彻底融入pos的前面
            first._node->prev = tmp;
        }
    }

    template<typename T>
    void list<T>::splice(list::iterator pos, list &x, list::iterator first, list::iterator last) {
        if(first != last) {
            size_type n = distance(first, last);
            transfer(pos, first, last);
            _size += n;
            x._size -= n;
        }
    }

    template<typename T>
    void list<T>::splice(list::iterator pos, list &x, list::iterator itr) {
        iterator tmp = itr;
        ++tmp;
        //待插的元素本来位于pos的前一位或者是pos自己的话啥也不做
        if(pos == itr || pos == tmp) return;
        transfer(pos, itr, tmp);
        ++_size;
        --x._size;
    }

    template<typename T>
    void list<T>::splice(list::iterator pos, list &x) {
        if(!x.empty()) {
            transfer(pos, x.begin(), x.end());
            _size += x._size;
            x._size = 0;
        }
    }

    template<typename T>
    template<typename Iter>
    void list<T>::copy_initialize(Iter first, Iter last) {
        _node = new_node();
        _node->prev = _node;
        _node->next = _node;
        size_type n = tinystl::distance(first, last);
        _size = n;
        try {
            for (; n > 0; --n, ++first) {
                auto node = create_node(*first);
                node->next = _node;
                node->prev = _node->prev;
                _node->prev = node;
                node->prev->next = node;
            }
        }
        catch (...) {
            clear();
            list_node_allocator::deallocate(_node);
            _node = nullptr;
            _size = 0;
            throw;
        }
    }

    template<typename T>
    void list<T>::unique() {
        if (_size <= 1) return;
        link_type cur = _node->next;
        link_type next = cur->next;
        while (cur != _node) {
            if (cur->data == next->data) {
                next = erase(iterator(next))._node;
            } else {
                cur = next;
                next = next->next;
            }
        }
    }

    template<typename T>
    void list<T>::remove(const T &val) {
        link_type cur = _node->next;
        while (cur != _node) {
            if (cur->data == val) {
                cur = erase(iterator(cur))._node;
            } else cur = cur->next;
        }
    }

    template<typename T>
    void list<T>::clear() {
        link_type cur = _node->next;
        while (cur != _node->prev) {
            link_type tmp = cur;
            cur = cur->next;
            destroy_node(tmp);
        }
        _size = 0;
        _node->next = _node;
        _node->prev = _node;
    }

    template<typename T>
    void list<T>::pop_back() {
        erase(--end());
    }

    template<typename T>
    void list<T>::pop_front() {
        erase(begin());
    }

    template<typename T>
    typename list<T>::iterator list<T>::erase(list::iterator pos) {
        link_type return_node = pos._node->next;
        pos._node->prev->next = pos._node->next;
        pos._node->next->prev = pos._node->prev;
        destroy_node(pos._node);
        --_size;
        return iterator(return_node);
    }

    template<typename T>
    void list<T>::push_back(const T &val) {
        insert(end(), val);
    }

    template<typename T>
    void list<T>::push_front(const T &val) {
        insert(begin(), val);
    }

    template<typename T>
    void list<T>::fill_initialize(list::size_type n, const T &val) {
        _node = new_node();
        _node->next = _node;
        _node->prev = _node;
        _size = n;
        try {
            for (; n > 0; --n) {
                link_type node = create_node(val);
                //这里是前插，不过反正都一样的结点就无所谓了
                node->next = _node->next;
                _node->next->prev = node;
                _node->next = node;
                node->prev = _node;
            }
        }
        catch (...) {
            clear();
            list_node_allocator::deallocate(_node);
            _node = nullptr;
            _size = 0;
            throw;
        }
    }

    template<typename T>
    typename list<T>::iterator list<T>::insert(list::iterator pos, const T &val) {
        //构造一个结点并且填入内容
        link_type tmp = create_node(val);
        //一顿骚操作把tmp结点接到pos的前一个位置（想象不出来过程就画图）
        tmp->next = pos._node;
        tmp->prev = pos._node->prev;
        pos._node->prev->next = tmp;
        pos._node->prev = tmp;

        ++_size;
        //返回插入元素，但是不存在空间移动问题，pos迭代器仍然有效
        return iterator(tmp);
    }
}

#endif //TINYTINYSTL_LIST_H
