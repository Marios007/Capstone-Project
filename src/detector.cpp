#include "detector.h"

std::vector<std::string> Detector::_outNames;
std::vector<std::string> Detector::_classes;

//default comstructor
Detector::Detector(std::string &model_config, std::string &classes_file, std::string &model_weights,  float &confThresholdIn, float &nmsThresholdIn)
{
    _modelConfig = model_config;
    _classesFile = classes_file;
    _modelWeights = model_weights;
    confThreshold = confThresholdIn;
    nmsThreshold = nmsThresholdIn;

    _net = cv::dnn::readNetFromDarknet(_modelConfig, _modelWeights);
    _net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    _net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    getOutputLayerNames();
    readClasses();
}

//copy constructor
Detector::Detector(const Detector &detector)
{
    _modelConfig = detector._modelConfig;
    _classesFile = detector._classesFile;
    _modelWeights = detector._modelWeights;
    _net = detector._net;
}

//copy asignment operator
Detector &Detector::operator=(const Detector &detector)
{
    if (this == &detector)
    {
        return *this;
    }
    _modelConfig = detector._modelConfig;
    _classesFile = detector._classesFile;
    _modelWeights = detector._modelWeights;
    
    _net = detector._net;

    return *this;
}

//move constructor
Detector::Detector(Detector &&detector)
{
    _modelConfig = std::move(detector._modelConfig);
    _classesFile = std::move(detector._classesFile);
    _modelWeights = std::move(detector._modelWeights);
    _net = std::move(detector._net);
}

// move assignment operator
Detector &Detector::operator=(Detector &&detector)
{
    if (this == &detector)
    {
        return *this;
    }
    _modelConfig = std::move(detector._modelConfig);
    _classesFile = std::move(detector._classesFile);
    _modelWeights = std::move(detector._modelWeights);
    _net = std::move(detector._net);

    return *this;
}

cv::Mat Detector::DetectObjects(cv::Mat &frame)
{
    cv::Mat blob;
    cv::dnn::blobFromImage(frame, blob, 1 / 255.0, cv::Size(inpWidth, inpHeight), cv::Scalar(0, 0, 0), true, false);
    _net.setInput(blob);
    std::vector<cv::Mat> outBlobs;
    _net.forward(outBlobs, _outNames);
    postProcessing(frame, outBlobs, confThreshold);
    cv::Mat detected_frame;
    frame.convertTo(detected_frame, CV_8U);
    return std::move(detected_frame);
}

// Remove bounding box with low confidence
void Detector::postProcessing(cv::Mat &frame, const std::vector<cv::Mat> &outs, float &confThreshold)
{
    std::vector<int> classIds;
    std::vector<float> confidencesVect;
    std::vector<cv::Rect> boxesVect;

    for (auto i = 0; i < outs.size(); ++i)
    {

        float *data = (float *)outs[i].data;
        for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
        {
            cv::Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
            double confidence;
            cv::Point classIdPoint;
            // Get  value and location of  maximum score
            minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
            if (confidence > confThreshold)
            {
                int centerX = (int)(data[0] * frame.cols);
                int centerY = (int)(data[1] * frame.rows);
                int width = (int)(data[2] * frame.cols);
                int height = (int)(data[3] * frame.rows);
                int left = centerX - width / 2;
                int top = centerY - height / 2;

                classIds.push_back(classIdPoint.x);
                confidencesVect.push_back((float)confidence);
                //std::cout << "left: " << left << " top: "<< top << " width: " << width << " height: " <<  height << std::endl;
                boxesVect.push_back(cv::Rect(left, top, width, height));
            }
        }
    }

    // Non-Max Supression
    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxesVect, confidencesVect, confThreshold, nmsThreshold, indices);
    for (size_t i = 0; i < indices.size(); ++i)
    {
        int idx = indices[i];
        cv::Rect box = boxesVect[idx];
        drawPred(classIds[idx], confidencesVect[idx], box.x, box.y, box.x + box.width, box.y + box.height, frame);
    }
}

void Detector::drawPred(int classId, float conf, int left, int top, int right, int bottom, cv::Mat &frame)
{
    // Draw a rectangle displaying the bounding box
    // Color: 0, 0, 255 == red / Blue, Green, Red 
    cv::rectangle(frame, cv::Point(left, top), cv::Point(right, bottom), cv::Scalar(0, 0, 255), 4);

    // Get the label and the confidence
    std::string classLabel = cv::format("%.2f", conf);
    if (!_classes.empty())
    {
        CV_Assert(classId < (int)_classes.size());
        classLabel = _classes[classId] + ":" + classLabel;
    }

    //Display the label and set font and size and position
    int baseLine;
    cv::Size labelSize = cv::getTextSize(classLabel, cv::FONT_HERSHEY_SIMPLEX , 1.0, 1, &baseLine);
    top = cv::max(top, labelSize.height);
    cv::rectangle(frame, cv::Point(left, top - round(1.5 * labelSize.height)), cv::Point(left + round(1.1 * labelSize.width), top + baseLine), cv::Scalar(255, 255, 255), cv::FILLED);
    cv::putText(frame, classLabel, cv::Point(left, top), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 1);
}

// Open and read the class file
void Detector::readClasses()
{
    std::ifstream stream(_classesFile.c_str());
    if (!stream.is_open())
        CV_Error(cv::Error::StsError, "File (" + _classesFile + ") not found.");
    std::string line;
    while (std::getline(stream, line))
    {
        _classes.push_back(line);
    }
}

void Detector::getOutputLayerNames()
{
    if (_outNames.empty())
    {
        std::vector<int> out_layers = _net.getUnconnectedOutLayers();
        std::vector<std::string> layer_names = _net.getLayerNames();
        _outNames.resize(out_layers.size());
        for (std::size_t i = 0; i < out_layers.size(); i++)
        {
            _outNames.at(i) = layer_names[out_layers[i] - 1];
        }
    }
}