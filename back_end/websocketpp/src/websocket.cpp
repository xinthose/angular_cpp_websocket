/*
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE.txt', which is part of this source code bundle.
 */

#include "../include/websocket.hpp"

websocket::websocket(int port, bool debug) // class constructor
{
	INFO << "started";

	try
	{
		// set data
		this->debug = debug;

		// initialize asio transport
		m_server.init_asio();

		// register handler callbacks
		m_server.set_open_handler(
			websocketpp::lib::bind(
				&websocket::on_open, this,
				::websocketpp::lib::placeholders::_1));
		m_server.set_close_handler(
			websocketpp::lib::bind(
				&websocket::on_close, this,
				::websocketpp::lib::placeholders::_1));
		m_server.set_message_handler(
			websocketpp::lib::bind(
				&websocket::on_message, this,
				::websocketpp::lib::placeholders::_1, ::websocketpp::lib::placeholders::_2));
		m_server.set_http_handler(
			websocketpp::lib::bind(
				&websocket::on_httpinit, this, &m_server,
				::websocketpp::lib::placeholders::_1));

		// Start a websocketpp::lib::thread to run the processing loop
		process_messages_thread = websocketpp::lib::thread(websocketpp::lib::bind(
			&websocket::process_messages, this));

		INFO << "listen on port " << port;
		m_server.listen(port);

		INFO << "start the server accept loop";
		m_server.start_accept();

		INFO << "start the ASIO io_service run loop";
		run_thread = boost::thread([this] {
			try
			{
				m_server.run();
			}
			catch (const std::exception &e)
			{
				ERROR << "error = " << e.what();
			}
			INFO << "run_thread stopping";
		});
	}
	catch (const std::exception &e)
	{
		ERROR << "e.what() = " << e.what();
		throw - 1;
	}

	INFO << "ended";
}

websocket::~websocket(void) // class destructor
{
	INFO << "started";

	try
	{
		if (process_messages_thread.joinable())
		{
			INFO << "end process_messages_thread";
			process_messages_thread.join();
		}
		if (run_thread.joinable())
		{
			INFO << "end run_thread";
			run_thread.join();
		}
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

void websocket::process_messages(void)
{
	websocketpp::lib::unique_lock<websocketpp::lib::mutex> lock(m_event_lock);

	INFO << "started";

	try
	{
		while (true)
		{

			while (m_events.empty())
			{
				m_event_cond.wait(lock);
			}
			INFO << "event found";

			event a = m_events.front();
			m_events.pop();

			lock.unlock();

			INFO << "event = " << a.type;
			if (a.type == CONNECT)
			{
				websocketpp::lib::lock_guard<websocketpp::lib::mutex> guard(m_connection_lock);
				m_connections.insert(a.hdl);
			}
			else if (a.type == DISCONNECT)
			{
				websocketpp::lib::lock_guard<websocketpp::lib::mutex> guard(m_connection_lock);
				m_connections.erase(a.hdl);
			}
			else if (a.type == MESSAGE)
			{
				websocketpp::lib::lock_guard<websocketpp::lib::mutex> guard(m_connection_lock);

				con_list::iterator it;
				for (it = m_connections.begin(); it != m_connections.end(); ++it)
				{
					m_server.send(*it, a.msg);
				}
			}
			else
			{
				ERROR << "unhandled event";
			}
		}
	}
	catch (const std::exception &e)
	{
		ERROR << "e.what() = " << e.what();
		throw - 1;
	}

	INFO << "ended";

	return;
}

bool websocket::on_httpinit(server* s, websocketpp::connection_hdl hdl) 
{
	INFO << "mark1";
	server::connection_ptr con = s->get_con_from_hdl(hdl.lock());
	con->append_header("Access-Control-Allow-Origin", "*");
	return true;
}

// Callbacks

void websocket::on_open(websocketpp::connection_hdl hdl)
{
	websocketpp::lib::lock_guard<websocketpp::lib::mutex> guard(m_event_lock);

	INFO << "started";

	try
	{
		m_events.push(event(CONNECT, hdl));
		m_event_cond.notify_one();
	}
	catch (const std::exception &e)
	{
		ERROR << "e.what() = " << e.what();
	}

	INFO << "ended";

	return;
}

void websocket::on_close(websocketpp::connection_hdl hdl)
{
	websocketpp::lib::lock_guard<websocketpp::lib::mutex> guard(m_event_lock);

	INFO << "started";

	try
	{
		m_events.push(event(DISCONNECT, hdl));
		m_event_cond.notify_one();
	}
	catch (const std::exception &e)
	{
		ERROR << "e.what() = " << e.what();
	}

	INFO << "ended";

	return;
}

void websocket::on_message(websocketpp::connection_hdl hdl, server::message_ptr msg)
{
	websocketpp::lib::lock_guard<websocketpp::lib::mutex> guard(m_event_lock);

	INFO << "started";

	try
	{
		m_events.push(event(MESSAGE, hdl, msg));
		m_event_cond.notify_one();
	}
	catch (const std::exception &e)
	{
		ERROR << "e.what() = " << e.what();
	}

	INFO << "ended";

	return;
}
