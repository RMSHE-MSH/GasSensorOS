#include <command_line_interface.h>
#include <sqlite3.h>

#include <data_table.hpp>
#include <file_explorer.hpp>
#include <fs_interface.hpp>
#include <tree_tool.hpp>

Command_Line_Interface CLI;
FileExplorer file;
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

    file.deletePath("/");
    file.createFile("/.os/dir1/test.txt");
    file.createFile("/.os/dir2/test.txt");
    file.createFile("/.os/dir3/test.txt");

    file.printTree();

    for (auto& i : file.searchPath("txt")) Serial.println(i.c_str());

    printf("[end] -> free_heap_size = %d\n", esp_get_free_heap_size());
}

void loop() { CLI.run(); }
