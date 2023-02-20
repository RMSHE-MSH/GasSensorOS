#include <CPPSTL.h>
#include <esp32_pin_defi.h>
#include <io.h>
#include <kernel_tasks.h>
#include <random.h>
#include <systime.h>

#include <code_interpreter.hpp>
#include <tree.hpp>

OLED oled;
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
    auto* node_1 = tree.root->findChild("node_1");

    // 向 node_1 添加两个子节点
    node_1->addChild("node_1_1");
    node_1->addChild("node_1_2");

    // 查找并获取 node_1_1节点
    auto* node_1_1 = tree.root->findChild("node_1_1");

    // 向 node_1_1 添加一个子节点
    node_1_1->addChild("node_1_1_1");

    // 遍历树结构
    auto tree_data = tree.traversalBFS(tree.root.get());
    for (auto& data : tree_data) Serial.println(data.c_str());

    Serial.println("");
    node_1_1->deleteChild("node_1_1_1");
    node_1->deleteChild("node_1_2");

    tree_data = tree.traversalBFS(tree.root.get());
    for (auto& data : tree_data) Serial.println(data.c_str());

    // 初始化OLED
    oled.OLED_Init();
    oled.OLED_ColorTurn(0);                       // 0正常显示 1反色显示
    oled.OLED_DisplayTurn(0);                     // 0正常显示 1翻转180度显示
    oled.OLED_DrawBMP(0, 0, 128, 64, RMSHE_IMG);  // 显示 RMSHE_Infinity LOGO;
    delay(2000);

    // runKernelTasks();
}

std::string code = "print(\"Hello GScode!\"); num a=0; num b = 1; print(a+b);";

void loop() {
    oled.print("RMSHE");
    // inte.interpreter(code);
    delay(2000);
}

// git config --global --unset http.proxy
// git config --global --unset https.proxy