//
// Created by dilin on 10/18/17.
//

#include "Server.h"

Server::Server(int port) :
acceptor(io_service, tcp::endpoint(tcp::v4(), port)),
socket(io_service)
{
    acceptor.accept(socket);
    this->port = port;
}

void Server::bitBufferToMat(boost::array<uchar, N> &buffer, Mat &binMask)
{
    int rows = (buffer[1] << 8) + buffer[0];
    int cols = (buffer[3] << 8) + buffer[2];

    assert(rows==HEIGHT && cols==WIDTH);

    if(!binMask.data)
        binMask = Mat(rows,cols,CV_8UC1);

    int bytePos = 4;
    uchar bitPos = 0;
    uchar temp;

    for(int i=0;i<binMask.rows;i++)
    {
        for(int j=0;j<binMask.cols;j++)
        {
            temp = buffer[bytePos];
            temp = (temp & (uchar)(1<<(7-bitPos)))>>(7-bitPos);
            binMask.at<uchar>(i,j) = (uchar)(temp*255);
            bitPos++;
            if(bitPos>=8)
            {
                bitPos = 0;
                bytePos++;
            }
        }
    }
}

void Server::receiveBinMask(Mat &binMask)
{
    boost::system::error_code error;
    size_t bytes_transferred = boost::asio::read(socket,boost::asio::buffer(buffer,N), error);
    if(!error && bytes_transferred==N)
    {
        bitBufferToMat(buffer,binMask);
    }
    else
    {
        throw ReceiveException(bytes_transferred,error);
    }
}

void Server::acceptConnection()
{
    socket.close();
    acceptor.accept(socket);
}
