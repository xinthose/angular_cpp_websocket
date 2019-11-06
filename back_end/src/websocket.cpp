/*
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE.txt', which is part of this source code bundle.
 */

#include "../include/websocket.hpp"

websocket::websocket (void)	// class constructor
{
	INFO << "started";

	try
	{
		
	}
	catch (const std::exception &e)
	{
		ERROR << "e.what() = " << e.what();
	}

	INFO << "ended";
}

websocket::~websocket (void)	// class destructor
{
	INFO << "started";

	try
	{
	}
	catch (int error)
	{
		ERROR << "error = " << error;
	}
	catch (const std::exception &e)
	{
		ERROR << "e.what() = " << e.what();
	}

	INFO << "ended";
}
