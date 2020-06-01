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

#include <boost/thread.hpp>

#include "logging.hpp" // Boost logging

// Example code: https://github.com/zaphoyd/websocketpp/blob/master/examples/broadcast_server/broadcast_server.cpp

class websocket
{
public:
	typedef websocketpp::server<websocketpp::config::asio> server;

	// Functions
	websocket(int port, bool debug);  // class constructor
	~websocket(); // class destructor
	void process_messages(void);
	bool on_httpinit(server* s, websocketpp::connection_hdl hdl);

	/// callbacks
	void on_open(websocketpp::connection_hdl hdl);
	void on_close(websocketpp::connection_hdl hdl);
	void on_message(websocketpp::connection_hdl hdl, server::message_ptr msg);

	// Variables
	enum event_type
	{
		CONNECT,
		DISCONNECT,
		MESSAGE,
		STATION_CLOSED,
	};
	struct event
	{
		event(event_type t, websocketpp::connection_hdl h) : type(t), hdl(h) {}
		event(event_type t, websocketpp::connection_hdl h, server::message_ptr m)
			: type(t), hdl(h), msg(m) {}

		event_type type;
		websocketpp::connection_hdl hdl;
		server::message_ptr msg;
	};

private:
	typedef std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl> > con_list;

	server m_server;
	con_list m_connections;
	std::queue<event> m_events;

	websocketpp::lib::mutex m_event_lock;
	websocketpp::lib::mutex m_connection_lock;
	websocketpp::lib::condition_variable m_event_cond;

	bool debug;
	boost::thread run_thread;
	websocketpp::lib::thread process_messages_thread;
};

#endif // __websocket_HPP
