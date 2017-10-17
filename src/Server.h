//
// Created by dilin on 10/17/17.
//

#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include<opencv2/opencv.hpp
#include "MyTypes.h"

using boost::asio::ip::tcp;


cv::Mat getMatFromBitBuffer(std::array<uchar,N> buffer)
{
    int rows = (buffer[1] << 8) + buffer[0];
    int cols = (buffer[3] << 8) + buffer[2];

    assert(rows>=0 && cols>=0);

    cv::Mat binMask(rows,cols,CV_8UC1);

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

class tcp_connection
        : public boost::enable_shared_from_this<tcp_connection>
{
public:
    typedef boost::shared_ptr<tcp_connection> pointer;

    static pointer create(boost::asio::io_service& io_service)
    {
        return pointer(new tcp_connection(io_service));
    }

    tcp::socket& socket()
    {
        return socket_;
    }

    void start()
    {
        boost::asio::async_read(socket_, boost::asio::buffer(bytes),
                                 boost::bind(&tcp_connection::handle_read, shared_from_this(),
                                             boost::asio::placeholders::error,
                                             boost::asio::placeholders::bytes_transferred));
    }

    cv::Mat binMask;

private:
    tcp_connection(boost::asio::io_service& io_service)
            : socket_(io_service)
    {
    }

    void handle_read(const boost::system::error_code& error,
                      size_t bytes_transferred)
    {
        if(!error)
        {
            binMask = getMatFromBitBuffer(bytes);
            boost::asio::async_read(socket_, boost::asio::buffer(bytes),
                                    boost::bind(&tcp_connection::handle_read, shared_from_this(),
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred));
        }
    }

    tcp::socket socket_;
    std::string message_;
    std::array<uchar, N> bytes;
};


class tcp_server
{
public:
    tcp_server(boost::asio::io_service& io_service,int port)
            : acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
    {
        start_accept();
    }

private:
    void start_accept()
    {
        tcp_connection::pointer new_connection =
                tcp_connection::create(acceptor_.get_io_service());

        acceptor_.async_accept(new_connection->socket(),
                               boost::bind(&tcp_server::handle_accept, this, new_connection,
                                           boost::asio::placeholders::error));
    }

    void handle_accept(tcp_connection::pointer new_connection,
                       const boost::system::error_code& error)
    {
        if (!error)
        {
            new_connection->start();
        }

        start_accept();
    }

    tcp::acceptor acceptor_;
};


#endif //SERVER_SERVER_H
