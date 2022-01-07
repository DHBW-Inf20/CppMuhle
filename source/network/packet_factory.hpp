#ifndef PACKET_FACTORY_HPP
#define PACKET_FACTORY_HPP

#include <map>
#include "packets/packet.hpp"

#include "packets/packet_socket_connect.cpp"
#include "packets/packet_socket_disconnect.cpp"
#include "packets/packet_message.cpp"
#include "packets/packet_login.cpp"
#include "packets/packet_muhle_field.cpp"
#include "packets/packet_game_request.cpp"
#include "packets/packet_game_code.cpp"
#include "packets/packet_game_place.cpp"
#include "packets/packet_game_attack.cpp"
#include "packets/packet_game_move.cpp"
#include "packets/packet_game_jump.cpp"

class packet_factory
{
private:
    std::map<char, packet*> packet_map;

    void initialize();
    void register_packet(packet*);
public:
    packet_factory();
    ~packet_factory();
    packet* get_packet_from_id(char);
};

#endif
