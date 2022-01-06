#ifndef NETWORK_CONTROLLER_HPP
#define NETWORK_CONTROLLER_HPP

/* network_controller.hpp
 *
 *  Used as a main interface between the server and business-Logic
*/
class network_controller
{
    public:
        network_controller();
        ~network_controller();
        void run();
};

#endif
