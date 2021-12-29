#ifndef CLIENT
#define CLIENT

#include <functional>
#include <vector>
#include <map>
#include <boost/asio.hpp>
#include "packet/packet_factory.hpp"

using tcp = boost::asio::ip::tcp;
using error_code_t = boost::system::error_code;

typedef struct connection
{
    tcp::socket socket;
    boost::asio::streambuf buf;
    connection(boost::asio::io_service &io_service) : socket(io_service)
    {
    }
    ~connection()
    {
        socket.close();
    }

} connection_t;

class client
{
private:
    ::packet_factory packet_factory;

    // std::map<int, std::vector<std::function<void(int id, packet* packet)>>> listeners;

    boost::asio::io_service io_service;
    tcp::endpoint endpoint;

    void handle_read(std::shared_ptr<connection_t> con);

    // std::unique_ptr<std::thread> client_thread;

public:
    std::map<int, std::vector<std::function<void(packet* packet)>>> listeners;

    client(std::string address, int port);
    ~client();
    std::string get_address();
    int get_port();
    void start();

    void send_packet(packet*);
    template <typename P>
    void register_packet_listener(std::function<void(P *packet)>);
};

#endif
