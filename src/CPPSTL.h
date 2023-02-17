#pragma once

#include <algorithm>      //提供了一系列算法，如排序、搜索、移除、变换等;
#include <array>          //array 是一种固定长度的数组，随机存取元素高效。
#include <bitset>         //bitset 是一种用于操作二进制位的容器，支持快速查询/修改二进制位。
#include <cmath>          //提供了一系列数学函数，如三角函数、指数函数、对数函数等;
#include <cstdint>        //提供了一系列定长整数类型，例如 uint8_t、uint16_t 等;
#include <cstdio>         //提供了一种读写 C 风格的文件和流的方式;
#include <ctime>          // 提供了一系列关于时间和日期的函数;
#include <deque>          //deque 是一种双端队列，随机存取元素高效，在队列两端插入删除元素也高效。
#include <forward_list>   //forward_list 是一种单向链表，随机存取元素效率低，在链表中插入删除元素高效
#include <functional>     //用来传递匿名函数作为函数的参数
#include <list>           //list 是一种双向链表，随机存取元素效率低，在链表中插入删除元素高效。
#include <map>            //map 是一种关联数组，以关键字为索引，支持快速插入/删除/查询元素。
#include <queue>          //queue 是一种先进先出（FIFO）的数据结构，只能在队列尾插入元素，在队列头删除元素。
#include <set>            //set 是一种有序不重复集合，支持快速插入/删除/查询元素，并且元素具有唯一性。
#include <sstream>        //提供了一种读写字符串流的方式;
#include <stack>          //stack 是一种后进先出（LIFO）的数据结构，只能在栈顶插入删除元素。
#include <string>         //提供了一种字符串类型;
#include <unordered_map>  //unordered_map 是一种无序关联数组，以关键字为索引，支持快速插入/删除/查询元素。
#include <unordered_set>  //unordered_set 是一种无序不重复集合，支持快速插入/删除/查询元素，元素具有唯一性。
#include <vector>         //vector 是一种动态数组，随机存取元素高效，在末尾插入删除元素也高效。

/*
vector 向量：动态数组，可以快速随机访问。
deque 双端队列：双端开口的动态数组，可以快速在两端插入/删除元素。
list 链表：双向链表，支持快速在任意位置插入/删除元素。
forward_list 单向链表：单向链表，支持快速在任意位置插入/删除元素。
array 数组：固定大小的数组，可以快速随机访问。
stack 栈：后进先出的数据结构，支持快速在栈顶插入/删除元素。
queue 队列：先进先出的数据结构，支持快速在队首插入/删除元素。
priority_queue 优先队列：元素按优先级排序的队列，支持快速在队首插入/删除元素。
set 集合：元素不重复的集合，支持快速插入/删除/查询元素。
multiset 多重集合：元素可以重复的集合，支持快速插入/删除/查询元素。
map 映射：键值对集合，支持快速插入/删除/查询键值对。
multimap 多重映射：键可以重复的键值对集合，支持快速插入/删除/查询键值对。
unordered_set 无序集合：元素不重复的无序集合，支持快速插入/删除/查询元素。
unordered_multiset 无序多重集合：元素可以重复的无序集合，支持快速插入/删除/查询元素。
unordered_map 无序映射：键值对集合，支持快速插入/删除/查询键值对。
unordered_multimap 无序多重映射：键可以重复的无序键值对集合，支持快速插入/删除/查询键值对。
*/

