#pragma once

#include <string>
#include <vector>

/**
 * [�ַ���ǿ������] ��������һ�������ǿ��, ���е÷�Խ��������Խ��ȫ;
 *
 * 1. �������ۣ��ַ���������������ĸ��Ӷ�ָ��֮һ��
 * 2. �ַ������ԣ������ַ����в�ͬ�ַ������࣬����Сд��ĸ����д��ĸ�����֡������ַ��ȡ�
 * 3. �ظ��Լ�飺����ַ�������ͬ�ַ����Ӵ��ظ����ֵ�Ƶ�ʣ�Ƶ���ظ����ܽ�������ĸ��Ӷȡ�
 * 4. �صļ��㣺�ؿ��������ַ����Ĳ���Ԥ���Ժ�����ԡ�
 */
class StringStrengthEvaluator {
   private:
    // ����������ֵ�Ȩ�أ����Ը������밲ȫ��Ҫ�����
    const float lengthWeight;          // ����Ȩ��
    const float diversityWeight;       // �ַ�������Ȩ��
    const float repetitivenessWeight;  // �ظ���Ȩ��
    const float entropyWeight;         // ��Ȩ��

   public:
    // ���캯������ʼ���������ֵ�Ȩ�أ����Ը������밲ȫ��Ҫ�����
    StringStrengthEvaluator(const float lengthWeight = 1.5, const float diversityWeight = 1.2, const float repetitivenessWeight = 1.7,
                            const float entropyWeight = 1.3)
        : lengthWeight(lengthWeight), diversityWeight(diversityWeight), repetitivenessWeight(repetitivenessWeight), entropyWeight(entropyWeight) {}

    // �����ַ�����ǿ��
    float evaluateString(const std::string &str);

   private:
    // �����ַ������ȼ���������ϳ����ַ����÷ָ���
    float evaluateLength(const std::string &str);
    // �����ַ������ԣ���ͬ�����ַ�����д��Сд�����֡������ַ���ʹ�ò�ͬȨ��
    float evaluateDiversity(const std::string &str);

    // ����KMP�㷨����Ĳ���ƥ���Partial Match Table��
    // ��������ڿ������ַ���������������ƥ��Ĳ���
    std::vector<int> buildPartialMatchTable(const std::string &pattern);

    // ����ַ������ظ�ģʽ�Ĵ��ڣ�ʹ�û������ںͼ򻯰��KMP�㷨
    float evaluateRepetitiveness(const std::string &str);

    // ��ȷ�����ַ�������
    float calculateEntropy(const std::string &str);
};

/*
int main() {
    std::string testString = "000000";
    float score = StringStrengthEvaluator::evaluateString(testString);
    std::cout << "�ۺϵ÷�: " << score << std::endl;
    return 0;
}
*/