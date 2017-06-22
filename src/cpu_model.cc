#include "cpu_model.h"

//@
cpu_model::cpu_model (const sc_module_name& nm)
    : sc_module(nm)
    , socket("socket")
{
    SC_REPORT_INFO_VERB(name(), "Building ...", SC_FULL);
    SC_THREAD(operation);
}

//@
void cpu_model::operation ()
{
    instruction_payload_u ins;
    tlm_generic_payload gp;
    sc_time delay = SC_ZERO_TIME;
    
    ins.s.image_address = sknobs_get_value(format("%s.image_address", name()), 0);
    ins.s.image_width = sknobs_get_value(format("%s.image_width", name()), 0);
    ins.s.image_height = sknobs_get_value(format("%s.image_height", name()), 0);
    ins.s.image_depth = sknobs_get_value(format("%s.image_depth", name()), 0);
    ins.s.filter_address = sknobs_get_value(format("%s.filter_address", name()), 0);
    ins.s.filter_size = sknobs_get_value(format("%s.filter_size", name()), 0);
    ins.s.filter_stride = sknobs_get_value(format("%s.filter_stride", name()), 0);
    ins.s.filter_number = sknobs_get_value(format("%s.filter_number", name()), 0);
    ins.s.psum_address = sknobs_get_value(format("%s.psum_address", name()), 0);
    gp.set_data_ptr(ins.dptr);
    gp.set_response_status(TLM_INCOMPLETE_RESPONSE);
    socket->b_transport(gp, delay);
    wait(delay);
    sc_stop();
}
