# Tree Structure Container Documentation
# 通用树数据结构容器帮助文档
---
# 协议 - License

> @file tree.hpp
> @date 26.02.2023
> @author RMSHE
>
> < GasSensorOS >
> Copyrght(C) 2023 RMSHE. All rights reserved.
>
> This program is free software : you can redistribute it and /or modify it under the terms of the GNU Affero General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details. You should have received a copy of the GNU Affero General Public License along with this program. If not, see < https://www.gnu.org/licenses/>.
>
> Electronic Mail : asdfghjkl851@outlook.com


# 概述 - Overview

这是一个轻量级的通用树数据结构容器, 源代码见`tree.hpp`文件. 该模块是 `GasSensorOS` 的组件之一, 最初是为了 `GS_Code` 代码解释器而开发的. 

这个类实现了一个通用的树数据结构，包括节点的添加、删除、查找、遍历等基本操作。该树数据结构由两个类组成：`TreeNode`和`Tree`。`TreeNode`表示一树节点，包含节点数据、子节点列表、父节点等属性；`Tree`表示整个树结构，包含根节点以及树的遍历、节点查找、节点删除等操作。具体来说，`TreeNode`类包含了添加子节点、查找子节点、删除子节点等方法。其中，添加子节点使用了`C++11`中的智能指针`std::unique_ptr`，确保了子节点的内存管理安全；查找子节点使用了递归的方式，深度优先遍历整个子树；删除子节点则使用了迭代的方式，遍历整个子树进行删除操作。`Tree`类包含了树的遍历、节点查找、节点删除等方法。其中，遍历操作分为深度优先遍历和广度优先遍历两种方式；节点查找操作同样使用了递归的方式，在根节点开始向下搜索整个子树；节点删除操作则使用了递归的方式，在整个子树中进行删除操作。总的来说，该树数据结构提供了基本的树操作，能够满足一些基本的需求。但是需要注意的是，该树数据结构没有进行任何的平衡操作，因此对于较大的树可能会存在效率问题。

*This is a lightweight, general-purpose tree data structure container, with the source code available in the `tree.hpp` file. This module is one of the components of GasSensorOS and was originally developed for the GS_Code code interpreter.*

## *Implementation*

*This class implements a general tree data structure, including basic operations such as adding, deleting, searching, and traversing nodes. The tree data structure consists of two classes: `TreeNode` and `Tree`.* 

*`TreeNode` represents a tree node, including node data, a list of child nodes, and parent node properties. `Tree` represents the entire tree structure, including root nodes and operations such as tree traversal, node searching, and node deletion.*

### *TreeNode Class*

*The `TreeNode` class contains methods for adding, searching, and deleting child nodes. Adding child nodes uses the smart pointer `std::unique_ptr` from C++11, ensuring safe memory management of child nodes. Searching for child nodes uses a recursive method, traversing the entire subtree in a depth-first manner. Deleting child nodes uses an iterative method, traversing the entire subtree for deletion operations.*

### *Tree Class*

*The `Tree` class contains methods for tree traversal, node searching, and node deletion. Traversal operations are divided into two methods: depth-first traversal and breadth-first traversal. Node searching uses a recursive method, searching the entire subtree downwards from the root node. Node deletion uses a recursive method to delete the entire subtree.*

## *Usage*

*This tree data structure provides basic tree operations that can meet some basic needs. However, it should be noted that this tree data structure does not perform any balancing operations, so there may be efficiency issues for larger trees.*

*To use this container, simply include the `tree.hpp` file in your project and instantiate the `Tree` class. You can then use the provided methods to add, delete, search, and traverse nodes in the tree data structure.*

## *Conclusion*

*Overall, this tree data structure provides a simple and efficient way to manage tree data structures in C++. It is a useful tool for those working with large sets of data that need to be organized in a tree structure.*

#  `TreeNode` 类 - `TreeNode` class

## 成员 - Members

### 构造函数 - Constructors

| 名称 - Name | 说明 - Description                                                                                                                                                                                                               |
| :---------- | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| TreeNode    | 构造节点. 该节点拥有值`data`和指向其父节点的指针`parent_node_ptr`这两个可构造的属性. <br/>*Constructing a node. The node has two constructible attributes, the value `data` and a pointer to its parent node `parent_node_ptr`.* |

### 变量 - Variables

| 名称 - Name | 说明 - Description                                                                      |
| :---------- | :-------------------------------------------------------------------------------------- |
| node_data   | 储存这个节点的值. <br/>*Store the value of this node.*                                  |
| children    | 储存指向这个节点的子节点的指针. <br/>*Stores a pointer to the child node of this node.* |
| parent      | 储存指向这个节点的父节点的指针. <br/>*Store a pointer to the parent of this node.*      |

### 方法 - Functions

| 名称 - Name    | 说明 - Description                                                                                                |
| :------------- | :---------------------------------------------------------------------------------------------------------------- |
| addChild       | 向当前节点添加一个子节点. <br/>*Add a child node to the current node.*                                            |
| findChild      | 在当前节点的子节点中查找节点. <br/>*Find a node among the children of the current node.*                          |
| findDescendant | 在当前节点的后裔中查找节点. <br/>*Find a node among the descendants of the current node.*                         |
| hasChildren    | 判断在当前节点的指定子节点是否有孩子. <br/>*Determine if a child of the current node has children.*               |
| deleteChild    | 删除父节点的一个无孩子节点(删除树叶节点). <br/>*Delete a childless node of the parent node (delete a leaf node).* |

# `Tree` 类 - `Tree` class
## 成员 - Members
### 构造函数 - Constructors

| 名称 - Name | 说明 - Description                                                                                             |
| :---------- | :------------------------------------------------------------------------------------------------------------- |
| Tree        | 构造一个树, 创建树的第一个节点(根节点). <br/>*Construct a tree, creating the first node (`root`) of the tree.* |

### 变量 - Variables

| 名称 - Name      | 说明 - Description                                                                                                                                                                |
| :--------------- | :-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| root             | 储存树的根节点. <br/>*The root node of the storage tree.*                                                                                                                         |
| current_node_ptr | 储存最后一次添加节点后的指针位置(初始化时设为根节点指针). <br/>*Store the position of the pointer after the last node was added (set to the root node pointer at initialization)* |

### 方法 - Functions

| 名称 - Name        | 说明 - Description                                                                                                                                                                                                                                            |
| :----------------- | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| addNode            | 向节点添加子节点, 并将指向新增节点的指针保存到类成员变量 `current_node_ptr` 中. <br/>*Add a child node to the node, and save the pointer to the new node in the class member variable `current_node_ptr`.*                                                    |
| traversalDFS       | 以深度优先的方式遍历树. <br/>*Traverses the tree in a depth-first style.*                                                                                                                                                                                     |
| traversalBFS       | 以广度优先的方式遍历树. <br/>*Traversing the tree in a breadth-first style.*                                                                                                                                                                                  |
| findNode           | 在树中查找节点. <br/>*Find nodes in the tree.*                                                                                                                                                                                                                |
| hasChildren        | 判断指定节点是否有孩子. <br/>*Determine if the specified node has children.*                                                                                                                                                                                  |
| getDepth           | 递归地计算树的深度(高度)(默认统计整颗树的深度). <br/>*Recursively calculates the depth (height) of the tree (by default the depth of the whole tree is counted).*                                                                                             |
| getDegree          | 获取节点的度(对于一个给定的节点，其子节点的数量称为度. 一个叶子的度数一定是零) <br/>*Get the degree of a node (for a given node, the number of its children is called the degree. The degree of a leaf must be zero)*                                         |
| get_degree_of_tree | 获取树的度(树的度是指树中一个节点的最大度) <br/>*Get the degree of the tree (the degree of the tree is the maximum degree of a node in the tree).*                                                                                                            |
| getBreadth         | 获取树或树枝的叶子数量(叶子即没有子节点的节点，也称做终端节点) <br/>*Get the number of leaves of the tree or branch (leaves are nodes without children, also known as terminal nodes).*                                                                       |
| getWidth           | 获取树的宽度或指定节点所在层的宽度(宽度指一个层的节点数) <br/>*Get the width of the tree or the width of the specified node's layer (width refers to the number of nodes in a layer).*                                                                        |
| getLevel           | 获取指定节点的层级(一个节点的层级是它与根节点之间唯一路径上的边的数量, 根节点层级为零). <br/>*Get the layer of the specified node (the layer of a node is the number of edges on the unique path between it and the root node, the root node layer is zero).* |
| getSize            | 获取树或树枝的大小(节点总数). <br/>*Get the size of a tree or branch.*                                                                                                                                                                                        |
| empty              | 判断树是否为空. <br/>*Determine if the tree is empty*                                                                                                                                                                                                         |
| deleteNode         | 递归删除一个节点及其后裔. <br/>*Recursively delete a node and its descendants.*                                                                                                                                                                               |
| deleteTree         | 删除一颗树. <br/>*Delete a tree.*                                                                                                                                                                                                                             |

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
    
    // 向根节点添加一个值为"node_1"的子节点.
    // Add a child node with the value "node_1" to the root node.
    tree.root->addChild("node_1");
}
```

```
ROOT
|--node_1
```

### 例2 - Example 2

以下代码演示了如何通过在树中以查找节点的方式添加子节点.

*The following code demonstrates how to add a child node in the tree by found the node.*

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

```C++
ROOT
|--node_1
   |--node_1_1
```
### 例3 - Example 3

以下代码演示了如何利用`addChild`的返回值.

*The following code demonstrates how to use the return value of `addChild`.*

```c++
#include <tree.hpp>
#include <string>
#include <Arduino.h>

void setup() {
    Serial.begin(115200);
    
	Tree<std::string> tree("ROOT");
    
    // 向根节点添加一个值为"node_1"子节点. 我们将 addChild 返回的指针赋值给 node_1_ptr, 这个指针即指向 node_1 的指针.
    // Add a child node with the value "node_1" to the root node. We assign the pointer returned by addChild to node_1_ptr, this pointer is the pointer to node_1.
    auto* node_1_ptr = tree.root->addChild("node_1");
    
    // 打印 "node_1" 节点的值.
    Serial.println(node_1_ptr->node_data.c_str());
}
```

```C++
ROOT
|--node_1
```
```C++
Output:

node_1
```
---
# `findChild`

 在当前节点的子节点中查找节点. <br/>*Find a node among the children of the current node.*

```c++
TreeNode<T>* parent_node_ptr->TreeNode<T>* findChild(const T& target_child_data);
```
## 参数 - Parameters

`parent_node_ptr`

指向父节点的指针, 表示查找该父节点的孩子.<br/>*A pointer to the parent node, which means to find the child of the parent node.*

`target_child_data`

表示待查找节点的值. <br/>*Indicates the value of the node to be find.*

## 返回值 - Return value

返回指向查找到的节点的指针，如果未找到则返回 `nullptr`.<br/>*Returns a pointer to the found node, or `nullptr` if it is not found.*

## 注解 - Remarks

通过用户提供的`parent_node_ptr`指针, 该函数会遍历该指针指向节点的`children`并且访问每个孩子的`node_data`与用户提供的`target_child_data`进行比较, 如果它们相等则返回指向查找到的节点的指针.



*With the user-supplied `parent_node_ptr` pointer, the function iterates through the `children` of the node pointed to by the pointer and accesses the `node_data` of each child to compare with the user-supplied `target_child_data`, returning a pointer to the found node if they are same.*

## 示例 - Example

```c++
#include <tree.hpp>
#include <string>

void setup() {
	Tree<std::string> tree("ROOT");
    
    tree.root->addChild("node_1");
    
    // 在根节点的孩子中查找并获取指向值为"node_1"的节点的指针.
    // Find and get a pointer to the node with the value "node_1" among the children of the root node.
    auto* node_1_ptr = tree.root->findChild("node_1");
    
    // 向 node_1 节点添加一个值为"node_1_1"的子节点.
    // Add a child node with value "node_1_1" to the node_1 node.
    node_1_ptr->addChild("node_1_1");
    
    // 在 node_1 节点的孩子中查找并获取指向值为"node_1_1"的节点的指针, 然后向该节点添加一个值为"node_1_1_1"的子节点.
    // Find and get a pointer to the node with the value "node_1_1" among the children of node_1, and add a child node with the value "node_1_1_1" to that node.
    node_1_ptr->findChild("node_1_1")->addChild("node_1_1_1");
}
```
```C++
ROOT
|--node_1
   |--node_1_1
      |--node_1_1_1
```

---

# `findDescendant`

在当前节点的后裔中查找节点. <br/>*Find a node among the descendants of the current node.*
```c++
TreeNode<T>* parent_node_ptr->TreeNode<T>* findDescendant(const T& target_node_data);
```
## 参数 - Parameters

`parent_node_ptr`

指向父节点的指针, 表示查找该父节点的后裔.<br/>*A pointer to the parent node, indicating to find the descendants of the parent node.*

`target_node_data`

表示待查找节点的值. <br/>*Indicates the value of the node to be find.*

## 返回值 - Return value

返回指向查找到的节点的指针，如果未找到则返回 `nullptr`.<br/>*Returns a pointer to the found node, or `nullptr` if it is not found.*

## 注解 - Remarks

这是一个在树结构中查找指定节点的方法，方法使用深度优先遍历，递归地在当前节点的子节点中查找指定数据，直到找到该数据或者遍历完整棵树。如果找到指定数据的节点，则返回该节点的指针；如果未找到，则返回 `nullptr`. (`findDescendant`与`findChild`的不同之处在于: `findChild`的查找范围为父节点的所有孩子, 而`findDescendant`的查找范围为父节点之后的所有节点.)



*This is a method to find a specified node in a tree structure. The method uses a depth-first traversal to recursively find the specified data in the children of the current node until the data is found or the entire tree is traversed. If the node with the specified data is found, it returns a pointer to that node; if not, it returns `nullptr`.* *(The difference between `findDescendant` and `findChild` is that `findChild` looks for all children of the parent node, while `findDescendant` looks for all nodes after the parent node.)*

## 示例 - Example

```c++
#include <tree.hpp>
#include <string>

void setup() {
	Tree<std::string> tree("ROOT");
    
    tree.root->addChild("node_1")->addChild("node_1_1")->addChild("node_1_1_1");
    
    // 在根节点的后裔中查找并获取指向值为"node_1_1_1"的节点的指针.
    // Find and get a pointer to the node with the value "node_1_1_1" among the descendants of the root node.
    auto* node_1_1_1_ptr = tree.root->findDescendant("node_1_1_1");
    
    node_1_1_1_ptr->addChild("node_1_1_1_1");
    node_1_1_1_ptr->addChild("node_1_1_1_2");
}
```
```c++
ROOT
|--node_1
   |--node_1_1
      |-node_1_1_1
         |-node_1_1_1_1
         |-node_1_1_1_2
```

---

# `hasChildren`

 判断在当前节点的指定子节点是否有孩子. <br/>*Determine if a child of the current node has children.*

```c++
TreeNode<T>* node_ptr->bool hasChildren();
```
## 参数 - Parameters

`node_ptr`

指向待判断节点的指针.<br/>*Pointer to the node to be judged*.

## 返回值 - Return value

如果存在子节点返回`true`, 否则返回`false`.<br/>*Returns `true` if a child node exists, otherwise returns `false`.*

## 注解 - Remarks

每个节点的子节点是以这种方式储存的: `std::vector<std::unique_ptr<TreeNode<T>>> children;`(即储存的是指向这个节点的子节点的指针). 本方法只需判断 `children`向量是否为空即可. 如果 `node_ptr->children` 不为空，则表示这个节点有孩子. 



*The children of each node are stored in this way: `std::vector<std::unique_ptr<TreeNode<T>>> children;` (i.e. a pointer to the children of this node is stored). This method simply determines if the `children` vector is empty. If `node_ptr->children` is not empty, then the node has children.*

## 示例 - Example

```c++
#include <tree.hpp>
#include <string>
#include <Arduino.h>

void setup() {
    Serial.begin(115200);
    
	Tree<std::string> tree("ROOT");
    
    auto* node_1_ptr = tree.root->addChild("node_1");
    
    // 判断根节点是否有孩子.
    // Determine if the root node has children.
    Serial.print("ROOT: ");
    Serial.println(tree.root->hasChildren() ? "With children" : "No children");
    
    // 判断 node_1_ptr 是否有孩子.
    // Determine if the node_1_ptr node has children.
    Serial.print("node_1: ");
    Serial.println(node_1_ptr->hasChildren() ? "With children" : "No children");
}
```
```C++
ROOT
|--node_1
```

```C++
Output:

ROOT: With children
node_1: No children
```
---

# `deleteChild`

删除父节点的一个无孩子节点(删除树叶节点). <br/>*Delete a childless node of the parent node (delete a leaf node).*


```c++
TreeNode<T>* parent_node_ptr->bool deleteChild(const T& target_child_data);
```
## 参数 - Parameters

`parent_node_ptr`

指向父节点的指针, 表示我们删除这个父节点的一个孩子.<br/>*A pointer to a parent node means we delete a child of that parent node.*

`target_node_data`

这里需要提供待删除的目标子节点的值.<br/>*Here you need to provide the value of the target child node to be deleted.*

## 返回值 - Return value

删除成功返回`true`，否则返回`false`.<br/>*Returns `true` for successful deletion, otherwise returns `false`.*

## 注解 - Remarks

这个函数首先通过调用 `findChild()` 函数查找要删除的子节点。如果找不到这个子节点，或者这个子节点有其他子节点，那么删除操作会失败，函数会返回 false。如果找到了要删除的子节点，并且它没有其他子节点，那么函数会遍历当前节点的所有子节点，找到要删除的子节点并删除它。在删除节点时，由于使用了 `std::unique_ptr` 来管理子节点，所以父节点可以在内存管理方面自动处理子节点的内存释放，不需要手动释放。最后，如果删除成功，函数会返回 true。

需要注意的是，这个函数只支持删除没有子节点的节点，即树枝的末端(树叶)。如果要删除整棵树或部分树枝，请使用 `deleteNode()` 函数。



*This function first uses the `findChild()` function to search for the child node to be deleted. If the child node is not found, or if it has other child nodes, the delete operation will fail and the function will return false. If the child node to be deleted is found and it does not have any other child nodes, the function will traverse all of the current node's child nodes, find the one to be deleted, and remove it. When deleting the node, because `std::unique_ptr` is used to manage the child node, the parent node can automatically handle the memory release of the child node without the need for manual release. Finally, if the deletion is successful, the function will return true.*

*It should be noted that this function only supports deleting nodes without child nodes, i.e. the end of the branch (leaf) of the tree. If you want to delete the entire tree or part of the branch, please use the `deleteNode()` function.*

## 示例 - Example

```c++
#include <tree.hpp>
#include <string>

void setup() {
	Tree<std::string> tree("ROOT");
    
    tree.root->addChild("node_1")->addChild("node_1_1");
    
    auto* node_1_ptr = tree.root->findChild("node_1");
    
    // 删除 node_1 的子节点 node_1_1.
    // Delete the child node node_1_1 of node_1.
    node_1_ptr->deleteChild("node_1_1");
}
```
```c++
ROOT
|--node_1
```

---
# `Tree`构造函数 - `Tree` constructor

构造一个树, 创建树的第一个节点(根节点). <br/>*Construct a tree, creating the first node (`root`) of the tree.*


```c++
Tree(const T& data) : root(make_unique<TreeNode<T>>(data)) {}
```
## 参数 - Parameters

`data`

表示根节点的值.<br/>*Indicates the value of the root node.*

## 返回值 - Return value

`void`

## 注解 - Remarks

构造函数采用了成员初始化列表的方式来初始化成员变量。其中，`root` 是一个 `std::unique_ptr` 类型的智能指针，用于管理根节点。`make_unique<TreeNode<T>>(data)` 是一个 C++11 引入的标准函数，用于在堆上创建一个新的节点，并返回一个 `std::unique_ptr` 智能指针，该智能指针会自动管理这个新节点的内存。函数的参数 `data` 表示根节点的数据。

因此，这段代码的作用是在堆上创建一个新的树根节点，并用 `std::unique_ptr` 智能指针来管理这个节点的内存。同时，将参数 `data` 的值传递给新节点的构造函数，用来初始化根节点的数据。



*The constructor uses member initialization list to initialize its member variables. `root` is a smart pointer of type `std::unique_ptr`, used to manage the root node of the binary tree. `make_unique<TreeNode<T>>(data)` is a standard function introduced in C++11, which creates a new node on the heap and returns a `std::unique_ptr` smart pointer that automatically manages the memory of the new node. The parameter `data` represents the data of the root node.*

*Therefore, this code creates a new tree root node on the heap, and uses `std::unique_ptr` smart pointer to manage the memory of this node. At the same time, it passes the value of the parameter `data` to the constructor of the new node to initialize the data of the root node.*

## 示例 - Example

```c++
#include <tree.hpp>
#include <string>

void setup() {
    // 构造一个树, 创建树的第一个节点(根节点). 
    // Construct a tree, creating the first node (root) of the tree.
	Tree<std::string> tree("ROOT");
}
```
```c++
ROOT
```

---
# `addNode`

向节点添加子节点, 并将指向新增节点的指针保存到类成员变量 `current_node_ptr` 中. <br/>*Add a child node to the node, and save the pointer to the new node in the class member variable `current_node_ptr`.*


```c++
 TreeNode<T>* addNode(TreeNode<T>* node_ptr, const T& data);
```
## 参数 - Parameters

`node_ptr`

表示在该节点下添加子节点.<br/>*Indicates that a child node is added under this node.*

`data`

表示节点的值, 即节点储存的内容. 其数据类型等于实例化树容器时的数据类型(可以是C++STL中的任意数据类型). <br/>*Represents the value of a node, i.e. the content stored in the node. Its data type is equal to the data type of the tree container when it is instantiated (it can be any data type in C++STL).*

## 返回值 - Return value

返回一个指向新加子节点的指针.<br/>*Returns a pointer to the newly added child node.*

## 注解 - Remarks

当调用 `addNode()` 函数时，它将创建一个新的 `TreeNode` 对象，该对象保存传递给函数的数据，并将指向新创建节点的指针添加到当前节点的 `children` 向量中。也就是说，`addNode()` 添加的是一个新的子节点。`addNode` 与 `addChild` 不同, `addNode` 是 `Tree class` 的成员, 而 `addChild` 是 `TreeNode class` 的成员, `addNode` 将父节点指针作为参数传递. 该函数还会将指向新增节点的指针保存到类成员变量 `current_node_ptr` 中, 以便用户更清楚当前树的编辑位置.



*When the `addNode()` function is called, it creates a new `TreeNode` object, which holds the data passed to the function and adds a pointer to the newly created node to the `children` vector of the current node. In other words, `addNode()` adds a new child node. Unlike `addNode`, which is a member of the `Tree class`, and `addChild`, which is a member of the `TreeNode class`, `addNode` passes the parent node pointer as an argument. This function also saves a pointer to the new node in the class member variable `current_node_ptr`, so that the user is more aware of the current edit position of the tree.*

## 示例 - Example

### 例1 - Example 1

该例子演示了如何使用 `addNode();` 函数添加一个节点.<br/>*This example demonstrates how to add a node using the `addNode();` function.*

```c++
#include <tree.hpp>
#include <string>

void setup() {
	Tree<std::string> tree("ROOT");
    
    // 向根节点添加一个值为"node_1"的子节点.
    // Add a child node with the value "node_1" to the root node.
    tree.addNode(tree.root, "node_1");
}
```
```c++
ROOT
|--node_1
```
### 例2 - Example 2

该例子演示了如何利用`addNode()`在添加新节点后会将指向新增节点的指针更新到`current_node_ptr`的特性.<br/>*This example shows how to take advantage of the fact that `addNode()` updates the pointer to the new node in `current_node_ptr` after it is added.*

```c++
#include <tree.hpp>
#include <string>

void setup() {
	Tree<std::string> tree("ROOT");
    
    // 这里利用了 tree 类成员变量 current_node_ptr 的初始值为指向根节点的指针, 然后向根节点添加一个值为"node_1"的子节点.
    // The initial value of the tree class member variable current_node_ptr is a pointer to the root node, and a child node with the value "node_1" is added to the root node.
    tree.addNode(tree.current_node_ptr, "node_1");
    
    // 上一次 addNode() 的调用时, 在向根节点添加子节点后, 函数就将指向新增节点的指针保存到类成员变量 current_node_ptr 中, 此时 current_node_ptr 为指向 node_1 都指针, 于是这条语句是在向 node_1 添加一个值为"node_1_1"的子节点.
    // In the last addNode() call, after adding a child node to the root node, the function saves a pointer to the new node in the class member variable current_node_ptr, which is a pointer to node_1, so this statement is adding a child node to node_1 with the value "node_1_1". to node_1.
    tree.addNode(tree.current_node_ptr, "node_1_1");
}
```
```c++
ROOT
|--node_1
   |--node_1_1
```


---
# `traversalDFS`

以深度优先的方式遍历树. <br/>*Traverses the tree in a depth-first style.*


```c++
std::vector<std::pair<T, TreeNode<T>*>> traversalDFS(TreeNode<T>* node_ptr = nullptr);
```
## 参数 - Parameters

`node_ptr`

提供一个节点指针，函数会以该节点为根节点递归遍历它所有的子嗣节点(若不传参则默认遍历整颗树).<br/>*Provide a node pointer, the function will use this node as the root node to recursively traverse all its descendant nodes (if no parameter is passed, the entire tree will be traversed by default).*

## 返回值 - Return value

返回一个向量, 其中包含从指定节点开始子树的所有值和对应的指针.<br/>*Returns a vector containing all values and corresponding pointers for the subtree starting from the specified node.*

```C++
std::vector<std::pair<T, TreeNode<T>*>> returnValue = traversalDFS();

// 使用迭代器遍历树中每个节点的值和指针.
// Iterate over the values and pointers of each node in the tree using iterators.
for (auto& data : tree_data) {
    data.first;		// node_value;
    data.second;	// node_ptr;
}
```

## 注解 - Remarks

这段代码定义了一个以深度优先方式遍历树的函数`traversalDFS`，可以遍历整个树或从指定节点开始递归遍历其所有子节点，返回一个包含所有遍历节点数据值和对应指针的向量。具体实现是通过递归遍历当前节点的每个子节点，将子节点的子树数据插入到包含当前节点的向量中。函数首先判断传入的节点指针是否为空，为空则默认遍历整个树。最后返回包含所有节点数据的向量。

深度优先遍历算法是递归的，它首先访问根节点，然后再递归地遍历每个子树。在每个节点访问完成后，递归函数回溯到其父节点继续遍历其他子树.



*This code defines a function called `traversalDFS` that traverses a tree in a depth-first manner. It can traverse the entire tree or recursively traverse all its child nodes starting from a specified node, and returns a vector containing the data values and corresponding pointers of all traversed nodes. The implementation involves recursively traversing each child node of the current node and inserting the subtree data of the child node into the vector containing the current node. The function first checks if the input node pointer is null, and if so, it defaults to traversing the entire tree. Finally, it returns a vector containing the data of all nodes.*

*The depth-first traversal algorithm is recursive, it first visits the root node and then recursively traverses each subtree. After visiting each node, the recursive function backtracks to its parent node to continue traversing other subtrees.*

## 示例 - Example

```c++
#include <tree.hpp>
#include <string>
#include <Arduino.h>

void setup() {
    Serial.begin(115200);
    
	Tree<std::string> tree("ROOT");
    
    tree.root->addChild("node_1")->addChild("node_1_1")->addChild("node_1_1_1");
    tree.root->addChild("node_2")->addChild("node_2_1");
    
    // 以深度优先的方式遍历整颗树. 
    // Traverse the entire tree in a depth-first style. 
    auto tree_data = tree.traversalDFS();
    
    // 使用迭代器遍历并打印树中每个节点的值.
    // Use an iterator to traverse and print the value of each node in the tree.
    for (auto& data : tree_data) Serial.println(data.first.c_str());
}
```
```c++
ROOT
|--node_1
|  |--node_1_1
|     |--node_1_1_1
|--node_2
   |--node_2_1
```
```c++
Output:

ROOT
node_1
node_1_1
node_1_1_1
node_2
node_2_1
```
---
# `traversalBFS`

以广度优先的方式遍历树. <br/>*Traversing the tree in a breadth-first style.*


```c++
 std::vector<std::pair<T, TreeNode<T>*>> traversalBFS(TreeNode<T>* node_ptr = nullptr);
```
## 参数 - Parameters

`node_ptr`

提供一个节点指针，函数会以该节点为根节点递归遍历它所有的子嗣节点(若不传参则默认遍历整颗树).<br/>*Provide a node pointer, the function will use this node as the root node to recursively traverse all its descendant nodes (if no parameter is passed, the entire tree will be traversed by default).*

## 返回值 - Return value

返回一个向量, 其中包含从指定节点开始子树的所有值和对应的指针.<br/>*Returns a vector containing all values and corresponding pointers for the subtree starting from the specified node.*

```C++
std::vector<std::pair<T, TreeNode<T>*>> returnValue = traversalBFS();

// 使用迭代器遍历树中每个节点的值和指针.
// Iterate over the values and pointers of each node in the tree using iterators.
for (auto& data : tree_data) {
    data.first;		// node_value;
    data.second;	// node_ptr;
}
```

## 注解 - Remarks

这个方法定义了一个广度优先搜索遍历树的函数 `traversalBFS`，它的输入参数是指向树节点的指针 `node_ptr`，默认为空，表示从根节点开始遍历。函数返回一个向量，向量中包含每个节点及其子节点的数据（类型为 `std::pair<T, TreeNode<T>*>`，其中 T 是节点存储的数据类型）。遍历是通过维护一个队列来完成的，从队列中取出一个节点，将其所有子节点插入队列中，并将该节点的数据及指针插入向量中，直到队列为空。如果输入的节点为空，则返回一个空向量。

广度优先遍历算法是按层遍历，从根节点开始，先遍历根节点，然后按照从左到右的顺序遍历其子节点，再依次遍历下一层的所有节点。



*This method defines a function `traversalBFS` that performs a breadth-first search traversal of a tree. Its input parameter is a pointer `node_ptr` that points to a tree node and defaults to `nullptr`, indicating that the traversal should start from the root node. The function returns a vector that contains the data of each node and its child nodes (of type `std::pair<T, TreeNode<T>*>`, where T is the data type stored in the node). The traversal is accomplished by maintaining a queue, extracting a node from the queue, inserting all its child nodes into the queue, and inserting the node's data and pointer into the vector until the queue is empty. If the input node is empty, an empty vector is returned.*

*The breadth-first traversal algorithm traverses a tree layer by layer, starting from the root node. It first visits the root node, then visits its child nodes from left to right, and then visits all nodes in the next layer in sequence.*

## 示例 - Example

```c++
#include <tree.hpp>
#include <string>
#include <Arduino.h>

void setup() {
	Serial.begin(115200);

    Tree<std::string> tree("ROOT");

    tree.root->addChild("node_1")->addChild("node_1_1")->addChild("node_1_1_1");
    tree.root->addChild("node_2")->addChild("node_2_1");

    // 以广度优先的方式遍历整颗树.
    // Traverse the entire tree in a breadth-first style.
    auto tree_data = tree.traversalBFS();

    for (auto& data : tree_data) Serial.println(data.first.c_str());
}
```
```c++
ROOT
|--node_1
|  |--node_1_1
|     |--node_1_1_1
|--node_2
   |--node_2_1
```
```c++
Output:

ROOT
node_1
node_2
node_1_1
node_2_1
node_1_1_1
```
---
# `findNode`

在树中查找节点. <br/>*Find nodes in the tree.*


```c++
TreeNode<T>* findNode(const T& target_node_data);
```
## 参数 - Parameters

`target_node_data`

要查找的节点的值. (本方法会在树中查找该值, 如果找到就返回指向拥有该值的节点的指针).<br/>*The value of the node to look for. (This method looks for the value in the tree and returns a pointer to the node that has the value if it is found).*

## 返回值 - Return value

返回指向查找到的节点的指针，如果未找到则返回 `nullptr`.<br/>*Returns a pointer to the found node, or `nullptr` if it is not found.*

## 注解 - Remarks

本方法定义了一个查找目标节点的函数 `findNode`，它的输入参数是目标节点的值 `target_node_data`，返回类型是指向节点的指针 `TreeNode<T>*`。在函数中，首先调用根节点的 `findDescendant` 函数在根节点的子节点中查找目标节点，如果查找成功，则直接返回该节点的指针。如果查找失败，则通过比较根节点的数据和目标节点的数据来判断是否为根节点，如果是，则返回根节点的指针，否则返回空指针。

该函数的实现是基于一个假设：在查找范围内（不包括根节点）只有一个节点具有目标数据。如果查找范围内有多个节点具有目标数据，则该函数只会返回其中一个节点的指针，而不是所有节点的指针。



*This method defines a function `findNode` to search for a target node in a tree. The input parameter of this function is the value of the target node `target_node_data`, and the return type is a pointer to the node `TreeNode<T>*`. In the function, it first calls the `findDescendant` function of the root node to search for the target node in the children of the root node. If the search is successful, it directly returns the pointer to that node. If the search fails, it determines whether it is the root node by comparing the data of the root node and the data of the target node. If it is, it returns the pointer to the root node. Otherwise, it returns a null pointer.*

*The implementation of this function is based on the assumption that only one node in the search range (excluding the root node) has the target data. If there are multiple nodes with the target data in the search range, the function will only return the pointer to one of them, not all of them.*

## 示例 - Example

```c++
#include <tree.hpp>
#include <string>

void setup() {
	Tree<std::string> tree("ROOT");
    
    tree.root->addChild("node_1")->addChild("node_1_1");
    
    // 在树中查找值为"node_1_1"的节点.
    // Find the node with the value "node_1_1" in the tree.
    auto node_1_1_ptr = tree.findNode("node_1_1");
    
    node_1_1_ptr->addChild("node_1_1_1");
}
```
```c++
ROOT
|--node_1
   |--node_1_1
      |--node_1_1_1
```

---
# `hasChildren`

判断指定节点是否有孩子. <br/>*Determine if the specified node has children.*


```c++
bool hasChildren(TreeNode<T>* node_ptr);
```
## 参数 - Parameters

`node_ptr`

提供一个节点指针，本方法会判断这个节点是否有孩子.<br>*Provide a node pointer and this method will determine if the node has children.*

## 返回值 - Return value

如果存在子节点返回`true`, 否则返回`false`.<br>*Returns true if a child node exists, otherwise returns false.*

## 注解 - Remarks

通过直接判断`node_ptr`指向的节点的`children`向量是否为空来确定是否存在孩子, 如果 `node_ptr->children` 不为空则表示这个节点有孩子.<br>*Determine if there are children by directly determining if the `children` vector of the node pointed to by `node_ptr` is empty, if `node_ptr->children` is not empty then the node has children.*

## 示例 - Example

```c++
#include <tree.hpp>
#include <string>
#include <Arduino.h>

void setup() {
    Serial.begin(115200);
    
	Tree<std::string> tree("ROOT");
    
    auto node_1_ptr = tree.root->addChild("node_1");
    auto node_1_1_ptr = node_1_ptr->addChild("node_1_1");
    
    // 判断 node_1 节点是否有孩子;
    // Determine if node_1 has children;
    bool hasChildren_node_1 = tree.hasChildren(node_1_ptr);
    
    // 判断 node_1_1 节点是否有孩子;
    // Determine if node_1_1 has children;
    bool hasChildren_node_1_1 = tree.hasChildren(node_1_1_ptr);
    
    Serial.println(hasChildren_node_1);
    Serial.println(hasChildren_node_1_1);
}
```
```c++
ROOT
|--node_1
|  |--node_1_1
```
```c++
Output:

true	// node_1 节点有孩子; node_1 has children;
false	// node_1_1 节点没有孩子; node_1_1 no children;
```

---
# `getDepth`

递归地计算树的深度(高度)(默认统计整颗树的深度). <br/>*Recursively calculates the depth (height) of the tree (by default the depth of the whole tree is counted).*


```c++
uint32_t getDepth(TreeNode<T>* node_ptr = nullptr);
```
## 参数 - Parameters

`node_ptr`

提供一个节点指针，表示从该节点开始统计树枝的深度，若设为root则为统计整颗树的深度(这也是无传参时的默认设置).<br>*Provide a pointer to the node from which the depth of the branch is counted, or if set to root, the depth of the whole tree (this is also the default setting when no parameters are passed).*

## 返回值 - Return value

返回指定树支的深度.<br/>*Returns the depth of the specified tree branch.*

## 注解 - Remarks

这段代码的作用是计算一棵树的深度，返回以某个节点为根节点的子树的最大深度。如果没有传入节点指针，则默认以根节点为参数计算树的深度。函数首先检查传入的节点是否为叶子节点，如果是，则返回1作为高度。否则，对于当前节点的每个子节点，递归计算其子树的高度，并找到其中最大的高度。最后返回最大高度加上1，即为整个树的高度。函数的实现使用了C++标准库中的`std::max`函数，用于比较两个数的大小并返回较大的那个数。<br>*The purpose of this code is to calculate the depth of a tree and return the maximum depth of the subtree rooted at a given node. If no node pointer is passed, the function defaults to calculating the depth of the tree rooted at the root node. The function first checks if the passed node is a leaf node, and if so, returns 1 as the height. Otherwise, for each child node of the current node, it recursively calculates the height of its subtree and finds the maximum height. Finally, it returns the maximum height plus 1, which is the height of the entire tree. The implementation of the function uses the `std::max` function from the C++ standard library to compare two numbers and return the larger one.*

## 示例 - Example

```c++
#include <tree.hpp>
#include <string>
#include <Arduino.h>

void setup() {
	Serial.begin(115200);

    Tree<std::string> tree("ROOT");

    tree.root->addChild("node_1")->addChild("node_1_1")->addChild("node_1_1_1");
    tree.root->addChild("node_2")->addChild("node_2_1");
    
    // 获取整颗树的深度.
    // Get the depth of the whole tree.
    uint32_t root_depth = tree.getDepth();
    
    // 获取以 node_2 为根节点的树枝的深度.
    // Get the depth of the tree branch with node_2 as the root node.
    uint32_t node_2_depth = tree.getDepth(tree.findNode("node_2"));
    
    Serial.println(root_depth);
    Serial.println(node_2_depth);
}
```
```c++
ROOT
|--node_1
|  |--node_1_1
|     |--node_1_1_1
|--node_2
   |--node_2_1
```
```c++
Output:

4	// 整颗树的深度; The depth of the whole tree;
2	// 以 node_2 为根节点的树枝的深度; The depth of the tree branch with node_2 as the root node;
```
---
# `getDegree`

获取节点的度(对于一个给定的节点，其子节点的数量称为度. 一个叶子的度数一定是零) <br/>*Get the degree of a node (for a given node, the number of its children is called the degree. The degree of a leaf must be zero)*


```c++
uint32_t getDegree(TreeNode<T>* node_ptr = nullptr);
```
## 参数 - Parameters

`node_ptr`

提供一个节点指针，本方法会获取该节点的度(无传参时默认获取根节点的度).<br>*Provide a node pointer, this method will get the degree of the node (default get the degree of the root node when no parameters are passed).*

## 返回值 - Return value

返回目标节点的度.<br>*Returns the degree of the target node.*

## 注解 - Remarks

这段代码的作用是计算一棵树的某个节点的度数，即返回以该节点为根节点的子树中子节点的数量。如果没有传入节点指针，则默认以根节点为参数计算该节点的度数。函数直接返回目标节点的子节点数量。<br>*The purpose of this code is to calculate the degree of a node of a tree, i.e., to return the number of children in the subtree with that node as the root. If no node pointer is passed in, the degree of the node is calculated by default with the root node as the argument. The function directly returns the number of children of the target node.*

## 示例 - Example

```c++
#include <tree.hpp>
#include <string>
#include <Arduino.h>

void setup() {
	Serial.begin(115200);

    Tree<std::string> tree("ROOT");

    tree.root->addChild("node_1")->addChild("node_1_1")->addChild("node_1_1_1");
    tree.root->addChild("node_2")->addChild("node_2_1");
    
    // 获取根节点的度.
    // Get the degree of the root node.
    uint32_t root_degree = tree.getDegree();
    
    // 获取 node_2 的度.
    // Get the degree of node_2.
    uint32_t node_2_degree = tree.getDegree(tree.findNode("node_2"));
    
    Serial.println(root_degree);
    Serial.println(node_2_degree);
}
```
```c++
ROOT
|--node_1
|  |--node_1_1
|     |--node_1_1_1
|--node_2
   |--node_2_1
```
```c++
Output:

2	// 根节点的度; The degree of the root node;
1	// node_2 的度; The degree of node_2;
```
---
# `get_degree_of_tree`

获取树的度(树的度是指树中一个节点的最大度, 即树中某个拥有最多子节点的父节点的子节点数) <br/>*Get the degree of the tree (the degree of the tree is the maximum degree of a node in the tree, i.e., the number of children of a parent node that has the most children in the tree).*


```c++
uint32_t get_degree_of_tree();
```
## 返回值 - Return value

返回树的度.<br>*Returns the degree of the tree.*

## 注解 - Remarks

本方法的作用是计算一棵树中所有节点的度数，并返回其中最大的度数。函数使用深度优先遍历遍历树的所有节点，并获取每个节点的子节点个数。遍历过程中，记录当前遍历到的最大度数，最后返回树中所有节点的度数的最大值。<br>*The purpose of this method is to calculate the degrees of all nodes in a tree and return the maximum degree. The function uses depth-first traversal to traverse all the nodes of the tree and get the number of child nodes for each node. During the traversal process, the current maximum degree is recorded, and finally the maximum value of the degrees of all nodes in the tree is returned.*

## 示例 - Example

```c++
#include <tree.hpp>
#include <string>
#include <Arduino.h>

void setup() {
	Serial.begin(115200);

    Tree<std::string> tree("ROOT");

    tree.root->addChild("node_1")->addChild("node_1_1")->addChild("node_1_1_1");
    tree.root->addChild("node_2")->addChild("node_2_1");
    
    // 获取树的度.
    // Get the degree of the tree.
    uint32_t tree_degree = tree.get_degree_of_tree();
    
    Serial.println(tree_degree);
}
```
```c++
ROOT
|--node_1
|  |--node_1_1
|     |--node_1_1_1
|--node_2
   |--node_2_1
```
```c++
Output:

2	// 整颗树中根节点拥有最多的子节点, 因此树的度等于根节点的度; The root node has the most children in the whole tree, so the degree of the tree is equal to the degree of the root node.;
```
---
# `getBreadth`

获取树或树枝的叶子数量(叶子即没有子节点的节点，也称做终端节点) <br/>*Get the number of leaves of the tree or branch (leaves are nodes without children, also known as terminal nodes).*


```c++
uint32_t getBreadth(TreeNode<T>* node_ptr = nullptr);
```
## 参数 - Parameters

`node_ptr`

提供一个节点指针，本方法会获取以该节点为根节点的树枝的叶子数量(无传参时默认获取整颗树的叶子数量).<br>*Provide a node pointer, this method will get the number of leaves of the tree branch with this node as the root node (the default is to get the number of leaves of the whole tree when no parameters are passed).*

## 返回值 - Return value

返回树或指定树枝的叶子数量.<br>*Returns the number of leaves of a tree or a specified branch.*

## 注解 - Remarks

该函数的作用是计算以某个节点为根节点的子树中的叶子节点数量。如果没有传入节点指针，则默认以根节点为参数计算叶子节点数量。函数首先将叶子节点数量初始化为0，然后对子树进行深度优先遍历。在遍历过程中，如果当前遍历到的节点没有子节点，则增加叶子数。最后返回子树中叶子节点的数量。<br>*The function is used to calculate the number of leaf nodes in the subtree rooted at a given node. If no node pointer is passed, the number of leaf nodes under the root node is calculated by default. The function first initializes the number of leaf nodes to 0, and then performs a depth-first traversal of the subtree. During the traversal, if the current node being visited has no children, the number of leaves is incremented. Finally, the function returns the number of leaf nodes in the subtree.*

## 示例 - Example

```c++
#include <tree.hpp>
#include <string>
#include <Arduino.h>

void setup() {
	Serial.begin(115200);

    Tree<std::string> tree("ROOT");

    tree.root->addChild("node_1")->addChild("node_1_1")->addChild("node_1_1_1");
    tree.root->addChild("node_2")->addChild("node_2_1");
    
    // 获取整颗树的叶子数量;
    // Get the number of leaves of the whole tree.
    uint32_t tree_breadth = tree.getBreadth();
    
    // 获取 node_1 节点的叶子数量;
    // Get the number of leaves of node_1.
    uint32_t node_1_breadth = tree.getBreadth(tree.findNode("node_1"));
    
    Serial.println(tree_breadth);
    Serial.println(node_1_breadth);
}
```
```c++
ROOT
|--node_1
|  |--node_1_1
|     |--node_1_1_1
|--node_2
   |--node_2_1
```
```c++
Output:

2	// tree_breadth;
1	// node_1_breadth;
```
---
# `getWidth`

获取树的宽度或指定节点所在层的宽度(宽度指一个层的节点数) <br/>*Get the width of the tree or the width of the specified node's layer (width refers to the number of nodes in a layer).*


```c++
uint32_t getWidth(TreeNode<T>* node_ptr = nullptr);
```
## 参数 - Parameters

`node_ptr`

提供一个节点指针，表示获取该节点所在层的宽度，无传参时默认获取整个树的宽度(拥有最大宽度的层级).<br>*Provide a node pointer to get the width of the layer in which the node is located, and by default get the width of the whole tree (the layer with the maximum width) when no parameters are passed.*

## 返回值 - Return value

无参数时返回树的宽度，有参数时返回目标节点所在层的宽度.<br>*Returns the width of the tree with no arguments and the width of the target node's layer with arguments.*

## 注解 - Remarks

该函数的作用是计算以某个节点为根节点的子树的宽度。如果没有传入节点指针，则默认以根节点为参数计算子树的宽度。函数首先使用广度优先搜索遍历每个节点，并记录每个层级的节点数，同时找到宽度最大的层级。最后，如果传入的指针是根节点指针或空指针，则返回整棵树的宽度，否则返回该节点所在层级的节点数。<br>*The purpose of this function is to calculate the width of a subtree rooted at a specific node. If no node pointer is passed in, the function will default to calculating the width of the subtree rooted at the root node. The function first traverses every node using breadth-first search and records the number of nodes at each level while finding the level with the maximum width. Finally, if the passed-in pointer is the root node pointer or a null pointer, it returns the width of the entire tree; otherwise, it returns the number of nodes at the level where the passed-in node resides.*

## 示例 - Example

```c++
#include <tree.hpp>
#include <string>
#include <Arduino.h>

void setup() {
	Serial.begin(115200);

    Tree<std::string> tree("ROOT");

    tree.root->addChild("node_1")->addChild("node_1_1")->addChild("node_1_1_1");
    tree.root->addChild("node_2")->addChild("node_2_1");
    
    // 获取整颗树的宽度;
    // Get the width of the whole tree.
    uint32_t tree_width = tree.getWidth();
    
    // 获取 node_1 节点的宽度;
    // Get the width of the node_1 node.
    uint32_t node_1_width = tree.getWidth(tree.findNode("node_1"));
    
    Serial.println(tree_width);
    Serial.println(node_1_width);
}
```
```c++
ROOT
|--node_1
|  |--node_1_1
|     |--node_1_1_1
|--node_2
   |--node_2_1
```
```c++
Output:

2	// tree_width;
1	// node_1_width;
```
---
# `getLevel`

获取指定节点的层级(一个节点的层级是它与根节点之间唯一路径上的边的数量, 根节点层级为零). <br/>*Get the layer of the specified node (the layer of a node is the number of edges on the unique path between it and the root node, the root node layer is zero).*


```c++
int32_t getLevel(TreeNode<T>* node_ptr);
```
## 参数 - Parameters

`node_ptr`

提供一个节点指针，表示获取该节点的层级.<br>*Provide a node pointer indicating the level at which to get the node.*

## 返回值 - Return value

返回指定节点的所在层级数, 如果提供的节点指针为空指针则返回`-1`.<br>*Returns the number of levels of the specified node, or `-1` if the supplied node pointer is a null pointer.*

## 注解 - Remarks

该函数的作用是计算给定节点在树中所处的层级。函数首先检查给定的节点指针是否为空，如果为空，则返回-1。然后，使用循环从给定节点向其父节点逐层查找，每找到一个父节点，就将节点的层级加1。直到查找到根节点，即节点的父节点为空为止。最后返回节点的层级。<br>*The purpose of this function is to calculate the level of a given node in a tree. The function first checks if the given node pointer is null, and if so, it returns -1. Then, it uses a loop to traverse up the tree from the given node to its parent node at each iteration and increment the node's level by 1. The loop continues until the root node is reached, which has no parent. Finally, the function returns the level of the node.*

## 示例 - Example

```c++
#include <tree.hpp>
#include <string>
#include <Arduino.h>

void setup() {
	Serial.begin(115200);

    Tree<std::string> tree("ROOT");

    tree.root->addChild("node_1")->addChild("node_1_1")->addChild("node_1_1_1");
    tree.root->addChild("node_2")->addChild("node_2_1");
    
    // 获取 node_2 的层级.
    // Get the level of node_2.
    int32_t node_2_level = tree.getLevel(tree.findNode("node_2"));
    
    // 获取 node_1_1_1 的层级.
    // Get the level of node_1_1_1.
    int32_t node_1_1_1_level = tree.getLevel(tree.findNode("node_1_1_1"));
    
    Serial.println(node_2_level);
    Serial.println(node_1_1_1_level);
}
```
```c++
ROOT
|--node_1
|  |--node_1_1
|     |--node_1_1_1
|--node_2
   |--node_2_1
```
```c++
Output:

1	// node_2_level;
3	// node_1_1_1_level;
```
---
# `getSize`

获取树或树枝的大小(节点总数). <br/>*Get the size of a tree or branch.*


```c++
uint32_t getSize(TreeNode<T>* node_ptr = nullptr)
```
## 参数 - Parameters

`node_ptr`

提供一个节点指针，表示获取以该节点为起点的树枝的节点总数，无传参时默认获取整个树的节点总数.<br>*Provide a node pointer to get the total number of nodes in the branch starting from this node, or get the total number of nodes in the whole tree by default if no parameters are passed.*

## 返回值 - Return value

返回树或指定树枝的节点总数(包含根节点在内).<br>*Returns the total number of nodes in the tree or the specified branch.(including the root node)*

## 注解 - Remarks

该函数的作用是计算以给定节点为根的子树中的节点总数。如果没有传入节点指针，则默认以根节点为参数计算子树的大小。函数首先检查传入的节点指针是否为空，如果为空，则将其设置为根节点。然后对以该节点为根的子树进行深度优先遍历，最后返回遍历得到的节点数。<br>*The function aims to calculate the total number of nodes in the subtree rooted at a given node. If no node pointer is passed, it defaults to calculating the size of the subtree rooted at the root node. The function first checks if the given node pointer is null, and if so, sets it to the root node. Then it performs a depth-first traversal of the subtree rooted at that node, and finally returns the number of nodes obtained during the traversal.*

## 示例 - Example

```c++
#include <tree.hpp>
#include <string>
#include <Arduino.h>

void setup() {
	Serial.begin(115200);

    Tree<std::string> tree("ROOT");

    tree.root->addChild("node_1")->addChild("node_1_1")->addChild("node_1_1_1");
    tree.root->addChild("node_2")->addChild("node_2_1");
    
    // 获取整颗数就节点总数;
    // Get the whole number of nodes.
    uint32_t root_Branch_size = tree.getSize();
    
	// 获取以 node_1 节点五根节点的树枝的节点总数;
    // Get the total number of nodes in the tree branch with five nodes at node_1.
    uint32_t node_1_Branch_size = tree.getSize(tree.findNode("node_1"));
    
    Serial.println(root_Branch_size);
    Serial.println(node_1_Branch_size);
}
```
```c++
ROOT
|--node_1
|  |--node_1_1
|     |--node_1_1_1
|--node_2
   |--node_2_1
```
```c++
Output:

6	// root_Branch_size;
3	// node_1_Branch_size;
```
---
# `empty`

判断树是否为空. <br/>*Determine if the tree is empty*


```c++
 bool empty();
```
## 返回值 - Return value

如果树为空，返回 `true`, 否则返回 `false`.<br>*If the tree is empty, return `true`, otherwise return `false`.*

## 注解 - Remarks

该函数的作用是判断树是否为空。如果根节点指针为空，则返回 `true`，否则返回 `false`。<br>*This function is used to determine if the tree is empty. If the root pointer is empty, it returns `true`, otherwise it returns `false`.*

## 示例 - Example

```c++
#include <tree.hpp>
#include <string>
#include <Arduino.h>

void setup() {
	Tree<std::string> tree("ROOT");
    
    tree.root->addChild("node_1")->addChild("node_1_1");
    
    // 判断树是否为空;
    // Determine if the tree is empty.
    bool is_empty_1 = tree.empty();
    
    // 删除整颗树;
    // Delete the whole tree.
    tree.deleteTree();
    
    // 判断树是否为空;
    // Determine if the tree is empty.
    bool is_empty_2 = tree.empty();
    
    Serial.println(is_empty_1);
    Serial.println(is_empty_2);
}
```
```c++
ROOT
|--node_1
|  |--node_1_1
```
```c++
Output:

false
true
```
---
# `deleteNode`

递归删除一个节点及其后裔. <br/>*Recursively delete a node and its descendants.*


```c++
bool deleteNode(TreeNode<T>* node_ptr);
```
## 参数 - Parameters

`node_ptr`

提供一个指向待删除节点的指针.<br>*Provide a pointer to the node to be deleted.*

## 返回值 - Return value

删除成功返回`true`，否则返回`false`.<br>*Returns `true` for successful deletion, otherwise returns `false`.*

## 注解 - Remarks

这段代码的作用是删除给定节点及其所有子节点。首先检查节点指针是否为空，如果为空则直接返回。然后遍历该节点的所有子节点，递归调用 `deleteNode` 函数来删除子节点。如果子节点是树叶节点，则将其从父节点的子节点列表中移除。最后检查当前节点是否为根节点，如果是，则返回 `false`，因为无法删除根节点。如果当前节点不是根节点，则将其从父节点的子节点列表中移除，并返回 `true`。<br>*The purpose of this code is to delete a given node and all its child nodes. It first checks if the node pointer is null, and if so, returns directly. Then it traverses all child nodes of the node and recursively calls the deleteNode function to delete them. If a child node is a leaf node, it is removed from the parent node's list of child nodes. Finally, it checks if the current node is the root node, and if so, returns false because it is not possible to delete the root node. If the current node is not the root node, it is removed from the parent node's list of child nodes, and true is returned.*

## 示例 - Example

```c++
#include <tree.hpp>
#include <string>
#include <Arduino.h>

void setup() {
	Serial.begin(115200);

    Tree<std::string> tree("ROOT");

    tree.root->addChild("node_1")->addChild("node_1_1")->addChild("node_1_1_1");
    tree.root->addChild("node_2")->addChild("node_2_1");
    
    for (auto& data : tree.traversalDFS()) Serial.println(data.first.c_str());
    
    // 删除 node_2_1 节点及其所有后裔.
    // Delete node_2_1 and all its descendants.
    bool delete_result_1 = tree.deleteNode(tree.findNode("node_2_1"));
    Serial.println(delete_result_1);
    
    for (auto& data : tree.traversalDFS()) Serial.println(data.first.c_str());
    
    // 删除 node_1 节点及其所有后裔.
    // Delete node_1 and all its descendants.
    bool delete_result_2 = tree.deleteNode(tree.findNode("node_1"));
    Serial.println(delete_result_2);
    
    for (auto& data : tree.traversalDFS()) Serial.println(data.first.c_str());
}
```
```c++
ROOT
|--node_1
|  |--node_1_1
|     |--node_1_1_1
|--node_2
   |--node_2_1
```
```c++
Output:

ROOT
node_1
node_1_1
node_1_1_1
node_2
node_2_1
true	// Delete node_2_1;
ROOT
node_1
node_1_1
node_1_1_1
node_2
true	// Delete node_1;
ROOT
node_2
```
---
# `deleteTree`

删除一颗树. <br/>*Delete a tree.*


```c++
void deleteTree();
```
## 注解 - Remarks

这段代码的作用是删除整棵树。它首先使用`deleteNode`函数删除根节点的所有子节点，然后使用`reset`函数将根节点指针重置为`nullptr`。<br>*The purpose of this code is to delete the entire tree. It first uses the `deleteNode` function to remove all child nodes of the root node, and then uses the `reset` function to reset the root node pointer to `nullptr`.*

## 示例 - Example

```c++
#include <tree.hpp>
#include <string>
#include <Arduino.h>

void setup() {
	Tree<std::string> tree("ROOT");
    
    tree.root->addChild("node_1")->addChild("node_1_1");
    
    // 判断树是否为空;
    // Determine if the tree is empty.
    bool is_empty_1 = tree.empty();
    
    // 删除整颗树;
    // Delete the whole tree.
    tree.deleteTree();
    
    // 判断树是否为空;
    // Determine if the tree is empty.
    bool is_empty_2 = tree.empty();
    
    Serial.println(is_empty_1);
    Serial.println(is_empty_2);
}
```
```c++
ROOT
|--node_1
|  |--node_1_1
```
```c++
Output:

false
true
```
---
# 结尾 - End

> 2023.03.25
>
> Powered by RMSHE. Copyrght(C) 2023 RMSHE. All rights reserved.