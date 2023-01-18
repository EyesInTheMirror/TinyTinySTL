//
// Created by 镜瞳 on 2023/1/14.
//

#ifndef TINYTINYSTL_DEQUE_H
#define TINYTINYSTL_DEQUE_H

#include "iterator.h"
#include "memory.h"
const size_t INITIAL_MAP_SIZE = 8;
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
        using data_allocator = tinystl::allocator<T>;
        using map_allocator = tinystl::allocator<T*>;
    private:
        map_pointer _map; //二级指针，数组中的每一个元素都是一个指针，指向一个缓冲区
        size_type map_size;
        iterator _start;
        iterator _finish;

        void create_map_and_buf(size_type n);
        void fill_initialize(size_type n, const T& val);
        void reallocate_map(size_type buf_to_add, bool add_at_front);
        void reserve_map_at_front(size_type buf_to_add = 1);
        void reserve_map_at_back(size_type buf_to_add = 1);
        void push_back_aux(const value_type& val);
        void push_front_aux(const value_type& val);

    public:
        deque(size_type n, const T& val) : _start(), _finish(), _map(nullptr), map_size(0)
        { fill_initialize(n, val);}
        explicit deque(size_type n) : _start(), _finish(), _map(nullptr), map_size(0)
        { fill_initialize(n, value_type());}
        deque() : _start(), _finish(), _map(nullptr), map_size(0)
        { fill_initialize(0, value_type());}
        ~deque() {
            if(_map != nullptr) {
                //clear();
                data_allocator::deallocate(*_start.node, buffer_size);
                *_start.node = nullptr;
                map_allocator::deallocate(_map, map_size);
                _map = nullptr;
            }
        }

        iterator begin() { return _start;}
        iterator end() { return _finish;}

        reference front() { return *_start;}
        iterator back() { return *(_finish - 1);}

        size_type size() { _finish - _start;}
        size_type max_size() { return static_cast<size_type>(-1);}

        bool empty() { return _start == _finish;}

        reference operator[](size_type n) {return *(_start + n);}

        void push_back(const value_type& val) {
            if(_finish.cur != _finish.last - 1) {
                tinystl::construct(_finish.cur, val);
                ++_finish.cur;
            }
            else {
                push_back_aux(val);
            }
        }
        void push_front(const value_type& val) {
            if(_start.cur != _start.first) {
                tinystl::construct(_start.cur - 1, val);
                --_start.cur;
            }
            else {
                push_front_aux(val);
            }
        }
    };

    template<typename T>
    void deque<T>::reserve_map_at_back(deque::size_type buf_to_add) {
        if(buf_to_add + 1 > map_size - (_finish.node - _map)) {
            reallocate_map(buf_to_add, false);
        }
    }

    template<typename T>
    void deque<T>::reserve_map_at_front(deque::size_type buf_to_add) {
        if(buf_to_add > _start.node - _map) {
            reallocate_map(buf_to_add, true);
        }
    }

    template<typename T>
    void deque<T>::push_front_aux(const value_type &val) {
        value_type x_copy = val;
        reserve_map_at_front();
        *(_start.node - 1) = data_allocator::allocate(buffer_size);
        try {
            _start.set_node(_start.node - 1);
            _start.cur = _start.last - 1;
            tinystl::construct(_start.cur, x_copy);
        }
        catch(...) {
            _start.set_node(_start.node + 1);
            _start.cur = _start.first;
            data_allocator::deallocate(*(_start.node - 1));
            throw;
        }
    }

    template<typename T>
    void deque<T>::push_back_aux(const value_type &val) {
        //缓冲区不够了调用此函数，即finish.cur处于最后一个位置
        value_type x_copy = val;
        //map空间不够了调用此函数换新map
        reserve_map_at_back();
        //申请新的缓冲区
        *(_finish.node + 1) = data_allocator::allocate(buffer_size);
        try {
            tinystl::construct(_finish.cur, x_copy);
        }
        catch(...) {
            data_allocator::deallocate(*(_finish.node + 1));
            throw;
        }
        _finish.set_node(_finish.node + 1);
        _finish.cur = _finish.first;
    }

    template<typename T>
    void deque<T>::reallocate_map(deque::size_type buf_to_add, bool add_at_front) {
        size_type old_buf_num = _finish.node - _start.node + 1;
        size_type new_buf_num = old_buf_num + buf_to_add;
        map_pointer new_n_start;

        if(map_size > 2 * new_buf_num) {//总的map空间还是很充足的情况
            new_n_start = _map + (map_size - new_buf_num) / 2
                          + (add_at_front ? buf_to_add : 0);
            if(new_n_start < _start.node) {
                std::copy(_start.node, _finish.node + 1, new_n_start);
            }
            else {
                std::copy_backward(_start.node, _finish.node + 1, new_n_start + old_buf_num);
            }
        }
        else {
            size_type new_map_size = map_size + std::max(map_size, buf_to_add) + 2;
            map_pointer new_map = map_allocator::allocate(new_map_size);
            new_n_start = new_map + (new_map_size - new_buf_num) / 2
                          + (add_at_front ? buf_to_add : 0);

            std::copy(_start.node, _finish.node + 1, new_n_start);

            map_allocator::deallocate(_map, map_size);

            _map = new_map;
            map_size = new_map_size;
        }
        _start.set_node(new_n_start);
        _finish.set_node(new_n_start + old_buf_num - 1);
        //时刻谨记这个函数仅仅根据即将加入的buf数量配置新map，还没轮到新缓冲区的加入过程
    }

    template<typename T>
    void deque<T>::create_map_and_buf(deque::size_type n) {
        //缓冲区数量。如果正好整除也得多一个，last得放最后一个的后面一个位置
        size_type num_nodes = n / buffer_size + 1;
        //map最少也准备8个位置，或者缓冲区数量加2（头尾空一个）
        map_size = std::max(INITIAL_MAP_SIZE, num_nodes + 2);
        _map = map_allocator::allocate(map_size);
        //设两个指针标识待初始化的头尾区间，尽量放在分配空间的正中间，方便向两边扩张
        map_pointer n_start = _map + (map_size - num_nodes) / 2;
        map_pointer n_finish = n_start + num_nodes - 1; //闭
        //接下来初始化缓冲区
        map_pointer cur;
        try {
            for(cur = n_start; cur <= n_finish; ++cur) {
                //map的每一个有效元素都指向一片缓冲区
                *cur = data_allocator::allocate(buffer_size);
            }
        }
        catch(...) {
            while(cur != n_start) {
                --cur;
                data_allocator::deallocate(*cur, buffer_size);
                *cur = nullptr;
            }
            throw;
        }

        _start.set_node(n_start);
        _finish.set_node(n_finish);
        _start.cur = _start.first;
        _finish.cur = _finish.first + n % buffer_size;
    }

    template<typename T>
    void deque<T>::fill_initialize(deque::size_type n, const T &val) {
        //创建map和缓冲区
        create_map_and_buf(n);

        map_pointer cur;
        for(cur = _start.node; cur < _finish.node; ++cur) {
            //给每一个完整的buffer赋初值
            tinystl::uninitialized_fill(*cur, *cur + buffer_size, val);
        }
        //单独处理最后一个buf
        tinystl::uninitialized_fill(_finish.first, _finish.cur, val);
    }
}


#endif //TINYTINYSTL_DEQUE_H
