/*
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE.txt', which is part of this source code bundle.
 */

#include "../include/eth_socket.hpp"

eth_socket::eth_socket (void)	// class constructor
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

eth_socket::~eth_socket (void)	// class destructor
{
	INFO << "started";

	try
	{
		close_eth_socket(1);
		close_eth_socket(2);
	}
	catch (int error_socket_1)
	{
		ERROR << "error_socket_1 = " << error_socket_1;
	}
	catch (const std::exception &e)
	{
		ERROR << "e.what() = " << e.what();
	}

	INFO << "ended";
}

// Connection

void eth_socket::open_eth_socket (int socket_num)
{
	INFO << "started";

	try
	{
		switch (socket_num)
		{
			case 1:
			{
				boost::mutex::scoped_lock lock (mutex_socket_1);
				INFO << "socket_1_host = " << socket_1_host << "; socket_1_port = " << socket_1_port;

				// 1. reset io services
				io_service_1.reset();
				io_service_1 = boost::make_shared <boost::asio::io_service> ();

				// 2. create endpoint
				boost::asio::ip::tcp::endpoint remote_endpoint(
					boost::asio::ip::address::from_string(socket_1_host),
					socket_1_port
				);

				// 3. reset socket
				socket_1.reset(new boost::asio::ip::tcp::socket(*io_service_1));

				// 4. connect socket
				socket_1->async_connect(remote_endpoint,
					boost::bind(
						&eth_socket::socket_1_connect_callback,
						this, boost::asio::placeholders::error
					)
				);

				// 5. start io_service thread
				if (io_service_1_thread.get()) io_service_1_thread->interrupt();
				io_service_1_thread.reset(new boost::thread (boost::bind(&eth_socket::run_io_service_1, this)));
				break;
			}
			case 2:
			{
				boost::mutex::scoped_lock lock (mutex_socket_2);
				INFO << "socket_2_host = " << socket_2_host << "; socket_2_port = " << socket_2_port;

				// 1. reset io services
				io_service_2.reset();
				io_service_2 = boost::make_shared <boost::asio::io_service> ();

				// 2. create endpoint
				boost::asio::ip::tcp::endpoint remote_endpoint(
					boost::asio::ip::address::from_string(socket_2_host),
					socket_2_port
				);

				// 3. reset socket
				socket_2.reset(new boost::asio::ip::tcp::socket(*io_service_2));

				// 4. connect socket
				socket_2->async_connect(remote_endpoint,
					boost::bind(
						&eth_socket::socket_2_connect_callback,
						this, boost::asio::placeholders::error
					)
				);

				// 5. start io_service thread
				if (io_service_2_thread.get()) io_service_2_thread->interrupt();
				io_service_2_thread.reset(new boost::thread (boost::bind(&eth_socket::run_io_service_2, this)));
				break;
			}
			case 3:
			{
				boost::mutex::scoped_lock lock (mutex_socket_3);
				INFO << "socket_3_host = " << socket_3_host << "; socket_3_port = " << socket_3_port;

				// 1. reset io services
				io_service_3.reset();
				io_service_3 = boost::make_shared <boost::asio::io_service> ();

				// 2. create endpoint
				boost::asio::ip::tcp::endpoint remote_endpoint(
					boost::asio::ip::address::from_string(socket_3_host),
					socket_3_port
				);

				// 3. reset socket
				socket_3.reset(new boost::asio::ip::tcp::socket(*io_service_3));

				// 4. connect socket
				socket_3->async_connect(remote_endpoint,
					boost::bind(
						&eth_socket::socket_3_connect_callback,
						this, boost::asio::placeholders::error
					)
				);

				// 5. start io_service thread
				if (io_service_3_thread.get()) io_service_3_thread->interrupt();
				io_service_3_thread.reset(new boost::thread (boost::bind(&eth_socket::run_io_service_3, this)));
				break;
			}
			case 4:
			{
				boost::mutex::scoped_lock lock (mutex_socket_4);
				INFO << "socket_4_host = " << socket_4_host << "; socket_4_port = " << socket_4_port;

				// 1. reset io services
				io_service_4.reset();
				io_service_4 = boost::make_shared <boost::asio::io_service> ();

				// 2. create endpoint
				boost::asio::ip::tcp::endpoint remote_endpoint(
					boost::asio::ip::address::from_string(socket_4_host),
					socket_4_port
				);

				// 3. reset socket
				socket_4.reset(new boost::asio::ip::tcp::socket(*io_service_4));

				// 4. connect socket
				socket_4->async_connect(remote_endpoint,
					boost::bind(
						&eth_socket::socket_4_connect_callback,
						this, boost::asio::placeholders::error
					)
				);

				// 5. start io_service thread
				if (io_service_4_thread.get()) io_service_4_thread->interrupt();
				io_service_4_thread.reset(new boost::thread (boost::bind(&eth_socket::run_io_service_4, this)));
				break;
			}
			default:
			{
				ERROR << "socket_num unhandled = " << socket_num;
				throw -2;
			}
		}
	}
	catch (const std::exception &e)
	{
		ERROR << "e.what() = " << e.what();
		throw -1;
	}

	INFO << "ended";

	return;
}

void eth_socket::close_eth_socket (int socket_num)
{
	INFO << "started";

	try
	{
		switch (socket_num)
		{
			case 1:
			{
				boost::mutex::scoped_lock lock (mutex_socket_1);
				if (socket_1.get()) {
					if (socket_1->is_open()) {
						//socket_1->shutdown(boost::asio::ip::tcp::socket::shutdown_both);	// shutdown read/write operations (causes issues, do not use)
						socket_1->cancel();
						socket_1->close();
					}
				}
				else {
					WARNING << "socket_1 is not open";
				}

				if (io_service_1_thread.get()) io_service_1_thread->interrupt();
				if (io_service_1.get()) {
					io_service_1->stop();
					io_service_1->reset();
				}
				else
				{
					WARNING << "io_service_1 is not open";
				}
				break;
			}
			case 2:
			{
				boost::mutex::scoped_lock lock (mutex_socket_2);
				if (socket_2.get()) {
					if (socket_2->is_open()) {
						//socket_2->shutdown(boost::asio::ip::tcp::socket::shutdown_both);	// shutdown read/write operations (causes issues, do not use)
						socket_2->cancel();
						socket_2->close();
					}
				}
				else {
					WARNING << "socket_2 is not open";
				}

				if (io_service_2_thread.get()) io_service_2_thread->interrupt();
				if (io_service_2.get()) {
					io_service_2->stop();
					io_service_2->reset();
				}
				else
				{
					WARNING << "io_service_2 is not open";
				}
				break;
			}
			case 3:
			{
				boost::mutex::scoped_lock lock (mutex_socket_3);
				if (socket_3.get()) {
					if (socket_3->is_open()) {
						//socket_3->shutdown(boost::asio::ip::tcp::socket::shutdown_both);	// shutdown read/write operations (causes issues, do not use)
						socket_3->cancel();
						socket_3->close();
					}
				}
				else {
					WARNING << "socket_3 is not open";
				}

				if (io_service_3_thread.get()) io_service_3_thread->interrupt();
				if (io_service_3.get()) {
					io_service_3->stop();
					io_service_3->reset();
				}
				else
				{
					WARNING << "io_service_3 is not open";
				}
				break;
			}
			case 4:
			{
				boost::mutex::scoped_lock lock (mutex_socket_4);
				if (socket_4.get()) {
					if (socket_4->is_open()) {
						//socket_4->shutdown(boost::asio::ip::tcp::socket::shutdown_both);	// shutdown read/write operations (causes issues, do not use)
						socket_4->cancel();
						socket_4->close();
					}
				}
				else {
					WARNING << "socket_4 is not open";
				}

				if (io_service_4_thread.get()) io_service_4_thread->interrupt();
				if (io_service_4.get()) {
					io_service_4->stop();
					io_service_4->reset();
				}
				else
				{
					WARNING << "io_service_4 is not open";
				}
				break;
			}
			default:
			{
				ERROR << "socket_num unhandled = " << socket_num;
				throw -2;
			}
		}
	}
	catch (const std::exception &e)
	{
		ERROR << "e.what() = " << e.what();
		throw -1;
	}

	INFO << "ended";

	return;
}

bool eth_socket::check_socket_status (int socket_num)
{
	try
	{
		switch (socket_num)
		{
			case 1:
			{
				if (!socket_1->is_open())
				{
					WARNING << "socket_1 is not open";
					return false;
				}
				break;
			}
			case 2:
			{
				if (!socket_2->is_open())
				{
					WARNING << "socket_2 is not open";
					return false;
				}
				break;
			}
			case 3:
			{
				if (!socket_3->is_open())
				{
					WARNING << "socket_3 is not open";
					return false;
				}
				break;
			}
			case 4:
			{
				if (!socket_4->is_open())
				{
					WARNING << "socket_4 is not open";
					return false;
				}
				break;
			}
			default:
			{
				ERROR << "socket_num unhandled = " << socket_num;
				throw -2;
			}
		}
	}
	catch (const std::exception &e)
	{
		ERROR << "e.what() = " << e.what();
		throw -1;
	}

	return true;
}

void eth_socket::run_io_service_1 (void)
{
	INFO << "started";

	bool debug = true;

	try
	{
		while (true)
		{
			boost::asio::io_service::work work(*io_service_1);
			io_service_1->run();
			io_service_1->reset();
			if (debug) INFO << "io_service_1 run complete";
			boost::this_thread::sleep (boost::posix_time::milliseconds (100));
		}
	}
	catch (const std::exception &e)
	{
		ERROR << "e.what() = " << e.what();
		return;
	}
	catch (boost::thread_interrupted)
	{
		INFO << "interrupted";
		return;
	}

	INFO << "ended";

	return;
}

void eth_socket::run_io_service_2 (void)
{
	INFO << "started";

	bool debug = true;

	try
	{
		while (true)
		{
			boost::asio::io_service::work work(*io_service_2);
			io_service_2->run();
			io_service_2->reset();
			if (debug) INFO << "io_service_2 run complete";
			boost::this_thread::sleep (boost::posix_time::milliseconds (100));
		}
	}
	catch (const std::exception &e)
	{
		ERROR << "e.what() = " << e.what();
		return;
	}
	catch (boost::thread_interrupted)
	{
		INFO << "interrupted";
		return;
	}

	INFO << "ended";

	return;
}

void eth_socket::run_io_service_3 (void)
{
	INFO << "started";

	bool debug = true;

	try
	{
		while (true)
		{
			boost::asio::io_service::work work(*io_service_3);
			io_service_3->run();
			io_service_3->reset();
			if (debug) INFO << "io_service_3 run complete";
			boost::this_thread::sleep (boost::posix_time::milliseconds (100));
		}
	}
	catch (const std::exception &e)
	{
		ERROR << "e.what() = " << e.what();
		return;
	}
	catch (boost::thread_interrupted)
	{
		INFO << "interrupted";
		return;
	}

	INFO << "ended";

	return;
}

void eth_socket::run_io_service_4 (void)
{
	INFO << "started";

	bool debug = true;

	try
	{
		while (true)
		{
			boost::asio::io_service::work work(*io_service_4);
			io_service_4->run();
			io_service_4->reset();
			if (debug) INFO << "io_service_4 run complete";
			boost::this_thread::sleep (boost::posix_time::milliseconds (100));
		}
	}
	catch (const std::exception &e)
	{
		ERROR << "e.what() = " << e.what();
		return;
	}
	catch (boost::thread_interrupted)
	{
		INFO << "interrupted";
		return;
	}

	INFO << "ended";

	return;
}

// Write

void eth_socket::write_data (std::string data, int socket_num)
{
	bool debug = false;

	if (debug) INFO << "started";

	try
	{
		switch (socket_num)
		{
			case 1:
			{
				boost::mutex::scoped_lock lock (mutex_socket_1);
				// 1. check socket status
				if (!socket_1->is_open())
				{
					WARNING << "socket_1 is not open";
					throw -3;
				}

				// 2. start asynchronous write
				socket_1->async_write_some(
					boost::asio::buffer(data.c_str(), data.size()),
					boost::bind(
						&eth_socket::socket_1_write_data_callback,
						this, boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred
					)
				);

				break;
			}
			case 2:
			{
				boost::mutex::scoped_lock lock (mutex_socket_2);
				// 1. check socket status
				if (!socket_2->is_open())
				{
					WARNING << "socket_2 is not open";
					throw -3;
				}

				// 2. start asynchronous write
				socket_2->async_write_some(
					boost::asio::buffer(data.c_str(), data.size()),
					boost::bind(
						&eth_socket::socket_2_write_data_callback,
						this, boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred
					)
				);

				break;
			}
			case 3:
			{
				boost::mutex::scoped_lock lock (mutex_socket_3);
				// 1. check socket status
				if (!socket_3->is_open())
				{
					WARNING << "socket_3 is not open";
					throw -3;
				}

				// 2. start asynchronous write
				socket_3->async_write_some(
					boost::asio::buffer(data.c_str(), data.size()),
					boost::bind(
						&eth_socket::socket_3_write_data_callback,
						this, boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred
					)
				);

				break;
			}
			case 4:
			{
				boost::mutex::scoped_lock lock (mutex_socket_4);
				// 1. check socket status
				if (!socket_4->is_open())
				{
					WARNING << "socket_4 is not open";
					throw -3;
				}

				// 2. start asynchronous write
				socket_4->async_write_some(
					boost::asio::buffer(data.c_str(), data.size()),
					boost::bind(
						&eth_socket::socket_4_write_data_callback,
						this, boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred
					)
				);

				break;
			}
			default:
			{
				ERROR << "socket_num unhandled = " << socket_num;
				throw -2;
			}
		}
	}
	catch (const std::exception &e)
	{
		ERROR << "e.what() = " << e.what();
		throw -1;
	}

	if (debug) INFO << "ended";

	return;
}

// Read

void eth_socket::read_data (int socket_num)
{
	INFO << "started";

	try
	{
		switch (socket_num)
		{
			case 1:
			{
				boost::mutex::scoped_lock lock (mutex_socket_1);
				// 1. check socket status
				if (!socket_1->is_open())
				{
					WARNING << "socket_1 is not open";
					throw -3;
				}

				// 2. start asynchronous write
				boost::asio::async_read_until(
					*socket_1, response_data_socket_1, socket_1_delimiter,
					boost::bind(
						&eth_socket::socket_1_read_data_callback,
						this, boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred
					)
				);
				break;
			}
			case 2:
			{
				boost::mutex::scoped_lock lock (mutex_socket_2);
				// 1. check socket status
				if (!socket_2->is_open())
				{
					WARNING << "socket_2 is not open";
					throw -3;
				}

				// 2. start asynchronous write
				boost::asio::async_read_until(
					*socket_2, response_data_socket_2, socket_2_delimiter,
					boost::bind(
						&eth_socket::socket_2_read_data_callback,
						this, boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred
					)
				);
				break;
			}
			case 3:
			{
				boost::mutex::scoped_lock lock (mutex_socket_3);
				// 1. check socket status
				if (!socket_3->is_open())
				{
					WARNING << "socket_3 is not open";
					throw -3;
				}

				// 2. start asynchronous write
				boost::asio::async_read_until(
					*socket_3, response_data_socket_3, socket_3_delimiter,
					boost::bind(
						&eth_socket::socket_3_read_data_callback,
						this, boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred
					)
				);
				break;
			}
			case 4:
			{
				boost::mutex::scoped_lock lock (mutex_socket_4);
				// 1. check socket status
				if (!socket_4->is_open())
				{
					WARNING << "socket_4 is not open";
					throw -3;
				}

				// 2. start asynchronous write
				boost::asio::async_read_until(
					*socket_4, response_data_socket_4, socket_4_delimiter,
					boost::bind(
						&eth_socket::socket_4_read_data_callback,
						this, boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred
					)
				);
				break;
			}
			default:
			{
				ERROR << "socket_num unhandled = " << socket_num;
				throw -2;
			}
		}
	}
	catch (const std::exception &e)
	{
		ERROR << "e.what() = " << e.what();
		throw -1;
	}

	INFO << "ended";

	return;
}

// Callbacks

/// Socket 1

void eth_socket::socket_1_connect_callback (const boost::system::error_code& error)
{
	boost::mutex::scoped_lock lock (mutex_socket_1);	// prevent multiple threads

	INFO << "started";

	try
	{
		// 1. check for errors
		if (error)
		{
			ERROR << "error.message() >> " << error.message().c_str();
			events.push(556);	// eth_socket_error
			return;
		}
		else
		{
			INFO << "connection to " << socket_1_host << ":" << socket_1_port << " succeeded";
		}

		// 2. set socket keep alive
		boost::asio::socket_base::keep_alive opt_keep_alive(true);
		socket_1->set_option(opt_keep_alive, error_1);
		if (error_1)
		{
			ERROR << "error_1.message() >> " << error_1.message().c_str();
			events.push(556);	// eth_socket_error
			return;
		}
		struct timeval tv;
		tv.tv_sec  = socket_1_timeout / 1000;
		tv.tv_usec = socket_1_timeout % 1000;
		setsockopt(socket_1->native(), SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
		setsockopt(socket_1->native(), SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
	}
	catch (const std::exception &e)
	{
		ERROR << "e.what() = " << e.what();
		events.push(556);	// eth_socket_error
		return;
	}

	INFO << "ended";

	return;
}

void eth_socket::socket_1_write_data_callback (const boost::system::error_code& error, size_t bytes_transferred)
{
	boost::mutex::scoped_lock lock (mutex_socket_1);	// prevent multiple threads

	bool debug = false;

	if (debug) INFO << "started";

	try
	{
		// 1. check for errors
		if (error)
		{
			ERROR << "error.message() >> " << error.message().c_str();
			events.push(556);	// eth_socket_error
			return;
		}
		if (socket_1.get() == NULL || !socket_1->is_open())
		{
			WARNING << "socket_1 is not open";
			events.push(556);	// eth_socket_error
			return;
		}

		if (debug) INFO << "data written to " << socket_1_host << ":" << socket_1_port << " succeeded; bytes_transferred = " << bytes_transferred;
	}
	catch (const std::exception &e)
	{
		ERROR << "e.what() = " << e.what();
		events.push(556);	// eth_socket_error
		return;
	}

	if (debug) INFO << "ended";

	return;
}

void eth_socket::socket_1_read_data_callback (const boost::system::error_code& error, size_t bytes_transferred)
{
	boost::mutex::scoped_lock lock (mutex_socket_1);	// prevent multiple threads

	bool debug = true;

	if (debug) INFO << "started";

	std::string socket_read_data, message, mark;
	boost::property_tree::ptree pt;
	std::stringstream ss;

	try
	{
		// reset io_service
		io_service_1->reset();

		// check for errors
		if (error)
		{
			ERROR << "error.message() >> " << error.message().c_str();
			events.push(556);	// eth_socket_error
			return;
		}
		if (socket_1.get() == NULL || !socket_1->is_open())
		{
			WARNING << "socket_1 is not open";
			events.push(556);	// eth_socket_error
			return;
		}

		// get data
		std::istream response_data_stream (&response_data_socket_1);
		std::getline(response_data_stream, socket_read_data);
		if (debug) INFO << "socket_read_data = " << socket_read_data << "; bytes_transferred = " << bytes_transferred;

		// handle response
		if (socket_read_data.length()) {
			// check known responses first
			if (socket_read_data.find("Welcome to ACTS Flex") != std::string::npos) {
				INFO << "read connection established";
			} else if (socket_read_data.find("REPLY00") != std::string::npos) {
				socket_1_keep_alive_rec = true;
			} else {	
				ss << socket_read_data;
				
				// parse JSON
				try {
					boost::property_tree::read_json(ss, pt);
					boost::optional <boost::property_tree::ptree&> child = pt.get_child_optional("message");
					if (child) {
						message = pt.get<std::string>("message");
						if (debug) INFO << "message = " << message;

						if (message == "device_alive")
						{
							seq_events.push(30); 	// socket_1_device_alive
							socket_1_keep_alive_rec = true;
						}
					}
					boost::optional <boost::property_tree::ptree&> child1 = pt.get_child_optional("mark");
					if (child1) {
						mark = pt.get<std::string>("mark");
						if (debug) INFO << "mark = " << mark;
						socket_1_message = mark;
						seq_events.push(37);	// Proximity Card Scanned
					}
				} catch (const boost::property_tree::json_parser_error& pt_err) {
					WARNING << "JSON parse error >> error = " << pt_err.message().c_str();
				}
			}
		} else {
			INFO << "no data to read";
		}

		// restart read
		boost::asio::async_read_until(
			*socket_1, response_data_socket_1, socket_1_delimiter,
			boost::bind(
				&eth_socket::socket_1_read_data_callback,
				this, boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred
			)
		);
	}
	catch (const std::exception &e)
	{
		ERROR << "e.what() = " << e.what();
		events.push(556);	// eth_socket_error
		return;
	}

	if (debug) INFO << "ended";

	return;
}

/// Socket 2

void eth_socket::socket_2_connect_callback (const boost::system::error_code& error)
{
	boost::mutex::scoped_lock lock (mutex_socket_2);	// prevent multiple threads

	INFO << "started";

	try
	{
		// 1. check for errors
		if (error)
		{
			ERROR << "error.message() >> " << error.message().c_str();
			events.push(131);	// eth_socket_2_error
			return;
		}
		else
		{
			INFO << "connection to " << socket_2_host << ":" << socket_2_port << " succeeded";
		}

		// 2. set socket keep alive
		boost::asio::socket_base::keep_alive opt_keep_alive(true);
		socket_2->set_option(opt_keep_alive, error_2);
		if (error_2)
		{
			ERROR << "error_2.message() >> " << error_2.message().c_str();
			events.push(131);	// eth_socket_2_error
			return;
		}
		struct timeval tv;
		tv.tv_sec  = socket_2_timeout / 1000;
		tv.tv_usec = socket_2_timeout % 1000;
		setsockopt(socket_2->native(), SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
		setsockopt(socket_2->native(), SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
	}
	catch (const std::exception &e)
	{
		ERROR << "e.what() = " << e.what();
		events.push(131);	// eth_socket_2_error
		return;
	}

	INFO << "ended";

	return;
}

void eth_socket::socket_2_write_data_callback (const boost::system::error_code& error, size_t bytes_transferred)
{
	boost::mutex::scoped_lock lock (mutex_socket_2);	// prevent multiple threads

	bool debug = false;

	if (debug) INFO << "started";

	try
	{
		// 1. check for errors
		if (error)
		{
			ERROR << "error.message() >> " << error.message().c_str();
			events.push(131);	// eth_socket_2_error
			return;
		}
		if (socket_2.get() == NULL || !socket_2->is_open())
		{
			WARNING << "socket_2 is not open";
			events.push(131);	// eth_socket_2_error
			return;
		}

		if (debug) INFO << "data written to " << socket_2_host << ":" << socket_2_port << " succeeded; bytes_transferred = " << bytes_transferred;
	}
	catch (const std::exception &e)
	{
		ERROR << "e.what() = " << e.what();
		events.push(131);	// eth_socket_2_error
		return;
	}

	if (debug) INFO << "ended";

	return;
}

void eth_socket::socket_2_read_data_callback (const boost::system::error_code& error, size_t bytes_transferred)
{
	boost::mutex::scoped_lock lock (mutex_socket_2);	// prevent multiple threads

	bool debug = false, debug1 = false;

	if (debug) INFO << "started";

	std::string socket_read_data, message;
	boost::property_tree::ptree pt;
	std::stringstream ss;

	try
	{
		// 1. reset io_service
		io_service_2->reset();

		// 2. check for errors
		if (error)
		{
			ERROR << "error.message() >> " << error.message().c_str();
			events.push(131);	// eth_socket_2_error
			return;
		}
		if (socket_2.get() == NULL || !socket_2->is_open())
		{
			WARNING << "socket_2 is not open";
			events.push(131);	// eth_socket_2_error
			return;
		}

		// 3. get data
		std::istream response_data_stream (&response_data_socket_2);
		std::getline(response_data_stream, socket_read_data);
		if (debug) INFO << "socket_read_data = " << socket_read_data << "; bytes_transferred = " << bytes_transferred;

		// 4. parse JSON
		ss << socket_read_data;
		boost::property_tree::read_json(ss, pt);
		message = pt.get<std::string>("message");
		if (debug || debug1) INFO << "message = " << message;

		// 5. handle data
		if (message == "device_alive")
		{
			seq_events.push(31); 	// socket_2_device_alive
			socket_2_keep_alive_rec = true;
		}

		// 5. restart read
		boost::asio::async_read_until(
			*socket_2, response_data_socket_2, socket_2_delimiter,
			boost::bind(
				&eth_socket::socket_2_read_data_callback,
				this, boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred
			)
		);
	}
	catch (const std::exception &e)
	{
		ERROR << "e.what() = " << e.what();
		events.push(131);	// eth_socket_2_error
		return;
	}

	if (debug) INFO << "ended";

	return;
}

/// Socket 3

void eth_socket::socket_3_connect_callback (const boost::system::error_code& error)
{
	boost::mutex::scoped_lock lock (mutex_socket_3);	// prevent multiple threads

	INFO << "started";

	try
	{
		// 1. check for errors
		if (error)
		{
			ERROR << "error.message() >> " << error.message().c_str();
			events.push(132);	// eth_socket_3_error
			return;
		}
		else
		{
			INFO << "connection to " << socket_3_host << ":" << socket_3_port << " succeeded";
		}

		// 2. set socket keep alive
		boost::asio::socket_base::keep_alive opt_keep_alive(true);
		socket_3->set_option(opt_keep_alive, error_3);
		if (error_3)
		{
			ERROR << "error_3.message() >> " << error_3.message().c_str();
			events.push(132);	// eth_socket_3_error
			return;
		}
		struct timeval tv;
		tv.tv_sec  = socket_3_timeout / 1000;
		tv.tv_usec = socket_3_timeout % 1000;
		setsockopt(socket_3->native(), SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
		setsockopt(socket_3->native(), SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
	}
	catch (const std::exception &e)
	{
		ERROR << "e.what() = " << e.what();
		events.push(131);	// eth_socket_2_error
		return;
	}

	INFO << "ended";

	return;
}

void eth_socket::socket_3_write_data_callback (const boost::system::error_code& error, size_t bytes_transferred)
{
	boost::mutex::scoped_lock lock (mutex_socket_3);	// prevent multiple threads

	bool debug = false;

	if (debug) INFO << "started";

	try
	{
		// 1. check for errors
		if (error)
		{
			ERROR << "error.message() >> " << error.message().c_str();
			events.push(132);	// eth_socket_3_error
			return;
		}
		if (socket_3.get() == NULL || !socket_3->is_open())
		{
			WARNING << "socket_3 is not open";
			events.push(132);	// eth_socket_3_error
			return;
		}

		if (debug) INFO << "data written to " << socket_3_host << ":" << socket_3_port << " succeeded; bytes_transferred = " << bytes_transferred;
	}
	catch (const std::exception &e)
	{
		ERROR << "e.what() = " << e.what();
		events.push(132);	// eth_socket_3_error
		return;
	}

	if (debug) INFO << "ended";

	return;
}

void eth_socket::socket_3_read_data_callback (const boost::system::error_code& error, size_t bytes_transferred)
{
	boost::mutex::scoped_lock lock (mutex_socket_3);	// prevent multiple threads

	bool debug = false, debug1 = false;

	if (debug) INFO << "started";

	std::string socket_read_data, message;
	boost::property_tree::ptree pt;
	std::stringstream ss;

	try
	{
		// 1. reset io_service
		io_service_3->reset();

		// 2. check for errors
		if (error)
		{
			ERROR << "error.message() >> " << error.message().c_str();
			events.push(132);	// eth_socket_3_error
			return;
		}
		if (socket_3.get() == NULL || !socket_3->is_open())
		{
			WARNING << "socket_3 is not open";
			events.push(132);	// eth_socket_3_error
			return;
		}

		// 3. get data
		std::istream response_data_stream (&response_data_socket_3);
		std::getline(response_data_stream, socket_read_data);
		if (debug) INFO << "socket_read_data = " << socket_read_data << "; bytes_transferred = " << bytes_transferred;

		// 4. parse JSON
		ss << socket_read_data;
		boost::property_tree::read_json(ss, pt);
		message = pt.get<std::string>("message");
		if (debug || debug1) INFO << "message = " << message;

		// 5. handle data
		if (message == "device_alive")
		{
			seq_events.push(32); 	// socket_3_device_alive
			socket_3_keep_alive_rec = true;
		}

		// 5. restart read
		boost::asio::async_read_until(
			*socket_3, response_data_socket_3, socket_3_delimiter,
			boost::bind(
				&eth_socket::socket_3_read_data_callback,
				this, boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred
			)
		);
	}
	catch (const std::exception &e)
	{
		ERROR << "e.what() = " << e.what();
		events.push(132);	// eth_socket_3_error
		return;
	}

	if (debug) INFO << "ended";

	return;
}

/// Socket 4

void eth_socket::socket_4_connect_callback (const boost::system::error_code& error)
{
	boost::mutex::scoped_lock lock (mutex_socket_4);	// prevent multiple threads

	INFO << "started";

	try
	{
		// 1. check for errors
		if (error)
		{
			ERROR << "error.message() >> " << error.message().c_str();
			events.push(133);	// eth_socket_4_error
			return;
		}
		else
		{
			INFO << "connection to " << socket_4_host << ":" << socket_4_port << " succeeded";
		}

		// 2. set socket keep alive
		boost::asio::socket_base::keep_alive opt_keep_alive(true);
		socket_4->set_option(opt_keep_alive, error_4);
		if (error_4)
		{
			ERROR << "error_4.message() >> " << error_4.message().c_str();
			events.push(133);	// eth_socket_4_error
			return;
		}
		struct timeval tv;
		tv.tv_sec  = socket_4_timeout / 1000;
		tv.tv_usec = socket_4_timeout % 1000;
		setsockopt(socket_4->native(), SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
		setsockopt(socket_4->native(), SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
	}
	catch (const std::exception &e)
	{
		ERROR << "e.what() = " << e.what();
		events.push(133);	// eth_socket_4_error
		return;
	}

	INFO << "ended";

	return;
}

void eth_socket::socket_4_write_data_callback (const boost::system::error_code& error, size_t bytes_transferred)
{
	boost::mutex::scoped_lock lock (mutex_socket_4);	// prevent multiple threads

	bool debug = false;

	if (debug) INFO << "started";

	try
	{
		// 1. check for errors
		if (error)
		{
			ERROR << "error.message() >> " << error.message().c_str();
			events.push(133);	// eth_socket_4_error
			return;
		}
		if (socket_4.get() == NULL || !socket_4->is_open())
		{
			WARNING << "socket_4 is not open";
			events.push(133);	// eth_socket_4_error
			return;
		}

		if (debug) INFO << "data written to " << socket_4_host << ":" << socket_4_port << " succeeded; bytes_transferred = " << bytes_transferred;
	}
	catch (const std::exception &e)
	{
		ERROR << "e.what() = " << e.what();
		events.push(133);	// eth_socket_4_error
		return;
	}

	if (debug) INFO << "ended";

	return;
}

void eth_socket::socket_4_read_data_callback (const boost::system::error_code& error, size_t bytes_transferred)
{
	boost::mutex::scoped_lock lock (mutex_socket_4);	// prevent multiple threads

	bool debug = false, debug1 = false;

	if (debug) INFO << "started";

	std::string socket_read_data, message;
	boost::property_tree::ptree pt;
	std::stringstream ss;

	try
	{
		// 1. reset io_service
		io_service_4->reset();

		// 2. check for errors
		if (error)
		{
			ERROR << "error.message() >> " << error.message().c_str();
			events.push(133);	// eth_socket_4_error
			return;
		}
		if (socket_4.get() == NULL || !socket_4->is_open())
		{
			WARNING << "socket_4 is not open";
			events.push(133);	// eth_socket_4_error
			return;
		}

		// 3. get data
		std::istream response_data_stream (&response_data_socket_4);
		std::getline(response_data_stream, socket_read_data);
		if (debug) INFO << "socket_read_data = " << socket_read_data << "; bytes_transferred = " << bytes_transferred;

		// 4. parse JSON
		ss << socket_read_data;
		boost::property_tree::read_json(ss, pt);
		message = pt.get<std::string>("message");
		if (debug || debug1) INFO << "message = " << message;

		// 5. handle data
		if (message == "device_alive")
		{
			seq_events.push(33); 	// socket_4_device_alive
			socket_4_keep_alive_rec = true;
		}

		// 5. restart read
		boost::asio::async_read_until(
			*socket_4, response_data_socket_4, socket_4_delimiter,
			boost::bind(
				&eth_socket::socket_4_read_data_callback,
				this, boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred
			)
		);
	}
	catch (const std::exception &e)
	{
		ERROR << "e.what() = " << e.what();
		events.push(133);	// eth_socket_4_error
		return;
	}

	if (debug) INFO << "ended";

	return;
}

//// Other

/* Notes
****************************************************************************************************************************************

*/
