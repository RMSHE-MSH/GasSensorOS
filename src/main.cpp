#include <command_table.hpp>

class COMMAND_FUNCS {
   public:
    void test1(const std::vector<std::string>& flags, const std::vector<std::string>& parameters) { Serial.println("test1"); }
    void test2(const std::vector<std::string>& flags, const std::vector<std::string>& parameters) { Serial.println("test2"); }
    void test3(const std::vector<std::string>& flags, const std::vector<std::string>& parameters) { Serial.println("test3"); }
};

void setup() {
    Serial.begin(115200);
    delay(4000);

    COMMAND_TABLE cmd_table;
    COMMAND_FUNCS cmd_funcs;

    cmd_table.add_cmd("test1", {}, std::bind(&COMMAND_FUNCS::test1, &cmd_funcs, std::placeholders::_1, std::placeholders::_2));
    cmd_table.add_cmd("test2", {}, std::bind(&COMMAND_FUNCS::test2, &cmd_funcs, std::placeholders::_1, std::placeholders::_2));
    cmd_table.add_cmd("test3", {}, std::bind(&COMMAND_FUNCS::test3, &cmd_funcs, std::placeholders::_1, std::placeholders::_2));

    cmd_table.print_commands_table();
    cmd_table.execute_cmd("test2", {}, {});

    cmd_table.delete_cmd("test2");

    cmd_table.print_commands_table();
    cmd_table.execute_cmd("test2", {}, {});
}

void loop() {}