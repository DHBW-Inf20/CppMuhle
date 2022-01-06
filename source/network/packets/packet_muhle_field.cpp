#include "packet.hpp"
#include <string>
#include "../../logic/helper_types.hpp"

#include <bitset>
#include <iostream>


/*
* packet_muhle_field
* Used to send the updated muhle data to the client
*/
class packet_muhle_field: public packet
{
public:
    int24 black;
    int24 white;

    packet_data_t serialize() 
    {
        char* data = (char*) malloc(2 * sizeof(int24));
        memcpy(data, &black, sizeof(int24));
        memcpy(data + sizeof(int24), &white, sizeof(int24));
        packet_data_t packet_data;
        packet_data.data = data;
        packet_data.size = 2 * sizeof(int24);
        // std::cout << std::bitset<64>(data) << std::endl;
        return packet_data;
    }

    void deserialize(packet_data_t &packet_data)
    {
        memcpy(&black, packet_data.data, sizeof(int24));
        memcpy(&white, packet_data.data + sizeof(int24), sizeof(int24));
        // std::cout << std::bitset<64>(&packet_data.data) << std::endl;

    }

    packet* clone()
    {
        return new packet_muhle_field();
    }

    char get_id()
    {
        return 2;
    }
};