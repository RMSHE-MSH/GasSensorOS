# Tree Structure Container Documentation
# 通用树数据结构容器帮助文档
---
# 协议 - License

 * *@file tree.hpp*
 * *@date 26.02.2023*
 * *@author RMSHE*
 * *< GasSensorOS >*
 * *Copyrght(C) 2023 RMSHE. All rights reserved.*
 * *This program is free software : you can redistribute it and /or modify*
 * *it under the terms of the GNU Affero General Public License as*
 * *published by the Free Software Foundation, either version 3 of the*
 * *License, or (at your option) any later version.*
 * *This program is distributed in the hope that it will be useful,*
 * *but WITHOUT ANY WARRANTY; without even the implied warranty of*
 * *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the*
 * *GNU Affero General Public License for more details.*
 * *You should have received a copy of the GNU Affero General Public License*
 * *along with this program.If not, see < https://www.gnu.org/licenses/>.*
 * *Electronic Mail : asdfghjkl851@outlook.com*


# 概述 - Overview

这是一个轻量级的通用树数据结构容器, 源代码见`tree.hpp`文件. 该模块是 `GasSensorOS` 的组件之一, 最初是为了 `GS Code` 代码解释器而开发的. 

这个类实现了一个通用的树数据结构，包括节点的添加、删除、查找、遍历等基本操作。该树数据结构由两个类组成：`TreeNode`和`Tree`。`TreeNode`表示一树节点，包含节点数据、子节点列表、父节点等属性；`Tree`表示整个树结构，包含根节点以及树的遍历、节点查找、节点删除等操作。具体来说，`TreeNode`类包含了添加子节点、查找子节点、删除子节点等方法。其中，添加子节点使用了`C++11`中的智能指针`std::unique_ptr`，确保了子节点的内存管理安全；查找子节点使用了递归的方式，深度优先遍历整个子树；删除子节点则使用了迭代的方式，遍历整个子树进行删除操作。`Tree`类包含了树的遍历、节点查找、节点删除等方法。其中，遍历操作分为深度优先遍历和广度优先遍历两种方式；节点查找操作同样使用了递归的方式，在根节点开始向下搜索整个子树；节点删除操作则使用了递归的方式，在整个子树中进行删除操作。总的来说，该树数据结构提供了基本的树操作，能够满足一些基本的需求。但是需要注意的是，该树数据结构没有进行任何的平衡操作，因此对于较大的树可能会存在效率问题。

#  `TreeNode` 类 - `TreeNode` class

## 成员 - Members

### 构造函数 - Constructors

| 名称 - Name | 说明 - Description                                           |
| :---------- | :----------------------------------------------------------- |
| TreeNode    | 构造节点. 该节点拥有值`data`和指向其父节点的指针`parent_node_ptr`这两个可构造的属性. <br/>*Constructing a node. The node has two constructible attributes, the value `data` and a pointer to its parent node `parent_node_ptr`.* |

### 变量 - Variables

| 名称 - Name | 说明 - Description                                           |
| :---------- | :----------------------------------------------------------- |
| node_data   | 储存这个节点的值. <br/>*Store the value of this node.*       |
| children    | 储存指向这个节点的子节点的指针. <br/>*Stores a pointer to the child node of this node.* |
| parent      | 储存指向这个节点的父节点的指针. <br/>*Store a pointer to the parent of this node.* |

### 方法 - Functions

| 名称 - Name    | 说明 - Description                                           |
| :------------- | :----------------------------------------------------------- |
| addChild       | 向当前节点添加一个子节点. <br/>*Add a child node to the current node.* |
| findChild      | 在当前节点的子节点中查找节点. <br/>*Find a node among the children of the current node.* |
| findDescendant | 在当前节点的后裔中查找节点. <br/>*Find a node among the descendants of the current node.* |
| hasChildren    | 判断在当前节点的指定子节点是否有孩子. <br/>*Determine if a child of the current node has children.* |
| deleteChild    | 删除父节点的一个无孩子节点(删除树叶节点). <br/>*Delete a childless node of the parent node (delete a leaf node).* |

# `Tree` 类 - `Tree` class
## 成员 - Members
### 构造函数 - Constructors

| 名称 - Name | 说明 - Description                                           |
| :---------- | :----------------------------------------------------------- |
| Tree        | 构造一个树, 创建树的第一个节点(根节点). <br/>*Construct a tree, creating the first node (`root`) of the tree.* |

### 变量 - Variables

| 名称 - Name      | 说明 - Description                                           |
| :--------------- | :----------------------------------------------------------- |
| root             | 储存树的根节点. <br/>*The root node of the storage tree.*    |
| current_node_ptr | 储存最后一次添加节点后的指针位置(初始化时设为根节点指针). <br/>*Store the position of the pointer after the last node was added (set to the root node pointer at initialization)* |

### 方法 - Functions

| 名称 - Name        | 说明 - Description                                           |
| :----------------- | :----------------------------------------------------------- |
| addNode            | 向节点添加子节点. <br/>*Adds a child node to a node.*        |
| traversalDFS       | 以深度优先的方式遍历树. <br/>*Traverses the tree in a depth-first style.* |
| traversalBFS       | 以广度优先的方式遍历树. <br/>*Traversing the tree in a breadth-first style.* |
| findNode           | 在树中查找节点. <br/>*Find nodes in the tree.*               |
| hasChildren        | 判断指定节点是否存有孩子. <br/>*Determine if the specified node has children.* |
| getDepth           | 递归地计算树的深度(高度)(默认统计整颗树的深度). <br/>*Recursively calculates the depth (height) of the tree (by default the depth of the whole tree is counted).* |
| getDegree          | 获取节点的度(对于一个给定的节点，其子节点的数量称为度. 一个叶子的度数一定是零) <br/>*Get the degree of a node (for a given node, the number of its children is called the degree. The degree of a leaf must be zero)* |
| get_degree_of_tree | 获取树的度(树的度是指树中一个节点的最大度) <br/>*Get the degree of the tree (the degree of the tree is the maximum degree of a node in the tree).* |
| getBreadth         | 获取树或树枝的叶子数量(叶子即没有子节点的节点，也称做终端节点) <br/>*Get the number of leaves of the tree or branch (leaves are nodes without children, also known as terminal nodes).* |
| getWidth           | 获取树的宽度或指定节点所在层的宽度(宽度指一个层的节点数) <br/>*Get the width of the tree or the width of the specified node's layer (width refers to the number of nodes in a layer).* |
| getLevel           | 获取指定节点的层级(一个节点的层级是它与根节点之间唯一路径上的边的数量, 根节点层级为零). <br/>*Get the layer of the specified node (the layer of a node is the number of edges on the unique path between it and the root node, the root node layer is zero).* |
| getSize            | 获取树或树枝的大小(节点数). <br/>*Get the size of a tree or branch.* |
| empty              | 判断树是否为空. <br/>*Determine if the tree is empty*        |
| deleteNode         | 递归删除一个节点及其后裔. <br/>*Recursively delete a node and its descendants.* |
| deleteTree         | 删除一颗树. <br/>*Delete a tree.*                            |

---

# `addChild`

向节点添加子节点. <br/>*Adds a child node to a node.*

```c++
TreeNode<T>* parent_node_ptr->TreeNode<T>* addChild(const T& data);
```

## 参数 - Parameters

`parent_node_ptr`

指向父节点的指针, 我们需要为新加节点指定一个父节点.<br/>*A pointer to the parent node, we need to specify a parent for the newly added node.*

`data`

表示节点的值, 即节点储存的内容. 其数据类型等于实例化树容器时的数据类型(可以是C++STL中的任意数据类型). <br/>*Represents the value of a node, i.e. the content stored in the node. Its data type is equal to the data type of the tree container when it is instantiated (it can be any data type in C++STL).*

## 返回值 - Return value

返回一个指向新加子节点的指针.<br/>*Returns a pointer to the newly added child node.*

## 注解 - Remarks

当调用 `addChild()` 方法时，它将创建一个新的 `TreeNode` 对象，该对象保存传递给函数的数据，并将指向新创建节点的指针添加到当前节点的 `children` 向量中。也就是说，`addChild()` 添加的是一个新的子节点。



*When the `addChild()` method is called, it creates a new `TreeNode` object, which holds the data passed to the function and adds a pointer to the newly created node to the `children` vector of the current node. Anyway, `addChild()` adds a new child node.*

## 示例 - Example

### 例1 - Example 1

以下代码演示了如何直接向根节点添加子节点.

*The following code demonstrates how to add a child node directly to the root node.*

```c++
#include <tree.hpp>
#include <string>

void setup() {
	Tree<std::string> tree("ROOT");
    
    // 向根节点添加一个值为"node_1的"子节点.
    // Add a child node with the value "node_1" to the root node.
    tree.root->addChild("node_1");
}
```

```
ROOT
|--node_1
```

### 例2 - Example 2

以下代码演示了如何通过在树中查找节点的方式添加子节点.

*The following code demonstrates how to add a child node by finding a node in the tree.*

```c++
#include <tree.hpp>
#include <string>

void setup() {
	Tree<std::string> tree("ROOT");
    
    // 查找并获取指向值为"ROOT"的节点(根节点)的指针.
    // Find and get a pointer to the node with the value "ROOT" (root node).
    auto* root_ptr = tree.findNode("ROOT");
    
    // 向根节点添加一个值为"node_1"的子节点.
    // Add a child node with the value "node_1" to the root node.
    root_ptr->addChild("node_1");
    
    // 查找并获取指向值为"node_1"的节点的指针, 然后向该节点添加一个值为"node_1_1"的子节点.
    // Find and get a pointer to the node with the value "node_1", then add a child node with the value "node_1_1" to that node.
    tree.findNode("node_1")->addChild("node_1_1");
}
```

```
ROOT
|--node_1
   |--node_1_1
```



# 简单示例 - Simple example

## 1. 如何实例化类? - How to instantiate a class?

```c++
#include <tree.hpp>
#include <string>

// 实例化一颗树, 创建树的根节点. 
// Instantiate a tree, create the root node of the tree.
Tree<std::string> tree("ROOT");
```

在上面的示例中, 我们实例化了一个数据类型为`std::string`名为`tree`的树容器, 并且将根节点的值设置为`"root"`.

### 备注:

- `std::string`可以是C++STL中的任意数据类型.
- `tree`是我们实例化的树的名称.
- `“ROOT”`是树根节点的值(数据类型是`std::string`).



*In the above example, we instantiate a tree container of data type `std::string` named `tree` and set the content of the root node to `"ROOT"`.*

### *note:*

- *`std::string` can be any data type in C++STL.*
- *`tree` is the name of the tree we instantiate*.
- *`"ROOT"` is the value of the root node of the tree (data type is `std::string`).*

## 2.  如何添加子节点? - How to add child nodes?


在实例化树容器后我们就可以向它添加节点. 添加节点的方式有多种, 但是均离不开两个参数, 一是指向父节点的指针`parent_node_ptr`, 二是待添加节点的值`data`. 具体的实现有两种:



*After instantiating the tree container, we can add nodes to it. There are various ways to add a node, but they all depend on two parameters, a pointer to the parent node `parent_node_ptr`, and the value of the node to be added `data`. There are two specific implementations:*

### 1. class TreeNode

```c++
TreeNode<T>* parent_node_ptr->TreeNode<T>* addChild(const T& data);
```

当调用 `addChild()` 函数时，它将创建一个新的 `TreeNode` 对象，该对象保存传递给函数的数据，并将指向新创建节点的指针添加到当前节点的 `children` 向量中。也就是说`addChild()` 做的工作是向当前节点添加一个子节点, 而指定父节点的任务由 `parent_node_ptr` 完成.



*When the `addChild()` function is called, it creates a new `TreeNode` object, which holds the data passed to the function and adds a pointer to the newly created node to the `children` vector of the current node. This means that what `addChild()` does is add a child node to the current node, while the task of specifying the parent node is done by `parent_node_ptr`.*

#### 示例代码 - Sample Code

```C++
#include <tree.hpp>
#include <string>

void setup() {
	Tree<std::string> tree("ROOT");
    /**
    * 向根节点添加一个子节点, 子节点的值为"node_1".
    */
    tree.root->addChild("node_1");
}
```



### 2. class Tree

```c++
TreeNode<T>* addNode(TreeNode<T>* node_ptr, const T& data);
```

