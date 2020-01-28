#include "../include/websocket.hpp"

int main (void)
{
	INFO << "program started";

	//bool debug = true;

	try
	{
		websocket _websocket;

		INFO << "initialize websocket server";
		_websocket.initialize();

		INFO << "start a thread to run the processing loop";
		websocketpp::lib::thread t(
			websocketpp::lib::bind(
				&websocket::process_messages,
				&_websocket
			)
		);

		INFO << "run the asio loop with the main thread";
		_websocket.run(8090);

		INFO << "join thread";
		t.join();
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
