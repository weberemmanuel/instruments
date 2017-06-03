#pragma once
#include "instrument.h"

#include <cstdint>

#include <iostream>
#include <thread>

#include <boost/asio.hpp>
#include <boost/chrono.hpp>

using namespace std;
using namespace boost::asio;

///////////////////////////////////////////////////////////////////////////////////////////////////
// class MX100

class MX100 : public SCPIInstrument
{
public:
	class Channel;

	enum class Config : int8_t
	{
		V2RatioTrack = 0,
		Unusued = 1,
		Unlinked = 2,
		CombinedVoltage = 3,
		CombinedCurrent = 4,
		AnalogControl = -1,
	};

	enum class ChannelId : uint8_t
	{
		CHAN1 = 0,
		CHAN2 = 1,
		CHAN3 = 2,
	};

	MX100(io_service& ioService, const std::string& host, int port)
		: SCPIInstrument(ioService, host, port,"\r\n")
		, m_Channel{ { *this,1 },{ *this,2 },{ *this,3 } }
	{
	}

	void setConfig(Config cfg)
	{
		stringstream ss;
		ss << "CONFIG" << " " << uint8_t(cfg);
		sendCommand(ss.str());
	}

	int getConfig()
	{
		string result(sendQuery("CONFIG?"));
		/*	std::istringstream iss(result);
		std::string channel, value;
		iss >> channel;
		iss >> value;*/
		return std::stoi(result);
	}

	MX100::Channel& channel(ChannelId id) {
		return m_Channel[int(id)];
	}

	MX100::Channel& operator[](ChannelId id) {
		return m_Channel[int(id)];
	}

	class Channel
	{
		Channel(SCPIInstrument& instrument, int n)
			: mInstrument(instrument)
			, mChannelNumber(n)
		{
		}

	public:
		void enable(bool enable)
		{
			stringstream ss;
			ss << "OP" << mChannelNumber << " " << int(enable);
			mInstrument.sendCommand(ss.str());
		}

		bool isEnabled(void)
		{
			stringstream ss;
			ss << "OP" << mChannelNumber << "?";
			string result(mInstrument.sendQuery(ss.str()));
			return (result == "1");
		}

		void setVoltage(float voltage)
		{
			stringstream ss;
			ss << "V" << mChannelNumber << " " << voltage;
			mInstrument.sendCommand(ss.str());
		}

		float getVoltage(void)
		{
			stringstream ss;
			ss << "V" << mChannelNumber << "?";
			string result(mInstrument.sendQuery(ss.str()));
			std::istringstream iss(result);
			std::string channel, value;
			iss >> channel;
			iss >> value;
			return std::stof(value);
		}

	private:
		SCPIInstrument&			mInstrument;
		int						mChannelNumber;
		friend class MX100;
	};

private:
	MX100::Channel	m_Channel[3];
};

//
///////////////////////////////////////////////////////////////////////////////////////////////////