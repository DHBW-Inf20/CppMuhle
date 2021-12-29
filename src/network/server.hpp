#ifndef SERVER
#define SERVER

#include <boost/asio.hpp>

using tcp = boost::asio::ip::tcp;
using error_code_t = boost::system::error_code;

int new_id = 0;

typedef struct connection
{
    int id;
    tcp::socket socket;
    boost::asio::streambuf buf;
    connection(boost::asio::io_service &io_service) : socket(io_service)
    {
        id = new_id++;
    }
    ~connection()
    {
        socket.close();
    }

} connection_t;

class server
{
    private:
        boost::asio::io_service io_service;
        tcp::endpoint endpoint;
        tcp::acceptor acceptor{io_service};

        void handle_accept();
        void handle_read(std::shared_ptr<connection_t> con);

        std::unique_ptr<std::thread> server_thread;

    public:
        server(int);
        ~server();
        int get_port();
        void start();
};

#endif
