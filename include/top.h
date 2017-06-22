#ifndef __TOP_H__
#define __TOP_H__

#include "common.h"
#include "cpu_model.h"
#include "ram.h"
#include "eyeriss.h"

class top : public sc_module
{
protected:
    unsigned int m_hsize = sknobs_get_value("eyeriss.hsize", 0);
    unsigned int m_vsize = sknobs_get_value("eyeriss.vsize", 0);
    unsigned int m_ramsize = sknobs_get_value(format("%s.ramsize", name()), 0x100000);
    cpu_model   cpu;
    ram         dram;
    eyeriss     eyeriss_0;
    int image_width;
    int image_height;
    int image_depth;
    int filter_size;
    int filter_stride;
    int filter_number;
    int length;

public:
    top(const sc_module_name& nm);
    void end_of_simulation ();
};

#endif /* __TOP_H__ */
