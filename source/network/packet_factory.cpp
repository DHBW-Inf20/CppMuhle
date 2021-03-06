#include "packet_factory.hpp"

packet_factory::packet_factory()
{
    this->initialize();
}

packet_factory::~packet_factory()
{
    for (auto &map : packet_map)
    {
        delete map.second;
    }
}

void packet_factory::register_packet(packet* packet)
{
    this->packet_map[packet->get_id()] = packet;
}

void packet_factory::initialize()
{
    this->register_packet(new packet_message());
    this->register_packet(new packet_login());
    this->register_packet(new packet_muhle_field());
    this->register_packet(new packet_game_request());
    this->register_packet(new packet_game_code());
    this->register_packet(new packet_game_code_not_found());
    this->register_packet(new packet_game_place());
    this->register_packet(new packet_game_attack());
    this->register_packet(new packet_game_move());
    this->register_packet(new packet_game_jump());
    this->register_packet(new packet_exchange_names());
    this->register_packet(new packet_game_ended());
}

packet* packet_factory::get_packet_from_id(char id)
{
    if (packet_map.find(id) == packet_map.end()) return nullptr;
    return this->packet_map[id]->clone(); 
}
