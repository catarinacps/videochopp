#include <array>
#include <iostream>
#include <string>

#include <opencv2/opencv.hpp>

#include "operations.hpp"

int main(int argc, char* argv[])
{
    int param = std::stoi(argv[1]);
    auto mode = static_cast<vc::modes>(param > 0 ? param : 0);

    cv::Mat captured, processed;
    std::string cap_name { "Video capture feed" }, proc_name { "Processed feed" };

    cv::VideoCapture feed(0);

    if (!feed.isOpened()) {
        std::cout << "Check if your camera is working or activated\n";

        return -1;
    }

    cv::namedWindow(cap_name, CV_WINDOW_AUTOSIZE);
    cv::namedWindow(proc_name, CV_WINDOW_AUTOSIZE);

    switch (mode) {
    case vc::modes::invalid:
        return -1;
    case vc::modes::gaussian:
        cv::createTrackbar("Convolution Matrix", proc_name, &vc::slider_1, 21, vc::trackbar_gauss);
        break;
    case vc::modes::bc: {
        cv::createTrackbar("Contrast", proc_name, &vc::slider_1, 100, vc::trackbar_contrast);
        cv::createTrackbar("Brightness", proc_name, &vc::slider_2, 100, vc::trackbar_brightness);
        break;
    }
    default:
        return -1;
    }

    while (!(cv::waitKey(1) == 27) and feed.isOpened()) {
        auto key = cv::waitKey(1);
        if (key == 27) {
            break;
        } else {
            mode = static_cast<vc::modes>(key);
        }

        feed >> captured;

        switch (mode) {
        case vc::modes::gaussian:
            cv::GaussianBlur(captured, processed, cv::Size(vc::kernel_size_gauss, vc::kernel_size_gauss), vc::sigma_gauss);
            break;
        case vc::modes::canny:
            cv::Canny(captured, processed, 10, 350);
            break;
        case vc::modes::sobel:
            cv::Sobel(captured, processed, -1, 1, 1);
            break;
        case vc::modes::bc:
            captured.convertTo(processed, -1, vc::alpha_contr, vc::beta_bright);
            break;
        case vc::modes::negative:
            captured.convertTo(processed, -1, -1, 255);
            break;
        case vc::modes::greyscale:
            cv::cvtColor(captured, processed, cv::COLOR_RGB2GRAY);
            break;
        case vc::modes::resize:
            cv::resize(captured, processed, cv::Size(), 0.5, 0.5);
            break;
        case vc::modes::rotate:
            cv::rotate(captured, processed, cv::ROTATE_90_COUNTERCLOCKWISE);
            break;
        case vc::modes::flip:
            cv::flip(captured, processed, 1);
            break;
        default:
            continue;
        }

        cv::imshow(cap_name, captured);
        cv::imshow(proc_name, processed);
    }

    cv::destroyAllWindows();

    return 0;
}
