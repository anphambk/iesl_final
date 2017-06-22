#include "controller.h"

controller::controller (const sc_module_name& nm)
    : sc_module(nm)
    , cpu_socket("cpu_socket")
    , dram_socket("dram_socket")
    , sram_socket("isram_socket")
{
    SC_REPORT_INFO_VERB(name(), "Building ...", SC_FULL);
    cpu_socket.register_b_transport(this, &controller::cpu_transport);
}

//@
void controller::cpu_transport(tlm_generic_payload& trans, sc_time& delay)
{
   
    memcpy(m_ins.dptr, trans.get_data_ptr(), sizeof(instruction_payload));
    int C = m_ins.s.image_depth;
    int M = m_ins.s.filter_number;
    int pe_channel = sknobs_get_value("top.cpu.pe_channel",0);
    int pe_filter = sknobs_get_value("top.cpu.pe_filter",0);
    for(int c=0;c<C;c++){
        for(int m=0;m<M;m++){
            dma_payload_u dma;
            instruction_payload_u ins;
            tlm_generic_payload dma_gp;
            tlm_generic_payload ins_gp;
            memcpy(m_ins.dptr, trans.get_data_ptr(), sizeof(instruction_payload));

            dma_gp.set_data_ptr(dma.dptr);
            ins_gp.set_data_ptr(ins.dptr);
            //
            dma.s.direction = 0;
            dma.s.length = m_ins.s.image_width * m_ins.s.image_height * sizeof(float);
            dma.s.address0 = m_ins.s.image_address + c * dma.s.length;
            dma.s.address1 = 0; // sram address
            dma_gp.set_response_status(TLM_INCOMPLETE_RESPONSE);
            delay += SYS_CLK;
            dram_socket->b_transport(dma_gp, delay);
            //
            dma.s.direction = 0;
            dma.s.length = m_ins.s.filter_size * m_ins.s.filter_size * sizeof(float);
            dma.s.address0 = m_ins.s.filter_address + (c*M + m) * dma.s.length;
            //dma.s.address1 = 0x00001000; // sram address (allocate later)
            dma.s.address1 = m_ins.s.image_width * m_ins.s.image_height * sizeof(float);
            dma_gp.set_response_status(TLM_INCOMPLETE_RESPONSE);
            delay += SYS_CLK;
            dram_socket->b_transport(dma_gp, delay);
            //
            ins = m_ins;
            ins.s.image_address = 0x00000000;
            //ins.s.filter_address = 0x00001000; // later
            ins.s.filter_address = dma.s.address1;
            //ins.s.psum_address = 0x00002000; // calcuate later
            ins.s.psum_address = ins.s.filter_address + m_ins.s.filter_size * m_ins.s.filter_size * sizeof(float);
            ins_gp.set_response_status(TLM_INCOMPLETE_RESPONSE);
            delay += SYS_CLK;
            sram_socket->b_transport(ins_gp, delay);
            //
            dma.s.direction = 1;
            {
                unsigned int width = m_ins.s.image_width - m_ins.s.filter_size + (m_ins.s.filter_size % 2);
                unsigned int height = m_ins.s.image_height - m_ins.s.filter_size + (m_ins.s.filter_size % 2);
                dma.s.length = width * height * sizeof(float);
            }
            dma.s.address0 = m_ins.s.psum_address + m * dma.s.length;
            //dma.s.address1 = 0x00002000; // should match the above value
            dma.s.address1 = ins.s.psum_address;
            dma_gp.set_response_status(TLM_INCOMPLETE_RESPONSE);
            delay += SYS_CLK;
            dram_socket->b_transport(dma_gp, delay);
            //
            wait(delay);
        }
    }
}
