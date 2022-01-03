#include "packet.hpp"

class packet_socket_disconnect: public packet
{
public:

    packet_data_t serialize() 
    {
        return packet_data_t();
    }

    void deserialize(packet_data_t &packet_data)
    {
    }

    packet* clone()
    {
        return new packet_socket_disconnect();
    }

    char get_id()
    {
        return -2;
    }
};
