#include "../include/websocket.hpp"

#include <boost/move/unique_ptr.hpp>

int main (void)
{
	INFO << "program started";

	//bool debug = true;

	try
	{
		struct {
			bool used = true;
			std::string address = "0.0.0.0";
			int port = 8090;
			bool debug = true;
		} websocket_config;

		boost::movelib::unique_ptr<websocket> _websocket;
		_websocket.reset(new websocket(websocket_config.port, websocket_config.debug));

		while (true) {
			pause();
		}
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
