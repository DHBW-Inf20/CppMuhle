#ifndef PACKET
#define PACKET

class packet
{
public:
    virtual char* serialize() = 0;
    virtual void deserialize(char*) = 0;
    virtual char get_id() = 0;
    virtual packet* clone() = 0;
};

#endif