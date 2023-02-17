#pragma once
#include <Arduino.h>

#include <list>
#include <map>
#include <memory>
#include <regex>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// 枚举类, 包括 关键字|标识符(变量名)|运算符|数字|标点符号|...
enum class TokenType { Keyword, Identifier, Operator, Number, Punctuator };

// Token虚拟基类;
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

class interpreter {
    friend class KeywordToken;
    friend class IdentifierToken;

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
        std::regex pattern("\\b" + word + "\\b");  // 匹配单词边界
        return std::regex_match(input, pattern);
    }

    void interpre(std::string code) {
        for (auto& i : std_strsplit(code, ";")) {
            tokens.push_back(std::make_unique<KeywordToken>("if"));
            tokens.push_back(std::make_unique<IdentifierToken>("x"));
        }

        for (const auto& token : tokens) {
            TokenType type = token->get_type();
            std::string value = token->get_value();
            // ...
        }
    }

   private:
    std::vector<std::unique_ptr<Token>> tokens;
};
