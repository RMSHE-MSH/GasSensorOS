/**
 * @file main.cpp
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

#include <CPPSTL.h>
#include <esp32_pin_defi.h>
#include <io.h>
#include <kernel_tasks.h>
#include <random.h>
#include <systime.h>

#include <random>

// #include <code_interpreter.hpp>
#include <fourier_transform.hpp>
#include <rivest_cipher_4.hpp>
#include <tree.hpp>
#include <wifi_connector.hpp>

/*
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

    // 向 node_1 添加两个子节点, 并且向 node_1_1 再添加一个子节点 node_1_1_1
    node_1->addChild("node_1_1")->addChild("node_1_1_1");
    node_1->addChild("node_1_2");

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
*/

FastFourierTransform fft;
DiscreteFourierTransform dft;

// 生成时域波形
uint32_t N = 1024;
float dt = 0.01;
float df = 1.0 / (N * dt);

std::vector<float> input(N);
std::vector<std::complex<float>> output;

WiFiConnector WIFI("ccdx-wifi", "", true);

void setup() {
    Serial.begin(115200);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.8, 1.8);

    for (int i = 0; i < N; ++i) {
        for (float j = 0; j < 16; j += 0.1) {
            input[i] = input[i] + sin((j * 2 * PI) * i * dt);
        }
        // input[i] = sin((10 * 2 * PI) * i * dt) + sin((20 * 2 * PI) * i * dt) + sin((30 * 2 * PI) * i * dt) + dis(gen);
    }

    delay(3000);

    WIFI.connect();

    // output = dft.DFT(input);
}

// RivestCipher4 RC4("000");

void loop() {
    // inte.interpreter(code);

    /*
    for (int i = 0; i < N; ++i) {
        float freq = i * df;
        float fft = abs(output[i]);

        Serial.print(input[i]);
        Serial.print(" ");
        Serial.print(freq);
        Serial.print(" ");
        Serial.print(fft);
        Serial.println();
    }*/

    // Serial.println(RC4.encrypt("2837271954").c_str());
}

// git config --global --unset http.proxy
// git config --global --unset https.proxy