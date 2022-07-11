#pragma once

#include <boost/asio.hpp>
#include <vector>

class SimpleSerial
{
private:
    boost::asio::io_service io;
    boost::asio::serial_port serial;

public:
    SimpleSerial(std::string port, unsigned int baud_rate);
    int readLine();
};

