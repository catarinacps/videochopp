#pragma once

namespace vc {

enum class modes {
    invalid,
    gaussian,
    canny,
    sobel,
    bc,
    negative,
    greyscale,
    resize,
    rotate,
    flip
};

auto slider_1 = 0;
auto slider_2 = 0;
auto kernel_size_gauss = 3;
auto sigma_gauss = 0.3 * ((kernel_size_gauss - 1) * 0.5 - 1) + 0.8;

double alpha_contr, beta_bright;

void trackbar_gauss(int, void*) {
    kernel_size_gauss = 2 * slider_1 + 1;
    sigma_gauss = 0.3 * ((kernel_size_gauss - 1) * 0.5 - 1) + 0.8;
}

void trackbar_contrast(int, void*) {
    alpha_contr = slider_1;
}

void trackbar_brightness(int, void*) {
    beta_bright = slider_2;
}
}