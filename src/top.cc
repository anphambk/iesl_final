#include "top.h"

top::top(const sc_module_name& nm)
    : sc_module(nm)
    , cpu("cpu")
    , dram("dram", 32, 0x650000)
    , eyeriss_0("eyeriss", m_hsize, m_vsize)
{
    unsigned int address;
    SC_REPORT_INFO_VERB(name(), "Building ...", SC_FULL);
    cpu.socket.bind(eyeriss_0.cpu_socket);
    eyeriss_0.dram_socket.bind(dram.port);
    //
    address = sknobs_get_value(format("%s.image_address", name()), 0);
    dram.initialize(address, sknobs_get_string(format("%s.image_file", name()), "image.txt"));
    address = sknobs_get_value(format("%s.filter_address", name()), 0);
    dram.initialize(address, sknobs_get_string(format("%s.filter_file", name()), "filter.txt"));
}

void top::end_of_simulation ()
{
    unsigned int address = sknobs_get_value(format("%s.psum_address", name()), 0);
    int W = sknobs_get_value("top.cpu.image_width", 5);
    int K = sknobs_get_value("top.cpu.filter_size", 3);
    int S = sknobs_get_value("top.cpu.filter_stride", 1);
    int num_filter = sknobs_get_value("top.cpu.filter_number", 1);
    int length = ((W-K)/S+1)*((W-K)/S+1)*num_filter;
    dram.dump(address, length);
}
