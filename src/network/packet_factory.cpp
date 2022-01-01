#include "packet_factory.hpp"

packet_factory::packet_factory()
{
    this->initialize();
}

void packet_factory::register_packet(packet* packet)
{
    this->packet_map[packet->get_id()] = packet;
}

void packet_factory::initialize()
{
    this->register_packet(new packet_hello_world());
}

packet* packet_factory::get_packet_from_id(char id)
{
    if (packet_map.find(id) == packet_map.end()) return nullptr;
    return this->packet_map[id]->clone(); 
}
