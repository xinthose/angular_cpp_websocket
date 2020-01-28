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

void websocket::initialize(std::string wss_server)
{
	boost::mutex::scoped_lock lock (mutex);

	INFO << "started";

	try
	{
		INFO << "connect to server >> wss_server = " << wss_server;
		h.connect(wss_server);

		INFO << "bind events";
		h.socket()->on("new_screen", std::bind(&websocket::new_screen, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
		h.socket()->on("trans_started", std::bind(&websocket::trans_started, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
		h.socket()->on("user_logged_in", std::bind(&websocket::user_logged_in, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
		h.socket()->on("user_logged_out", std::bind(&websocket::user_logged_out, this, std::placeholders::_1));

	}
	catch (const std::exception &e)
	{
		ERROR << "e.what() = " << e.what();
		throw -1;
	}

	INFO << "ended";

	return;
}

void websocket::emit_event(std::string event)
{
	boost::mutex::scoped_lock lock (mutex);

	INFO << "started";

	try
	{
		sio::message::list msg_list;
		h.socket()->emit(event);
	}
	catch (const std::exception &e)
	{
		ERROR << "e.what() = " << e.what();
		throw -1;
	}

	INFO << "ended";

	return;
}

void websocket::emit_event_data(std::string event, boost::property_tree::ptree pt)
{
	boost::mutex::scoped_lock lock (mutex);

	INFO << "started";

	try
	{
		// sio::message::list msg_list;
		// create data to send
		sio::message::ptr msg_ptr = sio::object_message::create();
		msg_ptr->get_map()["flowAcc"] = sio::double_message::create(pt.get<double>("flowAcc"));
		msg_ptr->get_map()["flowRate"] = sio::double_message::create(pt.get<double>("flowRate"));
		msg_ptr->get_map()["flowPH"] = sio::double_message::create(pt.get<double>("flowPH"));
		msg_ptr->get_map()["flowConductivity"] = sio::double_message::create(pt.get<double>("flowConductivity"));
		msg_ptr->get_map()["highLevel"] = sio::bool_message::create(pt.get<bool>("highLevel"));
		msg_ptr->get_map()["sampleInProgress"] = sio::bool_message::create(pt.get<bool>("sampleInProgress"));

		// emit event
		h.socket()->emit(event, msg_ptr);
	}
	catch (const std::exception &e)
	{
		ERROR << "e.what() = " << e.what();
		throw -1;
	}

	INFO << "ended";

	return;
}

// Event callbacks

void websocket::new_screen(const std::string& name, sio::message::ptr const& message, bool need_ack, sio::message::list& ack_message)
{
	boost::mutex::scoped_lock lock (mutex);

	INFO << "started";

	int new_screen = 0;
	bool debug = false;
	
	try
	{
		new_screen  = message->get_map()["screen"]->get_int();
		INFO << "new_screen = " << new_screen;
		current_screen.push(new_screen);

		if (debug) {
			INFO << "name = " << name << "; need_ack = " << need_ack;
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

void websocket::trans_started(const std::string& name, sio::message::ptr const& message, bool need_ack, sio::message::list& ack_message)
{
	boost::mutex::scoped_lock lock (mutex);

	INFO << "started";
	
	try
	{
		// get data
		trans_started_data.product_type = message->get_map()["product_type"]->get_int();
		trans_started_data.product_type_sub_type = message->get_map()["product_type_sub_type"]->get_int();
		trans_started_data.requested_volume = message->get_map()["requested_volume"]->get_int();
		trans_started_data.manifest_number = message->get_map()["manifest_number"]->get_string();
		INFO << "product_type = " << trans_started_data.product_type << "; product_type_sub_type = " << trans_started_data.product_type_sub_type << "; requested_volume = " << trans_started_data.requested_volume << "; manifest_number = " << trans_started_data.manifest_number;
		
		// push event sequence
		seq_events.push(3);	// From JavaScript: start flow
	}
	catch (const std::exception &e)
	{
		ERROR << "e.what() = " << e.what();
		throw -1;
	}

	INFO << "ended";

	return;
}

void websocket::user_logged_in(const std::string& name, sio::message::ptr const& message, bool need_ack, sio::message::list& ack_message)
{
	boost::mutex::scoped_lock lock (mutex);

	INFO << "started";
	
	try
	{
		// get data
		user.access_number = message->get_map()["access_number"]->get_string();
		user.truck_number = message->get_map()["truck_number"]->get_int();
		user.fill_point_num = message->get_map()["fill_point_num"]->get_int();
		INFO << "access_number = " << user.access_number << "; truck_number = " << user.truck_number << "; fill_point_num = " << user.fill_point_num;
		
		// push event sequence
		seq_events.push(8);	// From JavaScript: user logged in
	}
	catch (const std::exception &e)
	{
		ERROR << "e.what() = " << e.what();
		throw -1;
	}

	INFO << "ended";

	return;
}

void websocket::user_logged_out(sio::event &event)
{
	boost::mutex::scoped_lock lock (mutex);

	INFO << "started";
	
	try
	{
		// push event sequence
		seq_events.push(9);	// From JavaScript: user logged out
	}
	catch (const std::exception &e)
	{
		ERROR << "e.what() = " << e.what();
		throw -1;
	}

	INFO << "ended";

	return;
}
