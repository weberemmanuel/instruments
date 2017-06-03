// MIT License
//
// Copyright(c) 2017 Emmanuel Weber
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>

#include <iostream>
#include <thread>

#include <boost/asio.hpp>
#include <boost/chrono.hpp>

using namespace std;
using namespace boost::asio;

///////////////////////////////////////////////////////////////////////////////////////////////////
// class SCPIInstrument

class SCPIInstrument
{
public:
	SCPIInstrument(io_service& ioService, const std::string& host, int port, const std::string& separator = "\n")
		: mIoService(ioService)
		, mHost(host)
		, mPort(port)
		, mSocket(ioService)
		, mSeparator(separator)
	{
		char buffer[10];
		ip::tcp::resolver resolver(mIoService);
		ip::tcp::resolver::query query(host, itoa(port, buffer, 10));
		boost::asio::connect(mSocket, resolver.resolve(query));
	}

	string getIdentification(void)
	{
		return sendQuery("*IDN?");
	}

	void clearStatus(void)
	{
		sendCommand("CLS");
	}

	void reset(void)
	{
		sendCommand("*RST");
	}

	bool lockInterface(bool lock)
	{
		stringstream ss;
		ss << "IFLOCK " << int(lock);
		string result(sendQuery(ss.str()));
		return (result == "1");
	}

	bool isInterfaceLocked(void)
	{
		string result(sendQuery("IFLOCK?"));
		return (result == "1"); // MANTODO: theoretically 3 states
	}

	void tripReset(void)
	{
		sendCommand("TRIPRST");
	}

	void sendCommand(const std::string& command)
	{
		std::string response;
		std::string query(command + mSeparator);
		boost::asio::write(mSocket, boost::asio::buffer(query, query.length()));
	}

	std::string sendQuery(const std::string& command)
	{
		std::string response;
		std::string query(command + mSeparator);
		boost::asio::write(mSocket, boost::asio::buffer(query, query.length()));

		size_t bytes_read = boost::asio::read_until(mSocket, mBufferedData, mSeparator);
		if (bytes_read)
		{
			std::istream is(&mBufferedData);
			std::getline(is, response);
		}
		return response;
	}

private:
	io_service&				mIoService;
	const std::string&		mHost;
	int						mPort;
	ip::tcp::socket			mSocket;
	boost::asio::streambuf  mBufferedData;
	std::string             mSeparator;
};

//
///////////////////////////////////////////////////////////////////////////////////////////////////
