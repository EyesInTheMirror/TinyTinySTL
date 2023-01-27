//
// Created by 镜瞳 on 2023/1/27.
//

#ifndef TINYTINYSTL_QUEUE_H
#define TINYTINYSTL_QUEUE_H
#include "deque.h"


namespace tinystl {
    template<typename T, typename Sequence = deque<T> >
    class queue {
    public:
        using value_type = typename Sequence::value_type;
        using size_type = typename Sequence::size_type;
        using reference = typename Sequence::reference;
        using const_reference = typename Sequence::const_reference;

    private:
        Sequence c; //真正的容器

    public:
        [[nodiscard]] bool empty() const { return c.empty(); }

        size_type size() const { return c.size(); }

        reference front() { return c.front(); }

        const_reference front() const { return c.front(); }

        reference back() { return c.back(); }

        const_reference back() const { return c.back(); }

        void push(const value_type &val) { c.push_back(val); }

        void pop() { c.pop_front(); }
    };
}
#endif //TINYTINYSTL_QUEUE_H
