#pragma once

#include <string>
#include <vector>

/**
 * [字符串强度评估] 用于评价一串密码的强度, 其中得分越高则密码越安全;
 *
 * 1. 长度评价：字符串长度是最基本的复杂度指标之一。
 * 2. 字符多样性：分析字符串中不同字符的种类，包括小写字母、大写字母、数字、特殊字符等。
 * 3. 重复性检查：检测字符串中相同字符或子串重复出现的频率，频繁重复可能降低密码的复杂度。
 * 4. 熵的计算：熵可以量化字符串的不可预测性和随机性。
 */
class StringStrengthEvaluator {
   private:
    // 定义各项评分的权重，可以根据密码安全性要求调整
    const float lengthWeight;          // 长度权重
    const float diversityWeight;       // 字符多样性权重
    const float repetitivenessWeight;  // 重复性权重
    const float entropyWeight;         // 熵权重

   public:
    // 构造函数：初始化各项评分的权重，可以根据密码安全性要求调整
    StringStrengthEvaluator(const float lengthWeight = 1.5, const float diversityWeight = 1.2, const float repetitivenessWeight = 1.7,
                            const float entropyWeight = 1.3)
        : lengthWeight(lengthWeight), diversityWeight(diversityWeight), repetitivenessWeight(repetitivenessWeight), entropyWeight(entropyWeight) {}

    // 评价字符串的强度
    float evaluateString(const std::string &str);

   private:
    // 基于字符串长度计算分数，较长的字符串得分更高
    float evaluateLength(const std::string &str);
    // 评价字符多样性，不同类型字符（大写、小写、数字、特殊字符）使用不同权重
    float evaluateDiversity(const std::string &str);

    // 构建KMP算法所需的部分匹配表（Partial Match Table）
    // 这个表用于快速在字符串搜索中跳过已匹配的部分
    std::vector<int> buildPartialMatchTable(const std::string &pattern);

    // 检测字符串中重复模式的存在，使用滑动窗口和简化版的KMP算法
    float evaluateRepetitiveness(const std::string &str);

    // 精确计算字符串的熵
    float calculateEntropy(const std::string &str);
};

/*
int main() {
    std::string testString = "000000";
    float score = StringStrengthEvaluator::evaluateString(testString);
    std::cout << "综合得分: " << score << std::endl;
    return 0;
}
*/