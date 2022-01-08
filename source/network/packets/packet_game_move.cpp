#include "packet.hpp"
#include <string>
#include <bitset>
#include <iostream>


/*
* packet_muhle_field
* Used to send the updated muhle data to the client
*/
class packet_game_move: public packet
{
public:
    int from;
    int to;
    
    packet_data_t serialize() 
    {
        char* data = (char*) malloc(sizeof(int)*2);
        memcpy(data, &from, sizeof(int));
        memcpy(data + sizeof(int), &to, sizeof(int));
        packet_data_t packet_data;
        packet_data.data = data;
        packet_data.size = sizeof(int)*2;
        return packet_data;
    }

    void deserialize(packet_data_t &packet_data)
    {
        memcpy(&to, packet_data.data, sizeof(int));
        memcpy(&from, packet_data.data, sizeof(int)*2);
    }

    packet* clone()
    {
        return new packet_game_move();
    }

    char get_id()
    {
        return PACKET_GAME_MOVE_ID;
    }
};