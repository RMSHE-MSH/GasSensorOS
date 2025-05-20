# GSOS CLI Commands Cheat Sheet

## 🔣 命令行语法符号说明

下表描述了用于表示命令行语法的符号说明。

| 符号               | 描述                                         |
| :----------------- | :------------------------------------------- |
| 无括号文本         | 必须按原样输入的命令或参数。                 |
| `<尖括号内的文本>` | 必须提供具体值的占位符。                     |
| `[方括号内的文本]` | 可选项。                                     |
| `{大括号内的文本}` | 必选项集合，必须从中选择一项。               |
| 竖线符号 （`|`）   | 互斥项分隔符，必须从中选择一项。             |
| 省略号 （`...`）   | 可重复多次使用的项(表示前一项可以重复多次)。 |

---

## 📁 Flash Operating System Commands

### 1. 文件和目录操作

| 命令    | 功能                   | 语法 示例 和 描述                                            |
| ------- | ---------------------- | ------------------------------------------------------------ |
| `cd`    | 切换当前工作目录       | `cd <fullDirPath>`  *`cd /xxx/xx/x`*：切换到指定的绝对路径目录;<br/>`cd <dirName>`  *`cd xx`*：切换到当前工作目录下名为 `xx` 的子目录;<br/>`cd <`：返回到上一次访问的目录（后退）;<br/>`cd >`：前进至上一次撤销的目录;<br/>`cd ../` 或 `cd ..`：返回上一层目录; |
| `pwd`   | 打印当前工作目录       | `pwd`：打印当前工作目录的完整路径                            |
| `ls`    | 查看目录内容           | `ls`：查看当前工作目录下的内容；<br/>`ls <dirName>`  *`ls xx`*：查看当前工作目录下名为 `xx` 的子目录；<br/>`ls <fullDirPath>`  *`ls /xxx/xx`*：查看指定绝对路径目录的内容; |
| `tree`  | 以树状图列出目录的内容 | `tree`：以树状列出当前工作目录的结构；<br/>`tree <dirName>`  *`tree xx`*：查看当前工作目录下某子目录的树状结构；<br/>`tree <fullDirPath>`  *`tree /xxx/xx`*：查看指定路径的目录树; |
| `mkdir` | 创建目录               | `mkdir <dirName>`  *`mkdir xx`*：在当前工作目录下创建一个名为 `xx` 的目录;<br/>`mkdir <dirName> [dirName] [...]`  *`mkdir xx1 xx2`*：在当前工作目录下创建多个目录;<br/>`mkdir <fullDirPath>`  *`mkdir /xxx/xx`*：在指定路径下创建一个名为 `xx` 的目录;<br/>`mkdir <fullDirPath> [fullDirPath] [...]`  *`mkdir /xxx1/xx1 /xxx2/xx2`*：在指定路径下创建多个目录; |
| `rm`    | 删除目录和文件         | `rm <name>`  *`rm xx`*：删除当前工作目录下名为 `xx` 的目录或文件;<br/>`rm <name> [name] [...]`  *`rm xx1 xx2`*：删除当前工作目录下的多个目录或文件;<br/>`rm *`：删除当前工作目录下的所有目录和文件<br/>`rm <fullPath>`  *`rm /xxx/xx`*：删除指定路径下名为 `xx` 的目录或文件;<br/>`rm <fullPath> [fullPath] [...]`  *`rm /xxx1/xx1 /xxx2/xx2`*：删除指定路径下的多个目录或文件;<br/>`rm <fullPath>*`  *`rm /xxx/*`*：删除指定路径下的所有目录和文件; |
| `mv`    | 移动或重命名目录和文件 | `mv <oldName> <newName>`  *`mv file.txt new_name.txt `*：重命名工作目录下的目录或文件;<br/>`mv <name> <fullDirPath>`  *`mv file.txt /usr/dir`*：移动工作目录下的一个目录或文件到指定目录;<br/>`mv <name> [name] [...] <fullDirPath>`  *`mv file1.txt dir1 file2.txt /usr/dir`*：移动工作目录下的多个目录或文件到指定目录;<br/>`mv <fullPath> <fullDirPath>`  *`mv /dir1/file.txt /usr/dir`*：移动指定路径的一个文件或目录到目标目录;<br/>`mv <fullPath> [fullPath] [...] <fullDirPath>`  *`mv /dir1/file.txt /dir2 /usr/dir`*：移动多个指定路径下的文件或目录到目标目录; |

