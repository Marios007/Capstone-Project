#include "fileInput.h"

FileInput::FileInput(std::string &path) {
    end_of_file = false;
    std::ifstream image_stream(path);
    if (image_stream) {
        cap.open(path);
    }
    else {
        throw std::runtime_error("[ERROR:] Cannot open input file\n");
    }
}

bool FileInput::GetImageFrame(cv::Mat &frame) {
    if (!FileInput::EndOfFile()) {
        cap >> frame;
    }
    end_of_file = true;
    return !FileInput::EndOfFile();
}


bool FileInput::EndOfFile() {
    return end_of_file;
}

FileInput::~FileInput() {
    cap.release();
}


