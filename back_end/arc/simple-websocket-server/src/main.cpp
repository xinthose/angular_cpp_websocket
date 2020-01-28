#include "../include/websocket.hpp"

int main (void)
{
	INFO << "program started";

	//bool debug = true;

	try
	{
		// config
		websocket _websocket;
		std::string crt_path = "/home/elemech/Documents/github/test/angular_cpp_websocket/back_end/simple-websocket-server/ssl/server.crt";
		std::string key_path = "/home/elemech/Documents/github/test/angular_cpp_websocket/back_end/simple-websocket-server/ssl/server.key";
		int port = 8090;

		INFO << "initialize websocket server";
		_websocket.initialize(crt_path, key_path, port);

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
