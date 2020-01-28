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
#include <future>
#include <memory>

#include <simple-websocket-server/client_wss.hpp>
#include <simple-websocket-server/server_wss.hpp>

#include "logging.hpp" // Boost logging

// Example server: https://gitlab.com/eidheim/Simple-WebSocket-Server/blob/master/wss_examples.cpp 

class websocket
{
	public:
		// Functions
		websocket();  // class constructor
		~websocket(); // class destructor
		void initialize(std::string crt_path, std::string key_path, int port);

	private:
};

#endif // __websocket_HPP
