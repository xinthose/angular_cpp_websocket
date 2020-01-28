/*
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE.txt', which is part of this source code bundle.
 */

#include "../include/websocket.hpp"

websocket::websocket(void) // class constructor
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

websocket::~websocket(void) // class destructor
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

void websocket::initialize(std::string crt_path, std::string key_path, int port)
{
	INFO << "started";

	try
	{

		SimpleWeb::SocketServer<SimpleWeb::WSS> server(crt_path, key_path);
		server.config.port = port;
		// Example 1: echo WebSocket Secure endpoint
		// Added debug messages for example use of the callbacks
		// Test with the following JavaScript:
		//   var wss=new WebSocket("wss://localhost:8080/echo");
		//   wss.onmessage=function(evt){console.log(evt.data);};
		//   wss.send("test");
		auto &echo = server.endpoint["^/echo/?$"];

		echo.on_message = [](std::shared_ptr<SimpleWeb::SocketServer<SimpleWeb::WSS>::Connection> connection, std::shared_ptr<SimpleWeb::SocketServer<SimpleWeb::WSS>::InMessage> in_message) {
			auto out_message = in_message->string();

			INFO << "Server: Message received: \"" << out_message << "\" from " << connection.get();

			INFO << "Server: Sending message \"" << out_message << "\" to " << connection.get();

			// connection->send is an asynchronous function
			connection->send(out_message, [](const SimpleWeb::error_code &ec) {
				if (ec)
				{
					// See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
					INFO << "Server: Error sending message. " << "Error: " << ec << ", error message: " << ec.message();
				}
			});

			// Alternatively use streams:
			// auto out_message = make_shared<SimpleWeb::SocketServer<SimpleWeb::WSS>::OutMessage>();
			// *out_message << in_message->string();
			// connection->send(out_message);
		};

		echo.on_open = [](std::shared_ptr<SimpleWeb::SocketServer<SimpleWeb::WSS>::Connection> connection) {
			INFO << "Server: Opened connection " << connection.get();
		};

		// See RFC 6455 7.4.1. for status codes
		echo.on_close = [](std::shared_ptr<SimpleWeb::SocketServer<SimpleWeb::WSS>::Connection> connection, int status, const std::string & /*reason*/) {
			INFO << "Server: Closed connection " << connection.get() << " with status code " << status;
		};

		// Can modify handshake response header here if needed
		echo.on_handshake = [](std::shared_ptr<SimpleWeb::SocketServer<SimpleWeb::WSS>::Connection> /*connection*/, SimpleWeb::CaseInsensitiveMultimap & /*response_header*/) {
			return SimpleWeb::StatusCode::information_switching_protocols; // Upgrade to websocket
		};

		// See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
		echo.on_error = [](std::shared_ptr<SimpleWeb::SocketServer<SimpleWeb::WSS>::Connection> connection, const SimpleWeb::error_code &ec) {
			INFO << "Server: Error in connection " << connection.get() << ". "
				 << "Error: " << ec << ", error message: " << ec.message();
		};

		// Example 2: Echo thrice
		// Demonstrating queuing of messages by sending a received message three times back to the client.
		// Concurrent send operations are automatically queued by the library.
		// Test with the following JavaScript:
		//   var wss=new WebSocket("wss://localhost:8080/echo_thrice");
		//   wss.onmessage=function(evt){console.log(evt.data);};
		//   wss.send("test");
		auto &echo_thrice = server.endpoint["^/echo_thrice/?$"];
		echo_thrice.on_message = [](std::shared_ptr<SimpleWeb::SocketServer<SimpleWeb::WSS>::Connection> connection, std::shared_ptr<SimpleWeb::SocketServer<SimpleWeb::WSS>::InMessage> in_message) {
			auto out_message = std::make_shared<std::string>(in_message->string());

			connection->send(*out_message, [connection, out_message](const SimpleWeb::error_code &ec) {
				if (!ec)
					connection->send(*out_message); // Sent after the first send operation is finished
			});
			connection->send(*out_message); // Most likely queued. Sent after the first send operation is finished.
		};

		// Example 3: Echo to all WebSocket Secure endpoints
		// Sending received messages to all connected clients
		// Test with the following JavaScript on more than one browser windows:
		//   var wss=new WebSocket("wss://localhost:8080/echo_all");
		//   wss.onmessage=function(evt){console.log(evt.data);};
		//   wss.send("test");
		auto &echo_all = server.endpoint["^/echo_all/?$"];
		echo_all.on_message = [&server](std::shared_ptr<SimpleWeb::SocketServer<SimpleWeb::WSS>::Connection> /*connection*/, std::shared_ptr<SimpleWeb::SocketServer<SimpleWeb::WSS>::InMessage> in_message) {
			auto out_message = in_message->string();

			// echo_all.get_connections() can also be used to solely receive connections on this endpoint
			for (auto &a_connection : server.get_connections())
				a_connection->send(out_message);
		};

		// Start server and receive assigned port when server is listening for requests
		std::promise<unsigned short> server_port;
		std::thread server_thread([&server, &server_port]() {
			// Start server
			server.start([&server_port](unsigned short port) {
				server_port.set_value(port);
			});
		});
		INFO << "Server listening on port " << server_port.get_future().get();

		while (true) {
			pause();
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
