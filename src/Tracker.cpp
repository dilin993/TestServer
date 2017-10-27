//
// Created by dilin on 8/11/17.
//

#include "Tracker.h"

void Tracker::updateAssociation(bool detectionAssociated)
{
    age++;
    if(detectionAssociated)
    {
        totalVisibleCount += 1;
    }
    else if(!prevDetectionAssociated)
    {
        consectiveInvisibleCount++;
    }

    prevDetectionAssociated = detectionAssociated;
}

void Tracker::updateHistogram(cv::MatND histogram)
{
#define ALPHA 0.5
    if(this->histogram.dims==histogram.dims &&
            this->histogram.rows==histogram.rows &&
            this->histogram.cols==histogram.cols)
        this->histogram = ALPHA*(this->histogram) + (1-ALPHA)*histogram;
    else
        this->histogram = histogram;
}
