/**
 * @file ring_buffer.h
 * @date 13.05.2025
 * @date 2025-05-13
 * @author RMSHE
 *
 * < GasSensorOS >
 * Copyright(C) 2025 RMSHE. All rights reserved.
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

#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <cstddef>
#include <stdexcept>

/**
 * @class RingBuffer
 * @brief 通用环形缓冲区容器
 *
 * @tparam T        存储元素类型
 * @tparam Capacity 缓冲区容量，必须大于0
 *
 * @details
 * - 固定容量，编译期分配，无堆内存
 * - 自动覆盖最旧数据，保障持续写入
 * - 提供随机访问、首/尾访问及插入、弹出接口
 * - 适合 ESP32 等内存受限环境
 */
template <typename T, std::size_t Capacity>
class RingBuffer {
    static_assert(Capacity > 0, "RingBuffer 容量必须大于0");

   public:
    /**
     * @brief 构造函数，初始化缓冲区
     */
    RingBuffer() noexcept : head_(0), tail_(0), count_(0) {}

    /**
     * @brief 将元素推入缓冲区尾部
     * @param value 要插入的元素
     * @note 若缓冲区已满，则按插入顺序覆盖最旧元素
     */
    inline void pushBack(const T& value) noexcept {
        buffer_[tail_] = value;
        tail_ = (tail_ + 1) % Capacity;
        if (count_ < Capacity) {
            ++count_;
        } else {
            head_ = (head_ + 1) % Capacity;
        }
    }

    /**
     * @brief 从尾部弹出最新元素(删除缓冲区中最新的元素)
     * @return 是否成功弹出（非空时成功）
     */
    inline bool popBack() noexcept {
        if (empty()) return false;
        tail_ = (tail_ + Capacity - 1) % Capacity;
        --count_;
        return true;
    }

    /**
     * @brief 从头部弹出最旧元素(删除缓冲区中最旧的元素)
     * @return 是否成功弹出（非空时成功）
     */
    inline bool popFront() noexcept {
        if (empty()) return false;
        head_ = (head_ + 1) % Capacity;
        --count_;
        return true;
    }

    /**
     * @brief 获取最旧的元素
     * @return 最旧元素值
     * @throws std::out_of_range 若缓冲区为空
     */
    inline T& front() {
        if (empty()) throw std::out_of_range("RingBuffer::front: 缓冲区为空");
        return buffer_[head_];
    }

    /**
     * @brief 获取最新的元素
     * @return 最新元素值
     * @throws std::out_of_range 若缓冲区为空
     */
    inline T& back() {
        if (empty()) throw std::out_of_range("RingBuffer::back: 缓冲区为空");
        return buffer_[(tail_ + Capacity - 1) % Capacity];
    }

    /**
     * @brief 随机访问元素
     * @param index 位置索引（0 为最旧元素，size()-1 为最新元素）
     * @return 索引的元素值
     * @throws std::out_of_range 索引越界
     */
    inline T& at(std::size_t index) {
        if (index >= count_) throw std::out_of_range("RingBuffer::at: 索引越界");
        return buffer_[(head_ + index) % Capacity];
    }

    /**
     * @brief 清空缓冲区
     */
    inline void clear() noexcept { head_ = tail_ = count_ = 0; }

    /**
     * @brief 获取当前储存的元素数量
     * @return 当前存储的元素数量
     */
    inline std::size_t size() const noexcept { return count_; }

    /**
     * @brief 获取缓冲区的容量
     * @return 缓冲区最大容量
     */
    inline constexpr std::size_t capacity() const noexcept { return Capacity; }

    /**
     * @brief 判断缓冲区是否为空
     * @return true: 为空; false: 非空
     */
    inline bool empty() const noexcept { return count_ == 0; }

    /**
     * @brief 判断缓冲区是否已满
     * @return true: 已满; false: 未满
     */
    inline bool full() const noexcept { return count_ == Capacity; }

   private:
    T buffer_[Capacity];  ///< 数据存储
    std::size_t head_;    ///< 最旧元素索引
    std::size_t tail_;    ///< 下一个写入位置
    std::size_t count_;   ///< 当前元素数量
};

#endif  // RINGBUFFER_H
