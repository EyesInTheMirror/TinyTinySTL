//
// Created by 镜瞳 on 2022/12/13.
//

#ifndef TINYTINYSTL_TOOLS_H
#define TINYTINYSTL_TOOLS_H
//tools.h实现了几个重要工具，包括move，forward

#include <type_traits>
//move将左值参数无条件的转换为右值
template <typename T>
constexpr decltype(auto) move(T&& arg){
    using returnType = typename std::remove_reference_t<T>&&;
    return static_cast<returnType>(arg);
}

//forward实现完美转发，也就是函数实参和形参类型一致（还没彻底弄懂，所以先不写）


#endif //TINYTINYSTL_TOOLS_H
