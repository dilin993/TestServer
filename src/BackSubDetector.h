//
// Created by dilin on 10/18/17.
//

#ifndef SERVER_BACKSUBDETECTOR_H
#define SERVER_BACKSUBDETECTOR_H

#include "Detector.h"

class BackSubDetector : Detector
{
    void detect(cv::Mat &img);
};


#endif //SERVER_BACKSUBDETECTOR_H
