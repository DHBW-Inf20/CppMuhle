#include "../packet.hpp"
#include <iostream>
#include <string>

class packet_hello_world: public packet
{
public:
    std::string str;

    packet_data_t serialize() 
    {
        char* data = (char*) str.c_str();
        packet_data_t packet_data;
        packet_data.data = data;
        packet_data.size = str.length();
        return packet_data;
    }

    void deserialize(packet_data_t packet_data)
    {
        str = std::string(packet_data.data);
    }

    packet* clone()
    {
        return new packet_hello_world();
    }

    char get_id()
    {
        return 0;
    }
};

// int main() 
// {
//     packet_hello_world* phw = new packet_hello_world();
//     packet_data_t serialized = phw->serialize();
//     std::cout << serialized.data << std::endl;
// }
