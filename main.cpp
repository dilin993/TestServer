#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include<boost/array.hpp>
#include<opencv2/opencv.hpp>

#define BUF_SIZE(W,H) (W*H)/8+5
#define N BUF_SIZE(640,480)
#define FPS 30

using boost::asio::ip::tcp;
using namespace cv;


Mat getMatFromBitBuffer(unsigned char *buffer)
{
    int rows = (buffer[1] << 8) + buffer[0];
    int cols = (buffer[3] << 8) + buffer[2];

    Mat binMask(rows,cols,CV_8UC1);

    int bytePos = 4;
    unsigned char bitPos = 0;
    unsigned char temp;

    for(int i=0;i<binMask.rows;i++)
    {
        for(int j=0;j<binMask.cols;j++)
        {
            temp = buffer[bytePos];
            temp = (temp & (unsigned char)(1<<(7-bitPos)))>>(7-bitPos);
            binMask.at<unsigned char>(i,j) = (unsigned char)(temp*255);
            bitPos++;
            if(bitPos>=8)
            {
                bitPos = 0;
                bytePos++;
            }
        }
    }

    return binMask;
}


int main()
{
    try
    {
        const char *BINARY_MASK_R = "Binary Mask Recieved";

        namedWindow(BINARY_MASK_R, CV_WINDOW_AUTOSIZE);

        boost::asio::io_service io_service;
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 8080));
        tcp::socket socket(io_service);
        acceptor.accept(socket);
        for (;;)
        {
            boost::system::error_code error;
            boost::array<unsigned char, N> buf;
            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            std::cout << boost::system::system_error(error).what() << std::endl;

            Mat maskR = getMatFromBitBuffer(buf.c_array());

            imshow(BINARY_MASK_R,maskR);

            waitKey(1000/FPS);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
