#ifndef CLIENT
#define CLIENT

#include <boost/asio.hpp>

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
        boost::asio::io_service io_service;
        tcp::endpoint endpoint;

        std::unique_ptr<std::thread> server_thread;

        void handle_read(std::shared_ptr<connection_t> con);

    public:
        client(std::string address, int port);
        ~client();
        std::string get_address();
        int get_port();
        void start();
};

#endif
