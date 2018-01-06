//
// Created by dilin on 8/11/17.
//

#include "KalmanTracker.h"
KalmanTracker::KalmanTracker(): kf(STATE_SIZE,MEAS_SIZE,0,TYPE),
                                state(STATE_SIZE,1,TYPE),
                                meas(MEAS_SIZE,1,TYPE),
                                found(false)
{
    // Transition State Matrix A
    // Note: set dT at each processing step!
    // [ 1 0 dT 0  0 0 ]
    // [ 0 1 0  dT 0 0 ]
    // [ 0 0 1  0  0 0 ]
    // [ 0 0 0  1  0 0 ]
    // [ 0 0 0  0  1 0 ]
    // [ 0 0 0  0  0 1 ]
    setIdentity(kf.transitionMatrix);

    // Measure Matrix H
    // [ 1 0 0 0 0 0 ]
    // [ 0 1 0 0 0 0 ]
    // [ 0 0 0 0 1 0 ]
    // [ 0 0 0 0 0 1 ]
    kf.measurementMatrix = cv::Mat::zeros(MEAS_SIZE, STATE_SIZE, TYPE);
    kf.measurementMatrix.at<float>(0) = 1.0f;
    kf.measurementMatrix.at<float>(7) = 1.0f;
    kf.measurementMatrix.at<float>(16) = 1.0f;
    kf.measurementMatrix.at<float>(23) = 1.0f;

    // Process Noise Covariance Matrix Q
    // [ Ex   0   0     0     0    0  ]
    // [ 0    Ey  0     0     0    0  ]
    // [ 0    0   Ev_x  0     0    0  ]
    // [ 0    0   0     Ev_y  0    0  ]
    // [ 0    0   0     0     Ew   0  ]
    // [ 0    0   0     0     0    Eh ]
    //cv::setIdentity(kf.processNoiseCov, cv::Scalar(1e-2));
    kf.processNoiseCov.at<float>(0) = 1e-2;
    kf.processNoiseCov.at<float>(7) = 1e-2;
    kf.processNoiseCov.at<float>(14) = 5.0f;
    kf.processNoiseCov.at<float>(21) = 5.0f;
    kf.processNoiseCov.at<float>(28) = 1e-2;
    kf.processNoiseCov.at<float>(35) = 1e-2;

    // Measures Noise Covariance Matrix R
    cv::setIdentity(kf.measurementNoiseCov, cv::Scalar(1e-1));
}

cv::Rect KalmanTracker::getTarget()
{
    double precTick = ticks;
    ticks = (double) cv::getTickCount();

    double dT = (ticks - precTick) / cv::getTickFrequency(); //seconds

    // >>>> Matrix A
    kf.transitionMatrix.at<float>(2) = (float)dT;
    kf.transitionMatrix.at<float>(9) = (float)dT;
    // <<<< Matrix A

    state = kf.predict();
    cv::Rect bbox;
    bbox.x = cvRound(state.at<float>(0) - state.at<float>(4)/2);
    bbox.y = cvRound(state.at<float>(1)- state.at<float>(5)/2);
    bbox.width = cvRound(state.at<float>(4));
    bbox.height = cvRound(state.at<float>(5));

    return bbox;
}

void KalmanTracker::update(cv::Rect &detection)
{
    meas.at<float>(0) = detection.x + detection.width/2;
    meas.at<float>(1) = detection.y + detection.height/2;
    meas.at<float>(2) = (float)detection.width;
    meas.at<float>(3) = (float)detection.height;

    if (!found) // First detection!
    {
        // >>>> Initialization
        kf.errorCovPre.at<float>(0) = 1; // px
        kf.errorCovPre.at<float>(7) = 1; // px
        kf.errorCovPre.at<float>(14) = 1;
        kf.errorCovPre.at<float>(21) = 1;
        kf.errorCovPre.at<float>(28) = 1; // px
        kf.errorCovPre.at<float>(35) = 1; // px

        state.at<float>(0) = meas.at<float>(0);
        state.at<float>(1) = meas.at<float>(1);
        state.at<float>(2) = 0;
        state.at<float>(3) = 0;
        state.at<float>(4) = meas.at<float>(2);
        state.at<float>(5) = meas.at<float>(3);
        // <<<< Initialization

        kf.statePost = state;

        found = true;
    }
    else
        kf.correct(meas); // Kalman Correction
}

cv::Mat KalmanTracker::getState()
{
    return state;
}


