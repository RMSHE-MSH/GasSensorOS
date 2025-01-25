/**
 * @file tree_tool.hpp
 * @date 16.01.2025
 * @author RMSHE
 *
 * < GasSensorOS >
 * Copyright(C) 2025 RMSHE. All rights reserved.
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
#pragma once

/**
 * @brief 通用树数据结构容器的补丁工具类
 */
class TreeTool {
   public:
    /**
     * @brief 递归获取树的结构字符串，按缩进和连接符打印树结构
     *
     * 该函数递归地遍历树的每个节点，使用不同的连接符（如`├─`、`│  ├─`）表示层级关系，并返回整个树的结构字符串。
     *
     * @tparam T 节点的数据类型
     * @param node 当前树节点指针
     * @param prefix 当前节点的缩进前缀，默认空
     * @param isLast 是否为当前节点的最后一个子节点，默认false
     * @return std::string 返回树的结构字符串
     */
    template <typename T>
    std::string getTreeString(TreeNode<T>* node, const std::string& prefix = "", bool isLast = false) {
        if (node == nullptr) {
            return "";  ///< 如果节点为空，返回空字符串
        }

        std::string result;  // 用于存储构建好的树结构字符串

        // 拼接当前节点的数据
        result += prefix + (isLast ? "└─" : "├─") + node->node_data + "\n";

        // 递归遍历子节点，拼接树结构字符串
        size_t numChildren = node->children.size();  // 获取子节点数量，避免在循环中重复计算
        for (size_t i = 0; i < numChildren; ++i) {
            // 判断是否为最后一个子节点，决定连接符的格式
            std::string newPrefix = prefix + (isLast ? "  " : "│ ");
            result += getTreeString(node->children[i].get(), newPrefix, i == numChildren - 1);  ///< 递归调用，拼接子节点的树结构字符串
        }

        return result;  // 返回构建好的树结构字符串
    }

    /**
     * @brief 从树的根节点获取整个树的结构字符串
     *
     * 该函数从树的根节点开始调用递归函数，构建并返回整个树的结构字符串。
     *
     * @tparam T 节点的数据类型
     * @param tree 树的对象
     * @return std::string 返回整个树的结构字符串
     */
    template <typename T>
    std::string getTreeString(const Tree<T>& tree) {
        return getTreeString(tree.root.get());  ///< 从树的根节点开始生成树结构字符串
    }

    /**
     * @brief 获取指定节点的完整路径
     *
     * @tparam T 节点数据类型
     * @param node 指向目标节点的指针
     * @return std::string 完整路径字符串
     *
     * @note 此函数从目标节点开始，逐级向上追溯父节点，
     *       拼接各节点数据构建完整路径，直到根节点为止。
     */
    template <typename T>
    std::string getPath(TreeNode<T>* node) {
        std::string found_path = node->node_data;  // 初始化路径

        // 从目标节点向上遍历到根节点，构建完整路径
        while (node->parent != nullptr) {
            node = node->parent;                              // 移动到父节点
            found_path = node->node_data + "/" + found_path;  // 拼接当前节点和路径
        }

        // 返回找到的完整路径
        return found_path;
    }
};
