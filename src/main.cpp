#include <file_explorer.h>

void setup() {
    Serial.begin(115200);
    delay(4000);

    // 初始化文件操作对象
    FileExplorer file;

    // 在指定路径创建文件
    file.createFile("/source/vector/world.os");
    file.createFile("/source/vector/work.os");
    file.createFile("/source/vector/working.os");

    // 获取 source 目录下的树结构
    std::unique_ptr<Tree<std::string>> tree = file.getTree("/source");

    // 初始化 TreeTool 工具
    TreeTool tree_tool;

    // 调用 TreeTool 获取树结构的字符串表示
    std::string list_str = tree_tool.getTreeString(*tree);

    // 打印文件树的字符串表示
    Serial.println(list_str.c_str());

    file.deletePath("/");
}

void loop() {}
