#pragma once

#include "boost/iostreams/stream.hpp"
#include "boost/asio/serial_port.hpp"

class serial_device
{
public:
	typedef char char_type;
	typedef boost::iostreams::bidirectional_device_tag category;

	serial_device(boost::asio::serial_port& port) : serial_port(port) {};

	std::streamsize read(char* buf, std::streamsize n)
	{
		return serial_port.read_some(boost::asio::buffer(buf, n));
	}
	std::streamsize write(const char* buf, std::streamsize n)
	{
		return serial_port.write_some(boost::asio::buffer(buf, n));
	}

private:
	boost::asio::serial_port& serial_port;
};
