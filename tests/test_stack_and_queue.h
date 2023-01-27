//
// Created by 镜瞳 on 2023/1/27.
//

#ifndef TINYTINYSTL_TEST_STACK_AND_QUEUE_H
#define TINYTINYSTL_TEST_STACK_AND_QUEUE_H
#include "../queue.h"
#include "../stack.h"

namespace tinystl::test::stack_queue_test {
    void test_stack_queue() {
        std::cout << "------stack------" << std::endl;
        stack<int> st;
        st.push(10);
        st.push(100);
        std::cout << st.top() << std::endl;
        st.pop();
        std::cout << st.top() << std::endl;
        st.pop();
        std::cout << st.size() << ' ' << st.empty() << std::endl;
        std::cout << "------queue------" << std::endl;
        queue<int> q;
        q.push(1);
        q.push(2);
        std::cout << q.front() << std::endl;
        q.pop();
        std::cout << q.front() << std::endl;
    }
}
#endif //TINYTINYSTL_TEST_STACK_AND_QUEUE_H
