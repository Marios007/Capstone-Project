#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

//Class to export and store file 
class FileOutput {

public:
    //Constructor
    FileOutput(){};
    //Constructor with path 
    FileOutput(std::string &output_path); 
    // Destructor
    ~FileOutput(){}; 
    //Methods
    void WriteFile(cv::Mat &frame); 

protected:
    std::string _output_path = ""; //path default empty
};
