//
// Created by dilin on 8/11/17.
//

#include "Detector.h"

cv::MatND Detector::getHistogram(cv::Mat img, cv::Rect region)
{
    cv::Mat hsv_base;
    cv::cvtColor(img(region), hsv_base, cv::COLOR_BGR2HSV);

    cv::MatND hist_base;
    /// Using 50 bins for hue and 60 for saturation
    int h_bins = 50; int s_bins = 60;
    int histSize[] = { h_bins, s_bins };

    // hue varies from 0 to 179, saturation from 0 to 255
    float h_ranges[] = { 0, 180 };
    float s_ranges[] = { 0, 256 };

    const float* ranges[] = { h_ranges, s_ranges };

    // Use the o-th and 1-st channels
    int channels[] = { 0, 1 };


    calcHist( &hsv_base, 1, channels, cv::Mat(), hist_base, 2, histSize, ranges, true, false );
    normalize( hist_base, hist_base, 0, 1, cv::NORM_MINMAX, -1, cv::Mat() );

    return hist_base;
}

std::vector<cv::Rect> Detector::getDetections()
{
    return detections;
}

std::vector<cv::MatND> Detector::getHistograms()
{
    return histograms;
}
