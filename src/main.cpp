// ObjectTrackingCPP.cpp
#include "DataAssociation.h"
#include "BGSDetector.h"
#include "MyTypes.h"
#include "Server.h"



const char * DISPLAY_WINDOW = "Display Window";

int main(void)
{

    try
    {
        Server server(8080);
        Mat img;
        Mat display;
        BGSDetector bgsDetector;
        DataAssociation dataAssociation(60,5,WIDTH,HEIGHT);
        char chCheckForEscKey = 0;

        namedWindow(DISPLAY_WINDOW);

        while (chCheckForEscKey != 27)
        {

            try
            {
                server.receiveBinMask(img);
                // vector<Rect> detections = bgsDetector.detect(img);
                // vector<MatND> histograms = bgsDetector.histograms;
                // dataAssociation.assignTracks(detections,histograms);
                // applyColorMap(img, display, COLORMAP_BONE);
                // vector<Tracker *> &tracks = dataAssociation.getTracks();
                // for (int i = 0; i < tracks.size(); i++)
                // {
                //    if(tracks[i]->totalVisibleCount>2)
                //         rectangle(display, tracks[i]->getTarget().tl(), tracks[i]->getTarget().br(),
                //                   cv::Scalar(0, 255, 0), 2);
                // }

                imshow(DISPLAY_WINDOW,img);

            }
            catch (ReceiveException& e)
            {
                if(e.error!=boost::asio::error::eof)
                    throw e;
                else
                    server.acceptConnection();
                continue;
            }
            chCheckForEscKey = cv::waitKey(1000/FPS);
        }
    }
    catch (std::exception& e)
    {
        cerr << e.what() << endl;
    }

    return(0);
}