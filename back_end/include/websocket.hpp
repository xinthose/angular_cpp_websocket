/*
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE.txt', which is part of this source code bundle.
 */

#ifndef __WEBSOCKET_HPP
#define __WEBSOCKET_HPP

#include <string>
#include <vector>
#include <sstream>
#include <set>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/common/thread.hpp>

#include "logging.hpp"	// Boost logging

// Example code: https://github.com/zaphoyd/websocketpp/blob/master/examples/broadcast_server/broadcast_server.cpp

class websocket
{
	public:
		/// Functions
		websocket();	// class constructor
		~websocket();	// class destructor
		// Connection
		void open_websocket (int socket_num);
		void close_websocket (int socket_num);
		
	private:
};

#endif	// __websocket_HPP
