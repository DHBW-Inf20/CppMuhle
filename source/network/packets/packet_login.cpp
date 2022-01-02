#include "packet.hpp"
#include <iostream>
#include <string>

class packet_login: public packet
{
public:
    std::string name;

    packet_data_t serialize() 
    {
        char* data = (char*) name.c_str();
        packet_data_t packet_data;
        packet_data.data = data;
        packet_data.size = name.length();
        return packet_data;
    }

    void deserialize(packet_data_t &packet_data)
    {
        name = std::string(packet_data.data, packet_data.size);
    }

    void free(packet_data_t &packet_data)
    {
    }

    packet* clone()
    {
        return new packet_login();
    }

    char get_id()
    {
        return 1;
    }
};
