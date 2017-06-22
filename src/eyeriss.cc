#include "eyeriss.h"

//@
eyeriss::eyeriss (const sc_module_name& nm, unsigned int hsize, unsigned int vsize)
    : sc_module(nm)
    , m_hsize(hsize)
    , m_vsize(vsize)
    , cpu_socket("cpu_socket")
    , dram_socket("dram_socket")
    , controller_0("controller")
    , dram_controller_0("dram_controller")
    , main_buffer("main_buffer", 32, 0x1B000)
    , sram_controller_0("sram_controller")
    , pe_array_0("pe_array", hsize, vsize)
{
    SC_REPORT_INFO_VERB(name(), "Building ...", SC_FULL);
    cpu_socket.bind(controller_0.cpu_socket);
    //
    controller_0.dram_socket.bind(dram_controller_0.control_socket);
    controller_0.sram_socket.bind(sram_controller_0.control_socket);
    //
    dram_controller_0.dram_socket.bind(dram_socket);
    dram_controller_0.sram_socket.bind(main_buffer.port);
    //
    sram_controller_0.isram_socket.bind(main_buffer.port);
    sram_controller_0.fsram_socket.bind(main_buffer.port);
    sram_controller_0.psram_socket.bind(main_buffer.port);
    sram_controller_0.ipe_socket.bind(pe_array_0.isocket);
    sram_controller_0.fpe_socket.bind(pe_array_0.fsocket);
    for (int i = 0; i < hsize; i++)
    {
        sram_controller_0.ippe_socket.bind(pe_array_0.psocket[i]);
    }
    //
    for (int i = 0; i < hsize; i++)
    {
        pe_array_0.opsocket[i].bind(sram_controller_0.oppe_socket);
    }
}
