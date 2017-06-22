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
    int image_depth = m_ins.s.image_depth;
    int filter_number = m_ins.s.filter_number;
    int image_buffer = m_ins.s.image_width * m_ins.s.image_height * m_ins.s.image_depth * sizeof(float);
    int filter_bufer = m_ins.s.filter_size * m_ins.s.filter_size * sizeof(float);
    dma_payload_u dma;
    instruction_payload_u ins;
    tlm_generic_payload dma_gp;
    tlm_generic_payload ins_gp;

    dma_gp.set_data_ptr(dma.dptr);
    ins_gp.set_data_ptr(ins.dptr);
   
    // Buffer input feature map
    //dma.s.direction = 0;
    //dma.s.length = image_buffer;
    //dma.s.address0 = m_ins.s.image_address;
    //dma.s.address1 = 0;
    //dma_gp.set_response_status(TLM_INCOMPLETE_RESPONSE);
    //delay += SYS_CLK;
    //dram_socket->b_transport(dma_gp, delay);
    dma.s.direction = 0;
    dma.s.length = m_ins.s.image_width * m_ins.s.image_height * sizeof(float);
    for(int c = 0; c < image_depth; c++) {
        dma.s.address0 = m_ins.s.image_address + c * dma.s.length;
        dma.s.address1 = c * dma.s.length;
        dma_gp.set_response_status(TLM_INCOMPLETE_RESPONSE);
        delay += SYS_CLK;
        dram_socket->b_transport(dma_gp, delay);
    }

    // Calculate
    for(int m = 0; m < filter_number; m++){
        for(int c = 0; c < image_depth; c++){
            //
            dma.s.direction = 0;
            dma.s.length = filter_bufer;
            dma.s.address0 = m_ins.s.filter_address + m * filter_bufer * image_depth + c * filter_bufer;
            dma.s.address1 = image_buffer;
            dma_gp.set_response_status(TLM_INCOMPLETE_RESPONSE);
            delay += SYS_CLK;
            dram_socket->b_transport(dma_gp, delay);
            //
            ins = m_ins;
            ins.s.image_address = c * m_ins.s.image_width * m_ins.s.image_height * sizeof(float);
            ins.s.filter_address = image_buffer;
            ins.s.psum_address = image_buffer + filter_bufer;
            if(c == 0) ins.s.psum_accumulate = 0;
            else ins.s.psum_accumulate = 1;
            ins_gp.set_response_status(TLM_INCOMPLETE_RESPONSE);
            delay += SYS_CLK;
            sram_socket->b_transport(ins_gp, delay);
            //
            wait(delay);
        }
        // Write back result for each Synapse
        dma.s.direction = 1;
        unsigned int width = m_ins.s.image_width - m_ins.s.filter_size + (m_ins.s.filter_size % 2);
        unsigned int height = m_ins.s.image_height - m_ins.s.filter_size + (m_ins.s.filter_size % 2);
        dma.s.length = width * height * sizeof(float);
        dma.s.address0 = m_ins.s.psum_address + m * dma.s.length;
        dma.s.address1 = ins.s.psum_address;
        dma_gp.set_response_status(TLM_INCOMPLETE_RESPONSE);
        delay += SYS_CLK;
        dram_socket->b_transport(dma_gp, delay);
    }
}
