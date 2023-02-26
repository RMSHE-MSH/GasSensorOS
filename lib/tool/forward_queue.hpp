/**
 * @file forward_queue.hpp
 * @date 26.02.2023
 * @author RMSHE
 *
 * < GasSensorOS >
 * Copyright(C) 2023 RMSHE. All rights reserved.
 *
 * This program is free software : you can redistribute it and /or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.If not, see < https://www.gnu.org/licenses/>.
 *
 * Electronic Mail : asdfghjkl851@outlook.com
 */

/*
这段代码定义了一个名为 forward_queue 的类，实现了一个双端队列，其中 T 是一个泛型类型参数，表示队列中保存的元素类型。

类的公共接口包括以下成员函数：

构造函数：接受一个整数参数，表示队列的最大长度；
push：将新元素压入队列的顶部，如果队列已满，从底部弹出一个元素；
pop：从队列的底部弹出一个元素；
size：返回当前队列中元素的数量；
getItems：返回当前队列中的所有元素；
front：返回队列头的元素的引用；
const front：返回队列头的元素的常量引用；
back：返回队列尾的元素的引用；
const back：返回队列尾的元素的常量引用；
begin：返回队列开头的迭代器；
end：返回队列结尾的迭代器；
const begin：返回队列开头的常量迭代器；
const end：返回队列结尾的常量迭代器；
clear：清空队列中的所有元素；
empty：检查队列是否为空。
在类的内部，使用了一个 std::deque 类型的成员变量 m_queue 来保存队列中的元素。push 和 pop 函数分别使用 m_queue.push_front 和 m_queue.pop_back
操作来实现从顶部压入元素和从底部弹出元素的功能。

begin 和 end 函数分别使用 m_queue.begin 和 m_queue.end 操作来返回迭代器。需要注意的是，由于 forward_queue 类是一个模板类，因此需要在返回迭代器的函数前加上
typename 关键字，来表示 std::deque<T>::iterator 和 std::deque<T>::const_iterator 是一个类型。
*/

#pragma once
#include <deque>

/**
 * @brief 正向队列, 元素从顶部压入, 从底部弹出.
 * @param maxSize uint64_t表示队列的最大长度, 当压入的元素超过该值时回自动弹出底部的元素
 */
template <typename T>
class forward_queue {
   public:
    // 构造函数，接受一个整数参数，表示队列的最大长度
    forward_queue(uint64_t maxSize) : m_maxSize(maxSize) {}

    // 将新元素压入队列的顶部
    void push(T item) {
        // 如果队列已满，从底部弹出一个元素
        if (m_queue.size() >= m_maxSize) m_queue.pop_back();

        // 将新元素压入队列的顶部
        m_queue.push_front(item);
    }

    // 从队列的底部弹出一个元素
    void pop() {
        if (!m_queue.empty()) m_queue.pop_back();
    }

    // 返回当前队列中元素的数量
    uint64_t size() const { return m_queue.size(); }

    // 返回当前队列中的所有元素
    std::deque<T> getItems() const { return m_queue; }

    // 返回队列头的元素的引用
    T& front() { return m_queue.front(); }

    // 返回队列头的元素的常量引用
    const T& front() const { return m_queue.front(); }

    // 返回队列尾的元素的引用
    T& back() { return m_queue.back(); }

    // 返回队列尾的元素的常量引用
    const T& back() const { return m_queue.back(); }

    // 返回队列开头的迭代器
    typename std::deque<T>::iterator begin() { return m_queue.begin(); }

    // 返回队列结尾的迭代器
    typename std::deque<T>::iterator end() { return m_queue.end(); }

    // 返回队列开头的常量迭代器
    typename std::deque<T>::const_iterator begin() const { return m_queue.begin(); }

    // 返回队列结尾的常量迭代器
    typename std::deque<T>::const_iterator end() const { return m_queue.end(); }

    // 清空队列中的所有元素
    void clear() { m_queue.clear(); }

    // 检查队列是否为空
    bool empty() const { return m_queue.empty(); }

   private:
    uint64_t m_maxSize;     // 队列的最大长度
    std::deque<T> m_queue;  // 双端队列，用于保存元素
};

/*
以下是一个简单的示例，展示了如何使用 forward_queue 类：
#include <iostream>

#include "forward_queue.h"

int main() {
    // 创建一个最大长度为 3 的双端队列
    forward_queue<int> q(3);

    // 将元素 1、2、3、4 压入队列
    q.push(1);
    q.push(2);
    q.push(3);
    q.push(4);

    // 检查队列中元素的数量
    std::cout << "Queue size: " << q.size() << std::endl;

    // 访问队列头的元素
    std::cout << "Queue front: " << q.front() << std::endl;

    // 访问队列尾的元素
    std::cout << "Queue back: " << q.back() << std::endl;

    // 使用迭代器遍历队列中的所有元素
    std::cout << "Queue elements: ";
    for (auto iter = q.begin(); iter != q.end(); ++iter) {
        std::cout << *iter << " ";
    }
    std::cout << std::endl

*/