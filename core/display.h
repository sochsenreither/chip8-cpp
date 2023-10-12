#pragma once

#include <array>

class Display {
   public:
    static constexpr int m_width = 64;
    static constexpr int m_height = 32;
    static constexpr int scale = 10;

    void clear() {
        data.fill(0);
    }

    uint8_t& operator[](int index) {
        return data.at(index);
    }

    static constexpr int width() {
        return m_width * scale;
    }

    static constexpr int height() {
        return m_height * scale;
    }

    private:
    std::array<uint8_t, m_width * m_height> data = {0};
};