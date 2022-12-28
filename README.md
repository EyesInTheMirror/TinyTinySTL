# TinyTinySTL
## 进度追踪
2022.12.13 实现construct()和destroy()  
2022.12.17 实现allocator  
2022.12.20 学习迭代器，更新construct.h中的迭代器类型萃取  
2022.12.24 学习迭代器的萃取机制，实现iterator_traits以及迭代器的distance和advance方法 
2022.12.28 第一版非常简陋的vector，慢慢完善起来
## ToDo
uninitialized系列函数
## 备忘
vector用到了std::copy()、std::uninitialized_fill_n()
erase(iterator first, iterator last)、insert()还没定义
