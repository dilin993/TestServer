//
// Created by dilin on 10/18/17.
//

#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include "MyTypes.h"

class ReceiveException : public exception
{
public:
    ReceiveException(size_t bytes_transferred,boost::system::error_code error)
    {
        this->bytes_transferred = bytes_transferred;
        this->error = error;
    }

    const char * what () const throw ()
    {
        string errorStr =  to_string(bytes_transferred) +
                " bytes transferred. error code: " +
                boost::system::system_error(error).what();
        return errorStr.c_str();
    }
    size_t bytes_transferred;
    boost::system::error_code error;
};

class Server
{
public:
    Server(int port);
    void receiveBinMask(Mat &binMask);
    void acceptConnection();

private:
    boost::asio::io_service io_service;
    tcp::acceptor acceptor;
    tcp::socket socket;
    boost::array<uchar,N> buffer;
    int port;
    void bitBufferToMat(boost::array<uchar,N> &buffer, Mat &binMask);
};


#endif //SERVER_SERVER_H
