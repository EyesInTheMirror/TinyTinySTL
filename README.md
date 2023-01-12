# TinyTinySTL
## 进度追踪
2022.12.13 实现construct()和destroy()  
2022.12.17 实现allocator  
2022.12.20 学习迭代器，更新construct.h中的迭代器类型萃取  
2022.12.24 学习迭代器的萃取机制，实现iterator_traits以及迭代器的distance和advance方法  
2022.12.28 搭建vector框架  
2022.12.29 实现uninitialized_fill，并将与内存相关的操作收归到memory.h  
2023.01.02 完成第一版vector  
2023.01.03 实现initialized_copy，并整理vector的代码  
2023.01.06 测试vector功能，复盘写笔记  
2023.01.07 list结点、迭代器设计  
2023.01.08 list数据结构，构造函数  
2023.01.11 完成第一版list  

## ToDo 


## 备忘  
uninitialized、vector用到了algorithm的fill  
list的size别忘了更新  
list的fill_init不满足commit-rollback语义  