#include "../include/websocket.hpp"

int main (void)
{
	INFO << "program started";

	websocket _websocket;
	bool debug = true;

	try
	{

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
