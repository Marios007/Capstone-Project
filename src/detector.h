#pragma once

#include <fstream>
//#include <iostream>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

class Detector
{
    //thresholds
    float confThreshold; 
    float nmsThreshold;  

    // size of networks input image
    const int inpWidth = 416; 
    const int inpHeight = 416; 
    
public:
    Detector() = delete;
    //default comstructor
    Detector(std::string &model_config, std::string &classes_file, std::string &model_weights,  float &confThreshold, float &nmsThreshold); 
    //copy constructor
    Detector(const Detector &detector);
    //copy asignment operator
    Detector &operator=(const Detector &detector);
    //move constructor
    Detector(Detector &&detector); 
    // move assignment operator
    Detector &operator=(Detector &&detector);
    //destructor
    ~Detector() {}
    cv::Mat DetectObjects(cv::Mat &frame);

private:
    cv::dnn::Net _net;
    std::string _modelConfig = "";
    std::string _modelWeights = "";
    std::string _classesFile = "";

    void getOutputLayerNames();
    void postProcessing(cv::Mat &frame, const std::vector<cv::Mat> &outs, float &confThreshold);
    void drawPred(int classId, float conf, int left, int top, int right, int bottom, cv::Mat &frame);
    void readClasses();

    static std::vector<std::string> _outNames;
    static std::vector<std::string> _classes;
};