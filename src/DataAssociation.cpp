//
// Created by dilin on 8/13/17.
//

#include "DataAssociation.h"

DataAssociation::DataAssociation(double TRACK_INIT_TH,int REJ_TOL,
                                int width,int height):
        TRACK_INIT_TH(TRACK_INIT_TH),
        REJ_TOL(REJ_TOL),
        width(width),
        height(height)
{

}



DataAssociation::~DataAssociation()
{
    // Garbage Collection
    for(int i=0;i<tracks.size();i++)
    {
        delete tracks[i];
    }
}

vector<Tracker *> &DataAssociation::getTracks()
{
    return tracks;
}

void DataAssociation::assignTracks(vector<Rect> detections,vector<MatND> histograms)
{
    vector<vector<double>> costMat;

    for(int i=0;i<detections.size();i++)
    {
        if(tracks.size()==0) // initialize the first track
        {
            Tracker *tr = new KalmanTracker();
            tr->update(detections[i]);
            tr->updateAssociation(true);
            tr->updateHistogram(histograms[i]);
            tracks.push_back(tr);
        }
        else // calculate cost matrix
        {
            double min_e = DBL_MAX;
            vector<double> row(tracks.size());
            row.clear();
            for (int j = 0; j < tracks.size(); j++)
            {
                double e = averageError(detections[i],tracks[j]->getTarget(),
                                        histograms[i],tracks[j]->histogram,
                                        tracks[j]->getState());
                if (e < min_e)
                    min_e = e;
                row.push_back(e);
            }
            if (min_e > TRACK_INIT_TH) // initialize new track
            {
                Tracker *tr = new KalmanTracker();
                tr->update(detections[i]);
                tr->updateAssociation(true);
                tr->updateHistogram(histograms[i]);
                tracks.push_back(tr);
                double e = averageError(detections[i],tr->getTarget(),
                                        histograms[i],tr->histogram,
                                        tr->getState());
                row.push_back(e);
            }
            costMat.push_back(row);
        }
    }

//    cout << "cosMat.size() = " << costMat.size() << endl;

    // data association using hungarian algorithm
    if(costMat.size()>0)
    {
        HungarianAlgorithm hungarianAlgorithm;
        vector<int> assignment;
        double cost = hungarianAlgorithm.Solve(costMat,assignment);
        vector<bool> hasAssigned;

        // initialize hasAssigned to false
        for(int j=0;j<tracks.size();j++)
        {
            hasAssigned.push_back(false);
        }

        // assign detections
        for(int i=0;i<assignment.size();i++)
        {
            int j = assignment[i];
            if(j>=0)
            {
                tracks[j]->update(detections[i]); // assign detction i to track j
                tracks[j]->updateHistogram(histograms[i]);
                hasAssigned[j] = true;
            }
        }

        // delete unnecessary tracks
        cout << "current tracks: " << endl;
        for(int j=0;j<tracks.size();j++)
        {
            tracks[j]->updateAssociation(hasAssigned[j]);
            Mat state = tracks[j]->getState();
            float v = (float)sqrt(pow((float)(state.at<float>(2)),2.0)
                                  + pow((float)(state.at<float>(3)),2.0));
            cout << "[" << j << "]";
            cout << "\tx=" << state.at<float>(0);
            cout << " y=" << state.at<float>(1);
            cout << " vx=" << state.at<float>(2);
            cout << " vy=" << state.at<float>(3);
            cout << " w=" << state.at<float>(4);
            cout << " h=" << state.at<float>(5);
            cout << " v=" << v;
            cout << " assigned=" << hasAssigned[j] << endl;

            if(v>VEL_TH ||
                    state.at<float>(0) > width ||
                    state.at<float>(1) > height ||
                    state.at<float>(0) < 0 ||
                    state.at<float>(1) < 0 ||
               tracks[j]->consectiveInvisibleCount > REJ_TOL)
            {
                cout << "deleted track : " << j << " , size : " << tracks.size() << endl;
                //Tracker *tr = tracks[j];
                tracks.erase(tracks.begin()+j);
                //delete tr;
                j--;
            }
        }

    }
}

double DataAssociation::averageError(Rect a, Rect b)
{
    double de = sqrt(pow((double)(a.x-b.x),2.0) + pow((double)(a.y-b.y),2.0));
    double ds = sqrt(pow((double)(a.width-b.width),2.0) + pow((double)(a.height-b.height),2.0));
    return 0.9 * de + 0.1 * ds;
}

double DataAssociation::averageError(Rect a, Rect b, MatND histA, MatND histB,Mat state)
{
//    double dh = compareHist(histA,histB,HISTCMP_CORREL);
//    dh = 10.0 / (dh+1e-10);
    double e = averageError(a,b);
    return  e;
}

void DataAssociation::setSize(int width, int height)
{
    this->width = width;
    this->height = height;
}
