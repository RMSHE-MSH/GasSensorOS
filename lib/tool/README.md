# tree.hpp

轻量树数据结构容器测试程序:

```c++
#include <CPPSTL.h>
#include <esp32_pin_defi.h>
#include <io.h>
#include <kernel_tasks.h>
#include <random.h>
#include <systime.h>

#include <code_interpreter.hpp>
#include <tree.hpp>

Interpreter inte;

void setup() {
    Serial.begin(115200);

    delay(3000);

    // 创建树的根节点
    Tree<std::string> tree("root");

    // 向根节点添加两个子节点
    tree.root->addChild("node_1");
    tree.root->addChild("node_2");

    // 查找并获取 node_1 节点
    auto* node_1 = tree.findNode("node_1");

    // 向 node_1 添加两个子节点
    node_1->addChild("node_1_1");
    node_1->addChild("node_1_2");

    // 查找并获取 node_1_1节点
    auto* node_1_1 = tree.findNode("node_1_1");

    // 向 node_1_1 添加一个子节点
    node_1_1->addChild("node_1_1_1");

    // 遍历树结构
    auto tree_data = tree.traversalDFS();
    for (auto& data : tree_data) Serial.println(data.first.c_str());

    Serial.println("");
    // node_1_1->deleteChild("node_1_1_1");
    // node_1->deleteChild("node_1_2");
    // tree.deleteNode(node_1_1);
    Serial.println(tree.getDepth());
    Serial.println(tree.getDegree());
    Serial.println(tree.get_degree_of_tree());
    Serial.println(tree.getBreadth());
    Serial.println(tree.getSize());
    Serial.println(tree.getLevel(tree.findNode("root")));
    Serial.println(tree.empty());
    Serial.println(tree.getWidth());

    tree.deleteTree();

    tree_data = tree.traversalBFS();
    for (auto& data : tree_data) Serial.println(data.first.c_str());

    // runKernelTasks();
}

std::string code = "print(\"Hello GScode!\"); num a=0; num b = 1; print(a+b);";

void loop() {
    // inte.interpreter(code);
    delay(2000);
}

// git config --global --unset http.proxy
// git config --global --unset https.proxy
```

