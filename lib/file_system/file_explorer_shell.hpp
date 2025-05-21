/**
 * @file file_explorer_shell.hpp
 * @date 21.05.2025
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
#include <file_explorer.h>
#include <ring_buffer.h>

#include <serial_warning.hpp>
#include <sstream>
#include <string_edit.hpp>

class FileExplorerShell {
   public:
    /**
     * @brief 切换当前工作目录
     *
     * 实现如下功能：
     * - "cd /xxx/xx/x"：切换到指定的绝对路径目录
     * - "cd xx"：切换到当前工作目录下的子目录（相对路径）
     * - "cd <"：返回到上一次访问的目录（后退）
     * - "cd >"：前进至被撤销的目录
     * - "cd ../" 或 "cd .."：返回上一层目录
     *
     * @param flags 命令标志位（未使用）
     * @param parameters 命令参数，cd 仅使用第一个参数作为目标路径
     */
    void cd(const std::vector<std::string>& flags, const std::vector<std::string>& parameters) {
        // === 参数检查与初始化 ===

        // 若参数为空，直接返回，不进行目录切换
        if (parameters.empty()) return;

        // 目标路径取参数中的第一个字符串（cd 只有一个有效参数）
        const std::string& target = parameters[0];

        // 若目标路径与当前路径相同，直接返回，避免重复切换
        if (target == workingDir_) return;

        // === 特殊路径处理（前进、后退、上一级目录） ===
        if (handleSpecial(target)) return;

        // === 构造完整路径 ===
        std::string fullPath = buildFullPath(target);

        // === 路径有效性检查 ===

        // 若目标路径不存在于文件系统中，直接返回，不更新工作目录
        if (!file_.exists(fullPath)) return;

        // === 更新目录历史记录 ===

        // 将当前工作目录压入“后方”缓冲队列中，方便后续使用 "cd <"
        backQueue_.pushBack(workingDir_);

        // 清空“前方”缓冲队列，因为发生了新的跳转路径
        forwardQueue_.clear();

        // === 切换工作目录 ===

        // 更新工作目录
        workingDir_ = std::move(fullPath);
    }

    /**
     * @brief 打印当前工作目录
     *
     * @param flags 命令标志位（未使用）
     * @param parameters 命令参数（未使用）
     */
    void pwd(const std::vector<std::string>& flags, const std::vector<std::string>& parameters) { Serial.println(workingDir_.c_str()); }

    /**
     * @brief 查看目录内容
     *
     * 此命令支持三种路径模式：
     * - "ls"：查看当前工作目录下的内容；
     * - "ls xx"：查看当前目录下名为 xx 的子目录；
     * - "ls /xxx/xx"：查看指定的绝对路径。
     *
     * @param flags 命令标志位参数（本命令不使用，保留接口一致性）
     * @param parameters 命令参数，包含路径信息。
     */
    void ls(const std::vector<std::string>& flags, const std::vector<std::string>& parameters) {
        // 目标路径，默认设置为空字符串
        std::string fullPath;

        // 1. 处理路径构建逻辑
        // 如果没有参数：默认使用当前工作目录
        if (parameters.empty() || parameters[0].empty()) {
            fullPath = workingDir_;
        }
        // 如果有参数：则构造完整路径
        else {
            fullPath = buildFullPath(parameters[0]);
        }

        // 2. 路径合法性校验
        // 如果路径不存在则返回，避免无效访问
        if (!file_.exists(fullPath)) return;

        // 3. 显示目录内容
        // 打印目标目录内容到串口
        file_.printDir(fullPath);
    }

    /**
     * @brief 以树状图列出目录的内容
     *
     * 此命令支持三种路径模式：
     * - "tree"：查看当前工作目录下的内容；
     * - "tree xx"：查看当前目录下名为 xx 的子目录；
     * - "tree /xxx/xx"：查看指定的绝对路径。
     *
     * @param flags 命令标志位参数（本命令不使用，保留接口一致性）
     * @param parameters 命令参数，包含路径信息。
     */
    void tree(const std::vector<std::string>& flags, const std::vector<std::string>& parameters) {
        // 目标路径，默认设置为空字符串
        std::string fullPath;

        // 1. 处理路径构建逻辑
        // 如果没有参数：默认使用当前工作目录
        if (parameters.empty() || parameters[0].empty()) {
            fullPath = workingDir_;
        }
        // 如果有参数：则构造完整路径
        else {
            fullPath = buildFullPath(parameters[0]);
        }

        // 2. 路径合法性校验
        // 如果路径不存在则返回，避免无效访问
        if (!file_.exists(fullPath)) return;

        // 3. 显示目录内容
        // 打印目标目录内容到串口
        file_.printTree(fullPath);
    }

    /**
     * @brief 创建目录
     *
     * - "mkdir xx"：在当前工作目录下创建一个名为 xx 的目录
     * - "mkdir xx1 xx2 xx3"：在当前工作目录下创建多个目录
     * - "mkdir /xxx/xx"：在指定路径下创建一个名为 xx 的目录
     * - "mkdir /xxx1/xx1 /xxx1/xx2 /xxx2/xx3"：在指定路径下创建多个目录
     *
     * @param flags 命令标志位参数（本命令不使用，保留接口一致性）
     * @param parameters 命令参数，包含路径信息。
     */
    void mkdir(const std::vector<std::string>& flags, const std::vector<std::string>& parameters) {
        // 如果没有参数,则直接返回
        if (parameters.empty()) return;

        // 遍历所有路径参数
        for (const auto& target : parameters) {
            // 则构造完整路径
            const std::string fullPath = buildFullPath(target);

            // 创建空目录
            file_.createDir(fullPath);
        }
    }

    /**
     * @brief 删除目录或文件
     *
     * - "rm xx"：删除当前工作目录下名为 xx 的目录或文件
     * - "rm xx1 xx2 xx3"：删除当前工作目录下的多个目录或文件
     * - "rm *"：删除当前工作目录下的所有目录和文件
     * - "rm /xxx/xx"：删除指定路径(xxx)下名为 xx 的目录或文件
     * - "rm /xxx1/xx1 /xxx1/xx2 /xxx2/xx3"：删除指定路径下的多个目录或文件
     * - "rm /xxx/*"：删除指定路径(xxx)下的所有目录和文件
     *
     * @param flags 命令标志位参数（本命令不使用，保留接口一致性）
     * @param parameters 命令参数，包含路径信息。
     */
    void rm(const std::vector<std::string>& flags, const std::vector<std::string>& parameters) {
        // 如果用户未提供任何删除参数，直接返回，无需进一步操作
        if (parameters.empty()) return;

        // 遍历所有待删除参数，逐一执行删除逻辑
        for (const auto& param : parameters) {
            // 构造完整路径，支持相对路径与绝对路径统一计算
            const std::string fullPath = buildFullPath(param);  ///< 生成基于当前工作目录的完整路径

            // 判断是否为通配符模式（末尾为 '*'），用于批量删除
            const bool isWildcard = (!param.empty() && param.back() == '*');  ///< 检查参数结尾字符

            if (isWildcard) {
                // 计算通配符所指目录的父路径
                const std::string parentPath = getParentPath(fullPath);  ///< 提取通配符前的目录路径

                // 如果父路径不存在，跳过该参数，避免无效或错误操作
                if (!file_.exists(parentPath)) {  ///< 检查目录是否存在
                    continue;                     ///< 若不存在则忽略，进入下一个参数
                }

                // 删除父目录下所有内容，包括文件与子目录，释放空间
                file_.deletePath(parentPath);  ///< 递归清空目录内容

                // 重建空目录，保持目录结构与后续写入需求
                file_.createDir(parentPath);  ///< 在闪存中重新创建目录
            } else {
                // 对非通配符路径，若目标不存在则跳过，避免误操作
                if (!file_.exists(fullPath)) {  ///< 检查文件或目录是否存在
                    continue;                   ///< 不存在则跳过删除
                }

                // 直接删除文件或目录（若为目录则递归删除）
                file_.deletePath(fullPath);  ///< 删除文件或目录，包含内部内容
            }
        }
    }

    /**
     * @brief 移动或重命名目录和文件
     *
     * 支持的典型用法：
     * @code
     * cp file.txt new_name.txt          // 重命名工作目录下的文件或目录
     * cp file1.txt file2.txt /usr/dir   // 移动工作目录下的多个目录或文件到指定目录
     * cp /dir1/file.txt /dir2 /usr/dir  // 移动多个指定路径下的文件或目录到目标目录
     * @endcode
     *
     * @param flags 命令选项列表，可支持以下标志：
     *              - "-f": 强制覆盖已存在目标
     *              - "-v": 显示详细输出信息
     * @param parameters 路径参数列表，要求至少两个：一个或多个源路径 + 一个目标路径
     */
    void mv(const std::vector<std::string>& flags, const std::vector<std::string>& parameters) {
        // ------- 基本参数检查 -------
        if (parameters.size() < 2) {
            WARN(WarningLevel::ERROR, "参数不足: 至少需要一个源路径和一个目标路径.");
            return;
        }

        // ------- 标志位处理 -------
        const bool forceOverwrite = std::find(flags.begin(), flags.end(), "-f") != flags.end();  ///< 是否启用强制覆盖
        const bool verbose = std::find(flags.begin(), flags.end(), "-v") != flags.end();         ///< 是否启用详细日志

        // ------- 路径提取 -------
        const std::string& targetParam = parameters.back();  ///< 最后一个参数为目标路径
        const size_t sourceCount = parameters.size() - 1;    ///< 源文件或目录数量

        // ------- 构建完整路径 -------
        const std::string targetPath = buildFullPath(targetParam);  ///< 构造目标完整路径
        std::vector<std::string> sourcePaths;                       ///< 存储源路径的绝对路径

        sourcePaths.reserve(sourceCount);
        for (size_t i = 0; i < sourceCount; ++i) sourcePaths.emplace_back(buildFullPath(parameters[i]));

        // ------- 特殊情况：重命名逻辑 -------

        // 判断是否为重命名命令: 仅有一个源 & 源路径为相对路径 & 目标路径为相对路径 & 目标路径不存在;
        const bool isSingleRename =
            (sourceCount == 1) && !isAbsolutePath(parameters[0]) && !isAbsolutePath(targetParam) && (!file_.exists(targetPath) || forceOverwrite);

        if (isSingleRename) {
            // 重命名文件或目录
            file_.renamePath(sourcePaths[0], targetParam);
            if (verbose) std::cout << "Renamed: " << sourcePaths[0] << " -> " << targetParam << "\n";

            return;
        }

        // ------- 移动逻辑 -------

        // ------- 移动前检查目标目录 -------
        const std::string targetDirPath = targetPath;
        if (!file_.exists(targetDirPath) && !fs_.isDirectory(targetDirPath)) {
            WARN(WarningLevel::ERROR, "目标目录不存在或不是目录: ", targetDirPath);
            return;
        }

        // ------- 遍历并移动每个源 -------
        for (const std::string& sourcePath : sourcePaths) {
            // 源路径存在性检查
            if (!file_.exists(sourcePath)) {
                WARN(WarningLevel::WARNING, "源路径不存在，跳过: ", sourcePath);
                continue;
            }

            // 构造目标子路径（防止重名覆盖）
            std::string baseName = getName(sourcePath);                           ///< 提取源的文件名或目录名
            std::string fullTargetPath = buildFullPath(baseName, targetDirPath);  ///< 拼接目录与文件名，生成完整路径

            // 如果目标已存在且未强制覆盖，则自动重命名
            if (file_.exists(fullTargetPath)) {
                if (forceOverwrite) {
                    file_.deletePath(fullTargetPath);  ///< 强制模式下先删除
                } else {
                    fullTargetPath = buildFullPath("(New)" + baseName, targetDirPath);  ///< 自动生成唯一新名称
                    WARN(WarningLevel::WARNING, "命名冲突，已重命名目标: ", fullTargetPath);
                }
            }

            // 执行移动操作
            file_.movePath(sourcePath, fullTargetPath);
            if (verbose) {
                std::cout << "Moved: " << sourcePath << " -> " << fullTargetPath << "\n";
            }
        }
    }

    /**
     * @brief 复制目录和文件
     *
     * 支持的典型用法：
     * @code
     * mv file1.txt file2.txt /usr/dir   // 复制工作目录下的多个目录或文件到指定目录
     * mv /dir1/file.txt /dir2 /usr/dir  // 复制多个指定路径下的文件或目录到目标目录
     * @endcode
     *
     * @param flags 命令选项列表，可支持以下标志：
     *              - "-f": 强制覆盖已存在目标
     *              - "-v": 显示详细输出信息
     * @param parameters 路径参数列表，要求至少两个：一个或多个源路径 + 一个目标路径
     */
    void cp(const std::vector<std::string>& flags, const std::vector<std::string>& parameters) {
        // ------- 基本参数检查 -------
        if (parameters.size() < 2) {
            WARN(WarningLevel::ERROR, "参数不足: 至少需要一个源路径和一个目标目录.");
            return;
        }

        // ------- 标志位处理 -------
        const bool forceOverwrite = std::find(flags.begin(), flags.end(), "-f") != flags.end();
        const bool verbose = std::find(flags.begin(), flags.end(), "-v") != flags.end();

        // ------- 路径提取 -------
        const std::string targetParam = parameters.back();
        const size_t sourceCount = parameters.size() - 1;

        // 构建目标完整路径
        const std::string targetDirPath = buildFullPath(targetParam);

        // 目标必须是已存在的目录
        if (!file_.exists(targetDirPath) || !fs_.isDirectory(targetDirPath)) {
            WARN(WarningLevel::ERROR, "目标目录不存在或不是目录: ", targetDirPath);
            return;
        }

        // 构建每个源的完整路径
        std::vector<std::string> sourcePaths;
        sourcePaths.reserve(sourceCount);
        for (size_t i = 0; i < sourceCount; ++i) {
            sourcePaths.emplace_back(buildFullPath(parameters[i]));
        }

        // ------- 遍历并复制每个源到目标目录 -------
        for (const auto& sourcePath : sourcePaths) {
            if (!file_.exists(sourcePath)) {
                WARN(WarningLevel::WARNING, "源路径不存在，跳过: ", sourcePath);
                continue;
            }

            // 目标子路径 = 目标目录 + 源的基本名称
            std::string baseName = getName(sourcePath);
            std::string targetPath = buildFullPath(baseName, targetDirPath);

            // 冲突处理
            if (file_.exists(targetPath)) {
                if (forceOverwrite) {
                    file_.deletePath(targetPath);
                } else {
                    WARN(WarningLevel::WARNING, "目标已存在，跳过复制: ", targetPath);
                    continue;
                }
            }

            // 执行复制
            file_.copyPath(sourcePath, targetPath);
            if (verbose) {
                std::cout << "Copied: " << sourcePath << " -> " << targetPath << "\n";
            }
        }
    }

    /**
     * @brief 创建空文件
     *
     * @param flags 命令标志位参数（本命令不使用，保留接口一致性）
     * @param parameters 命令参数，包含路径信息。
     */
    void touch(const std::vector<std::string>& flags, const std::vector<std::string>& parameters) {
        // ———— 参数校验 ————
        if (parameters.empty()) {
            WARN(WarningLevel::ERROR, "参数错误：必须指定至少一个文件路径.");
            return;
        }

        // ———— 遍历所有目标路径 ————
        for (const auto& rawPath : parameters) {
            // 1. 构造绝对路径，避免后续每次都重复解析
            const std::string fullFilePath = buildFullPath(rawPath);

            // 创建新空文件
            file_.createFile(fullFilePath);
        }
    }

    /**
     * @brief 将指定文件的全部内容一次性输出到终端
     *
     * @param flags      命令标志列表，本命令不使用任何标志
     * @param parameters 参数列表，唯一元素为要查看的文件路径或文件名称
     */
    void cat(const std::vector<std::string>& flags, const std::vector<std::string>& parameters) {
        // ----- 1. 参数数量校验 -----
        // 本命令仅接受一个参数：文件路径或文件名称
        if (parameters.size() != 1) {
            WARN(WarningLevel::ERROR,
                 "[参数错误] 用法: cat <fileName|fullFilePath>");  // 提示正确用法
            return;                                                // 参数错误，退出
        }

        // ----- 2. 构建完整文件路径 -----
        // 将相对路径或文件名转换为绝对路径，确保定位正确
        const std::string fullPath = buildFullPath(parameters[0]);

        // ----- 3. 检查文件是否存在 -----
        // 避免读取不存在的文件导致错误或异常
        if (!file_.exists(fullPath)) {
            WARN(WarningLevel::ERROR, "[文件不存在] 无法找到文件: ", fullPath);
            return;  // 文件不存在，退出
        }

        // ----- 4. 读取文件内容 -----
        // 一次性将整个文件读入内存
        std::string content = file_.readFileAsString(fullPath);

        // ----- 5. 输出到终端 -----
        // 将读取到的内容直接写入标准输出，不做额外处理
        Serial.print(content.c_str());
    }

    /**
     * @brief 将一行文本写入指定文件
     * @details
     *   支持两种写入模式：
     *     - "-w"：覆盖写入（write），即清空目标文件后写入
     *     - "-a"：追加写入（append），即保留原内容后追加（默认模式）
     *   文本内容由 parameters 列表中除最后一项外的所有元素拼接而成，
     *   最后一项为目标文件路径，可为相对路径或绝对路径。
     *
     * @param flags      写入模式标志列表，若含 "-w" 则覆盖写入，否则追加写入
     * @param parameters 文本片段和文件路径列表，最后一项为文件路径，其余项拼接为写入内容
     */
    void echo(const std::vector<std::string>& flags, const std::vector<std::string>& parameters) {
        // 1. 校验标志和参数

        // 标志校验
        /// flags 至多只能传一个；传多个视为格式错误
        if (flags.size() > 1) {
            WARN(WarningLevel::ERROR, "[标志错误] 只能接受一个标志, 用法: echo [-w|-a] <string> <fileName|fullFilePath>");
            return;
        }

        // 参数校验
        /// parameters 至少应包含两个：一个或多个字符串 + 一个文件路径
        if (parameters.size() < 2) {
            WARN(WarningLevel::ERROR, "[参数错误] 参数数量不足, 用法: echo [-w|-a] <string> <fileName|fullFilePath>");
            return;
        }

        // 2. 解析写入模式：默认追加 ("a")，如遇 "-w" 切换为覆盖 ("w")
        const char* mode = "a";                              // 默认追加写入
        if (!flags.empty() && flags[0] == "-w") mode = "w";  // 覆盖写入

        // 3. 拼接写入内容：parameters[0..n-2] 拼接成完整字符串，末尾加换行
        size_t textCount = parameters.size() - 1;                                          ///< 文本片段数量
        size_t estimatedSize = 1;                                                          ///< 为末尾换行预留 1 字节
        for (size_t i = 0; i < textCount; ++i) estimatedSize += parameters[i].size() + 1;  ///< 统计每段长度及空格

        // 预估总长度：所有片段长度 + 空格数 + 换行符
        std::string content;             ///< 最终写入内容
        content.reserve(estimatedSize);  ///< 预分配，减少堆分配次数

        for (size_t i = 0; i < textCount; ++i) {
            if (i > 0) content.push_back(' ');  ///< 插入空格分隔每段文本
            content.append(parameters[i]);      ///< 追加当前文本片段
        }
        content.push_back('\n');  ///< 写入末尾换行

        // ----- 4. 构建完整文件路径 -----
        const std::string fullPath = buildFullPath(parameters.back());  ///< 将最后一项路径转换为绝对路径

        // ----- 5. 执行写入操作 -----
        bool success = file_.writeFileAsString(fullPath, content, mode);

        // ----- 6. 结果检查 -----
        if (!success) {  ///< 写入失败时发出警告
            WARN(WarningLevel::ERROR, "[写入失败] 无法写入文件: ", fullPath);
        }
    }

    /**
     * @brief 向文件写入格式化文本
     * @details
     *   支持两种写入模式：
     *     - "-w"：覆盖写入（write），即清空目标文件后写入
     *     - "-a"：追加写入（append），即保留原内容后追加（默认模式）
     *   文本内容由 parameters 列表中除最后一项外的所有元素拼接而成，
     *   最后一项为目标文件路径，可为相对路径或绝对路径。
     *
     * @param flags      写入模式标志列表，若含 "-w" 则覆盖写入，否则追加写入
     * @param parameters 文本片段和文件路径列表，最后一项为文件路径，其余项拼接为写入内容
     */
    void printf(const std::vector<std::string>& flags, const std::vector<std::string>& parameters) {
        // 1. 校验标志和参数

        // 标志校验
        /// flags 至多只能传一个；传多个视为格式错误
        if (flags.size() > 1) {
            WARN(WarningLevel::ERROR, "[标志错误] 只能接受一个标志, 用法: echo [-w|-a] <string> <fileName|fullFilePath>");
            return;
        }

        // 参数校验
        /// parameters 至少应包含两个：一个或多个字符串 + 一个文件路径
        if (parameters.size() < 2) {
            WARN(WarningLevel::ERROR, "[参数错误] 参数数量不足, 用法: echo [-w|-a] <string> <fileName|fullFilePath>");
            return;
        }

        // 2. 解析写入模式：默认追加 ("a")，如遇 "-w" 切换为覆盖 ("w")
        const char* mode = "a";                              // 默认追加写入
        if (!flags.empty() && flags[0] == "-w") mode = "w";  // 覆盖写入

        // 3. 拼接写入内容：parameters[0..n-2] 拼接成完整字符串，末尾加换行
        size_t textCount = parameters.size() - 1;  ///< 文本片段数量
        size_t estimatedSize = 0;
        for (size_t i = 0; i < textCount; ++i) estimatedSize += parameters[i].size() + 1;  ///< 统计每段长度及空格

        // 预估总长度：所有片段长度 + 空格数 + 换行符
        std::string content;             ///< 最终写入内容
        content.reserve(estimatedSize);  ///< 预分配，减少堆分配次数

        for (size_t i = 0; i < textCount; ++i) {
            if (i > 0) content.push_back(' ');  ///< 插入空格分隔每段文本
            content.append(parameters[i]);      ///< 追加当前文本片段
        }

        Serial.println(content.c_str());

        // ----- 4. 构建完整文件路径 -----
        const std::string fullPath = buildFullPath(parameters.back());  ///< 将最后一项路径转换为绝对路径

        // ----- 5. 执行写入操作 -----
        bool success = file_.writeFileAsString(fullPath, content, mode);

        // ----- 6. 结果检查 -----
        if (!success) {  ///< 写入失败时发出警告
            WARN(WarningLevel::ERROR, "[写入失败] 无法写入文件: ", fullPath);
        }
    }

    /**
     * @brief 在指定目录或当前工作目录下精确查找文件或目录
     *
     * @details
     * - 支持两种调用形式：
     *     1. `find <name>`：在当前工作目录查找名称完全匹配 `<name>` 的文件或目录
     *     2. `find <fullDirPath> <name>`：在指定目录 `<fullDirPath>` 下查找名称完全匹配 `<name>` 的文件或目录
     *
     * @param flags      标志列表，本命令不使用任何标志，由框架层统一忽略
     * @param parameters 参数列表：
     *                   - 当 `parameters.size() == 1` 时，`parameters[0]` 为待查找名称
     *                   - 当 `parameters.size() == 2` 时，`parameters[0]` 为目录路径，`parameters[1]` 为待查找名称
     */
    void find(const std::vector<std::string>& flags, const std::vector<std::string>& parameters) {
        // ----- 1. 参数数量校验 -----
        // 本命令仅允许 1 或 2 个参数，多于或少于时打印错误并返回
        if (parameters.size() < 1 || parameters.size() > 2) {
            WARN(WarningLevel::ERROR, "[参数错误] 用法: find [fullDirPath] <name>");
            return;  // 参数数目不正确，退出
        }

        // ----- 2. 确定搜索目录和目标名称 -----
        std::string parentPath;  // 待搜索的父目录绝对路径
        std::string targetName;  // 待查找的文件或目录名称

        if (parameters.size() == 1) {
            // 仅提供名称时，使用当前工作目录搜索
            targetName = parameters[0];  // 设置查找名称
            parentPath = workingDir_;    // 使用成员变量记录的当前工作目录
        } else {
            // 提供目录路径和名称时，先将路径转为绝对路径，再设置名称
            parentPath = buildFullPath(parameters[0]);  // 构建绝对路径
            targetName = parameters[1];                 // 设置查找名称
        }

        // ----- 3. 执行查找 -----
        // 调用 FileExplorer::findPath，返回所有匹配路径
        std::vector<std::string> results = file_.findPath(targetName, parentPath);

        // ----- 4. 输出查找结果 -----
        if (results.empty()) {
            // 若无任何匹配结果，则打印信息并返回
            WARN(WarningLevel::INFO, "[无匹配结果] 未找到: ", targetName);
            return;  // 无结果，退出
        }

        // 遍历所有匹配路径并输出
        for (const auto& path : results) Serial.println(path.c_str());
    }

    /**
     * @brief   在指定目录或当前工作目录下模糊搜索文件或目录
     *
     * @details
     * - 支持两种调用形式：
     *     1. `search <name>`：在当前工作目录下递归搜索名称与 `<name>` 相似度 ≥ 阈值的文件或目录
     *     2. `search <fullDirPath> <name>`：在指定目录 `<fullDirPath>` 下递归搜索名称与 `<name>` 相似度 ≥ 阈值的文件或目录
     * - 相似度计算由底层 FileExplorer::searchPath 实现，可基于编辑距离、前缀匹配等算法
     *
     * @param flags      标志列表，本命令不使用任何标志，由框架层统一忽略
     * @param parameters 参数列表：
     *                   - 当 `parameters.size() == 1` 时，`parameters[0]` 为待搜索名称
     *                   - 当 `parameters.size() == 2` 时，`parameters[0]` 为搜索起始目录，`parameters[1]` 为待搜索名称
     */
    void search(const std::vector<std::string>& flags, const std::vector<std::string>& parameters) {
        // ----- 1. 参数数量校验 -----
        // 本命令仅允许 1 或 2 个参数，超出范围则报错并返回
        if (parameters.size() < 1 || parameters.size() > 2) {
            WARN(WarningLevel::ERROR, "[参数错误] 用法: search [fullDirPath] <name>");
            return;  // 参数不合法，退出
        }

        // ----- 2. 确定搜索目录和目标名称 -----
        std::string parentPath;  // 搜索起始目录的绝对路径
        std::string targetName;  // 待搜索名称

        if (parameters.size() == 1) {
            // 仅提供名称时，使用当前工作目录
            targetName = parameters[0];  // 赋值待搜索名称
            parentPath = workingDir_;    // 使用成员变量记录的当前工作目录
        } else {
            // 同时提供路径和名称时，先构建绝对路径
            parentPath = buildFullPath(parameters[0]);  // 构建并获取绝对路径
            targetName = parameters[1];                 // 赋值待搜索名称
        }

        // ----- 3. 执行模糊搜索 -----
        // 调用底层接口，threshold 默认为 0.0f，表示返回所有名称中含有 targetName 的项
        constexpr float threshold = 0.0f;  // 相似度阈值，可在未来扩展为参数
        std::vector<std::string> results = file_.searchPath(targetName, parentPath, threshold);

        // ----- 4. 输出搜索结果 -----
        if (results.empty()) {
            // 若未找到任何匹配项，则打印信息并返回
            WARN(WarningLevel::INFO, "[无匹配结果] 未找到: ", targetName);
            return;  // 无结果，退出
        }

        // 遍历所有匹配路径并输出
        for (const auto& path : results) Serial.println(path.c_str());
    }

   private:
    /**
     * @brief 处理特殊路径操作：前进、后退、返回上一级目录
     * @param cmd 输入命令字符串
     * @return 已处理返回 true，未处理返回 false
     */
    bool handleSpecial(const std::string& cmd) {
        if (cmd == "<") return goBack();                     // 后退
        if (cmd == ">") return goForward();                  // 前进
        if (cmd == "../" || cmd == "..") return goParent();  // 返回上一级目录
        return false;
    }

    /**
     * @brief 后退至上一次访问的目录
     * @return 操作成功返回 true，否则 false
     */
    bool goBack() {
        if (backQueue_.empty()) return false;  // 后方缓冲队列不能为空
        forwardQueue_.pushBack(workingDir_);   // 将当前工作目录推入前方缓冲队列
        workingDir_ = backQueue_.back();       // 更新工作目录为上一次访问的目录
        backQueue_.popBack();                  // 后方缓冲队列弹出上一次访问的目录

        return true;
    }

    /**
     * @brief 前进至最近一次后退前的目录
     * @return 操作成功返回 true，否则 false
     */
    bool goForward() {
        if (forwardQueue_.empty()) return false;  // 前方缓冲队列不能为空
        backQueue_.pushBack(workingDir_);         // 将当前工作目录推入后方缓冲队列
        workingDir_ = forwardQueue_.back();       // 更新工作目录为最近一次后退前的目录
        forwardQueue_.popBack();                  // 前方缓冲队列弹出最近一次后退前的目录

        return true;
    }

    /**
     * @brief 切换到当前目录的父目录
     * @return 操作成功返回 true，否则 false
     */
    bool goParent() {
        // 分割当前路径为各级目录
        std::vector<std::string> tokens = splitter_.split(workingDir_, {"/"});
        if (tokens.empty()) return false;  // 已在根目录，无法上移

        // 后方缓冲队列，清空前方缓冲队列
        backQueue_.pushBack(workingDir_);
        forwardQueue_.clear();

        // 移除最后一级目录，重建路径
        tokens.pop_back();
        std::string parent = "/";
        for (const auto& level : tokens) parent += level + "/";

        // 去除末尾多余 '/'
        if (parent.size() > 1 && parent.back() == '/') parent.pop_back();

        workingDir_ = std::move(parent);

        return true;
    }

    /**
     * @brief 从绝对路径中提取路径的文件名或目录名部分
     *
     * @param fullPath 输入文件或目录的绝对路径
     * @return 返回文件名或目录名
     */
    std::string getName(const std::string& fullPath) { return splitter_.split(fullPath, {"/"}).back(); }

    /**
     * @brief 获取一个路径的父路径
     *
     * - 若输入为根目录 "/"，或不包含任何斜杠，则返回空字符串 ""
     * - 若父目录为根目录，则返回 "/"
     * - 其它情况，返回去除末尾文件或目录名后的路径
     *
     * @param path 输入路径字符串
     * @return 返回父目录路径
     */
    std::string getParentPath(const std::string& path) {
        // 1. 如果输入为空或恰为根目录 "/"，则无父目录，直接返回空字符串
        if (path.empty() || path == "/") return "";

        // 2. 查找路径中，最末尾的不为 '/' 的位置，用于去除尾部多余斜杠
        //    find_last_not_of 能在常量时间内定位，无需额外内存分配
        const std::size_t lastNonSlash = path.find_last_not_of('/');
        // 如果找不到任何非斜杠字符，说明路径全由斜杠组成（如 "///"），视作根目录
        if (lastNonSlash == std::string::npos) return "";

        // 3. 在 lastNonSlash 位置之前，查找最后一个 '/'，用于分隔父路径与子路径
        //    rfind 从指定位置向前搜索，可减少搜索区间
        const std::size_t slashPos = path.rfind('/', lastNonSlash);
        // 如果没有找到任何分隔符，则说明输入路径不包含父目录部分，返回空
        if (slashPos == std::string::npos) return "";

        // 如果分隔符在最前端，即 pos==0，则父目录即根目录 "/"
        if (slashPos == 0) return "/";

        // 4. 对于一般情况，直接截取从 0 到 slashPos 的子串作为父路径
        //    substr 会分配新的字符串，但仅分配必要长度，性能开销有限
        return path.substr(0, slashPos);
    }

    /**
     * @brief 判断路径是否为绝对路径
     * @param path 输入路径字符串
     * @return true 表示是绝对路径（以 '/' 开头）
     */
    bool isAbsolutePath(const std::string& path) { return !path.empty() && path[0] == '/'; }

    /**
     * @brief 构造完整的绝对路径
     *
     * - 判断输入的路径字符串是否是绝对路径, 如果不是则自动补全构造完整路径
     *
     * @param target 目标路径字符串（绝对或相对）
     * @param basePath 基路径,基于基路径构建完整路径(默认为工作路径)
     * @return 返回完整的绝对路径
     */
    std::string buildFullPath(const std::string& target) { return buildFullPath(target, workingDir_); }
    std::string buildFullPath(const std::string& target, const std::string& basePath) {
        std::string fullPath;

        // 判断目标路径是否为绝对路径（如以 '/' 开头）
        if (isAbsolutePath(target)) {
            fullPath = target;
        } else {
            // 相对路径：需要将当前工作目录与目标路径拼接

            // 情况一：当前目录为根目录 "/"
            // 特殊处理避免拼接出 "//xx"（根目录与目标之间无需额外加斜杠）
            if (basePath == "/") {
                fullPath = "/" + target;
            }
            // 情况二：普通路径，拼接斜杠和子目录名
            else {
                fullPath = basePath + "/" + target;
            }
        }

        return fullPath;
    }

   private:
    FSInterface fs_;                            ///< 文件系统接口
    FileExplorer file_;                         ///< 文件操作对象
    RingBuffer<std::string, 16> backQueue_;     ///< 后方环形缓冲队列，最多存储 16 条记录
    RingBuffer<std::string, 16> forwardQueue_;  ///< 前方环形缓冲队列，最多存储 16 条记录
    std::string workingDir_ = "/";              ///< 当前工作目录，初始化为根目录
    StringSplitter splitter_;                   ///< 字符串分割工具，用于路径拆分
};