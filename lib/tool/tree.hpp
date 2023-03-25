/**
 * @file tree.hpp
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
这段代码实现了一个通用的树数据结构，包括节点的添加、删除、查找、遍历等基本操作。
该树数据结构由两个类组成：TreeNode和Tree。TreeNode表示一个树节点，包含节点数据、子节点列表、父节点等属性；Tree表示整个树结构，
包含根节点以及树的遍历、节点查找、节点删除等操作。
具体来说，TreeNode类包含了添加子节点、查找子节点、删除子节点等方法。其中，添加子节点使用了C++11中的智能指针std::unique_ptr，
确保了子节点的内存管理安全；查找子节点使用了递归的方式，深度优先遍历整个子树；
删除子节点则使用了迭代的方式，遍历整个子树进行删除操作。
Tree类包含了树的遍历、节点查找、节点删除等方法。其中，遍历操作分为深度优先遍历和广度优先遍历两种方式；
节点查找操作同样使用了递归的方式，在根节点开始向下搜索整个子树；节点删除操作则使用了递归的方式，在整个子树中进行删除操作。
总的来说，该树数据结构提供了基本的树操作，能够满足一些基本的需求。但是需要注意的是，该树数据结构没有进行任何的平衡操作，因此对于较大的树可能会存在效率问题。
*/

#pragma once

#include <algorithm>
#include <make_ptr.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

// @note 一个 TreeNode 对象代表了一棵树中的一个节点，其中包含了当前节点的数据和指向它的父节点的指针以及指向其子节点的所有指针。
template <typename T>
class TreeNode {
   public:
    T node_data;                                         // 储存这个节点的值
    std::vector<std::unique_ptr<TreeNode<T>>> children;  // 储存指向这个节点的子节点的指针
    TreeNode<T>* parent;                                 // 储存指向这个节点的父节点的指针

    /**
     * @brief "TreeNode"树节点构造函数: 创建一个新的节点对象，构造节点.
     * @param data const T&类型的参数，表示根节点的数据(data的数据类型可任意).
     * @param parent_node_ptr TreeNode<T>* 类型的参数，表示指向父节点的指针, 默认为nullptr.
     * @note 用法：TreeNode< std::string > node("data", parent_node_ptr);
     */
    TreeNode(const T& data, TreeNode<T>* parent_node_ptr = nullptr) : node_data(data), parent(parent_node_ptr) {}

    /**
     * @brief 向当前节点添加一个子节点
     * @param data const T&类型的参数，表示节点的值.
     * @return TreeNode<T>* 返回一个指向新加子节点的指针
     * @note 当调用 addChild() 函数时，它将创建一个新的 TreeNode 对象，该对象保存传递给函数的数据，并将指向新创建节点的指针添加到当前节点的 children
     * 向量中。也就是说，addChild() 添加的是一个新的子节点。使用示例：parent_node_ptr->addChild(data) / parent_node_ptr->addChild(data0)->addChild(data1);
     */
    TreeNode<T>* addChild(const T& data) {
        // 为类分配内存并创建对象时会自动调用类的构造函数TreeNode(const T& data, TreeNode<T>* parent_node_ptr = nullptr);
        // parent_node_ptr->addChild(data); 在这个语句中 this 即是 parent_node_ptr;
        children.emplace_back(make_unique<TreeNode>(data, this));  // 向父节点添加一个指向子节点的指针；

        return this->findChild(data);  // 返回一个指向刚刚添加的子节点的指针
    }

    /**
     * @brief 在当前节点的子节点中查找指定数据的节点
     * @param target_child_data const T&类型的参数，表示要查找的节点数据(值).
     * @return TreeNode<T>* 指向查找到的节点的指针，如果未找到返回 nullptr.
     * @note 使用示例：parent_node_ptr->findChild(target_child_data);
     */
    TreeNode<T>* findChild(const T& target_child_data) {
        // 遍历当前节点的每一个子节点
        for (auto& child : children) {
            if (child->node_data == target_child_data) {
                // 如果当前子节点的数据等于要查找的数据，则返回该子节点的指针。
                return child.get();
            }
        }
        return nullptr;  // 如果遍历完所有子节点都没有找到，则返回 nullptr 表示没有找到该节点。
    }

    /**
     * @brief 在当前节点的后裔中查找一个指定数据值的节点.
     * @param target_node_data const T&类型的参数，表示要查找的节点数据(值).
     * @return TreeNode<T>* 指向查找到的节点的指针，如果未找到返回 nullptr.
     * @note 使用示例：parent_node_ptr->findDescendant(target_node_data);
     */
    TreeNode<T>* findDescendant(const T& target_node_data) {
        // 遍历当前节点的每一个子节点
        for (auto& child : children) {
            if (child->node_data == target_node_data) {
                // 如果当前子节点的数据等于要查找的数据，则返回该子节点的指针。
                return child.get();
            } else {
                // 否则，递归地调用子节点的 findDescendant
                // 方法来查找是否存在指定数据的节点，如果找到，则返回该子节点的指针。这是一个深度优先的递归遍历方式。
                TreeNode<T>* found = child->findDescendant(target_node_data);
                if (found != nullptr) return found;
            }
        }
        return nullptr;  // 如果遍历完所有子节点都没有找到，则返回 nullptr 表示没有找到该节点。
    }

    /**
     * @brief 判断当前节点的一个子节点是否有孩子.
     * @param node_ptr TreeNode<T>* 待判断节点的指针
     * @return 如果存在子节点返回true, 否则返回false
     * @note 使用方法: node_ptr->hasChildren();
     */
    bool hasChildren() const {
        return !this->children.empty();  // 如果 node_ptr->children 不为空，则表示这个节点有子节点。
    }

    /**
     * @brief 删除父节点的一个无孩子子节点(删除树叶节点)
     * @param target_child_data const T& 这里需要提供待删除的目标子节点的值
     * @return 删除成功返回true，否则返回false
     * @note 注意这个函数只支持删除没有子节点的节点，即树枝的末端(树叶)。
     * 若要删除整颗树或部分树枝请使用"deleteNode();"函数
     * 使用示例：parent_node_ptr->deleteChild(target_child_data);
     */
    bool deleteChild(const T& target_child_data) {
        TreeNode<T>* child_node_ptr = findChild(target_child_data);  // 从当前父节点查找要删除的子节点的指针

        // 判断这个子节点是否也存在子节点，这里只支持删除没有子节点的节点(树叶), 如果存在子节点或找不到要删除的子节点，返回 false
        if (child_node_ptr == nullptr || hasChildren(child_node_ptr) == true) return false;

        // 遍历当前父节点的所有子节点，在父节点中删除要删除的节点
        for (auto it = children.begin(); it != children.end(); ++it) {
            // 从迭代器获取子节点的指针，如果该指针是要删除的目标子节点则删除它，
            // 由于使用了 std::unique_ptr来管理子节点，所以父节点可以在内存管理方面自动处理子节点的内存释放，不需要手动释放。
            if (it->get() == child_node_ptr) {
                children.erase(it);  // 移除的目标子节点
                break;
            }
        }

        return true;
    }
};

template <typename T>
class Tree {
   public:
    std::unique_ptr<TreeNode<T>> root;           // 储存树的根节点
    TreeNode<T>* current_node_ptr = root.get();  // 储存最后一次添加节点后的指针位置(初始化时设为根节点指针)

    /**
     * @brief "Tree"树构造函数: 创建一个新的Tree对象，构造根节点.
     * @param data const T&类型的参数，表示根节点的数据.
     * @return void
     * @note 用法：Tree< std::string > tree0("root");
     */
    Tree(const T& data) : root(make_unique<TreeNode<T>>(data)) {}

    /**
     * @brief 向当前节点添加一个子节点
     * @param node_ptr TreeNode<T>*类型的参数(指向节点的指针)，表示在该节点下添加子节点.
     * @param data const T&类型的参数，表示要添加的节点的数据.
     * @return TreeNode<T>* 返回一个指向刚刚添加的子节点的指针
     * @note 当调用 `addNode()` 函数时，它将创建一个新的 `TreeNode`
     * 对象，该对象保存传递给函数的数据，并将指向新创建节点的指针添加到当前节点的`children`向量中。也就是说，`addNode()` 添加的是一个新的子节点。`addNode`
     * 与 `addChild` 不同, `addNode` 是 `Tree class` 的成员, 而 `addChild` 是`TreeNode class` 的成员, `addNode` 将父节点指针作为参数传递.
     * 该函数还会将指向新增节点的指针保存到类成员变量 `current_node_ptr`中,以便用户更清楚当前树的编辑位置.
     */
    TreeNode<T>* addNode(TreeNode<T>* node_ptr, const T& data) { return current_node_ptr = node_ptr->addChild(data); }

    /**
     * @brief 以深度优先的方式遍历树
     * @param node_ptr TreeNode<T>* 提供一个节点指针，函数会以该节点为根节点递归遍历它所有的子嗣节点(若不传参则默认遍历整颗树).
     * @return 返回一个向量, 其中包含从指定节点开始子树的所有节点数据值和对应的指针 std::vector<std::pair<T, TreeNode<T>*>>
     * @note 深度优先遍历算法是递归的，它首先访问根节点，然后再递归地遍历每个子树。在每个节点访问完成后，递归函数回溯到其父节点继续遍历其他子树
     */
    std::vector<std::pair<T, TreeNode<T>*>> traversalDFS(TreeNode<T>* node_ptr = nullptr) {
        // 默认节点指针设置为根节点
        if (node_ptr == nullptr) node_ptr = root.get();

        // 如果节点为空，直接返回一个空向量
        if (node_ptr == nullptr) return {};

        // 创建一个向量，用于存储当前节点和其子节点的数据
        std::vector<std::pair<T, TreeNode<T>*>> tree_data;

        // 将当前节点的数据插入到 tree_data 向量的末尾
        tree_data.emplace_back(std::pair<T, TreeNode<T>*>(node_ptr->node_data, node_ptr));

        // 遍历当前节点的每个子节点
        for (auto& child : node_ptr->children) {
            // 递归遍历当前子节点的子树，并将其存储在 deep_tree_data 向量中
            auto deep_tree_data = traversalDFS(child.get());

            // 将当前子节点的子树数据插入到 tree_data 向量的末尾
            tree_data.insert(tree_data.end(), deep_tree_data.begin(), deep_tree_data.end());
        }

        // 返回包含当前节点及其所有子节点的数据的向量
        return tree_data;
    }

    /**
     * @brief 以广度优先的方式遍历树。
     * @param node_ptr TreeNode<T>* 提供一个节点指针，函数会以该节点为根节点递归遍历所有的子节点(若不传参则默认遍历整颗树).
     * @return 返回一个向量, 其中包含从指定节点开始子树的所有节点数据值和对应的指针 std::vector<std::pair<T, TreeNode<T>*>>
     * @note 广度优先遍历算法是按层遍历，从根节点开始，先遍历根节点，然后按照从左到右的顺序遍历其子节点，再依次遍历下一层的所有节点。
     */
    std::vector<std::pair<T, TreeNode<T>*>> traversalBFS(TreeNode<T>* node_ptr = nullptr) {
        // 默认节点指针设置为根节点
        if (node_ptr == nullptr) node_ptr = root.get();

        // 当节点为空时返回空向量
        if (node_ptr == nullptr) return {};

        // 创建一个向量，用于存储当前节点和其子节点的数据
        std::vector<std::pair<T, TreeNode<T>*>> tree_data;

        // 定义队列，并将根节点推入队列
        std::queue<TreeNode<T>*> node_queue;
        node_queue.push(node_ptr);

        // 遍历队列，直到队列为空
        while (!node_queue.empty()) {
            // 取出队列头部的节点后将其弹出
            auto current_node = node_queue.front();
            node_queue.pop();

            // 将当前节点的数据值和指针插入到 tree_data 向量的末尾
            tree_data.emplace_back(std::pair<T, TreeNode<T>*>(current_node->node_data, current_node));

            // 将当前节点的所有子节点推入队列
            for (auto& child : current_node->children) node_queue.push(child.get());
        }

        // 返回包含当前节点及其所有子节点的数据的向量
        return tree_data;
    }

    /**
     * @brief 在树中查找指定数据的节点
     * @param target_node_data const T&类型的参数，表示要查找的节点数据(值).
     * @return TreeNode<T>* 指向查找到的节点的指针，如果未找到返回 nullptr.
     * @note 使用示例：tree.findNode(target_node_data);
     */
    TreeNode<T>* findNode(const T& target_node_data) {
        // 在根节点下查找目标节点(查找范围不包含根节点).
        TreeNode<T>* node_ptr = root->findDescendant(target_node_data);

        /*
        如果上一步的查找结果 node_ptr 为空指针，则：
            如果根节点的数据等于要查找目标数据，则我们可断定用户查找的是根节点直接(返回根节点的指针)，否则即在包含根节点的整颗树的范围内找不到目标节点(返回空指针)
        否则：
            在根节点下查找到目标节点, 直接返回查找到的节点的指针.
        */
        return node_ptr == nullptr ? (root->node_data == target_node_data ? root.get() : nullptr) : node_ptr;
    }

    /**
     * @brief 判断指定节点是否存在子节点
     * @param node_ptr TreeNode<T>* 待判断节点的指针
     * @return 如果存在子节点返回true, 否则返回false
     */
    bool hasChildren(TreeNode<T>* node_ptr) const {
        return !node_ptr->children.empty();  // 如果 node_ptr->children 不为空，则表示这个节点有子节点。
    }

    /**
     * @brief 递归地计算树的深度(高度)(默认统计整颗树的深度)
     * @param node_ptr TreeNode<T>*类型的参数(指向节点的指针)，表示从该节点开始统计树枝的深度，若设为root则为统计整颗树的深度(这也是无传参时的默认设置)
     * @return uint32_t 返回树的深度.
     * @note 使用示例：1.统计整颗树的深度：tree.getHeight();    2.统计从 node1 节点开始的树枝深度：tree.getHeight(node1_ptr);
     */
    uint32_t getDepth(TreeNode<T>* node_ptr = nullptr) {
        // 默认节点指针设置为根节点
        if (node_ptr == nullptr) node_ptr = root.get();

        // 如果节点没有子节点，说明当前节点是叶子节点，返回 1 作为高度
        if (hasChildren(node_ptr) == false) return 1;

        uint32_t max_depth = 0;  // 定义最大深度（一个树中最少有一个树枝最长）；

        // 如果节点有子节点，则递归计算子节点的高度，并找到其中最大的高度
        for (const auto& child : node_ptr->children) max_depth = std::max(max_depth, getDepth(child.get()));

        // 返回最大高度加上 1，即为整个树的高度
        return max_depth + 1;
    }

    /**
     * @brief 获取节点的度(对于一个给定的节点，其子节点的数量称为度. 一个叶子的度数一定是零)
     * @param node_ptr TreeNode<T>*类型的参数(指向节点的指针)，表示获取该节点的子节点的个数，无传参时默认获取根节点的度.
     * @return uint32_t 返回目标节点的子节点的个数.
     */
    uint32_t getDegree(TreeNode<T>* node_ptr = nullptr) {
        // 默认节点指针设置为根节点
        if (node_ptr == nullptr) node_ptr = root.get();

        // 直接返回目标节点的子节点个数；
        return node_ptr->children.size();
    }

    /**
     * @brief 获取树的度(树的度是指树中一个节点的最大度, 即树中某个拥有最多子节点的父节点的子节点数)
     * @return 返回树的度
     */
    uint32_t get_degree_of_tree() {
        uint32_t max_degree = 0;  // 初始化最大度为0

        // 对树进行深度优先遍历获取所有节点的子节点个数，这里将当前最大值max_degree与遍历到的父节点的子节点个数进行比较后取较大值更新回max_degree中.
        for (auto& node : traversalDFS()) max_degree = std::max(max_degree, node.second->children.size());

        // 遍历完成后返回树中所有结点的度的最大值
        return max_degree;
    }

    /**
     * @brief 获取树或树枝的叶子数量(叶子即没有子节点的节点，也称做终端节点)
     * @param node_ptr TreeNode<T>*类型的参数(指向节点的指针)，表示获取以该节点为起点的树枝的叶子个数，无传参时默认获取整个树的叶子数量.
     * @return 返回树或指定树枝的叶子数量
     */
    uint32_t getBreadth(TreeNode<T>* node_ptr = nullptr) {
        // 默认节点指针设置为根节点
        if (node_ptr == nullptr) node_ptr = root.get();

        uint32_t num_leaves = 0;  // 初始化叶子数为0

        // 对树进行深度优先遍历;
        for (auto& node : traversalDFS(node_ptr))
            // 如果一个节点没有子节点，则增加叶子数.
            if (hasChildren(node.second) == false) ++num_leaves;

        // 返回树的叶子数量
        return num_leaves;
    }

    /**
     * @brief 获取树的宽度或指定节点所在层的宽度(宽度指一个层的节点数)
     * @param node_ptr TreeNode<T>*类型的参数(指向节点的指针)，表示获取该节点所在层的宽度，无传参时默认获取整个树的宽度(拥有最大宽度的层级).
     * @return 无参数时返回树的宽度，有参数时返回参数节点所在层的宽度.
     */
    uint32_t getWidth(TreeNode<T>* node_ptr = nullptr) {
        // 如果没有传递指针，则默认使用根节点指针。
        if (node_ptr == nullptr) node_ptr = root.get();

        uint32_t max_level_width = 0;                         // 用于储存树的宽度(拥有最大宽度的层级)
        std::unordered_map<uint32_t, uint32_t> level_widths;  // 用于存储每个层级的节点数的哈希表

        // 使用广度优先搜索遍历每个节点
        for (auto& node : traversalBFS(node_ptr)) {
            uint32_t level = static_cast<uint32_t>(getLevel(node.second));  // 获取当前节点的层级
            auto iter = level_widths.find(level);                           // 查找当前层级是否已经在哈希表中存在

            if (iter != level_widths.end()) {
                ++iter->second;                                             // 如果存在，就增加该层级的节点数
                max_level_width = std::max(max_level_width, iter->second);  // 更新树的宽度
            } else
                level_widths.insert({level, 1});  // 如果不存在，就将该层级的节点数设置为1
        }

        // 如果传入的指针是根节点指针或空指针，则返回树的宽度；否则返回该节点所在层级的节点数
        return node_ptr == root.get() ? max_level_width : level_widths.find(getLevel(node_ptr))->second;
    }

    /**
     * @brief 获取指定节点的层级(一个节点的层级是它与根节点之间唯一路径上的边的数量, 根节点层级为零)
     * @param node_ptr TreeNode<T>*类型的参数(指向节点的指针)，表示获取该节点的层级.
     * @return 返回指定节点的所在层级数, 如果提供的节点指针为空指针则返回-1
     */
    int32_t getLevel(TreeNode<T>* node_ptr) {
        // 当节点为空时返回错误信息
        if (node_ptr == nullptr) return -1;

        int32_t level = 0;  // 初始化节点层级为0

        // 从提供的节点 node_ptr 开始向根节点查找, 在每一次循环中增加 level 的数量, 直到遍历到根节点(根节点没有父节点)
        while (node_ptr->parent != nullptr) {
            level++;
            node_ptr = node_ptr->parent;
        }

        return level;
    }

    /**
     * @brief 获取树或树枝的大小(节点数)
     * @param node_ptr TreeNode<T>*类型的参数(指向节点的指针)，表示获取以该节点为起点的树枝的节点总数，无传参时默认获取整个树的节点总数.
     * @return 返回树或指定树枝的节点总数
     */
    uint32_t getSize(TreeNode<T>* node_ptr = nullptr) {
        // 默认节点指针设置为根节点
        if (node_ptr == nullptr) node_ptr = root.get();

        // 对树进行深度优先遍历然后返回树中的节点数;
        return traversalDFS(node_ptr).size();
    }

    /**
     * @brief 判断树是否为空
     * @return 如果树为空，则返回 true；如果向量不为空，则返回 false。
     */
    bool empty() { return root == nullptr ? true : false; }

    /**
     * @brief 递归删除一个节点及其后裔
     * @param node_ptr TreeNode<T>* 这里需要提供指向待删除节点的指针
     * @return 删除成功返回true，否则返回false
     * @note 使用示例：tree.deleteNode(node_1);
     */
    bool deleteNode(TreeNode<T>* node_ptr) {
        // 如果节点为空，直接返回
        if (node_ptr == nullptr) return false;

        // 获取该节点的所有子节点，并遍历删除它们
        auto& children = node_ptr->children;
        for (auto it = children.begin(); it != children.end();) {
            auto& child = *it;  // it 是迭代器, *it 是迭代器所指的内容.

            // 这里是在判断一个节点是否有孩子.
            if (hasChildren(child.get()) == true)
                deleteNode(child.get());  // 如果子节点不是树叶节点(有孩子)，则递归调用 deleteNode
            else
                it = children.erase(it);  // 移除树叶节点(如果删除成功，该函数会返回指向被删除元素之后的元素的迭代器)
        }

        // 检查当前节点node_ptr是否为根节点，如果是，则返回 false, 这里无法删除根节点.
        if (node_ptr->parent == nullptr) return false;

        // 在删除完这个节点的孩子后, 删除它自身
        auto parent = node_ptr->parent;  // 获取当前节点的父节点指针

        // 在父节点的子节点列表中查找并移除当前节点
        parent->children.erase(
            // 在容器中查找符合某个条件的元素，并将其移动到容器的末尾
            std::remove_if(parent->children.begin(),  // 查找的起始位置
                           parent->children.end(),    // 查找的终止位置

                           // 查找谓词，用于确定哪些元素符合要求。该函数或函数对象接受一个元素作为参数.
                           // remove_if算法会将child传入这个匿名函数, 如果 child指针与node_ptr相同则返回 true.
                           [node_ptr](std::unique_ptr<TreeNode<T>>& child) { return child.get() == node_ptr; }),
            parent->children.end()  // 移除的终止位置
        );

        return true;
    }

    /**
     * @brief 移除一颗树.
     * @note 使用示例：tree.deleteTree();
     */
    void deleteTree() {
        deleteNode(root.get());  // 删除根节点的所有子嗣节点;
        root.reset();            // 移除根节点(将根节点重置为nullptr);
    }

    // Tree的析构函数
    ~Tree() { deleteTree(); }
};