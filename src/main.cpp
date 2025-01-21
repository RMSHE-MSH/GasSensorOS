#include <command_line_interface.h>

#include <data_table.hpp>
#include <file_explorer.hpp>
#include <fs_interface.hpp>
#include <tree_tool.hpp>

Command_Line_Interface CLI;
StringSplitter splitter;
FileExplorer fe;
FileManager file;
FSInterface fs1;
TreeTool tree_tool;

/*
void setup() {
    Serial.begin(115200);
    delay(4000);

    FSInterface fs;

    fs.mount();

    fs.open("/test1.txt", "w");

    // 要写入的数据
    std::string data = "Hello, LittleFS!";

    // 调用 write 函数写入数据
    size_t bytesWritten = fs.write(data.c_str(), data.size());

    // 输出写入的字节数
    Serial.print("写入了 ");
    Serial.print(bytesWritten);
    Serial.println(" 字节数据");

    fs.close();

    fs.open("/test1.txt", "r");

    // 创建一个缓冲区来存储数据
    uint8_t buffer[512];  // 假设一次最多读取 512 字节

    // 从文件读取数据
    size_t bytesRead = fs.read(buffer, sizeof(buffer));

    // 输出读取的字节数
    Serial.print("读取了 ");
    Serial.print(bytesRead);
    Serial.println(" 字节数据:");

    // 输出读取的数据
    for (size_t i = 0; i < bytesRead; i++) {
        Serial.print((char)buffer[i]);  // 将字节作为字符打印
    }

    fs.close();

    if (fs.exists("/test1.txt")) {
        Serial.println("\n文件存在");
    } else {
        Serial.println("\n文件不存在");
    }

    Serial.println(fs.getSize("/test1.txt"));

    fs.remove("/test1.txt");

    fs.unmount();
}

*/

void setup() {
    Serial.begin(115200);
    delay(4000);

    fe.deletePath("/");

    DataTable table(7, 13, "*");
    table.replaceRow({"", "", "", "", "", "", "", "", "", "", "", ""}, 8);
    table.replaceRow({"A", "B", "C", "D", "E", "F"}, 0);

    table.printTable();
    table.saveTable("/.os/table.csv");
    // Serial.println(fe.readFileAsString("/.os/table.csv").c_str());

    fe.createFile("/.os/dir1/test1.txt");
    fe.createFile("/.os/dir2/test2.txt");
    fe.createFile("/.os/dir3/test3.txt");

    fe.printTree();

    DataTable table_load(1, 1, "");
    table_load.loadTable("/.os/table.csv");
    table_load.printTable();
}

void loop() { CLI.run(); }
