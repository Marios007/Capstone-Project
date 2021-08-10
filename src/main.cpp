#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>
#include "buffer.h"
#include "fileInput.h"
#include "detector.h"
#include "fileOutput.h"

std::string conf_path{"../model/yolov3-tiny.cfg"};
std::string weights_path{"../model/yolov3-tiny.weights"};
std::string classes_path{"../model/coco.names"};
const int windowHeight = 1024;
const int windowWidth = 768;

Buffer<cv::Mat> framebuffer;
std::string image_out = "../result_pic.jpg";

int main(int argc, char **argv)
{
    // define options for command line input
    const cv::String keys =

        "{h help         |      | Command example:\t./openCVdetector --image=../data/bike.jpg}"
        "{c conf         |   .5 | Confidence threshold.}"
        "{n nms          |   .4 | Non-max suppression threshold.}"
        "{input i        |<none>| path for input image. }";

    cv::CommandLineParser parser(argc, argv, keys);

    parser.about("This script is using openCV for object detection in pictures.");

    // if user enters help or add no parameter
    if (parser.has("help") || argc == 1)
    {
        parser.printMessage();
        return 0;
    }

    // read in the threasholds if c or n is added
    float nmsThreshold = parser.get<float>("n");
    std::cout << "nmsThreshold: " << nmsThreshold << std::endl;
    float confThreshold = parser.get<float>("c");
    std::cout << "confThreshold: " << confThreshold << std::endl;

    // Check the  input file. Open a video file or an image file or a camera stream.
    if (parser.has("input"))
    {
        std::string filePath = parser.get<std::string>("input");
        std::ifstream input_stream(filePath);
        if (!input_stream)
        {
            std::cout << "No picture or video recognised\n";
            return 0;
        }
        std::unique_ptr<FileInput> capture = std::make_unique<FileInput>(filePath);
        //create image container
        cv::Mat container;
        capture->GetImageFrame(container);
        std::unique_ptr<Detector> detectedObject = std::make_unique<Detector>(conf_path, classes_path, weights_path, confThreshold, nmsThreshold);
        cv::Mat detected_frame = detectedObject->DetectObjects(container);
        std::unique_ptr<FileOutput> output = std::make_unique<FileOutput>(image_out);
        output->WriteFile(detected_frame);

        //create a window, set size and show. Exit with any button 
        static const std::string windowName = "Detecting objects with openCV";
        namedWindow(windowName, cv::WINDOW_NORMAL);
        cv::resizeWindow(windowName, windowHeight, windowWidth);
        imshow(windowName, container);
        cv::waitKey(0);
        cv::destroyAllWindows();
    }

    else
    {
        std::cout << "Invalid input\n";
    }
}