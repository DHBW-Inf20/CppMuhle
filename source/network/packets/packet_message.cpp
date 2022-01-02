#include "packet.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include <string>

class packet_message: public packet
{
public:
    std::string str;

    packet_data_t serialize() 
    {
        // char* data = (char*) str.c_str();
        char* data = (char*) std::malloc(str.length());
        std::memcpy(data, str.c_str(), str.length());
        packet_data_t packet_data;
        packet_data.data = data;
        packet_data.size = str.length();
        return packet_data;
    }

    void deserialize(packet_data_t &packet_data)
    {
        str = std::string(packet_data.data, packet_data.size);
    }

    void free(packet_data_t &packet_data)
    {
        delete[] packet_data.data;
    }

    packet* clone()
    {
        return new packet_message();
    }

    char get_id()
    {
        return 0;
    }
};
