#include "sram_controller.h"

//@
sram_controller::sram_controller (const sc_module_name& nm)
    : sc_module(nm)
    , control_socket("control_socket")
    , isram_socket("isram_socket")
    , fsram_socket("fsram_socket")
    , psram_socket("psram_socket")
    , ipe_socket("ipe_socket")
    , fpe_socket("fpe_socket")
    , ippe_socket("ippe_socket")
    , oppe_socket("oppe_socket")
{
    SC_REPORT_INFO_VERB(name(), "Building ...", SC_FULL);
    control_socket.register_b_transport(this, &sram_controller::control_transport);
    oppe_socket.register_b_transport(this, &sram_controller::opsum_transport);
    SC_THREAD(image_operation);
    SC_THREAD(filter_operation);
    SC_THREAD(psum_operation);
    SC_THREAD(opsum_operation);
}

//@
void sram_controller::end_of_elaboration()
{
    assert(ippe_socket.size() == ippe_socket.size());
    m_size = ippe_socket.size();
}

//@
void sram_controller::control_transport (tlm_generic_payload& trans, sc_time& delay)
{
    memcpy(m_ins.dptr, trans.get_data_ptr(), sizeof(instruction_payload));
    delay += SYS_CLK;
    command_start.notify(delay);
    wait(command_finish);
    trans.set_response_status(TLM_OK_RESPONSE);
}

//@
void sram_controller::image_operation ()
{
    tlm_generic_payload gp;
    sc_time delay = SC_ZERO_TIME;
    unsigned char data [IBUFFER_SIZE];
    unsigned int width;
    unsigned int height;
    unsigned int address;
    
    gp.set_data_ptr(data);
    while(true)
    {
        wait(command_start);
        address = m_ins.s.image_address;
        width = m_ins.s.image_width;
        height = m_ins.s.image_height;
        gp.set_data_length(width * sizeof(float));
        for (int i = 0; i < height; i++)
        {
            // read phase
            delay = SC_ZERO_TIME;
            gp.set_address(address + i * gp.get_data_length());
            gp.set_command(TLM_READ_COMMAND);
            gp.set_response_status(TLM_INCOMPLETE_RESPONSE);
            isram_socket->b_transport(gp, delay);
            wait(delay);
            // write phase
            delay = SC_ZERO_TIME;
            gp.set_address(i);
            gp.set_command(TLM_WRITE_COMMAND);
            gp.set_response_status(TLM_INCOMPLETE_RESPONSE);
            ipe_socket->b_transport(gp, delay);
            wait(delay);
            SC_REPORT_INFO(name(), format("Sent image %s", gp_to_string(gp)));
        }
        image_done.notify();
    }
}

//@
void sram_controller::filter_operation ()
{
    tlm_generic_payload gp;
    sc_time delay = SC_ZERO_TIME;
    unsigned char data [FBUFFER_SIZE];
    unsigned int width;
    unsigned int height;
    unsigned int address;
    
    gp.set_data_ptr(data);
    while(true)
    {
        wait(command_start);
        address = m_ins.s.filter_address;
        width = m_ins.s.filter_size;
        height = m_ins.s.filter_size;
        gp.set_data_length(width * sizeof(float));
        for (int i = 0; i < height; i++)
        {
            // read phase
            delay = SC_ZERO_TIME;
            gp.set_address(address + i * gp.get_data_length());
            gp.set_command(TLM_READ_COMMAND);
            gp.set_response_status(TLM_INCOMPLETE_RESPONSE);
            fsram_socket->b_transport(gp, delay);
            wait(delay);
            // write phase
            delay = SC_ZERO_TIME;
            gp.set_address(i);
            gp.set_command(TLM_WRITE_COMMAND);
            gp.set_response_status(TLM_INCOMPLETE_RESPONSE);
            fpe_socket->b_transport(gp, delay);
            wait(delay);
            SC_REPORT_INFO(name(), format("Sent filter %s", gp_to_string(gp)));
        }
        filter_done.notify();
    }
}

//@
void sram_controller::psum_operation ()
{
    tlm_generic_payload gp;
    sc_time delay = SC_ZERO_TIME;
    unsigned char data [PBUFFER_SIZE];
    unsigned int width;
    unsigned int height;
    unsigned int address;
    
    gp.set_data_ptr(data);
    while(true)
    {
        wait(image_done & filter_done);
        address = m_ins.s.psum_address;
        width = m_ins.s.image_width - m_ins.s.filter_size + (m_ins.s.filter_size % 2);
        height = m_size;
        gp.set_data_length(width * sizeof(float));
        for (int i = 0; i < height; i++)
        {
            // read phase
            delay = SC_ZERO_TIME;
            if (m_ins.s.psum_accumulate == 1)
            {
                gp.set_address(address + i * gp.get_data_length());
                gp.set_command(TLM_READ_COMMAND);
                gp.set_response_status(TLM_INCOMPLETE_RESPONSE);
                psram_socket->b_transport(gp, delay);
            }
            else
            {
                memset(data, 0, width);//zero??
                delay += SYS_CLK;
            }
            wait(delay);
            // write phase
            delay = SC_ZERO_TIME;
            gp.set_command(TLM_WRITE_COMMAND);
            gp.set_response_status(TLM_INCOMPLETE_RESPONSE);
            gp.set_address(m_ins.s.psum_address + i * gp.get_data_length());
            ippe_socket[i]->b_transport(gp, delay);
            wait(delay);
            SC_REPORT_INFO(name(), format("Sent psum %s", gp_to_string(gp)));
        }
    }
}

//@
void sram_controller::opsum_transport (int id, tlm_generic_payload& trans, sc_time& delay)
{
    SC_REPORT_INFO(name(), format("Received psum %s", gp_to_string(trans)));
    trans.set_address(m_ins.s.psum_address + id * trans.get_data_length());
    m_fifo.put(&trans);
    delay += trans.get_data_length() / sizeof(float) * SYS_CLK;
}

//@
void sram_controller::opsum_operation()
{
    tlm_generic_payload* gp;
    sc_time delay;
    unsigned int cnt = 0;
    while(true)
    {
        gp = m_fifo.get();
        wait(gp->get_data_length() / sizeof(float) * SYS_CLK);
        delay = SC_ZERO_TIME;
        gp->set_command(TLM_WRITE_COMMAND);
        gp->set_response_status(TLM_INCOMPLETE_RESPONSE);
        psram_socket->b_transport(*gp, delay);
        wait(delay);
        SC_REPORT_INFO(name(), format("Written psum %s", gp_to_string(*gp)));

        if (++cnt == m_size)
        {
            cnt = 0;
            command_finish.notify();
        }
    }
}
