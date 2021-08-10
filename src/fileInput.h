#pragma once

#include <fstream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

//Base class to get image input.
class FileInput
{
public:
    //Constructor
    FileInput(std::string &path);
    // extract frame from image
    bool GetImageFrame(cv::Mat &frame); 
    //parse end of  file
    bool EndOfFile(); 
    //Destructor
    ~FileInput();

protected:
    bool end_of_file;  
    cv::VideoCapture cap; 
};
