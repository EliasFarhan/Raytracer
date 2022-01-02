#pragma once
#include <array>
#include <string_view>
#include "color.h"

namespace ray
{
    template<int width, int height>
    class Image {
        public:
            [[nodiscard]] Color& GetColor(int x, int y);
            [[nodiscard]] Color GetColor(int x, int y) const;
            void WritePng(std::string_view path);
        private:
            std::array<std::array<Color, width>, height> image;

    };

    template <int width, int height>
    Color& Image<width, height>::GetColor(int x, int y)
    {
        return image[y][x];
    }

    template <int width, int height>
    Color Image<width, height>::GetColor(int x, int y) const
    {
        return image[y][x];
    }

    template <int width, int height>
    void Image<width, height>::WritePng(std::string_view path)
    {
        stbi_flip_vertically_on_write(1);
        stbi_write_png(path.data(), width, height, 3, image.data(), width * sizeof(ray::Color));
    }
} // namespace ray
