# `TreeTool` 类

## 简介

`TreeTool` 是通用树数据结构容器 `tree.hpp` 的补丁工具类，提供了递归获取树结构的功能。其主要作用是帮助用户以树形结构字符串的形式输出树，便于调试和查看树的层级关系。

---

## 类方法

###  ① 获取树结构字符串（从指定节点）

```c++
std::string getTreeString(TreeNode<T>* node, const std::string& prefix = "", bool isLast = false)
```

从指定节点开始，递归获取树的结构字符串。

**参数：**
- `node`：当前树节点的指针，从此节点开始生成树的结构字符串。
- `prefix`：当前节点的缩进前缀，默认为空字符串。
- `isLast`：表示当前节点是否为最后一个子节点，默认为 `false`。

**返回值：**
- 返回从指定节点开始的树结构字符串。

### ② 获取整棵树的结构字符串

```c++
std::string getTreeString(const Tree<T>& tree)
```

通过树的根节点，调用 `getTreeString` 方法递归生成整颗树的结构字符串。

**参数：**

- `tree`：树对象，包含树的根节点。

**返回值：**

- 返回整个树的结构字符串。

### ③ 获取节点完整路径

```c++
std::string getPath(TreeNode<T>* node)
```

获取指定节点的完整路径。

**参数：**

- `node`：指向目标节点的指针。

**返回值：**

- 返回目标节点的完整路径字符串。

---

## 使用示例

假设你已经有一个树结构：

```cpp
Tree<std::string> tree("root");
```

这棵树的结构如下：

```cpp
ROOT
|--node_1
|  |--node_1_1
|     |--node_1_1_1
|--node_2
   |--node_2_1
```

### 示例 1：获取整颗树的结构字符串

你可以使用 `TreeTool` 获取整颗树的结构字符串：

```cpp
#include <tree.hpp>
#include <tree_tool.hpp>

TreeTool tree_tool;

void setup() {
    Serial.begin(115200);
    delay(4000);
    
    // 创建树并添加节点
    Tree<std::string> tree("ROOT");
    tree.root->addChild("node_1")->addChild("node_1_1")->addChild("node_1_1_1");
    tree.root->addChild("node_2")->addChild("node_2_1");

    // 获取树的结构字符串
    std::string tree_str = tree_tool.getTreeString(tree);
    Serial.println(tree_str.c_str());
}
```

**输出结果：**

```cpp
├─ROOT
│ ├─node_1
│ │ └─node_1_1
│ │   └─node_1_1_1
│ └─node_2
│   └─node_2_1
```

### 示例 2：获取 `node_1` 节点下的树的结构字符串

如果你只想获取某个子树的结构，比如 `node_1` 节点下的树，可以按以下方式操作：

```cpp
#include <tree.hpp>
#include <tree_tool.hpp>

TreeTool tree_tool;

void setup() {
    Serial.begin(115200);
    delay(4000);
    
    // 创建树并添加节点
    Tree<std::string> tree("ROOT");
    tree.root->addChild("node_1")->addChild("node_1_1")->addChild("node_1_1_1");
    tree.root->addChild("node_2")->addChild("node_2_1");

    // 获取 `node_1` 节点下的树的结构字符串
    std::string tree_str = tree_tool.getTreeString(tree.findNode("node_1"));
    Serial.println(tree_str.c_str());
}
```

**输出结果：**

```cpp
├─node_1
│ └─node_1_1
│   └─node_1_1_1
```

### 示例 3：获取节点完整路径

你可以使用 `TreeTool` 获取整颗树的结构字符串：

```cpp
#include <tree.hpp>
#include <tree_tool.hpp>

TreeTool tree_tool;

void setup() {
    Serial.begin(115200);
    delay(4000);

    // 创建树并添加节点
    Tree<std::string> tree("ROOT");
    tree.root->addChild("node_1")->addChild("node_1_1")->addChild("node_1_1_1");
    tree.root->addChild("node_2")->addChild("node_2_1");

    // 获取指向节点 "node_1_1_1" 的指针
    std::vector<TreeNode<std::string>*> found_target_nodes = tree.findNode("node_1_1_1");
    TreeNode<std::string>* node_ptr = found_target_nodes[0];

    // 获取节点 "node_1_1_1" 的路径
    std::string found_path = tree_tool.getPath(node_ptr);

    Serial.println(found_path.c_str());
}
```

**输出结果：**

```cpp
ROOT/node_1/node_1_1/node_1_1_1
```

---

## 注意事项

- `TreeTool` 通过递归方式生成树的结构字符串，每个节点会根据其层级使用不同的连接符（`├─`、`└─`、`│`）来表示其在树中的位置。
- 如果传入的节点为空指针（`nullptr`），则输出为空字符串。
- 使用 `getTreeString` 方法时，可以灵活地获取整棵树或任意子树的结构字符串。

---