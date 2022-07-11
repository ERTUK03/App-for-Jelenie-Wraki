#include "SimpleSerial.h"

SimpleSerial::SimpleSerial(std::string port, unsigned int baud_rate) : io(), serial(io, port)
{
    serial.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
}

int SimpleSerial::readLine()
{
    char c;
    int result = 0;
    for (;;)
    {
        boost::asio::read(serial, boost::asio::buffer(&c, 1));
        if (int(c) > 47 && int(c) < 58)
        {
            result *= 10;
            result += int(c) - 48;
        }
        else if (c == '\n') return result;
    }
}
