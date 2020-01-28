#include "../include/websocket.hpp"

#include <memory>

int main (void)
{
	INFO << "program started";

	int command = 0;

	try
	{
		// configuration
		websocket _websocket;
		std::string wss = "http://10.0.0.119:8090";

		INFO << "initialize websocket server";
		_websocket.initialize(wss);

		loop:
		std::cout << "********************************************" << std::endl; 
		std::cout << "* 1. emit station_closed" << std::endl;
		std::cout << "* 2. emit station_open" << std::endl;
		std::cout << "* 3. emit flow_updated" << std::endl;
		std::cout << "* Any Other Key >> Quit" << std::endl;
		std::cout << "********************************************" << std::endl; 
		std::cin >> command;
		switch (command)
		{
			case 1:	// station_closed
			{
				INFO << "emit station_closed";
				_websocket.emit_event("station_closed");
				break;
			}
			case 2:	// station_open
			{
				INFO << "emit station_open";
				_websocket.emit_event("station_open");
				break;
			}
			case 3:	// emit flow_updated
			{
				INFO << "emit flow_updated";
				
				// build data to send
				boost::property_tree::ptree pt;
				pt.put("flowAcc", 150);
				pt.put("flowRate", 25);
				pt.put("flowPH", 3.21);
				pt.put("flowConductivity", 0);
				pt.put("highLevel", false);
				pt.put("sampleInProgress", false);

				// send data
				_websocket.emit_event_data("flow_updated", pt);
				break;
			}
			default: {
				return 0;	// exit program
			}
		}
		goto loop;	
	}
	catch (const std::exception &e)
	{
		ERROR << "e.what() = " << e.what();
		return -1;
	}
	catch (int error)
	{
		ERROR << "error >> " << error;
		return -2;
	}

	FATAL << "main ended";

	return 0;
}
