//
// Created by dilin on 8/11/17.
//

#ifndef PEOPLETECKERV2_DETECTOR_H
#define PEOPLETECKERV2_DETECTOR_H

#include <iostream>
#include <opencv2/opencv.hpp>

class Detector
{
public:
    virtual void detect(cv::Mat &img) = 0;
    std::vector<cv::Rect> getDetections();
    std::vector<cv::MatND> getHistograms();
    static cv::MatND getHistogram(cv::Mat img,cv::Rect region);

private:
    std::vector<cv::Rect> detections;
    std::vector<cv::MatND> histograms;
};


#endif //PEOPLETECKERV2_DETECTOR_H
