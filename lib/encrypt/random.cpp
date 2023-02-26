/*
< GasSensorOS random >
Copyright(C) < 2023 > <RMSHE>

This program is free software : you can redistribute it and /or modify
it under the terms of the GNU Affero General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.If not, see < https://www.gnu.org/licenses/>.

Electronic Mail : asdfghjkl851@outlook.com
*/

#include <esp_system.h>
#include <random.h>

#include <random>

std::random_device rd;  // 用于获取随机种子

/*
1.函数 trueRandom 接受一个可选参数 length，用于指定要生成的 vector 的长度。如果未提供 length，则默认为 1。
2.函数创建一个名为 vec_xor_random 的 uint8_t 类型的空 vector。
3.函数进入一个循环，直到 vector 的长度等于或大于请求的 length，生成随机数并将其添加到 vec_xor_random vector 中。
4.在循环内部，函数使用 esp_random() 函数生成一个真随机数。
5.然后，使用另一个循环，将真随机数逐个数字地处理，重复地将该数字除以 10，直到数字变成零。
6.对于真随机数的每个数字，函数使用 Mersenne Twister 算法和一个范围为 [0,9] 的均匀分布函数生成一个伪随机数。
7.然后，将伪随机数与真随机数进行异或运算，并对结果进行模 10 运算，以获取范围为 [0,9] 的随机数。
8.将得到的随机数添加到 vec_xor_random vector 中。
9.一旦 vec_xor_random vector 的长度等于或大于请求的 length，函数将创建一个名为 slice 的新 vector，该 vector 包含使用取两个迭代器的 vector 构造函数获得的
vec_xor_random vector 的前 length 个元素。
10.函数返回包含请求数量的真随机数的 slice vector。
*/

/**
 * @brief 生成指定长度的真随机数.
 * @param length 真随机数的长度(默认为1)
 * @return vector<uint8_t>
 */
std::vector<uint8_t> trueRandom(uint16_t length) {
    std::vector<uint8_t> vec_xor_random;

    // 不断的循环生成随机数直到符合或超出用户的长度要求 length 为止;
    do {
        // 生成真随机数，范围为[0, UINT32_MAX]
        uint32_t true_random = esp_random();

        // 根据真随机数的长度逐位生成伪随机数并与之进行异或运算后模10(%10运算保证了任意两个 0 到 9 的整数做运算后仍在 0 到 9 区间内),
        // 最后将 (true XOR pseudo)%10 得到的[0, 9]区间内的随机数添加到 vector<uint8_t> vec_xor_random 中;
        do {
            vec_xor_random.push_back(((true_random % 10) ^ []() -> uint32_t {
                                         std::mt19937 gen(rd());                              // 使用Mersenne Twister作为随机数生成器
                                         std::uniform_int_distribution<uint32_t> dist(0, 9);  // 均匀分布函数，范围为[0, 9]
                                         return dist(gen);                                    // 生成伪随机数，并返回
                                     }()) %                                                   // 对异或运算后的结果模10
                                     10);
        } while (true_random /= 10);
    } while (vec_xor_random.size() < length);

    // 从 vec_xor_random 中取出前 length 个元素
    std::vector<uint8_t> slice(vec_xor_random.begin(), vec_xor_random.begin() + length);
    return slice;
}