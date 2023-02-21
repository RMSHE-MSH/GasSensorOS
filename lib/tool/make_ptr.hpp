#pragma once

#include <memory>

/*
用于创建一个智能指针对象:
这段代码定义了一个模板函数 make_unique，用于创建并返回一个智能指针 unique_ptr，其中模板参数 T 代表指向的类型，参数 Args 代表构造函数参数类型的列表。

函数的实现部分首先创建一个 unique_ptr，指向一个通过 new 运算符创建的类型为 T 的对象，并传递构造函数所需的参数列表。 std::forward 用于将 args
转发给构造函数，确保构造函数接收到正确类型的参数。

由于返回的是 unique_ptr，因此可以确保指针所有权的唯一性，避免内存泄漏的问题.
*/
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <typename T, typename... Args>
std::shared_ptr<T> make_shared(Args&&... args) {
    return std::shared_ptr<T>(new T(std::forward<Args>(args)...));
}