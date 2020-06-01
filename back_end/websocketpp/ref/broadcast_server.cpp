#include <websocketpp/config/asio_no_tls.hpp>

#include <websocketpp/server.hpp>

#include <iostream>
#include <set>

/*#include <boost/websocketpp::lib::thread.hpp>
#include <boost/websocketpp::lib::thread/websocketpp::lib::mutex.hpp>
#include <boost/websocketpp::lib::thread/websocketpp::lib::condition_variable;.hpp>*/
#include <websocketpp/common/thread.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

/* on_open insert websocketpp::connection_hdl into channel
 * on_close remove websocketpp::connection_hdl from channel
 * on_message queue send to all channels
 */

enum event_type
{
    SUBSCRIBE,
    UNSUBSCRIBE,
    MESSAGE
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

class broadcast_server
{
public:
    broadcast_server()
    {
        // Initialize Asio Transport
        m_server.init_asio();

        // Register handler callbacks
        m_server.set_open_handler(websocketpp::lib::bind(&broadcast_server::on_open, this, ::websocketpp::lib::placeholders::_1));
        m_server.set_close_handler(websocketpp::lib::bind(&broadcast_server::on_close, this, ::websocketpp::lib::placeholders::_1));
        m_server.set_message_handler(websocketpp::lib::bind(&broadcast_server::on_message, this, ::websocketpp::lib::placeholders::_1, ::websocketpp::lib::placeholders::_2));
    }

    void run(uint16_t port)
    {
        // listen on specified port
        m_server.listen(port);

        // Start the server accept loop
        m_server.start_accept();

        // Start the ASIO io_service run loop
        try
        {
            m_server.run();
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
    }

    void on_open(websocketpp::connection_hdl hdl)
    {
        {
            websocketpp::lib::lock_guard<websocketpp::lib::mutex> guard(m_event_lock);
            //std::cout << "on_open" << std::endl;
            m_events.push(event(SUBSCRIBE, hdl));
        }
        m_event_cond.notify_one();
    }

    void on_close(websocketpp::connection_hdl hdl)
    {
        {
            websocketpp::lib::lock_guard<websocketpp::lib::mutex> guard(m_event_lock);
            //std::cout << "on_close" << std::endl;
            m_events.push(event(UNSUBSCRIBE, hdl));
        }
        m_event_cond.notify_one();
    }

    void on_message(websocketpp::connection_hdl hdl, server::message_ptr msg)
    {
        // queue message up for sending by processing websocketpp::lib::thread
        {
            websocketpp::lib::lock_guard<websocketpp::lib::mutex> guard(m_event_lock);
            //std::cout << "on_message" << std::endl;
            m_events.push(event(MESSAGE, hdl, msg));
        }
        m_event_cond.notify_one();
    }

    void process_messages()
    {
        while (1)
        {
            websocketpp::lib::unique_lock<websocketpp::lib::mutex> lock(m_event_lock);

            while (m_events.empty())
            {
                m_event_cond.wait(lock);
            }

            event a = m_events.front();
            m_events.pop();

            lock.unlock();

            if (a.type == SUBSCRIBE)
            {
                websocketpp::lib::lock_guard<websocketpp::lib::mutex> guard(m_connection_lock);
                m_connections.insert(a.hdl);
            }
            else if (a.type == UNSUBSCRIBE)
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
                // undefined.
            }
        }
    }

private:
    typedef std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> con_list;

    server m_server;
    con_list m_connections;
    std::queue<event> m_events;

    websocketpp::lib::mutex m_event_lock;
    websocketpp::lib::mutex m_connection_lock;
    websocketpp::lib::condition_variable m_event_cond;
};

int main()
{
    try
    {
        broadcast_server server_instance;

        // Start a websocketpp::lib::thread to run the processing loop
        websocketpp::lib::thread t(websocketpp::lib::bind(&broadcast_server::process_messages, &server_instance));

        // Run the asio loop with the main websocketpp::lib::thread
        server_instance.run(9002);

        t.join();
    }
    catch (websocketpp::exception const &e)
    {
        std::cout << e.what() << std::endl;
    }
}
