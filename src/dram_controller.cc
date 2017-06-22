#include "dram_controller.h"

//@
dram_controller::dram_controller (const sc_module_name& nm)
    : sc_module(nm)
    , control_socket("control_socket")
    , dram_socket("dram_socket")
    , sram_socket("sram_socket")
    , m_peq("m_peq")
{
    SC_REPORT_INFO_VERB(name(), "Building ...", SC_FULL);
    control_socket.register_b_transport(this, &dram_controller::b_transport);
}

//@
void dram_controller::b_transport (tlm_generic_payload& trans, sc_time& delay)
{
    tlm_generic_payload gp;
    unsigned char buffer[10000];
    sc_time int_delay;
   
    memcpy(m_ins.dptr, trans.get_data_ptr(), sizeof(dma_payload));
    gp.set_data_ptr(buffer);
    
    gp.set_data_length(m_ins.s.length);
    if (m_ins.s.direction == 0)
    {
        // read phase
        int_delay = SC_ZERO_TIME;
        gp.set_command(TLM_READ_COMMAND);
        gp.set_address(m_ins.s.address0);
        gp.set_response_status(TLM_INCOMPLETE_RESPONSE);
        dram_socket->b_transport(gp, int_delay);
        wait(int_delay);
        // write phase
        int_delay = SC_ZERO_TIME;
        gp.set_command(TLM_WRITE_COMMAND);
        gp.set_address(m_ins.s.address1);
        gp.set_response_status(TLM_INCOMPLETE_RESPONSE);
        sram_socket->b_transport(gp, int_delay);
        wait(int_delay);
        SC_REPORT_INFO(name(), format("Sent DRAM->SRAM %s", gp_to_string(gp)));
    }
    else
    {
        // read phase
        int_delay = SC_ZERO_TIME;
        gp.set_command(TLM_READ_COMMAND);
        gp.set_address(m_ins.s.address1);
        gp.set_response_status(TLM_INCOMPLETE_RESPONSE);
        sram_socket->b_transport(gp, int_delay);
        wait(int_delay);
        // write phase
        int_delay = SC_ZERO_TIME;
        gp.set_command(TLM_WRITE_COMMAND);
        gp.set_address(m_ins.s.address0);
        gp.set_response_status(TLM_INCOMPLETE_RESPONSE);
        dram_socket->b_transport(gp, int_delay);
        wait(int_delay);
        SC_REPORT_INFO(name(), format("Sent SRAM->DRAM %s", gp_to_string(gp)));
    }
    delay += SYS_CLK;
}
