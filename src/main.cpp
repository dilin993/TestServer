#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include<boost/array.hpp>
#include<opencv2/opencv.hpp>

#include "MyTypes.h"
#include "Server.h"


using boost::asio::ip::tcp;
using namespace cv;

const char *BINARY_MASK_R = "Binary Mask Received";


int main()
{
    try
    {
        boost::asio::io_service io_service;


        tcp_server server(io_service,8080);
        io_service.run();

        namedWindow(BINARY_MASK_R, CV_WINDOW_AUTOSIZE);

        Mat binMask;

        while(true)
        {
            std::cout << "Connection count: " << server.connection_count() << std::endl;
            binMask = server.getRecivedMask(0);
            imshow(BINARY_MASK_R,binMask);
            waitKey(1000/FPS);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
