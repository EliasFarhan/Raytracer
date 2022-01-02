#include "color.h"
#include <array>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

int main() {

    // Image

    constexpr int image_width = 256;
    constexpr int image_height = 256;

    // Render
    std::array<std::array<ray::Color, image_width>, image_height> image;

    for (int j = image_height-1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {
            const auto r = static_cast<double>(i) / (image_width-1);
            const auto g = static_cast<double>(j) / (image_height-1);
            const auto b = 0.25;
            auto& pixel = image[j][i];
            pixel.r = static_cast<std::uint8_t>(255.999 * r);
            pixel.g = static_cast<std::uint8_t>(255.999 * g);
            pixel.b = static_cast<std::uint8_t>(255.999 * b);
        }
    }
    stbi_flip_vertically_on_write(1);
    stbi_write_png("output_01.png", image_width, image_height, 3, image.data(), image_width*sizeof(ray::Color));
    
}