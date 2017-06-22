#ifndef __BROADCAST_H__
#define __BROADCAST_H__

#include "common.h"

class broadcast : public sc_module
{
public:
    simple_target_socket< broadcast > isocket;
    multi_passthrough_initiator_socket< broadcast > osocket;

protected:
    unsigned int m_size;

public:
    broadcast (const sc_module_name& nm);
    
protected:
    void end_of_elaboration();
    void b_transport(tlm_generic_payload& trans, sc_time& delay);
};

#endif /* __BROADCAST_H__ */
