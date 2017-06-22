
#ifndef __DRAM_CONTROLLER_H__
#define __DRAM_CONTROLLER_H__

#include "common.h"
#include "protocols.h"

class dram_controller : public sc_module
{
public:
    simple_target_socket< dram_controller > control_socket;
    simple_initiator_socket< dram_controller > dram_socket;
    simple_initiator_socket< dram_controller > sram_socket;
    
protected:
    dma_payload_u m_ins;
    peq_with_get< tlm_generic_payload >  m_peq;
    
public:
    dram_controller (const sc_module_name& nm);

protected:
    void b_transport(tlm_generic_payload& trans, sc_time& delay);
};

#endif /* __DRAM_CONTROLLER_H__ */
