# TinyTinySTL
## 进度追踪
2022.12.13 实现construct()和destroy()  
2022.12.17 实现allocator  
2022.12.20 学习迭代器，更新construct.h中的迭代器类型萃取  
2022.12.24 学习迭代器的萃取机制，实现iterator_traits以及迭代器的distance和advance方法  
2022.12.28 第一版非常简陋的vector，慢慢完善起来  
2022.12.29 实现uninitialized_fill，并将与内存相关的操作收归到memory.h
## ToDo
vector功能完善 
## 备忘
vector用到了std::copy()  
erase(iterator first, iterator last)、insert()还没定义  
uninitialized用到了algorithm的fill  
