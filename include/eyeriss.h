#ifndef __EYERISS_H__
#define __EYERISS_H__

#include "common.h"
#include "controller.h"
#include "dram_controller.h"
#include "ram.h"
#include "sram_controller.h"
#include "pe_array.h"

class eyeriss : public sc_module
{
public:
    tlm_target_socket<> cpu_socket;
    tlm_initiator_socket<> dram_socket;
    
protected:
    unsigned int m_hsize;
    unsigned int m_vsize;
    controller       controller_0     ;
    dram_controller  dram_controller_0;
    ram              main_buffer      ;
    sram_controller  sram_controller_0;
    pe_array         pe_array_0       ;
    
public:
    eyeriss (const sc_module_name& nm, unsigned int hsize, unsigned int vsize);
    
protected:
};

#endif /* __EYERISS_H__ */
