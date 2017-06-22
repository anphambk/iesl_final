
#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "common.h"
#include "protocols.h"

class controller : public sc_module
{
public:
    simple_target_socket< controller > cpu_socket;
    simple_initiator_socket< controller > dram_socket;
    simple_initiator_socket< controller > sram_socket;
    
protected:
    instruction_payload_u m_ins;
    
public:
    controller (const sc_module_name& nm);
    
protected:
    void cpu_transport(tlm_generic_payload& trans, sc_time& delay);
};

#endif /* __CONTROLLER_H__ */
