# TinyTinySTL
## 进度追踪
2022.12.13 实现construct()和destroy()  
2022.12.17 实现allocator  
2022.12.20 学习迭代器，更新construct.h中的迭代器类型萃取  
2022.12.24 学习迭代器的萃取机制，实现iterator_traits以及迭代器的distance和advance方法  
2022.12.28 搭建vector框架  
2022.12.29 实现uninitialized_fill，并将与内存相关的操作收归到memory.h  
2023.01.02 完成第一版vector  
## ToDo 
uninitialized_copy()，替换vector中的实现  
复盘vector，写笔记  
## 备忘 
erase(iterator first, iterator last)、insert()还没定义  
uninitialized、vector用到了algorithm的fill  
