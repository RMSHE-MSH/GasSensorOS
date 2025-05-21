# GSOS CLI Commands Cheat Sheet

本文档整理了 `GasSensorOS` 目前支持的全部命令行操作，适用于嵌入式平台的常用管理任务。文档遵循统一语法规则，便于快速查阅和学习。

## 🔣 命令行语法符号说明

下表描述了用于表示命令行语法的符号说明。

| 符号               | 描述                                         |
| :----------------- | :------------------------------------------- |
| 无括号文本         | 必须按原样输入的命令或参数。                 |
| `<尖括号内的文本>` | 必须提供具体值的占位符。                     |
| `[方括号内的文本]` | 可选项。                                     |
| `{大括号内的文本}` | 必选项集合，必须从中选择一项。               |
| 竖线符号 （`\|`）  | 互斥项分隔符，必须从中选择一项。             |
| 省略号 （`...`）   | 可重复多次使用的项(表示前一项可以重复多次)。 |

---

## 📁 Flash Operating System Commands

### 1. 文件和目录操作

| 命令    | 功能                              | 语法 示例 和 描述                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    |
| ------- | --------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| `cd`    | `cd`<br/>切换当前工作目录         | `cd <fullDirPath>`  *`cd /xxx/xx/x`*：切换到指定的绝对路径目录;<br/>`cd <dirName>`  *`cd xx`*：切换到当前工作目录下名为 `xx` 的子目录;<br/>`cd <`：返回到上一次访问的目录（后退）;<br/>`cd >`：前进至上一次撤销的目录;<br/>`cd ../` 或 `cd ..`：返回上一级目录;                                                                                                                                                                                                                                                                                                                                                                                                                      |
| `pwd`   | `pwd`<br/>打印当前工作目录        | `pwd`：打印当前工作目录的完整路径                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    |
| `ls`    | `ls`<br/>查看目录内容             | `ls`：查看当前工作目录下的内容；<br/>`ls <dirName>`  *`ls xx`*：查看当前工作目录下名为 `xx` 的子目录；<br/>`ls <fullDirPath>`  *`ls /xxx/xx`*：查看指定绝对路径目录的内容;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           |
| `tree`  | `tree`<br/>以树状图列出目录的内容 | `tree`：以树状列出当前工作目录的结构；<br/>`tree <dirName>`  *`tree xx`*：查看当前工作目录下某子目录的树状结构；<br/>`tree <fullDirPath>`  *`tree /xxx/xx`*：查看指定路径的目录树;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   |
| `mkdir` | `mkdir`<br/>创建目录              | `mkdir <dirName>`  *`mkdir xx`*：在当前工作目录下创建一个名为 `xx` 的目录;<br/>`mkdir <dirName> [dirName] [...]`  *`mkdir xx1 xx2`*：在当前工作目录下创建多个目录;<br/>`mkdir <fullDirPath>`  *`mkdir /xxx/xx`*：在指定路径下创建一个名为 `xx` 的目录;<br/>`mkdir <fullDirPath> [fullDirPath] [...]`  *`mkdir /xxx1/xx1 /xxx2/xx2`*：在指定路径下创建多个目录;                                                                                                                                                                                                                                                                                                                       |
| `rm`    | `rm`<br/>删除目录和文件           | `rm <name>`  *`rm xx`*：删除当前工作目录下名为 `xx` 的目录或文件;<br/>`rm <name> [name] [...]`  *`rm xx1 xx2`*：删除当前工作目录下的多个目录或文件;<br/>`rm *`：删除当前工作目录下的所有目录和文件<br/>`rm <fullPath>`  *`rm /xxx/xx`*：删除指定路径下名为 `xx` 的目录或文件;<br/>`rm <fullPath> [fullPath] [...]`  *`rm /xxx1/xx1 /xxx2/xx2`*：删除指定路径下的多个目录或文件;<br/>`rm <fullPath>*`  *`rm /xxx/*`*：删除指定路径下的所有目录和文件;<br/><br/>`[⚠️警告]` 删除操作不可恢复，请谨慎使用 `rm /` 和路径通配形式。                                                                                                                                                         |
| `mv`    | `mv`<br/>移动或重命名目录和文件   | `-f`: 强制覆盖已存在的目标;<br/>`-v`: 显示详细输出信息;<br/><br/>`mv [-f] [-v] <oldName> <newName>`  *`mv file.md new_name.md `*：重命名工作目录下的目录或文件;<br/>`mv [-f] [-v] <name> <fullDirPath>`  *`mv file.md /usr/dir`*：移动工作目录下的一个目录或文件到指定目录;<br/>`mv [-f] [-v] <name> [name] [...] <fullDirPath>`  *`mv file1.md dir1 file2.md /usr/dir`*：移动工作目录下的多个目录或文件到指定目录;<br/>`mv [-f] [-v] <fullPath> <fullDirPath>`  *`mv /dir1/file.md /usr/dir`*：移动指定路径的一个文件或目录到目标目录;<br/>`mv [-f] [-v] <fullPath> [fullPath] [...] <fullDirPath>`  *`mv /dir1/file.md /dir2 /usr/dir`*：移动多个指定路径下的文件或目录到目标目录; |
| `cp`    | `cp`<br/>复制目录和文件           | `-f`: 强制覆盖已存在的目标；<br/>`-v`: 显示详细输出信息;<br/><br/>`cp [-f] [-v] <name> <fullDirPath>`  *`cp file.md /usr/dir`*：将工作目录下的文件或目录复制到指定目录；<br/>`cp [-f] [-v] <name> [name] [...] <fullDirPath>`  *`cp file1.md dir1 file2.md /usr/dir`* ：将多个工作目录下的文件或目录复制到指定目录；<br/>`cp [-f] [-v] <fullPath> <fullDirPath>`  *`cp /dir1/file.md /usr/dir`*：将指定路径的文件或目录复制到指定目录；<br/>`cp [-f] [-v] <fullPath> [fullPath] [...] <fullDirPath>`  *`cp /dir1/file.md /dir2 /usr/dir`*：将多个指定路径下的文件或目录复制到目标目录；                                                                                              |
| `touch` | `touch`<br/>创建空文件            | `touch <fileName>`  *`touch file.md`*: 在当前工作目录下创建一个空文件;<br/>`touch <fileName> [fileName] [...]`  *`touch file1.md file2.md`*: 在当前工作目录下创建多个空文件;<br/>`touch <fullFilePath>`  *`touch /dir/file.md`*: 在指定目录下创建一个空文件;<br/>`touch <fullFilePath> [fullFilePath] [...]`  *`touch /dir/file1.md /file2.md`*: 在指定目录下创建多个空文件;                                                                                                                                                                                                                                                                                                         |

### 2. 文件内容查看与写入

| 命令             | 功能                     | 语法 示例 和 描述      |
| ---------------- | ------------------------ | ---------------------- |
| `cat {<fileName>\|<fullFilePath>}`        | `cat`<br/>查看文件内容 | `cat <fileName>`  *`cat file.txt`*: 查看工作目录下一个名为`file.txt`文件的内容;<br/>`cat <fullFilePath>`  *`cat /dir/file.txt`*: 查看指定路径文件的内容;<br/><br/>`[📌注意]` 该命令将整个文件内容一次性输出到终端，适用于小文件快速预览; |
| `echo [-w\|-a] <string> {<fileName>\|<fullFilePath>}`     | `echo`<br/>向文件写入单行文本                                                                                                                                                                                                           | `-w`: 覆盖写入；<br/>`-a`:  追加写入(不提供标志时默认为追加写入);<br/><br/>`echo [-w\|-a] <string> <fileName>`  *`echo -w Hello,world! file.md`*: 向工作目录下的文件写入文本;<br/>`echo [-w\|-a] <string> <fullFilePath>`  *`echo -a Hello,world! /dir/file.md`*: 向指定路径的文件写入文本;<br/><br/>`[📌注意1]` 如果文件不存在，则会创建新文件并写入内容;<br/>`[📌注意2]` 该命令会自动为每次写入的文本换行，适合逐行追加;                      |
| `printf [-w\|-a] <string> {<fileName>\|<fullFilePath>}`      | `printf`<br/>向文件写入格式化文本                                                                                                                                                                                                       | `-w`: 覆盖写入；<br/>`-a`:  追加写入(不提供标志时默认为追加写入);<br/><br/>`printf [-w\|-a] <string> <fileName>`  *`printf -w Hello\tworld! file.md`*: 向工作目录下的文件写入文本;<br/>`printf [-w\|-a] <string> <fullFilePath>`  *`printf -a Hello\nworld! /dir/file.md`*: 向指定路径的文件写入文本;<br/><br/>`[📌注意1]` 如果文件不存在，则会创建新文件并写入内容;<br/>`[📌注意2]` 该命令后续会升级更强大的功能;<br/>`[📌支持的转义符]`:  `\t`  `\n`; |

> **说明**
>
> - `-w` 覆盖写入；`-a` 追加写入（默认）。
> - 文件不存在时会自动创建。
> - `echo` 会自动在末尾加换行；`printf` 不会。

### 3. 查找命令

| 命令                          | 功能                            | 语法 示例 和 描述                                                                                                                                                                                                                                         |
| ----------------------------- | ------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `find [fullDirPath] <name>`   | `find`<br/>精确查找文件和目录   | `find <name>`  *`find file.md`*: 在工作目录下查找文件或目录;<br/>`find <fullDirPath> <name>`  *`find /dir file.md`*: 在指定路径下查找文件或目录;<br/><br/>`[📌注意1]` 该命令用于查找名称完全匹配的文件或目录;                                              |
| `search [fullDirPath] <name>` | `search`<br/>模糊搜索文件和目录 | `search <name>`  *`search file.md`*: 在工作目录下搜索文件或目录;<br/>`search <fullDirPath> <name>`  *`search /dir file.md`*: 在指定路径下搜索文件或目录;<br/><br/>`[📌注意1]` 该命令用于搜索名称相似的文件或目录;<br/>`[📌注意2]` 搜索结果按相似度降序排列; |

> **说明**
>
> - `find`：名称完全匹配。
> - `search`：结果按相似度降序输出。
> - 单参数时在当前工作目录下搜索，两参数时在指定路径下搜索。


---
> @file GSOS CLI Commands Cheat Sheet.md
> @date 21.05.2025
> @author RMSHE
>
>  < GasSensorOS >
>  Copyright(C) 2025 RMSHE. All rights reserved.
>
>  This program is free software : you can redistribute it and /or modify
>  it under the terms of the GNU Affero General Public License as
>  published by the Free Software Foundation, either version 3 of the
>  License, or (at your option) any later version.
>
>  This program is distributed in the hope that it will be useful,
>  but WITHOUT ANY WARRANTY; without even the implied warranty of
>  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
>  GNU Affero General Public License for more details.
>
>  You should have received a copy of the GNU Affero General Public License
>  along with this program.If not, see < https://www.gnu.org/licenses/>.
>
>  Electronic Mail : asdfghjkl851@outlook.com