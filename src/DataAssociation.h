//
// Created by dilin on 8/13/17.
//

#ifndef PEOPLETRACKINGSYSTEM_DATAASSOCIATION_H
#define PEOPLETRACKINGSYSTEM_DATAASSOCIATION_H

#define VEL_TH 60
#include "Detector.h"
#include "Tracker.h"
#include "KalmanTracker.h"
#include<iostream>
#include <opencv2/opencv.hpp>
#include "hungarian.h"

using namespace std;
using  namespace cv;

class DataAssociation
{
public:
    DataAssociation(double TRACK_INIT_TH,int REJ_TOL,int width,int height);
    vector<Tracker*> & getTracks();
    void assignTracks(vector<Rect> detections,vector<MatND> histograms);
    ~DataAssociation();
    void setSize(int width,int height);
private:
    vector<Tracker*> tracks;
    double TRACK_INIT_TH;
    double REJ_TOL;
    int width;
    int height;
    double averageError(Rect a, Rect b);
    double averageError(Rect a, Rect b,MatND hisA,MatND histB,Mat state);
};


#endif //PEOPLETRACKINGSYSTEM_DATAASSOCIATION_H
