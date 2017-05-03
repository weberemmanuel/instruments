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

#include "stdafx.h"

#include "MX100.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//											MAIN
///////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	try
	{
		io_service my_io_service;
		MX100 mx100tp(my_io_service, "192.168.1.125", 9221);
		cout << "IDN:    " << mx100tp.getIdentification() << std::endl;

		//if (mx100tp.lockInterface(true))
		{
			cout << "CONFIG: " << mx100tp.getConfig() << std::endl;
			mx100tp.setConfig(MX100::Config::Unlinked);
			cout << "CONFIG: " << mx100tp.getConfig() << std::endl;

			mx100tp.channel(MX100::ChannelId::CHAN1).setVoltage(1.001f);
			mx100tp.channel(MX100::ChannelId::CHAN2).setVoltage(2.12f);
			mx100tp.channel(MX100::ChannelId::CHAN3).setVoltage(3.23f);

			cout << "V1: " << mx100tp.channel(MX100::ChannelId::CHAN1).getVoltage() << std::endl;
			cout << "V2: " << mx100tp.channel(MX100::ChannelId::CHAN2).getVoltage() << std::endl;
			cout << "V3: " << mx100tp.channel(MX100::ChannelId::CHAN3).getVoltage() << std::endl;

			mx100tp[MX100::ChannelId::CHAN1].enable(true);
			mx100tp[MX100::ChannelId::CHAN2].enable(true);
			mx100tp[MX100::ChannelId::CHAN3].enable(true);

			this_thread::sleep_for(chrono::seconds(2));

			mx100tp[MX100::ChannelId::CHAN1].enable(false);
			mx100tp[MX100::ChannelId::CHAN2].enable(false);
			mx100tp[MX100::ChannelId::CHAN3].enable(false);

		//	mx100tp.lockInterface(false);
		}
		cout << "LOCAL   " << std::endl;
		mx100tp.sendCommand("LOCAL");
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//											END
///////////////////////////////////////////////////////////////////////////////////////////////////
