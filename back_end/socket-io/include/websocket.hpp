/*
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE.txt', which is part of this source code bundle.
 */

#ifndef __WEBSOCKET_HPP
#define __WEBSOCKET_HPP

// Standard
#include <string>
#include <vector>
#include <sstream>
#include <set>
#include <functional>

// Boost
#include <boost/lockfree/queue.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

// Headers
#include "logging.hpp" // Boost logging
#include "socket-io/sio_client.h"

// Github project URL: https://github.com/socketio/socket.io-client-cpp

class websocket
{
	public:
		// Functions
		websocket();  // class constructor
		~websocket(); // class destructor
		void initialize (std::string wss_server);
		void emit_event(std::string event);
		void emit_event_data(std::string event, boost::property_tree::ptree pt);

		/// Event callbacks
		void new_screen(const std::string& name, sio::message::ptr const& message, bool need_ack, sio::message::list& ack_message);
		void trans_started(const std::string& name, sio::message::ptr const& message, bool need_ack, sio::message::list& ack_message);
		void user_logged_in(const std::string& name, sio::message::ptr const& message, bool need_ack, sio::message::list& ack_message);
		void user_logged_out(sio::event &event);

		// Variables
		boost::lockfree::queue <int> events {1000}, seq_events {1000}, current_screen {1000};
		struct {
			std::string access_number = "";
			int truck_number = 0;
			int fill_point_num = 0;
		} user;

		struct {
			int product_type = 0;
			int product_type_sub_type = 0;
			int requested_volume = 0;
			std::string manifest_number = "";
		} trans_started_data;

	private:
		// Variables
		boost::mutex mutex;
		sio::client h;
};

#endif // __websocket_HPP
