#include "top.h"

top::top(const sc_module_name& nm)
    : sc_module(nm)
    , cpu("cpu")
    , dram("dram", 32, m_ramsize)
    , eyeriss_0("eyeriss", m_hsize, m_vsize)
{
    unsigned int address;
    SC_REPORT_INFO_VERB(name(), "Building ...", SC_FULL);
    cpu.socket.bind(eyeriss_0.cpu_socket);
    eyeriss_0.dram_socket.bind(dram.port);
    //
    image_width = sknobs_get_value("top.cpu.image_width", 5);
    image_height = sknobs_get_value("top.cpu.image_height", 5);
    image_depth = sknobs_get_value("top.cpu.image_depth", 1);
    filter_size = sknobs_get_value("top.cpu.filter_size", 3);
    filter_stride = sknobs_get_value("top.cpu.filter_stride", 1);
    filter_number = sknobs_get_value("top.cpu.filter_number", 1);
    //
    address = sknobs_get_value(format("%s.image_address", name()), 0);
    dram.initialize(address, sknobs_get_string(format("%s.image_file", name()), "image.txt"));
    //length = image_width*image_height*image_depth;
    //dram.dump(address, length);
    address = sknobs_get_value(format("%s.filter_address", name()), 0);
    dram.initialize(address, sknobs_get_string(format("%s.filter_file", name()), "filter.txt"));
    //length = filter_size*filter_size*image_depth*filter_number;
    //dram.dump(address, length);
}

void top::end_of_simulation ()
{
    unsigned int address = sknobs_get_value(format("%s.psum_address", name()), 0);
    length = ((image_width-filter_size)/filter_stride+1)*((image_width-filter_size)/filter_stride+1)*filter_number;
    dram.dump(address, length);
}
