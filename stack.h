//
// Created by 镜瞳 on 2023/1/27.
//

#ifndef TINYTINYSTL_STACK_H
#define TINYTINYSTL_STACK_H

#include "deque.h"


namespace tinystl {
    template<typename T, typename Sequence = deque<T> >
    class stack {
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

        reference top() { return c.back(); }

        const_reference top() const { return c.back(); }

        void push(const value_type &val) { c.push_back(val); }

        void pop() { c.pop_back(); }
    };
}
#endif //TINYTINYSTL_STACK_H
