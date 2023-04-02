#pragma once
#include <algorithm>
#include <cmath>
#include <complex>
#include <vector>

class FastFourierTransform {
   public:
    /**
     * @brief 快速傅里叶变换
     * @param input_sequence std::vector<float>类型实数域数据
     * @param inverse false 为正变换, true 为反变换(默认为正变换)
     */
    std::vector<std::complex<float>> FFT(std::vector<float> input_sequence, bool inverse = false) {
        /*
         * 首先，n & (n - 1) 的结果为0，当且仅当n是2的幂次方。如果n是2的幂次方，则m直接取n，不需要进行补零或截断操作。
         * 否则，m需要取离n最近的较小的2的幂次方或较大的2的幂次方。
         * 使用 builtin_clz 函数来计算n的二进制表示中前导零的个数，然后通过移位运算来得到最接近的2的幂次方。
         * 最后，使用三目运算符来判断是否需要进行补零或截断操作，直接resize到m即可。
         */
        uint32_t n = input_sequence.size();
        uint32_t m = n & (n - 1) ? 1 << (32 - CountLeadingZeros(n)) : n;
        input_sequence.resize(std::min(m, n));

        // 这里将实数域转为复数域;
        std::vector<std::complex<float>> data(input_sequence.begin(), input_sequence.end());

        // 若反转为 false 则进行FFT正变换, 若反转为 true 则进行FFT逆变换;
        inverse == false ? fast_fourier_transform(data) : inverse_fast_fourier_transform(data);

        return data;
    }

   private:
    // 计算一个32位无符号整数的二进制表示中前导零的个数;
    static uint32_t CountLeadingZeros(uint32_t x) {
        uint32_t n = 0;
        if (x == 0) return 32;
        if (x <= 0x0000FFFF) n += 16, x <<= 16;
        if (x <= 0x00FFFFFF) n += 8, x <<= 8;
        if (x <= 0x0FFFFFFF) n += 4, x <<= 4;
        if (x <= 0x3FFFFFFF) n += 2, x <<= 2;
        if (x <= 0x7FFFFFFF) n += 1;
        return n;
    }

    // 快速傅里叶变换
    static void fast_fourier_transform(std::vector<std::complex<float>> &input_sequence) {
        const uint32_t sequence_size = input_sequence.size();

        // 如果序列长度小于等于1，则不需要继续计算
        if (sequence_size <= 1) return;

        // 缓存计算结果，避免重复计算
        std::vector<std::complex<float>> even_sequence(sequence_size / 2), odd_sequence(sequence_size / 2);
        for (uint32_t i = 0; i < sequence_size / 2; ++i) {
            even_sequence[i] = input_sequence[2 * i];     // 偶数序列
            odd_sequence[i] = input_sequence[2 * i + 1];  // 奇数序列
        }

        // 递归计算偶数和奇数序列的FFT
        fast_fourier_transform(even_sequence);
        fast_fourier_transform(odd_sequence);

        // 合并偶数和奇数序列的FFT结果
        const auto twiddle_factor = std::polar(1.0, -2 * PI / sequence_size);
        std::complex<float> factor(1, 0);
        for (uint32_t i = 0; i < sequence_size / 2; ++i) {
            input_sequence[i] = even_sequence[i] + factor * odd_sequence[i];
            input_sequence[i + sequence_size / 2] = even_sequence[i] - factor * odd_sequence[i];
            factor *= twiddle_factor;  // 更新旋转因子
        }
    }

    // 快速傅里叶逆变换
    static void inverse_fast_fourier_transform(std::vector<std::complex<float>> &input_sequence) {
        // 先计算FFT，然后对结果取共轭并除以n即可得到逆变换的结果
        fast_fourier_transform(input_sequence);
        const uint32_t sequence_size = input_sequence.size();
        for (auto &element : input_sequence) {
            element = std::conj(element) * static_cast<float>(1.0 / sequence_size);  // 取共轭并除以n
        }
    }
};

class DiscreteFourierTransform {
   public:
    std::vector<std::complex<float>> DFT(std::vector<float> input_sequence, bool inverse = false) {
        // 这里将实数域转为复数域;
        std::vector<std::complex<float>> data(input_sequence.begin(), input_sequence.end());

        // 若反转为 false 则进行DFT正变换, 若反转为 true 则进行DFT逆变换;
        if (inverse == false)
            return discrete_fourier_transform(data);
        else
            return inverse_discrete_fourier_transform(data);
    }

   private:
    // 计算离散傅里叶变换
    std::vector<std::complex<float>> discrete_fourier_transform(const std::vector<std::complex<float>> &input_vector) const {
        uint32_t N = input_vector.size();
        std::vector<std::complex<float>> output_vector(N);
        for (uint32_t k = 0; k < N; k++) {
            // 对于每个频率 k，计算对应的和
            for (uint32_t n = 0; n < N; n++) {
                // 对于每个样本 n，计算频率为 k 的分量
                output_vector[k] += input_vector[n] * std::exp(std::complex<float>(0, -2 * M_PI * k * n / N));
            }
        }
        return output_vector;
    }

    // 计算离散傅里叶逆变换
    std::vector<std::complex<float>> inverse_discrete_fourier_transform(const std::vector<std::complex<float>> &input_vector) const {
        uint32_t N = input_vector.size();
        std::vector<std::complex<float>> output_vector(N);
        for (uint32_t n = 0; n < N; n++) {
            // 对于每个样本 n，计算对应的和
            for (uint32_t k = 0; k < N; k++) {
                // 对于每个频率 k，计算样本为 n 的分量
                output_vector[n] += input_vector[k] * std::exp(std::complex<float>(0, 2 * M_PI * k * n / N));
            }
            // 对和进行缩放，得到正确的输出
            output_vector[n] /= N;
        }
        return output_vector;
    }
};