
#ifndef __SRAM_CONTROLLER_H__
#define __SRAM_CONTROLLER_H__

#include "common.h"
#include "protocols.h"

class sram_controller : public sc_module
{
public:
    static const unsigned int IBUFFER_SIZE = 100;
    static const unsigned int FBUFFER_SIZE = 100;
    static const unsigned int PBUFFER_SIZE = 100;
    
    simple_target_socket< sram_controller > control_socket;
    simple_initiator_socket< sram_controller > isram_socket;
    simple_initiator_socket< sram_controller > fsram_socket;
    simple_initiator_socket< sram_controller > psram_socket;
    simple_initiator_socket< sram_controller > ipe_socket;
    simple_initiator_socket< sram_controller > fpe_socket;
    multi_passthrough_initiator_socket< sram_controller > ippe_socket;
    multi_passthrough_target_socket< sram_controller > oppe_socket;
    
protected:
    unsigned int m_size;
    instruction_payload_u m_ins;
    sc_event command_start;
    sc_event command_finish;
    sc_event image_done;
    sc_event filter_done;
    tlm_fifo< tlm_generic_payload* > m_fifo;
    
public:
    sram_controller (const sc_module_name& nm);
    
protected:
    SC_HAS_PROCESS(sram_controller);
    void end_of_elaboration();
    void control_transport(tlm_generic_payload& trans, sc_time& delay);
    void opsum_transport (int id, tlm_generic_payload& trans, sc_time& delay);
    void image_operation ();
    void filter_operation ();
    void psum_operation ();
    void opsum_operation ();
};

#endif /* __SRAM_CONTROLLER_H__ */
