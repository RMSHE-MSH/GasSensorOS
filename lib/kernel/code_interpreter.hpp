#pragma once
#include <Arduino.h>

#include <forward_queue.hpp>
#include <make_ptr.hpp>
#include <memory>
#include <regex>
#include <stack>
#include <tree.hpp>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// 枚举类, 包括 关键字|标识符(变量名)|运算符|数字|字符串|标点符号|...
enum class TokenType { Keyword, Identifier, Operator, Number, String, Punctuator };

// Token虚拟抽象基类;
class Token {
   public:
    virtual ~Token() {}
    virtual TokenType get_type() const = 0;
    virtual std::string get_value() const = 0;
};

// 关键字子类(令牌)
class KeywordToken : public Token {
   public:
    KeywordToken(const std::string& value) : value_(value) {}
    TokenType get_type() const override { return TokenType::Keyword; }
    std::string get_value() const override { return value_; }

   private:
    std::string value_;
};

// 标识符(变量名)子类(令牌)
class IdentifierToken : public Token {
   public:
    IdentifierToken(const std::string& value) : value_(value) {}
    TokenType get_type() const override { return TokenType::Identifier; }
    std::string get_value() const override { return value_; }

   private:
    std::string value_;
};

// 运算符子类(令牌)
class OperatorToken : public Token {
   public:
    OperatorToken(const std::string& value) : value_(value) {}
    TokenType get_type() const override { return TokenType::Operator; }
    std::string get_value() const override { return value_; }

   private:
    std::string value_;
};

// 数字子类(令牌)
class NumberToken : public Token {
   public:
    NumberToken(const std::string& value) : value_(value) {}
    TokenType get_type() const override { return TokenType::Number; }
    std::string get_value() const override { return value_; }

   private:
    std::string value_;
};

// 字符串子类(令牌)
class StringToken : public Token {
   public:
    StringToken(const std::string& value) : value_(value) {}
    TokenType get_type() const override { return TokenType::String; }
    std::string get_value() const override { return value_; }

   private:
    std::string value_;
};

// 标点符号子类(令牌)
class PunctuatorToken : public Token {
   public:
    PunctuatorToken(const std::string& value) : value_(value) {}
    TokenType get_type() const override { return TokenType::Punctuator; }
    std::string get_value() const override { return value_; }

   private:
    std::string value_;
};

//...

class Interpreter {
    friend class Token;
    friend class KeywordToken;
    friend class IdentifierToken;
    friend class OperatorToken;
    friend class NumberToken;
    friend class StringToken;
    friend class PunctuatorToken;

   public:
    /**
     * @brief Arduino字符串分割, 将一个给定的字符串 "input" 根据另一个字符串 "separator" 进行分割，并将分割后的每一段字符串存储在一个 vector 容器中.
     * @param input 输入一个String类型的代码字符串
     * @param separator 输入一个String类型的字符串作为分割字符串
     * @return vector<String>
     */
    std::vector<String> strsplit(String input, String separator) {
        std::vector<String> vecStr;
        while (input.indexOf(separator) != -1) {
            uint32_t splitIndex = input.indexOf(separator);
            String segment = input.substring(0, splitIndex);
            vecStr.push_back(segment);
            input = input.substring(splitIndex + separator.length());
        }
        vecStr.push_back(input);
        return vecStr;
        /*
        该函数的作用是将一个给定的字符串 "input" 根据另一个字符串 "separator" 进行分割，并将分割后的每一段字符串存储在一个 vector 容器中。

        这个函数首先使用 while 循环来检查输入字符串中是否存在 "separator"，如果存在，则使用 "input.indexOf(separator)" 方法来找到 "separator"
        第一次出现的位置，然后使用 "input.substring(0, splitIndex)" 方法来截取从 0 到 "splitIndex" 位置的字符串，将截取的字符串存储在 vector 容器 "vecStr"
        中。接着，使用 "input = input.substring(splitIndex + 分割字符的长度);" 更新输入字符串，以便在下一次循环中继续查找 "separator"。

        当循环结束后，将剩余的字符串 "input" 添加到 vector 容器 "vecStr" 中，并返回该容器。
        */
    }

    /**
     * @brief C++STL字符串分割, 将一个给定的字符串 "input" 根据另一个字符串 "separator" 进行分割，并将分割后的每一段字符串存储在一个 vector 容器中.
     * @param input 输入一个std::string类型的代码字符串
     * @param separator 输入一个std::string类型的字符串作为分割字符串
     * @return vector<std::string>
     */
    std::vector<std::string> std_strsplit(std::string input, std::string separator) {
        std::vector<std::string> vecStr;
        size_t splitIndex;
        while ((splitIndex = input.find(separator)) != std::string::npos) {
            std::string segment = input.substr(0, splitIndex);
            vecStr.push_back(segment);
            input = input.substr(splitIndex + separator.length());
        }
        vecStr.push_back(input);
        return vecStr;
    }

    /**
     * @brief 判断一句话中一个单词是否存在(正则表达).
     * @param input 输入一个std::string类型的代码字符串
     * @param word 判断这个单词是否存在于字符串中
     * @return 存在返回true, 不存在返回false
     */
    bool hasWord(const std::string& input, const std::string& word) {
        std::regex pattern("\\b" + word + "\\b");  // 构建正则表达规则，匹配单词边界
        return std::regex_match(input, pattern);
    }

    /**
     * @brief 判断一段字符串是否符合你提供的正则表达规则.
     * @param input 输入一个std::string类型的代码字符串
     * @param rule 提供一个std::string类型的正则表达规则
     * @return 若这个字符串符合提供的规则返回true, 否则返回false
     */
    bool matchesRegex(const std::string& input, const std::string& rule) {
        std::regex pattern(rule);  // 构建正则表达规则
        return std::regex_match(input, pattern);
    }

    /**
     * @brief 用于提取字符串.
     * @param input 输入一个std::string类型的代码字符串
     * @param rule 提供一个std::string类型的正则表达规则
     * @return 返回匹配到的字符串向量，其中每个元素都是一个unique_ptr指针，指向一个匹配到的字符串
     */
    std::vector<std::unique_ptr<std::string>> extract_strings(const std::string& input, const std::string& rule) {
        std::regex pattern(rule);                                              // 构造正则表达式
        std::sregex_token_iterator iter(input.begin(), input.end(), pattern);  // 匹配字符串
        std::sregex_token_iterator end;

        // 存储匹配到的字符串
        std::vector<std::unique_ptr<std::string>> result;

        // 遍历匹配到的字符串，并将每个字符串作为unique_ptr指针存储到结果数组中
        for (auto i = iter; i != end; ++i) result.push_back(make_unique<std::string>(i->str()));

        return result;
    }

    void interpreter(std::string code) {
        lexical_analysis(code);
        // syntax_analysis();

        Serial.println(code.c_str());
        Serial.println("");

        for (const auto& token : tokens) {
            TokenType type = token->get_type();
            std::string value = token->get_value();

            if (type == TokenType::Keyword) Serial.print("KEY[");
            if (type == TokenType::Identifier) Serial.print("IDE[");
            if (type == TokenType::Operator) Serial.print("OPE[");
            if (type == TokenType::Number) Serial.print("NUM[");
            if (type == TokenType::String) Serial.print("STR[");
            if (type == TokenType::Punctuator) Serial.print("PUN[");

            Serial.print(value.c_str());
            Serial.println("]");
            // ...
        }
        tokens.clear();
    }

   private:
    /**
     * @brief 对原始代码进行词法分析，生成token流.
     * @param input 传入原始代码的字符串
     * @return void
     */
    void lexical_analysis(const std::string& input) {
        // 通过正则表达式匹配，将字符串input分解为tokens_temp
        // 该正则表达式会匹配单词、非单词非空格字符或空格
        std::vector<std::unique_ptr<std::string>> tokens_temp = extract_strings(input, R"([\w']+|[^\w\s]|\ )");

        // 通过正则表达式匹配，将字符串input中被双引号包围的字符串分解为string_temp
        std::vector<std::unique_ptr<std::string>> string_temp = extract_strings(input, R"(\"[^\"]*\")");

        // 记录当前正在处理的字符串
        auto string_pptr = string_temp.begin();
        bool current_string = false;

        // 遍历tokens_temp中的每个token
        for (auto i = tokens_temp.begin(); i != tokens_temp.end(); ++i) {
            if (keyword_list.find(**i) != keyword_list.end()) {    // 匹配关键字
                tokens.push_back(make_unique<KeywordToken>(**i));  // 添加关键字token到tokens中
                continue;
            } else if (**i == "\"" && **(i - 1) != "\\") {  // 匹配字符串
                if (current_string == false) {              // 如果该token是一个字符串的开头，则将其添加到tokens中
                    tokens.push_back(make_unique<StringToken>(**string_pptr));
                    ++string_pptr;
                }
                current_string = !current_string;  // 记录当前字符串的状态，即是否处理完成
                continue;
            } else if (matchesRegex(**i, "^[a-zA-Z_]\\w*") == true && current_string == false) {  // 匹配标识符(字母或下划线开头)
                tokens.push_back(make_unique<IdentifierToken>(**i));                              // 添加标识符token到tokens中
                continue;
            } else if (operator_list.find(**i) != operator_list.end()) {  // 匹配运算符
                tokens.push_back(make_unique<OperatorToken>(**i));        // 添加运算符token到tokens中
                continue;
            } else if (matchesRegex(**i, "^(\\-\\+)?\\d+(\\.\\d+)?$") == true) {  // 匹配数字(正数负数和小数)
                tokens.push_back(make_unique<NumberToken>(**i));                  // 添加数字token到tokens中
                continue;
            } else if (punctuator_list.find(**i) != punctuator_list.end()) {  // 匹配标点符号
                tokens.push_back(make_unique<PunctuatorToken>(**i));          // 添加标点符号token到tokens中
                continue;
            }
        }
    }

    /**
     * @brief 语法分析, 将tokens转换为语法树.
     * @return void
     */
    // void syntax_analysis() {
    //     // 获取一个token的数据类型和值
    //     auto get_token = [](const std::unique_ptr<Token>& token_iterator) -> std::pair<TokenType, std::string> {
    //         TokenType type = token_iterator->get_type();
    //         std::string value = token_iterator->get_value();

    //         return std::pair<TokenType, std::string>(type, value);
    //     };

    //     forward_queue<std::pair<TokenType, std::string>> current_type(2);

    //     for (auto pptr = tokens.begin(); pptr != tokens.end(); ++pptr) {
    //         current_type.push(get_token(*pptr));

    //         switch (current_type.begin()->first) {
    //             case TokenType::Keyword:

    //                 break;
    //         }
    //     }

    //     // token = get_token(*i);
    //     // if (token.first == TokenType::Keyword && (token.second == "num" || token.second == "str")) {
    //     //     token = get_token(*(i + 1));
    //     //     if (token.first == TokenType::Operator && token.second == "=") {
    //     //         token = get_token(*(i + 2));
    //     //         if (token.first == TokenType::Identifier) {
    //     //             variables.insert(std::pair<std::string, std::unique_ptr<std::pair<double, std::string>>>(token.second, {}));
    //     //         }
    //     //     }
    //     // }
    // }

    // 储存 GS Coed 的变量<变量名, 变量值(数字/字符串)>;
    std::unordered_map<std::string, std::unique_ptr<std::pair<double, std::string>>> variables;

    // token流对象;
    std::vector<std::unique_ptr<Token>> tokens;

    // 关键字表(没有优先级)
    std::unordered_set<std::string> keyword_list = {"print", "num", "str", "def", "if", "while"};

    // 运算符表(没有优先级)
    std::unordered_set<std::string> operator_list = {"+", "-", "*", "/", "=", "==", "!=", "<", "<=", ">", ">=", "!", "&&", "||"};

    // 标点符号表(没有优先级)
    std::unordered_set<std::string> punctuator_list = {",", ";", "(", ")", "{", "}"};
};

/*
// 该匿名函数返回字符串 word1|word2|word3...(逻辑或关系) 的格式使得正则匹配多个关键字中的任意一个.
        auto rule = [this](std::unordered_set<std::string> word_list) -> std::string {
            std::string regex_rule_str;

            // 将 keyword_list(关键字列表)中的单词用"|"连接起来.
            for (const auto& word : word_list) regex_rule_str += word + "|";

            // 去除最后一个"|".
            if (!regex_rule_str.empty()) regex_rule_str.pop_back();

            return regex_rule_str;
        };
*/