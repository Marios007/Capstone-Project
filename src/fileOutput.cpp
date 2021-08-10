#include "fileOutput.h"

//write file
void FileOutput::WriteFile(cv::Mat &frame) 
{
    //TODO if file already exist append a number
    cv::imwrite(_output_path, frame);
}

//set path to store file
FileOutput::FileOutput(std::string &path) 
{
    _output_path = path;
}
