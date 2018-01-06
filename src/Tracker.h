//
// Created by dilin on 8/11/17.
//

#ifndef PEOPLETECKERV2_TRACKER_H
#define PEOPLETECKERV2_TRACKER_H

#include <iostream>
#include <opencv2/opencv.hpp>

class Tracker
{
public:
    virtual void update(cv::Rect &bbox) = 0;
    void updateAssociation(bool detectionAssociated);
    void updateHistogram(cv::MatND histogram);
    virtual cv::Rect getTarget() = 0;
    long age=0;
    long totalVisibleCount=0;
    long consectiveInvisibleCount=0;
    cv::MatND histogram;
    virtual cv::Mat getState()=0;  // [x,y,v_x,v_y,w,h]
protected:
    bool prevDetectionAssociated= false;
};


#endif //PEOPLETECKERV2_TRACKER_H
