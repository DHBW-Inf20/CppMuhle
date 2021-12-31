#include "../packet.hpp"
#include <iostream>
#include <string>

class packet_hello_world: public packet
{
public:
    std::string str;

    char* serialize() 
    {
        char* data = (char*) str.c_str();
        return data;
    }

    void deserialize(char* data)
    {
        str = std::string(data);
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
