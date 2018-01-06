//
// Created by dilin on 8/11/17.
//

#ifndef PEOPLETECKERV2_KALMANTRACKER_H
#define PEOPLETECKERV2_KALMANTRACKER_H
#include "Tracker.h"

#define STATE_SIZE 6
#define MEAS_SIZE 4


class KalmanTracker : public  Tracker
{
public:
    KalmanTracker();
    cv::Rect getTarget();
    void update(cv::Rect& bbox);
    cv::Mat getState();
private:
    static const int TYPE = CV_32F;
    cv::KalmanFilter kf;
    cv::Mat state;  // [x,y,v_x,v_y,w,h]
    cv::Mat meas;    // [z_x,z_y,z_w,z_h]
    double ticks = 0;
    bool found;
};


#endif //PEOPLETECKERV2_KALMANTRACKER_H
