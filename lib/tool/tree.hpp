#pragma once

#include <make_unique.hpp>
#include <memory>
#include <vector>

// @note 一个 TreeNode 对象代表了一棵树中的一个节点，其中包含了当前节点的数据以及其所有指向子节点的指针。
template <typename T>
class TreeNode {
   public:
    T node_data;                                         // 储存这个节点的值
    std::vector<std::unique_ptr<TreeNode<T>>> children;  // 储存指向这个节点的子节点的指针

    /**
     * @brief "TreeNode"树节点构造函数: 创建一个新的节点对象，构造节点.
     * @param data const T&类型的参数，表示根节点的数据(data的数据类型可任意).
     * @return void
     * @note 用法：TreeNode< std::string > node("data");
     */
    TreeNode(const T& data) : node_data(data) {}

    /**
     * @brief 向当前节点添加一个子节点
     * @param data const T&类型的参数，表示节点的数据.
     * @return void
     * @note 当调用 addChild() 函数时，它将创建一个新的 TreeNode 对象，该对象保存传递给函数的数据，并将指向新创建节点的指针添加到当前节点的 children
     * 向量中。也就是说，addChild() 添加的是一个新的子节点。使用示例：parent_node_ptr->addChild(data);
     */
    void addChild(const T& data) {
        children.push_back(make_unique<TreeNode>(data));  // 向父节点添加一个指向刚才构造的子节点的指针；
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
            } else {
                // 否则，递归地调用子节点的 findChild 方法来查找是否存在指定数据的节点，如果找到，则返回该子节点的指针。这是一个深度优先的递归遍历方式。
                TreeNode<T>* found = child->findChild(target_child_data);
                if (found != nullptr) return found;
            }
        }
        return nullptr;  // 如果遍历完所有子节点都没有找到，则返回 nullptr 表示没有找到该节点。
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
     * @brief 删除父节点的一个无孩子子节点
     * @param target_child_data const T& 这里需要提供待删除的目标子节点的值
     * @return 删除成功返回true，否则返回false
     * @note 注意这个函数只支持删除没有子节点的节点，即树枝的末端。可以配合其他函数使用(例如："findChild();").
     * 若要删除整颗树或部分树枝请使用"deleteTree();"函数. 使用示例：parent_node_ptr->deleteChild(target_child_data);
     */
    bool deleteChild(const T& target_child_data) {
        auto child_node_ptr = findChild(target_child_data);  // 从当前父节点查找要删除的子节点的指针
        if (child_node_ptr == nullptr) return false;         // 如果找不到要删除的子节点，返回 false

        if (hasChildren(child_node_ptr) == true) return false;  // 判断这个子节点是否也存在子节点，这里只支持删除没有子节点的节点

        // 遍历当前父节点的所有子节点，在父节点中删除要删除的节点
        for (auto it = children.begin(); it != children.end(); ++it) {
            // 从迭代器获取子节点的指针，如果该指针是要删除的目标子节点则删除它，
            // 由于使用了 std::unique_ptr来管理子节点，所以父节点可以在内存管理方面自动处理子节点的内存释放，不需要手动释放。
            if (it->get() == child_node_ptr) {
                children.erase(it);
                break;
            }
        }

        return true;
    }
};

template <typename T>
class Tree {
   public:
    std::unique_ptr<TreeNode<T>> root;  // 储存树的根节点

    /**
     * @brief "Tree"树构造函数: 创建一个新的Tree对象，构造根节点.
     * @param data const T&类型的参数，表示根节点的数据.
     * @return void
     * @note 用法：Tree< std::string > tree0("root");
     */
    Tree(const T& data) : root(make_unique<TreeNode<T>>(data)) {}

    /**
     * @brief 以深度优先的方式遍历树
     * @param node_ptr TreeNode<T>* 提供一个节点指针，函数会以该节点为根节点递归遍历所有的子节点(若想遍历整个树提供根节点即可: tree.root.get()).
     * @return 返回一个向量, 其中包含从指定节点开始子树的所有节点数据
     * @note 深度优先遍历算法是递归的，它首先访问根节点，然后再递归地遍历每个子树。在每个节点访问完成后，递归函数回溯到其父节点继续遍历其他子树
     */
    std::vector<T> traversalDFS(TreeNode<T>* node_ptr) {
        // 如果节点为空，直接返回一个空向量
        if (node_ptr == nullptr) return {};

        // 创建一个向量，用于存储当前节点和其子节点的数据
        std::vector<T> tree_data;

        // 将当前节点的数据插入到 tree_data 向量的末尾
        tree_data.emplace_back(node_ptr->node_data);

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
     * @brief 以广度优先搜索的方式遍历树。
     * @param node_ptr TreeNode<T>* 提供一个节点指针，函数会以该节点为根节点递归遍历所有的子节点(若想遍历整个树提供根节点即可: tree.root.get()).
     * @return 返回一个向量, 其中包含从指定节点开始子树的所有节点数据
     * @note 广度优先遍历算法是按层遍历，从根节点开始，先遍历根节点，然后按照从左到右的顺序遍历其子节点，再依次遍历下一层的所有节点。
     */
    std::vector<T> traversalBFS(TreeNode<T>* node_ptr) {
        // 当节点为空时返回空向量
        if (node_ptr == nullptr) return {};

        // 创建一个向量，用于存储当前节点和其子节点的数据
        std::vector<T> tree_data;

        // 定义队列，并将根节点推入队列
        std::queue<TreeNode<T>*> node_queue;
        node_queue.push(node_ptr);

        // 遍历队列，直到队列为空
        while (!node_queue.empty()) {
            // 取出队列头部的节点后将其弹出
            auto current_node = node_queue.front();
            node_queue.pop();

            // 将当前节点的数据插入到 tree_data 向量的末尾
            tree_data.emplace_back(current_node->node_data);

            // 将当前节点的所有子节点推入队列
            for (auto& child : current_node->children) node_queue.push(child.get());
        }

        // 返回包含当前节点及其所有子节点的数据的向量
        return tree_data;
    }
};

/*
bool deleteChild(TreeNode<T>* parent_node_ptr, const T& target_child_data) {
        auto child_node_ptr = parent_node_ptr->findChild(target_child_data);  // 从父节点查找要删除的子节点的指针
        if (child_node_ptr == nullptr) return false;                          // 如果找不到要删除的子节点，返回 false

        if (hasChildren(child_node_ptr) == true) return false;  // 判断这个子节点是否也存在子节点，这里只支持删除没有子节点的节点

        // 遍历父节点的所有子节点，在父节点中删除要删除的节点
        for (auto& it : parent_node_ptr->children) {
            // 从迭代器获取子节点的指针，如果该指针是要删除的目标子节点则删除它，
            // 由于使用了 std::unique_ptr来管理子节点，所以父节点可以在内存管理方面自动处理子节点的内存释放，不需要手动释放。
            if (*it->get() == child_node_ptr) {
                parent_node_ptr->children.erase(*it);
                break;
            }
        }

        return true;
    }
*/